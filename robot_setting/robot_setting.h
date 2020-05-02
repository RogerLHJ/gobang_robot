/*
    name:           五子棋AI
    author：        Roger
    create date:    2020/03/23
    version:        2.0
*/

#ifndef ROBOT_SETTING_H
#define ROBOT_SETTING_H

#include <iostream>
#include <vector>
#include <cstdio>

//棋盘的大小
const int ChessBoardSize = 15;

//棋子的值
typedef struct ChessValue
{
    int type_value;

    ChessValue()
    {
        type_value = 0;
    }

}* ChessValuePtr;

//棋子
typedef struct  ChessPostion
{
    int x;
    int y;
}* ChessPostionPtr;


//每个位置的分数
typedef struct  PositionScore
{
    int own_side_socre;
    int opponent_socre;
    
    PositionScore(int score_a = 0,int score_b = 0)
    {
        own_side_socre = score_a;
        opponent_socre = score_b;
    }

}* PositionScorePtr;

//需要判断的位置
typedef struct  JudgePosition
{
    int x;
    int y;
    int beginx;
    int beginy;
    int endx;
    int endy;
}* JudgePositionptr;

//最大分数位置
typedef struct  MaxScorePosition
{
    int x;
    int y;
    int max_score;
    int second_score;

    MaxScorePosition()
    {
        x = 0;
        y = 0;
        max_score = 0;
        second_score = 0; 
    }

}* MaxScorePositionPtr;

//定义棋子的方向类型
enum direction{dir_horizontal, dir_vertical, dir_main_diagonal, dir_anti_diagonal};

//定义棋子的分数
const int LINK5         =       1000; 
const int LINK_ALIVE4   =       100;
const int LINK_ALIVE3   =       50;
const int LINK_ALIVE2   =       25;
const int JUMP_ALIVE4   =       50;
const int JUMP_ALIVE3   =       25;
const int JUMP_ALIVE2   =       10;
const int SLEEP4        =       50;
const int SLEEP3        =       25;
const int SLEEP2        =       10;
const int OTHER         =       1;
const int NOT_EMPTY     =       0;

//定义游戏的类型
const int TYPE_PLAY_CHESS = 1;
const int TYPE_REGRET = 2;

#endif
