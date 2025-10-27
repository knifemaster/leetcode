#ifndef WEB_SEARCH_WORKER_H
#define WEB_SEARCH_WORKER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

struct SearchResult {
    QString title;
    QString imageUrl;
    QString sourceLink;
    
    bool isValid() const {
        return !imageUrl.isEmpty() && QUrl(imageUrl).isValid();
    }
};

class WebSearchWorker : public QObject {
    Q_OBJECT
public:
    explicit WebSearchWorker(const QString& query, QObject* parent = nullptr);
    ~WebSearchWorker();

    // Установка ключей API
    void setApiKeys(const QString& apiKey, const QString& cseId);

public slots:
    void doSearch();
    void cancelSearch();

signals:
    void searchFinished(const QList<SearchResult>& results);
    void searchError(const QString& error);
    void searchProgress(const QString& message);

private slots:
    void onReplyFinished();
    void onReplyError(QNetworkReply::NetworkError error);
    void onTimeout();

private:
    QUrl buildSearchUrl() const;
    QString urlEncodeQuery(const QString& query) const;
    bool validateApiKeys() const;
    QList<SearchResult> parseSearchResults(const QByteArray& response) const;
    void cleanup();
    
    QString m_query;
    QString m_api_key;
    QString m_cse_id;
    
    QNetworkAccessManager* m_networkManager;
    QNetworkReply* m_currentReply;
    QTimer* m_timeoutTimer;
    
    bool m_isSearching;
    
    static const int MAX_RESULTS = 10;
    static const int REQUEST_TIMEOUT_MS = 30000;
};

#endif // WEB_SEARCH_WORKER_H