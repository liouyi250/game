#include "checkerboard.h"

//x,y代表棋盘左上角点的坐标
CheckerBoard::CheckerBoard(int rows,int cols,int height,int width,int picWidth,int picHeight,int x,int y)
{
    cellWidth=width/cols;
    cellHeight=height/rows;
    this->width=width;
    this->height=height;
    this->x=x;
    this->y=y;
    this->picWidth=picWidth;
    this->picHeight=picHeight;
//由于两个边界都算在棋盘内，所有要有等号
    for(int i=0;i<=rows;i++)
        for(int j=0;j<=cols;j++)
        {
            POINT *center=new POINT;
            center->x= x + width/cols * j;
            center->y= y + height/rows * i;
            centerPos.push_back(center);
        }
}
//获取棋盘中心位置
std::vector<POINT*> CheckerBoard::getCenterPos() const
{
    return centerPos;
}
//每一格的宽度
int CheckerBoard::getCellWidth() const
{
    return cellWidth;
}
//每一格的高度
int CheckerBoard::getCellHeight() const
{
    return cellHeight;
}
//棋盘宽度，最外围的两条边界线之间的距离
int CheckerBoard::getWidth() const
{
    return width;
}
//棋盘高度，最外围两条边界线之间的距离
int CheckerBoard::getHeight() const
{
    return height;
}
//寻找距离x,y最近的棋盘线交点中心
int CheckerBoard::findNearestPoint(int x,int y)
{
	if(x<this->x) x=this->x;
	if(x>this->x + width) x=this->x + this->width;
	if(y<this->y) y=this->y;
	if(y>this->y + height) y=this->y + height;
	int a,b;
	a=(int)((x-this->x)*1.0/32 + 0.5);
	b=(int)((y-this->y)*1.0/32 + 0.5);
	return b * (width/cellWidth+1) + a;
}
//清除vector
void CheckerBoard::clean()
{
    for(auto it=centerPos.begin();it!=centerPos.end();it++)
    {
        if(*it!=nullptr)
        {
            delete *it;
            *it=nullptr;
        }
    }
    centerPos.clear();
    std::vector<POINT*> tmp;
    tmp.swap(centerPos);
}
//析构
CheckerBoard::~CheckerBoard()
{
    clean();
}
