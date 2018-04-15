#include"stdafx.h"
#include"DataArgumentation.h"
std::string Generate_Gray(std::string filename, DAType type) {
	cv::Mat img = cv::imread(filename, 0);
	cv::cvtColor(img, img, CV_GRAY2BGR);
	std::string pure = filename.substr(0, filename.length() - 4);
	std::string old_txtfile = pure + ".txt";
	std::string new_imgfile = pure + "_GS" + ".jpg";
	std::string new_txtfile = pure + "_GS" + ".txt";
	cv::imwrite(new_imgfile, img);
	if (type == DAType::YOLO) {
		ispring::File::FileCopy(old_txtfile, new_txtfile);
	}
	return new_imgfile;
}
std::string Generate_SaltAndPepper(std::string filename, DAType type) {
	cv::Mat img = cv::imread(filename);
	int num_of_noise_pixels = (int)((double)(img.rows * img.cols * 3)*0.003);
	for (int i = 0; i < num_of_noise_pixels; i++) {
		int r = rand() % img.rows;  // noise로 바꿀 행을 임의로 선택
		int c = rand() % img.cols;  // noise로 바꿀 열을 임의로 선택
		if (rand() % 2) {
			img.at<cv::Vec3b>(r, c) = cv::Vec3b(255, 255, 255);
		} else {
			img.at<cv::Vec3b>(r, c) = cv::Vec3b(0, 0, 0);
		}
	}
	std::string pure = filename.substr(0, filename.length() - 4);
	std::string old_txtfile = pure + ".txt";
	std::string new_imgfile = pure + "_SP" + ".jpg";
	std::string new_txtfile = pure + "_SP" + ".txt";
	cv::imwrite(new_imgfile, img);
	if (type == DAType::YOLO) {
		ispring::File::FileCopy(old_txtfile, new_txtfile);
	}
	return new_imgfile;
}
std::string Generate_FilpLR(std::string filename, DAType type) {
	cv::Mat img = cv::imread(filename);
	cv::flip(img, img, 1);
	std::string pure = filename.substr(0, filename.length() - 4);
	std::string old_txtfile = pure + ".txt";
	std::string new_imgfile = pure + "_FLR" + ".jpg";
	std::string new_txtfile = pure + "_FLR" + ".txt";
	cv::imwrite(new_imgfile, img);
	if (type == DAType::YOLO) {
		std::fstream fin, fout;
		fin.open(old_txtfile, std::ios::in);
		fout.open(new_txtfile, std::ios::out);
		while (fin.eof() == false) {
			std::string line;
			std::getline(fin, line);
			if (line.length() == 0)break;
			std::istringstream iss;
			iss.str(line);
			int c;
			float x, y, w, h;
			iss >> c >> x >> y >> w >> h;
			fout << c << " " << (1.0F - x) << " " << y << " " << w << " " << h << std::endl;
		}
		fin.close();
		fout.close();
		
	}
	return new_imgfile;
}
std::string Generate_FilpTB(std::string filename, DAType type) {
	cv::Mat img = cv::imread(filename);
	cv::flip(img, img, 0);
	std::string pure = filename.substr(0, filename.length() - 4);
	std::string old_txtfile = pure + ".txt";
	std::string new_imgfile = pure + "_FTB" + ".jpg";
	std::string new_txtfile = pure + "_FTB" + ".txt";
	cv::imwrite(new_imgfile, img);
	if (type == DAType::YOLO) {
		std::fstream fin, fout;
		fin.open(old_txtfile, std::ios::in);
		fout.open(new_txtfile, std::ios::out);
		while (fin.eof() == false) {
			std::string line;
			std::getline(fin, line);
			if (line.length() == 0)break;
			std::istringstream iss;
			iss.str(line);
			int c;
			float x, y, w, h;
			iss >> c >> x >> y >> w >> h;
			fout << c << " " << x << " " << (1.0F - y) << " " << w << " " << h << std::endl;
		}
		fin.close();
		fout.close();
		
	} 
	return new_imgfile;
}
std::string Generate_Slip(std::string filename, DAType type) {
	cv::Mat img = cv::imread(filename);
	cv::Mat out = cv::Mat::zeros(img.size(), img.type());
	img(cv::Rect(1, 0, img.cols - 1, img.rows)).copyTo(out(cv::Rect(0, 0, img.cols - 1, img.rows)));
	img = out&img;
	std::string pure = filename.substr(0, filename.length() - 4);
	std::string old_txtfile = pure + ".txt";
	std::string new_imgfile = pure + "_SL" + ".jpg";
	std::string new_txtfile = pure + "_SL" + ".txt";
	cv::imwrite(new_imgfile, img);
	if (type == DAType::YOLO) {
		ispring::File::FileCopy(old_txtfile, new_txtfile);
		
	} 
	return new_imgfile;
}
std::string Generate_Blur(std::string filename, DAType type) {
	cv::Mat img = cv::imread(filename);
	cv::blur(img, img, cv::Size(3, 3));
	std::string pure = filename.substr(0, filename.length() - 4);
	std::string old_txtfile = pure + ".txt";
	std::string new_imgfile = pure + "_BR" + ".jpg";
	std::string new_txtfile = pure + "_BR" + ".txt";
	cv::imwrite(new_imgfile, img);
	if (type == DAType::YOLO) {
		ispring::File::FileCopy(old_txtfile, new_txtfile);
		
	} 
	return new_imgfile;
}
void GammaCorrection(cv::Mat& src, cv::Mat& dst, float fGamma) {
	unsigned char lut[256];
	for (int i = 0; i < 256; i++) {
		lut[i] = cv::saturate_cast<uchar>(pow((float)(i / 255.0), fGamma) * 255.0f);
	}
	dst = src.clone();
	const int channels = dst.channels();
	switch (channels) {
		case 1:
		{
			cv::MatIterator_<uchar> it, end;
			for (it = dst.begin<uchar>(), end = dst.end<uchar>(); it != end; it++)
				*it = lut[(*it)];
			break;
		}
		case 3:
		{
			cv::MatIterator_<cv::Vec3b> it, end;
			for (it = dst.begin<cv::Vec3b>(), end = dst.end<cv::Vec3b>(); it != end; it++) {
				(*it)[0] = lut[((*it)[0])];
				(*it)[1] = lut[((*it)[1])];
				(*it)[2] = lut[((*it)[2])];
			}
			break;
		}
	}
}
std::string Generate_GammaCorrection1(std::string filename, DAType type) {
	cv::Mat in = cv::imread(filename);
	cv::Mat img;
	GammaCorrection(in, img, 0.5F);
	std::string pure = filename.substr(0, filename.length() - 4);
	std::string old_txtfile = pure + ".txt";
	std::string new_imgfile = pure + "_GC1" + ".jpg";
	std::string new_txtfile = pure + "_GC1" + ".txt";
	cv::imwrite(new_imgfile, img);
	if (type == DAType::YOLO) {
		ispring::File::FileCopy(old_txtfile, new_txtfile);
		
	} 
	return new_imgfile;
}
std::string Generate_GammaCorrection2(std::string filename, DAType type) {
	cv::Mat in = cv::imread(filename);
	cv::Mat img;
	GammaCorrection(in, img, 1.5F);
	std::string pure = filename.substr(0, filename.length() - 4);
	std::string old_txtfile = pure + ".txt";
	std::string new_imgfile = pure + "_GC2" + ".jpg";
	std::string new_txtfile = pure + "_GC2" + ".txt";
	cv::imwrite(new_imgfile, img);
	if (type == DAType::YOLO) {
		ispring::File::FileCopy(old_txtfile, new_txtfile);
		
	} 
	return new_imgfile;
}