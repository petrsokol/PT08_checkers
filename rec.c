#include <stdlib.h>
#include <string.h>
#include <stdio.h>

constexpr char QUEEN_W = 'W';
constexpr char PIECE_W = 'w';
constexpr char QUEEN_B = 'B';
constexpr char PIECE_B = 'b';
constexpr char BLANK   = ' ';

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
  return (p -> type == QUEEN_W || p -> type == QUEEN_B);
}

bool isWhite (PIECE * p) 
{
  return (p -> type == QUEEN_W || p -> type == PIECE_W);
}

/*--------------------------------------------------------------*/

typedef struct
{
  int size;
  char ** data;
} BOARD;

int initBoard (BOARD * board, const int boardSize_in)
{
  board -> size = boardSize_in;

  // allocate memory
  board -> data = (char **) malloc(board -> size * sizeof(*(board -> data)));
  for (int j = 0; j < board -> size; ++j)
    board -> data[j] = (char *) malloc(board -> size * sizeof(*(board -> data[j])));

  // start with empty board
  for (int j = 0; j < board -> size; ++j)
    for (int i = 0; i < board -> size; ++i)
      board -> data[j][i] = BLANK;

  return EXIT_SUCCESS;
}

void printBoard (const BOARD * board)
{
  for (int j = 0; j < board -> size; ++j) {
    for (int i = 0; i < board -> size; ++i) {
      printf("[%c]", board -> data[j][i]);
    }
    printf("\n");
  }
}

void freeBoard (BOARD * board)
{
  for (int j = 0; j < board -> size; ++j)
    free(board -> data[j]);
  free (board -> data);
}

bool isEmpty (const BOARD * board, const int x, const int y)
{
  return board -> data[y][x] == BLANK;
}

bool hasWhite (const BOARD * board, const int x, const int y)
{
  return (board -> data[y][x] == PIECE_W || board -> data[y][x] == QUEEN_W);
}

void posToString (const int x, const int y)
{
  printf("%c%c", 'a' + x, '0' + y);
}

bool getNext (const BOARD * board, int * x, int * y)
{
  while (true)
  {
    // move to next available square
    *y = *y + ++(*x) / board -> size;
    *x = *x % board -> size;

    // return false if there are no more white pieces
    if (*x >= board -> size || *y >= board -> size)
    {
      printf("getNext: No other pieces were found.\n");
      return false;
    }

    // return true if [x, y] corresponds to a white piece
    if (hasWhite(board, *x, *y))
    {
      printf("getNext: Found a white piece at [%d, %d].\n", *x, *y);
      return true;
    }
  }
}

/*--------------------------------------------------------------*/

int readBoardSize (int * size)
{
  printf("Velikost hraci plochy:\n");
  constexpr int MAX_BOARD_SIZE = 26;
  constexpr int MIN_BOARD_SIZE = 3;
  if (scanf(" %d", size) != 1
      || * size < MIN_BOARD_SIZE
      || * size > MAX_BOARD_SIZE)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

/*--------------------------------------------------------------*/

bool isValidType (const char type)
{
  return (type == PIECE_W
       || type == PIECE_B
       || type == QUEEN_W
       || type == QUEEN_B);
}

bool isValid (const BOARD * board, const int x, const int y)
{
  return (x >= 0 && x < board -> size
       && y >= 0 && y < board -> size
       && (x + y) % 2 == 0);
}

/*--------------------------------------------------------------*/

int movePiece (BOARD * board, int * captures, int xFrom, int yFrom, int xTo, int yTo)
{
  // get piece from starting position
  char piece = board -> data[yFrom][xFrom];

  // check for invalid starting position
  if (!isValid(board, xFrom, yFrom) || !isEmpty(board, xFrom, yFrom))
  {
    printf("movePiece: ilegal start pos [%d, %d]\n", xFrom, yFrom);
    return EXIT_FAILURE;
  }

  // check for invalid ending position
  if (!isValid(board, xTo, yTo) || !isEmpty(board, xFrom, yFrom))
  {
    printf("movePiece: ilegal end pos [%d, %d]\n", xTo, yTo);
    return EXIT_FAILURE;
  }

  // check for nondiagonal movement
  if (abs(xTo - xFrom) != abs(yTo - yFrom))
  {
    printf("movePiece: non-diagonal movement between [%d, %d] and [%d, %d]\n", 
      xFrom, yFrom, xTo, yTo);
    return EXIT_FAILURE;
  }

  // moving the piece
  board -> data[yFrom][xFrom] = BLANK;
  board -> data[yTo][xTo] = piece;
  return EXIT_SUCCESS;
}

/*--------------------------------------------------------------*/

/**
 * does not catch wa1 as a wrong input.
 * does not catch w a 1 as a wrong input.
 */
int loadPiece (PIECE * p, BOARD * board)
{
  // read input
  int y_in;
  char type_in, x_in;
  constexpr int EXPECTED_INPUTS = 3;
  int result = scanf(" %c %c%d", &type_in, &x_in, &y_in);

  // adjust inputs for a1 to be at [0, 0]
  int xPos_in = x_in - 'a';
  int yPos_in = y_in - 1;

  // check for EOF
  if (result == EOF)
    return EXIT_SUCCESS;

  // check for correct input format
  if (result != EXPECTED_INPUTS)
  {
    printf("loadPiece: wrong input.\n");
    return EXIT_FAILURE;
  }

  // check for correct type of piece
  if (!isValidType(type_in))
  {
    printf("loadPiece: invalid piece.\n");
    return EXIT_FAILURE;
  }

  // check if piece is on the board
  if (!isValid(board, xPos_in, yPos_in))
  {
    printf("loadPiece: invalid position.\n");
    return EXIT_FAILURE;
  }

  // check if position is empty
  if (!isEmpty(board, xPos_in, yPos_in))
  {
    printf("loadPiece: position not empty.\n");
    return EXIT_FAILURE;
  }

  // edit the piece
  p -> type = type_in;
  p -> xPos = xPos_in;
  p -> yPos = yPos_in;

  // place the piece on board for future reference
  board -> data[yPos_in][xPos_in] = p -> type;

  return EXIT_SUCCESS;
}

int loadPieces(BOARD * board)
{
  printf("Pozice kamenu:\n");
  while (!feof(stdin))
  {
    PIECE p;
    if (loadPiece(&p, board))
      return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

/*--------------------------------------------------------------*/

/*
program to load checker pieces into an array to be used for game logic later
*/
int main () {
  int size;
  if (readBoardSize(&size))
    return errorIO();

  BOARD board;
  initBoard(&board, size);
  if (loadPieces(&board))
    return errorIO();

  printBoard(&board);

  int x = -1;
  int y = 0;
  int sum = 0;
  while (getNext(&board, &x, &y))
  {
    printf("> main: ready to operate with [%d, %d]\n", x, y);
    sum++;
  }
  printf ("> main: in total found %d white pieces.\n", sum);
  freeBoard(&board);
  return EXIT_SUCCESS;
}

/*
kameny budou zvlášť ve vektoru a zvlášť na šachovnici, musíš se vypořádat s duplikátama
 - funkce bool captureAt(int, int); se musí vypořádat s oběma zároveň atd.
*/
