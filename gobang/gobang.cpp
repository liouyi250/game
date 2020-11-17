#include "gobang.h"

Gobang::Gobang(CheckerBoard *board)
{
    black=new std::vector<CHESSMAN*>;
    white=new std::vector<CHESSMAN*>;
    this->board=board;
}

bool Gobang::addChess(CHESSMAN* chessman)
{
//�Ǻ���ʱ
    if(chessman->color==BLACK){
    	if(!hasChess(black,chessman->x,chessman->y) && !hasChess(white,chessman->x,chessman->y)){
    		black->push_back(chessman);
    		return true;
    	}

    }
//����ʱ
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
//�жϸ�λ���Ƿ���ĳһ��������
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
//�ж�ĳһ���Ƿ�ʤ��
bool Gobang::isVictory(std::vector<CHESSMAN*> *vec,CHESSMAN* last)
{
    int x=last->x, y= last->y;
    int count=0;
    int tmp1,tmp2;
    int cellWidth=board->getCellWidth();
    int cellHeight=board->getCellHeight();
    //�������
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
    //��������
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
    //��������
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
    //��������
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
//�жϸ��������ڵ�һ���Ƿ�ʤ��
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
//���vector����ɾ�����������¿�ʼ
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
//���¿�ʼ��Ϸ
void Gobang::restart()
{
	cleanVector(black);
	cleanVector(white);
}
//ɾ��vector
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
//����
Gobang::~Gobang()
{
    clean();
}
