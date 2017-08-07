#include "stdafx.h"

#include "MainFrame.h"
#include "CMainFrame2.h"

CAppModule _Module;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPSTR szCmdLine, int nCmdShow)
{
    fusion::CMainFrame wndMain;
    //fusion::MainWnd wndMain;

    // Create & show our main window
    if (nullptr == wndMain.Create(nullptr, CWindow::rcDefault, _T("WTL Frame"), WS_OVERLAPPEDWINDOW))
    {
        // Bad news, window creation failed
        return 1;
    }

    wndMain.ShowWindow(nCmdShow);
    wndMain.UpdateWindow();

    // Run the message loop
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT fusion::CMainFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DestroyWindow();
	return 0;
}

LRESULT fusion::CMainFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0);
	return 0;
}

LRESULT fusion::CMainFrame::OnSize(UINT nType, CSize Extent)
{
	cdbg << "OnSize:: " << Extent.cx << ", " << Extent.cy << "\n";
	UpdateLayout();
	return 1;
}

BOOL fusion::CMainFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//if (zDelta > 0)
	//{
	//	m_timelineView.Zoom(2.0);
	//}
	//else
	//{
	//	m_timelineView.Zoom(0.5);
	//}
	return TRUE;
}

void fusion::CMainFrame::DisablePaneHeader(CPaneContainer& panecontainer)
{
	panecontainer.SetPaneContainerExtendedStyle(PANECNT_NOCLOSEBUTTON, 0);
	panecontainer.m_cxyHeader = 0;
}

void fusion::CMainFrame::AddTab(const std::wstring name)
{
	auto lvi = std::make_shared<CLogViewTabItem2>();
	m_tabitems.push_back(lvi);
	lvi->SetText(name.c_str());
	lvi->Create(*this);

    //timeline.Create(*this, CWindow::rcDefault, gdi::CTimelineView::GetWndClassName(), WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | SS_OWNERDRAW);


	int newIndex = GetTabCtrl().GetItemCount();
	GetTabCtrl().InsertItem(newIndex, lvi.get());
	GetTabCtrl().SetCurSel(newIndex);
}

LRESULT fusion::CMainFrame::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	auto rect = RECT();
	GetClientRect(&rect);

	// block 1
	//auto lvi = std::make_shared<CLogViewTabItem2>();
	//m_tabitems.push_back(lvi);
	//auto& timeline = lvi->GetTimeLineView();
	//lvi->InitTimeLine();
	//timeline.Create(*this, CWindow::rcDefault, gdi::CTimelineView::GetWndClassName(), WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | SS_OWNERDRAW);

	//timeline.SetView(0.0, 1000.0);
 //   timeline.ShowWindow(true);

	// block 2
	CreateTabWindow(*this, rect, CTCS_CLOSEBUTTON | CTCS_DRAGREARRANGE);
	AddTab(L"Tab1");
	AddTab(L"Tab2");

	ShowTabControl();
	return 0;
}