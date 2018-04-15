#pragma once
#include<cstring>
#include<array>
#include"mspring/view.h"
#include"mspring/control.h"
#include"repository.h"
#include"ispring/All.h"

#include"cfg_darknet_reference.h"
#include"cfg_darknet19.h"
#include"cfg_densenet201.h"
#include"cfg_resnet50.h"
#include"gen_anchors.h"
struct YOLOBOX {
	int m_class;
	float x;
	float y;
	float w;
	float h;
};
class ExportYOLOv2 : public VirtualView {
public:
	ExportYOLOv2(CWnd* wnd);
	virtual ~ExportYOLOv2();
public:
	MStatic* m_stc_basemodel;
	MButtonCheck* m_chk_darknet_r;
	MStatic* m_stc_darknet_r;
	MButtonCheck* m_chk_darknet19;
	MStatic* m_stc_darknet19;
	MButtonCheck* m_chk_resnet50;
	MStatic* m_stc_resnet50;
	MButtonCheck* m_chk_densenet201;
	MStatic* m_stc_densenet201;
	//==========ANCHORS
	MStatic* m_stc_anchors;
	MButtonCheck* m_chk_anchor3;
	MStatic* m_stc_anchor3;
	MButtonCheck* m_chk_anchor5;
	MStatic* m_stc_anchor5;
	MButtonCheck* m_chk_anchor7;
	MStatic* m_stc_anchor7;
	MButtonCheck* m_chk_anchor9;
	MStatic* m_stc_anchor9;
	MButtonCheck* m_chk_anchor11;
	MStatic* m_stc_anchor11;
	//===========RESOLUTION
	MStatic* m_stc_resolution;
	MButtonCheck* m_chk_resolution416;
	MStatic* m_stc_resolution416;
	MButtonCheck* m_chk_resolution480;
	MStatic* m_stc_resolution480;
	MButtonCheck* m_chk_resolution544;
	MStatic* m_stc_resolution544;
	MButtonCheck* m_chk_resolution608;
	MStatic* m_stc_resolution608;
	MButtonCheck* m_chk_resolution1088;
	MStatic* m_stc_resolution1088;
	MButtonCheck* m_chk_resolution_random;
	MStatic* m_stc_resolution_random;
	//============NOISE
	MStatic* m_stc_noise;
	MButtonCheck* m_chk_noise_gray;
	MStatic* m_stc_noise_gray;
	MButtonCheck* m_chk_noise_dot;
	MStatic* m_stc_noise_dot;
	MButtonCheck* m_chk_noise_flipLR;
	MStatic* m_stc_noise_flipLR;
	MButtonCheck* m_chk_noise_flipTB;
	MStatic* m_stc_noise_flipTB;
	MButtonCheck* m_chk_noise_blur;
	MStatic* m_stc_noise_blur;
	//================OPTION
	int GetWidth() {
		if (m_chk_resolution416->check == true) {
			return 416;
		}
		if (m_chk_resolution480->check == true) {
			return 480;
		}
		if (m_chk_resolution544->check == true) {
			return 544;
		}
		if (m_chk_resolution608->check == true) {
			return 608;
		}
		if (m_chk_resolution1088->check == true) {
			return 1088;
		}
		if (m_chk_resolution_random->check == true) {
			return 416;
		}
		return 416;
	}
	int GetRandom() {
		if (m_chk_resnet50->check == true) {
			return 0;
		}
		if (m_chk_resolution_random->check == true) {
			return 1;
		} else {
			return 0;
		}
	}
	std::pair<char*, std::string> GetCFG() {
		static char tmp[65536];
		std::pair<char*, std::string> cfg;
		if (m_chk_darknet_r->check == true) {
			cfg.first = cfg_darknetR;
			cfg.second = "darknet-reference";
		} else if (m_chk_darknet19->check == true) {
			cfg.first = cfg_darknet19;
			cfg.second = "darknet19";
		} else if (m_chk_resnet50->check == true) {
			cfg.first = cfg_resnet50;
			cfg.second = "resnet50";
		} else if (m_chk_densenet201->check == true) {
			memset(tmp, 0, 65536);
			strcpy_s(tmp, 65535, cfg_densenet201_part1);
			strcat_s(tmp, 65535, cfg_densenet201_part2);
			cfg.first = tmp;
			cfg.second = "densenet201";
		}
		return cfg;
	}
	int GetClasses() {
		int classes = 0;
		for (auto&e : *g_export_class_data) {
			if (e.second == true) {
				classes++;
			}
		}
		return classes;
	}
	int GetNumOfAnchors() {
		if (m_chk_anchor3->check == true) {
			return 3;
		}
		if (m_chk_anchor5->check == true) {
			return 5;
		}
		if (m_chk_anchor7->check == true) {
			return 7;
		}
		if (m_chk_anchor9->check == true) {
			return 9;
		}
		if (m_chk_anchor11->check == true) {
			return 11;
		}
		return 1;
	}
	std::vector<MControlObject*> m_objs;
	std::vector<MControlObject*> m_base_model;
	std::vector<MControlObject*> m_anchors;
	std::vector<MControlObject*> m_resolution;
	std::vector<MControlObject*> m_noise;
	void Export();
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

