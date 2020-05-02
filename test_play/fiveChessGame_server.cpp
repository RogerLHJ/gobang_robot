#include"fiveChessGame_server.h"
#include "../robot_base/gobang_robot.h"
#include "../message_data/message_data.h"

#include <unistd.h>

const Chess fiveChessGame::chessInitFlag = { 0,' '};
int fiveChessGame::chess_seq = 0;

//初始化
fiveChessGame::fiveChessGame() {
	initChessBoard(*this);
}

fiveChessGame::~fiveChessGame()
{

}
//下棋函数
void fiveChessGame::play()
{
	Coordinate pos;	//玩家的下棋点
	ChessPostion pos1;	//机器人下棋点
	const Chess whiteChess = {-1,'O'};
	const Chess blackChess = {1,'X'};
	GobangRobot *robot1 = new GobangRobot();
	robot1->createRobot(1,1,0);
	int current_seat = 0;
	while (true) 
	{
		if(current_seat == 0)
		{
			std::cout<<"-----You have tow choose, regret(1) or play chess(2)-----"<<std::endl;
			int choose_type = 2;
			std::cin>>choose_type;
			if(choose_type == 1 && chess_seq != 0)
			{
				std::cout<<"-----Please input 2 regret position-----"<<std::endl;
				int reg_pos[5];
				for(int i=0;i<4;i++)
				{
					std::cin>>reg_pos[i];
				}
				std::vector<MessageData* > mess_vec;
				for(int i=0;i<2;i++)
				{
					MessageData *reg_message = new MessageData();
					reg_message->putInt(Data_PositionX, reg_pos[2*i]);
					reg_message->putInt(Data_PositionY, reg_pos[2*i+1]);
					mess_vec.push_back(reg_message);
				}
				MessageData r_mess;
				r_mess.putMDataArray(Data_Array, mess_vec.data(), mess_vec.size());
				r_mess.putInt(Data_ActionType, TYPE_REGRET);
				robot1->receiveMessage(r_mess);
				std::cout<<"regret success! "<<std::endl;
				for(int i=0;i<2;i++)
				{
					this->ChessBoard[reg_pos[2*i]][reg_pos[2*i+1]] = chessInitFlag;
				}
				continue;
			}
			else
			{
				while(true)
				{
					std::cout<<"-----It is you turn, split x and y by space-----"<<std::endl;
					std::cin>>pos.x>>pos.y;
					if(pos.x > 15 || pos.x < 1 || pos.y > 15 || pos.y < 1)
					{
						std::cin.clear();
						while(std::cin.get() != '\n')continue;
						std::cout<<"-----Please input current position-----"<<std::endl;
						continue;
					}
					std::cout<<"-----Are you sure?(y/n)-----"<<std::endl;
					char sure = 'n';
					std::cin>>sure;
					if(sure == 'y')
					{
						if(judgePosition(pos, *this))break;
						else std::cout<<"-----Please input the current position-----"<<std::endl;
						
					}
					else if(sure == 'n')std::cout<<"-----Please input the new position-----"<<std::endl;
					else std::cout<<"-----please input y/n-----"<<std::endl;
				}
				this->ChessBoard[pos.x][pos.y] = blackChess;
			}
			
		}

		else
		{
			while (true)
			{
				robot1->choosePosition(pos1);
				std::cout<<"Robot position: "<<"("<<pos1.x<<","<<pos1.y<<")"<<std::endl;
				pos.x = pos1.x;
				pos.y = pos1.y;
				if(judgePosition(pos, *this))break;
			}
			this->ChessBoard[pos.x][pos.y] = whiteChess;
		}
	
		MessageData message;
		message.putInt(Data_PositionX, pos.x);
		message.putInt(Data_PositionY, pos.y);
		message.putInt(Data_Seatno, current_seat);
		message.putInt(Data_ActionType, TYPE_PLAY_CHESS);
		robot1->receiveMessage(message);
		std::cout<<"play success! "<<std::endl;
		if (getResult(pos, *this) != 0)break;
		chess_seq++;
		current_seat = current_seat^1;
		std::cout << pos.x << " " << pos.y << std::endl;
		printChessBoard(*this);
		sleep(1);
	}
	printf("game over\n");
}

//初始化棋盘
void fiveChessGame::initChessBoard(fiveChessGame& object)
{
	object.checkflag = 0;
	for (int i = 0; i < N+1; i++) {
		for (int j = 0; j < N+1; j++) {
			object.ChessBoard[i][j].type_value = chessInitFlag.type_value;	//棋子的值：-1为空，0为白棋，1为黑棋
			object.ChessBoard[i][j].type = chessInitFlag.type;
		}
	}
}

void fiveChessGame::printChessBoard(const fiveChessGame& object)	//打印棋盘
{
	system("cls");	//清空屏幕
	for (int i = 0; i < N + 1; i++) {
		for (int j = 0; j < N + 1; j++) {
			if (i == 0) {
				if (j != 0)
					printf(" %2d ", j);
				else
					printf(" ");
			}
			else if (j == 0)
				printf("%2d", i);
			else {
				if (i < N + 1) {
					printf(" %c |",object.ChessBoard[i][j].type);
				}
			}
		}
		printf("\n");
		printf("  ");
		for (int k = 0; k < N; k++) {
			printf("---|");
		}
		printf("\n");
	}
}

//判断下棋位置是否合法
bool fiveChessGame::judgePosition(const Coordinate& pos, const fiveChessGame& object)
{
	if (pos.x > 0 && pos.x <= N && pos.y > 0 && pos.y <= N) {
		std::cout<<"judge pos"<<pos.x<<" "<<pos.y<<std::endl;
		std::cout<<"judge pos"<<object.ChessBoard[pos.x][pos.y].type_value<<std::endl;
		if (object.ChessBoard[pos.x][pos.y].type_value == chessInitFlag.type_value
					&&object.ChessBoard[pos.x][pos.y].type == chessInitFlag.type)
			return true;
	}
	std::cout<<"fail!"<<std::endl;
	return false;
}

void fiveChessGame::playChess(Coordinate& pos, const int& useid, const Chess& flag, fiveChessGame& object)
{	
	//printChessBoard(object);	//打印棋盘
	while (true) {	
		printf("玩家%d下棋\n", useid);
		scanf("%d %d", &pos.x, &pos.y);
		if (judgePosition(pos, object)) break;	//判断下棋位置是否合法
		printf("输入坐标不合法\n");
	}
	object.ChessBoard[pos.x][pos.y] = flag;
}

int fiveChessGame::judgeResult(const Coordinate& pos, const fiveChessGame& object)
{
	int beginX = 0;
	int endX = 0;
	int beginY = 0;
	int endY = 0;
	//std::cout << pos.x << " " << pos.y << std::endl;
	//计算列条件
	beginX = (pos.x - 4) > 0 ? (pos.x - 4) : 1;
	endX = ((pos.x + 4) > N ? N : (pos.x + 4));
	//std::cout << beginX << " " << endX << std::endl;
	for (int j = pos.y, i = beginX; i + 4 <= endX; i++) {
		int result = countResult(i, j, 2, object);
		if (result == -5)return 1;	//返回1代表白棋赢
		else if (result == 5)return 2;	//黑棋赢
	}

	//计算行条件
	beginY = (pos.y - 4) > 0 ? (pos.y - 4) : 1;
	endY = (pos.y + 4) > N ? N :(pos.y + 4);
	for (int i = pos.x, j = beginY; j + 4 <= endY; j++) {
		int result = countResult(i, j, 1, object);
		if (result == -5)return 1;	
		else if (result == 5)return 2;
	}

	int len = 0;
	//计算主对角线
	len = pos.x > pos.y ? pos.y - 1 : pos.x - 1;	
	if (len > 4)len = 4;
	beginX = pos.x - len;
	beginY = pos.y - len;	//计算起点

	len = pos.x > pos.y ? (N - pos.x) : (N - pos.y);
	if (len > 4)len = 4;
	endX = pos.x + len;
	endY = pos.y + len;

	for (int i = beginX, j = beginY; (i + 4 <= endX) && (j + 4 <= endY); ++i,++j) {
		int result = countResult(i, j, 3, object);
		if (result == -5)return 1;
		else if (result == 5)return 2;
	}

	//计算副对角线
	len = (pos.x - 1) > (N - pos.y) ? (N - pos.y) : (pos.x - 1);
	if (len > 4)len = 4;
	beginX = pos.x - len;
	beginY = pos.y + len;

	len = (N - pos.x) > (pos.y - 1) ? (pos.y - 1) : (N - pos.x);
	if (len > 4)len = 4;
	endX = pos.x + len;
	endY = pos.y - len;

	for (int i = beginX, j = beginY; (i + 4 <= endX) && (j - 4 >= endY); ++i, --j) {
		int result = countResult(i, j, 4, object);
		if (result == -5)return 1;
		else if (result == 5)return 2;
	}


	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (object.ChessBoard[i][j].type_value == chessInitFlag.type_value)return 0;
		}
	}
	return -1;
}


int fiveChessGame::countResult(const int& posx, const int& posy, const int& model, const fiveChessGame& object)
{
	int result = 0;
	switch (model)//model = 1,2,3,4 分别表示行列与主副对角线的判断
	{
	case 1:
		result = 0;
		for (int i = 0; i < 5; i++) {
			result += object.ChessBoard[posx][posy + i].type_value;
		}
		break;
	case 2:
		result = 0;
		for (int i = 0; i < 5; i++)
			result += object.ChessBoard[posx + i][posy].type_value;
		break;
	case 3:
		result = 0;
		for (int i = 0; i < 5; i++)
			result += object.ChessBoard[posx + i][posy + i].type_value;
		break;
	case 4:
		result = 0;
		for (int i = 0; i < 5; i++)
			result += object.ChessBoard[posx + i][posy - i].type_value;
		break;
	default:
		break;
	}
	return result;
}

int fiveChessGame::getResult(const Coordinate& pos, const fiveChessGame& object)
{
	std::cout << pos.x << " " << pos.y << std::endl;
	int result = judgeResult(pos, object);
	if (result != 0) {
		printChessBoard(object);
		if (result == 1)printf("white win\n");
		else if (result == 2)printf("black win\n");
		else if (result == -1)printf("planishing\n");
		return 1;
	}
	else
		return 0;
}
//int fiveChessGame::checkflag = 0;

void fiveChessGame::testPlay(Coordinate& pos, const int& userid, const Chess& flag, fiveChessGame& object)
{
	sleep(1);
	printChessBoard(object);

	int x = 0;
	int y = 0;
	int index = 0;
	int diretion[4] = {0,1,-1,0};
	if(object.checkflag < 2){
		/*x = rand()%7+5;
		srand((unsigned int)time(NULL));
		y = rand()%7+5;
		srand((unsigned int)time(NULL));
*/
		pos.x = 8+object.checkflag;
		pos.y = 8+object.checkflag;
		object.checkflag++;
	}


	else{
		while (true) {
			index = rand()%4;
			srand((unsigned int)time(NULL));
			x = pos.x+diretion[index]-1;
			index = rand()%4;
			srand((unsigned int)time(NULL));
			y = pos.y+diretion[index]-1;     

			Coordinate checkpos;
			checkpos.x = x+1;
			checkpos.y = y+1;
			if (judgePosition(checkpos,object))break;
			//printf("ERROR!\n");
		}
		pos.x = x+1;
		pos.y = y+1;
	}
	
	object.ChessBoard[pos.x][pos.y] = flag;
}
