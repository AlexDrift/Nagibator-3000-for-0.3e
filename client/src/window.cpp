#include "main.h"

// Code taken from http://www.rohitab.com/discuss/topic/37441-how-to-change-color-of-selected-text-in-listbox/page__view__findpost__p__10081946

// Child Window/Control IDs
#define IDC_LBLINFO             100
#define IDC_LSTCUSTOM           101
#define IDC_INPUTBOX			102
// Globals
HWND hwnd, texthwnd, loghwnd, inputhwnd, switchers_list, spam_amount, change_ip;
HINSTANCE g_hInst;
HFONT g_hfText;

#define MINUS 190

#define SetItemPos( id, x, y, w, h ) \
	hDWP = DeferWindowPos(hDWP, GetDlgItem(hwnd, id), NULL, x, y, w, h, SWP_NOZORDER | SWP_NOREDRAW);

#define CLICKED(ID) MAKELONG(ID, BN_CLICKED)
#define AddButton(id, text) \
	hWndChild = CreateWindowEx(0, WC_BUTTON, TEXT(text), BS_DEFPUSHBUTTON | BS_TEXT | WS_CHILD | WS_TABSTOP | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)id, g_hInst, NULL); \
	SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_hfText, FALSE);

#define AddTextBox(id, text, limit) \
	hWndChild = CreateWindowEx(0, WC_EDIT, TEXT(text), WS_CHILD | WS_TABSTOP | WS_VISIBLE | WS_BORDER, 0, 0, 0, 0, hWnd, (HMENU)id, g_hInst, NULL); \
	SendMessage(hWndChild, EM_LIMITTEXT, limit, 0); \
	SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_hfText, FALSE);

#define AddListBox(id) \
	hWndChild = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTBOX, NULL, LBS_NOINTEGRALHEIGHT | LBS_NOTIFY | LBS_OWNERDRAWFIXED | WS_VSCROLL | WS_CHILD | WS_TABSTOP | WS_VISIBLE, \
	0, 0, 0, 0, hWnd, (HMENU)(id), g_hInst, NULL); \
	SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_hfText, FALSE);

#define AddStaticText(id, text) \
	hWndChild = CreateWindowEx(0, WC_STATIC, TEXT(text), SS_LEFT | WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, (HMENU)(id), g_hInst, NULL); \
	SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_hfText, FALSE);

// Prototypes
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CalcWindowTextSize(HWND hWnd, LPRECT rcFit);
BOOL CenterWindow(HWND hWnd, HWND hWndCenter);

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
				// This message is sent for each owner drawn child and only once for LBS_OWNERDRAWFIXED.
		// If we had used LBS_OWNERDRAWVARIABLE it is sent for each item.
	case WM_MEASUREITEM:
		{
			LPMEASUREITEMSTRUCT lpMeasureItem = (LPMEASUREITEMSTRUCT)lParam;
			// Is this measure request for our control?
			if(lpMeasureItem->CtlID == IDC_LSTCUSTOM)
			{
				TEXTMETRIC tm;
				HWND hWndItem = GetDlgItem(hWnd, IDC_LSTCUSTOM);
				HDC  hdcItem  = GetDC(hWndItem);
				PlaySound("Sound/welcome.wav",NULL,SND_ASYNC);
				if(GetTextMetrics(hdcItem, &tm))
					// Set the item height to that of the font + 10px padding
					lpMeasureItem->itemHeight = tm.tmInternalLeading + 
					tm.tmHeight + tm.tmExternalLeading + 2;

				ReleaseDC(hWndItem, hdcItem);
				return TRUE;
			}
		}
		break;

		// This message is sent for each owner drawn child when it needs to be rendered.
	case WM_DRAWITEM:
		{
			LPDRAWITEMSTRUCT lpDrawItem = (LPDRAWITEMSTRUCT)lParam;
			// Is this draw request for our control?
			if(lpDrawItem->CtlID == IDC_LSTCUSTOM)
			{
				// Get the text pointer
				LPTSTR lpText = (LPTSTR)lpDrawItem->itemData;
				COLORREF textColor = RGB(255, 0, 0);
				COLORREF bkColor = RGB(0, 0, 0);

				if(!strncmp(lpText, "[ЧАТ] ", 7))
					textColor = RGB(settings.bChatColorRed, settings.bChatColorGreen, settings.bChatColorBlue);

				if(!strncmp(lpText, "[СЕРВЕР] ", 7))
					textColor = RGB(settings.bCMsgRed, settings.bCMsgGreen, settings.bCMsgBlue);
								

				if(settings.iFind)
				{
					for(int i = 0; i < MAX_FIND_ITEMS; i++)
					{
						if(!settings.findItems[i].iExists)
							continue;

						if(strstr(lpText, settings.findItems[i].szFind))
						{
							textColor = RGB(settings.findItems[i].bTextRed,
								settings.findItems[i].bTextGreen,
								settings.findItems[i].bTextBlue);
							bkColor = RGB(settings.findItems[i].bBkRed,
								settings.findItems[i].bBkGreen,
								settings.findItems[i].bBkBlue);
						}
					}
				}

				// Set colors based on selected state
				if(lpDrawItem->itemState & ODS_SELECTED)
				{
					SetBkColor(lpDrawItem->hDC, RGB(0, 0, 0));
					SetTextColor(lpDrawItem->hDC, textColor);
				}
				else
				{
					SetBkColor(lpDrawItem->hDC, bkColor);
					SetTextColor(lpDrawItem->hDC, textColor);
				}

				// Draw the text and fill in the background at the same time
				//  with 5px offset for padding
				ExtTextOut(lpDrawItem->hDC,
					lpDrawItem->rcItem.left + 5, lpDrawItem->rcItem.top + 5,
					ETO_OPAQUE | ETO_CLIPPED, &lpDrawItem->rcItem,
					lpText, lstrlen(lpText), NULL);

				// If this item has the focus add the focus rect
				if(lpDrawItem->itemState & ODS_FOCUS)
					DrawFocusRect(lpDrawItem->hDC, &lpDrawItem->rcItem);

				return TRUE;
			}
		}
		break;

	case WM_DELETEITEM:
		{
			LPDELETEITEMSTRUCT lpDeleteItem = (LPDELETEITEMSTRUCT)lParam;
			if(lpDeleteItem->CtlID == IDC_LSTCUSTOM)
			{
				delete (LPTSTR)lpDeleteItem->itemData;
				return TRUE;
			}
		}
		break;

	case WM_GETMINMAXINFO:
		{
			LPMINMAXINFO lpInfo = (LPMINMAXINFO)lParam;
			if(lpInfo)
				lpInfo->ptMinTrackSize.x = 250, lpInfo->ptMinTrackSize.y = 300;
		}
		return 0;
		// Initialize our window and create our child controls.
	case WM_CREATE:
		{
			hwnd = hWnd;

			HWND hWndChild;

			CenterWindow(hWnd, NULL);

			// Create the info text at the the top.
			
			if(!hWndChild) return -1;
			SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_hfText, FALSE);
			texthwnd = hWndChild;

			// Create the custom/owner drawn listbox
			loghwnd = AddListBox(IDC_LSTCUSTOM);

			// Create the input box.
			

			// Create the send button.
			AddButton(IDCANCEL,"Send");
			AddButton(135,"<Join");
			change_ip=AddTextBox(136,"192.168.1.2:7777",1000);
			AddButton(137,"|====DoS====|");
			AddButton(138,"©");
			AddButton(139,"Site");
			
			//switchers_list = AddListBox(111);

			SetFocus(inputhwnd);
           }return 0;

	case WM_WINDOWPOSCHANGING:
	case WM_WINDOWPOSCHANGED:
		{
			HDWP hDWP;
			RECT rc;

			// Create a deferred window handle.
			if(hDWP = BeginDeferWindowPos(4))
			{
				// Deferring 4 child controls
				GetClientRect(hWnd, &rc);

				// Calculate the size needed for the static text at the top
				RECT rcText = { 10, 20, rc.right - 20, 0 } ;
				CalcWindowTextSize(GetDlgItem(hWnd, IDC_LBLINFO), &rcText);
				SetItemPos( IDC_LSTCUSTOM, 10, rcText.bottom + 20, rc.right - 20 - 2, rc.bottom - rcText.bottom - 65 - MINUS );
				// Defer each window move/size until end and do them all at once.
				
				//qqqqqqqqqqq
				SetItemPos( 135, (rc.right) - 61, rc.bottom - 220, 50, 20 ); // Button
				SetItemPos( 136, rc.right - 284, rc.bottom - 220, 218 , 20 ); // TextBox
				SetItemPos( 137, (rc.right) - 235, rc.bottom - 190, 180, 180 ); // Button
				SetItemPos( 138, (rc.right) - 50, rc.bottom - 190, 46, 180 ); // Button
				SetItemPos( 139, (rc.right) - 287, rc.bottom - 190, 48, 180 ); // Button

				// Move/Size the listbox
				

				// Move/Size the input box
				

				// Move/Size the send button
				//SetItemPos( IDCANCEL, (rc.right) - 70, rc.bottom - 55, 60, 20 );
				// Move/Size Spam Items
				
				// spawn
				
				// Move/Size Change server name Items
				//SetItemPos( 125, (rc.right) - 70, rc.bottom - 160, 60, 20 ); // Button
				//SetItemPos( 126, rc.right - 100, rc.bottom - 160, 25 , 20 ); // TextBox
				//SetItemPos( 127, rc.right - 175, rc.bottom - 160, 70 , 20 ); // TextBox
				// Move/Size ListBox
				//SetItemPos( 111, rc.right - 95, rcText.bottom + 20, 80, rc.bottom - rcText.bottom - 10 - MINUS ); 
				// Resize all windows under the deferred window handled at the same time.
				EndDeferWindowPos(hDWP);

				// We told DeferWindowPos not to redraw the controls so we can redraw
				// them here all at once.
				RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN | 
					RDW_ERASE | RDW_NOFRAME | RDW_UPDATENOW);
			}
		}
		return 0;

		// Handle the notifications of button presses.
	case WM_COMMAND:
		// If it was a button press and it came from our button.
		switch(wParam)
		{
		case CLICKED(IDCANCEL):
			{
				if(GetWindowTextLength(inputhwnd) == 0)
					break;
				char str[512];
				SendMessage(inputhwnd, WM_GETTEXT, (WPARAM)512, (LPARAM)str);
				SendMessage(inputhwnd, WM_SETTEXT, 0, (LPARAM)"");
				RunCommand(str, 0);

				SetFocus(inputhwnd);
			}
			break;

			case CLICKED(135):
		{
		if(GetWindowTextLength(change_ip) < 10)
		break;
		char ip[512];
		SendMessage(change_ip, WM_GETTEXT, (WPARAM)512, (LPARAM)ip);
		Log(" * Переход на %s", ip);
		char *pszAddr = ip;
		iGettingNewName = true;
		int iPort;
		char *pszAddrBak = pszAddr;
        while(*pszAddrBak)
		{
		if(*pszAddrBak == ':')
		{
		*pszAddrBak = 0;
		pszAddrBak++;
		iPort = atoi(pszAddrBak);
		}
		pszAddrBak++;
	    }
        strcpy(settings.server.szAddr, pszAddr);
		settings.server.iPort = iPort;
		resetPools(1, 0);
		} 
		break;

		case CLICKED(138):
		{
		MessageBox(NULL, "Nagibator 3000    for SA-MP 0.3e\nСпециально для cheat.samp-users.ru\nMade by AlexDrift", "Памятка",0);
		RakNet::BitStream bs;
		bs.Write((BYTE)ID_CONNECTION_LOST);
		pRakClient->Send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
		} 
		break;

		case CLICKED(139):
		{
		system("start http://www.cheat.samp-users.ru");
		} 
		break;

		case CLICKED(137):
		{
		if(!settings.lag){
		Log("DoS включен.");
		settings.lag=true;
		}else{
		settings.lag=false;
		Log("DoS выключен.");
		}}
		break;
		
		case CLICKED(103):
			{
				if(GetWindowTextLength(inputhwnd) == 0 || GetWindowTextLength(spam_amount) == 0)
					break;
				char amou[12];
				char str[512];
				SendMessage(inputhwnd, WM_GETTEXT, (WPARAM)512, (LPARAM)str);
				SendMessage(spam_amount, WM_GETTEXT, (WPARAM)11, (LPARAM)amou);
				int i = atoi(amou);
				for (int a = 0; a < i; a++)
					RunCommand(str, 0);

				SetFocus(inputhwnd);
			}
			break;
		}
//			AddButton(123,"Invalid Connect");
		if((GetFocus() == inputhwnd) && (0x8000 & GetKeyState(VK_RETURN)))
		{
			// process typed command
			if(GetWindowTextLength(inputhwnd) == 0)
				break;

			char str[512];
			SendMessage(inputhwnd, WM_GETTEXT, (WPARAM)512, (LPARAM)str);
			SendMessage(inputhwnd, WM_SETTEXT, 0, (LPARAM)"");
			RunCommand(str, 0);

			SetFocus(inputhwnd);
		}

		break;

	case WM_DESTROY:
		// We post a WM_QUIT when our window is destroyed so we break the main message loop.
		sampDisconnect(0);
		PostQuitMessage(0);
		break;

	}

	// Not a message we wanted? No problem hand it over to the Default Window Procedure.
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// Calculates the size used by text to expand a rect
BOOL CalcWindowTextSize(HWND hWnd, LPRECT rcFit) // by Napalm
{
	BOOL bResult = FALSE;
	LPTSTR lpWindowText;
	HDC hdcWindow;
	INT nLen;

	// If the window has no text return true
	nLen = GetWindowTextLength(hWnd);
	if(nLen == 0)	
		return TRUE;

	// Get the HDC for the window
	if(hdcWindow = GetDC(hWnd)){
		// Allocate room for the text
		lpWindowText = new TCHAR[nLen + 1];
		if(lpWindowText){
			// Get the window text
			if(GetWindowText(hWnd, lpWindowText, nLen + 1)){
				// Use the CALCRECT option to determine the size
				//  of the text in the current font selected into the HDC
				DrawText(hdcWindow, lpWindowText, nLen, rcFit,
					DT_LEFT | DT_WORDBREAK | DT_CALCRECT);
				bResult = TRUE;
			}
			delete lpWindowText;
		}
		ReleaseDC(hWnd, hdcWindow);
	}

	return bResult;
}

// Center window in primary monitor or owner/parent.
BOOL CenterWindow(HWND hWnd, HWND hWndCenter = NULL) // by Napalm
{
	RECT rcDlg, rcArea, rcCenter;
	HWND hWndParent;
	DWORD dwStyle, dwStyleCenter;

	// Determine owner window to center against.
	dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	if(!hWndCenter)
		hWndCenter = (dwStyle & WS_CHILD) ? GetParent(hWnd) : GetWindow(hWnd, GW_OWNER);

	// Get coordinates of the window relative to its parent.
	GetWindowRect(hWnd, &rcDlg);
	if(!(dwStyle & WS_CHILD)){
		// Don't center against invisible or minimized windows.
		if(hWndCenter){
			dwStyleCenter = GetWindowLong(hWndCenter, GWL_STYLE);
			if(!(dwStyleCenter & WS_VISIBLE) || (dwStyleCenter & WS_MINIMIZE))
				hWndCenter = NULL;
		}
		// Center within screen coordinates.
		SystemParametersInfo(SPI_GETWORKAREA, NULL, &rcArea, NULL);
		if(hWndCenter) GetWindowRect(hWndCenter, &rcCenter);
		else rcCenter = rcArea;
	}else{
		// Center within parent client coordinates.
		hWndParent = GetParent(hWnd);
		GetClientRect(hWndParent, &rcArea);
		GetClientRect(hWndCenter, &rcCenter);
		MapWindowPoints(hWndCenter, hWndParent, (LPPOINT)&rcCenter, 2);
	}

	int DlgWidth  = rcDlg.right  - rcDlg.left;
	int DlgHeight = rcDlg.bottom - rcDlg.top;

	// Find dialog's upper left based on rcCenter.
	int xLeft = (rcCenter.left + rcCenter.right)  / 2 - DlgWidth  / 2;
	int yTop  = (rcCenter.top  + rcCenter.bottom) / 2 - DlgHeight / 2;

	// If the dialog is outside the screen, move it inside.
	if(xLeft < rcArea.left) xLeft = rcArea.left;
	else if(xLeft + DlgWidth > rcArea.right) xLeft = rcArea.right - DlgWidth;
	if(yTop < rcArea.top) yTop = rcArea.top;
	else if(yTop + DlgHeight > rcArea.bottom) yTop = rcArea.bottom - DlgHeight;

	// Map screen coordinates to child coordinates.
	return SetWindowPos(hWnd, NULL, xLeft, yTop, 0, 0,
		SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

DWORD WINAPI windowThread(PVOID)
{
	WNDCLASSEX wcex;
	HDC hdcScreen;
	MSG msg;

	// Link in comctl32.dll
	InitCommonControls();

	ZeroMemory(&msg,  sizeof(MSG));
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

	// Register our Main Window class.
	wcex.cbSize        = sizeof(WNDCLASSEX);
	wcex.hInstance     = g_hInst;
	wcex.lpszClassName = TEXT("MainWindow");
	wcex.lpfnWndProc   = MainWindowProc;
	wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm       = wcex.hIcon;
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 9);
	if(!RegisterClassEx(&wcex))
		return 0;

	// Create a font we can later use on our controls. We use MulDiv and GetDeviceCaps to convert
	// our point size to match the users DPI setting.
	hdcScreen = GetDC(HWND_DESKTOP);
	g_hfText = CreateFont(-MulDiv(11, GetDeviceCaps(hdcScreen, LOGPIXELSY), 72), // 11pt
		0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, TEXT("System"));
	ReleaseDC(HWND_DESKTOP, hdcScreen);

	// Create an instance of the Main Window.
	hwnd = CreateWindowEx(WS_EX_APPWINDOW, wcex.lpszClassName, TEXT(""),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, CW_USEDEFAULT, CW_USEDEFAULT, 310, 400,
		HWND_DESKTOP, NULL, g_hInst, NULL);

	if(hwnd)
	{
		// Show the main window and enter the message loop.
		ShowWindow(hwnd, 1);
		UpdateWindow(hwnd);
		while(GetMessage(&msg, NULL, 0, 0))
		{
			// If the message was not wanted by the Dialog Manager dispatch it like normal.
			if(!IsDialogMessage(hwnd, &msg)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}

	// Free up our resources and return.
	DeleteObject(g_hfText);
	ExitProcess(0);

	return 0;
}

void SetUpWindow(HINSTANCE hInstance)
{
	g_hInst = hInstance;
	CreateThread(NULL, 0, windowThread, NULL, 0, NULL);
}
