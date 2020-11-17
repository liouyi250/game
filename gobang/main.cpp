
#include <Windows.h>
#include <gdiplus.h>
#include <vector>
#include <stdio.h>
#include "gobang.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HBITMAP hbitmap=NULL;
Gdiplus::Image *image=NULL;//����ͼƬ��png��ʽ
BITMAP bmp; //ȫ�ֱ�������ʾ����ͼƬ
CheckerBoard *board=NULL; //���̶���
Gobang *gobang=NULL;//���Ӷ���


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	static TCHAR AppName[] = TEXT("gobang");
	HWND   hwnd;
	MSG    msg;
	WNDCLASS wndclass;
	//����λͼ
	hbitmap=(HBITMAP)LoadImage(NULL,"1.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
//���λͼ
    GetObject(hbitmap,sizeof(BITMAP),&bmp);

//�������д����ʼ��gdiplus����
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartupInput gdiplusInput;
    Gdiplus::GdiplusStartup(&gdiplusToken,&gdiplusInput,NULL);

    wndclass.style =CS_DBLCLKS;
    wndclass.lpfnWndProc=WndProc; //���崰�ڴ�����
    wndclass.cbClsExtra=0; //����������չ
    wndclass.cbWndExtra=0; //����ʵ������չ
    wndclass.hInstance=hInstance; //��ǰʵ�����
    wndclass.hIcon=LoadIcon(NULL,IDI_APPLICATION); //���ڵ���С��ͼ��Ϊȱʡͼ��
    wndclass.hCursor=LoadCursor(NULL,IDC_ARROW); //  ���ڲ��ü�ͷ���
    wndclass.hbrBackground=(HBRUSH)(GetStockObject(BLACK_BRUSH)); //���ڱ���Ϊ��ɫ
    wndclass.lpszMenuName=NULL; //�����޲˵�
    wndclass.lpszClassName=AppName; //��������Ϊ�����ڡ�

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"), AppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(AppName,TEXT("������"),WS_OVERLAPPEDWINDOW  & ~WS_MAXIMIZEBOX  &~WS_THICKFRAME,358,93,650,582,NULL,NULL,hInstance,NULL);

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
	//ɾ��λͼ���
	DeleteObject(hbitmap);
	return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rect;
	static int picWidth,picHeight;//ͼƬ�Ŀ�Ⱥ͸߶�
	static int m=0,s=0;//�ֺ���
	static UINT_PTR timer=0;//��ʱ��

    static bool flag=false;//flagΪ���ʾ����,flagΪ�ٱ�ʾ����
    RECT chess={540,100,590,150}; //��ʾ��һ�ֺڰ����λ��

	switch (message)
	{
	case WM_CREATE:
		{
			picWidth=bmp.bmWidth;
			picHeight=bmp.bmHeight;
//�½����̺����Ӷ��󣬳�ʼ����ʾ��ʱ��λ�ã���������ͼƬ
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
//��λͼ��ʾ�ڴ�����
			HDC hDCMem=CreateCompatibleDC(hdc);
			HBITMAP hOldBmp=(HBITMAP)SelectObject(hDCMem,hbitmap);
			StretchBlt(hdc,0,0,bmp.bmWidth,bmp.bmHeight,hDCMem,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			SelectObject(hDCMem,hOldBmp);
			DeleteDC(hDCMem);
//��������ʾ�ڴ�����
			Gdiplus::Graphics g(hdc);

			for(auto it=gobang->getBlackChess()->begin();it!=gobang->getBlackChess()->end();it++)
			{
				g.DrawImage(image,(*it)->x,(*it)->y,0,0,16,16,Gdiplus::UnitPixel);
			}
			for(auto it=gobang->getWhiteChess()->begin();it!=gobang->getWhiteChess()->end();it++)
			{
				g.DrawImage(image,(*it)->x,(*it)->y,21,0,16,16,Gdiplus::UnitPixel);
			}
//��ʾ��һ����
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
//�½���ʱ��
			if(timer==0)
				timer=SetTimer(hwnd,1,1000,NULL);
			int x=(short)LOWORD(lParam);
			int y=(short)HIWORD(lParam);
			if(x > picWidth) return 0;
//��ȡ���������������ϵĵ�
			int index=board->findNearestPoint(x,y);
           //�������Ӷ��� 
	    CHESSMAN *chessman=new CHESSMAN;
            chessman->x=board->getCenterPos()[index]->x -8;
            chessman->y=board->getCenterPos()[index]->y -8;
            flag ? chessman->color=BLACK : chessman->color=WHITE;
//�����λ��û������������
            if(gobang->addChess(chessman))
            {
            	flag=!flag;
            	RECT paintRect;
            	paintRect.left=chessman->x;
            	paintRect.top=chessman->y;
            	paintRect.right=chessman->x+16;
            	paintRect.bottom=chessman->y+16;
//�ػ��������µ�λ��
            	InvalidateRect(hwnd,&paintRect,true);
//�ػ���ʾ��һ�����λ��
            	InvalidateRect(hwnd,&chess,true);
//�������Ѿ�ʤ��
            	if(gobang->victory(chessman))
            	{
            		if(chessman->color == BLACK)
//���¿�ʼ
            			if(IDYES==MessageBox(hwnd,TEXT("�Ƿ����¿�ʼ"),TEXT("�����ʤ"),MB_YESNO | MB_ICONQUESTION))
            			{
            				gobang->restart();
            				flag=false;
            				InvalidateRect(hwnd,NULL,TRUE);
            				m=0;s=0;
            				KillTimer(hwnd,timer);
            				timer=0;
            			}else
//�˳�����
            				SendMessage(hwnd,WM_CLOSE,wParam,lParam);
            		else{
            			if(IDYES==MessageBox(hwnd,TEXT("�Ƿ����¿�ʼ"),TEXT("�����ʤ"),MB_YESNO | MB_ICONQUESTION)){
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
//�ڸ�λ�����������ӣ�ɾ���ղ��½��Ķ���
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
//����������ɫ
		SetTextColor(hdc,RGB(255,0,0));
		DrawText(hdc,sText,-1,&rect,DT_CENTER);
		ReleaseDC(hwnd,hdc);
		return 0;
	}

	case WM_DESTROY:
//�����˳���ɾ��ȫ�ֶ���
		delete image;
		DeleteObject(hbitmap);

		PostQuitMessage(0);
		return 0;

	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

