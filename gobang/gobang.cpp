#include "gobang.h"

Gobang::Gobang(CheckerBoard *board)
{
    black=new std::vector<CHESSMAN*>;
    white=new std::vector<CHESSMAN*>;
    this->board=board;
}

bool Gobang::addChess(CHESSMAN* chessman)
{
//是黑棋时
    if(chessman->color==BLACK){
    	if(!hasChess(black,chessman->x,chessman->y) && !hasChess(white,chessman->x,chessman->y)){
    		black->push_back(chessman);
    		return true;
    	}

    }
//白棋时
    else{
    	if(!hasChess(white,chessman->x,chessman->y) && !hasChess(black,chessman->x,chessman->y)){
    		white->push_back(chessman);
    		return true;
    	}
    }
    return false;
}


std::vector<CHESSMAN*>* Gobang::getBlackChess() const
{
	return black;
}
std::vector<CHESSMAN*>* Gobang::getWhiteChess() const
{
	return white;
}
//判断该位置是否有某一方的棋子
bool Gobang::hasChess(std::vector<CHESSMAN*> *vec,int x,int y)
{
    for(auto it=vec->begin();it!=vec->end();it++)
    {
        if((*it)->x== x && (*it)->y== y)
        {
            return true;
        }
    }
    return false;
}
//判断某一方是否胜利
bool Gobang::isVictory(std::vector<CHESSMAN*> *vec,CHESSMAN* last)
{
    int x=last->x, y= last->y;
    int count=0;
    int tmp1,tmp2;
    int cellWidth=board->getCellWidth();
    int cellHeight=board->getCellHeight();
    //计算横向
    tmp1=x;
    while(tmp1 >=0 && hasChess(vec,tmp1,y))
    {
        count++;
        tmp1 -= cellWidth;
    }
    tmp1 =x+cellWidth;
    while(tmp1 <= board->getWidth() &&  hasChess(vec,tmp1,y))
    {
        count++;
        tmp1 += cellWidth;;
    }
    if(count >=5)
        return true;
    //计算竖向
    count=0;
    tmp2=y;
    while(tmp2 >=0 && hasChess(vec,x,tmp2))
    {
        count++;
        tmp2 -= cellHeight;
    }
    tmp2 = y +cellHeight;
    while(tmp2 <= board->getHeight() &&  hasChess(vec,x,tmp2))
    {
        count++;
        tmp2 += cellHeight;
    }
    if(count >=5)
        return true;
    //计算左上
    count=0;
    tmp1=x;
    tmp2=y;
    while(tmp1 >=0  && tmp2>=0 && hasChess(vec,tmp1,tmp2))
    {
        count++;
        tmp2 -= cellHeight;
        tmp1 -= cellWidth;
    }
    tmp1 = x + cellWidth;
    tmp2 = y+ cellHeight;
    while(tmp2 <= board->getHeight() && tmp1 <=board->getWidth() &&  hasChess(vec,tmp1,tmp2))
    {
        count++;
        tmp2 += cellHeight;
        tmp1 += cellWidth;
    }
    if(count >=5)
        return true;
    //计算右上
    count=0;
    tmp1=x;
    tmp2=y;
    while(tmp1 <= board->getWidth()  && tmp2>=0 && hasChess(vec,tmp1,tmp2))
    {
        count++;
        tmp2 -= cellHeight;
        tmp1 += cellWidth;
    }
    tmp1 = x - cellWidth;
    tmp2 = y + cellHeight;
    while(tmp2 <= board->getHeight() && tmp1 >=0 &&  hasChess(vec,tmp1,tmp2))
    {
        count++;
        tmp2 += cellHeight;
        tmp1 -= cellWidth;
    }
    if(count>=5)
        return true;
    return false;
    
}
//判断该棋子所在的一方是否胜利
bool Gobang::victory(CHESSMAN* last)
{
    if(last->color==WHITE)
    {
        return isVictory(white,last);
    }
    else
    {
        return isVictory(black,last);
    }
}
//清空vector但不删除，方便重新开始
void Gobang::cleanVector(std::vector<CHESSMAN*> *vec)
{
    for(auto it=vec->begin();it!=vec->end();it++)
    {
        if(*it!=nullptr)
        {
            delete *it;
            *it=nullptr;
        }
    }
    vec->clear();

}
//重新开始游戏
void Gobang::restart()
{
	cleanVector(black);
	cleanVector(white);
}
//删除vector
void Gobang::clean()
{
	cleanVector(black);
	std::vector<CHESSMAN*> tmp1;
	tmp1.swap(*black);
	delete black;
	black=nullptr;

	cleanVector(white);
	std::vector<CHESSMAN*> tmp2;
	tmp1.swap(*white);
	delete white;
	white=nullptr;
}
//析构
Gobang::~Gobang()
{
    clean();
}
