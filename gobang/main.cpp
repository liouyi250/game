
#include <Windows.h>
#include <gdiplus.h>
#include <vector>
#include <stdio.h>
#include "gobang.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HBITMAP hbitmap=NULL;
Gdiplus::Image *image=NULL;//棋子图片，png格式
BITMAP bmp; //全局变量，表示棋盘图片
CheckerBoard *board=NULL; //棋盘对象
Gobang *gobang=NULL;//棋子对象


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	static TCHAR AppName[] = TEXT("gobang");
	HWND   hwnd;
	MSG    msg;
	WNDCLASS wndclass;
	//加载位图
	hbitmap=(HBITMAP)LoadImage(NULL,"1.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
//填充位图
    GetObject(hbitmap,sizeof(BITMAP),&bmp);

//以下三行代码初始化gdiplus环境
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartupInput gdiplusInput;
    Gdiplus::GdiplusStartup(&gdiplusToken,&gdiplusInput,NULL);

    wndclass.style =CS_DBLCLKS;
    wndclass.lpfnWndProc=WndProc; //定义窗口处理函数
    wndclass.cbClsExtra=0; //窗口类无扩展
    wndclass.cbWndExtra=0; //窗口实例无扩展
    wndclass.hInstance=hInstance; //当前实例句柄
    wndclass.hIcon=LoadIcon(NULL,IDI_APPLICATION); //窗口的最小化图标为缺省图标
    wndclass.hCursor=LoadCursor(NULL,IDC_ARROW); //  窗口采用箭头光标
    wndclass.hbrBackground=(HBRUSH)(GetStockObject(BLACK_BRUSH)); //窗口背景为白色
    wndclass.lpszMenuName=NULL; //窗口无菜单
    wndclass.lpszClassName=AppName; //窗口类名为“窗口”

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"), AppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(AppName,TEXT("五子棋"),WS_OVERLAPPEDWINDOW  & ~WS_MAXIMIZEBOX  &~WS_THICKFRAME,358,93,650,582,NULL,NULL,hInstance,NULL);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{


		}
	}
	//删除位图句柄
	DeleteObject(hbitmap);
	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;
	static int picWidth,picHeight;//图片的宽度和高度
	static int m=0,s=0;//分和秒
	static UINT_PTR timer=0;//计时器

    static bool flag=false;//flag为真表示黑棋,flag为假表示白棋
    RECT chess={540,100,590,150}; //显示下一手黑白棋的位置

	switch (message)
	{
	case WM_CREATE:
		{
			picWidth=bmp.bmWidth;
			picHeight=bmp.bmHeight;
//新建棋盘和棋子对象，初始化显示计时器位置，加载棋子图片
			board=new CheckerBoard(14,14,448,448,picWidth,picHeight,20,88);
			gobang=new Gobang(board);
			image=Gdiplus::Image::FromFile(L"2.png");
			rect.left=500;
			rect.right=640;
			rect.top=500;
			rect.bottom=540;
			return 0;
		}


	case WM_PAINT:
		{ 
			hdc = BeginPaint(hwnd, &ps);
//将位图显示在窗口上
			HDC hDCMem=CreateCompatibleDC(hdc);
			HBITMAP hOldBmp=(HBITMAP)SelectObject(hDCMem,hbitmap);
			StretchBlt(hdc,0,0,bmp.bmWidth,bmp.bmHeight,hDCMem,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			SelectObject(hDCMem,hOldBmp);
			DeleteDC(hDCMem);
//将棋子显示在窗口上
			Gdiplus::Graphics g(hdc);

			for(auto it=gobang->getBlackChess()->begin();it!=gobang->getBlackChess()->end();it++)
			{
				g.DrawImage(image,(*it)->x,(*it)->y,0,0,16,16,Gdiplus::UnitPixel);
			}
			for(auto it=gobang->getWhiteChess()->begin();it!=gobang->getWhiteChess()->end();it++)
			{
				g.DrawImage(image,(*it)->x,(*it)->y,21,0,16,16,Gdiplus::UnitPixel);
			}
//显示下一手棋
			HBRUSH  hYellowBrush=(HBRUSH)CreateSolidBrush(RGB(255,255,0));
			HBRUSH hOldBrush=(HBRUSH)SelectObject(hdc,hYellowBrush);
			Rectangle(hdc,chess.left,chess.top,chess.right,chess.bottom);
			if(flag)
				g.DrawImage(image,Gdiplus::RectF(chess.left+9,chess.top+9,32,32),0,0,16,16,Gdiplus::UnitPixel);
			else
				g.DrawImage(image,Gdiplus::RectF(chess.left+9,chess.top+9,32,32),21,0,16,16,Gdiplus::UnitPixel);
			SelectObject(hdc,hOldBrush);
			DeleteObject(hYellowBrush);
			EndPaint(hwnd, &ps);
			return 0;
		} 

	case WM_LBUTTONUP:
		{
//新疆计时器
			if(timer==0)
				timer=SetTimer(hwnd,1,1000,NULL);
			int x=(short)LOWORD(lParam);
			int y=(short)HIWORD(lParam);
			if(x > picWidth) return 0;
//获取离鼠标最近的棋盘上的点
			int index=board->findNearestPoint(x,y);
           //创建棋子对象 
	    CHESSMAN *chessman=new CHESSMAN;
            chessman->x=board->getCenterPos()[index]->x -8;
            chessman->y=board->getCenterPos()[index]->y -8;
            flag ? chessman->color=BLACK : chessman->color=WHITE;
//如果该位置没有其他的棋子
            if(gobang->addChess(chessman))
            {
            	flag=!flag;
            	RECT paintRect;
            	paintRect.left=chessman->x;
            	paintRect.top=chessman->y;
            	paintRect.right=chessman->x+16;
            	paintRect.bottom=chessman->y+16;
//重绘棋子落下的位置
            	InvalidateRect(hwnd,&paintRect,true);
//重绘显示下一首棋的位置
            	InvalidateRect(hwnd,&chess,true);
//该手棋已经胜利
            	if(gobang->victory(chessman))
            	{
            		if(chessman->color == BLACK)
//重新开始
            			if(IDYES==MessageBox(hwnd,TEXT("是否重新开始"),TEXT("黑棋获胜"),MB_YESNO | MB_ICONQUESTION))
            			{
            				gobang->restart();
            				flag=false;
            				InvalidateRect(hwnd,NULL,TRUE);
            				m=0;s=0;
            				KillTimer(hwnd,timer);
            				timer=0;
            			}else
//退出程序
            				SendMessage(hwnd,WM_CLOSE,wParam,lParam);
            		else{
            			if(IDYES==MessageBox(hwnd,TEXT("是否重新开始"),TEXT("白棋获胜"),MB_YESNO | MB_ICONQUESTION)){
            			      gobang->restart();
            			      flag=false;
            			      InvalidateRect(hwnd,NULL,TRUE);
            			      m=0;s=0;
            			      KillTimer(hwnd,timer);
            			      timer=0;
            			}else
            			      SendMessage(hwnd,WM_CLOSE,wParam,lParam);
            			}
            	}
            }
//在该位置有其他棋子，删除刚才新建的对象
            else{
            	delete chessman;
            	chessman = nullptr;
            }
			return 0;
		}
	case WM_TIMER:
	{
		hdc=GetDC(hwnd);
		
		if(s<59)
			s++;
		else{
			if(m<60){
				m++;
				s=0;
			}
		}
		char sText[6];
		sprintf(sText,"%02d:%02d",m,s);
//设置文字颜色
		SetTextColor(hdc,RGB(255,0,0));
		DrawText(hdc,sText,-1,&rect,DT_CENTER);
		ReleaseDC(hwnd,hdc);
		return 0;
	}

	case WM_DESTROY:
//程序退出，删除全局对象
		delete image;
		DeleteObject(hbitmap);

		PostQuitMessage(0);
		return 0;

	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

