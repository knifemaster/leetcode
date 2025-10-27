#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QFileDialog>
#include <QStringList>
#include <QLineEdit>        
#include <QListWidget>      
#include <QListWidgetItem>  
#include <QNetworkAccessManager> 
#include <QNetworkReply>         
#include <QScrollArea>      
#include <QWidget>          
#include <QEvent>           
#include <QMouseEvent>      
#include <QList>            
#include <QThread>               
#include <QTabWidget>           
#include <QTextEdit>            
#include <opencv2/videoio.hpp>  
#include "yolo_processor.h"
#include "web_search_worker.h" 
#include "ocr_processor.h"      
#include "settings_dialog.h" 

#include <QMenu>                
#include <QFileSystemModel>     
#include <QTreeView>            
#include <QDir>                 
#include <QDesktopServices>
#include <QToolBar>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

 
private slots:
    void loadImage();
    void loadFolder();
    void showNextImage();
    void showPrevImage();
    void runDetection();
    void updateFilter();
    void updateParams();
    void applyCurrentFilter();
    void handleSearch();        
    void listItemClicked(QListWidgetItem *item);
    void openSettingsDialog();
    void saveImage(); 
    
    // --- СЛОТЫ ДЛЯ WEB-ПОИСКА ---
    void startImageSearch();
    void handleSearchResults(const QList<SearchResult>& results);
    void handleSearchError(const QString& error);
    void handleDownloadFinished(); 
    void downloadImage(const QString& imageUrl);

    // --- СЛОТЫ ДЛЯ ВИДЕО ---
    void loadVideo();
    void videoSliderMoved(int frameNumber);
    void thumbnailClicked(QListWidgetItem *item); 
    
    // --- СЛОТ ДЛЯ OCR ---
    void startOcrRecognition();
    void selectWorkingDirectory();
    
    // --- СЛОТЫ ДЛЯ ФАЙЛОВОГО ДЕРЕВА ---
    void showContextMenu(const QPoint &pos);
    void handleFileTreeClick(const QModelIndex &index);
    void openFileInExternalApp(const QModelIndex &index);
    void openContainingFolder(const QModelIndex &index);
    void deleteFile(const QModelIndex &index);
    void saveFileAs(const QModelIndex &index);
    
    // --- СЛОТЫ ДЛЯ ТУЛБАРА ---
    void createNewFolder();
    void deleteSelectedFile();
    void refreshFileTree();
    void renameSelectedFile();

    // --- копирование в коллекцию
    void copyFileToCurrentDirectory();

    // --- СЛОТЫ ДЛЯ ФАЙЛОВОГО СПИСКА ---
    void showFileListContextMenu(const QPoint &pos);
    void copyFileFromListToTree(const QString &sourceFilePath);


private:
    // Приватные служебные функции
    void setupUI();
    void createActions();
    void createConnections();
    void loadImageByIndex(int index);
    void cleanupSearchWorker();
    
    void updateThumbnailPreview(int frameNumber, const QPoint& globalPos);
    void hideThumbnailPreview();
    
    bool eventFilter(QObject *obj, QEvent *event) override;


private:
    QLabel *imageLabel;
    
    QTabWidget *leftTabWidget;

    QComboBox *filterBox;
    QSlider *brightnessSlider, *contrastSlider, *hueSlider, *saturationSlider, *blurSlider;
    QSlider *sharpenStrengthSlider;
    QSlider *thresholdValue;
    
    QPushButton *runButton; //, *nextButton, *prevButton;

    QLineEdit *searchBox;
    QListWidget *fileList;
    
    QLineEdit *searchQueryInput;
    QPushButton *searchWebButton;
    QNetworkAccessManager *netManager;
    int activeDownloads;

    // --- ЭЛЕМЕНТЫ ДЛЯ ВИДЕО ---
    QAction *loadVideoAction;
    QSlider *videoSlider;
    cv::VideoCapture videoCapture;
    int totalFrames = 0;
    int currentFrame = -1;
    QLabel *thumbnailPreviewLabel;
    
    // --- ЭЛЕМЕНТЫ ДЛЯ МИНИАТЮР (для вкладки "Выделение") ---
    QScrollArea *thumbnailScrollArea;    
    QWidget *thumbnailContainer;         
    QVBoxLayout *thumbnailLayout;        
    QList<QLabel*> thumbnailLabels;      
    
    // --- YOLO И ОБРАБОТКА ---
    cv::Mat displayedImage; 
    cv::Mat currentImage;   
    QStringList imageFiles;
    int currentIndex = -1;

    YoloProcessor yolo;
    ImageFilter currentFilter = ImageFilter::None;
    FilterParams params;

    // --- АСИНХРОННЫЙ ПОИСК ---
    QThread *m_searchThread = nullptr;
    WebSearchWorker *m_searchWorker = nullptr;
    
    // --- ЭЛЕМЕНТЫ ДЛЯ OCR ---
    QComboBox *ocrLanguageBox;
    QPushButton *recognizeOcrButton;
    QTextEdit *ocrResultTextEdit;
    OcrProcessor ocr;

    // --- ЭЛЕМЕНТЫ ДЛЯ ФАЙЛОВОГО ДЕРЕВА ---
    QFileSystemModel *fileSystemModel;
    QTreeView *fileTree;
    QToolBar *toolbar;

    QString m_currentDestPath;

};

#endif // MAINWINDOW_H