#pragma once
#include<deque>
#include<omp.h>
#include"MSpring/MSpring.h"
#include"Theme.h"
#include"ispring/All.h"
#include "ExcelFormat.h"
#include"gen_anchors.h"


#include"cfg_darknet_reference.h"
#include"cfg_darknet19.h"
#include"cfg_densenet201.h"
#include"cfg_resnet50.h"
#include"cfg_darknet53.h"

#include"DataArgumentation.h"
#include"TSpringMsgBox.h"
struct YOLOBOX {
	int m_class;
	float x;
	float y;
	float w;
	float h;
};
struct RCNNBOX {
	int m_class;
	int x1, y1, x2, y2;
};
class ExportView :
	public VirtualView {
public:
	ExportView(CWnd* wnd);
	~ExportView();
public:
	MListBox m_list_class;
	bool isSameClassList() {
		if (m_list_class->m_data.size() != GetApp().g_class_data->size()) {
			return false;
		}
		for (size_t i = 0; i < m_list_class->m_data.size(); i++) {
			if (m_list_class->m_data[i].first != GetApp().g_class_data->at(i).first) {
				return false;
			}
		}
		return true;
	}
	MStatic m_stc_title_architecture;
	MButtonCheck m_chk_yolo;
	MStatic m_stc_yolo;
	MButtonCheck m_chk_fasterrcnn;
	MStatic m_stc_fasterrcnn;
	MButtonCheck m_chk_dlibcnn;
	MStatic m_stc_dlibcnn;
	MButtonCheck m_chk_dlibsvm;
	MStatic m_stc_dlibsvm;
	///Base Model
	MStatic m_stc_title_basemodel;
	std::vector<TString> m_str_models;
	std::vector<MButtonCheck> m_chk_models;
	std::vector<MStatic> m_stc_models;
	///Options
	MStatic m_stc_title_option;
	std::vector<TString> m_str_option;
	std::vector<MButtonCheck> m_chk_option;
	std::vector<MStatic> m_stc_option;
	int GetNumOfAnchors() {
		for (int i = 3; i < 21; i += 2) {
			OStringStream oss;
			oss << i << " anchors";
			auto ptr = GetChkBtnByString(m_chk_option, m_stc_option, oss.str());
			if (ptr!=nullptr && ptr->check) {
				return i;
			}
		}
		return 0;
	}
	///Size
	MStatic m_stc_title_size;
	std::vector<TString> m_str_size;
	std::vector<MButtonCheck> m_chk_size;
	std::vector<MStatic> m_stc_size;
	int GetResolution() {
		//if (GetChkBtnByString(m_chk_size, m_stc_size, TEXT("Random"))->check) {
		//	return 416;
		//}
		for (int i = 288; i <= 1088; i += 32) {
			OStringStream oss;
			oss << i << "x" << i;
			auto ptr = GetChkBtnByString(m_chk_size, m_stc_size, oss.str());
			if (ptr != nullptr && ptr->check) {
				return i;
			}
		}
		return 0;
	}
	int GetExportSize() {
		size_t i = 0;
		int arr[] = { 352,416,480,544,608,850,1088,608 };
		for (i = 0; i < m_chk_size.size(); i++) {
			if (m_chk_size[i]->check == true) break;
		}
		if (m_chk_export_origin->check == true) {
			return -1;
		} else {
			return arr[i];
		}
	}
	std::pair<char*, std::string> GetCFG() {
		static char tmp[65536];
		std::pair<char*, std::string> cfg;
		if (GetChkBtnByString(m_chk_models, m_stc_models, TEXT("DarknetREF"))->check == true) {
			cfg.first = cfg_darknetR;
			cfg.second = "darknetREF";
		} else if (GetChkBtnByString(m_chk_models, m_stc_models, TEXT("Darknet19"))->check == true) {
			cfg.first = cfg_darknet19;
			cfg.second = "darknet19";
		} else if (GetChkBtnByString(m_chk_models, m_stc_models, TEXT("Resnet50"))->check == true) {
			cfg.first = cfg_resnet50;
			cfg.second = "resnet50";
		} else if (GetChkBtnByString(m_chk_models, m_stc_models, TEXT("Densenet201"))->check == true) {
			memset(tmp, 0, 65536);
			strcpy_s(tmp, 65535, cfg_densenet201_part1);
			strcat_s(tmp, 65535, cfg_densenet201_part2);
			cfg.first = tmp;
			cfg.second = "densenet201";
		} else if (GetChkBtnByString(m_chk_models, m_stc_models, TEXT("Darknet53"))->check == true) {
			cfg.first = cfg_darknet53;
			cfg.second = "darknet53";
		}
		return cfg;
	}
	///Noise
	MStatic m_stc_title_noise;
	std::vector<TString> m_str_noise;
	std::vector<MButtonCheck> m_chk_noise;
	std::vector<MStatic> m_stc_noise;

	MButtonCheck GetChkBtnByString(std::vector<MButtonCheck>& chk, std::vector<MStatic>& stc, TString str) {
		for (size_t i = 0; i < chk.size(); i++) {
			if (stc[i]->m_text == str) {
				return chk[i];
			}
		}
		return nullptr;
	}
	MButtonCheck m_chk_rotate;
	MStatic m_stc_rotate;
	MButtonCheck m_chk_boundingbox;
	MStatic m_stc_boundingbox;
	MButtonCheck m_chk_ignore_rbox;
	MStatic m_stc_ignore_rbox;
	MButtonCheck m_chk_debug_info;
	MStatic m_stc_debug_info;
	MButtonCheck m_chk_export_origin;
	MStatic m_stc_export_origin;
	MButtonCheck m_chk_advanced_anchors;
	MStatic m_stc_advanced_anchors;

	MSingleEdit m_edit_name;
	MButton m_btn_export;
	MStatic m_stc_name;
	static void DownloadFile(std::string url, std::string file) {
		std::ostringstream oss;
		oss << "powershell \"[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; (New-Object System.Net.WebClient).DownloadFile(\'"
			<< url << "\',\'" << file << "\')\"";

		STARTUPINFOA si;
		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		PROCESS_INFORMATION pi;
		char buffer[1024] = { 0 };
		strcpy_s(buffer, 1024, oss.str().c_str());
		
		CreateProcessA(
			NULL,buffer , NULL, NULL,
			TRUE, 0, NULL, NULL, &si, &pi
		);
		WaitForSingleObject(pi.hProcess, INFINITE);
	}
	std::deque<MStatic> m_doc;
	char * ConvertWCtoC(const wchar_t* str) {
		char* pStr;
		int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
		pStr = new char[strSize];
		WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);
		return pStr;
	}
	bool IsMoreThanNTag(int n) {
		int C = 0;
		for (auto &e : *GetApp().g_image_data) {
			//std::string filename(e.first.begin(), e.first.end());
			char* filename = ConvertWCtoC((e.first.data()));
			std::vector<TagInfo> taginfo = this->ReadTagInfo(filename);
			for (auto&f : taginfo) {
				if (f.m_class != -100) {
					if (GetExportClass(f.m_class) >= 0) {
						C++;
						if (C == n)return true;
					}
				}
			}
		}
		return false;
	}
	void WriteDocument() {
		for (auto&txt : m_doc) {
			txt->m_text = TEXT("");
		}
		if (m_chk_yolo->check == true) {
			m_doc[0]->m_text = TEXT("YOLO : Real-Time Object Detection , Faster than FasterRCNN and reasonable mAP.");
		} else if (m_chk_fasterrcnn->check == true) {
			m_doc[0]->m_text = TEXT("FasterRCNN : High accuracy, That's all. This is CNTK wrapper.");
		} else if (m_chk_dlibcnn->check == true) {
			m_doc[0]->m_text = TEXT("dlib cnn : Sorry, Not implemented yet.");
		}

		if (GetChkBtnByString(m_chk_models,m_stc_models,TEXT("DarknetREF"))->check == true) {
			m_doc[1]->m_text = TEXT("Darknet Reference : 144FPS , Fast trainning , Light weight.");
		} else if (GetChkBtnByString(m_chk_models, m_stc_models, TEXT("Darknet19"))->check == true) {
			m_doc[1]->m_text = TEXT("Darknet19 : Default base model of YOLOv2.");
		} else if (GetChkBtnByString(m_chk_models, m_stc_models, TEXT("Resnet50"))->check == true) {
			m_doc[1]->m_text = TEXT("Resnet50 : High accuracy , Can detect small object.");
		} else if (GetChkBtnByString(m_chk_models, m_stc_models, TEXT("Densenet201"))->check == true) {
			m_doc[1]->m_text = TEXT("Densenet201 : Very deep network , High accuracy , Can detect small and big object.");
		} else if (GetChkBtnByString(m_chk_models, m_stc_models, TEXT("Darknet53"))->check == true) {
			m_doc[1]->m_text = TEXT("Darknet53 : This is YOLOv3!!! Say YOLO Again~~~~");
		} else if (GetChkBtnByString(m_chk_models, m_stc_models, TEXT("Alexnet"))->check == true) {
			m_doc[1]->m_text = TEXT("Alexnet :  Basic CNN network.");
		} else if (GetChkBtnByString(m_chk_models, m_stc_models, TEXT("VGG16"))->check == true) {
			m_doc[1]->m_text = TEXT("VGG16 : Heavy, Slow, but.... you don't have an other option in CNTK FasterRCNN.");
		} else if (GetChkBtnByString(m_chk_models, m_stc_models, TEXT("VGG19"))->check == true) {
			m_doc[1]->m_text = TEXT("VGG19 : What is difference between VGG16 and VGG19?");
		}
		if (m_chk_yolo->check == true) {
			m_doc[2]->m_text = TEXT("Anchor : Can predict more boxes if you choose many anchors.");
		}
		if (GetChkBtnByString(m_chk_size, m_stc_size, TEXT("352x352"))->check == true) {
			m_doc[3]->m_text = TEXT("352x352 : This option is for embedded system.");
		} else if (GetChkBtnByString(m_chk_size, m_stc_size, TEXT("416x416"))->check == true) {
			m_doc[3]->m_text = TEXT("416x416 : Usually, this resolution is used.");
		} else if (GetChkBtnByString(m_chk_size, m_stc_size, TEXT("480x480"))->check == true) {
			m_doc[3]->m_text = TEXT("480x480 : Not recommend.");
		} else if (GetChkBtnByString(m_chk_size, m_stc_size, TEXT("544x544"))->check == true) {
			m_doc[3]->m_text = TEXT("544x544 : Can detect small object , But, consider random option.");
		} else if (GetChkBtnByString(m_chk_size, m_stc_size, TEXT("608x608"))->check == true) {
			m_doc[3]->m_text = TEXT("608x608 : Can detect small object , But, consider random option.");
		} else if (GetChkBtnByString(m_chk_size, m_stc_size, TEXT("1088x1088"))->check == true) {
			m_doc[3]->m_text = TEXT("1088x1088 : Warning, This option makes training time super slowly.");
		} else if (GetChkBtnByString(m_chk_size, m_stc_size, TEXT("Random"))->check == true) {
			m_doc[3]->m_text = TEXT("Random : This option changes the size of the convolution network every 10 iteration.");
		} else if (GetChkBtnByString(m_chk_size, m_stc_size, TEXT("850x850"))->check == true) {
			m_doc[3]->m_text = TEXT("850x850 : CNTK FasterRCNN's default input size.");
		}
		m_doc[4]->m_text = TEXT("Noise : It generates additional train data. All options apply for each noise data and original data. except GammaCorrection");
			
		if (m_chk_rotate->check == true) {
			m_doc[6]->m_text = TEXT("It supports rotated rect type tag. It generates rotated image if the tag is rotated.");
		} else if (m_chk_boundingbox->check == true) {
			m_doc[6]->m_text = TEXT("Use bounding box of rotated rect.");
		} else if (m_chk_ignore_rbox->check == true) {
			m_doc[6]->m_text = TEXT("Ignore rotated rect.");
		}
		if (m_chk_debug_info->check) {
			m_doc[7]->m_text = TEXT("It will generate crop image by tag box, anchors and image statistics.");
		}
	}
	void Export_YOLO();
	void Export_FasterRCNN();
	std::vector<TagInfo> ReadTagInfo(std::string filename) {
		std::vector<TagInfo> taginfo;
		std::string tsp_path = filename.substr(0, filename.find_last_of('.')) + ".tsp";
		std::fstream fin;
		fin.open(tsp_path, std::ios::in);
		if (fin.is_open() == false) {
			return taginfo;
		}
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
			int _id = -1;
			iss >> _class >> rr.center.x >> rr.center.y >> rr.size.width >> rr.size.height >> rr.angle >> _id;
			taginfo.push_back(TagInfo(_class, rr, _id));
		}
		return taginfo;
	}
	struct ImageStatistics {
		int nOfTotal;
		std::vector<int> nOfImages;
		std::vector<int> nOfSamples;
	};
	void GenExportImage(std::string filename, std::fstream& fout_train_txt, ImageStatistics& stat,std::ostream& fout_roi_txt =std::cout);
	int GetExportClass(int c);
	std::string GetOriginalName(std::string path, std::string sdir, std::string ext) {
		std::string::size_type slash = path.find_last_of("/\\") + 1;
		std::string::size_type dot = path.find_last_of(".");
		std::string pure = path.substr(slash, dot - slash);
		std::string tmp = pure;
		std::string test = sdir + tmp + ext;
		int c = 1;
		while (ispring::File::FileExist(test) == true) {
			tmp = pure + "(" + std::to_string(c) + ")";
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
	void SaveCropImage(std::string filename,cv::Mat img, int real_class) {
		std::string dir=("Debug\\crop\\"
									 + ispring::String::PadNum(GetExportClass(real_class), 4) + "("
									 + std::string(m_list_class->m_data[real_class].first.begin(), m_list_class->m_data[real_class].first.end())
									 + ")\\");
		ispring::File::DirectoryMake(dir);
		std::string cropname = GetOriginalName(filename, dir, ".jpg");
		cv::imwrite(dir+cropname+".jpg", img);
	}
	std::vector<YOLOBOX> GetYOLOBOX(cv::Mat& img, std::vector<TagInfo> tag_info) {
		std::vector<YOLOBOX> yolobox;
		for (auto&e : tag_info) {
			if (m_chk_ignore_rbox->check == true && e.m_rect.angle != 0) {	//각도 무시일경우 angle이 있으면 무시
				continue;
			}
			int C = GetExportClass(e.m_class);
			if (C == -100) {
				continue;
			}
			cv::Rect2f rect = e.m_rect.boundingRect2f();
			YOLOBOX box;
			box.m_class = C;
			box.x = (rect.x + rect.width / 2.0F) / img.cols;
			box.y = (rect.y + rect.height / 2.0F) / img.rows;
			box.w = rect.width / img.cols;
			box.h = rect.height / img.rows;
			yolobox.push_back(box);
		}
		return yolobox;
	}
	std::vector<RCNNBOX> GetRCNNBOX(cv::Mat& img, std::vector<TagInfo> tag_info,int size) {
		int W, H;
		if (size > 0) {
			W = H = size;
		} else {
			W = img.cols;
			H = img.rows;
		}
		std::vector<RCNNBOX> rcnnbox;
		for (auto&e : tag_info) {
			if (m_chk_ignore_rbox->check == true && e.m_rect.angle != 0) {	//각도 무시일경우 angle이 있으면 무시
				continue;
			}
			int C = GetExportClass(e.m_class);
			if (C == -100 || C==-1) {
				continue;
			}
			cv::Rect2f rect = e.m_rect.boundingRect2f();
			float rx1 = rect.x / img.cols;
			float ry1 = rect.y / img.rows;
			float rx2 = (rect.x + rect.width) / img.cols;
			float ry2 = (rect.y + rect.height) / img.rows;
			RCNNBOX box;
			box.m_class = C;
			box.x1 = static_cast<int>(rx1*W);
			box.y1 = static_cast<int>(ry1*H);
			box.x2 = static_cast<int>(rx2*W);
			box.y2 = static_cast<int>(ry2*H);
			mspring::SetRange(box.x1, 0, W - 1);
			mspring::SetRange(box.y1, 0, H - 1);
			mspring::SetRange(box.x2, 0, W - 1);
			mspring::SetRange(box.y2, 0, H - 1);
			rcnnbox.push_back(box);
		}
		return rcnnbox;
	}
	void GenTagInfos(const ImageStatistics& stat) {
		ExcelFormat::BasicExcel xls;
		xls.New(1);
		ExcelFormat::BasicExcelWorksheet* sheet = xls.GetWorksheet(0);
		ExcelFormat::XLSFormatManager fmt_mgr(xls);
		for (int i = 0; i < 5; i++) {
			sheet->SetColWidth(i, 8000);
		}
		ExcelFormat::ExcelFont font_bold;
		font_bold._weight = FW_BOLD;
		ExcelFormat::CellFormat fmt_bold(fmt_mgr);
		fmt_bold.set_font(font_bold);
		sheet->Cell(0, 0)->Set("TSpring Image Statistics");
		sheet->Cell(0, 0)->SetFormat(fmt_bold);
		sheet->Cell(1, 0)->Set("Total Positive Images");
		sheet->Cell(1, 0)->SetFormat(fmt_bold);
		sheet->Cell(1, 1)->Set(stat.nOfTotal);
		
		sheet->Cell(3, 0)->Set("Class");
		sheet->Cell(3, 0)->SetFormat(fmt_bold);
		sheet->Cell(3, 1)->Set("# of images");
		sheet->Cell(3, 1)->SetFormat(fmt_bold);
		sheet->Cell(3, 2)->Set("# of samples");
		sheet->Cell(3, 2)->SetFormat(fmt_bold);
		int col = 0, row = 4;
		int idx = 0;
		for (auto&e : m_list_class->m_data) {
			if (e.second == true) {
				sheet->Cell(row, 0)->Set(e.first.c_str());
				sheet->Cell(row, 1)->Set(stat.nOfImages[GetExportClass(idx)]);
				sheet->Cell(row, 2)->Set(stat.nOfSamples[GetExportClass(idx)]);
				row++;
			}
			idx++;
		}
		xls.SaveAs("Debug\\stat.xls");
	}
	int rcnn_cnt = 0;
	std::vector<std::string> GetRCNNImages() {
		std::vector<std::string> ret;
		std::fstream fin("train_img_file.txt", std::ios::in);
		while (fin.eof() == false) {
			std::string line;
			std::getline(fin, line);
			if (line.length() == 0)break;
			std::istringstream iss(line);
			int n;
			std::string filename;
			iss >> n >> filename;
			ret.push_back(filename);
		}
		fin.close();
		return ret;
	}
	std::vector<std::vector<RCNNBOX>> GetRCNNBoxes() {
		std::vector<std::vector<RCNNBOX>> ret;
		std::fstream fin("train_roi_file.txt", std::ios::in);
		while (fin.eof() == false) {
			std::string line;
			std::getline(fin, line);
			if (line.length() == 0)break;
			std::istringstream iss(line);
			int n;
			std::string dummy;
			iss >> n >> dummy;
			std::vector<RCNNBOX> each;
			while (true) {
				RCNNBOX box;
				box.x1 = -1;
				iss >> box.x1;
				if (box.x1 == -1)break;
				float f;
				iss >> box.y1 >> box.x2 >> box.y2 >> f;
				box.m_class = static_cast<int>(f);
				each.push_back(box);
			}
			ret.push_back(each);
		}
		fin.close();
		return ret;
	}
public:

	int OnCreate()override;
	void OnDestroy()override;

	void OnPaint(CDC* pDC)override;
	void OnSetFocus(CWnd* pOldWnd)override;
	void OnKillFocus(CWnd* pNewWnd)override;

	void OnLButtonDown(UINT nFlags, CPoint point)override;
	void OnLButtonUp(UINT nFlags, CPoint point)override;
	void OnLButtonDblClk(UINT nFlags, CPoint point)override;
	void OnRButtonUp(UINT nFlags, CPoint point)override;
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)override;
	void OnMouseMove(UINT nFlags, CPoint point)override;
	void OnMouseLeave()override;

	BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)override;
	void OnSize(UINT nType, int cx, int cy)override;
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)override;
	void OnTimer(UINT_PTR nIDEvent)override;
	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)override;
	LRESULT OnComposition(WPARAM wParam, LPARAM lParam)override;
};

