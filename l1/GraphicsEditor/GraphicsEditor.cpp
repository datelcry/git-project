// GraphicsEditor.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <windows.h>
#include <CommDlg.h>
#include <stdio.h>
#include <string>
#include "GraphicsEditor.h"

using namespace std;

#define MAX_LOADSTRING 100
#define ID_BUTTONLINE 1000
#define ID_BUTTTONRECTANGLE 1001
#define ID_BUTTONELLIPSE 1002
#define ID_BUTTONPOLYGON 1003
#define ID_BUTTONPOLYLINE 1004
#define ID_EDIT 1005
#define ID_BUTTONTEXT 1006
#define CM_FILE_OPEN 1007
#define CM_FILE_SAVE 1008
#define FILE_NAME1 "Metafile1.emf"
#define FILE_NAME2 "Metafile2.emf"
#define ID_BUTTONCOLORPEN 1009
#define ID_BUTTONZOOM 1010
#define ID_BUTTONPAN 1011
#define ID_BUTTONPRINT 1012
#define ID_BUTTONNEW 1013
#define ID_BUTTONWIDTH 1014
#define ID_BUTTONWIDTH1 1015
#define ID_BUTTONWIDTH2 1016
#define ID_BUTTONWIDTH3 1017
#define ID_BUTTONWIDTH4 1018
#define ID_BUTTONWIDTH5 1019
#define ID_BUTTONWIDTH6 1020
#define ID_BUTTONCOLORBRUSH 1021
#define ID_BUTTONCOLOR 1022
#define ID_BUTTONCOLORNONE 1023
#define ID_BUTTONCOLORFILL 1024
#define ID_BUTTONRHOMB 1025

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
static int width=0;



// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GRAPHICSEDITOR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GRAPHICSEDITOR));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GRAPHICSEDITOR));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_GRAPHICSEDITOR);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle,WS_OVERLAPPEDWINDOW|WS_MAXIMIZE,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, SW_MAXIMIZE);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//

BOOL CreateMenuItem(HMENU hMenu,char *str,UINT uIns,UINT uCom,HMENU hSubMenu,BOOL flag,UINT fType)
	{
	   MENUITEMINFO mii;
	   mii.cbSize=sizeof(MENUITEMINFO);
	   mii.fMask=MIIM_STATE|MIIM_TYPE|MIIM_SUBMENU|MIIM_ID;
	   mii.fType=fType;
	   mii.fState=MFS_ENABLED;
	   mii.dwTypeData=str;
	   mii.cch=sizeof(str);
	   mii.wID=uCom;
	   mii.hSubMenu=hSubMenu;
	   return InsertMenuItem(hMenu,uIns,flag,&mii);
	}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC static hdc,hdc1,hdc2;
	static HDC hCompatibleDC=0;
	static HDC hBitmapDC=0;
	HDC static hdcSaveFile;
	HWND static hButtonLine,hButtonRectangle,hButtonEllipse,hButtonPolyline,hButtonPolygon,hEdit,
		hButtonText,hButtonRhomb,hButtonZoom,hButtonPan,hButtonColorPen;
	static CHOOSECOLOR cc1,cc2;
	int static x0,y0,x1,y1,x2,y2,oldMixMode;
	HENHMETAFILE hEnhMtf;
	static bool b,bPoly,bText,flag,bPolyline;
	static bool bScaled=FALSE;
	static int id;
	int i;
	static HBRUSH hBrush;
	static HPEN hPen;
	static HMENU hMainMenu,hFileMenu,hShapeMenu,hActionMenu,hButtonWidth,hButtonColor,hButtonColorBrush;
    static OPENFILENAME ofn;
    static char fullpath[256],filename[256],dir[256];
	ENHMETAHEADER emh;
	RECT rect,rect1;
	static HBITMAP hCompatibleBitmap,hBitmap;
	static COLORREF  crCustColor[16];
	static int xBegin=0,yBegin=0,fl=0;
	PRINTDLG pd;
	HDC          hdcMem;
	HBITMAP      hbmMem;
	HANDLE       hOld;
	static double scale;
	static int f=0;
	int zDelta;
	TEXTMETRIC tm;
	static string text;
	static int cxChar, cyChar;
	static int xT,yT;
	static DOCINFO di;

	switch (message)
	{
	case WM_CREATE:
		ShowWindow(hWnd, SW_MAXIMIZE);
		hdc=GetDC(hWnd);	
		//hEdit=CreateWindow("edit",NULL,WS_VISIBLE|WS_CHILD|WS_BORDER,
    	//		0,0,100,20,hWnd,(HMENU)ID_EDIT,hInst,NULL);		
		scale=1;
		hMainMenu=CreateMenu();
		hFileMenu=CreatePopupMenu();
		hShapeMenu=CreatePopupMenu();
		hActionMenu=CreatePopupMenu();
		hButtonWidth=CreatePopupMenu();
		hButtonColor=CreatePopupMenu();
		hButtonColorBrush=CreatePopupMenu();
		i=0;
		CreateMenuItem(hFileMenu,"&New",i++,ID_BUTTONNEW,NULL,FALSE,MFT_STRING);
		CreateMenuItem(hFileMenu,"&Open",i++,CM_FILE_OPEN,NULL,FALSE,MFT_STRING);
		CreateMenuItem(hFileMenu,"&Save",i++,CM_FILE_SAVE,NULL,FALSE,MFT_STRING);
		CreateMenuItem(hFileMenu,"&Print",i++,ID_BUTTONPRINT,NULL,FALSE,MFT_STRING);
        i=0; 
		CreateMenuItem(hShapeMenu,"&Линия",i++,ID_BUTTONLINE,NULL,FALSE,MFT_STRING);
		CreateMenuItem(hShapeMenu,"&Rectangle",i++,ID_BUTTTONRECTANGLE,NULL,FALSE,MFT_STRING);
		CreateMenuItem(hShapeMenu,"&Ellipse",i++,ID_BUTTONELLIPSE,NULL,FALSE,MFT_STRING);
		CreateMenuItem(hShapeMenu,"&Polyline",i++,ID_BUTTONPOLYLINE,NULL,FALSE,MFT_STRING);
		CreateMenuItem(hShapeMenu,"&Polygon",i++,ID_BUTTONPOLYGON,NULL,FALSE,MFT_STRING);
		CreateMenuItem(hShapeMenu,"&Text",i++,ID_BUTTONTEXT,NULL,FALSE,MFT_STRING);
		CreateMenuItem(hShapeMenu,"&Rhomb",i++,ID_BUTTONRHOMB,NULL,FALSE,MFT_STRING);
		i=0;
		CreateMenuItem(hButtonWidth,"&1",i++,ID_BUTTONWIDTH1,NULL,FALSE,MFT_STRING);
		CreateMenuItem(hButtonWidth,"&2",i++,ID_BUTTONWIDTH2,NULL,FALSE,MFT_STRING);
		CreateMenuItem(hButtonWidth,"&3",i++,ID_BUTTONWIDTH3,NULL,FALSE,MFT_STRING);
		CreateMenuItem(hButtonWidth,"&4",i++,ID_BUTTONWIDTH4,NULL,FALSE,MFT_STRING);
		CreateMenuItem(hButtonWidth,"&5",i++,ID_BUTTONWIDTH5,NULL,FALSE,MFT_STRING);
		CreateMenuItem(hButtonWidth,"&6",i++,ID_BUTTONWIDTH6,NULL,FALSE,MFT_STRING);

		i=0;
		CreateMenuItem(hActionMenu,"&Zoom",i++,ID_BUTTONZOOM,NULL,FALSE,MFT_STRING);
		CreateMenuItem(hActionMenu,"&Pan",i++,ID_BUTTONPAN,NULL,FALSE,MFT_STRING);


		i=0;
		CreateMenuItem(hButtonColorBrush,"&None",i++,ID_BUTTONCOLORNONE,NULL,FALSE,MFT_STRING);
		CreateMenuItem(hButtonColorBrush,"&Fill",i++,ID_BUTTONCOLORFILL,NULL,FALSE,MFT_STRING);

		i=0;
		CreateMenuItem(hButtonColor,"&Pen",i++,ID_BUTTONCOLORPEN,NULL,FALSE,MFT_STRING);
		//CreateMenuItem(hButtonColor,"&Brush",i++,ID_BUTTONCOLORBRUSH,NULL,FALSE,MFT_STRING);
		CreateMenuItem(hButtonColor,"&Brush",i++,0,hButtonColorBrush,FALSE,MFT_STRING);

		i=0;
		CreateMenuItem(hMainMenu,"&File",i++,0,hFileMenu,FALSE,MFT_STRING);
		CreateMenuItem(hMainMenu,"&Shape",i++,0,hShapeMenu,FALSE,MFT_STRING);
		CreateMenuItem(hMainMenu,"&Action",i++,0,hActionMenu,FALSE,MFT_STRING);
		CreateMenuItem(hMainMenu,"&Color",i++,0,hButtonColor,FALSE,MFT_STRING);
		CreateMenuItem(hMainMenu,"&Width",i++,0,hButtonWidth,FALSE,MFT_STRING);
		SetMenu(hWnd,hMainMenu);
		DrawMenuBar(hWnd);
		GetClientRect(hWnd,&rect);
		hdc1=CreateEnhMetaFile(NULL,NULL,NULL,NULL);
		flag=false;
		
		hBrush=(HBRUSH)GetStockObject(NULL_BRUSH);
		SelectObject(hdc,hBrush);
		hPen=(HPEN)GetStockObject(BLACK_PEN);
		SelectObject(hdc1,hBrush);
		SelectObject(hdc1,hPen);

		width=0;
		hCompatibleDC=CreateCompatibleDC(hdc);
		hCompatibleBitmap=CreateCompatibleBitmap(hdc,rect.right,rect.bottom);
		hBitmapDC=CreateCompatibleDC(hdc);
		hBitmap=CreateCompatibleBitmap(hdc,rect.right,rect.bottom);
		DeleteObject(SelectObject(hCompatibleDC,hCompatibleBitmap));
		DeleteObject(SelectObject(hCompatibleDC, (HBRUSH) WHITE_BRUSH)); 
		PatBlt(hCompatibleDC, 0, 0, rect.right, rect.bottom, PATCOPY);
		DeleteObject(SelectObject(hBitmapDC,hBitmap));
		DeleteObject(SelectObject(hBitmapDC, (HBRUSH) WHITE_BRUSH)); 
		PatBlt(hBitmapDC, 0, 0, rect.right, rect.bottom, PATCOPY);
		DeleteObject(SelectObject(hCompatibleDC,hPen));
		DeleteObject(SelectObject(hCompatibleDC,hBrush));
		DeleteObject(SelectObject(hBitmapDC,hPen));
		DeleteObject(SelectObject(hBitmapDC,hBrush));

		SelectObject(hdc,GetStockObject(SYSTEM_FIXED_FONT));
		GetTextMetrics(hdc,&tm);
		cxChar=tm.tmAveCharWidth;
		cyChar=tm.tmHeight;
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		bText=false;
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
		case ID_BUTTONLINE:
			id=ID_BUTTONLINE;
			scale=1;
			xBegin=0;
			yBegin=0;
			bPoly=false;
			bPolyline=false;
			InvalidateRect(hWnd,NULL,FALSE);
			break;
		case ID_BUTTONELLIPSE:
			id=ID_BUTTONELLIPSE;
			scale=1;
			xBegin=0;
			yBegin=0;
			bPoly=false;
			bPolyline=false;
			InvalidateRect(hWnd,NULL,FALSE);
			break;
		case ID_BUTTTONRECTANGLE:
			id=ID_BUTTTONRECTANGLE;
			scale=1;
			xBegin=0;
			yBegin=0;
			bPoly=false;
			bPolyline=false;
			InvalidateRect(hWnd,NULL,FALSE);
			break;
		case ID_BUTTONRHOMB:
			id=ID_BUTTONRHOMB;
			scale=1;
			xBegin=0;
			yBegin=0;
			bPoly=false;
			bPolyline=false;
			InvalidateRect(hWnd,NULL,FALSE);
			break;
		case ID_BUTTONPOLYLINE:
			id=ID_BUTTONPOLYLINE;
			scale=1;
			xBegin=0;
			bPoly=false;
			InvalidateRect(hWnd,NULL,FALSE);
			break;
		case ID_BUTTONPOLYGON:
			id=ID_BUTTONPOLYGON;
			scale=1;
			xBegin=0;
			yBegin=0;
			bPolyline=false;
			InvalidateRect(hWnd,NULL,FALSE);
			break;
		case ID_BUTTONTEXT:
			id=ID_BUTTONTEXT;
			scale=1;
			xBegin=0;
			yBegin=0;
			bPoly=false;
			bPolyline=false;
			InvalidateRect(hWnd,NULL,FALSE);
			break;
		case ID_BUTTONZOOM:
			id=ID_BUTTONZOOM;
			bScaled=TRUE;
			InvalidateRect(hWnd,NULL,FALSE);
			break;
		case ID_BUTTONPAN:
			id=ID_BUTTONPAN;
			break;
		case ID_BUTTONNEW:
			GetClientRect(hWnd,&rect);
			hdc1=CreateEnhMetaFile(NULL,NULL,NULL,NULL);
			flag=false;
			DeleteObject(SelectObject(hdc1,hBrush));
			DeleteObject(SelectObject(hdc1,hPen));

			width=0;
			DeleteObject(hCompatibleDC);
			DeleteObject(hBitmapDC);
			hCompatibleDC=CreateCompatibleDC(hdc);
			hCompatibleBitmap=CreateCompatibleBitmap(hdc,rect.right,rect.bottom);
			DeleteObject(SelectObject(hCompatibleDC,hCompatibleBitmap));
			DeleteObject(SelectObject(hCompatibleDC, (HBRUSH) WHITE_BRUSH)); 
			PatBlt(hCompatibleDC, 0, 0, rect.right, rect.bottom, PATCOPY);
			DeleteObject(SelectObject(hCompatibleDC,hPen));
			DeleteObject(SelectObject(hCompatibleDC,hBrush));
			hBitmapDC=CreateCompatibleDC(hdc);
			hBitmap=CreateCompatibleBitmap(hdc,rect.right,rect.bottom);
			DeleteObject(SelectObject(hBitmapDC,hBitmap));
			DeleteObject(SelectObject(hBitmapDC, (HBRUSH) WHITE_BRUSH)); 
			PatBlt(hBitmapDC, 0, 0, rect.right, rect.bottom, PATCOPY);
			DeleteObject(SelectObject(hBitmapDC,hPen));
			DeleteObject(SelectObject(hBitmapDC,hBrush));
			InvalidateRect(hWnd,NULL,TRUE);
			UpdateWindow(hWnd);
			break;
		case ID_BUTTONWIDTH1:
			width=1;
			DeleteObject(hPen);
			hPen = CreatePen(PS_SOLID, width, cc1.rgbResult);
			DeleteObject(SelectObject(hCompatibleDC, hPen));
			DeleteObject(SelectObject(hBitmapDC, hPen));
			DeleteObject(SelectObject(hdc1,hPen));
			break;
		case ID_BUTTONWIDTH2:
			width=2;
			DeleteObject(hPen);
			hPen = CreatePen(PS_SOLID, width, cc1.rgbResult);
			DeleteObject(SelectObject(hCompatibleDC, hPen));
			DeleteObject(SelectObject(hBitmapDC, hPen));
			DeleteObject(SelectObject(hdc1,hPen));
			break;
		case ID_BUTTONWIDTH3:
			width=3;
			DeleteObject(hPen);
			hPen = CreatePen(PS_SOLID, width, cc1.rgbResult);
			DeleteObject(SelectObject(hCompatibleDC, hPen));
			DeleteObject(SelectObject(hBitmapDC, hPen));
			DeleteObject(SelectObject(hdc1,hPen));
			break;
		case ID_BUTTONWIDTH4:
			width=4;
			DeleteObject(hPen);
			hPen = CreatePen(PS_SOLID, width, cc1.rgbResult);
			DeleteObject(SelectObject(hCompatibleDC, hPen));
			DeleteObject(SelectObject(hBitmapDC, hPen));
			DeleteObject(SelectObject(hdc1,hPen));
			break;
		case ID_BUTTONWIDTH5:
			width=5;
			DeleteObject(hPen);
			hPen = CreatePen(PS_SOLID, width, cc1.rgbResult);
			DeleteObject(SelectObject(hCompatibleDC, hPen));
			DeleteObject(SelectObject(hBitmapDC, hPen));
			DeleteObject(SelectObject(hdc1,hPen));
			break;
		case ID_BUTTONWIDTH6:
			width=6;
			DeleteObject(hPen);
			hPen = CreatePen(PS_SOLID, width, cc1.rgbResult);
			DeleteObject(SelectObject(hCompatibleDC, hPen));
			DeleteObject(SelectObject(hBitmapDC, hPen));
			DeleteObject(SelectObject(hdc1,hPen));
			break;
		case ID_BUTTONPRINT:
			// Заполняем PRINTDLG
			ZeroMemory(&pd, sizeof(pd));
			pd.lStructSize = sizeof(pd);
			pd.hwndOwner   = hWnd;
			pd.hDevMode    = NULL; // Не забудьте освободить или сохранить hDevMode
			pd.hDevNames   = NULL; // Не забудьте освободить или сохранить hDevNames
			pd.Flags       = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC;
			pd.nCopies     = 1;
			pd.nFromPage   = 0xFFFF;
			pd.nToPage     = 0xFFFF;
			pd.nMinPage    = 1;
			pd.nMaxPage    = 0xFFFF;


			if (PrintDlg(&pd)==TRUE)
			{
				int Rx = GetDeviceCaps(pd.hDC, LOGPIXELSX);
				int Ry = GetDeviceCaps(pd.hDC, LOGPIXELSY);
				di.cbSize=sizeof(DOCINFO);
				di.lpszDocName="Print Picture";
				di.fwType=NULL;
				di.lpszDatatype=NULL;
				di.lpszOutput=NULL;
				// Теперь можно что-нибудь вывести на печать
				StartDoc(pd.hDC,&di);
				StartPage(pd.hDC);
				GetClientRect(hWnd,&rect);
				hdcMem = CreateCompatibleDC(hdc);
				hbmMem = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
				int Rx1 = GetDeviceCaps(hdcMem, LOGPIXELSX);
				int Ry1 = GetDeviceCaps(hdcMem, LOGPIXELSY);
				hOld=SelectObject(hdcMem,hbmMem);
				FillRect(hdcMem,&rect,WHITE_BRUSH);
				StretchBlt(hdcMem,0,0,(int)(rect.right*scale),(int)(rect.bottom*scale),
					hBitmapDC,xBegin,yBegin,rect.right,rect.bottom,SRCCOPY);
				SelectObject(hdcMem,(HBRUSH)GetStockObject(NULL_BRUSH));
				SelectObject(hdcMem,(HPEN)GetStockObject(BLACK_PEN));
				Rectangle(hdcMem,0,0,(int)(rect.right*scale),(int)(rect.bottom*scale));
				//BitBlt(pd.hDC,0,0,rect.right,rect.bottom,hdcMem,0,0,SRCCOPY);
				StretchBlt(pd.hDC, 0, 0,(int)((float)(0.91*rect.right*((float)(Rx/Rx1)))), (int)((float)(0.91*rect.bottom*((float)(Ry/Ry1)))),
				hdcMem, 0, 0, rect.right, rect.bottom, SRCCOPY);
				SelectObject(hdcMem, hOld);
				DeleteObject(hbmMem);
				DeleteDC    (hdcMem);
				EndPage(pd.hDC);
				EndDoc(pd.hDC);
				// По завершению удаляем DC.
				DeleteDC(pd.hDC);
			}
			break;
		case CM_FILE_OPEN:
			ofn.lStructSize=sizeof(OPENFILENAME);
			ofn.hwndOwner=hWnd;
			ofn.hInstance=hInst; // дескриптор копии приложения
			ofn.lpstrFilter="Metafile (*.emf)\0*.emf\0Все файлы (*.*)\0*.*\0";
			ofn.nFilterIndex=1;
			ofn.lpstrFile=fullpath;
    		ofn.nMaxFile=sizeof(fullpath);
			ofn.lpstrFileTitle=filename;
			ofn.nMaxFileTitle=sizeof(filename);
			ofn.lpstrInitialDir=dir;
			ofn.lpstrTitle="Open file...";
			ofn.Flags=OFN_EXPLORER|OFN_CREATEPROMPT|OFN_ALLOWMULTISELECT;
			if(GetOpenFileName(&ofn))
			{
				scale=1;
				xBegin=0;
				yBegin=0;
                hdc1=CreateEnhMetaFile(NULL,NULL,NULL,NULL); 
			    hEnhMtf=GetEnhMetaFile(fullpath);
				GetEnhMetaFileHeader(hEnhMtf,sizeof(ENHMETAHEADER),&emh);
				SetRect(&rect,emh.rclBounds.left,emh.rclBounds.top,emh.rclBounds.right,emh.rclBounds.bottom);
				DeleteObject(SelectObject(hdc1,hBrush));
				DeleteObject(SelectObject(hdc1,hPen));
				PlayEnhMetaFile(hdc1,hEnhMtf,&rect);
				
				GetClientRect(hWnd,&rect1);
				DeleteDC(hBitmapDC);
				hBitmapDC=CreateCompatibleDC(hdc);
				hBitmap=CreateCompatibleBitmap(hdc,rect1.right,rect1.bottom);
				DeleteObject(SelectObject(hBitmapDC,hBitmap));
				DeleteObject(SelectObject(hBitmapDC, (HBRUSH) WHITE_BRUSH)); 
				PatBlt(hBitmapDC, 0, 0, rect1.right, rect1.bottom, PATCOPY);
				DeleteObject(SelectObject(hBitmapDC,hPen));
				DeleteObject(SelectObject(hBitmapDC,hBrush));
				PlayEnhMetaFile(hBitmapDC,hEnhMtf,&rect);

				InvalidateRect(hWnd,NULL,TRUE);
				UpdateWindow(hWnd);
				DeleteEnhMetaFile(hEnhMtf);
			}
		    break;
		case CM_FILE_SAVE:
			ofn.lStructSize=sizeof(OPENFILENAME);
			ofn.hwndOwner=hWnd;
			ofn.hInstance=hInst; // дескриптор копии приложения
			ofn.lpstrFilter="Metafile (*.emf)\0*.emf\0Все файлы (*.*)\0*.*\0";
			ofn.nFilterIndex=1;
			ofn.lpstrFile=fullpath;
			ofn.nMaxFile=sizeof(fullpath);
			ofn.lpstrFileTitle=filename;
			ofn.nMaxFileTitle=sizeof(filename);
			ofn.lpstrInitialDir=dir;
			ofn.lpstrTitle="Save file as...";
			ofn.Flags=OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY|OFN_EXPLORER;
			if(GetSaveFileName(&ofn))
			{
		    	hEnhMtf=CloseEnhMetaFile(hdc1);
				CopyEnhMetaFile(hEnhMtf,fullpath);
				hdc1=CreateEnhMetaFile(NULL,NULL,NULL,NULL);
				GetEnhMetaFileHeader(hEnhMtf,sizeof(ENHMETAHEADER),&emh);
				SetRect(&rect,emh.rclBounds.left,emh.rclBounds.top,emh.rclBounds.right,emh.rclBounds.bottom);
				DeleteObject(SelectObject(hdc1,hBrush));
				DeleteObject(SelectObject(hdc1,hPen));
				PlayEnhMetaFile(hdc1,hEnhMtf,&rect);
				CloseEnhMetaFile((HDC)hEnhMtf);
				DeleteEnhMetaFile(hEnhMtf);
			    //CloseHandle((HDC)hEnhMtf);
			}
			break;
		case ID_BUTTONCOLORPEN:
			cc1.lStructSize = sizeof(CHOOSECOLOR);
			cc1.hInstance = NULL;
			cc1.hwndOwner = hWnd;
			cc1.lpCustColors = crCustColor;
			cc1.Flags = CC_RGBINIT|CC_FULLOPEN;
			cc1.lCustData = 0L;
			cc1.lpfnHook = NULL;
			cc1.rgbResult = RGB(0x80, 0x80, 0x80);
			cc1.lpTemplateName = NULL;

			if(ChooseColor(&cc1))
			{
				DeleteObject(hPen);
				hPen = CreatePen(PS_SOLID, width, cc1.rgbResult);
				DeleteObject(SelectObject(hCompatibleDC, hPen));
				DeleteObject(SelectObject(hBitmapDC,hPen));
				DeleteObject(SelectObject(hdc1,hPen));
			}

			break;
		case ID_BUTTONCOLORFILL:
			cc2.lStructSize = sizeof(CHOOSECOLOR);
			cc2.hInstance = NULL;
			cc2.hwndOwner = hWnd;
			cc2.lpCustColors = crCustColor;
			cc2.Flags = CC_RGBINIT|CC_FULLOPEN;
			cc2.lCustData = 0L;
			cc2.lpfnHook = NULL;
			cc2.rgbResult = RGB(0x80, 0x80, 0x80);
			cc2.lpTemplateName = NULL;

			if(ChooseColor(&cc2))
			{
				DeleteObject(hBrush);
				hBrush=CreateSolidBrush(cc2.rgbResult);
				DeleteObject(SelectObject(hCompatibleDC, hBrush));
				DeleteObject(SelectObject(hBitmapDC,hBrush));
				DeleteObject(SelectObject(hdc1,hBrush));
			}
			break;
		case ID_BUTTONCOLORNONE:
			DeleteObject(hBrush);
			hBrush=(HBRUSH)GetStockObject(NULL_BRUSH);
			DeleteObject(SelectObject(hCompatibleDC, hBrush));
			DeleteObject(SelectObject(hBitmapDC,hBrush));
			DeleteObject(SelectObject(hdc1,hBrush));
			break;
		default:
			id=0;
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_MOUSEMOVE:
		GetClientRect(hWnd,&rect);
		if(b&&(bPoly==false)&&bText==false)
			{
				hCompatibleBitmap=CreateCompatibleBitmap(hdc,rect.right,rect.bottom);
				DeleteObject(SelectObject(hCompatibleDC,hCompatibleBitmap));
				BitBlt(hCompatibleDC,0,0,rect.right,rect.bottom,hBitmapDC,0,0,SRCCOPY);
			  switch(id)
			  {
				  case ID_BUTTONLINE:
			        x2=(short)LOWORD(lParam);
			        y2=(short)HIWORD(lParam);
			        MoveToEx(hCompatibleDC,x1,y1,NULL);
			        LineTo(hCompatibleDC,x2,y2);
					break;
				  case ID_BUTTTONRECTANGLE:
					  x2=(short)LOWORD(lParam);
					  y2=(short)HIWORD(lParam);
					  Rectangle(hCompatibleDC,x1,y1,x2,y2);
					  break;
				  case ID_BUTTONRHOMB:
					  x2=(short)LOWORD(lParam);
					  y2=(short)HIWORD(lParam);
					  MoveToEx(hCompatibleDC,(x1+x2)/2,y1,NULL);
					  LineTo(hCompatibleDC,x2,(y1+y2)/2);
					  LineTo(hCompatibleDC,(x1+x2)/2,y2);
					  LineTo(hCompatibleDC,x1,(y1+y2)/2);
					  LineTo(hCompatibleDC,(x1+x2)/2,y1);
					  break;
				  case ID_BUTTONELLIPSE:
					  x2=(short)LOWORD(lParam);
					  y2=(short)HIWORD(lParam);
					  Ellipse(hCompatibleDC,x1,y1,x2,y2);
					  break;
				  case ID_BUTTONPAN:
					 x1=(short)((short)LOWORD(lParam)/scale);
					 y1=(short)((short)HIWORD(lParam)/scale);
					 xBegin+=(x2-x1);
					 yBegin+=(y2-y1);
					 x2=x1;
					 y2=y1;
					 break;
			  }
			  f=2;
			  InvalidateRect(hWnd,NULL,FALSE);
			  UpdateWindow(hWnd);
			}
		if(b&&bPoly&&bText==false)
			{
			   hCompatibleBitmap=CreateCompatibleBitmap(hdc,rect.right,rect.bottom);
		       DeleteObject(SelectObject(hCompatibleDC,hCompatibleBitmap));
			   BitBlt(hCompatibleDC,0,0,rect.right,rect.bottom,hBitmapDC,0,0,SRCCOPY);
			   x2=(short)LOWORD(lParam);
			   y2=(short)HIWORD(lParam);
			   MoveToEx(hCompatibleDC,x1,y1,NULL);
			   LineTo(hCompatibleDC,x2,y2);
			   f=2;
			   InvalidateRect(hWnd,NULL,FALSE);
			   UpdateWindow(hWnd);
			}
		break;
	case WM_LBUTTONDOWN:
		if(id==ID_BUTTONZOOM)
			id=ID_BUTTONPAN;
		switch(id)
		{
			case ID_BUTTONELLIPSE:
			case ID_BUTTONLINE:
			case ID_BUTTONRHOMB:
            case ID_BUTTTONRECTANGLE:
				x1=x2=(short)LOWORD(lParam);
				y1=y2=(short)HIWORD(lParam);
				//if((id==ID_BUTTONLINE))
				//	MoveToEx(hBitmapDC,x1,y1,NULL);
				break;
			case ID_BUTTONPOLYLINE:
            case ID_BUTTONPOLYGON:
				if (bPolyline==true)
					bPolyline=false;
				if(bPoly==false)
				{
			     	x0=x1=x2=(short)LOWORD(lParam);
				    y0=y1=y2=(short)HIWORD(lParam);
					bPoly=true;
			    }
				break;
		    break;
			case ID_BUTTONTEXT:
				bText=true;
				break;
			case ID_BUTTONPAN:
				x1=x2=(short)((short)LOWORD(lParam)/scale);
				y1=y2=(short)((short)HIWORD(lParam)/scale);
			break;
		}
		SetCapture(hWnd);
		b=true;
		break;
	case WM_RBUTTONUP:
		if(bPoly)
		{
		   
		   x2=(short)LOWORD(lParam);
		   y2=(short)HIWORD(lParam);
		   MoveToEx(hdc1,x1,y1,NULL);
		   LineTo(hdc1,x2,y2);
		   MoveToEx(hBitmapDC,x1,y1,NULL);
		   LineTo(hBitmapDC,x2,y2);
		   if(id==ID_BUTTONPOLYGON)
		   {
			  LineTo(hdc1,x0,y0);
			  LineTo(hBitmapDC,x0,y0);
		   }
		   bPoly=false;
		   bPolyline = false;
		   f=1;
		   InvalidateRect(hWnd,NULL,FALSE);
		   UpdateWindow(hWnd);
		}

	case WM_LBUTTONUP:
		ReleaseCapture();
		if(bText)
		{
			xT=(short)LOWORD(lParam);
			yT=(short)HIWORD(lParam);
			text.clear();
			b=false;
			break;
		}
		GetClientRect(hWnd,&rect);
		hCompatibleBitmap=CreateCompatibleBitmap(hdc,rect.right,rect.bottom);
		DeleteObject(SelectObject(hCompatibleDC,hCompatibleBitmap));
		BitBlt(hCompatibleDC,0,0,rect.right,rect.bottom,hBitmapDC,0,0,SRCCOPY);
		if(b&&(bPoly==false))
		{
	    	x2=(short)LOWORD(lParam);
	    	y2=(short)HIWORD(lParam);
		    switch (id)
		    {
		    	case ID_BUTTONLINE:
			    	MoveToEx(hdc1,x1,y1,NULL);
				    LineTo(hdc1,x2,y2);
					MoveToEx(hBitmapDC,x1,y1,NULL);
					LineTo(hBitmapDC,x2,y2);
				    break;
		     	case ID_BUTTONELLIPSE:
			     	Ellipse(hdc1,x1,y1,x2,y2);
					Ellipse(hBitmapDC,x1,y1,x2,y2);
			    	break;
			    case ID_BUTTTONRECTANGLE:
				    Rectangle(hdc1,x1,y1,x2,y2);
					Rectangle(hBitmapDC,x1,y1,x2,y2);
				    break;
				case ID_BUTTONRHOMB:
				  x2=(short)LOWORD(lParam);
				  y2=(short)HIWORD(lParam);
				  MoveToEx(hdc1,(x1+x2)/2,y1,NULL);
					  LineTo(hdc1,x2,(y1+y2)/2);
					  LineTo(hdc1,(x1+x2)/2,y2);
					  LineTo(hdc1,x1,(y1+y2)/2);
					  LineTo(hdc1,(x1+x2)/2,y1);
					  MoveToEx(hBitmapDC,(x1+x2)/2,y1,NULL);
					  LineTo(hBitmapDC,x2,(y1+y2)/2);
					  LineTo(hBitmapDC,(x1+x2)/2,y2);
					  LineTo(hBitmapDC,x1,(y1+y2)/2);
					  LineTo(hBitmapDC,(x1+x2)/2,y1);
					  break;
		    }
			f=1;
			InvalidateRect(hWnd,NULL,FALSE);
			UpdateWindow(hWnd);
	    	b=false;
		}
		if(b&&bPoly)
		{
			MoveToEx(hdc1,x1,y1,NULL);
			MoveToEx(hBitmapDC,x1,y1,NULL);
	    	x2=(short)LOWORD(lParam);
	    	y2=(short)HIWORD(lParam);
			LineTo(hdc1,x2,y2);
			LineTo(hBitmapDC,x2,y2);
			x1=x2;
			y1=y2;
			b=false;
			f=1;
			InvalidateRect(hWnd,NULL,FALSE);
			UpdateWindow(hWnd);
		}
		break;
	case WM_MOUSEWHEEL:
		bText=false;
		if(id!=ID_BUTTONZOOM&&id!=ID_BUTTONPAN)
		{
			scale=1;
		    id=ID_BUTTONPAN;
		}
		zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		if((zDelta>0)&&(scale<3))
		   scale=scale+0.03;
		if((zDelta<0)&&(scale>0.3))
			scale=scale-0.03;
		InvalidateRect(hWnd,NULL,FALSE);
		UpdateWindow(hWnd);
		break;

	case WM_SIZE:
		
		break;
	case  WM_CHAR:
		if(bText)
		{
			char c=(char)wParam;
			if(c==VK_RETURN)
				bText=false;
			else
				text+=c;
			TextOut(hBitmapDC,xT,yT,text.c_str(),strlen(text.c_str()));
			TextOut(hdc1,xT,yT,text.c_str(),strlen(text.c_str()));
			InvalidateRect(hWnd,NULL,FALSE);
			UpdateWindow(hWnd);
		}
		break;

	case WM_PAINT:
		hdc2 = BeginPaint(hWnd, &ps);
		switch (id)
		{
			case ID_BUTTONZOOM:
	    	case ID_BUTTONPAN:
	    	{
			        GetClientRect(hWnd,&rect);
					hdcMem = CreateCompatibleDC(hdc);
					hbmMem = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
					hOld=SelectObject(hdcMem,hbmMem);
					FillRect(hdcMem,&rect,WHITE_BRUSH);
		    		StretchBlt(hdcMem,0,0,(int)(rect.right*scale),(int)(rect.bottom*scale),
					hBitmapDC,xBegin,yBegin,rect.right,rect.bottom,SRCCOPY);
			    	SelectObject(hdcMem,(HBRUSH)GetStockObject(NULL_BRUSH));
			    	SelectObject(hdcMem,(HPEN)GetStockObject(BLACK_PEN));
			    	Rectangle(hdcMem,0,0,(int)(rect.right*scale),(int)(rect.bottom*scale));
					BitBlt(hdc2,0,0,rect.right,rect.bottom,hdcMem,0,0,SRCCOPY);
					SelectObject(hdcMem, hOld);
					DeleteObject(hbmMem);
					DeleteDC    (hdcMem);
	    		break;
	    	}
	    	default:
	    	{
				GetClientRect(hWnd,&rect);
				if(f==0)
				{
					BitBlt(hdc2, 0, 0,rect.right,rect.bottom,hBitmapDC,0, 0, SRCCOPY);
				}
				if(f==1)
				{
					//DeleteObject(SelectObject(hBitmapDC,hCompatibleBitmap));
					//BitBlt(hBitmapDC, 0, 0,rect.right,rect.bottom,hCompatibleDC,0, 0, SRCCOPY);
					BitBlt(hdc2, 0, 0,rect.right,rect.bottom,hBitmapDC,0, 0, SRCCOPY);
					f=0;
				}
				if(f==2)
				{
				    BitBlt(hdc2, 0, 0,rect.right,rect.bottom,hCompatibleDC,0, 0, SRCCOPY);
					f=0;
				}
		    	break;
		    }
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_ERASEBKGND:
		GetClientRect(hWnd,&rect);
		FillRect(hdc,&rect,WHITE_BRUSH);
		break;
	case WM_DESTROY:
		  ReleaseDC(hWnd,hdc);
		  hEnhMtf=CloseEnhMetaFile(hdc1);
		  DeleteEnhMetaFile(hEnhMtf);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}