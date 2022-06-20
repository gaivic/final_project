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

#define P std::pair<int, int>



struct Point {
    int x, y;
	Point() : Point(0, 0) {}
	Point(float x, float y) : x(x), y(y) {}
	bool operator==(const Point& rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const Point& rhs) const {
		return !operator==(rhs);
	}
	Point operator+(const Point& rhs) const {
		return Point(x + rhs.x, y + rhs.y);
	}
	Point operator-(const Point& rhs) const {
		return Point(x - rhs.x, y - rhs.y);
	}
};

class Board {
public:
    enum SPOT_STATE {
        EMPTY = 0,
        BLACK = 1,
        WHITE = 2
    };
    static const int SIZE = 15;
    std::array<std::array<int, SIZE>, SIZE> board;
    int empty_count;
    int cur_player;
    bool done;
    int winner;
private:
    int get_next_player(int player) const {
        return 3 - player;
    }
    bool is_spot_on_board(Point p) const {
        return 0 <= p.x && p.x < SIZE && 0 <= p.y && p.y < SIZE;
    }
    int get_disc(Point p) const {
        return board[p.x][p.y];
    }
    void set_disc(Point p, int disc) {
        board[p.x][p.y] = disc;
    }
    bool is_disc_at(Point p, int disc) const {
        if (!is_spot_on_board(p))
            return false;
        if (get_disc(p) != disc)
            return false;
        return true;
    }
    bool is_spot_valid(Point center) const {
        if (get_disc(center) != EMPTY)
            return false;
        return true;
    }
    
public:
    Board(std::array<std::array<int, SIZE>, SIZE> cur_board, int player) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = cur_board[i][j];
            }
        }
        cur_player = player;
    }
    Board(Board const& cur_board){
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = cur_board.board[i][j];
            }
        }
        cur_player = cur_board.cur_player;
        done = cur_board.done;
        winner = cur_board.winner;
    }
    std::vector<Point> get_valid_spots()const{
        std::vector<Point> valid_spots;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                Point p (i, j);
                if(is_spot_valid(p))
                    valid_spots.push_back(p);
            }
        }
        return valid_spots;
    }
    bool put_disc(Point p) {
        if(!is_spot_valid(p)) {
            winner = get_next_player(cur_player);
            done = true;
            return false;
        }
        set_disc(p, cur_player);
        empty_count--;
        // Check Win
        if (checkwin(cur_player)) {
            done = true;
            winner = cur_player;
        }
        if (empty_count == 0) {
            done = true;
            winner = EMPTY;
        }

        // Give control to the other player.
        cur_player = get_next_player(cur_player);
        return true;
    }
    bool checkwin(int disc){
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (is_disc_at(Point(i, j), disc)){
                    bool iswin = true;
                    if (i + 4 < SIZE) {
                        for(int k = 0; k < 5; k++)
                            if (!is_disc_at(Point(i+k, j), disc)) {
                                iswin = false;
                                break;
                            }
                        if (iswin) return true;
                    }
                    iswin = true;
                    if (j + 4 < SIZE) {
                        for(int k = 0; k < 5; k++)
                            if (!is_disc_at(Point(i, j+k), disc)) {
                                iswin = false;
                                break;
                            }
                        if (iswin) return true;
                    }
                    iswin = true;
                    if (i + 4 < SIZE && j + 4 < SIZE) {
                        for(int k = 0; k < 5; k++)
                            if (!is_disc_at(Point(i+k, j+k), disc)) {
                                iswin = false;
                                break;
                            }
                        if (iswin) return true;
                    }
                    iswin = true;
                    if (i - 4 >= 0 && j + 4 < SIZE) {
                        for(int k = 0; k < 5; k++)
                            if (!is_disc_at(Point(i-k, j+k), disc)) {
                                iswin = false;
                                break;
                            }
                        if (iswin) return true;
                    }
                }
            }
        }
        return false;
    }
    
    //calculate the value of the board
    int get_state_value();
    int score_return(int board[15][15]);

    
};
enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

int player;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> root_board;
std::vector<Point> next_valid_spots;

void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> root_board[i][j];
            if(root_board[i][j] == 0)
                next_valid_spots.push_back({(float)i, (float)j});
        }
    }
}



int MinMax(Board cur, int depth, int alpha , int beta, int next_player){
    if(depth == 0 || cur.done) return cur.get_state_value();
    
    // if maximizing player
    if(next_player == player){
        
        int value = -INF;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if(cur.board[i][j] == 0){
                    Board next(cur);
                    Point p(i, j);
                    next.put_disc(p);
                    value = std::max( value, MinMax(next, depth-1, alpha, beta, 3-next_player));
                    alpha = std::max(alpha, value);
                    if(alpha >= beta) break;
                }
            }
        }
        return value;
    }
    else if(next_player == 3- player){

        int value = INF;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if(cur.board[i][j] == 0){
                    Board next(cur);
                    Point p(i, j);
                    next.put_disc(p);
                    value = std::min( value, MinMax(next, depth-1, alpha, beta, 3-next_player));
                    beta = std::min(alpha, value);
                    if(alpha >= beta) break;
                }
            }
        }
        return value;
    }
    
}

// root board 
void write_valid_spot(std::ofstream& fout) {
    int n_valid_spots = (int)next_valid_spots.size();
    srand(time(NULL));

    int index = -1;
    int alpha = -INF;
    int beta = INF;


    for(int i=0;i < n_valid_spots;i++){
        Board next(root_board , player);
        next.put_disc(next_valid_spots[i]);
        int value = MinMax(next , 0 ,alpha , beta , 3-player);
        if(value > alpha){
            alpha = value , index = i;
        } 
    }

    Point p = next_valid_spots[index];
    fout << p.x << " " << p.y << std::endl;
    fout.flush();
    

}


int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);

    read_board(fin);
    write_valid_spot(fout);

    fin.close();
    fout.close();
    return 0;
}


int Board::score_return(int board[15][15]){

    int score[3][3][3][3][3][3];
    //黑五连 boardI胜 (这里的黑不是指先手，而是指boardI)（棋型 1 //4000）
	score[1][1][1][1][1][1]=WIN;
	score[1][1][1][1][1][0]=WIN;
	score[0][1][1][1][1][1]=WIN;
	score[1][1][1][1][1][2]=WIN;
	score[2][1][1][1][1][1]=WIN;

	//白五连 boardI负  （棋型 2 // -4000）
	score[2][2][2][2][2][2]=LOSE;
	score[2][2][2][2][2][0]=LOSE;
	score[0][2][2][2][2][2]=LOSE;
	score[2][2][2][2][2][1]=LOSE;
	score[1][2][2][2][2][2]=LOSE;


	//黑活四   （棋型 3 // 2000）
	score[0][1][1][1][1][0]=FLEX4;

	//白活四	（棋型 4 // -2000）
	score[0][2][2][2][2][0]=flex4;


	//黑活三	（棋型 7 // 1000）
	score[0][1][1][1][0][0]=FLEX3;
	score[0][1][1][0][1][0]=FLEX3;
	score[0][1][0][1][1][0]=FLEX3;
	score[0][0][1][1][1][0]=FLEX3;

	//白活三	（棋型 8 // -1000）
	score[0][2][2][2][0][0]=flex3;
	score[0][2][2][0][2][0]=flex3;
	score[0][2][0][2][2][0]=flex3;
	score[0][0][2][2][2][0]=flex3;


	//黑活二	（棋型 11 // 400）
	score[0][1][1][0][0][0]=FLEX2;
	score[0][1][0][1][0][0]=FLEX2;
	score[0][1][0][0][1][0]=FLEX2;
	score[0][0][1][1][0][0]=FLEX2;
	score[0][0][1][0][1][0]=FLEX2;
	score[0][0][0][1][1][0]=FLEX2;

	//白活二	（棋型 12 // -600）
	score[0][2][2][0][0][0]=flex2;
	score[0][2][0][2][0][0]=flex2;
	score[0][2][0][0][2][0]=flex2;
	score[0][0][2][2][0][0]=flex2;
	score[0][0][2][0][2][0]=flex2;
	score[0][0][0][2][2][0]=flex2;


	//黑活一	（棋型 15 // 100）
	score[0][1][0][0][0][0]=FLEX1;
	score[0][0][1][0][0][0]=FLEX1;
	score[0][0][0][1][0][0]=FLEX1;
	score[0][0][0][0][1][0]=FLEX1;

	//白活一	（棋型 16 // -150）
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
									score[p1][p2][p3][p4][p5][p6]=BLOCK4;  //（棋型 5 // 1000）
								}
							}

							//白冲四
							else if((x==0 && y==4) || (ix==0 && iy==4))
							{
								if(score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=block4;  //（棋型 6 // -1000）
								}
							}

							//黑眠三
							else if((x==3 && y==0) || (ix==3 && iy==0))
							{
								if(score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=BLOCK3;  //（棋型 9 // 400）
								}
							}

							//白眠三
							else if((x==0 && y==3) || (ix==0 && iy==3))
							{
								if(score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=block3;  //（棋型 10 // -600）
								}
							}

							//黑眠二
							else if((x==2 && y==0) || (ix==2 && iy==0))
							{
								if(score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=BLOCK2; //（棋型 13 // 100）
								}
							}

							//白眠二
							else if((x==0 && y==2) || (ix==0 && iy==2))
							{
								if(score[p1][p2][p3][p4][p5][p6]==0)
								{
									score[p1][p2][p3][p4][p5][p6]=block2;  //（棋型 14 // -150）
								}
							}

						}
					}
				}
			}
		}
	}
    //各棋型权重
	int weight[17]={0,4000,-4000,2000,-2000,1000,-1000,1000,-1000,400,-600,400,-600,100,-150,100,-150};

	int i,j,s;

	int stat[4][17]={0};

	int STAT[17];

	//棋型统计
	/*
	0 1 2 ... 14
	1
	2
	.
	.
	.
	14
	*/
	for(i=0;i<15;i++)
	{
		for(j=0;j<10;j++)
		{
			s=score[board[i][j]][board[i][j+1]][board[i][j+2]][board[i][j+3]][board[i][j+4]][board[i][j+5]];
			/*
			横向五子的情况
			i=0,j=0 board[0][0] board[0][1] board[0][2] board[0][3] board[0][4] board[0][5]
				j=1 board[0][1,2,3,4,5,6]
				...
				j=9 board[0][9,10,11,12,13,14,15]
			...
			i=14,j=0 board[14][0,1,2,3,4,5]
				j=1 board[14][1,2,3,4,5,6]
				...
				j=9 board[14][9,10,11,12,13,14,15]
			*/
			stat[1][s]++;
		}
	}

	for(j=0;j<15;j++)
	{
		for(i=0;i<10;i++)
		{
			s=score[board[i][j]][board[i+1][j]][board[i+2][j]][board[i+3][j]][board[i+4][j]][board[i+5][j]];
			/*
			竖向五子的情况
			j=0,i=0 board[0][0]
					board[1][0]
					board[2][0]
					board[3][0]
					board[4][0]
					board[5][0]

			*/
			stat[2][s]++;
		}
	}

	for(i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
		{
			s=score[board[i][j]][board[i+1][j+1]][board[i+2][j+2]][board[i+3][j+3]][board[i+4][j+4]][board[i+5][j+5]];
			/*
			斜向五子的情况1
			i=0,j=0 board[0][0]
						board[1][1]
							board[2][2]
							...
								board[5][5]
			*/
			stat[3][s]++;
		}
	}

	for(i=5;i<15;i++)
	{
		for(j=0;j<10;j++)
		{
			s=score[board[i][j]][board[i-1][j+1]][board[i-2][j+2]][board[i-3][j+3]][board[i-4][j+4]][board[i-5][j+5]];
			/*
			斜向五子的情况2
			i=5,j=0 board[5][0]
				board[4][1]
			board[3][2]
		board[2][3]
	board[1][4]
board[0][5]
			*/
			stat[0][s]++;
		}
	}

	s=0;

	//初步评分累加
	for(i=1;i<17;i++)
	{
		s+=(stat[1][i]+stat[2][i]+stat[3][i]+stat[0][i])*weight[i]; //当前棋局的得分

		STAT[i]=(stat[1][i]>0)+(stat[2][i]>0)+(stat[3][i]>0)+(stat[0][i]>0);  //存在这种棋型的方向的个数
	}

	return s;
}

int Board::get_state_value()
{
    
    if(cur_player == 1){
        int invboard[15][15];
        for(int i=0; i<15; i++)
	    {
		    for(int j=0; j<15; j++)
		    {
		        if(board[i][j] == 0)
                    invboard[i][j] = 0;
                if(board[i][j] == 1)
                    invboard[i][j] = 2;
                if(board[i][j] == 2)
                    invboard[i][j] = 1;

            }
	    }
        return score_return(invboard);
    }
    
    else if(cur_player == 2){

        int nor_board[15][15];
        for(int i=0; i<15; i++)
	    {
		    for(int j=0; j<15; j++)
		    {
		        if(board[i][j] == 0)
                    nor_board[i][j] = 0;
                if(board[i][j] == 1)
                    nor_board[i][j] = 1;
                if(board[i][j] == 2)
                    nor_board[i][j] = 2;

            }
	    }
        return score_return(nor_board);

    }
}
