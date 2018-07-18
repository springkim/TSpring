#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
//#include"Theme.h"
inline std::vector<float> IOU(std::pair<float, float> val, std::vector<std::pair<float, float>> centroids) {
	std::vector<float> similarities;
	for (size_t i = 0; i < centroids.size(); i++) {
		float s = 0.0F;
		float& w = val.first;
		float& h = val.second;
		float& cw = centroids[i].first;
		float& ch = centroids[i].second;
		if (cw > w && ch > h) {
			s = (w*h) / (cw*ch);
		} else if (cw > w && ch <= h) {
			s = w*ch / (w*h + (cw - w)*ch);
		} else if (cw <= w && ch >= h) {
			s = cw*h / (w*h + cw*(ch - h));
		} else {
			s = (cw*ch) / (w*h);
		}
		similarities.push_back(1.0F - s);
	}
	return similarities;
}
inline std::vector<std::pair<float, float>> KMeans(std::vector<std::pair<float, float>> X, std::vector<std::pair<float, float>> centroids,std::vector<int>& label) {
	std::vector<std::pair<float, float>> ret;
	size_t N = X.size();
	size_t K = centroids.size();
	std::vector<std::vector<float>> D;
	std::vector<int> prev_assignments;
	while (true) {
		for (int i = 0; i < N; i++) {
			std::vector<float> d = IOU(X[i], centroids);
			D.push_back(d);
		}
		std::vector<int> assignments;
		for (int i = 0; i < N; i++) {
			int a = static_cast<int>(std::min_element(D[i].begin(), D[i].end()) - D[i].begin());
			assignments.push_back(a);
		}
		if (assignments == prev_assignments) {
			label = assignments;
			ret = centroids;
			break;
		}
		std::vector<std::pair<float, float>> centroid_sums;
		centroid_sums.assign(K, std::pair<float, float>(0.0F, 0.0F));
		for (int i = 0; i < N; i++) {
			centroid_sums[assignments[i]].first += X[i].first;
			centroid_sums[assignments[i]].second += X[i].second;
		}
		for (int i = 0; i < K; i++) {
			size_t cnt = std::max(std::count(assignments.begin(), assignments.end(), i), 1LL);
			centroids[i].first = centroid_sums[i].first / cnt;
			centroids[i].second = centroid_sums[i].second / cnt;
		}
		prev_assignments = assignments;
	}

	std::sort(ret.begin(), ret.end());
	return ret;
}
inline std::string GetAnchors(int NUM,int WH,std::vector<std::string> images,bool dbg_info,bool is_yolov3) {
	std::vector<std::pair<float, float>> annotation_dims;
	std::set<std::pair<float, float>> set_annotation_dims;
	cv::Mat img = cv::Mat::zeros(1920, 1920, CV_8UC3) + cv::Scalar(0,0,0);
	for (auto&file : images) {
		file = file.substr(0, file.find_last_of('.')) + ".txt";
		std::fstream fin(file, std::ios::in);
		if (fin.is_open() == true) {
			while (fin.eof() == false) {
				std::string line;
				std::getline(fin, line);
				if (line.length() == 0)break;
				std::istringstream iss;
				iss.str(line);
				float c, x, y, w, h;
				iss >> c >> x >> y >> w >> h;
				set_annotation_dims.insert(std::make_pair(w, h));
			}
			fin.close();
		}
	}
	for (auto&e : set_annotation_dims) {
		annotation_dims.push_back(e);
	}
	std::sort(annotation_dims.begin(), annotation_dims.end(), [](std::pair<float, float> a, std::pair<float, float> b)->bool {
		return a.first*a.second < b.first*b.second;
	});
	
	std::vector<std::pair<float, float>> centroids;

	for (int i = 0; i < NUM; i++) {
		centroids.push_back(annotation_dims[annotation_dims.size() / (NUM + 2)*(i + 1)]);
	}
	std::vector<int> labels;
	centroids = KMeans(annotation_dims, centroids,labels);

	if (dbg_info) {
		for (size_t i = 0; i < annotation_dims.size(); i++) {
			int W = static_cast<int>(annotation_dims[i].first * 1920);
			int H = static_cast<int>(annotation_dims[i].second * 1920);
			cv::circle(img, cv::Point(W, H), 5, ispring::CV::GetRGB(labels[i]), CV_FILLED);
		}
		for (size_t i = 0; i < centroids.size(); i++) {
			int W = static_cast<int>(centroids[i].first * 1920);
			int H = static_cast<int>(centroids[i].second * 1920);
			cv::line(img, cv::Point(0, H), cv::Point(W, H), cv::Scalar(255, 255, 255), 2);
			cv::line(img, cv::Point(W, 0), cv::Point(W, H), cv::Scalar(255, 255, 255), 2);
		}
		cv::imwrite("Debug\\anchors.png",img);
	}
	if (is_yolov3==false) {
		for (auto&e : centroids) {
			e.first *= WH / 32;
			e.second *= WH / 32;
		}
	} else {
		for (auto&e : centroids) {
			e.first *= WH;
			e.second *= WH;
		}
	}
	
	std::ostringstream oss;
	oss.precision(3);
	for (int i = 0; i < centroids.size() - 1; i++) {
		oss << centroids[i].first << "," << centroids[i].second << ", ";
	}
	oss << centroids.back().first << "," << centroids.back().second;
	return oss.str();
}

inline std::string GetAdvancedAnchors(int NUM, int WH, std::vector<std::string> images, bool dbg_info, bool is_yolov3) {
	std::vector<std::pair<float, float>> annotation_dimsW, annotation_dimsH;
	std::set<std::pair<float, float>> set_annotation_dimsW, set_annotation_dimsH;
	int NUM_W = static_cast<int>(ceil(NUM / 2));
	int NUM_H = NUM - NUM_W;
	cv::Mat img = cv::Mat::zeros(1920, 1920, CV_8UC3) + cv::Scalar(0, 0, 0);
	for (auto&file : images) {
		file = file.substr(0, file.find_last_of('.')) + ".txt";
		std::fstream fin(file, std::ios::in);
		if (fin.is_open() == true) {
			while (fin.eof() == false) {
				std::string line;
				std::getline(fin, line);
				if (line.length() == 0)break;
				std::istringstream iss;
				iss.str(line);
				float c, x, y, w, h;
				iss >> c >> x >> y >> w >> h;
				if (w >= h) {
					set_annotation_dimsW.insert(std::make_pair(w, h));
				} else {
					set_annotation_dimsH.insert(std::make_pair(w, h));
				}
			}
			fin.close();
		}
	}
	for (auto&e : set_annotation_dimsW) {
		annotation_dimsW.push_back(e);
	}
	for (auto&e : set_annotation_dimsH) {
		annotation_dimsH.push_back(e);
	}
	std::sort(annotation_dimsW.begin(), annotation_dimsW.end(), [](std::pair<float, float> a, std::pair<float, float> b)->bool {
		return a.first*a.second < b.first*b.second;
	});
	std::sort(annotation_dimsH.begin(), annotation_dimsH.end(), [](std::pair<float, float> a, std::pair<float, float> b)->bool {
		return a.first*a.second < b.first*b.second;
	});

	std::vector<std::pair<float, float>> centroidsW, centroidsH;
	for (int i = 0; i < NUM_W; i++) {
		centroidsW.push_back(annotation_dimsW[annotation_dimsW.size() / (NUM_W + 2)*(i + 1)]);
	}
	for (int i = 0; i < NUM_H; i++) {
		centroidsH.push_back(annotation_dimsH[annotation_dimsH.size() / (NUM_H + 2)*(i + 1)]);
	}
	std::vector<int> labelsW, labelsH;
	centroidsW = KMeans(annotation_dimsW, centroidsW, labelsW);
	centroidsH = KMeans(annotation_dimsH, centroidsH, labelsH);

	if (dbg_info) {
		for (size_t i = 0; i < annotation_dimsW.size(); i++) {
			int W = static_cast<int>(annotation_dimsW[i].first * 1920);
			int H = static_cast<int>(annotation_dimsW[i].second * 1920);
			cv::circle(img, cv::Point(W, H), 5, ispring::CV::GetRGB(labelsW[i]), CV_FILLED);
		}
		for (size_t i = 0; i < annotation_dimsH.size(); i++) {
			int W = static_cast<int>(annotation_dimsH[i].first * 1920);
			int H = static_cast<int>(annotation_dimsH[i].second * 1920);
			cv::circle(img, cv::Point(W, H), 5, ispring::CV::GetRGB(labelsH[i] + NUM_W), CV_FILLED);
		}
		for (size_t i = 0; i < centroidsW.size(); i++) {
			int W = static_cast<int>(centroidsW[i].first * 1920);
			int H = static_cast<int>(centroidsW[i].second * 1920);
			cv::line(img, cv::Point(0, H), cv::Point(W, H), cv::Scalar(255, 255, 255), 2);
			cv::line(img, cv::Point(W, 0), cv::Point(W, H), cv::Scalar(255, 255, 255), 2);
		}
		for (size_t i = 0; i < centroidsH.size(); i++) {
			int W = static_cast<int>(centroidsH[i].first * 1920);
			int H = static_cast<int>(centroidsH[i].second * 1920);
			cv::line(img, cv::Point(0, H), cv::Point(W, H), cv::Scalar(255, 255, 255), 2);
			cv::line(img, cv::Point(W, 0), cv::Point(W, H), cv::Scalar(255, 255, 255), 2);
		}

		cv::imwrite("Debug\\anchors(advanced).png", img);
	}
	if (is_yolov3 == false) {
		for (auto&e : centroidsW) {
			e.first *= WH / 32;
			e.second *= WH / 32;
		}
		for (auto&e : centroidsH) {
			e.first *= WH / 32;
			e.second *= WH / 32;
		}
	} else {
		for (auto&e : centroidsW) {
			e.first *= WH;
			e.second *= WH;
		}
		for (auto&e : centroidsH) {
			e.first *= WH;
			e.second *= WH;
		}
	}
	std::vector<std::pair<float, float>> centroids;
	for (auto&e : centroidsW) {
		centroids.push_back(e);
	}
	for (auto&e : centroidsH) {
		centroids.push_back(e);
	}
	std::ostringstream oss;
	oss.precision(3);
	for (int i = 0; i < centroids.size() - 1; i++) {
		oss << centroids[i].first << "," << centroids[i].second << ", ";
	}
	oss << centroids.back().first << "," << centroids.back().second;
	return oss.str();
}