#ifndef GOBANG_ROBOT_H
#define GOBANG_ROBOT_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include "../robot_setting/robot_setting.h"
#include "../message_data/message_data.h"

class GobangRobot
{

//函数
public:
    GobangRobot()
    {

    }

    ~GobangRobot()
    {
        if(gobang_robot != NULL)
        {
            delete gobang_robot;
            gobang_robot = NULL;
        }
        
    }

    void createRobot(int level = 1, int seatno = 0, int off_seat = 0)
    {
        //选择一个级别
        switch (level)
        {
        case 1:
            gobang_robot = new GobangRobot();
            break;
        
        default:
            break;
        }

        //初始化棋子序号
        gobang_robot->chess_seq = 0;

        //为座位号赋值
        gobang_robot->seat_id = seatno;

        //记录先手座位号
        gobang_robot->offensive_seat = off_seat;

        gobang_robot->chess_type = (offensive_seat == seatno ? 1 : -1);

        //初始化棋盘
        gobang_robot->initChessBoard();

        //初始化分数盘
        gobang_robot->initPostionScore();

    }

    //接收信息的方法
    void receiveMessage(MessageData message_data);


    //在外调用的统一接口
    void choosePosition(ChessPostion &chess_position);

    //里面的接口
    void selectMaxPosition(ChessPostion &chess_position);

private:

    //初始化棋盘
    bool initChessBoard();

    //初始化分数盘
    bool initPostionScore();

private:

    void gameAction(const int& action_type, MessageData message);

    //计算位置的分数
    void countScore();

    //按方向算分
    void countByDirection(const int& x, const int& y, int& own_score, int& opp_score);

    //为该位置的这个方向打分
    int markScheme(const JudgePosition& judge_position, const direction& cnt_direction, const int& play_type);

    //获取每个位置的棋子类型
    int getChessValue(const direction& type, const int& step, int& pre_pos_first, int& pre_pos_second, int& chess_vaule);

    //按方向值来决定位置分数的函数
    int markScore(const JudgePosition& judge_position, const direction& cnt_direction, const int& play_type, const int& score);

        

//属性
protected:

    int chess_type;                                                         //本方的棋子类型

    int chess_seq;                                                          //棋子序号

    ChessValue chess_board[ChessBoardSize + 2][ChessBoardSize + 2];         //棋盘

    PositionScore position_score[ChessBoardSize + 1][ChessBoardSize + 1];   //每个位置的分数

    MaxScorePosition max_score_position;                                      //最大分数的位置

    int seat_id;                                                            //座位号

    int offensive_seat;                                                     //先手的座位号

private:

    GobangRobot *gobang_robot;                                              //一个下棋的机器人

};

typedef GobangRobot* GobangRobotPtr;


#endif