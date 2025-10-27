#include <QDebug>
#include "yolo_processor.h"
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>
#include <iostream>
#include <algorithm> 
#include <set>       
#include <cmath>

using namespace cv;
using namespace cv::dnn;

// ----------------------------------------------------------------------
// ПРИВАТНЫЕ ФУНКЦИИ
// ----------------------------------------------------------------------

void YoloProcessor::loadClasses(const std::string& namesPath) {
    classes.clear();
    std::ifstream ifs(namesPath.c_str());
    if (ifs.is_open()) {
        std::string line;
        while (std::getline(ifs, line)) {
            classes.push_back(line);
        }
    } else {
        std::cerr << "Warning: Could not open names file: " << namesPath << std::endl;
    }
}

std::vector<std::string> YoloProcessor::getOutputsNames(const Net& net) {
    static std::vector<std::string> names;
    if (names.empty()) {
        std::vector<int> outLayers = net.getUnconnectedOutLayers();
        std::vector<std::string> layersNames = net.getLayerNames();

        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i)
            names[i] = layersNames[outLayers[i] - 1];
    }
    return names;
}

void YoloProcessor::drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat& frame) {
    // Рисуем ограничивающую рамку
    rectangle(frame, Point(left, top), Point(right, bottom), Scalar(0, 255, 0), 2);

    // Получаем метку
    std::string label = classes.empty() ? "" : classes[classId];
    label = format("%.2f", conf) + " " + label;

    // Отображаем метку над рамкой
    int baseLine;
    Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    top = std::max(top, labelSize.height);
    rectangle(frame, Point(left, top - round(1.5 * labelSize.height)), 
              Point(left + round(1.5 * labelSize.width), top + baseLine), Scalar(0, 255, 0), FILLED);
    putText(frame, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 0), 1);
}

void YoloProcessor::findObjects(const Mat& frame,
                                const std::vector<Mat>& outs,
                                std::vector<int>& classIds,
                                std::vector<float>& confidences,
                                std::vector<Rect>& boxes) {
    classIds.clear();
    confidences.clear();
    boxes.clear();

    for (size_t i = 0; i < outs.size(); ++i) {
        float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols) {
            Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            Point classIdPoint;
            double confidence;
            // Получаем максимальную вероятность для класса
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            
            if (confidence > confThreshold) {
                int centerX = (int)(data[0] * frame.cols);
                int centerY = (int)(data[1] * frame.rows);
                int width = (int)(data[2] * frame.cols);
                int height = (int)(data[3] * frame.rows);
                int left = centerX - width / 2;
                int top = centerY - height / 2;

                classIds.push_back(classIdPoint.x);
                confidences.push_back((float)confidence);
                boxes.push_back(Rect(left, top, width, height));
            }
        }
    }

    // Применяем Non-Maximum Suppression (NMS) для фильтрации перекрывающихся рамок
    std::vector<int> indices;
    NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices); 

    // Сохраняем только те, что прошли NMS
    std::vector<Rect> finalBoxes;
    std::vector<int> finalClassIds;
    std::vector<float> finalConfidences;

    for (size_t i = 0; i < indices.size(); ++i) {
        int idx = indices[i];
        finalBoxes.push_back(boxes[idx]);
        finalClassIds.push_back(classIds[idx]);
        finalConfidences.push_back(confidences[idx]);
    }
    
    boxes = finalBoxes;
    classIds = finalClassIds;
    confidences = finalConfidences;
}

// ----------------------------------------------------------------------
// ПУБЛИЧНЫЕ ФУНКЦИИ
// ----------------------------------------------------------------------

void YoloProcessor::loadModel(const std::string& cfgPath,
                              const std::string& weightsPath,
                              const std::string& namesPath) {
    try {
        net = readNetFromDarknet(cfgPath, weightsPath);
        net.setPreferableBackend(DNN_BACKEND_OPENCV);
        net.setPreferableTarget(DNN_TARGET_CPU); // Используем CPU для совместимости
        loadClasses(namesPath);
        netLoaded = true;
        std::cout << "YOLO model loaded successfully." << std::endl;
    } catch (const cv::Exception& e) {
        std::cerr << "OpenCV Error loading YOLO model: " << e.what() << std::endl;
        netLoaded = false;
    }
}

cv::Mat YoloProcessor::detectObjects(const cv::Mat& image) {
    if (!netLoaded || image.empty()) {
        return image.clone();
    }

    Mat frame = image.clone();
    Mat blob;
    blobFromImage(frame, blob, 1/255.0, cv::Size(416, 416), cv::Scalar(0,0,0), true, false);

    net.setInput(blob);
    
    std::vector<Mat> outs;
    net.forward(outs, getOutputsNames(net));

    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<Rect> boxes;
    
    findObjects(frame, outs, classIds, confidences, boxes);

    for (size_t i = 0; i < boxes.size(); ++i) {
        Rect box = boxes[i];
        drawPred(classIds[i], confidences[i], box.x, box.y, box.x + box.width, box.y + box.height, frame);
    }

    return frame;
}

std::vector<std::string> YoloProcessor::getDetectedClasses(const cv::Mat& image) {
    std::vector<std::string> detectedClasses;
    if (!netLoaded || image.empty()) {
        return detectedClasses;
    }

    Mat frame = image.clone();
    Mat blob;
    blobFromImage(frame, blob, 1/255.0, cv::Size(416, 416), cv::Scalar(0,0,0), true, false);
    net.setInput(blob);
    
    std::vector<Mat> outs;
    net.forward(outs, getOutputsNames(net));

    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<Rect> boxes;
    
    findObjects(frame, outs, classIds, confidences, boxes);
    
    std::set<std::string> uniqueClasses;
    for (int id : classIds) {
        if (id >= 0 && id < classes.size()) {
            uniqueClasses.insert(classes[id]);
        }
    }
    
    detectedClasses.assign(uniqueClasses.begin(), uniqueClasses.end());
    return detectedClasses;
}

cv::Mat YoloProcessor::applyFilter(const cv::Mat& image,
                                    ImageFilter filter,
                                    const FilterParams& params) {
    try {
        if (image.empty()) {
            return image;
        }

        Mat result = image.clone();

        // 1. Применение коррекции яркости/контраста
        // alpha (контраст) = 1 + contrast/100
        // beta (яркость) = brightness
        double alpha = 1.0 + params.contrast / 100.0;
        int beta = params.brightness;
        result.convertTo(result, -1, alpha, beta);

        // 2. Применение фильтров
        switch (filter) {
            case ImageFilter::Blur: {
                qDebug() << "Applying Blur";
                if (params.blur > 0) {
                    cv::blur(result, result, cv::Size(params.blur * 2 + 1, params.blur * 2 + 1));
                }
                break;
            }
            case ImageFilter::GAUSSIAN_BLUR: {
                qDebug() << "Applying Gaussian Blur";
                if (params.blur > 0) {
                    cv::GaussianBlur(result, result, cv::Size(params.blur * 2 + 1, params.blur * 2 + 1), 0);
                }
                break;
            }
            case ImageFilter::Edge: {
                qDebug() << "Applying Edge";
                Mat gray;
                if (result.channels() == 3) {
                    cvtColor(result, gray, COLOR_BGR2GRAY);
                } else {
                    gray = result.clone();
                }
                cv::Canny(gray, result, 100, 200);
                // Конвертация обратно в 3-канальное изображение для QLabel
                if (image.channels() == 3 && result.channels() == 1) {
                    cvtColor(result, result, COLOR_GRAY2BGR);
                }
                break;
            }
            case ImageFilter::HSV: {
                qDebug() << "Applying HSV (Hue:" << params.hue << ", Saturation:" << params.saturation << ")";
                Mat hsv;
                cvtColor(result, hsv, COLOR_BGR2HSV);

                std::vector<Mat> hsv_planes;
                split(hsv, hsv_planes);

                // Коррекция Hue
                hsv_planes[0] += params.hue / 2; // OpenCV H [0-180], Qt H [-180-180]
                
                // Коррекция Saturation
                double sat_factor = 1.0 + params.saturation / 100.0;
                hsv_planes[1].convertTo(hsv_planes[1], -1, sat_factor);
                
                merge(hsv_planes, hsv);
                cvtColor(hsv, result, COLOR_HSV2BGR);
                break;
            }
            case ImageFilter::Sharpen: {
                qDebug() << "Applying Sharpen with strength:" << params.sharpen_strength;
                if (params.sharpen_strength > 0) {
                    Mat blurred;
                    cv::GaussianBlur(result, blurred, cv::Size(0, 0), 3);
                    
                    // Усиление резкости: result + amount * (result - blurred)
                    // amount = sharpen_strength / 100.0
                    double amount = params.sharpen_strength / 100.0;
                    cv::addWeighted(result, 1.0 + amount, blurred, -amount, 0, result);
                }
                break;
            }
            case ImageFilter::Threshold: {
                qDebug() << "Applying Threshold";
                Mat gray;
                if (result.channels() == 3) {
                    cvtColor(result, gray, COLOR_BGR2GRAY);
                } else {
                    gray = result.clone();
                }
                cv::threshold(gray, result, params.threshold_value, 255, cv::THRESH_BINARY);
                if (image.channels() == 3 && result.channels() == 1) {
                    cvtColor(result, result, COLOR_GRAY2BGR);
                }
                break;
            }
            case ImageFilter::Grayscale: {
                 qDebug() << "Applying Grayscale";
                 if (result.channels() == 3) {
                    cvtColor(result, result, COLOR_BGR2GRAY);
                    // Конвертация обратно в 3-канальное изображение для QLabel
                    cvtColor(result, result, COLOR_GRAY2BGR);
                 }
                 break;
            }

            default:
                break;
        }
        
        return result;
        
    } catch (const cv::Exception& e) {
        std::cerr << "OpenCV error in applyFilter: " << e.what() << std::endl;
        return image.clone();
    } catch (const std::exception& e) {
        std::cerr << "Error in applyFilter: " << e.what() << std::endl;
        return image.clone();
    }
}