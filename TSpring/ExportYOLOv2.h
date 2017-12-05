#pragma once
#include "mspring/view.h"
#include"mspring/control.h"
#include"repository.h"

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

	std::vector<MControlObject*> m_objs;
	std::vector<MControlObject*> m_base_model;
	std::vector<MControlObject*> m_anchors;
	std::vector<MControlObject*> m_resolution;
	std::vector<MControlObject*> m_noise;
	void Export(bool is_rotate, bool is_ignore);
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

