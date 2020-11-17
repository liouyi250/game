#ifndef CHECKERBOARD_H
#define CHECKERBOARD_H
#include <vector>
#include <windows.h>

enum COLOR
{
    BLACK=0,
    WHITE=1
};

typedef struct _CHESSMAN : tagPOINT
{
    enum COLOR color;
}CHESSMAN;


class CheckerBoard
{
    private:
        int cellWidth;//每格宽度
        int cellHeight;//每格高度
	int width;//棋盘宽度，不包括外框
	int height;//棋盘高度，不包括外框
	int x;//棋格第一个点x坐标
	int y;//棋格第一个点y坐标
	int picWidth;//整张棋盘图片宽度
	int picHeight;//整张棋盘图片高度
        std::vector<POINT*> centerPos;
    private:
    public:
        CheckerBoard(int rows,int cols,int height,int width,int picWidth,int picHeight,int x,int y)
;
        std::vector<POINT*> getCenterPos() const;
        int getCellWidth() const;
        int getCellHeight() const;
        int getWidth() const;
        int getHeight() const;
        int findNearestPoint(int x,int y);
        void clean();
        ~CheckerBoard();

};
#endif
