#ifndef GOBANG_H
#define GOBANG_H

#include <vector>
#include "checkerboard.h"
class Gobang
{
    private:
        std::vector<CHESSMAN*> *black;
        std::vector<CHESSMAN*> *white;
        CheckerBoard *board;

    private:
        bool isVictory(std::vector<CHESSMAN*> *vec,CHESSMAN* last);
        bool hasChess(std::vector<CHESSMAN*> *vec,int x,int y);

    
    public:
        Gobang(CheckerBoard *board);
        bool addChess(CHESSMAN* chessman);
        std::vector<CHESSMAN*>* getBlackChess() const;
        std::vector<CHESSMAN*>* getWhiteChess() const;
        bool victory(CHESSMAN *last);
        void cleanVector(std::vector<CHESSMAN*> *vec);
        void restart();
        void clean();
        ~Gobang();
};



#endif
