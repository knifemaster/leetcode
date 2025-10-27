#ifndef OCR_PROCESSOR_H
#define OCR_PROCESSOR_H

#include <QString>
#include <opencv2/core/mat.hpp>

class OcrProcessor {
public:
    OcrProcessor();
    ~OcrProcessor();

    // Основная функция для распознавания текста из cv::Mat
    QString recognizeText(const cv::Mat& image, const QString& languageCode);

private:
    // В реальной реализации здесь может храниться tesseract::TessBaseAPI
};

#endif // OCR_PROCESSOR_H