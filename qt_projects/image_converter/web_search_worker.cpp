#include "web_search_worker.h"
#include <nlohmann/json.hpp>
#include <QDebug>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QSslConfiguration>

using json = nlohmann::json;

WebSearchWorker::WebSearchWorker(const QString& query, QObject* parent)
    : QObject(parent), 
      m_query(query),
      m_networkManager(nullptr),
      m_currentReply(nullptr),
      m_timeoutTimer(nullptr),
      m_isSearching(false)
{
    m_networkManager = new QNetworkAccessManager(this);
    m_timeoutTimer = new QTimer(this);
    m_timeoutTimer->setSingleShot(true);
    
    connect(m_timeoutTimer, &QTimer::timeout,
            this, &WebSearchWorker::onTimeout);
}

WebSearchWorker::~WebSearchWorker() {
    qDebug() << "WebSearchWorker destructor called";
    cancelSearch();
}

// ДОБАВЛЕНО: реализация метода setApiKeys
void WebSearchWorker::setApiKeys(const QString& apiKey, const QString& cseId) {
    m_api_key = apiKey;
    m_cse_id = cseId;
    qDebug() << "API keys set successfully";
}

void WebSearchWorker::cancelSearch() {
    if (m_isSearching) {
        qDebug() << "Cancelling search...";
        
        if (m_currentReply) {
            m_currentReply->disconnect(); // Отключаем все соединения
            m_currentReply->abort();
            m_currentReply->deleteLater();
            m_currentReply = nullptr;
        }
        
        m_timeoutTimer->stop();
        m_isSearching = false;
    }
}

QString WebSearchWorker::urlEncodeQuery(const QString& query) const {
    return QUrl::toPercentEncoding(query);
}

QUrl WebSearchWorker::buildSearchUrl() const {
    QString encoded_query = urlEncodeQuery(m_query);
    
    QUrl url("https://www.googleapis.com/customsearch/v1");
    QUrlQuery query;
    query.addQueryItem("key", m_api_key);
    query.addQueryItem("cx", m_cse_id);
    query.addQueryItem("q", encoded_query);
    query.addQueryItem("searchType", "image");
    query.addQueryItem("num", QString::number(MAX_RESULTS));
    
    url.setQuery(query);
    return url;
}

bool WebSearchWorker::validateApiKeys() const {
    if (m_api_key.isEmpty() || m_cse_id.isEmpty()) {
        qWarning() << "API keys are not set";
        return false;
    }
    
    if (m_api_key.length() < 20) {
        qWarning() << "API key appears to be invalid (too short)";
        return false;
    }
    
    return true;
}

QList<SearchResult> WebSearchWorker::parseSearchResults(const QByteArray& response) const {
    QList<SearchResult> results;
    
    try {
        json j = json::parse(response.constData(), response.constData() + response.size());
        
        if (!j.contains("items")) {
            qWarning() << "No 'items' field in API response";
            if (j.contains("error")) {
                QString errorMsg = QString::fromStdString(
                    j["error"].value("message", "Unknown API error"));
                qWarning() << "API Error:" << errorMsg;
            }
            return results;
        }
        
        for (const auto& item : j["items"]) {
            SearchResult result;
            
            if (item.contains("link")) {
                result.imageUrl = QString::fromStdString(item.value("link", ""));
                
                QUrl imageUrl(result.imageUrl);
                if (!imageUrl.isValid() || imageUrl.scheme().isEmpty()) {
                    qWarning() << "Invalid image URL:" << result.imageUrl;
                    continue;
                }
            } else {
                qWarning() << "Item missing 'link' field";
                continue;
            }
            
            result.title = QString::fromStdString(item.value("title", "No Title"))
                          .left(255).trimmed();
            
            if (result.title.isEmpty()) {
                result.title = "Untitled";
            }
            
            if (item.contains("image") && item["image"].contains("contextLink")) {
                result.sourceLink = QString::fromStdString(
                    item["image"].value("contextLink", ""));
            } else if (item.contains("displayLink")) {
                QString displayLink = QString::fromStdString(
                    item.value("displayLink", ""));
                if (!displayLink.isEmpty()) {
                    result.sourceLink = "https://" + displayLink;
                }
            }
            
            if (result.sourceLink.isEmpty()) {
                result.sourceLink = "No source available";
            }
            
            if (result.isValid()) {
                results.append(result);
            }
        }
        
        qDebug() << "Parsed" << results.size() << "valid results from API response";
        
    } catch (const json::exception& e) {
        qCritical() << "JSON parsing error:" << e.what();
        throw;
    }
    
    return results;
}

void WebSearchWorker::cleanup() {
    if (m_currentReply) {
        m_currentReply->disconnect();
        m_currentReply->deleteLater();
        m_currentReply = nullptr;
    }
    m_timeoutTimer->stop();
    m_isSearching = false;
}

void WebSearchWorker::doSearch() {
    if (m_isSearching) {
        qWarning() << "Search already in progress, cancelling previous...";
        cancelSearch();
    }
    
    if (!validateApiKeys()) {
        emit searchError("API Keys are not properly configured.");
        return;
    }
    
    m_isSearching = true;
    
    emit searchProgress("Building search request...");
    
    QUrl searchUrl = buildSearchUrl();
    
    if (!searchUrl.isValid()) {
        emit searchError("Invalid search URL constructed.");
        m_isSearching = false;
        return;
    }
    
    qDebug() << "Starting search for:" << m_query;
    qDebug() << "Search URL:" << searchUrl.toString(QUrl::RemoveUserInfo);
    
    QNetworkRequest request(searchUrl);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Qt-WebSearch-Client/1.0");
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setProtocol(QSsl::TlsV1_2OrLater);
    request.setSslConfiguration(sslConfig);
    
    emit searchProgress("Sending request to Google API...");
    
    // Создаем новый запрос
    m_currentReply = m_networkManager->get(request);
    
    // Подключаем обработчики ТОЛЬКО для текущего запроса
    connect(m_currentReply, &QNetworkReply::finished,
            this, &WebSearchWorker::onReplyFinished);
    connect(m_currentReply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::errorOccurred),
            this, &WebSearchWorker::onReplyError);
    
    m_timeoutTimer->start(REQUEST_TIMEOUT_MS);
    
    emit searchProgress("Waiting for API response...");
}

void WebSearchWorker::onReplyFinished() {
    if (!m_isSearching) {
        qDebug() << "Search was cancelled, ignoring finished signal";
        return;
    }
    
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) {
        qCritical() << "Invalid reply in onReplyFinished";
        cleanup();
        return;
    }
    
    m_timeoutTimer->stop();
    
    emit searchProgress("Processing API response...");
    
    // Проверяем, что это наш текущий запрос
    if (reply != m_currentReply) {
        qWarning() << "Received finished signal from unknown reply";
        reply->deleteLater();
        cleanup();
        return;
    }
    
    if (reply->error() != QNetworkReply::NoError) {
        // Ошибка уже обработана в onReplyError
        reply->deleteLater();
        cleanup();
        return;
    }
    
    QByteArray responseData = reply->readAll();
    reply->deleteLater();
    m_currentReply = nullptr;
    
    if (responseData.isEmpty()) {
        emit searchError("Empty response from server.");
        cleanup();
        return;
    }
    
    qDebug() << "Received response size:" << responseData.size() << "bytes";
    
    try {
        QList<SearchResult> results = parseSearchResults(responseData);
        
        if (results.isEmpty()) {
            emit searchError("No valid image results found for your search.");
        } else {
            qDebug() << "Search completed successfully. Found" << results.size() << "results.";
            emit searchFinished(results);
        }
        
    } catch (const json::exception& e) {
        QString errorMsg = QString("Failed to parse API response: %1").arg(e.what());
        qCritical() << errorMsg;
        emit searchError(errorMsg);
    } catch (const std::exception& e) {
        QString errorMsg = QString("Search error: %1").arg(e.what());
        qCritical() << errorMsg;
        emit searchError(errorMsg);
    }
    
    cleanup();
}

void WebSearchWorker::onReplyError(QNetworkReply::NetworkError error) {
    if (!m_isSearching) {
        return; // Поиск уже отменен
    }
    
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;
    
    m_timeoutTimer->stop();
    
    QString errorString = reply->errorString();
    QString detailedError;
    
    switch (error) {
    case QNetworkReply::OperationCanceledError:
        qDebug() << "Search was cancelled";
        return; // Не отправляем сигнал ошибки при отмене
    case QNetworkReply::TimeoutError:
        detailedError = "Request timeout - server is not responding.";
        break;
    case QNetworkReply::ConnectionRefusedError:
        detailedError = "Connection refused - server may be down.";
        break;
    case QNetworkReply::HostNotFoundError:
        detailedError = "Host not found - check your internet connection.";
        break;
    case QNetworkReply::SslHandshakeFailedError:
        detailedError = "SSL handshake failed - security issue.";
        break;
    case QNetworkReply::ContentAccessDenied:
        detailedError = "Access denied - check your API keys.";
        break;
    default:
        detailedError = errorString;
        break;
    }
    
    qCritical() << "Network error:" << error << "-" << detailedError;
    emit searchError(QString("Network Error: %1").arg(detailedError));
    
    cleanup();
}

void WebSearchWorker::onTimeout() {
    if (m_isSearching && m_currentReply) {
        qCritical() << "Request timeout after" << REQUEST_TIMEOUT_MS << "ms";
        emit searchError("Request timeout - server is taking too long to respond.");
        cancelSearch();
    }
}