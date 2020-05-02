#ifndef _FIVE_CHESS_GAME
#define _FIVE_CHESS_GAME

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

const int N = 15;	//���̴�С

//����ṹ��
typedef struct Coordinate {
	int x;	
	int y;

}Coordinate;

//���ӽṹ��
typedef struct Chess {
	int type_value;	//���ӵ�ֵ
	char type;	//���ӵ�����
}Chess;

class  fiveChessGame
{
public:
	 fiveChessGame();
	 virtual ~ fiveChessGame();
	 void play();
public:
	static const Chess chessInitFlag;	//���̳�ʼֵ
	Chess ChessBoard[N+1][N+1];	//����
	static int chess_seq; 				//�������
	//Coordinate position;
    static void initChessBoard(fiveChessGame& object);		//��ʼ������
	static void playChess(Coordinate& pos, const int& userid,const Chess& flag, fiveChessGame& object);	//����ĺ���
	static bool judgePosition(const Coordinate& pos,const fiveChessGame& object);	//�ж�����λ���Ƿ�Ϸ�
	static int judgeResult(const Coordinate& pos,const fiveChessGame& object);	//�жϽ��
	static int getResult(const Coordinate& pos,const fiveChessGame& object);	//�õ����
	static int countResult(const int& posx, const int& posy, const int& model,const fiveChessGame& object);	//������
	static void printChessBoard(const fiveChessGame& object);	//��ӡ����
	static void testPlay(Coordinate& pos, const int& userid, const Chess& flag, fiveChessGame& object);
	int checkflag;
};
#endif // !fiveChessGame
