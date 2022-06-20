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

int Board::get_state_value()
{
    int invboard[15][15];
    if(cur_player == 2){
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
    else if(cur_player == 1){
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
