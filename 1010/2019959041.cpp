#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <ctype.h>
#include <string.h>


int intro_game(void);
void display_game_over(long minutes, long seconds);
void draw_check02(int r, int c);
void gotoxy(int x, int y);
void number_display(int n);
void display_rule(int n);
void make_question(int n, char question[][10]);
int game_control(char question[][10], char answer[][10], int n, int *count);
void clear_text(int n);

int main(void)
{
	srand(time(NULL));

	while(1)
	{
		int n, count = 0;
		char question[10][10] = {0};
		char answer[10][10] = {0};
		long pst, seconds, minutes;
		clock_t start, end;
		bool game_interrupted = false;

		n = intro_game();

		if (n == 0) {
			break;
		}

		system("cls");
		draw_check02(n, n);
		number_display(n);
		make_question(n, question);
		display_rule(n);
		
		start = clock();
		do
		{
			if (game_control(question, answer, n, &count) == -1) {
				game_interrupted = true;
				break;
			}
		} while (count < n * n / 2);

		if (!game_interrupted) {
			end = clock();
			pst = (end - start) / CLOCKS_PER_SEC;
			minutes = pst / 60;
			seconds = pst % 60;
			
			display_game_over(minutes, seconds);

			char key = getch();
			if (key == 27) {
				break;
			}
		}
	}
	system("cls");
	printf("Thank you for playing!\n");
	return 0;
}

void gotoxy(int x, int y)
{
	COORD Pos = {x - 1, y - 1};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

int intro_game(void)
{
	int n = 0;
	char key;
	system("cls");
    printf("\n");
	printf("========================================\n");
	printf("||                                    ||\n");
	printf("||     CARD MATCHING MEMORY GAME      ||\n");
	printf("||                                    ||\n");
	printf("========================================\n\n");
	printf("Match all the pairs of cards!\n\n");

	do {
		printf("Please select a difficulty level:\n");
        printf("1. Level 1 (4x4)\n");
        printf("2. Level 2 (6x6)\n");
        printf("3. Level 3 (8x8)\n\n");
        printf("(Press ESC to exit)\n");
        printf(">> ");

		key = getch();

		if (key == '1') n = 4;
		else if (key == '2') n = 6;
		else if (key == '3') n = 8;
		else if (key == 27) {
			return 0;
		} else {
			system("cls");
			printf("\nInvalid input. Please enter 1, 2, or 3.\n\n");
		}
	} while (n == 0);
	
	printf("\n\nPress any key to start the game...");
	getch();
	return n;
}

void display_game_over(long minutes, long seconds)
{
	system("cls");
	printf("\n");
	printf("========================================\n");
	printf("||                                    ||\n");
	printf("||         G A M E  C L E A R         ||\n");
	printf("||                                    ||\n");
	printf("========================================\n\n");

	printf("          Your Record: %ld minutes %ld seconds\n\n", minutes, seconds);

	printf("   Press any key to return to the title...\n");
	printf("          (Press ESC to exit)\n");
}


void clear_text(int n)
{
	int i;
	for (i=0; i<3; i++)
	{
		gotoxy(1, 5 + (n*2) + 2 + i);
		printf("                                                              ");
	}
}

int game_control(char question[][10], char answer[][10], int n, int *count)
{
   int x1, y1_num, x2, y2_num;
   char y1_char, y2_char;
   char buffer[20];

   while (1) {
	   gotoxy(2, 5 + (n*2) + 2);
	   printf("Enter coordinates for the first card (e.g., 1 A): ");
	   
	   fgets(buffer, sizeof(buffer), stdin);
       if (kbhit()){
            if(getch()==27) return -1;
       }

       if (strcmp(buffer, "end\n") == 0) {
           *count = (n*n)/2;
           return 0;
       }


	   if (sscanf(buffer, "%d %c", &x1, &y1_char) == 2) {
		   y1_char = toupper(y1_char);
		   y1_num = y1_char - 'A' + 1;

		   if (x1 > 0 && x1 <= n && y1_num > 0 && y1_num <= n) {
			   if(answer[y1_num-1][x1-1] == 'O') {
					gotoxy(2, 5 + (n*2) + 4);
					printf("This card is already open. Choose another one.");
					Sleep(1000);
					gotoxy(2, 5 + (n*2) + 4); printf("                                              ");
					continue;
			   }
			   break;
		   }
	   }
	   gotoxy(2, 5 + (n*2) + 4);
	   printf("Invalid format. Please enter like '1 A' (number: 1-%d, letter: A-%c).", n, 'A'+n-1);
	   Sleep(1500);
	   gotoxy(2, 5 + (n*2) + 4); printf("                                                                      ");
	   gotoxy(2, 5 + (n*2) + 2); printf("                                                                      ");
   }
   gotoxy(5 + 4 * (x1 - 1), 4 + 2 * (y1_num - 1));
   printf("%c", question[y1_num-1][x1-1]);

   while (1) {
	   gotoxy(2, 5 + (n*2) + 3);
	   printf("Enter coordinates for the second card (e.g., 1 A): ");

	   fgets(buffer, sizeof(buffer), stdin);
	   if (kbhit()){
            if(getch()==27) return -1;
       }
	   
	   if (sscanf(buffer, "%d %c", &x2, &y2_char) == 2) {
			y2_char = toupper(y2_char);
			y2_num = y2_char - 'A' + 1;

			if (x2 > 0 && x2 <= n && y2_num > 0 && y2_num <= n) {
				if(answer[y2_num-1][x2-1] == 'O') {
					gotoxy(2, 5 + (n*2) + 4);
					printf("This card is already open. Choose another one.");
					Sleep(1000);
					gotoxy(2, 5 + (n*2) + 4); printf("                                              ");
					continue;
				}
				if (x1 == x2 && y1_num == y2_num) {
					gotoxy(2, 5 + (n*2) + 4);
					printf("You chose the same card. Choose a different one.");
					Sleep(1000);
					gotoxy(2, 5 + (n*2) + 4); printf("                                                  ");
					continue;
				}
				break;
			}
	   }
	   gotoxy(2, 5 + (n*2) + 4);
	   printf("Invalid format. Please enter like '1 A' (number: 1-%d, letter: A-%c).", n, 'A'+n-1);
	   Sleep(1500);
	   gotoxy(2, 5 + (n*2) + 4); printf("                                                                      ");
	   gotoxy(2, 5 + (n*2) + 3); printf("                                                                      ");
   }
   gotoxy(5 + 4 * (x2 - 1), 4 + 2 * (y2_num - 1));
   printf("%c", question[y2_num-1][x2-1]);

   if (question[y1_num-1][x1-1]==question[y2_num-1][x2-1])
   {
       Beep(784, 200);
	   answer[y1_num-1][x1-1]='O';
	   answer[y2_num-1][x2-1]='O';
	   (*count)++;
   } else {
       Beep(261, 200);
   }
   Sleep(1000);

   for(y1_num=0; y1_num<n; y1_num++)
   {
	   for(x1=0; x1<n; x1++)
	   {
		   gotoxy(5 + 4 * x1, 4 + 2 * y1_num);
		   if (answer[y1_num][x1]=='O')
			   printf("%c", question[y1_num][x1]);
		   else
			   printf("#");
	   }
   }
   clear_text(n);
   return 0;
}

void number_display(int n)
{
   int i;
   for(i=1;i<=n;i++)
   {
	   gotoxy(5 + 4 * (i-1), 2);
	   printf("%d", i);
   }
   for(i=1;i<=n;i++)
   {
	   gotoxy(1, 4 + 2 * (i - 1));
	   printf("%c", 'A'+i-1);
   }
}

void display_rule(int n)
{
   int i, j;
   for(i=0; i<n; i++)
   {
	   for(j=0; j<n; j++)
	   {
		  gotoxy(5 + 4 * j, 4 + 2 * i);
		  printf("#");
	   }
   }
}

void make_question(int n, char question[][10])
{
   int i, j;
   int board_size = n * n;
   int num_pairs = board_size / 2;
   char deck[100];

   char card_types[50]={'A','B','C','D','E','F','G','H','I','J',
	   'K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	   '!','@','#','$','%','^','&','*','(',')','_','+'};

   for(i = 0; i < num_pairs; i++) {
       deck[i*2] = card_types[i];
       deck[i*2 + 1] = card_types[i];
   }

   for(i = board_size - 1; i > 0; i--) {
       int rnd = rand() % (i + 1);
       char temp = deck[i];
       deck[i] = deck[rnd];
       deck[rnd] = temp;
   }

   int k = 0;
   for(i = 0; i < n; i++) {
       for(j = 0; j < n; j++) {
           question[i][j] = deck[k++];
       }
   }
}

void draw_check02(int c, int r)
{
    int i, j;
    
    gotoxy(3, 3);
    printf("+");
    for (i = 0; i < c; i++) printf("---+");

    for (i = 0; i < r; i++) {
        gotoxy(3, 4 + i * 2);
        printf("|");
        for (j = 0; j < c; j++) printf("   |");
        
        gotoxy(3, 5 + i * 2);
        printf("+");
        for (j = 0; j < c; j++) printf("---+");
    }
}

