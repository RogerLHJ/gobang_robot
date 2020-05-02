#include "gobang_robot.h"
#include <iostream>
#include <cstdio>



//测试所使用的消息接口
void GobangRobot::receiveMessage(MessageData message_data)
{
    int action_type = 0;
    message_data.getInt(Data_ActionType, action_type);
    std::cout<<"action_type: "<<action_type<<std::endl;
    //判断动作类型是否有效
    if(action_type != 0)
    {
        gobang_robot->gameAction(action_type, message_data);

    }
}

void GobangRobot::gameAction(const int& action_type, MessageData message)
{
    switch (action_type)
    {
    case TYPE_PLAY_CHESS:
    {
        int pos_x = 0;
        int pos_y = 0;

        int current_seat = -1;

        message.getInt(Data_PositionX, pos_x);
        message.getInt(Data_PositionY, pos_y);
        message.getInt(Data_Seatno, current_seat);
        //对方的棋子
        if(current_seat != seat_id)
        {
            chess_board[pos_x][pos_y].type_value = -chess_type ;
        }
        else
        {
            chess_board[pos_x][pos_y].type_value = chess_type;
        }
        chess_seq++;
        break;
    }

    case TYPE_REGRET:
    {
        int pos_x = 0;
        int pos_y = 0;
        int pos_size = 0;
        //获取数组的大小
        message.getMDataArraySize(Data_Array, pos_size);
        //循环取出数组中的元素
        for(int i = 0; i < pos_size; i++)
        {
            MessageData* cp_array_data = new MessageData();
            MessageData** array_data = &cp_array_data;
            message.getMDataArray(Data_Array, array_data, i);
            (*array_data)->getInt(Data_PositionX, pos_x);
            (*array_data)->getInt(Data_PositionY, pos_y);
            chess_board[pos_x][pos_y].type_value = 0;
            chess_seq--;
            if(cp_array_data != NULL)
            {
                delete cp_array_data;
                cp_array_data = NULL;
                array_data = NULL;
            }
        }
        break;
    }
    
    default:
        break;
    }

}

//选棋的接口
void GobangRobot::choosePosition(ChessPostion &chess_position)
{
    if(gobang_robot->chess_seq == 0)
    {
        chess_position.x = 8;
        chess_position.y = 8;
    }
    else
    {
        gobang_robot->selectMaxPosition(chess_position);
    }
}

//选择棋子的函数
void GobangRobot::selectMaxPosition(ChessPostion &chess_position)
{
    countScore();
    max_score_position.max_score = 0;
    max_score_position.second_score = 0;
    max_score_position.x = 0;
    max_score_position.y = 0;

    //记录一个位置的最大分值
    int pos_score[2];

    //打印观察分数
    for(int x = 1; x <= ChessBoardSize; x++)
    {
        for(int y = 1; y <= ChessBoardSize; y++)
        {
            std::cout<<" ("<<position_score[x][y].own_side_socre <<","
            << position_score[x][y].opponent_socre<<") " ;
        }
        std::cout<<std::endl;
    }

    //遍历棋盘进行选择
    for(int x = 1; x <=  ChessBoardSize; x++)
    {
        for(int y = 1; y <= ChessBoardSize; y++)
        {   
            if(position_score[x][y].own_side_socre == 0) continue;

            //防守大于进攻的情况
            if(position_score[x][y].opponent_socre > position_score[x][y].own_side_socre
            && position_score[x][y].own_side_socre < 1000)
            {
                pos_score[0] = position_score[x][y].opponent_socre;
                pos_score[1] = position_score[x][y].own_side_socre;
            }
            
            //选择进攻
            else
            {
                //形成连5
                if(position_score[x][y].own_side_socre >= 1000)
                {
                    pos_score[0] = 10000;
                    pos_score[1] = position_score[x][y].opponent_socre;
                }
                else
                {
                    pos_score[0] = position_score[x][y].own_side_socre;
                    pos_score[1] = position_score[x][y].opponent_socre;
                }
            }

            //与原位置的分值进行判断
            if(pos_score[0] > max_score_position.max_score)
            {
                max_score_position.x = x;
                max_score_position.y = y;
                max_score_position.max_score = pos_score[0];
                max_score_position.second_score = pos_score[1];
            }

            //最高分相等，比较第二高分
            else if(pos_score[0] == max_score_position.max_score) 
            {
                if(pos_score[1] > max_score_position.second_score)
                {
                    max_score_position.x = x;
                    max_score_position.y = y;
                    max_score_position.max_score = pos_score[0];
                    max_score_position.second_score = pos_score[1];
                }
            }

            //其他情况不需要判断
            //else
            
        }
    }

    if(max_score_position.max_score > 0)
    {
        chess_position.x = max_score_position.x;
        chess_position.y = max_score_position.y;
    }
}

//初始化棋盘
bool GobangRobot::initChessBoard()
{
    int board_size =  ChessBoardSize + 2;
    for(int x = 0; x < board_size; x++)
    {
        for(int y = 0; y < board_size; y++)
        {
            chess_board[x][y].type_value = 0;
        }
    }
    for(int x = 0; x < board_size; x++)
    {
        chess_board[x][0].type_value = -2;
        chess_board[x][ChessBoardSize + 1].type_value = -2;
    }

    for(int y = 0; y < board_size; y++)
    {
        chess_board[0][y].type_value = -2;
        chess_board[ChessBoardSize + 1][y].type_value = -2;
    }

    for(int x = 0; x < 17; x++)
    {
        for(int y = 0; y < 17; y++)
        {
            std::cout<<chess_board[x][y].type_value <<" ";
        }
        std::cout<<std::endl;
    }
    return true;
}

//初始化分数盘
bool GobangRobot::initPostionScore()
{
    int board_size =  ChessBoardSize + 1;
    for(int x = 0; x < board_size; x++)
    {
        for(int y = 0; y < board_size; y++)
        {
            position_score[x][y].own_side_socre = 0;
            position_score[x][y].opponent_socre = 0;
        }

    }
    return true;
}


//具体的打分细节
void GobangRobot::countScore()
{
    //初始化算分的棋盘
    initPostionScore();
    //遍历整个棋盘开始算分
    for(int x = 1; x <= ChessBoardSize; x++)
    {
        for(int y = 1; y <= ChessBoardSize; y++)
        {
            //跳过非空的位置
            if(chess_board[x][y].type_value != 0)continue;
            //调用方向函数进行打分
            countByDirection(x, y, position_score[x][y].own_side_socre, position_score[x][y].opponent_socre);
        }
    }
}

//按方向打分
void GobangRobot::countByDirection(const int& x, const int& y, int& own_score, int& opp_score)
{
    JudgePosition judge_position;
    judge_position.x = x;
    judge_position.y = y;
    own_score = 0;
    opp_score = 0;
    int opp_chess_type = 0 - chess_type;

    int N = ChessBoardSize;

    //判断横方向
    judge_position.beginx = (x - 4) > 0 ? (x - 4) : 1;
	judge_position.endx = (x + 4) > N ? N : (x + 4);
    judge_position.beginy = y;
    judge_position.endy = y;
    own_score += markScheme(judge_position,dir_horizontal,chess_type);
    opp_score += markScheme(judge_position,dir_horizontal,opp_chess_type); 

    //判断竖方向
    judge_position.beginx = x;
    judge_position.endx = x;
    judge_position.beginy = (y - 4) > 0 ? (y - 4) : 1;
    judge_position.endy = (y + 4) > N ? N : (y + 4);
    own_score += markScheme(judge_position,dir_vertical,chess_type);
    opp_score += markScheme(judge_position,dir_vertical,opp_chess_type); 

    int len = 0;
    //判断主对角线方向
    len = x > y ? (y - 1) : (x - 1);
    if(len > 4)len = 4;
    judge_position.beginx = x - len;
    judge_position.beginy = y - len;

    len = x > y ? (N - x) : (N - y); 
    if(len > 4)len = 4;
    judge_position.endx = x + len;
    judge_position.endy = y + len;
    
    if(judge_position.endx == judge_position.beginx)
    {
        own_score += 1;
        opp_score += 1;
    }
    else
    {
        own_score += markScheme(judge_position, dir_main_diagonal,chess_type);
        opp_score += markScheme(judge_position, dir_main_diagonal,opp_chess_type); 
    }

    //判断副对角线
    len = (x - 1) > (N - y) ? (N - y) : (x - 1);
    if(len > 4)len = 4;
    judge_position.beginx = x - len;
    judge_position.beginy = y + len;

    len = (N - x) > (y - 1) ? (y - 1) : (N - x);
	if (len > 4)len = 4;
	judge_position.endx = x + len;
	judge_position.endy = y - len;

    if(judge_position.endx == judge_position.beginx)
    {
        own_score += 1;
        opp_score += 1;
    }
    else
    {
        own_score += markScheme(judge_position, dir_anti_diagonal,chess_type);
        opp_score += markScheme(judge_position, dir_anti_diagonal,opp_chess_type); 
    }  

}

//打分系统
int GobangRobot::markScheme(const JudgePosition& judge_position, 
const direction& cnt_direction, const int& play_type)
{
    int score_cnt = 1;          //分数总和，初始化为1
    int max_score = 0;          //记录该方向的最高分
    int x = judge_position.x;   //需要判断的位置
    int y = judge_position.y;  
    int chess_value = 0;        //位置的分数

    //计分的始止端
    int cnt_begin = 0;      //统计分数的开始位置
    int cnt_end = 0;        //统计分数的结束位置
    int front_step = -1;     //向前一步
    int back_step = 1;      //向后一步
    int limit_front = 0;    //位置的起止
    int limit_back = 0;
    int limit_begin = 0;
    int limit_end = 0;

    //记录需要传的信息
    JudgePosition jd_position;
    jd_position.x = judge_position.x;
    jd_position.y = judge_position.y;
    jd_position.beginx = 0;
    jd_position.endx = 0;

    //记录当前位置的变量
    int present_pos_f = x;
    int present_pos_s = y;

    cnt_begin = judge_position.x;
    cnt_end = judge_position.x;
    //判断应该是以5个位置为单位的窗口滑动
    limit_begin = judge_position.beginx;
    limit_end = judge_position.endx;
    if(limit_end - limit_begin > 4)
    {
        limit_front = judge_position.x - judge_position.beginx;
        limit_back = 4 - limit_front;
    }
    else 
    {
        limit_front = judge_position.x - judge_position.beginx;
        limit_back =  judge_position.endx - judge_position.x;;
    }
    //竖向是特例
    if(cnt_direction ==  dir_vertical)
    {
        present_pos_f = y;
        present_pos_s = x;
        limit_begin = judge_position.beginy;
        limit_end = judge_position.endy;
        if(limit_end - limit_begin > 4)
        {
            limit_front = judge_position.y - judge_position.beginy;
            limit_back = 4 - limit_front;
        }
        else 
        {
            limit_front = judge_position.y - judge_position.beginy;
            limit_back =  judge_position.endy - judge_position.y;;
        }
        cnt_begin = judge_position.y;
        cnt_end = judge_position.y;
    }

    while(present_pos_f + limit_back <= limit_end)
    {
        int judge_limit = present_pos_f - limit_front;
        while(present_pos_f > judge_limit)
        {
            getChessValue(cnt_direction, front_step, present_pos_f, present_pos_s, chess_value);
            if(chess_value == -play_type)break;
            if(chess_value == play_type)
            {
                score_cnt += (chess_value*play_type);
                cnt_begin = present_pos_f;
            }
        }
        present_pos_f = (cnt_direction ==  dir_vertical ? y : x);
        present_pos_s = (cnt_direction ==  dir_vertical ? x : y);
        judge_limit = present_pos_f + limit_back;
        while(present_pos_f < judge_limit)
        {
            getChessValue(cnt_direction, back_step, present_pos_f, present_pos_s, chess_value);
            if(chess_value == -play_type)break;
            if(chess_value == play_type)
            {
                score_cnt += (chess_value*play_type);
                cnt_end = present_pos_f;
            }
        }
        if(max_score <= score_cnt)
        {
            max_score = score_cnt;
            jd_position.beginx = cnt_begin;
            jd_position.endx = cnt_end;
        }
        else if(max_score == score_cnt)
        {
            int old_len = jd_position.endx - jd_position.beginx;
            int new_len = cnt_end - cnt_begin;
            if(old_len > new_len)
            {
                jd_position.beginx = cnt_begin;
                jd_position.endx = cnt_end;
            }
        }

        present_pos_f = (cnt_direction ==  dir_vertical ? y : x);
        present_pos_s = (cnt_direction ==  dir_vertical ? x : y);
        limit_back += 1;
        limit_front = 4 - limit_back; 
        score_cnt = 1;
        cnt_begin = cnt_end = present_pos_f;
    }

    //传入分数判断空位类型
    return markScore(jd_position,cnt_direction,play_type,max_score);
}

//计算每个方向的分数
int GobangRobot::getChessValue(const direction& type, const int& step, 
int& pre_pos_first, int& pre_pos_second, int& chess_vaule)
{
    switch (type)
    {
    case  dir_horizontal:
        pre_pos_first += step;
        chess_vaule = chess_board[pre_pos_first][pre_pos_second].type_value;
        break;
    case  dir_vertical:
        pre_pos_first += step;
        chess_vaule = chess_board[pre_pos_second][pre_pos_first].type_value;
        break;
    case  dir_main_diagonal:
        pre_pos_first += step;
        pre_pos_second += step;
        chess_vaule = chess_board[pre_pos_first][pre_pos_second].type_value;
        break;
    case  dir_anti_diagonal:
        pre_pos_first += step;
        pre_pos_second -= step;
        chess_vaule = chess_board[pre_pos_first][pre_pos_second].type_value;
        break;
    
    default:
        break;
    }
    return 1;
}


int GobangRobot::markScore(const JudgePosition& judge_position, const direction& cnt_direction, 
const int& play_type, const int& score)
{
    //记录眠或活的分数
    int la_score = 0;   //活连
    int ja_score = 0;   //跳连
    int sleep_score = 0;    //眠的分数
    int ret_score = 0;

    //定义始止的棋子类型
    int begin_type = 0;
    int end_type = 0;

    //起止位置长度
    int to_begin_len = 0;
    int to_end_len = 0;
    int btoe_len = 0;
    //计算起止位置的长度
    to_begin_len = judge_position.x - judge_position.beginx;
    to_end_len = judge_position.endx - judge_position.x;
    btoe_len = judge_position.endx - judge_position.beginx;

    //根据分数，定义返回的分值
    switch (score)
    {
    //连5直接返回
    case 5:
        return LINK5;
        break;
    //1分放回其他
    case 1:
        return OTHER;
        break;
    case 4:
        la_score = LINK_ALIVE4;
        ja_score = JUMP_ALIVE4;
        sleep_score = SLEEP4;
        break;
    case 3:
        la_score = LINK_ALIVE3;
        ja_score = JUMP_ALIVE3;
        sleep_score = SLEEP3;
        break;
    case 2:
        la_score = LINK_ALIVE2;
        ja_score = JUMP_ALIVE2;
        sleep_score = SLEEP2;
        break;
    default:
        return NOT_EMPTY;
        break;
    }

    //获取不同方向的起止的类型值
    switch (cnt_direction)
    {
    case  dir_horizontal:
    {
        begin_type = chess_board[judge_position.beginx - 1][judge_position.y].type_value;
        end_type = chess_board[judge_position.endx + 1][judge_position.y].type_value;
        break;
    }

    case  dir_vertical:
    {
        begin_type = chess_board[judge_position.x][judge_position.beginx - 1].type_value;
        end_type = chess_board[judge_position.x][judge_position.endx + 1].type_value;
        break;
    }

    case  dir_main_diagonal:
    {
        begin_type = chess_board[judge_position.beginx - 1][judge_position.y - to_begin_len - 1].type_value;
        end_type = chess_board[judge_position.endx + 1][judge_position.y + to_end_len + 1].type_value;
        break;
    }

    case  dir_anti_diagonal:
    {
        begin_type = chess_board[judge_position.beginx - 1][judge_position.y + to_begin_len + 1].type_value;
        end_type = chess_board[judge_position.endx + 1][judge_position.y - to_end_len - 1].type_value;
        break;
    }  
    
    default:
        break;
    }

    //判断该空位两端是否都是对方棋子
    if((begin_type == -play_type || begin_type == -2) 
    && (end_type == -play_type || end_type == -2))ret_score = OTHER;

    //判断两端是否都为空
    else if(begin_type == 0 && end_type == 0)
    {
        ret_score = (btoe_len + 1 > score ? ja_score : la_score);
    }

    else
    {
        ret_score = sleep_score;
    }

    return ret_score;
    
}


