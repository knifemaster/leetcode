#include "mainwindow.h"
#include "settings_dialog.h" 
#include "ocr_processor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QSplitter>
#include <QFileInfo>
#include <QDir>
#include <QSet>
#include <QList>
#include <QListWidgetItem>
#include <QRegularExpression> 
#include <QUrlQuery>          
#include <QStandardPaths>     
#include <QImage>             
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>           
#include <algorithm>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp> 
#include <QPixmap>
#include <QScrollBar> 
#include <QDesktopServices> 
#include <QApplication>
#include <QThread>
#include <QTextEdit> 
#include <QInputDialog>
#include <QToolBar>
#include <QDebug>

// --- Вспомогательная функция для конвертации cv::Mat в QPixmap ---
QPixmap matToPixmap(const cv::Mat& mat) {
    if (mat.empty()) return QPixmap();

    cv::Mat temp;
    if (mat.channels() == 1) {
        cv::cvtColor(mat, temp, cv::COLOR_GRAY2BGR);
    } else if (mat.channels() == 3) {
        temp = mat;
    } else {
        return QPixmap();
    }

    cv::cvtColor(temp, temp, cv::COLOR_BGR2RGB);

    QImage image(temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
    return QPixmap::fromImage(image);
}
// -----------------------------------------------------------------

MainWindow::~MainWindow() {
    cleanupSearchWorker();
    
    if (netManager) {
        netManager->clearAccessCache();
        disconnect(netManager, nullptr, nullptr, nullptr);
    }
    
    QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QDir dir(tempDir);
    
    QStringList filters;
    filters << "web_img_*.jpg" << "web_img_*.jpeg" << "web_img_*.png"; 
    
    for (const QString& fileName : dir.entryList(filters, QDir::Files)) {
        dir.remove(fileName);
    }
    
    if (videoCapture.isOpened()) {
        videoCapture.release();
    }
}

void MainWindow::cleanupSearchWorker() {
    if (m_searchWorker) {
        m_searchWorker->cancelSearch();
        m_searchWorker->disconnect();
        m_searchWorker->deleteLater();
        m_searchWorker = nullptr;
    }
    
    if (m_searchThread) {
        if (m_searchThread->isRunning()) {
            m_searchThread->quit();
            m_searchThread->wait(500);
        }
        m_searchThread->deleteLater();
        m_searchThread = nullptr;
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      imageLabel(new QLabel(this)),
      leftTabWidget(new QTabWidget(this)),
      filterBox(new QComboBox(this)),
      brightnessSlider(new QSlider(Qt::Horizontal, this)),
      contrastSlider(new QSlider(Qt::Horizontal, this)),
      hueSlider(new QSlider(Qt::Horizontal, this)),
      saturationSlider(new QSlider(Qt::Horizontal, this)),
      blurSlider(new QSlider(Qt::Horizontal, this)),
      sharpenStrengthSlider(new QSlider(Qt::Horizontal, this)),
      thresholdValue(new QSlider(Qt::Horizontal, this)),
      runButton(new QPushButton("Запустить YOLO (F5)", this)),
      //nextButton(new QPushButton(">", this)),
      //prevButton(new QPushButton("<", this)),
      searchBox(new QLineEdit(this)),
      fileList(new QListWidget(this)),
      searchQueryInput(new QLineEdit(this)),
      searchWebButton(new QPushButton("Поиск в Web (GCS)", this)), 
      recognizeOcrButton(new QPushButton("Распознать текст", this)),
      ocrLanguageBox(new QComboBox(this)),
      ocrResultTextEdit(new QTextEdit(this)), 
      netManager(new QNetworkAccessManager(this)),
      activeDownloads(0),
      loadVideoAction(nullptr),
      videoSlider(nullptr),
      thumbnailPreviewLabel(nullptr),
      thumbnailScrollArea(nullptr), 
      thumbnailContainer(nullptr),
      thumbnailLayout(nullptr),
      m_searchThread(nullptr),
      m_searchWorker(nullptr),
      fileSystemModel(new QFileSystemModel(this)), 
      fileTree(new QTreeView(this)),
      toolbar(nullptr),
      m_currentDestPath(QDir::currentPath())
{
    yolo.loadModel("../yolov3.cfg", "../yolov3.weights", "../coco.names");

    setupUI();
    createActions();
    createConnections();

    setWindowTitle("QtYoloApp");
    resize(1200, 800);
}

void MainWindow::setupUI() {
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, this);
    
    // 1. НАСТРОЙКА ЦЕНТРАЛЬНОЙ ОБЛАСТИ (Изображение + Слайдер)
    videoSlider = new QSlider(Qt::Horizontal, this);
    videoSlider->setRange(0, 0); 
    videoSlider->setEnabled(false);
    
    thumbnailPreviewLabel = new QLabel(this);
    thumbnailPreviewLabel->setFixedSize(160, 90); 
    thumbnailPreviewLabel->setScaledContents(true);
    thumbnailPreviewLabel->setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint); 
    thumbnailPreviewLabel->hide();
    videoSlider->installEventFilter(this);

    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true); 
    
    QWidget *centralImageContainer = new QWidget;
    QVBoxLayout *centralLayout = new QVBoxLayout(centralImageContainer);
    centralLayout->addWidget(imageLabel);
    centralLayout->addWidget(videoSlider); 
    centralLayout->setContentsMargins(0, 0, 0, 0);
    
    // 2. НАСТРОЙКА ТАБ-ВИДЖЕТА (СЛЕВА)
    leftTabWidget->setMinimumWidth(300); 
    
    QWidget *toolsTab = new QWidget(leftTabWidget);
    QWidget *transformsTab = new QWidget(leftTabWidget);
    QWidget *selectionTab = new QWidget(leftTabWidget);
    QWidget *recognitionTab = new QWidget(leftTabWidget);
    
    QVBoxLayout *toolsLayout = new QVBoxLayout(toolsTab);
    QVBoxLayout *transformsLayout = new QVBoxLayout(transformsTab);
    QVBoxLayout *selectionLayout = new QVBoxLayout(selectionTab);
    QVBoxLayout *recognitionLayout = new QVBoxLayout(recognitionTab);

    toolsLayout->setContentsMargins(10, 10, 10, 10);
    transformsLayout->setContentsMargins(10, 10, 10, 10);
    selectionLayout->setContentsMargins(10, 10, 10, 10);
    recognitionLayout->setContentsMargins(10, 10, 10, 10);

    // --- Блок 1: Инструменты (Навигация, YOLO, Список файлов) ---
    QHBoxLayout *navLayout = new QHBoxLayout;
    // --- Блок 4: Распознавание (Web-поиск и OCR) ---
    toolsLayout->addWidget(new QLabel("--- Web Search (GCS) ---"));
    toolsLayout->addWidget(new QLabel("Search Query:"));
    toolsLayout->addWidget(searchQueryInput);
    toolsLayout->addWidget(searchWebButton);
    toolsLayout->addSpacing(20);

    //navLayout->addWidget(prevButton);
    //navLayout->addSpacing(5);
    //navLayout->addWidget(nextButton);

    toolsLayout->addWidget(new QLabel("📂 **File Tree (Local)**:"));

    fileSystemModel->setRootPath(QDir::currentPath());
    fileSystemModel->setReadOnly(false);
    fileTree->setModel(fileSystemModel);
    fileTree->setRootIndex(fileSystemModel->index(QDir::currentPath()));
    for (int i = 1; i < fileSystemModel->columnCount(); ++i) {
        fileTree->hideColumn(i);
    }
    
    fileTree->setHeaderHidden(false);
    fileTree->setContextMenuPolicy(Qt::CustomContextMenu);
    fileTree->setFixedHeight(250);
    fileTree->setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::SelectedClicked);

    toolsLayout->addWidget(fileTree);
    toolsLayout->addSpacing(15);

    toolsLayout->addLayout(navLayout);
    toolsLayout->addSpacing(10);
    toolsLayout->addWidget(runButton);
    toolsLayout->addSpacing(15);
    toolsLayout->addWidget(new QLabel("Local Files / Web Downloads:"));
    toolsLayout->addWidget(fileList);
    toolsLayout->addStretch(1);

    
    // --- Блок 2: Преобразования (Фильтры и Слайдеры) ---
    transformsLayout->setSpacing(8);
    
    filterBox->addItem("None");
    filterBox->addItem("Blur");
    filterBox->addItem("Edge");
    filterBox->addItem("HSV");
    filterBox->addItem("Gaussian blur");
    filterBox->addItem("Sharpen");
    filterBox->addItem("Threshold");
    filterBox->addItem("Grayscale");

    brightnessSlider->setRange(-100, 100);
    contrastSlider->setRange(-100, 100);
    hueSlider->setRange(-180, 180);
    saturationSlider->setRange(-100, 100);
    blurSlider->setRange(0, 10);
    sharpenStrengthSlider->setRange(0, 100); 
    thresholdValue->setRange(0, 255);        
    
    transformsLayout->addWidget(new QLabel("Filter:"));
    transformsLayout->addWidget(filterBox);
    transformsLayout->addSpacing(5);
    
    transformsLayout->addWidget(new QLabel("Brightness:"));
    transformsLayout->addWidget(brightnessSlider);
    transformsLayout->addWidget(new QLabel("Contrast:"));
    transformsLayout->addWidget(contrastSlider);
    transformsLayout->addWidget(new QLabel("Hue:"));
    transformsLayout->addWidget(hueSlider);
    transformsLayout->addWidget(new QLabel("Saturation:"));
    transformsLayout->addWidget(saturationSlider);
    transformsLayout->addWidget(new QLabel("Blur (Radius):"));
    transformsLayout->addWidget(blurSlider);
    transformsLayout->addWidget(new QLabel("Sharpen Strength (0-100):"));
    transformsLayout->addWidget(sharpenStrengthSlider);
    transformsLayout->addWidget(new QLabel("Threshold Value (0-255):"));
    transformsLayout->addWidget(thresholdValue);
    transformsLayout->addStretch(1);

    // --- Блок 3: Выделение (Локальный поиск и Миниатюры) ---
    thumbnailContainer = new QWidget(this);
    thumbnailLayout = new QVBoxLayout(thumbnailContainer);// --- Блок 4: Распознавание (Web-поиск и OCR) ---
    //recognitionLayout->addWidget(new QLabel("--- Web Search (GCS) ---"));
    //recognitionLayout->addWidget(new QLabel("Search Query:"));
    //recognitionLayout->addWidget(searchQueryInput);
    //recognitionLayout->addWidget(searchWebButton);
    //recognitionLayout->addSpacing(20);

    thumbnailLayout->setAlignment(Qt::AlignTop);
    thumbnailLayout->setContentsMargins(0, 0, 0, 0);
    thumbnailLayout->setSpacing(5); 
    thumbnailContainer->setLayout(thumbnailLayout);

    thumbnailScrollArea = new QScrollArea(this); 
    thumbnailScrollArea->setWidgetResizable(true);
    thumbnailScrollArea->setWidget(thumbnailContainer);
    thumbnailScrollArea->setFixedHeight(200); 
    
    selectionLayout->addWidget(new QLabel("Thumbnail Area (Video/Sequence):"));
    selectionLayout->addWidget(thumbnailScrollArea);
    selectionLayout->addSpacing(10);
    selectionLayout->addWidget(new QLabel("Local Search (Filter by Class):"));
    selectionLayout->addWidget(searchBox);
    selectionLayout->addStretch(1);


    recognitionLayout->addWidget(new QLabel("--- OCR Recognition ---"));
    
    ocrLanguageBox->addItem("English (eng)", "eng");
    ocrLanguageBox->addItem("Russian (rus)", "rus");
    
    recognitionLayout->addWidget(new QLabel("Recognition Language:"));
    recognitionLayout->addWidget(ocrLanguageBox);
    recognitionLayout->addWidget(recognizeOcrButton);
    recognitionLayout->addSpacing(10);
    recognitionLayout->addWidget(new QLabel("Recognized Text:"));
    ocrResultTextEdit->setReadOnly(true);
    ocrResultTextEdit->setFixedHeight(200); 
    recognitionLayout->addWidget(ocrResultTextEdit);
    
    recognitionLayout->addStretch(1);

    // --- Добавление вкладок ---
    leftTabWidget->addTab(toolsTab, "Инструменты");
    leftTabWidget->addTab(transformsTab, "Преобразования");
    leftTabWidget->addTab(selectionTab, "Выделение");
    leftTabWidget->addTab(recognitionTab, "Распознавание");
    
    // 3. СБОРКА ГЛАВНОГО СПЛИТТЕРА
    mainSplitter->addWidget(leftTabWidget); 
    mainSplitter->addWidget(centralImageContainer);
    
    mainSplitter->setStretchFactor(0, 1); 
    mainSplitter->setStretchFactor(1, 3); 
    
    setCentralWidget(mainSplitter);

    // === СОЗДАНИЕ ТУЛБАРА ДЛЯ ФАЙЛОВОГО ДЕРЕВА ===
    toolbar = this->addToolBar("File Operations");
    toolbar->setMovable(false);

    // Action: Create New Folder
    QAction *newFolderAction = new QAction(QIcon::fromTheme("folder-new"), "New Folder", this);
    newFolderAction->setShortcut(QKeySequence("Ctrl+N"));
    toolbar->addAction(newFolderAction);

    // Action: Delete
    QAction *deleteAction = new QAction(QIcon::fromTheme("edit-delete"), "Delete", this);
    deleteAction->setShortcut(QKeySequence::Delete);
    toolbar->addAction(deleteAction);

    toolbar->addSeparator();

    // Action: Refresh
    QAction *refreshAction = new QAction(QIcon::fromTheme("view-refresh"), "Refresh", this);
    refreshAction->setShortcut(QKeySequence::Refresh);
    toolbar->addAction(refreshAction);

    // Action: Rename
    QAction *renameAction = new QAction(QIcon::fromTheme("edit-rename"), "Rename", this);
    renameAction->setShortcut(QKeySequence("F2"));
    toolbar->addAction(renameAction);

    fileList->setContextMenuPolicy(Qt::CustomContextMenu);


    // Подключаем сигналы тулбара
    connect(newFolderAction, &QAction::triggered, this, &MainWindow::createNewFolder);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteSelectedFile);
    connect(refreshAction, &QAction::triggered, this, &MainWindow::refreshFileTree);
    connect(renameAction, &QAction::triggered, this, &MainWindow::renameSelectedFile);
}

void MainWindow::createActions() {
    QMenu *fileMenu = menuBar()->addMenu("Файл");
    QAction *loadImageAction = new QAction("Загрузить изображение...", this);
    QAction *loadFolderAction = new QAction("Загрузить папку...", this);
    QAction *saveImageAction = new QAction("Сохранить изображение...", this); 
    QAction *settingsAction = new QAction("YOLO Settings...", this);
    QAction *selectDirAction = new QAction("Select Working Directory...", this);

    loadVideoAction = new QAction(tr("Load &Video..."), this);

    fileMenu->addAction(loadImageAction);
    fileMenu->addAction(loadFolderAction);
    fileMenu->addAction(selectDirAction);
    fileMenu->addAction(loadVideoAction); 
    fileMenu->addAction(saveImageAction);
    fileMenu->addSeparator(); 
    menuBar()->addAction(settingsAction);

    connect(loadImageAction, &QAction::triggered, this, &MainWindow::loadImage);
    connect(loadFolderAction, &QAction::triggered, this, &MainWindow::loadFolder);
    connect(saveImageAction, &QAction::triggered, this, &MainWindow::saveImage); 
    connect(settingsAction, &QAction::triggered, this, &MainWindow::openSettingsDialog);
    connect(selectDirAction, &QAction::triggered, this, &MainWindow::selectWorkingDirectory);
}

void MainWindow::createConnections() {
    //connect(nextButton, &QPushButton::clicked, this, &MainWindow::showNextImage);
    //connect(prevButton, &QPushButton::clicked, this, &MainWindow::showPrevImage);
    connect(runButton, &QPushButton::clicked, this, &MainWindow::runDetection);
    connect(filterBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateFilter);
    connect(brightnessSlider, &QSlider::valueChanged, this, &MainWindow::updateParams);
    connect(contrastSlider, &QSlider::valueChanged, this, &MainWindow::updateParams);
    connect(hueSlider, &QSlider::valueChanged, this, &MainWindow::updateParams);
    connect(saturationSlider, &QSlider::valueChanged, this, &MainWindow::updateParams);
    connect(blurSlider, &QSlider::valueChanged, this, &MainWindow::updateParams);
    connect(searchBox, &QLineEdit::returnPressed, this, &MainWindow::handleSearch);
    connect(fileList, &QListWidget::itemClicked, this, &MainWindow::listItemClicked);
    connect(sharpenStrengthSlider, &QSlider::valueChanged, this, &MainWindow::updateParams);
    connect(thresholdValue, &QSlider::valueChanged, this, &MainWindow::updateParams);
    connect(searchWebButton, &QPushButton::clicked, this, &MainWindow::startImageSearch);
    
    connect(loadVideoAction, &QAction::triggered, this, &MainWindow::loadVideo); 
    connect(videoSlider, &QSlider::sliderMoved, this, &MainWindow::videoSliderMoved); 
    
    connect(recognizeOcrButton, &QPushButton::clicked, this, &MainWindow::startOcrRecognition);

    connect(fileTree, &QTreeView::customContextMenuRequested, this, &MainWindow::showContextMenu);
    connect(fileTree, &QTreeView::clicked, this, &MainWindow::handleFileTreeClick);

    connect(fileList, &QListWidget::customContextMenuRequested, this, &MainWindow::showFileListContextMenu);
}

// ----------------------------------------------------------------------
// --- МЕТОДЫ ДЛЯ РАБОТЫ С ВИДЕО (Функциональные) ---
// ----------------------------------------------------------------------

void MainWindow::loadVideo() {
    QString videoFile = QFileDialog::getOpenFileName(this, tr("Load Video"), "", tr("Video Files (*.mp4 *.avi *.mkv *.mov *.webm)"));

    if (videoFile.isEmpty()) return;

    if (videoCapture.isOpened()) videoCapture.release();
    
    fileList->clear();
    imageFiles.clear();
    currentIndex = -1;
    
    videoCapture.open(videoFile.toStdString());
    
    if (videoCapture.isOpened()) {
        totalFrames = static_cast<int>(videoCapture.get(cv::CAP_PROP_FRAME_COUNT));
        
        videoSlider->setRange(0, totalFrames > 0 ? totalFrames - 1 : 0);
        videoSlider->setValue(0);
        videoSlider->setEnabled(totalFrames > 1);
        
        videoSliderMoved(0); 
        
        setWindowTitle(QString("QtYoloApp - %1").arg(QFileInfo(videoFile).fileName()));
    } else {
        QMessageBox::critical(this, tr("Error"), tr("Could not open video file."));
        videoSlider->setRange(0, 0);
        videoSlider->setEnabled(false);
        totalFrames = 0;
        imageLabel->setText(tr("Video loading failed."));
    }
}

void MainWindow::videoSliderMoved(int frameNumber) {
    if (!videoCapture.isOpened() || frameNumber < 0 || frameNumber >= totalFrames) {
        return;
    }

    if (currentFrame != frameNumber) {
        videoCapture.set(cv::CAP_PROP_POS_FRAMES, frameNumber);
        
        cv::Mat frame;
        if (videoCapture.read(frame) && !frame.empty()) {
            currentFrame = frameNumber;
            currentImage = frame.clone(); 
            
            cv::Mat processedImage = yolo.applyFilter(currentImage, currentFilter, params);
            QPixmap pixmap = matToPixmap(processedImage);
            
            imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            
            videoSlider->setValue(currentFrame);
        } else {
            qWarning() << "Failed to read frame at index" << frameNumber;
        }
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == videoSlider && videoCapture.isOpened()) {
        
        if (event->type() == QEvent::MouseMove) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            
            int x = mouseEvent->pos().x();
            int sliderWidth = videoSlider->width();
            int sliderMin = videoSlider->minimum();
            int sliderMax = videoSlider->maximum();
            
            int frameNumber = sliderMin + (static_cast<double>(x) / sliderWidth) * (sliderMax - sliderMin);
            frameNumber = qBound(sliderMin, frameNumber, sliderMax); 
            
            QPoint globalPos = videoSlider->mapToGlobal(mouseEvent->pos());
            
            updateThumbnailPreview(frameNumber, globalPos);
            
            return false; 
        } else if (event->type() == QEvent::Leave || event->type() == QEvent::MouseButtonPress) {
            hideThumbnailPreview();
            return false;
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::updateThumbnailPreview(int frameNumber, const QPoint& globalPos) {
    static int lastPreviewFrame = -1;
    cv::Mat thumbnailFrame;
    
    if (frameNumber != lastPreviewFrame) {
        double originalPos = videoCapture.get(cv::CAP_PROP_POS_FRAMES);
        
        videoCapture.set(cv::CAP_PROP_POS_FRAMES, frameNumber);
        
        if (videoCapture.read(thumbnailFrame) && !thumbnailFrame.empty()) {
            
            cv::resize(thumbnailFrame, thumbnailFrame, cv::Size(160, 90));
            
            QPixmap pixmap = matToPixmap(thumbnailFrame);
            thumbnailPreviewLabel->setPixmap(pixmap);
            
            lastPreviewFrame = frameNumber;
        } else {
            thumbnailPreviewLabel->clear();
            hideThumbnailPreview();
            lastPreviewFrame = -1;
            videoCapture.set(cv::CAP_PROP_POS_FRAMES, originalPos);
            return; 
        }
        
        videoCapture.set(cv::CAP_PROP_POS_FRAMES, originalPos);
    }
    
    int x = globalPos.x() - thumbnailPreviewLabel->width() / 2;
    int y = globalPos.y() - thumbnailPreviewLabel->height() - 10; 
    thumbnailPreviewLabel->move(x, y);
    
    if (thumbnailPreviewLabel->isHidden()) {
        thumbnailPreviewLabel->show();
    }
}

void MainWindow::hideThumbnailPreview() {
    thumbnailPreviewLabel->hide();
}

// ----------------------------------------------------------------------
// --- МЕТОДЫ ОБРАБОТКИ ИЗОБРАЖЕНИЙ И ФАЙЛОВ ---
// ----------------------------------------------------------------------

void MainWindow::loadImageByIndex(int index) {
    if (index < 0 || index >= imageFiles.size()) return;

    QString filePath = imageFiles.at(index);
    cv::Mat img = cv::imread(filePath.toStdString());

    if (!img.empty()) {
        currentImage = img.clone();
        currentIndex = index;

        if (videoCapture.isOpened()) videoCapture.release();
        videoSlider->setRange(0, 0);
        videoSlider->setEnabled(false);
        currentFrame = -1;
        totalFrames = 0;

        applyCurrentFilter();
        setWindowTitle(QString("QtYoloApp - %1").arg(QFileInfo(filePath).fileName()));
        fileList->setCurrentRow(index); 
    } else {
        QMessageBox::warning(this, "Load Error", "Could not load image file: " + filePath);
    }
}

void MainWindow::loadImage() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Загрузить изображение"), "", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
    if (filePath.isEmpty()) return;
    
    imageFiles.clear();
    fileList->clear();
    imageFiles.append(filePath);
    fileList->addItem(QFileInfo(filePath).fileName());
    loadImageByIndex(0);
}

void MainWindow::loadFolder() {
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("Загрузить папку"));
    if (dirPath.isEmpty()) return;

    QDir dir(dirPath);
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp";
    
    QStringList files = dir.entryList(filters, QDir::Files | QDir::NoDotAndDotDot);
    
    imageFiles.clear();
    fileList->clear();
    
    for (const QString& file : files) {
        imageFiles.append(dirPath + QDir::separator() + file);
        fileList->addItem(file);
    }
    
    if (!imageFiles.isEmpty()) {
        loadImageByIndex(0);
    } else {
        QMessageBox::information(this, "Load Info", "No image files found in the selected folder.");
    }
}

void MainWindow::showNextImage() {
    if (videoCapture.isOpened()) {
        videoSliderMoved(currentFrame + 1);
        return;
    }
    if (imageFiles.isEmpty()) return;
    int nextIndex = (currentIndex + 1) % imageFiles.size();
    loadImageByIndex(nextIndex);
}

void MainWindow::showPrevImage() {
    if (videoCapture.isOpened()) {
        videoSliderMoved(currentFrame - 1);
        return;
    }
    if (imageFiles.isEmpty()) return;
    int prevIndex = (currentIndex - 1 + imageFiles.size()) % imageFiles.size();
    loadImageByIndex(prevIndex);
}

void MainWindow::runDetection() {
    if (currentImage.empty()) return;
    
    cv::Mat detected = yolo.detectObjects(currentImage);
    displayedImage = detected.clone();
    
    QPixmap pixmap = matToPixmap(displayedImage);
    imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::updateFilter() {
    QString filterName = filterBox->currentText();
    if (filterName == "None") currentFilter = ImageFilter::None;
    else if (filterName == "Blur") currentFilter = ImageFilter::Blur;
    else if (filterName == "Edge") currentFilter = ImageFilter::Edge;
    else if (filterName == "HSV") currentFilter = ImageFilter::HSV;
    else if (filterName == "Gaussian blur") currentFilter = ImageFilter::GAUSSIAN_BLUR;
    else if (filterName == "Sharpen") currentFilter = ImageFilter::Sharpen;
    else if (filterName == "Threshold") currentFilter = ImageFilter::Threshold;
    else if (filterName == "Grayscale") currentFilter = ImageFilter::Grayscale;
    
    applyCurrentFilter();
}

void MainWindow::updateParams() {
    params.brightness = brightnessSlider->value();
    params.contrast = contrastSlider->value();
    params.hue = hueSlider->value();
    params.saturation = saturationSlider->value();
    params.blur = blurSlider->value();
    params.sharpen_strength = sharpenStrengthSlider->value();
    params.threshold_value = thresholdValue->value();

    applyCurrentFilter();
}

void MainWindow::applyCurrentFilter() {
    if (currentImage.empty()) return;

    cv::Mat filtered = yolo.applyFilter(currentImage, currentFilter, params);
    displayedImage = filtered.clone();

    QPixmap pixmap = matToPixmap(displayedImage);
    imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::saveImage() {
    if (displayedImage.empty()) {
        QMessageBox::warning(this, "Save Error", "No image to save.");
        return;
    }

    QString savePath = QFileDialog::getSaveFileName(this, tr("Сохранить изображение"), 
                                                    "", 
                                                    tr("Image Files (*.png *.jpg)"));
    
    if (savePath.isEmpty()) return;

    if (cv::imwrite(savePath.toStdString(), displayedImage)) {
        QMessageBox::information(this, "Save Success", "Image successfully saved to " + savePath);
    } else {
        QMessageBox::critical(this, "Save Error", "Failed to save image.");
    }
}

void MainWindow::handleSearch() {
    QString query = searchBox->text().trimmed().toLower();
    if (query.isEmpty()) {
        fileList->clear();
        for (const QString& file : imageFiles) {
             fileList->addItem(QFileInfo(file).fileName());
        }
        return;
    }

    QSet<int> matchingIndices;
    
    for (int i = 0; i < imageFiles.size(); ++i) {
        cv::Mat img = cv::imread(imageFiles.at(i).toStdString());
        if (!img.empty()) {
            std::vector<std::string> detectedClasses = yolo.getDetectedClasses(img);
            for (const std::string& className : detectedClasses) {
                if (QString::fromStdString(className).toLower().contains(query)) {
                    matchingIndices.insert(i);
                    break;
                }
            }
        }
    }

    fileList->clear();
    for (int index : matchingIndices) {
        fileList->addItem(QFileInfo(imageFiles.at(index)).fileName());
    }
    
    if (matchingIndices.isEmpty()) {
        QMessageBox::information(this, "Поиск", QString("Объекты '%1' не найдены в текущих изображениях.").arg(query));
    } else {
        loadImageByIndex(*matchingIndices.begin());
    }
}

void MainWindow::listItemClicked(QListWidgetItem *item) {
    QString fileNameOnly = item->text();
    for (int i = 0; i < imageFiles.size(); ++i) {
        if (QFileInfo(imageFiles.at(i)).fileName() == fileNameOnly) {
            if (i != currentIndex) {
                loadImageByIndex(i);
            }
            return;
        }
    }
}

void MainWindow::openSettingsDialog() {
    SettingsDialog dialog(yolo.getConfThreshold(), yolo.getNMSThreshold(), this);
    if (dialog.exec() == QDialog::Accepted) {
        yolo.setConfThreshold(dialog.getConfThreshold());
        yolo.setNMSThreshold(dialog.getNMSThreshold());
    }
}

// ----------------------------------------------------------------------
// --- МЕТОДЫ ДЛЯ OCR (РАСПОЗНАВАНИЕ ТЕКСТА) ---
// ----------------------------------------------------------------------

void MainWindow::startOcrRecognition() {
    if (currentImage.empty()) {
        QMessageBox::warning(this, "OCR Error", "Please load an image first.");
        return;
    }

    recognizeOcrButton->setEnabled(false);
    recognizeOcrButton->setText("Распознаю...");
    ocrResultTextEdit->clear();
    QApplication::processEvents();

    QString langCode = ocrLanguageBox->currentData().toString();
    
    QString recognizedText = ocr.recognizeText(currentImage, langCode);

    ocrResultTextEdit->setText(recognizedText);
    recognizeOcrButton->setEnabled(true);
    recognizeOcrButton->setText("Распознать текст");
    
    if (recognizedText.isEmpty()) {
         QMessageBox::information(this, "OCR Info", "No text was recognized.");
    }
}

// ----------------------------------------------------------------------
// --- МЕТОДЫ ДЛЯ WEB-ПОИСКА (Асинхронные) ---
// ----------------------------------------------------------------------

void MainWindow::startImageSearch() {
    QString query = searchQueryInput->text().trimmed();
    if (query.isEmpty()) {
        QMessageBox::warning(this, "Web Search", "Please enter a search query.");
        return;
    }
    
    cleanupSearchWorker();
    
    searchWebButton->setEnabled(false);
    searchWebButton->setText("Searching...");
    fileList->clear();
    imageFiles.clear();
    currentIndex = -1;
    imageLabel->clear();
    imageLabel->setText("Searching for images...");
    QApplication::processEvents();
    
    m_searchThread = new QThread(this);
    m_searchWorker = new WebSearchWorker(query);
    
    QString apiKey = "AIzaSyCsYV0Hm-4dKnbt6XY6fqhoQcpOTQEGzYs"; 
    QString cseId = "260b4ce11998e41d2";
    
    if (apiKey.contains("YOUR_") || cseId.contains("YOUR_")) {
         QMessageBox::critical(this, "API Error", "Please set your Google API Key and CSE ID in mainwindow.cpp to use web search.");
         searchWebButton->setEnabled(true);
         searchWebButton->setText("Поиск в Web (GCS)");
         cleanupSearchWorker();
         return;
    }
    
    m_searchWorker->setApiKeys(apiKey, cseId);
    m_searchWorker->moveToThread(m_searchThread);

    connect(m_searchThread, &QThread::started, m_searchWorker, &WebSearchWorker::doSearch);
    connect(m_searchWorker, &WebSearchWorker::searchFinished, this, &MainWindow::handleSearchResults);
    connect(m_searchWorker, &WebSearchWorker::searchError, this, &MainWindow::handleSearchError);
    
    connect(m_searchWorker, &WebSearchWorker::searchFinished, m_searchThread, &QThread::quit);
    connect(m_searchWorker, &WebSearchWorker::searchError, m_searchThread, &QThread::quit);
    connect(m_searchThread, &QThread::finished, m_searchWorker, &QObject::deleteLater);
    connect(m_searchThread, &QThread::finished, m_searchThread, &QObject::deleteLater);
    
    connect(m_searchThread, &QThread::finished, this, [this]() {
        m_searchWorker = nullptr;
        m_searchThread = nullptr;
    });

    m_searchThread->start();
}

void MainWindow::handleSearchResults(const QList<SearchResult>& results) {
    searchWebButton->setEnabled(true);
    searchWebButton->setText("Поиск в Web (GCS)");
    imageLabel->setText("Click on an image to load.");
    
    if (results.isEmpty()) {
        QMessageBox::information(this, "Web Search", "No images found for your query.");
        return;
    }

    fileList->clear();
    imageFiles.clear();
    
    for (const SearchResult& result : results) {
        downloadImage(result.imageUrl);
    }
}

void MainWindow::handleSearchError(const QString& error) {
    searchWebButton->setEnabled(true);
    searchWebButton->setText("Поиск в Web (GCS)");
    imageLabel->setText("Search failed.");
    QMessageBox::critical(this, "Web Search Error", "Search failed: " + error);
    
    m_searchWorker = nullptr;
    m_searchThread = nullptr;
}

void MainWindow::downloadImage(const QString& imageUrl) {
    QUrl url(imageUrl);
    if (!url.isValid()) {
        qWarning() << "Invalid URL:" << imageUrl;
        return;
    }

    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

    QNetworkReply *reply = netManager->get(request);
    activeDownloads++;

    connect(reply, &QNetworkReply::finished, this, &MainWindow::handleDownloadFinished);
}

void MainWindow::handleDownloadFinished() {
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    activeDownloads--;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray imageData = reply->readAll();
        QImage image;
        
        if (image.loadFromData(imageData)) {
            QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
            QString tempFileName = tempDir + QDir::separator() + "web_img_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmsszzz") + ".jpg";
            
            if (image.save(tempFileName, "JPG", 85)) {
                imageFiles.append(tempFileName);
                fileList->addItem(QFileInfo(tempFileName).fileName());
                
                if (imageFiles.size() == 1) {
                    loadImageByIndex(0);
                }
            } else {
                qWarning() << "Failed to save image to:" << tempFileName;
            }
        } else {
            qWarning() << "Failed to load image data from URL:" << reply->url().toString();
        }
    } else {
        qWarning() << "Image download failed:" << reply->errorString() << "for URL:" << reply->url().toString();
    }
    
    reply->deleteLater();
}

void MainWindow::thumbnailClicked(QListWidgetItem *item) {
    listItemClicked(item);
}

// ----------------------------------------------------------------------
// --- МЕТОДЫ ДЛЯ ФАЙЛОВОГО ДЕРЕВА ---
// ----------------------------------------------------------------------

void MainWindow::handleFileTreeClick(const QModelIndex &index) {
    if (!index.isValid()) return;
    
    QFileInfo fileInfo = fileSystemModel->fileInfo(index);
    QString filePath = fileInfo.absoluteFilePath();
    
    if (fileInfo.isFile()) {
        QString ext = fileInfo.suffix().toLower();
        if (QStringList{"png", "jpg", "jpeg", "bmp"}.contains(ext)) {
            imageFiles.clear();
            fileList->clear();
            imageFiles.append(filePath);
            fileList->addItem(fileInfo.fileName());
            loadImageByIndex(0);
        }
    }
}

void MainWindow::showContextMenu(const QPoint &pos) {
    QModelIndex index = fileTree->indexAt(pos);
    
    // 1. Определение целевой директории (m_currentDestPath)
    if (index.isValid()) {
        QFileInfo fileInfo = fileSystemModel->fileInfo(index);
        if (fileInfo.isDir()) {
            // Если щелкнули правой кнопкой по папке, то цель - сама папка
            m_currentDestPath = fileInfo.absoluteFilePath();
        } else {
            // Если щелкнули правой кнопкой по файлу, то цель - родительская директория
            m_currentDestPath = fileInfo.dir().absolutePath();
        }
    } else {
        // Если щелкнули правой кнопкой по пустому месту, то цель - текущий корень дерева
        m_currentDestPath = fileSystemModel->rootPath();
    }

    // 2. Создание меню и действий
    QMenu contextMenu(this);

    // --- НОВОЕ ДЕЙСТВИЕ: Copy File to Current Location ---
    QAction *copyFileAction = contextMenu.addAction(tr("Copy File Here..."));
    connect(copyFileAction, &QAction::triggered, this, &MainWindow::copyFileToCurrentDirectory);
    
    // --- Существующие действия ---
    QAction *openAction = new QAction(tr("Open"), this);
    QAction *openFolderAction = new QAction(tr("Open Containing Folder"), this);
    QAction *deleteAction = new QAction(tr("Delete"), this);
    QAction *saveAsAction = new QAction(tr("Save As..."), this);
    
    // Предполагаемые действия для QFileSystemModel
    QAction *newFolderAction = contextMenu.addAction(tr("New Folder..."));
    QAction *renameAction = contextMenu.addAction(tr("Rename"));


    if (index.isValid()) {
        contextMenu.addAction(openAction);
        contextMenu.addAction(openFolderAction);
        contextMenu.addSeparator();
        contextMenu.addAction(copyFileAction); // <-- Добавление нового действия
        contextMenu.addSeparator();
        
        // Управление файлами/папками
        contextMenu.addAction(newFolderAction);
        contextMenu.addAction(renameAction);
        contextMenu.addAction(deleteAction);
        
        if (fileSystemModel->fileInfo(index).isFile()) {
            contextMenu.addAction(saveAsAction); 
        }
        
        // Подключение существующих действий (через лямбда-функции для примера)
        connect(openAction, &QAction::triggered, this, [this, index](){ openFileInExternalApp(index); });
        connect(openFolderAction, &QAction::triggered, this, [this, index](){ openContainingFolder(index); });
        connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteSelectedFile);
        connect(renameAction, &QAction::triggered, this, &MainWindow::renameSelectedFile);
        // ... (другие connect-ы)
    } else {
        // Если кликнули на пустое место
        contextMenu.addAction(copyFileAction);
        contextMenu.addAction(newFolderAction);
    }
    
    // Подключение нового действия
    connect(newFolderAction, &QAction::triggered, this, &MainWindow::createNewFolder);

    contextMenu.exec(fileTree->mapToGlobal(pos));
}

void MainWindow::openFileInExternalApp(const QModelIndex &index) {
    QFileInfo fileInfo = fileSystemModel->fileInfo(index);
    if (fileInfo.isFile()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
    }
}

void MainWindow::openContainingFolder(const QModelIndex &index) {
    QFileInfo fileInfo = fileSystemModel->fileInfo(index);
    QString path;
    if (fileInfo.isDir()) {
        path = fileInfo.absoluteFilePath();
    } else {
        path = fileInfo.absolutePath();
    }
    
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void MainWindow::selectWorkingDirectory() {
    QString dirPath = QFileDialog::getExistingDirectory(this, tr("Select Working Directory"), 
                                                        QDir::currentPath());

    if (dirPath.isEmpty()) return;

    fileSystemModel->setRootPath(dirPath);
    fileTree->setRootIndex(fileSystemModel->index(dirPath));
}

void MainWindow::deleteFile(const QModelIndex &index) {
    QFileInfo fileInfo = fileSystemModel->fileInfo(index);
    QString path = fileInfo.absoluteFilePath();
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Confirm Deletion"), 
                                  QString("Are you sure you want to delete:\n%1").arg(fileInfo.fileName()),
                                  QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (QFile::remove(path)) {
            qDebug() << "Successfully deleted:" << path;
        } else {
            QMessageBox::critical(this, tr("Deletion Failed"), tr("Could not delete file: %1").arg(path));
        }
    }
}

void MainWindow::saveFileAs(const QModelIndex &index) {
    QFileInfo fileInfo = fileSystemModel->fileInfo(index);
    QString currentPath = fileInfo.absoluteFilePath();
    
    QString savePath = QFileDialog::getSaveFileName(this, tr("Save File As"),
                                                    currentPath,
                                                    tr("Images (*.png *.jpg *.jpeg *.bmp)"));
    
    if (!savePath.isEmpty()) {
        QFile::copy(currentPath, savePath);
        QMessageBox::information(this, tr("Saved"), tr("File successfully saved to:\n%1").arg(savePath));
    }
}

// ----------------------------------------------------------------------
// --- МЕТОДЫ ДЛЯ ТУЛБАРА (НОВЫЕ) ---
// ----------------------------------------------------------------------

void MainWindow::createNewFolder() {
    QModelIndex index = fileTree->currentIndex();
    if (!index.isValid()) {
        index = fileTree->rootIndex();
    }
    
    if (!fileSystemModel->isDir(index)) {
        index = index.parent();
    }
    
    bool ok;
    QString folderName = QInputDialog::getText(this, "New Folder", 
                                               "Folder name:", 
                                               QLineEdit::Normal,
                                               "New Folder", &ok);
    if (ok && !folderName.isEmpty()) {
        QModelIndex newIndex = fileSystemModel->mkdir(index, folderName);
        if (newIndex.isValid()) {
            fileTree->expand(index);
            fileTree->setCurrentIndex(newIndex);
        } else {
            QMessageBox::warning(this, "Error", "Could not create folder!");
        }
    }
}

void MainWindow::deleteSelectedFile() {
    QModelIndex index = fileTree->currentIndex();
    if (!index.isValid()) {
        return;
    }
    
    QString name = fileSystemModel->fileName(index);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete", 
                                  "Delete \"" + name + "\"?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (!fileSystemModel->remove(index)) {
            QMessageBox::warning(this, "Error", "Could not delete \"" + name + "\"!");
        }
    }
}

void MainWindow::refreshFileTree() {
    fileSystemModel->setRootPath(fileSystemModel->rootPath());
}

void MainWindow::renameSelectedFile() {
    QModelIndex index = fileTree->currentIndex();
    if (index.isValid()) {
        fileTree->edit(index);
    }
}

void MainWindow::copyFileToCurrentDirectory() {
    // 1. Проверка пути назначения
    if (m_currentDestPath.isEmpty() || !QDir(m_currentDestPath).exists()) {
        QMessageBox::warning(this, tr("Error"), tr("Target directory is invalid or not defined."));
        return;
    }

    // 2. Выбор исходного файла
    QString sourceFile = QFileDialog::getOpenFileName(this, 
                                                    tr("Select File to Copy"), 
                                                    QDir::homePath(), 
                                                    tr("All Files (*.*)"));

    if (sourceFile.isEmpty()) {
        return; // Пользователь отменил выбор
    }

    QFileInfo sourceInfo(sourceFile);
    // Формирование полного пути назначения: TargetDir/FileName
    QString destPath = m_currentDestPath + QDir::separator() + sourceInfo.fileName();

    // 3. Проверка на существование и запрос на перезапись
    if (QFile::exists(destPath)) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Confirm Overwrite"),
                                    tr("File '%1' already exists in the target directory. Overwrite?").arg(sourceInfo.fileName()),
                                    QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }
        // Удаляем существующий файл
        if (!QFile::remove(destPath)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not remove existing file: %1. Check permissions.").arg(sourceInfo.fileName()));
            return;
        }
    }

    // 4. Выполнение копирования
    if (QFile::copy(sourceFile, destPath)) {
        QMessageBox::information(this, tr("Success"), 
                                tr("File successfully copied to: %1").arg(QDir::toNativeSeparators(m_currentDestPath)));
        
        // 5. Обновление файлового дерева
        QModelIndex indexToRefresh = fileSystemModel->index(m_currentDestPath);
        if (indexToRefresh.isValid()) {
             // force update of the view
             fileSystemModel->setRootPath(fileSystemModel->rootPath()); 
        }

    } else {
        QMessageBox::critical(this, tr("Error"), 
                            tr("File copy failed to '%1'. Check permissions.").arg(QDir::toNativeSeparators(m_currentDestPath)));
    }
}

void MainWindow::showFileListContextMenu(const QPoint &pos) {
    QListWidgetItem *item = fileList->itemAt(pos);
    if (!item) return;

    // Получаем полный путь к файлу
    QString fileNameOnly = item->text();
    QString fullFilePath;
    for (const QString& filePath : imageFiles) {
        if (QFileInfo(filePath).fileName() == fileNameOnly) {
            fullFilePath = filePath;
            break;
        }
    }

    if (fullFilePath.isEmpty()) return;

    QMenu contextMenu(this);

    // Действие для копирования файла
    QAction *copyToFolderAction = contextMenu.addAction(tr("Copy to Folder in Tree"));
    QAction *openInExternalAction = contextMenu.addAction(tr("Open in External App"));
    QAction *openContainingFolderAction = contextMenu.addAction(tr("Open Containing Folder"));

    connect(copyToFolderAction, &QAction::triggered, this, [this, fullFilePath]() {
        copyFileFromListToTree(fullFilePath);
    });
    connect(openInExternalAction, &QAction::triggered, this, [fullFilePath]() {
        QDesktopServices::openUrl(QUrl::fromLocalFile(fullFilePath));
    });
    connect(openContainingFolderAction, &QAction::triggered, this, [fullFilePath]() {
        QFileInfo fileInfo(fullFilePath);
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absolutePath()));
    });

    contextMenu.exec(fileList->mapToGlobal(pos));
}


void MainWindow::copyFileFromListToTree(const QString &sourceFilePath) {
    // Получаем выбранную папку в дереве
    QModelIndex currentTreeIndex = fileTree->currentIndex();
    QString targetDir;

    if (currentTreeIndex.isValid()) {
        QFileInfo fileInfo = fileSystemModel->fileInfo(currentTreeIndex);
        if (fileInfo.isDir()) {
            targetDir = fileInfo.absoluteFilePath();
        } else {
            // Если выбран файл, используем его родительскую директорию
            targetDir = fileInfo.dir().absolutePath();
        }
    } else {
        // Если ничего не выбрано, используем корневую директорию дерева
        targetDir = fileSystemModel->rootPath();
    }

    if (targetDir.isEmpty() || !QDir(targetDir).exists()) {
        QMessageBox::warning(this, tr("Error"), tr("No valid target directory selected in the file tree."));
        return;
    }

    QFileInfo sourceInfo(sourceFilePath);
    QString targetPath = targetDir + QDir::separator() + sourceInfo.fileName();

    // Проверка на существование файла
    if (QFile::exists(targetPath)) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Confirm Overwrite"),
                                    tr("File '%1' already exists in the target directory. Overwrite?").arg(sourceInfo.fileName()),
                                    QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }
        // Удаляем существующий файл
        if (!QFile::remove(targetPath)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not remove existing file: %1. Check permissions.").arg(sourceInfo.fileName()));
            return;
        }
    }

    // Копируем файл
    if (QFile::copy(sourceFilePath, targetPath)) {
        qDebug() << sourceFilePath;
        qDebug() << targetPath;
        QMessageBox::information(this, tr("Success"), 
                                tr("File successfully copied to:\n%1").arg(QDir::toNativeSeparators(targetDir)));
        
        // Обновляем файловое дерево
        refreshFileTree();
    } else {
        QMessageBox::critical(this, tr("Error"), 
                            tr("File copy failed to:\n%1\nCheck permissions.").arg(QDir::toNativeSeparators(targetDir)));
    }
}