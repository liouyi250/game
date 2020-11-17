#include "checkerboard.h"

//x,y�����������Ͻǵ������
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
//���������߽綼���������ڣ�����Ҫ�еȺ�
    for(int i=0;i<=rows;i++)
        for(int j=0;j<=cols;j++)
        {
            POINT *center=new POINT;
            center->x= x + width/cols * j;
            center->y= y + height/rows * i;
            centerPos.push_back(center);
        }
}
//��ȡ��������λ��
std::vector<POINT*> CheckerBoard::getCenterPos() const
{
    return centerPos;
}
//ÿһ��Ŀ��
int CheckerBoard::getCellWidth() const
{
    return cellWidth;
}
//ÿһ��ĸ߶�
int CheckerBoard::getCellHeight() const
{
    return cellHeight;
}
//���̿�ȣ�����Χ�������߽���֮��ľ���
int CheckerBoard::getWidth() const
{
    return width;
}
//���̸߶ȣ�����Χ�����߽���֮��ľ���
int CheckerBoard::getHeight() const
{
    return height;
}
//Ѱ�Ҿ���x,y����������߽�������
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
//���vector
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
//����
CheckerBoard::~CheckerBoard()
{
    clean();
}
