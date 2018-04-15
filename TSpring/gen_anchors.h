#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
#include"Theme.h"
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
				annotation_dims.push_back(std::make_pair(w, h));
			}
			fin.close();
		}
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
		for (size_t i = 0; i < centroids.size(); i++) {
			int W = static_cast<int>(centroids[i].first * 1920);
			int H = static_cast<int>(centroids[i].second * 1920);
			cv::line(img, cv::Point(0, H), cv::Point(W, H), cv::Scalar(255, 255, 255), 2);
			cv::line(img, cv::Point(W, 0), cv::Point(W, H), cv::Scalar(255, 255, 255), 2);
		}
		for (size_t i = 0; i < annotation_dims.size(); i++) {
			int W = static_cast<int>(annotation_dims[i].first* 1920);
			int H = static_cast<int>(annotation_dims[i].second* 1920);
			cv::circle(img, cv::Point(W, H), 5, ispring::CV::GetRGB(labels[i]),CV_FILLED);
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