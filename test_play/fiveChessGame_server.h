#ifndef _FIVE_CHESS_GAME
#define _FIVE_CHESS_GAME

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

const int N = 15;	//棋盘大小

//坐标结构体
typedef struct Coordinate {
	int x;	
	int y;

}Coordinate;

//棋子结构体
typedef struct Chess {
	int type_value;	//棋子的值
	char type;	//棋子的类型
}Chess;

class  fiveChessGame
{
public:
	 fiveChessGame();
	 virtual ~ fiveChessGame();
	 void play();
public:
	static const Chess chessInitFlag;	//棋盘初始值
	Chess ChessBoard[N+1][N+1];	//棋盘
	static int chess_seq; 				//棋子序号
	//Coordinate position;
    static void initChessBoard(fiveChessGame& object);		//初始化棋盘
	static void playChess(Coordinate& pos, const int& userid,const Chess& flag, fiveChessGame& object);	//下棋的函数
	static bool judgePosition(const Coordinate& pos,const fiveChessGame& object);	//判断下棋位置是否合法
	static int judgeResult(const Coordinate& pos,const fiveChessGame& object);	//判断结果
	static int getResult(const Coordinate& pos,const fiveChessGame& object);	//得到结果
	static int countResult(const int& posx, const int& posy, const int& model,const fiveChessGame& object);	//计算结果
	static void printChessBoard(const fiveChessGame& object);	//打印棋盘
	static void testPlay(Coordinate& pos, const int& userid, const Chess& flag, fiveChessGame& object);
	int checkflag;
};
#endif // !fiveChessGame
