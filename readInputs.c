#include <stdlib.h>
#include <string.h>
#include <stdio.h>

constexpr int MAX_BOARD_SIZE = 26; 
constexpr int MIN_BOARD_SIZE = 3;

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

/*--------------------------------------------------------------*/

typedef struct
{
  int boardSize;
  char ** data;
} BOARD;

int initBoard (BOARD * board, const int boardSize_in)
{
  board -> boardSize = boardSize_in;

  // allocate memory
  board -> data = (char **) malloc(board -> boardSize * sizeof(*(board -> data)));
  for (int j = 0; j < board -> boardSize; ++j)
    board -> data[j] = (char *) malloc(board -> boardSize * sizeof(*(board -> data[j])));

  // start with empty board
  for (int j = 0; j < board -> boardSize; ++j)
    for (int i = 0; i < board -> boardSize; ++i)
      board -> data[j][i] = ' ';

  return EXIT_SUCCESS;
}

void printBoard (const BOARD * board)
{
  for (int j = 0; j < board -> boardSize; ++j) {
    for (int i = 0; i < board -> boardSize; ++i) {
      printf("[%c]", board -> data[j][i]);
    }
    printf("\n");
  }
}

void freeBoard (BOARD * board)
{
  for (int j = 0; j < board -> boardSize; ++j)
    free(board -> data[j]);
  free (board -> data);
}

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

/*--------------------------------------------------------------*/

bool isValidType (char type)
{
  return (type == 'w'
       || type == 'W'
       || type == 'b'
       || type == 'B');
}

bool isValid (char x_in, int y_in, int boardSize)
{
  constexpr char X_SHIFT = 'a';
  int xPos = x_in - X_SHIFT;
  int yPos = y_in - 1; // indexing from zero
  return (xPos >= 0 && xPos < boardSize
       && yPos >= 0 && xPos < boardSize
       && (xPos + yPos) % 2 == 0);
}

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
  if (!isValid(x_in, y_in, board -> boardSize))
    return EXIT_FAILURE;

  // check if position is empty
  if (board -> data[yPos_in][xPos_in] != ' ')
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
  int boardSize;
  if (readBoardSize(&boardSize))
    return errorIO();

  BOARD board;
  initBoard(&board, boardSize);
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
