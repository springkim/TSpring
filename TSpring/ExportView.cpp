#include "stdafx.h"
#include "ExportView.h"


ExportView::ExportView(CWnd* wnd) : VirtualView(wnd) {

}


ExportView::~ExportView() {
}
int ExportView::OnCreate() {
	VirtualView::OnCreate();
	return 1;
}
void ExportView::OnDestroy() {
	VirtualView::OnDestroy();
}

void ExportView::OnPaint(CDC* pDC) {
	VirtualView::OnPaint(pDC);
	pDC->TextOutW(0, 0, TEXT("ExportView"));
}
void ExportView::OnSetFocus(CWnd* pOldWnd) {
	VirtualView::OnSetFocus(pOldWnd);
}
void ExportView::OnKillFocus(CWnd* pNewWnd) {
	VirtualView::OnKillFocus(pNewWnd);
}

void ExportView::OnLButtonDown(UINT nFlags, CPoint point) {
	VirtualView::OnLButtonDown(nFlags, point);
}
void ExportView::OnLButtonUp(UINT nFlags, CPoint point) {
	VirtualView::OnLButtonUp(nFlags, point);
}
void ExportView::OnLButtonDblClk(UINT nFlags, CPoint point) {
	VirtualView::OnLButtonDblClk(nFlags, point);
}
void ExportView::OnRButtonUp(UINT nFlags, CPoint point) {
	VirtualView::OnRButtonUp(nFlags, point);
}
BOOL ExportView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	VirtualView::OnMouseWheel(nFlags, zDelta, pt);
	return TRUE;
}
void ExportView::OnMouseMove(UINT nFlags, CPoint point) {
	VirtualView::OnMouseMove(nFlags, point);
}
void ExportView::OnMouseLeave() {
	VirtualView::OnMouseLeave();
}

BOOL ExportView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
	VirtualView::OnSetCursor(pWnd, nHitTest, message);
	return TRUE;
}
void ExportView::OnSize(UINT nType, int cx, int cy) {
	VirtualView::OnSize(nType, cx, cy);
}

void ExportView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	VirtualView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void ExportView::OnTimer(UINT_PTR nIDEvent) {
	VirtualView::OnTimer(nIDEvent);
}
void ExportView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	VirtualView::OnChar(nChar, nRepCnt, nFlags);
}
LRESULT ExportView::OnComposition(WPARAM wParam, LPARAM lParam) {
	VirtualView::OnComposition(wParam, lParam);
	return 1;
}