#pragma once
#include<opencv2/opencv.hpp>
#include<ispring/All.h>
enum DAType {
	YOLO,FasterRCNN,DLIB
};
std::string Generate_Gray(std::string filename, DAType type = DAType::YOLO);
std::string Generate_SaltAndPepper(std::string filename, DAType type = DAType::YOLO);
std::string Generate_FilpLR(std::string filename, DAType type = DAType::YOLO);
std::string Generate_FilpTB(std::string filename, DAType type = DAType::YOLO);
std::string Generate_Slip(std::string filename, DAType type = DAType::YOLO);
std::string Generate_Blur(std::string filename, DAType type = DAType::YOLO);
void GammaCorrection(cv::Mat& src, cv::Mat& dst, float fGamma);
std::string Generate_GammaCorrection1(std::string filename, DAType type = DAType::YOLO);
std::string Generate_GammaCorrection2(std::string filename, DAType type = DAType::YOLO);