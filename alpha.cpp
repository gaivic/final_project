#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <string>
#include <sstream>
#include <vector>
// value for each case

#define WIN 1 //4000
#define LOSE 2 //-4000
#define FLEX4 3 //2000
#define flex4 4 //-2000
#define BLOCK4 5 //1000
#define block4 6 //-1000
#define FLEX3 7 //1000
#define flex3 8 //-1000
#define BLOCK3 9 //400
#define block3 10 //-600
#define FLEX2 11 //400
#define flex2 12 //-600
#define BLOCK2 13 //100
#define block2 14 //-150
#define FLEX1 15 //100
#define flex1 16 //-150
#define INF 0x7FFFFFFF
void set_score();
class Board {
public:
    static const int SIZE = 15;
    std::array<std::array<int, SIZE>, SIZE> board;
    int remain;
    
public:
    Board(std::array<std::array<int, SIZE>, SIZE> cur_board, int player) {
		remain = 0;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = cur_board[i][j];
				if(!board[i][j])remain++;
        }
    }
	}
    Board(Board const& cur_board){
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = cur_board.board[i][j];
            }
        }
        remain = cur_board.remain;
    }
	
};

enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

int player;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> root_board;
int get_state_value(Board b );

void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> root_board[i][j];
        }
    }
}

int AlphaBeta(Board cur, int depth, int alpha , int beta, int next_player){
    if(depth == 0 || cur.remain == 0) return get_state_value(cur);
	Board next(cur);
    // if maximizing player
    if(next_player == player){
        int value = -INF;
		for(int i = 0;i < SIZE;i++)for(int j = 0;j < SIZE ; j++){
            if(next.board[i][j] == 0){
				next.board[i][j] = next_player;
				next.remain--;
            	value = std::max ( value , AlphaBeta(next , depth-1, alpha ,beta , 3-next_player));
            	alpha = std::max(value , alpha);
            	if(alpha >= beta) break;
				next.board[i][j] = 0;
				next.remain++;
			}
        }
        return value;
    }
    else if(next_player == 3- player){
        int value = INF;
		for(int i = 0;i < SIZE;i++)for(int j = 0;j < SIZE ; j++){
			if(next.board[i][j] == 0){
            	next.board[i][j] = next_player;
				next.remain--;
            	value = std::min(value , AlphaBeta(next , depth-1, alpha ,beta ,3-next_player));
            	beta = std::min(value,beta);
            	if(alpha >= beta) break;
				next.board[i][j] = 0;
				next.remain++;
			}
        }
        return value;
    }
    
}

// root board 
void write_valid_spot(std::ofstream& fout) {

    srand(time(NULL));
	set_score();
	
    int x = -1 , y = -1;
    int alpha = -INF;
    int beta = INF;
	Board next(root_board , player);
	//std::cout << "fuck";
	for(int i = 0;i < SIZE;i++)for(int j = 0;j < SIZE ; j++){
		if(next.board[i][j] == 0){
			next.board[i][j] = player;
			next.remain--;
			int value = AlphaBeta(next , 1 ,alpha , beta , 3-player);
        	if(value > alpha){
            	alpha = value , x = i , y = j;
				//std::cout << x << y << "\n"; 
        	}
			next.board[i][j] = 0;
			next.remain++;
		}
	}
	std::cout << x << y << "\n"; 
    fout << x << " " << y << std::endl;
    fout.flush(); 
}


int main(int, char** argv) {
	//std::cout << "fuck";
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    write_valid_spot(fout);

    fin.close();
    fout.close();
    return 0;
}

// state value and score rule

int score[3][3][3][3][3][3];
//int weight[17]={0,5000,-8000,3000,-5000,1000,-2000,1000,-1000,400,-600,400,-600,100,-150,100,-150};
int weight[17]={0,4000,-8000,2500,-5000,1000,-2000,1000,-2000,400,-600,400,-600,100,-150,100,-150};

void set_score(){
    //黑五連  （ 1 //4000）
	score[1][1][1][1][1][1]=WIN;
	score[1][1][1][1][1][0]=WIN;
	score[0][1][1][1][1][1]=WIN;
	score[1][1][1][1][1][2]=WIN;
	score[2][1][1][1][1][1]=WIN;

	//白五連  （ 2 // -4000）
	score[2][2][2][2][2][2]=LOSE;
	score[2][2][2][2][2][0]=LOSE;
	score[0][2][2][2][2][2]=LOSE;
	score[2][2][2][2][2][1]=LOSE;
	score[1][2][2][2][2][2]=LOSE;


	//黑活四   ( 3 // 2000）
	score[0][1][1][1][1][0]=FLEX4;

	//白活四   （ 4 // -2000）
	score[0][2][2][2][2][0]=flex4;


	//黑活三	（ 7 // 1000）
	score[0][1][1][1][0][0]=FLEX3;
	score[0][1][1][0][1][0]=FLEX3;
	score[0][1][0][1][1][0]=FLEX3;
	score[0][0][1][1][1][0]=FLEX3;

	//白活三	（ 8 // -1000）
	score[0][2][2][2][0][0]=flex3;
	score[0][2][2][0][2][0]=flex3;
	score[0][2][0][2][2][0]=flex3;
	score[0][0][2][2][2][0]=flex3;


	//黑活二	（ 11 // 400）
	score[0][1][1][0][0][0]=FLEX2;
	score[0][1][0][1][0][0]=FLEX2;
	score[0][1][0][0][1][0]=FLEX2;
	score[0][0][1][1][0][0]=FLEX2;
	score[0][0][1][0][1][0]=FLEX2;
	score[0][0][0][1][1][0]=FLEX2;

	//白活二	（ 12 // -600）
	score[0][2][2][0][0][0]=flex2;
	score[0][2][0][2][0][0]=flex2;
	score[0][2][0][0][2][0]=flex2;
	score[0][0][2][2][0][0]=flex2;
	score[0][0][2][0][2][0]=flex2;
	score[0][0][0][2][2][0]=flex2;


	//黑活一	（ 15 // 100）
	score[0][1][0][0][0][0]=FLEX1;
	score[0][0][1][0][0][0]=FLEX1;
	score[0][0][0][1][0][0]=FLEX1;
	score[0][0][0][0][1][0]=FLEX1;

	//白活一	（ 16 // -150）
	score[0][2][0][0][0][0]=flex1;
	score[0][0][2][0][0][0]=flex1;
	score[0][0][0][2][0][0]=flex1;
	score[0][0][0][0][2][0]=flex1;

	
	int p1,p2,p3,p4,p5,p6,x,y,ix,iy;

	for(p1=0;p1<3;p1++)
	{
		for(p2=0;p2<3;p2++)
		{
			for(p3=0;p3<3;p3++)
			{
				for(p4=0;p4<3;p4++)
				{
					for(p5=0;p5<3;p5++)
					{
						for(p6=0;p6<3;p6++)
						{
							x=0;
							y=0;
							ix=0;
							iy=0;
							if(p1==1) x++;
							else if(p1==2) y++;

							if(p2==1) x++,ix++;
							else if(p2==2) y++,iy++;

							if(p3==1) x++,ix++;
							else if(p3==2) y++,iy++;

							if(p4==1) x++,ix++;
							else if(p4==2) y++,iy++;

							if(p5==1) x++,ix++;
							else if(p5==2) y++,iy++;

							if(p6==1) ix++;
							else if(p6==2) iy++;

							//黑冲四
							if((x==4 && y==0) || (ix==4 && iy==0))
							{
								if(score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=BLOCK4;  //（ 5 // 1000）
								}
							}

							//白冲四
							else if((x==0 && y==4) || (ix==0 && iy==4))
							{
								if(score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=block4;  //（ 6 // -1000）
								}
							}

							//黑眠三
							else if((x==3 && y==0) || (ix==3 && iy==0))
							{
								if(score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=BLOCK3;  //（ 9 // 400）
								}
							}

							//白眠三
							else if((x==0 && y==3) || (ix==0 && iy==3))
							{
								if(score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=block3;  //（ 10 // -600）
								}
							}

							//黑眠二
							else if((x==2 && y==0) || (ix==2 && iy==0))
							{
								if(score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=BLOCK2; //（ 13 // 100）
								}
							}

							//白眠二
							else if((x==0 && y==2) || (ix==0 && iy==2))
							{
								if(score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=block2;  //（ 14 // -150）
								}
							}

						}
					}
				}
			}
		}
	}
	
}


int get_state_value(Board b){
	std::array<std::array<int, SIZE>, SIZE> temp_board = b.board;
	
	std::array<std::array<int, SIZE>, SIZE> board;

	if(player == 2){
		for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if(temp_board[i][j] == 0){
                   board[i][j] = 0;
                }
				if(temp_board[i][j] == 1){
                   board[i][j] = 2;
                }
				if(temp_board[i][j] == 2){
                   board[i][j] = 1;
                }
            }
        }

	}
	else if(player == 1)
	{
		board = temp_board;
	}
    //各棋型权重
	int i,j,s;

	int stat[4][17]={0};

	for(i=0;i<15;i++)
	{
		for(j=0;j<10;j++)
		{
			s=score[board[i][j]][board[i][j+1]][board[i][j+2]][board[i][j+3]][board[i][j+4]][board[i][j+5]];
			stat[1][s]++;
		}
	}

	for(j=0;j<15;j++)
	{
		for(i=0;i<10;i++)
		{
			s=score[board[i][j]][board[i+1][j]][board[i+2][j]][board[i+3][j]][board[i+4][j]][board[i+5][j]];
			stat[2][s]++;
		}
	}

	for(i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			s=score[board[i][j]][board[i+1][j+1]][board[i+2][j+2]][board[i+3][j+3]][board[i+4][j+4]][board[i+5][j+5]];
			stat[3][s]++;
		}
	}

	for(i=5;i<15;i++)
	{
		for(j=0;j<10;j++)
		{
			s=score[board[i][j]][board[i-1][j+1]][board[i-2][j+2]][board[i-3][j+3]][board[i-4][j+4]][board[i-5][j+5]];
			stat[0][s]++;
		}
	}

	s=0;

	for(i=1;i<17;i++)
	{
		s+=(stat[1][i]+stat[2][i]+stat[3][i]+stat[0][i])*weight[i]; //total score
	}
	
	return s;
}
