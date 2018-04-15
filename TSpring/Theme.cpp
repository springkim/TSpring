#include"stdafx.h"
#include"Theme.h"
void DrawBitmap(CDC* pDC, CRect rect, UINT id) {
	CDC mDC;
	BITMAP bmp_info;
	mDC.CreateCompatibleDC(pDC);
	CBitmap* bmp, *old_bmp = nullptr;
	static std::map<UINT, CBitmap*> cbmp_map;
	if (cbmp_map.find(id) == cbmp_map.end()) {
		bmp = new CBitmap;
		bmp->LoadBitmap(id);
		cbmp_map.insert(std::make_pair(id, bmp));
	} else {
		bmp = cbmp_map.find(id)->second;
	}



	bmp->GetBitmap(&bmp_info);
	old_bmp = mDC.SelectObject(bmp);
	int w = rect.Width();
	int h = rect.Height();
	if (bmp_info.bmWidth != rect.Width() || bmp_info.bmHeight != rect.Height()) {
		w = bmp_info.bmWidth*rect.Height() / bmp_info.bmHeight;
		h = rect.Height();
		if (w > rect.Width()) {
			w = rect.Width();
			h = bmp_info.bmHeight*rect.Width() / bmp_info.bmWidth;
		}
	}
	int x = (rect.Width() - w) / 2 + rect.left;
	int y = (rect.Height() - h) / 2 + rect.top;
	pDC->SetStretchBltMode(HALFTONE);
	pDC->StretchBlt(x, y, w, h, &mDC, 0, 0, bmp_info.bmWidth, bmp_info.bmHeight, SRCCOPY);
	mDC.SelectObject(old_bmp);
}

void ShowProgressBar(CDC* pDC, CRect rect_window) {
	CPen null_pen;
	null_pen.CreatePen(PS_NULL, 0, RGB(0, 0, 0));

	//Draw status window
	CBrush bk_brush;
	bk_brush.CreateSolidBrush(GetTheme().ColorBK());
	CBrush* old_brush = pDC->SelectObject(&bk_brush);
	CRect rect;

	rect.left = (rect_window.left + rect_window.right) / 2 - 250;
	rect.right = rect.left + 500;
	rect.top = (rect_window.top + rect_window.bottom) / 2 - 50;
	rect.bottom = rect.top + 100;
	CPen pen;
	pen.CreatePen(PS_SOLID, 4, GetTheme().ColorBorder());
	pDC->SelectObject(&pen);
	pDC->RoundRect(rect, CPoint(10, 10));
	pDC->SelectObject(&null_pen);
	pDC->SelectObject(old_brush);
	//Draw Progressbar
	CBrush probk_brush;
	probk_brush.CreateSolidBrush(MControlObject::GetBrightColor(GetTheme().ColorBK()));
	old_brush = pDC->SelectObject(&probk_brush);
	CRect progress_rect = rect;
	progress_rect.top += static_cast<LONG>(rect.Height()*0.1);
	progress_rect.bottom = static_cast<LONG>(progress_rect.top + rect.Height()*0.3);
	progress_rect.left = static_cast<LONG>(rect.left + rect.Width()*0.05);
	progress_rect.right = static_cast<LONG>(rect.right - rect.Width()*0.05);
	pDC->RoundRect(progress_rect, CPoint(10, 10));
	pDC->SelectObject(old_brush);
	//Draw progress thumb
	CBrush proth_brush;
	proth_brush.CreateSolidBrush(GetTheme().ColorTitle());
	old_brush = pDC->SelectObject(&proth_brush);
	CRect thumb_rect = progress_rect;
	if (GetApp().g_analysing_count != 0) {
		float ratio = GetApp().g_progress_current / static_cast<float>(GetApp().g_progress_total);
		thumb_rect.right = static_cast<LONG>(thumb_rect.left + progress_rect.Width()*ratio);
	} else {
		thumb_rect.right = thumb_rect.left;
	}
	pDC->RoundRect(thumb_rect, CPoint(10, 10));
	pDC->SelectObject(old_brush);
	//Draw status text
	CRect status_rect = rect;
	status_rect.bottom -= static_cast<LONG>(rect.Height()*0.1);
	status_rect.top = static_cast<LONG>(status_rect.bottom - rect.Height()*0.3);
	status_rect.left = static_cast<LONG>(rect.left + rect.Width()*0.05);
	status_rect.right = static_cast<LONG>(rect.right - rect.Width()*0.05);
	OStringStream oss;
	oss << GetApp().g_export_str << TEXT(" ... (") << GetApp().g_progress_current << TEXT("/") << GetApp().g_progress_total << TEXT(")");
	
	int font_height = mspring::Font::GetRealFontHeight(TEXT("Arial"), status_rect.Height(), pDC, oss.str() , true);

	int font_height2 = mspring::Font::GetRealFontHeight(TEXT("Arial"), status_rect.Width(), pDC, oss.str() , false);
	font_height = mspring::Min(font_height, font_height2);
	CFont font;
	font.CreatePointFont(font_height, TEXT("Arial"));
	CFont* old_font = pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(253, 246, 227));
	pDC->SetBkMode(TRANSPARENT);

	pDC->TextOutW(status_rect.left, status_rect.top, oss.str().c_str());
	//dbb.getDC().RoundRect(status_rect, CPoint(10, 10));
	pDC->SelectObject(old_brush);


	pDC->SelectObject(old_font);
	font.DeleteObject();
	bk_brush.DeleteObject();
}