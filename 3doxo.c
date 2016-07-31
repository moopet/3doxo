#include<conio.h>
#include<time.h>              /* For random(), just in case */
#include<stdio.h>
#include<stdlib.h>


#define  flushkeybuf()  while ( kbhit() ) getch()
#define  MySymbol			(move % 2) ? 'X' : 'O'
#define  TheirSymbol		(move % 2) ? 'O' : 'X'

#define  EMPTY          0
#define  OFF            0
#define  ON             1
#define  HC             0
#define  CH             1
#define  HH             2
#define  CC             3

char     grid[65];
int      move, flashwin;
int      player       = HC;


				/**************************************************/
				/* Main menu and other related bollocks           */
				/**************************************************/

main()
  {
  int  menuopt, pos;
  char scrbuf[4000];

  tits();

  cleargrid();

  gotoxy(33, 22);
  displine("Press ? for Help");
  gotoxy(18, 24);
  displine("* * *");
  textcolor(RED);
  cprintf(" 3DOXO v2 Copyright(c) 1993, (now PD) ");
  displine("* * *");
  gotoxy(12, 25);
  displine("* * *");
  textcolor(RED);
  cprintf(" By Ben Sinclair, & with thanks to Neil Crabbe ");
  displine("* * *");

  flushkeybuf();
  while( !kbhit() );
  gotoxy(15, 24);
  clreol();

  while(1)
	 {
	 menuopt = 0;
	 gotoxy(33, 22);
	 if ( move < 65 )
		{
		displine(" MOVE #");
		textcolor(BROWN);
		cprintf("%02d", move);
		displine(", [");
		textcolor(BROWN);
		putch(MySymbol);
		displine("]");
		clreol();
		}
	 gotoxy(15, 23);
	 clreol();
	 gotoxy(1, 25);
	 clreol();

	 while ( player == CC && move > 1 && move < 65 && !kbhit() )
		{
		gotoxy(34, 22);
		displine("MOVE #");
		textcolor(MAGENTA);
		cprintf("%02d", move);
		displine(", [");
		textcolor(MAGENTA);
		putch(MySymbol);
		displine("]");
		compplay();
		}

	 if ( move == 65 )
		{
		gotoxy(15, 23);
		clreol();
		gotoxy(35, 23);
		printf("Stalemate!");
		gameover();
		}

	 pos = 0;
	 menuopt = getch();
	 menuopt -= ( 'a' - 'A' ) * ( menuopt >= 'a' && menuopt <= 'z' );

	 switch ( menuopt )
		{
		case 27 :
		case 'Q':   if ( move == 1 )
							endall();
						else if ( rusure("REALLY quit") )
						  endall();
						break;
		case 'N':   if ( move == 1 )
							beep();
						else if ( rusure("End current game") )
						  cleargrid();
						break;
		case 'O':   gettext(1, 1, 80, 25, scrbuf);
						textcolor(LIGHTGRAY);
						clrscr();
						textcolor(WHITE);
						cprintf("Type ");
						textcolor(LIGHTRED);
						cprintf("EXIT");
						textcolor(WHITE);
						cprintf(" to return to 3DOXO.\n\n");
						cursor(ON);
						spawnl(0, getenv("COMSPEC"), 0L);
						puttext(1, 1, 80, 25, scrbuf);
						cursor(OFF);
						break;
		case '?':   gettext(1, 1, 80, 25, scrbuf);
						clrscr();
						system("TYPE 3DOXO.TXT | MORE");
						gotoxy(1, 25);
						printf("\n\n Press any key to continue");
						flushkeybuf();
						getch();
						puttext(1, 1, 80, 25, scrbuf);
						break;
		case 'P':   if ( move > 1 ) break;
						if ( player != CH && player != CC )
							beep();
						else
							{
							gotoxy(34, 24);
							printf("              ");
							compplay();
							}
						break;
		case 'S':   if ( move > 1 )
							beep();
						else
							{
							if ( ++player > CC )
							  player = HC;
							dispplayer();
							}
						break;
		case 'A':
		case 'B':
		case 'C':
		case 'D':   if ( (move == 1 && (player == CH || player == CC)) || move == 64 )
							beep();
						else
							{
							gotoxy(39, 23);
							printf("%c,?            Z ¯ Cancel Move", menuopt);
							gotoxy(41, 23);
							cursor(ON);
							while( (pos < 'A' || pos >'P') && pos != 'Z' )
								{
								pos = getch();
								pos -= ('a' - 'A') * ( pos >= 'a' && pos <= 'z' );
								if ( (pos < 'A' || pos > 'P') && pos != 'Z' )
								  beep();
								}
							cursor(OFF);
							if ( pos != 'Z' )
								{
								printf("%c", pos);
								clreol();
								pos += ((menuopt - 'A') * 16) - 'A';
								if ( grid[pos] != EMPTY )
									{
									gotoxy(37, 25);
									printf("Bad Move!");
									beep();
									delay(600);
									}
								else
									{
									grid[pos] = MySymbol;
									dispgrid();
									listmove(pos);
									move++;
									delay(400);
									if ( iswin() )
										{
										flashwin = OFF;
										gotoxy(15, 23);
										clreol();
										gotoxy(30, 23);
										printf("Player %c wins (Human)", TheirSymbol);
										gameover();
										}
									else
										{
										flashwin = OFF;
										if ( player != HH )
											compplay();
										}
									while( !kbhit() );
									}
								}
							}
		default :	   break;
		}
	 }
  }

				/**************************************************/
				/* Ask you if you want to play again              */
				/**************************************************/

gameover()
  {
  beep();
  clreol();
  if ( !rusure(" Play Again") )
	 endall();
  cleargrid();
  }

				/**************************************************/
				/* Get confirmation from the user for doing (st)  */
				/**************************************************/

rusure( char *question )
  {
  char c = 0;

  gotoxy(37 - strlen(question) / 2, 25);
  displine(question);
  displine("(y/n)? ");
  while ( c != 'Y' && c != 'y' && c != 'N' &&
			 c != 'n' && c != 13 && c != 27 )
	 c = getch();
  if ( c == 'Y' || c == 'y' || c == 13 )
	 displine("Yes.");
  else
	 displine("No.");
  delay(400);
  if ( c == 'Y' || c == 'y' || c == 13 )
	 return(1);
  return(0);
  }

				/**************************************************/
				/* Display a string in a colourised form          */
				/**************************************************/

displine(char *str)
  {
  int x;

  for (x = 0; x < strlen(str); x ++)
	 {
	 if ((str[x] >= 'a' && str[x] <= 'z') || (str[x] >= 'A' && str[x] <= 'Z') ||
		 (str[x] >= '0' && str[x] <= '9'))
		textcolor(BROWN);
	 else
		textcolor(GREEN);
	 putch(str[x]);
	 }
  }

				/**************************************************/
				/* Display the empty grid and menu stuff          */
				/**************************************************/

cleargrid()
  {
  int c;

  move     = 1;
  flashwin = OFF;
  for ( c = 0; c < 64; c++ )
	 grid[c] = EMPTY;

  gotoxy(1, 1);
  textcolor(GREEN);
  cprintf("  ù ³ ù ³ ù ³ ù        ù ³ ù ³ ù ³ ù        ù ³ ù ³ ù ³ ù        ù ³ ù ³ ù ³ ù \n\r");
  cprintf(" ÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ      ÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ      ÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ      ÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ\n\r");
  cprintf("  ù ³ ù ³ ù ³ ù        ù ³ ù ³ ù ³ ù        ù ³ ù ³ ù ³ ù        ù ³ ù ³ ù ³ ù \n\r");
  cprintf(" ÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ      ÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ      ÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ      ÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ\n\r");
  cprintf("  ù ³ ù ³ ù ³ ù        ù ³ ù ³ ù ³ ù        ù ³ ù ³ ù ³ ù        ù ³ ù ³ ù ³ ù \n\r");
  cprintf(" ÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ      ÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ      ÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ      ÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ\n\r");
  cprintf("  ù ³ ù ³ ù ³ ù        ù ³ ù ³ ù ³ ù        ù ³ ù ³ ù ³ ù        ù ³ ù ³ ù ³ ù \n\r");
  textcolor(GREEN);
  cprintf("\n        %c                    %c                    %c                    %c\n\r", 127, 127, 127, 127);
  displine("       [A]                  [B]                  [C]                  [D]");
  printf("\n\n");
  gotoxy(2, 13);
  displine("[X] PLAYER:                     A ³ B ³ C ³ D");
  gotoxy(2, 14);
  displine("[O] PLAYER:                    ÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ      A-D ¯ Enter Move");
  gotoxy(2, 15);
  displine("                                E ³ F ³ G ³ H       S ¯ Set Players");
  gotoxy(33, 16);
  displine("ÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ      N ¯ New Game");
  gotoxy(2, 17);
  displine("                                I ³ J ³ K ³ L       O ¯ OS Shell");
  gotoxy(33, 18);
  displine("ÄÄÄÅÄÄÄÅÄÄÄÅÄÄÄ      Q ¯ Quit");
  gotoxy(33, 19);
  displine(" M ³ N ³ O ³ P");
  gotoxy(1, 16);
  textcolor(BLUE);
  cprintf("ÚÄÄÄÄÄÄÄÄÄÄÄÄ¿");
  cprintf("\n\r³");
  textcolor(LIGHTGRAY);
  printf("LAST 5 MOVES");
  textcolor(BLUE);
  cprintf("³\n\rÃÄÄÄÄÄÄÄÄÄÄÄÄ´");
  cprintf("\n\r³            ³");
  cprintf("\n\r³            ³");
  cprintf("\n\r³            ³");
  cprintf("\n\r³            ³");
  cprintf("\n\r³            ³");
  cprintf("\n\rÀÄÄÄÄÄÄÄÄÄÄÄÄÙ");
  gotoxy(1, 25);
  clreol();
  dispplayer();
  }

				/**************************************************/
				/* Display the selection of players (HUMAN, etc)  */
				/**************************************************/

dispplayer()
  {
  gotoxy(14, 13);
  if ( player == HC || player == HH )
	 displine("HUMAN   ");
  else
	 displine("COMPUTER");
  gotoxy(14, 14);
  if ( player == CH || player == HH )
	 displine("HUMAN   ");
  else
	 displine("COMPUTER");
  gotoxy(34, 24);
  if ( (player == CH || player == CC) && move == 1 )
	 displine("P ¯ Start Play");
  else
	 displine("              ");
  }

				/**************************************************/
				/* Check to see if there is a winning line around */
				/**************************************************/

iswin()
  {
  register int c, r;

	/* X ROWS */

  for ( r = 0; r < 16; r++ )
	 {
	 if ( winline(r * 4, 1, 2, 3) )        return(grid[r * 4]);

	/* Z COLUMNS */

	 if ( winline(r, 16, 32, 48) )         return(grid[r]);
	 }

	/* Y COLUMNS */

  for ( c = 0; c < 60; c += 16 )
	 for ( r = 0; r < 4; r++ )
		if ( winline(c + r, 4, 8, 12) )     return(grid[c + r]);

	/* X-Y DIAGS */

  for ( c = 0; c < 4; c++ )
	 {
	 if ( winline(c * 16, 5, 10, 15) )     return(grid[c * 16]);
	 if ( winline(c * 16 + 3, 3, 6, 9) )   return(grid[c * 16 + 3]);

	/* X-Z DIAGS */

	 if ( winline(c, 20, 40, 60) )         return(grid[c]);
	 if ( winline(c + 12, 12, 24, 36) )    return(grid[c + 12]);

	/* Y-Z DIAGS */

	 if ( winline(c * 4, 17, 34, 51) )     return(grid[c * 4]);
	 if ( winline(c * 4 + 3, 15, 30, 45) ) return(grid[c * 4 + 3]);
	 }

	/* X-Y-Z DIAGS */

  if ( winline(0, 21, 42, 63) )           return(grid[0]);
  if ( winline(3, 19, 38, 57) )           return(grid[3]);
  if ( winline(12, 13, 26, 39) )          return(grid[12]);
  if ( winline(15, 11, 22, 33) )          return(grid[15]);

  return(0);
  }

				/**************************************************/
				/* Check to see if a particular line is a winner  */
				/**************************************************/

int winline( int x, int a, int b, int c )
  {
  if ( grid[x] == EMPTY ) return(0);
  if ( grid[x] == grid[x + a] && grid[x] == grid[x + b] &&
		 grid[x] == grid[x + c] )
	 {
	 if ( flashwin )
		{
		textcolor(YELLOW + BLINK);
		gotoxy(3 + (x / 16) * 21 + 4 * (x % 4),
				 1 + 2 * ((x % 16) / 4));
		putch(grid[x]);
		gotoxy(3 + ((x + a) / 16) * 21 + 4 * ((x + a) % 4),
				 1 + 2 * (((x + a) % 16) / 4));
		putch(grid[x]);
		gotoxy(3 + ((x + b) / 16) * 21 + 4 * ((x + b) % 4),
				 1 + 2 * (((x + b) % 16) / 4));
		putch(grid[x]);
		gotoxy(3 + ((x + c) / 16) * 21 + 4 * ((x + c) % 4),
				 1 + 2 * (((x + c) % 16) / 4));
		putch(grid[x]);
		textcolor(LIGHTGRAY);
		}
	 return(grid[x]);
	 }
  return(0);
  }

				/**************************************************/
				/* Computer's simplistic AI algorithms            */
				/**************************************************/

compplay()
  {
  int c, c2, x, count;

  gotoxy(35, 21);
  textcolor(LIGHTGRAY + BLINK);
  if ( move > 6 )
	 cprintf("Thinking...");
  textcolor(LIGHTGRAY);

																			/* INSTANT WIN ? */

  if ( move > 5 )
	 {
	 for ( c = 0; c < 64; c++ )
		{
		if ( grid[c] != EMPTY ) continue;
		grid[c] = MySymbol;
		if ( iswin() )
		  {
		  compmove(c);
		  return;
		  }
		grid[c] = EMPTY;
		}

																	  /* BLOCK INSTANT WIN */
	 for ( c = 0; c < 64; c++ )
		{
		if ( grid[c] != EMPTY ) continue;
		grid[c] = TheirSymbol;
		if ( iswin() )
		  {
		  compmove(c);
		  return;
		  }
		grid[c] = EMPTY;
		}

																		  /* POSSIBLE FORK ? */
	 if ( move > 6 )
		for ( c = 0; c < 64; c++ )
		  {
		  count = 0;
		  if ( grid[c] != EMPTY ) continue;
		  grid[c] = MySymbol;
		  for ( x = 1; x < 64; x++ )
			 {
			 if ( grid[x] != EMPTY ) continue;
			 grid[x] = MySymbol;
			 if ( iswin() ) count++;
			 grid[x] = EMPTY;
			 if ( count > 1 )
				{
				compmove(c);
				return;
				}
			 }
		  grid[c] = EMPTY;
		  }

																			/* TWO MOVE WIN */
	 if ( move > 6 && random(100) > 50 )
		for ( c = 0; c < 63; c++ )
		  {
		  if ( grid[c] != EMPTY ) continue;
		  grid[c] = MySymbol;
		  for ( x = c + 1; x < 64; x++ )
			 {
			 if ( grid[x] != EMPTY ) continue;
			 grid[x] = MySymbol;
			 if ( iswin() )
				{
				grid[x] = EMPTY;
				compmove(c);
				return;
				}
			 grid[x] = EMPTY;
			 }
		  grid[c] = EMPTY;
		  }
	 else
		if ( move > 6 )
		  for ( c = 63; c > 0; c-- )
			 {
			 if ( grid[c] != EMPTY ) continue;
			 grid[c] = MySymbol;
			 for ( x = c - 1; x >= 0; x-- )
				{
				if ( grid[x] != EMPTY ) continue;
				grid[x] = MySymbol;
				if ( iswin() )
				  {
				  grid[x] = EMPTY;
				  compmove(c);
				  return;
				  }
				grid[x] = EMPTY;
				}
			 grid[c] = EMPTY;
			 }

																	 /* BLOCK POSSIBLE FORK */
	 if ( move > 6 )
		for ( c = 0; c < 64; c++ )
		  {
		  count = 0;
		  if ( grid[c] != EMPTY ) continue;
		  grid[c] = TheirSymbol;
		  for ( x = 1; x < 64; x++ )
			 {
			 if ( grid[x] != EMPTY ) continue;
			 grid[x] = TheirSymbol;
			 if ( iswin() )
				count++;
			 grid[x] = EMPTY;
			 if ( count > 1 )
				{
				compmove(c);
				return;
				}
			 }
		  grid[c] = EMPTY;
		  }

																	/* BLOCK TWO MOVE WIN */
  if ( move > 6 && ( x = random(100)) > 50 )
		for ( c = 0; c < 63; c++ )
		  {
		  if ( grid[c] != EMPTY ) continue;
		  grid[c] = TheirSymbol;
		  for ( x = c + 1; x < 64; x++ )
			 {
			 if ( grid[x] != EMPTY ) continue;
			 grid[x] = TheirSymbol;
			 if ( iswin() )
				{
				grid[x] = EMPTY;
				compmove(c);
				return;
				}
			 grid[x] = EMPTY;
			 }
		  grid[c] = EMPTY;
		  }
	 else
		if ( move > 6 )
		  for ( c = 63; c > 0; c-- )
			 {
			 if ( grid[c] != EMPTY ) continue;
			 grid[c] = TheirSymbol;
			 for ( x = c - 1; x >= 0; x-- )
				{
				if ( grid[x] != EMPTY ) continue;
				grid[x] = TheirSymbol;
				if ( iswin() )
				  {
				  grid[x] = EMPTY;
				  compmove(c);
				  return;
				  }
				grid[x] = EMPTY;
				}
			 grid[c] = EMPTY;
			 }
	 }  /* big IF */


	/* GRAB CORNERS ? */

	 if ( random(100) > 50 )
		{
		if ( grid[48] == EMPTY )
		  {
		  compmove(48);
		  return;
		  }
		if ( grid[51] == EMPTY )
		  {
		  compmove(51);
		  return;
		  }
		if ( grid[60] == EMPTY )
		  {
		  compmove(60);
		  return;
		  }
		if ( grid[63] == EMPTY )
		  {
		  compmove(63);
		  return;
		  }

		for ( c = 0; c < 3; c++ )
		  {
		  if ( grid[c * 16] == EMPTY )
			 {
			 compmove(c * 16);
			 return;
			 }
		  if ( grid[c * 16 + 3] == EMPTY )
			 {
			 compmove(c * 16 + 3);
			 return;
			 }
		  if ( grid[c * 16 + 12] == EMPTY )
			 {
			 compmove(c * 16 + 12);
			 return;
			 }
		  if ( grid[c * 16 + 15] == EMPTY )
			 {
			 compmove(c * 16 + 15);
			 return;
			 }
		  }
		}

	/* RANDOM PLACING - LAST RESORT */

  randomize();
  for(c = random(64); grid[c] != EMPTY; c = random(64));
  compmove(c);
  }

				/**************************************************/
				/* Display entire grid (bug fix no longer needed) */
				/**************************************************/

dispgrid()
  {
  int pos;

  for (pos = 0; pos < 64; pos++)
	 if ( grid[pos] != EMPTY )
		{
		gotoxy(3 + (pos / 16) * 21 + 4 * (pos % 4), 1 + 2 * ((pos % 16) / 4));
		if ( grid[pos] == 'X' )
		  textcolor(CYAN);
		else
		  textcolor(LIGHTRED);
		putch(grid[pos]);
		}
  textcolor(LIGHTGRAY);
  flashwin = ON;
  }

				/**************************************************/
				/* Update list of last five moves                 */
				/**************************************************/

listmove( int pos )
  {
  char lastmove[20];

  movetext(2, 20, 13, 24, 2, 19);
  gotoxy(2, 23);
  sprintf(lastmove, "%02d: [%c]: %c,%c", move, grid[pos],
			 pos / 16 + 'A', pos % 16 + 'A');
  displine(lastmove);
  }

				/**************************************************/
				/* Display computer's move                        */
				/**************************************************/

compmove( int pos )
  {
  grid[pos] = (MySymbol);
  gotoxy(26, 23);
  cprintf("Player %c (Computer) moves ", MySymbol);
  printf("%c,%c", pos / 16 + 'A', pos % 16 + 'A');
  clreol();
  listmove(pos);
  dispgrid();
  move++;
  gotoxy(15, 21);
  clreol();
  if ( iswin() )
	 {
	 gotoxy(15, 23);
	 clreol();
	 gotoxy(28, 23);
	 printf("Player %c wins (Computer)", TheirSymbol);
	 gameover();
	 }
  flashwin = OFF;
  if ( player != CC )
	 while( !kbhit() );
  }


				/**************************************************/
				/* Display end title and exit to OS               */
				/**************************************************/

endall()
  {
  int x;
  char email[] = "ssusncla@rdg.ac.uk";

  textcolor(LIGHTGRAY);
  clrscr();
  oneline("ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»");
  oneline("º                                                                             º");
  logo();
  oneline("º   °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°° º");
  oneline("º                                                                             º");
  oneline("ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼");

  printf("\nHope you enjoyed the game... Ben (");
  for (x = 0; x < strlen(email); x++)
	 {
	 textcolor(x % 8 + 2);
	 putch(email[x]);
	 }
  printf(")\n");
  cursor(ON);
  exit(0);
  }

				/**************************************************/
				/* Colourising subproc for tits and endall        */
				/**************************************************/

oneline(char *str)
  {
  int x;

  if ( wherex() > 1 )
	 printf("\n");
  for (x = 0; x < 79; x++)
	 {
	 if (str[x] == 'Û')
		textcolor(BROWN);
	 else if (str[x] == '°')
		textcolor(MAGENTA);
	 else
		textcolor(CYAN);
	 putch(str[x]);
	 }
  }

				/**************************************************/
				/* Display opening title screen                   */
				/**************************************************/

logo()
  {
  oneline("º ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ   º");
  oneline("º   °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°° º");
  oneline("º                                                                             º");
  oneline("º  ÛÛÛÛÛÛÛÛÛÛÛÛ     ÛÛÛÛÛÛÛ             ÛÛÛÛÛ   ÛÛÛÛ       ÛÛÛÛ   ÛÛÛÛÛ       º");
  oneline("º    °°°°ÛÛÛÛ°°°°    ÛÛ°°°ÛÛÛÛ        ÛÛÛ °°ÛÛÛ   ÛÛÛ°    ÛÛÛ°°°ÛÛÛ °°ÛÛÛ     º");
  oneline("º       ÛÛÛ°°°°      ÛÛ°°   °°ÛÛÛ    ÛÛ °°°   ÛÛ°  ÛÛÛ°  ÛÛÛ°°°ÛÛ °°°   ÛÛ°   º");
  oneline("º     ÛÛÛÛ°°°        ÛÛ°°       ÛÛ° ÛÛ °°      ÛÛ°  ÛÛÛ°ÛÛÛ°°°ÛÛ °°      ÛÛ°  º");
  oneline("º       °ÛÛÛÛÛ  ÛÛÛ  ÛÛ°°       ÛÛ°°ÛÛ°°       ÛÛ°°  ÛÛÛÛÛ°°° ÛÛ°°       ÛÛ°° º");
  oneline("º  ÛÛ      °°ÛÛ°  °°°ÛÛ°°       ÛÛ°°ÛÛ°°       ÛÛ°° ÛÛÛ°ÛÛÛ°  ÛÛ°°       ÛÛ°° º");
  oneline("º ÛÛ °°      ÛÛ°°    ÛÛ°°     ÛÛÛ °° ÛÛ°      ÛÛ °°ÛÛÛ°°°ÛÛÛ°  ÛÛ°      ÛÛ °° º");
  oneline("º  ÛÛÛ      ÛÛ °°    ÛÛ°° ÛÛÛÛ  °°°   ÛÛÛ   ÛÛÛ °°ÛÛÛ°°°  ÛÛÛ°  ÛÛÛ   ÛÛÛ °°  º");
  oneline("º    ÛÛÛÛÛÛÛÛ °°    ÛÛÛÛÛÛÛ °°°°        ÛÛÛÛÛ °°ÛÛÛÛ°°°    ÛÛÛÛ   ÛÛÛÛÛ °°°   º");
  oneline("º      °°°°°°°°       °°°°°°°             °°°°°   °°°°       °°°°   °°°°°     º");
  oneline("º                                                                             º");
  oneline("º ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ   º");
  }


tits()
  {
  clrscr();
  cursor(OFF);
  oneline("ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»");
  oneline("º               ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ           ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ           º");
  oneline("º               ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ°°         ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ°°         º");
  oneline("º                 °°°°ÛÛÛÛÛÛÛÛ°°°°°°°°           °°°°ÛÛÛÛÛÛÛÛ°°°°°°°°         º");
  logo();
  oneline("º   °°°°°°°°°°°°°°°°°°ÛÛÛÛÛÛÛÛ°°°°°°°°°°°°°°°°°°°°°°°ÛÛÛÛÛÛÛÛ°°°°°°°°°°°°°°°° º");
  oneline("º                     ÛÛÛÛÛÛÛÛ°°                     ÛÛÛÛÛÛÛÛ°°               º");
  oneline("º               ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ           ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ           º");
  oneline("º               ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ°°         ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ°°         º");
  oneline("º                 °°°°°°°°°°°°°°°°°°°°           °°°°°°°°°°°°°°°°°°°°         º");
  oneline("ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼");
  flushkeybuf();
  while( !kbhit() );
  clrscr();
  }

				/**************************************************/
				/* Make an annoying/amusing/informative beep      */
				/**************************************************/

beep()
  {
  int x;
  for (x = 500; x > 0; sound(x * x--)) delay(2 + (x < 100));
  nosound();
  }

				/**************************************************/
				/* Turn the cursor display on (1), and off (0)    */
				/**************************************************/

cursor( int s )
  {
  struct { unsigned ax, bx, cx, dx, si, di, cflag, flags; } TREGS;

  TREGS.ax = 0x0100;
  TREGS.bx = 0x0000;
  TREGS.cx = 0x2607;

  if ( s )
	 TREGS.cx = 0x0607;

  int86(0x0010, &TREGS, &TREGS);
  }
