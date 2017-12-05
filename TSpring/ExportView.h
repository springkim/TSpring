#pragma once
#include<deque>
#include "mspring/view.h"
#include"mspring/control.h"
#include"ExportYOLOv2.h"
#include"repository.h"
class ExportView :
	public VirtualView {
public:
	ExportView(CWnd* wnd);
	~ExportView();
public:
	MListBox* m_list_class;
	bool isSameClassList() {
		if (m_list_class->m_data.size() != g_class_data->size()) {
			return false;
		}
		for (size_t i = 0; i < m_list_class->m_data.size(); i++) {
			if (m_list_class->m_data[i].first != g_class_data->at(i).first) {
				return false;
			}
		}
		return true;
	}
	MStatic* m_stc_architecture;
	MButtonCheck* m_chk_yolov2;
	MStatic* m_stc_yolov2;
	MButtonCheck* m_chk_fasterrcnn;
	MStatic* m_stc_fasterrcnn;
	MButtonCheck* m_chk_dlibcnn;
	MStatic* m_stc_dlibcnn;
	std::vector<MControlObject*> m_objs;

	MButtonCheck* m_chk_rotate;
	MStatic* m_stc_rotate;
	MButtonCheck* m_chk_boundingbox;
	MStatic* m_stc_boundingbox;
	MButtonCheck* m_chk_ignore_rbox;
	MStatic* m_stc_ignore_rbox;

	MEdit* m_edit_name;
	MButton* m_btn_export;
	MStatic* m_stc_name;


	std::deque<MStatic*> m_doc;

	ExportYOLOv2 m_yolov2;
	void WriteDocument() {
		for (auto&txt : m_doc) {
			txt->m_text = TEXT("");
		}
		if (m_chk_yolov2->check == true) {
			m_doc[0]->m_text = TEXT("YOLO v2 : Real-Time Object Detection , Faster than FasterRCNN and reasonable mAP.");
			if (m_yolov2.m_chk_darknet_r->check == true) {
				m_doc[1]->m_text = TEXT("Darknet Reference : 144FPS , Fast trainning , Light weight.");
			} else if (m_yolov2.m_chk_darknet19->check == true) {
				m_doc[1]->m_text = TEXT("Darknet19 : YOLO's default base model.");
			} else if (m_yolov2.m_chk_resnet50->check == true) {
				m_doc[1]->m_text = TEXT("Resnet50 : High accuracy , Can detect small object.");
			} else if (m_yolov2.m_chk_densenet201->check == true) {
				m_doc[1]->m_text = TEXT("Densenet201 : Very deep network , High accuracy , Can detect small and big object.");
			}
			m_doc[2]->m_text = TEXT("Anchor : Can predict more boxes if you choose many anchors.");
			if (m_yolov2.m_chk_resolution416->check == true) {
				m_doc[3]->m_text = TEXT("416x416 : Usually, this resolution is used.");
			} else if (m_yolov2.m_chk_resolution480->check == true) {
				m_doc[3]->m_text = TEXT("480x480 : Not recommend.");
			} else if (m_yolov2.m_chk_resolution544->check == true) {
				m_doc[3]->m_text = TEXT("544x544 : Can detect small object , But, consider random option.");
			} else if (m_yolov2.m_chk_resolution608->check == true) {
				m_doc[3]->m_text = TEXT("608x608 : Can detect small object , But, consider random option.");
			} else if (m_yolov2.m_chk_resolution1088->check == true) {
				m_doc[3]->m_text = TEXT("1088x1088 : Warning, This option makes training time super slowly.");
			} else if (m_yolov2.m_chk_resolution_random->check == true) {
				m_doc[3]->m_text = TEXT("Random : This option changes the size of the convolution network every 10 iteration.");
			}
			m_doc[4]->m_text = TEXT("Noise : It generates additional train data. All options apply for each noise data and original data.");
			
		} else if (m_chk_fasterrcnn->check == true) {
			m_doc[0]->m_text = TEXT("FasterRCNN : Sorry, Not implemented yet.");
		} else if (m_chk_dlibcnn->check == true) {
			m_doc[0]->m_text = TEXT("dlib cnn : Sorry, Not implemented yet.");
		}
		if (m_chk_rotate->check == true) {
			m_doc[6]->m_text = TEXT("It supports rotated rect type tag. It generates rotated image if the tag is rotated.");
		} else if (m_chk_boundingbox->check == true) {
			m_doc[6]->m_text = TEXT("Use bounding box of rotated rect.");
		} else if (m_chk_ignore_rbox->check == true) {
			m_doc[6]->m_text = TEXT("Ignore rotated rect.");
		}

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

