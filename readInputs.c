#include <stdlib.h>
#include <string.h>
#include <stdio.h>

constexpr int MAX_BOARD_SIZE = 26; 
constexpr int MIN_BOARD_SIZE = 3;
constexpr int MAX_PIECE_COUNT = MAX_BOARD_SIZE * 2;

int errorIO()
{
  printf("Nespravny vstup.\n");
  return EXIT_FAILURE;
}

/*--------------------------------------------------------------*/

typedef struct 
{
  char type;
  int xPos;
  int yPos;
} PIECE;

void pieceToString (PIECE * p)
{
  printf("%c at [%d, %d]\n", p -> type, p -> xPos, p -> yPos);
}

bool isQueen (PIECE * p)
{
  return (p -> type == 'W' || p -> type == 'B');
}

bool isWhite (PIECE * p) 
{
  return (p -> type == 'W' || p -> type == 'w');
}

typedef struct 
{
  int cols;
  int rows;
  PIECE data[26][26]; // [x, y]
} BOARD;

typedef struct 
{
  BOARD board;
  int piecesRemaining;
  PIECE pieces[100];
  
} GAME_STATE;

/*--------------------------------------------------------------*/

int readBoardSize (int * boardSize)
{
  printf("Velikost hraci plochy:\n");
  if (scanf(" %d", boardSize) != 1
      || * boardSize < MIN_BOARD_SIZE
      || * boardSize > MAX_BOARD_SIZE)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

bool isValidType (char type)
{
  return (type == 'w'
       || type == 'W'
       || type == 'b'
       || type == 'B');
}

bool isOnValidSquare (char x_in, int y_in, int boardSize)
{
  constexpr char X_SHIFT = 'a';
  int xPos = x_in - X_SHIFT;
  int yPos = y_in - 1; // indexing from zero
  return (xPos >= 0 && xPos < boardSize
       && yPos >= 0 && xPos < boardSize
       && (xPos + yPos) % 2 == 0);
}

int loadPiece (PIECE * p, const int boardSize, char board[][MAX_BOARD_SIZE])
{
  constexpr int EXPECTED_INPUTS = 3;

  int y_in;
  char type_in, x_in;
  int result = scanf(" %c %c%d", &type_in, &x_in, &y_in);
  int xPos_in = x_in - 'a';
  int yPos_in = y_in - 1;

  // check for EOF
  if (result == EOF)
    return EXIT_SUCCESS;

  // check for correct input format
  if (result != EXPECTED_INPUTS)
    return EXIT_FAILURE;

  // check for correct type of piece
  if (!isValidType(type_in))
    return EXIT_FAILURE;

  // check if piece is on the board
  if (!isOnValidSquare(x_in, y_in, boardSize))
    return EXIT_FAILURE;

  // check if position is empty
  if (board[xPos_in][yPos_in] != ' ')
    return EXIT_FAILURE;

  p -> type = type_in;
  p -> xPos = xPos_in;
  p -> yPos = yPos_in;

  board[xPos_in][yPos_in] = p -> type;

  return EXIT_SUCCESS;
}

void initBoard (char board[][MAX_BOARD_SIZE])
{
  for (int j = 0; j < MAX_BOARD_SIZE; ++j)
    for (int i = 0; i < MAX_BOARD_SIZE; ++i)
      board[i][j] = ' ';
}

void printBoard (const char board[][MAX_BOARD_SIZE], const int boardSize)
{
  for (int j = 0; j < boardSize; ++j) {
    for (int i = 0; i < boardSize; ++i)
    {
      printf("[%c]", board[i][j]);
    }
    printf("\n");
  }
}

/*--------------------------------------------------------------*/

/*
program to load checker pieces into an array to be used for game logic later
*/
int main () {
  int boardSize;
  if (readBoardSize(&boardSize))
    return errorIO();

  int max_piece_count_regarding_the_actual_size_of_the_board;
  if (boardSize == 3)
    max_piece_count_regarding_the_actual_size_of_the_board = 5;
  else
    max_piece_count_regarding_the_actual_size_of_the_board = 2 * boardSize;

  char board[MAX_BOARD_SIZE][MAX_BOARD_SIZE];
  initBoard(board);

  int n = 0;
  PIECE pieces[MAX_BOARD_SIZE * MAX_BOARD_SIZE]; // lepší pomocí append
  printf("Pozice kamenu:\n");
  while (!feof(stdin))
  {
    PIECE p;
    if (loadPiece(&p, boardSize, board))
      return errorIO();

    pieces[n++] = p;
  }
  printBoard(board, boardSize);
  return EXIT_SUCCESS;
}

/*
kameny budou zvlášť ve vektoru a zvlášť na šachovnici, musíš se vypořádat s duplikátama
 - funkce bool captureAt(int, int); se musí vypořádat s oběma zároveň atd.
*/