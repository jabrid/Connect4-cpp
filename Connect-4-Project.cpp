#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>

void draw_board();

#define HEIGHT 6
#define WIDTH 7
int counter = 0;
bool winner = false;

int board_info[HEIGHT][WIDTH] = {{0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0}};

bool check_for_winner(int x, int y, int player);
bool check_diagonal_combo_SW_NE(int x, int y, int player);
bool check_diagonal_combo_NW_SE(int x, int y, int player);
bool check_vertical_combo(int x, int y, int player);
bool check_horizontal_combo(int x, int y, int player);

int LastMoveX, LastMoveY;

int main()
{

	std::cout << "************************** \n";
	std::cout << "************************** \n";
	std::cout << "****** CONNECT FOUR ****** \n";
	std::cout << "************************** \n";
	std::cout << "************************** \n";
	std::cout << "*****X = ORDINATEUR****** \n";
	std::cout << "*******O = JOUEUR******** \n";

	int tube[2];
	int tube2[2];
	int x=0;
	int y;

	int message_read, message_read2, message_write, message_write2;

	pipe(tube);
	pipe(tube2);

	pid_t pid = fork();

	// child process
	if(pid == 0)
	{

		while (x==0)  //child process
		{


			read(tube2[0], &message_read2, 4 );

			std::cout << "\nCoup jouer par l'ordinateur:\n";
			int player =1;
			int number2 = 0;
			   while (board_info[(HEIGHT-1)-number2][(message_read2-1)] != 0)
			   {
				  number2++;
			   };

			   board_info[(HEIGHT-1)-number2][message_read2-1] = player;
			   LastMoveY = (HEIGHT-1)-number2;
			   LastMoveX = message_read2-1;

			draw_board();

			winner = check_for_winner(LastMoveX, LastMoveY, 1);
				  if (winner)
				  {
					 std::cout << "\n****L'ordinateur est le gagnant!***\n";
					 break;
				  }

			std::cout << "\nChoisir un nombre de 1 a 7: ";
			std::cin >> message_write;

			player = 2;

			while(message_write > 7 || message_write <= 0)
			{
				std::cout << "Erreur. Choisir un nombre valide: ";
				std::cin >>message_write;

			}

			int number = 0;
			
			while (board_info[(HEIGHT-1)-number][(message_write-1)] != 0)
			{
			  number++;
			  if (number > (HEIGHT-1))
			  {
				 std::cout << "\nErreur, choisir a nouveau: ";
				 std::cin >> message_write;
				 number = 0;
			  }
			};

		    board_info[(HEIGHT-1)-number][message_write-1] = player;
		    LastMoveY = (HEIGHT-1)-number;
		    LastMoveX = message_write-1;

			draw_board();
			winner = check_for_winner(LastMoveX, LastMoveY, 2);
			
			if (winner)
		    {
				std::cout << "\n***Le joueur est le gagnant!***\n";
				 break;
			}

			std::cout << "\n0 POUR SORTIR DU JEU";
			std::cout << "\n1 a 9 POUR CONTINUER\n";
			std::cin>> y;
			if (y == 0)
			{

			exit(EXIT_SUCCESS);
			}
			write(tube[1], &message_write, 4);

		}
	}

	else if(pid>0) //parent process
	{
		int player =1;
		while(x==0){

			std::cout << "\nL'ordinateur vient de jouer... \n";
			message_write2 = rand() % 7 + 1;

			while(message_write2 > 7 || message_write2 <= 0)
			{
				std::cout << "Erreur. Choisir un nombre valide: ";
				message_write2 = rand() % 7 + 1;
			}

			int number = 0;
			while (board_info[(HEIGHT-1)-number][(message_write2-1)] != 0)
			{
				  number++;
				  if (number > (HEIGHT-1))
				  {
					 std::cout << "\nErreur, choisir a nouveau: ";
					 message_write2 = rand() % 7 + 1;
					 number = 0;
				  }
			};

			   board_info[(HEIGHT-1)-number][message_write2-1] = player;
			   LastMoveY = (HEIGHT-1)-number;
			   LastMoveX = message_write2-1;

			std::cout << "\n0 POUR SORTIR DU JEU";
			std::cout << "\n1 a 9 POUR CONTINUER\n";
			std::cin>> y;
			if (y == 0)
			{
				exit(EXIT_SUCCESS);
			}
			
			write(tube2[1], &message_write2, 4);
			read(tube[0], &message_read, 4);
		
		}
	}
}

void draw_board()
{
   std::cout << "\n";

   for (int y = 0; y < HEIGHT; y++)
   {
      for (int x = 0; x < WIDTH; x++)
      {
         std::cout << "| ";
         if (board_info[y][x] == 0) std::cout << " ";
         else if (board_info[y][x] == 1) std::cout << "X";
         else if (board_info[y][x] == 2) std::cout << "O";
      }
      std::cout << "\n---------------------\n";
   }
}

bool check_for_winner(int x, int y, int player)
{
   bool winner;

   if (check_diagonal_combo_SW_NE(x,y,player)) return true;
   else if (check_diagonal_combo_NW_SE(x,y,player)) return true;
   else if (check_vertical_combo(x,y,player)) return true;
   else if (check_horizontal_combo(x,y,player)) return true;
   else return false;
}




bool check_diagonal_combo_SW_NE(int x, int y, int player)
{
   int score = 1;
   int count = 1;

   while((y-count >= 0) && (x+count < WIDTH))
   {
      if (board_info[y-count][x+count] == player)
      {
         score++;
         count++;
      }
      else break;
   }

   count = 1;
   while((y+count < HEIGHT) && (x-count >=0))
   {
      if (board_info[y+count][x-count] == player)
      {
         score++;
         count++;
      }
      else break;
   }

   if (score == 4) return true;
   else return false;
}

bool check_diagonal_combo_NW_SE(int x, int y, int player)
{
   int score = 1;
   int count = 1;

   while((y+count >= 0) && (x+count < WIDTH))
   {
      if (board_info[y+count][x+count] == player)
      {
         score++;
         count++;
      }
      else break;
   }

   count = 1;
   while((y-count < HEIGHT) && (x-count >=0))
   {
      if (board_info[y-count][x-count] == player)
      {
         score++;
         count++;
      }
      else break;
   }

   if (score == 4) return true;
   else return false;
}

bool check_vertical_combo(int x, int y, int player)
{
   int score = 1;
   int count = 1;

   while(y+count >= 0 && y+count < HEIGHT)
   {
      if (board_info[y+count][x] == player)
      {
         score++;
         count++;
      }
      else break;
   }

   if (score == 4) return true;
   else return false;
}

bool check_horizontal_combo(int x, int y, int player)
{
   int score = 1;
   int count = 1;

   while((x+count >= 0) && (x+count < WIDTH))
   {
      if (board_info[y][x+count] == player)
      {
         score++;
         count++;
      }
      else break;
   }

   count = 1;
   while((x-count < WIDTH) && (x-count >=0))
   {
      if (board_info[y][x-count] == player)
      {
         score++;
         count++;
      }
      else break;
   }

   if (score == 4) return true;
   else return false;
}
