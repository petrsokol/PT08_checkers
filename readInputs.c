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

bool isEmpty (const BOARD * board, int x, int y)
{
  return board -> data[y][x] == BLANK;
}

/*--------------------------------------------------------------*/

int readBoardSize (int * size)
{
  printf("Velikost hraci plochy:\n");
  if (scanf(" %d", size) != 1
      || * size < MIN_BOARD_SIZE
      || * size > MAX_BOARD_SIZE)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

/*--------------------------------------------------------------*/

bool isValidType (char type)
{
  return (type == PIECE_W
       || type == QUEEN_W
       || type == PIECE_B
       || type == QUEEN_B);
}

bool isValid (const BOARD * board, char x_in, int y_in)
{
  constexpr char X_SHIFT = 'a';
  int xPos = x_in - X_SHIFT;
  int yPos = y_in - 1; // indexing from zero
  return (xPos >= 0 && xPos < board -> size
       && yPos >= 0 && xPos < board -> size
       && (xPos + yPos) % 2 == 0);
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
    return EXIT_FAILURE;

  // check for correct type of piece
  if (!isValidType(type_in))
    return EXIT_FAILURE;

  // check if piece is on the board
  if (!isValid(board, x_in, y_in))
    return EXIT_FAILURE;

  // check if position is empty
  if (board -> data[yPos_in][xPos_in] != BLANK)
    return EXIT_FAILURE;

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
  freeBoard(&board);
  return EXIT_SUCCESS;
}

/*
kameny budou zvlášť ve vektoru a zvlášť na šachovnici, musíš se vypořádat s duplikátama
 - funkce bool captureAt(int, int); se musí vypořádat s oběma zároveň atd.
*/
