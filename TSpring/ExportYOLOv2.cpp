#include "stdafx.h"
#include "ExportYOLOv2.h"
std::string GetOriginalName(std::string path,std::string sdir,std::string ext) {
	std::string::size_type slash = path.find_last_of("/\\") + 1;
	std::string::size_type dot= path.find_last_of(".");
	std::string pure = path.substr(slash, dot - slash);
	std::string tmp = pure;
	std::string test = sdir + tmp + ext;
	int c = 1;
	while (ispring::File::FileExist(test) == true) {
		tmp =pure+ "(" + std::to_string(c) + ")";
		test = sdir + tmp + ext;
		c++;
	}
	for (size_t i = 0; i < tmp.size(); i++) {
		if (tmp[i] == ' ') {
			tmp[i] = '_';
		}
	}
	return tmp;
}
void GenerateCFG(ExportYOLOv2* yolov2,std::string sdir) {
	//make cfg
	int BATCH = 64;
	int SUBDIVISION = 64;
	int WIDTH = yolov2->GetWidth();
	int HEIGHT = yolov2->GetWidth();
	int MAX_BATCHES = static_cast<int>(std::max(60000.0, (g_export_class_data->size()*g_train_ratio) / 64.0*360.0));
	int CLASSES = yolov2->GetClasses();
	int NUM = yolov2->GetNumOfAnchors();
	int FILTERS = (CLASSES + 5)*NUM;
	int RANDOM = yolov2->GetRandom();
	char* cfg_format = yolov2->GetCFG().first;
	std::string cfg_name = yolov2->GetCFG().second;
	static char buffer[65536];
	std::vector<std::string> images = ispring::File::FileList(sdir + "train/", "*.jpg");
	std::string ANCHORS = GetAnchors(NUM, WIDTH, images);
	if (*g_is_dbg_info == true) {
		std::istringstream iss;
		iss.str(ANCHORS);

		cv::Mat img = cv::Mat::zeros(HEIGHT, WIDTH, CV_8UC3) + cv::Scalar(255, 255, 255);
		for (int i = 0; i < NUM; i++) {
			float a, b;
			char c;
			iss >> a >> c >> b >> c;
			cv::rectangle(img, cv::Rect(10 + i * 5, 10 + i * 5, a * 32, b * 32), ispring::CV::GetRGB(i));
		}
		cv::imwrite(sdir + "Debug\\anchors.png", img);

	}
	sprintf_s(buffer,65536, cfg_format, BATCH, SUBDIVISION, HEIGHT, WIDTH, MAX_BATCHES, FILTERS, ANCHORS.c_str(), CLASSES, NUM, RANDOM);
	///TODO
	std::fstream fout;	
	///CFG
	fout.open(sdir+cfg_name+".cfg", std::ios::out);
	fout << buffer;
	fout.close();
	///DATA
	std::string prj_name = mspring::String::ToString(g_project_name);
	fout.open(sdir + prj_name + ".data",std::ios::out);
	fout << "classes=" << CLASSES << std::endl;
	fout << "train=train.txt" << std::endl;
	fout << "valid=valid.txt" << std::endl;
	fout << "names=" << prj_name << ".names" << std::endl;
	fout << "backup=backup/";
	fout.close();
	///NAMES
	fout.open(sdir + prj_name + ".names",std::ios::out);
	for (auto&e : *g_export_class_data) {
		if (e.second == true) {
			fout << mspring::String::ToString(e.first) << std::endl;
		}
	}
	fout.close();
	///BAT
	fout.open(sdir + "train-" + cfg_name + ".bat", std::ios::out);
	fout << "cd \"bin\"" << std::endl;
	fout << "\"YOLOv2_Train_SE.exe\" ../ " << prj_name + ".data " << cfg_name + ".cfg" << std::endl;
	fout << "cd .." << std::endl;
	fout << "pause";
	fout.close();
}
std::vector<TagInfo> GetTagInfo(std::string img_path, std::vector<int> table,bool with_ignore=true) {
	std::vector<TagInfo> tag_data;
	std::string tsp_path = img_path.substr(0, img_path.find_last_of('.')) + ".tsp";
	std::fstream fin;
	fin.open(tsp_path, std::ios::in);
	tag_data.clear();
	if (fin.is_open() == true) {
		while (fin.eof() == false) {
			std::istringstream iss;
			std::string line;
			std::getline(fin, line);
			if (line.empty() == true) {
				continue;
			}
			iss.str(line);
			int _class;
			cv::RotatedRect rr;
			iss >> _class >> rr.center.x >> rr.center.y >> rr.size.width >> rr.size.height >> rr.angle;
			if (_class != -100) {
				if (_class >= table.size()) {	//클래스 리스트에 없는 클래스 무시
					continue;
				}
				if (table[_class] == -1) {	//export 하지 않는 클래스이면 무시
					continue;
				}
					tag_data.push_back(TagInfo(table[_class], rr));
			} else {
				if (with_ignore == true) {
					tag_data.push_back(TagInfo(_class, rr));
				}
			}
			
		}
	}
	return tag_data;
}
std::vector<int> GetClassTable() {
	std::vector<int> allow;
	int i = 0;
	for (size_t j = 0; j < g_export_class_data->size(); j++) {
		if (g_export_class_data->at(j).second == true) {
			allow.push_back(i);
			i++;
		} else {
			allow.push_back(-1);
		}
	}
	return allow;
}
std::vector<YOLOBOX> GetYOLOBOX(cv::Mat& img,std::vector<TagInfo> tag_info) {
	std::vector<YOLOBOX> yolobox;
	for (auto&e : tag_info) {
		if (*g_is_ignore == true && e.m_rect.angle != 0) {	//각도 무시일경우 angle이 있으면 무시
			continue;
		}
		if (e.m_class == -100) {
			continue;
		}
		cv::Rect2f rect = e.m_rect.boundingRect2f();
		YOLOBOX box;
		box.m_class = e.m_class;
		box.x = (rect.x + rect.width / 2.0F)/img.cols;
		box.y = (rect.y + rect.height / 2.0F)/img.rows;
		box.w = rect.width / img.cols;
		box.h = rect.height / img.rows;
		yolobox.push_back(box);
	}
	return yolobox;
}
std::vector<YOLOBOX> GetExistingYOLOBOX(std::string img_path) {
	std::string txt_path = img_path.substr(0, img_path.find_last_of('.')) + ".txt";
	std::fstream fin;
	std::vector<YOLOBOX> yolobox;
	fin.open(txt_path, std::ios::in);
	if (fin.is_open() == false) {
		ISPRING_VERIFY("File path error");
	} else {
		while (fin.eof() == false) {
			std::string line;
			std::getline(fin, line);
			if (line.length() == 0)break;
			std::istringstream iss;
			iss.str(line);
			YOLOBOX box;
			iss >> box.m_class >> box.x >> box.y >> box.w >> box.h;
			yolobox.push_back(box);
		}
		fin.close();
	}
	return yolobox;
}
void GenGrayImages(std::vector<std::string>& train_path,std::string dir) {
	std::vector<std::string> newdata;
	for (size_t i = 0; i < train_path.size(); i++) {
		cv::Mat img = cv::imread(train_path[i],cv::IMREAD_GRAYSCALE);
		std::vector<YOLOBOX> yolobox = GetExistingYOLOBOX(train_path[i]);
		std::string file_noext = GetOriginalName(train_path[i], dir, ".jpg");
		std::string file_img = file_noext + ".jpg";
		std::string file_txt = file_noext + ".txt";
		cv::imwrite(dir + file_img, img);
		std::fstream fout;
		fout.open(dir + file_txt, std::ios::out);
		if (fout.is_open() == false) {
			ISPRING_VERIFY("file open fail");
		} else {
			for (size_t j = 0; j < yolobox.size(); j++) {
				if (j != 0) {
					fout << std::endl;
				}
				fout << yolobox[j].m_class << " " << yolobox[j].x << " " << yolobox[j].y << " " << yolobox[j].w << " " << yolobox[j].h;
			}
			fout.close();
		}
		newdata.push_back(dir + file_img);
		g_progress = g_complete_images++ * 94 / g_total_images + 5;
	}
	for (auto&e : newdata) {
		train_path.push_back(e);
	}
}
void GenSaltNPepperImages(std::vector<std::string>& train_path, std::string dir) {
	std::vector<std::string> newdata;
	for (size_t i = 0; i < train_path.size(); i++) {
		cv::Mat img = cv::imread(train_path[i], cv::IMREAD_COLOR);

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
		std::vector<YOLOBOX> yolobox = GetExistingYOLOBOX(train_path[i]);
		std::string file_noext = GetOriginalName(train_path[i], dir, ".jpg");
		std::string file_img = file_noext + ".jpg";
		std::string file_txt = file_noext + ".txt";
		cv::imwrite(dir + file_img, img);
		std::fstream fout;
		fout.open(dir + file_txt, std::ios::out);
		if (fout.is_open() == false) {
			ISPRING_VERIFY("file open fail");
		} else {
			for (size_t j = 0; j < yolobox.size(); j++) {
				if (j != 0) {
					fout << std::endl;
				}
				fout << yolobox[j].m_class << " " << yolobox[j].x << " " << yolobox[j].y << " " << yolobox[j].w << " " << yolobox[j].h;
			}
			fout.close();
		}
		newdata.push_back(dir + file_img);
		g_progress = g_complete_images++ * 94 / g_total_images + 5;
	}
	for (auto&e : newdata) {
		train_path.push_back(e);
	}
}
void GenBlurImages(std::vector<std::string>& train_path, std::string dir) {
	std::vector<std::string> newdata;
	for (size_t i = 0; i < train_path.size(); i++) {
		cv::Mat img = cv::imread(train_path[i], cv::IMREAD_COLOR);
		cv::blur(img, img, cv::Size(5, 5));
		std::vector<YOLOBOX> yolobox = GetExistingYOLOBOX(train_path[i]);
		std::string file_noext = GetOriginalName(train_path[i], dir, ".jpg");
		std::string file_img = file_noext + ".jpg";
		std::string file_txt = file_noext + ".txt";
		cv::imwrite(dir + file_img, img);
		std::fstream fout;
		fout.open(dir + file_txt, std::ios::out);
		if (fout.is_open() == false) {
			ISPRING_VERIFY("file open fail");
		} else {
			for (size_t j = 0; j < yolobox.size(); j++) {
				if (j != 0) {
					fout << std::endl;
				}
				fout << yolobox[j].m_class << " " << yolobox[j].x << " " << yolobox[j].y << " " << yolobox[j].w << " " << yolobox[j].h;
			}
			fout.close();
		}
		newdata.push_back(dir + file_img);
		g_progress = g_complete_images++ * 94 / g_total_images + 5;
	}
	for (auto&e : newdata) {
		train_path.push_back(e);
	}
}
void GenFlipLRImages(std::vector<std::string>& train_path, std::string dir) {
	std::vector<std::string> newdata;
	for (size_t i = 0; i < train_path.size(); i++) {
		cv::Mat img = cv::imread(train_path[i], cv::IMREAD_COLOR);
		
		cv::flip(img, img, 1);

		std::vector<YOLOBOX> yolobox = GetExistingYOLOBOX(train_path[i]);
		for (auto&box : yolobox) {
			box.x = 1.0F - box.x;
		}

		std::string file_noext = GetOriginalName(train_path[i], dir, ".jpg");
		std::string file_img = file_noext + ".jpg";
		std::string file_txt = file_noext + ".txt";
		cv::imwrite(dir + file_img, img);
		std::fstream fout;
		fout.open(dir + file_txt, std::ios::out);
		if (fout.is_open() == false) {
			ISPRING_VERIFY("file open fail");
		} else {
			for (size_t j = 0; j < yolobox.size(); j++) {
				if (j != 0) {
					fout << std::endl;
				}
				fout << yolobox[j].m_class << " " << yolobox[j].x << " " << yolobox[j].y << " " << yolobox[j].w << " " << yolobox[j].h;
			}
			fout.close();
		}
		newdata.push_back(dir + file_img);
		g_progress = g_complete_images++ * 94 / g_total_images + 5;
	}
	for (auto&e : newdata) {
		train_path.push_back(e);
	}
}
void GenFlipTBImages(std::vector<std::string>& train_path, std::string dir) {
	std::vector<std::string> newdata;
	for (size_t i = 0; i < train_path.size(); i++) {
		cv::Mat img = cv::imread(train_path[i], cv::IMREAD_COLOR);

		cv::flip(img, img, 0);

		std::vector<YOLOBOX> yolobox = GetExistingYOLOBOX(train_path[i]);
		for (auto&box : yolobox) {
			box.y = 1.0F - box.y;
		}

		std::string file_noext = GetOriginalName(train_path[i], dir, ".jpg");
		std::string file_img = file_noext + ".jpg";
		std::string file_txt = file_noext + ".txt";
		cv::imwrite(dir + file_img, img);
		std::fstream fout;
		fout.open(dir + file_txt, std::ios::out);
		if (fout.is_open() == false) {
			ISPRING_VERIFY("file open fail");
		} else {
			for (size_t j = 0; j < yolobox.size(); j++) {
				if (j != 0) {
					fout << std::endl;
				}
				fout << yolobox[j].m_class << " " << yolobox[j].x << " " << yolobox[j].y << " " << yolobox[j].w << " " << yolobox[j].h;
			}
			fout.close();
		}
		newdata.push_back(dir + file_img);
		g_progress = g_complete_images++ * 94 / g_total_images + 5;
	}
	for (auto&e : newdata) {
		train_path.push_back(e);
	}
}
void _SaveCropImage(cv::Mat& img, cv::Rect rect,std::string dir,std::string name) {
	if (*g_is_dbg_info == true) {
		static size_t idx = 0;
		static std::string gdir = "";
		if (gdir.length() == 0) {
			gdir = dir;
		}
		if (gdir != dir) {
			idx = 0;
			gdir = dir;
		}
		//==begin
		std::string pure = name.substr(name.find_last_of("/\\") + 1);
		mspring::SetRange(rect.x, 0, img.cols - 1);
		mspring::SetRange(rect.y, 0, img.rows - 1);
		if (rect.x + rect.width >= img.cols) {
			rect.width = img.cols - rect.x - 1;
		}
		if (rect.y + rect.height >= img.rows) {
			rect.height = img.rows - rect.y - 1;
		}
		cv::Mat crop = img(rect).clone();
		std::ostringstream oss;
		oss.width(8);
		oss.fill('0');
		oss << idx++ << "_" << pure << ".jpg";
		cv::imwrite(gdir + oss.str(), crop);
	}
}
void GenImages(ExportYOLOv2* yolov2,std::string sdir) {
	std::string dir_train = sdir + "train\\";
	std::string dir_valid = sdir + "valid\\";
	std::string dir_backup = sdir + "backup\\";
	std::string dir_bin = sdir + "bin\\";
	ispring::File::DirectoryMake(dir_train);
	ispring::File::DirectoryMake(dir_valid);
	ispring::File::DirectoryMake(dir_backup);
	ispring::File::DirectoryMake(dir_bin);

	std::string dir_debug = sdir + "Debug\\";
	ispring::File::DirectoryMake(dir_debug);
	std::string dir_debug_crop = dir_debug + "crop\\";
	ispring::File::DirectoryMake(dir_debug_crop);

	ispring::Web::Download("https://www.dropbox.com/s/3ny3j6ab73scu2g/YOLOv2_Train_SE.exe?dl=1", dir_bin + "YOLOv2_Train_SE.exe");
	g_progress = 2;
	ispring::Web::Download("https://www.dropbox.com/s/7g9j1bwgietdiht/cudnn64_5.dll?dl=1", dir_bin + "cudnn64_5.dll");
	g_progress = 3;
	std::vector<int> table = GetClassTable();

	std::vector<std::string> img_original;
	for (size_t i = 0; i < g_image_data->size(); i++) {
		std::string img_path = mspring::String::ToString(g_image_data->at(i).first);
		std::vector<TagInfo> tags=GetTagInfo(img_path, table,false);
		if (tags.size() > 0) {
			img_original.push_back(img_path);
		}
	}
	g_progress = 4;
	std::vector<std::string> img_train, img_valid;
	{	///suffle
		srand(921126);
		for (size_t i = 0; i < img_original.size(); i++) {
			int idx = rand() % img_original.size();
			std::swap(img_original[i], img_original[idx]);
		}
	}
	g_progress = 5;
	int mul = 1;
	if (yolov2->m_chk_noise_gray->check == true) {
		mul *= 2;
	}
	if (yolov2->m_chk_noise_dot->check == true) {
		mul *= 2;
	}
	if (yolov2->m_chk_noise_blur->check == true) {
		mul *= 2;
	}
	if (yolov2->m_chk_noise_flipLR->check == true) {
		mul *= 2;
	}
	if (yolov2->m_chk_noise_flipTB->check == true) {
		mul *= 2;
	}
	
	
	{	///seperate train, valid
		size_t i = 0;
		for (; i < img_original.size()*(g_train_ratio / 100.0); i++) {
			img_train.push_back(img_original[i]);
		}
		for (; i < img_original.size(); i++) {
			img_valid.push_back(img_original[i]);
		}
	}
	g_total_images = mul*img_train.size() + img_valid.size();
	g_complete_images = 0;
	g_progress = g_complete_images*94 / g_total_images+5;
	std::vector<std::string> train_path;
	std::vector<std::string> valid_path;
	try{	///실제로 이미지를 추출(train)
		for (size_t i = 0; i < img_train.size(); i++) {
			cv::Mat img = cv::imread(img_train[i]);
			std::vector<TagInfo> tag = GetTagInfo(img_train[i], table);
			for (auto&_tag : tag) {	//ignore box 칠하기
				if (_tag.m_class == -100) {
					cv::rectangle(img, _tag.m_rect.boundingRect(), cv::Scalar(0, 0, 0),CV_FILLED);
				}
			}
			if (*g_is_rotate == true) {
				//angle은 무조건 양수
				for (size_t j = 0; j < 180; j++) {
					std::vector<TagInfo> rtag;
					for (size_t k = 0; k < tag.size(); k++) {
						if (tag[k].m_class != -100) {
							if (j == static_cast<int>(360 + tag[k].m_rect.angle) % 180) {
								rtag.push_back(tag[k]);
							}
						}
					}
					if (rtag.empty() == false) {
						std::ostringstream oss;
						oss.width(4);
						oss.fill('0');
						oss << j;
						cv::Mat rimg;
						cv::Point out;
						rimg = ispring::CV::ImageRotateOuter(img, static_cast<double>(j),&out);
						cv::Point mid(img.cols / 2, img.rows / 2);
						for (size_t k = 0; k < rtag.size(); k++) {
							cv::Point2f pts[4];
							std::vector<cv::Point> ptsi(4);
							rtag[k].m_rect.points(pts);
							for (size_t t = 0; t < 4; t++) {
								cv::Point2d after;
								double rad = -static_cast<double>(j)*CV_PI / 180.0;
								after.x = (pts[t].x - mid.x)*cos(rad) - (pts[t].y - mid.y)*sin(rad) + mid.x;
								after.y = (pts[t].x - mid.x)*sin(rad) + (pts[t].y - mid.y)*cos(rad) + mid.y;
								after.x += out.x;
								after.y += out.y;
								ptsi[t].x = static_cast<int>(after.x);
								ptsi[t].y = static_cast<int>(after.y);
							}
			
							cv::Rect rect=cv::boundingRect(ptsi);
							///SaveCropImage==============================================
							_SaveCropImage(rimg, rect, dir_debug_crop, img_train[i]);
							///========================================================
							rtag[k].m_rect.angle = 0;
							rtag[k].m_rect.center.x = rect.x + rect.width / 2.0F;
							rtag[k].m_rect.center.y = rect.y + rect.height / 2.0F;
							rtag[k].m_rect.size.width = static_cast<float>(rect.width);
							rtag[k].m_rect.size.height = static_cast<float>(rect.height);
							
							//cv::rectangle(rimg, rtag[k].m_rect.boundingRect(), cv::Scalar(0, 0, 255), 2);
						}

						std::vector<YOLOBOX> yolobox = GetYOLOBOX(rimg, rtag);
						std::string file_noext = GetOriginalName(img_train[i], dir_train, oss.str() + ".jpg") + oss.str();
						std::string file_img = file_noext + ".jpg";
						std::string file_txt = file_noext + ".txt";
						if (g_maximum_size != -1) {
							if (rimg.cols > g_maximum_size) {
								cv::resize(rimg, rimg, cv::Size(static_cast<int>(g_maximum_size), static_cast<int>(rimg.rows * g_maximum_size / rimg.cols)));
							}
						}
						cv::imwrite(dir_train + file_img, rimg);
						std::fstream fout;
						fout.open(dir_train + file_txt, std::ios::out);
						if (fout.is_open() == false) {
							ISPRING_VERIFY("file open fail");
						} else {
							for (size_t k = 0; k < yolobox.size(); k++) {
								if (k != 0) {
									fout << std::endl;
								}
								fout << yolobox[k].m_class << " " << yolobox[k].x << " " << yolobox[k].y << " " << yolobox[k].w << " " << yolobox[k].h;
							}
							fout.close();
						}
						train_path.push_back(dir_train + file_img);
					}
				}
			} else {
				///SaveCropImage==============================================
				for (size_t j = 0; j < tag.size(); j++) {
					if (tag[j].m_class != -100) {
						_SaveCropImage(img, tag[j].m_rect.boundingRect(), dir_debug_crop, img_train[i]);
					}
				}
				///========================================================
				std::vector<YOLOBOX> yolobox = GetYOLOBOX(img, tag);
				std::string file_noext = GetOriginalName(img_train[i], dir_train, ".jpg");
				std::string file_img = file_noext + ".jpg";
				std::string file_txt = file_noext + ".txt";
				if (g_maximum_size != -1) {
					if (img.cols > g_maximum_size) {
						cv::resize(img, img, cv::Size(static_cast<int>(g_maximum_size), static_cast<int>(img.rows * g_maximum_size / img.cols)));
					}
				}
				cv::imwrite(dir_train + file_img, img);
				std::fstream fout;
				fout.open(dir_train + file_txt, std::ios::out);
				if (fout.is_open() == false) {
					ISPRING_VERIFY("file open fail");
				} else {
					for (size_t j = 0; j < yolobox.size(); j++) {
						if (j != 0) {
							fout << std::endl;
						}
						fout << yolobox[j].m_class << " " << yolobox[j].x << " " << yolobox[j].y << " " << yolobox[j].w << " " << yolobox[j].h;
					}
					fout.close();
				}
				train_path.push_back(dir_train + file_img);
			}
			g_progress = g_complete_images++ * 94 / g_total_images + 5;
		}
		if (yolov2->m_chk_noise_gray->check == true) {
			GenGrayImages(train_path, dir_train);
		}
		if (yolov2->m_chk_noise_dot->check == true) {
			GenSaltNPepperImages(train_path, dir_train);
		}
		if (yolov2->m_chk_noise_blur->check == true) {
			GenBlurImages(train_path, dir_train);
		}
		if (yolov2->m_chk_noise_flipLR->check == true) {
			GenFlipLRImages(train_path, dir_train);
		}
		if (yolov2->m_chk_noise_flipTB->check == true) {
			GenFlipTBImages(train_path, dir_train);
		}
		std::fstream fout(sdir + "train.txt",std::ios::out);
		for(size_t i=0;i<train_path.size();i++){
			if (i != 0) {
				fout << std::endl;
			}
			std::string::size_type slash = train_path[i].find_last_of("\\/") + 1;
			std::string pure = train_path[i].substr(slash, train_path[i].length() - slash);
			pure = "train/" + pure;
			fout << pure;
		}
		fout.close();
	} catch (std::exception&e) {
		ISPRING_VERIFY(e.what());
	}
	try {
		for (size_t i = 0; i < img_valid.size(); i++) {
			cv::Mat img = cv::imread(img_valid[i]);
			std::vector<TagInfo> tag = GetTagInfo(img_valid[i], table);

			if (*g_is_rotate == true) {
				//angle은 무조건 양수
				for (size_t j = 0; j < 180; j++) {
					std::vector<TagInfo> rtag;
					for (size_t k = 0; k < tag.size(); k++) {
						if (tag[k].m_class != -100) {
							if (j == static_cast<int>(360 + tag[k].m_rect.angle) % 180) {
								rtag.push_back(tag[k]);
							}
						}
					}
					if (rtag.empty() == false) {
						std::ostringstream oss;
						oss.width(4);
						oss.fill('0');
						oss << j;
						cv::Mat rimg;
						cv::Point out;
						rimg = ispring::CV::ImageRotateOuter(img, static_cast<double>(j), &out);
						cv::Point mid(img.cols / 2, img.rows / 2);
						for (size_t k = 0; k < rtag.size(); k++) {
							cv::Point2f pts[4];
							std::vector<cv::Point> ptsi(4);
							rtag[k].m_rect.points(pts);
							for (size_t t = 0; t < 4; t++) {
								cv::Point2d after;
								double rad = -static_cast<double>(j)*CV_PI / 180.0;
								after.x = (pts[t].x - mid.x)*cos(rad) - (pts[t].y - mid.y)*sin(rad) + mid.x;
								after.y = (pts[t].x - mid.x)*sin(rad) + (pts[t].y - mid.y)*cos(rad) + mid.y;
								after.x += out.x;
								after.y += out.y;
								ptsi[t].x = static_cast<int>(after.x);
								ptsi[t].y = static_cast<int>(after.y);
							}

							cv::Rect rect = cv::boundingRect(ptsi);
							///SaveCropImage==============================================
							_SaveCropImage(rimg, rect, dir_debug_crop, img_train[i]);
							///========================================================
							rtag[k].m_rect.angle = 0;
							rtag[k].m_rect.center.x = rect.x + rect.width / 2.0F;
							rtag[k].m_rect.center.y = rect.y + rect.height / 2.0F;
							rtag[k].m_rect.size.width = static_cast<float>(rect.width);
							rtag[k].m_rect.size.height = static_cast<float>(rect.height);
						}

						std::vector<YOLOBOX> yolobox = GetYOLOBOX(rimg, rtag);
						std::string file_noext = GetOriginalName(img_valid[i], dir_valid, oss.str() + ".jpg") + oss.str();
						std::string file_img = file_noext + ".jpg";
						std::string file_txt = file_noext + ".txt";
						if (g_maximum_size != -1) {
							if (rimg.cols > g_maximum_size) {
								cv::resize(rimg, rimg, cv::Size(static_cast<int>(g_maximum_size), static_cast<int>(rimg.rows * g_maximum_size / rimg.cols)));
							}
						}
						cv::imwrite(dir_valid + file_img, rimg);
						std::fstream fout;
						fout.open(dir_valid + file_txt, std::ios::out);
						if (fout.is_open() == false) {
							ISPRING_VERIFY("file open fail");
						} else {
							for (size_t k = 0; k < yolobox.size(); k++) {
								if (k != 0) {
									fout << std::endl;
								}
								fout << yolobox[k].m_class << " " << yolobox[k].x << " " << yolobox[k].y << " " << yolobox[k].w << " " << yolobox[k].h;
							}
							fout.close();
						}
						valid_path.push_back(dir_valid + file_img);
					}
				}
			} else {
				///SaveCropImage==============================================
				for (size_t j = 0; j < tag.size(); j++) {
					if (tag[j].m_class != -100) {
						_SaveCropImage(img, tag[j].m_rect.boundingRect(), dir_debug_crop, img_train[i]);
					}
				}
				///========================================================
				std::vector<YOLOBOX> yolobox = GetYOLOBOX(img, tag);
				std::string file_noext = GetOriginalName(img_valid[i], dir_valid, ".jpg");
				std::string file_img = file_noext + ".jpg";
				std::string file_txt = file_noext + ".txt";
				if (g_maximum_size != -1) {
					if (img.cols > g_maximum_size) {
						cv::resize(img, img, cv::Size(g_maximum_size, img.rows * g_maximum_size / img.cols));
					}
				}
				cv::imwrite(dir_valid + file_img, img);
				std::fstream fout;
				fout.open(dir_valid + file_txt, std::ios::out);
				if (fout.is_open() == false) {
					ISPRING_VERIFY("file open fail");
				} else {
					for (size_t j = 0; j < yolobox.size(); j++) {
						if (j != 0) {
							fout << std::endl;
						}
						fout << yolobox[j].m_class << " " << yolobox[j].x << " " << yolobox[j].y << " " << yolobox[j].w << " " << yolobox[j].h;
					}
					fout.close();
				}
				valid_path.push_back(dir_valid + file_img);
			}
			g_progress = g_complete_images++ * 94 / g_total_images + 5;
		}
		std::fstream fout(sdir + "valid.txt", std::ios::out);
		for (size_t i = 0; i<valid_path.size(); i++) {
			if (i != 0) {
				fout << std::endl;
			}
			std::string::size_type slash = valid_path[i].find_last_of("\\/") + 1;
			std::string pure = valid_path[i].substr(slash, valid_path[i].length() - slash);
			pure = "valid/" + pure;
			fout << pure;
		}
		fout.close();
	} catch (std::exception&e) {
		ISPRING_VERIFY(e.what());
	}
	g_complete_images = g_total_images;
	GenerateCFG(yolov2, sdir);

	g_progress = 100;
}
UINT ExportThread(void* param) {
	ExportYOLOv2* yolov2 = reinterpret_cast<ExportYOLOv2*>(param);
	std::vector<std::pair<CString, bool>>& _images = *g_image_data;
	//========================================
	std::string sdir = mspring::String::ToString(g_export_dir);
	if (sdir.back() != '/' && sdir.back() != '\\') {
		sdir.push_back('\\');
	}
	ispring::File::DirectoryMake(sdir);
	//==============================
	g_progress = 1;

	GenImages(yolov2, sdir);
	
	g_exporting = false;
	//::MessageBoxA(NULL, "Finish", "Success", MB_OK);
	return 1;
}
void ExportYOLOv2::Export() {
	g_progress = 0;
	g_exporting = true;
	AfxGetMainWnd()->SetTimer(7777, 20,nullptr);
	::AfxBeginThread(ExportThread, (void*)this);
}

ExportYOLOv2::ExportYOLOv2(CWnd* wnd):VirtualView(wnd) {
	int base = 160;
	int text_width = 100;
	m_stc_basemodel = new MStatic(wnd, MRect(MRectPosition::LT, base, 10, text_width+30, 30));
	m_stc_basemodel->m_color_text = &g_solarized_yellow;
	m_stc_basemodel->m_text = TEXT("Base model");

	m_chk_darknet_r = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 70, 25, 25));
	m_chk_darknet_r->m_color_text = &g_lv_color_text_white;
	m_stc_darknet_r = new MStatic(wnd, MRect(MRectPosition::LT, base+30, 70, text_width, 25));
	m_stc_darknet_r->m_color_text = &g_lv_color_text;
	m_stc_darknet_r->m_text = TEXT("Darknet-R");

	m_chk_darknet19 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 130, 25, 25));
	m_chk_darknet19->m_color_text = &g_lv_color_text_white;
	m_stc_darknet19 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 130, text_width, 25));
	m_stc_darknet19->m_color_text = &g_lv_color_text;
	m_stc_darknet19->m_text = TEXT("Darknet19");

	m_chk_resnet50 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 190, 25, 25));
	m_chk_resnet50->m_color_text = &g_lv_color_text_white;
	m_stc_resnet50 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 190, text_width, 25));
	m_stc_resnet50->m_color_text = &g_lv_color_text;
	m_stc_resnet50->m_text = TEXT("Resnet50");

	m_chk_densenet201 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 250, 25, 25));
	m_chk_densenet201->m_color_text = &g_lv_color_text_white;
	m_stc_densenet201 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 250, text_width, 25));
	m_stc_densenet201->m_color_text = &g_lv_color_text;
	m_stc_densenet201->m_text = TEXT("Densenet201");

	m_base_model.push_back(m_chk_darknet_r);
	m_base_model.push_back(m_chk_darknet19);
	m_base_model.push_back(m_chk_resnet50);
	m_base_model.push_back(m_chk_densenet201);
	
	m_objs.push_back(m_stc_basemodel);
	m_objs.push_back(m_chk_darknet_r);
	m_objs.push_back(m_stc_darknet_r);
	m_objs.push_back(m_chk_darknet19);
	m_objs.push_back(m_stc_darknet19);
	m_objs.push_back(m_chk_resnet50);
	m_objs.push_back(m_stc_resnet50);
	m_objs.push_back(m_chk_densenet201);
	m_objs.push_back(m_stc_densenet201);
	//=================ANCHORS
	base = 310;
	m_stc_anchors = new MStatic(wnd, MRect(MRectPosition::LT, base, 10, text_width+30, 30));
	m_stc_anchors->m_color_text = &g_solarized_yellow;
	m_stc_anchors->m_text = TEXT("Anchors");

	m_chk_anchor3 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 70, 25, 25));
	m_chk_anchor3->m_color_text = &g_lv_color_text_white;
	m_stc_anchor3 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 70, text_width, 25));
	m_stc_anchor3->m_color_text = &g_lv_color_text;
	m_stc_anchor3->m_text = TEXT("3 anchors");

	m_chk_anchor5 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 130, 25, 25));
	m_chk_anchor5->m_color_text = &g_lv_color_text_white;
	m_stc_anchor5 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 130, text_width, 25));
	m_stc_anchor5->m_color_text = &g_lv_color_text;
	m_stc_anchor5->m_text = TEXT("5 anchor");

	m_chk_anchor7 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 190, 25, 25));
	m_chk_anchor7->m_color_text = &g_lv_color_text_white;
	m_stc_anchor7 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 190, text_width, 25));
	m_stc_anchor7->m_color_text = &g_lv_color_text;
	m_stc_anchor7->m_text = TEXT("7 anchors");

	m_chk_anchor9 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 250, 25, 25));
	m_chk_anchor9->m_color_text = &g_lv_color_text_white;
	m_stc_anchor9 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 250, text_width, 25));
	m_stc_anchor9->m_color_text = &g_lv_color_text;
	m_stc_anchor9->m_text = TEXT("9 anchors");

	m_chk_anchor11 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 310, 25, 25));
	m_chk_anchor11->m_color_text = &g_lv_color_text_white;
	m_stc_anchor11 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 310, text_width, 25));
	m_stc_anchor11->m_color_text = &g_lv_color_text;
	m_stc_anchor11->m_text = TEXT("11 anchors");
	m_anchors.push_back(m_chk_anchor3);
	m_anchors.push_back(m_chk_anchor5);
	m_anchors.push_back(m_chk_anchor7);
	m_anchors.push_back(m_chk_anchor9);
	m_anchors.push_back(m_chk_anchor11);
	m_objs.push_back(m_stc_anchors);
	m_objs.push_back(m_chk_anchor3);
	m_objs.push_back(m_stc_anchor3);
	m_objs.push_back(m_chk_anchor5);
	m_objs.push_back(m_stc_anchor5);
	m_objs.push_back(m_chk_anchor7);
	m_objs.push_back(m_stc_anchor7);
	m_objs.push_back(m_chk_anchor9);
	m_objs.push_back(m_stc_anchor9);
	m_objs.push_back(m_chk_anchor11);
	m_objs.push_back(m_stc_anchor11);
	//==============RESOLUTION
	base = 460;
	m_stc_resolution = new MStatic(wnd, MRect(MRectPosition::LT, base, 10, text_width+30, 30));
	m_stc_resolution->m_color_text = &g_solarized_yellow;
	m_stc_resolution->m_text = TEXT("Resolution");
	m_chk_resolution416 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 70, 25, 25));
	m_chk_resolution416->m_color_text = &g_lv_color_text_white;
	m_stc_resolution416 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 70, text_width, 25));
	m_stc_resolution416->m_color_text = &g_lv_color_text;
	m_stc_resolution416->m_text = TEXT("416x416");

	m_chk_resolution480 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 130, 25, 25));
	m_chk_resolution480->m_color_text = &g_lv_color_text_white;
	m_stc_resolution480 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 130, text_width, 25));
	m_stc_resolution480->m_color_text = &g_lv_color_text;
	m_stc_resolution480->m_text = TEXT("480x480");

	m_chk_resolution544 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 190, 25, 25));
	m_chk_resolution544->m_color_text = &g_lv_color_text_white;
	m_stc_resolution544 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 190, text_width, 25));
	m_stc_resolution544->m_color_text = &g_lv_color_text;
	m_stc_resolution544->m_text = TEXT("544x544");

	m_chk_resolution608 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 250, 25, 25));
	m_chk_resolution608->m_color_text = &g_lv_color_text_white;
	m_stc_resolution608 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 250, text_width, 25));
	m_stc_resolution608->m_color_text = &g_lv_color_text;
	m_stc_resolution608->m_text = TEXT("608x608");

	m_chk_resolution1088 = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 310, 25, 25));
	m_chk_resolution1088->m_color_text = &g_lv_color_text_white;
	m_stc_resolution1088 = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 310, text_width, 25));
	m_stc_resolution1088->m_color_text = &g_lv_color_text;
	m_stc_resolution1088->m_text = TEXT("1088x1088");

	m_chk_resolution_random = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 370, 25, 25));
	m_chk_resolution_random->m_color_text = &g_lv_color_text_white;
	m_stc_resolution_random = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 370, text_width, 25));
	m_stc_resolution_random->m_color_text = &g_lv_color_text;
	m_stc_resolution_random->m_text = TEXT("Random size");

	m_resolution.push_back(m_chk_resolution416);
	m_resolution.push_back(m_chk_resolution480);
	m_resolution.push_back(m_chk_resolution544);
	m_resolution.push_back(m_chk_resolution608);
	m_resolution.push_back(m_chk_resolution1088);
	m_resolution.push_back(m_chk_resolution_random);
	m_objs.push_back(m_stc_resolution);
	m_objs.push_back(m_chk_resolution416);
	m_objs.push_back(m_stc_resolution416);
	m_objs.push_back(m_chk_resolution480);
	m_objs.push_back(m_stc_resolution480);
	m_objs.push_back(m_chk_resolution544);
	m_objs.push_back(m_stc_resolution544);
	m_objs.push_back(m_chk_resolution608);
	m_objs.push_back(m_stc_resolution608);
	m_objs.push_back(m_chk_resolution1088);
	m_objs.push_back(m_stc_resolution1088);
	m_objs.push_back(m_chk_resolution_random);
	m_objs.push_back(m_stc_resolution_random);
	//=====NOISE
	base = 610;
	m_stc_noise = new MStatic(wnd, MRect(MRectPosition::LT, base, 10, text_width+30, 30));
	m_stc_noise->m_color_text = &g_solarized_yellow;
	m_stc_noise->m_text = TEXT("Noise");
	m_chk_noise_gray = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 70, 25, 25));
	m_chk_noise_gray->m_color_text = &g_lv_color_text_white;
	m_stc_noise_gray = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 70, 150, 25));
	m_stc_noise_gray->m_color_text = &g_lv_color_text;
	m_stc_noise_gray->m_text = TEXT("Gray");

	m_chk_noise_dot = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 130, 25, 25));
	m_chk_noise_dot->m_color_text = &g_lv_color_text_white;
	m_stc_noise_dot = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 130, 150, 25));
	m_stc_noise_dot->m_color_text = &g_lv_color_text;
	m_stc_noise_dot->m_text = TEXT("Salt&Pepper");

	m_chk_noise_flipLR = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 190, 25, 25));
	m_chk_noise_flipLR->m_color_text = &g_lv_color_text_white;
	m_stc_noise_flipLR = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 190, 150, 25));
	m_stc_noise_flipLR->m_color_text = &g_lv_color_text;
	m_stc_noise_flipLR->m_text = TEXT("Flip(LR)");

	m_chk_noise_flipTB = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 250, 25, 25));
	m_chk_noise_flipTB->m_color_text = &g_lv_color_text_white;
	m_stc_noise_flipTB = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 250, 150, 25));
	m_stc_noise_flipTB->m_color_text = &g_lv_color_text;
	m_stc_noise_flipTB->m_text = TEXT("Flip(TB)");

	m_chk_noise_blur = new MButtonCheck(wnd, MRect(MRectPosition::LT, base, 310, 25, 25));
	m_chk_noise_blur->m_color_text = &g_lv_color_text_white;
	m_stc_noise_blur = new MStatic(wnd, MRect(MRectPosition::LT, base + 30, 310, 150, 25));
	m_stc_noise_blur->m_color_text = &g_lv_color_text;
	m_stc_noise_blur->m_text = TEXT("Blur");
	
	m_noise.push_back(m_chk_noise_gray);
	m_noise.push_back(m_chk_noise_dot);
	m_noise.push_back(m_chk_noise_flipLR);
	m_noise.push_back(m_chk_noise_flipTB);
	m_noise.push_back(m_chk_noise_blur);
	m_objs.push_back(m_stc_noise);
	m_objs.push_back(m_chk_noise_gray);
	m_objs.push_back(m_stc_noise_gray);
	m_objs.push_back(m_chk_noise_dot);
	m_objs.push_back(m_stc_noise_dot);
	m_objs.push_back(m_chk_noise_flipLR);
	m_objs.push_back(m_stc_noise_flipLR);
	m_objs.push_back(m_chk_noise_flipTB);
	m_objs.push_back(m_stc_noise_flipTB);
	m_objs.push_back(m_chk_noise_blur);
	m_objs.push_back(m_stc_noise_blur);
	//INIT
	m_chk_darknet19->check = true;
	m_chk_anchor7->check = true;
	m_chk_resolution416->check = true;
}
ExportYOLOv2::~ExportYOLOv2() {
}
int ExportYOLOv2::OnCreate() {
	VirtualView::OnCreate();
	return 1;
}
void ExportYOLOv2::OnDestroy() {
	VirtualView::OnDestroy();
}

void ExportYOLOv2::OnPaint(CDC* pDC) {
	VirtualView::OnPaint(pDC);
	for (auto&obj : m_objs) {
		obj->OnPaint(pDC);
	}

}
void ExportYOLOv2::OnSetFocus(CWnd* pOldWnd) {
	VirtualView::OnSetFocus(pOldWnd);
}
void ExportYOLOv2::OnKillFocus(CWnd* pNewWnd) {
	VirtualView::OnKillFocus(pNewWnd);
}

void ExportYOLOv2::OnLButtonDown(UINT nFlags, CPoint point) {
	VirtualView::OnLButtonDown(nFlags, point);
	for (size_t i = 0; i < m_base_model.size(); i++) {
		if (m_base_model[i]->OnLButtonDown() == M_CLICKED) {
			for (size_t j = 0; j < m_base_model.size(); j++) {
				dynamic_cast<MButtonCheck*>(m_base_model[j])->check = false;
			}
			dynamic_cast<MButtonCheck*>(m_base_model[i])->check = true;
		}
	}
	for (size_t i = 0; i < m_anchors.size(); i++) {
		if (m_anchors[i]->OnLButtonDown() == M_CLICKED) {
			for (size_t j = 0; j < m_anchors.size(); j++) {
				dynamic_cast<MButtonCheck*>(m_anchors[j])->check = false;
			}
			dynamic_cast<MButtonCheck*>(m_anchors[i])->check = true;
		}
	}
	m_chk_resolution_random->disable = m_chk_resnet50->check;
	if (m_chk_resolution_random->disable == true) {
		m_chk_resolution_random->check = false;
		bool check = false;
		for (size_t j = 0; j < m_resolution.size(); j++) {
			check|=dynamic_cast<MButtonCheck*>(m_resolution[j])->check;
		}
		if (check == false) {
			m_chk_resolution416->check = true;
		}
	}
	for (size_t i = 0; i < m_resolution.size(); i++) {
		if (m_resolution[i]->OnLButtonDown() == M_CLICKED) {
			for (size_t j = 0; j < m_resolution.size(); j++) {
				dynamic_cast<MButtonCheck*>(m_resolution[j])->check = false;
			}
			dynamic_cast<MButtonCheck*>(m_resolution[i])->check = true;
		}
	}
	for (size_t i = 0; i < m_noise.size(); i++) {
		m_noise[i]->OnLButtonDown();
	}
	
}
void ExportYOLOv2::OnLButtonUp(UINT nFlags, CPoint point) {
	VirtualView::OnLButtonUp(nFlags, point);
}
void ExportYOLOv2::OnLButtonDblClk(UINT nFlags, CPoint point) {
	VirtualView::OnLButtonDblClk(nFlags, point);
}
void ExportYOLOv2::OnRButtonUp(UINT nFlags, CPoint point) {
	VirtualView::OnRButtonUp(nFlags, point);
}
BOOL ExportYOLOv2::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	VirtualView::OnMouseWheel(nFlags, zDelta, pt);
	return TRUE;
}
void ExportYOLOv2::OnMouseMove(UINT nFlags, CPoint point) {
	VirtualView::OnMouseMove(nFlags, point);
	for (auto&obj : m_objs) {
		obj->OnMouseMove();
	}
}
void ExportYOLOv2::OnMouseLeave() {
	VirtualView::OnMouseLeave();
}

BOOL ExportYOLOv2::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
	VirtualView::OnSetCursor(pWnd, nHitTest, message);
	return TRUE;
}
void ExportYOLOv2::OnSize(UINT nType, int cx, int cy) {
	VirtualView::OnSize(nType, cx, cy);
}

void ExportYOLOv2::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	VirtualView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void ExportYOLOv2::OnTimer(UINT_PTR nIDEvent) {
	VirtualView::OnTimer(nIDEvent);
}
void ExportYOLOv2::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	VirtualView::OnChar(nChar, nRepCnt, nFlags);
}
LRESULT ExportYOLOv2::OnComposition(WPARAM wParam, LPARAM lParam) {
	VirtualView::OnComposition(wParam, lParam);
	return 1;
}

