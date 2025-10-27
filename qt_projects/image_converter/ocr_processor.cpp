#include "ocr_processor.h"
#include <QDebug>
#include <QMessageBox>
#include <opencv2/imgproc.hpp>

// --- Требуется линковка с Tesseract (см. CMakeLists.txt) ---
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
// -----------------------------------------------------------

OcrProcessor::OcrProcessor() {
    // Ничего не делаем, инициализация TessBaseAPI будет внутри recognizeText для потокобезопасности
}

OcrProcessor::~OcrProcessor() {
    // Очистка Tesseract
}

QString OcrProcessor::recognizeText(const cv::Mat& image, const QString& languageCode) {
    if (image.empty()) {
        return "Error: Input image is empty.";
    }
    
    // --- РЕАЛИЗАЦИЯ TESSERACT OCR ---
    // Этот блок является примером, который требует наличия файлов данных Tesseract (.traineddata)
    
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    
    // 1. Инициализация Tesseract
    // nullptr - для пути к данным Tesseract (ищет в стандартных местах)
    if (api->Init(nullptr, languageCode.toStdString().c_str())) {
        delete api;
        return "OCR Initialization Error. Check Tesseract installation and language data (.traineddata) for language: " + languageCode;
    }

    // 2. Предобработка изображения OpenCV (Grayscale -> Threshold)
    cv::Mat gray_image, binary_image;

    // Конвертировать в оттенки серого, если это цветное изображение
    if (image.channels() == 3) {
        cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
    } else {
        gray_image = image.clone();
    }
    
    // Применить бинаризацию (ОЦУ используется для автоматического выбора порога)
    cv::threshold(gray_image, binary_image, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    // 3. Конвертация OpenCV Mat в Leptonica Pix
    // Tesseract предпочитает ввод в формате Leptonica Pix
    Pix *pixImage = pixCreate(binary_image.cols, binary_image.rows, 8);
    for (int y = 0; y < binary_image.rows; ++y) {
        for (int x = 0; x < binary_image.cols; ++x) {
            // Устанавливаем пиксель (предполагая 8-битный одноканальный Mat)
            int val = binary_image.at<uchar>(y, x);
            pixSetPixel(pixImage, x, y, val);
        }
    }

    // 4. Выполнение OCR
    api->SetImage(pixImage);
    char *outText = api->GetUTF8Text();

    // 5. Очистка
    QString result = QString::fromUtf8(outText);
    delete [] outText;
    pixDestroy(&pixImage);
    api->End();
    delete api;

    return result.trimmed();
}