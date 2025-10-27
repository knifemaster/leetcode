#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <set> 

// --- фильтры изображения ---\n
enum class ImageFilter {
    None,
    Blur,
    Edge,
    HSV,
    GAUSSIAN_BLUR,
    Sharpen,   // НОВЫЙ
    Threshold, // НОВЫЙ
    Grayscale // НОВЫЙ
};

// --- параметры фильтра ---\n
struct FilterParams {
    int blur = 0;
    int brightness = 0;
    int contrast = 0;
    int hue = 0;
    int saturation = 0;

    int sharpen_strength = 0; // Для силы повышения резкости (0-100)
    int threshold_value = 127;
};

class YoloProcessor {
public:
    YoloProcessor() : netLoaded(false) {} 
    
    void loadModel(const std::string& cfgPath,
                   const std::string& weightsPath,
                   const std::string& namesPath);

    bool isLoaded() const { return netLoaded; }

    cv::Mat detectObjects(const cv::Mat& image);          
    
    std::vector<std::string> getDetectedClasses(const cv::Mat& image);

    cv::Mat applyFilter(const cv::Mat& image,
                        ImageFilter filter,
                        const FilterParams& params);      

    float getConfThreshold() const { return confThreshold; }
    float getNMSThreshold() const { return nmsThreshold; }
    void setConfThreshold(float val) { confThreshold = val; }
    void setNMSThreshold(float val) { nmsThreshold = val; }

private:
    void loadClasses(const std::string& namesPath);
    void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame);
    std::vector<std::string> getOutputsNames(const cv::dnn::Net& net);
    
    // НОВАЯ ПРИВАТНАЯ ФУНКЦИЯ: Выполняет обработку выхода YOLO (NMS)
    void findObjects(const cv::Mat& frame, 
                     const std::vector<cv::Mat>& outs,
                     std::vector<int>& classIds, 
                     std::vector<float>& confidences, 
                     std::vector<cv::Rect>& boxes); 

    cv::dnn::Net net;
    std::vector<std::string> classes;
    bool netLoaded;
    float confThreshold = 0.5f; // Порог уверенности
    float nmsThreshold = 0.4f;  // Порог NMS
};