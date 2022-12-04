#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MAX_LINE 8192
#define MAX_HEIGHT 512
#define MAX_WIDTH 512
#define BRIGHTEN 50

typedef struct
{
  int lines;
  int cols;
  int highest;
  int **matrix;
} Pgm;

void getInfo(Pgm *pgm);
void build(Pgm *pgm);
void brighten(int **matrix, Pgm *pgm);
void zoom(int **matrix, Pgm *pgm);
void polarize(int **matrix, Pgm *pgm);
void rotate(int **matrix, Pgm *pgm);

int main(int argc, char **argv) // Por simplicidade usarei o .pgm usado como exemplo.
{
  Pgm pgm;
  getInfo(&pgm); // Vai pegar as dimensions da matrix e o maior valor.

  pgm.matrix = (int **)malloc(pgm.lines * sizeof(int *));
  for (int i = 0; i < pgm.lines; i++)
    pgm.matrix[i] = (int *)malloc(pgm.cols * sizeof(int));

  build(&pgm);                // Construo a matriz
  brighten(pgm.matrix, &pgm); // Clareia
  rotate(pgm.matrix, &pgm);   // Roda
  // zoom(pgm.matrix, &pgm);
  //  printMatrix();
  return 0;
}

void getInfo(Pgm *pgm)
{
  char buffer[512];
  int currentLine = 1, line, counter = 0, total = 0, i = 0, countSpaces = 0;
  bool d = true;
  bool keepReading = true;
  FILE *f = fopen("glassware_noisy.pgm", "r");
  do
  {
    if (counter == 0)
      line = 2;
    else
      line = 3;
    counter++;
    do
    {
      fgets(buffer, MAX_LINE, f);
      if (feof(f))
      {
        keepReading = false;
        break;
      }
      if (currentLine == line)
      {
        if (line == 2)
        {
          keepReading = false;
          char *dimensions = strtok(buffer, " ");
          while (dimensions != NULL)
          { // A linha possui 2 valores (cols, lines), leio esses valores e salvo no struct.
            if (d == true)
              pgm->cols = atoi(dimensions);
            else
              pgm->lines = atoi(dimensions);
            d = false;
            dimensions = strtok(NULL, " ");
          }
        }
        else if (line == 3)
        {
          pgm->highest = atoi(buffer);
          keepReading = false;
        }
      }
      currentLine++;
    } while (keepReading);
  } while (counter < 2);
  fclose(f);
  printf("cols: %d, line: %d, highest: %d", pgm->cols, pgm->lines, pgm->highest);

  // pegando os valores da matriz do arquivo e salvando no struct;
}

void build(Pgm *pgm)
{
  char buffer[512];
  FILE *f = fopen("glassware_noisy.pgm", "r");
  bool keepReading = true;
  int line = 0, total = 0, countSpaces = 0, i = 0;

  do
  {
    fgets(buffer, MAX_LINE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    if (feof(f))
    {
      printf("\nCheguei no final do arquivo");
      keepReading = false;
      break;
    }
    if (line >= 3)
    {
      char *values = strtok(buffer, " ");
      while (values != NULL)
      {
        total++;
        // printf("values: %d, cols: %d, pgm->matrix[%d][%d], total: %d\n", atoi(values), pgm->cols, i, countSpaces, total);
        pgm->matrix[i][countSpaces] = atoi(values);
        if (i == pgm->lines - 1 && countSpaces == pgm->cols - 1)
          break;
        countSpaces++;
        if (countSpaces == pgm->cols)
        {
          countSpaces = 0;
          i++;
        }
        values = strtok(NULL, " ");
      }
    }
    line++;
  } while (keepReading);
  fclose(f);

  // Salvando as informações do struct em um arquivo pgm temporario.
  printf("\ncols: %d, lines: %d, highest: %d, matrix[0][0]: %d", pgm->cols, pgm->lines, pgm->highest, pgm->matrix[0][0]);
  f = fopen("glassware_noisyTemp.pgm", "wb");
  fprintf(f, "P2\n");
  fprintf(f, "%d %d\n", pgm->cols, pgm->lines);
  fprintf(f, "%d\n", pgm->highest);
  for (int i = 0; i < pgm->lines; i++)
  {
    for (int j = 0; j < pgm->cols; j++)
    {
      // printf("\nmatrix[%d][%d]: %3d", i, j, pgm->matrix[27][90]);
      fprintf(f, "%d  ", pgm->matrix[i][j]);
    }
    fprintf(f, "\n");
  }
  fclose(f);
}

void brighten(int **matrix, Pgm *pgm)
{
  int bright[pgm->lines][pgm->cols];
  for (int i = 0; i < pgm->lines; i++)
    for (int j = 0; j < pgm->cols; j++)
    {
      // printf("\n Brighten matrix[%d][%d]: %d", i, j, matrix[i][j]);
      if (matrix[i][j] + BRIGHTEN > 255)
      {
        bright[i][j] = matrix[i][j] + (255 - matrix[i][j]);
      }
      else
        bright[i][j] = matrix[i][j] + BRIGHTEN;
    }
  FILE *f = fopen("glassware_noisy_bright.pgm", "wb");
  fprintf(f, "P2\n");
  fprintf(f, "%d %d\n", pgm->cols, pgm->lines);
  fprintf(f, "%d\n", (pgm->highest + BRIGHTEN < 255) ? pgm->highest + BRIGHTEN : 255);
  for (int i = 0; i < pgm->lines; i++)
  {
    for (int j = 0; j < pgm->cols; j++)
      fprintf(f, "%d ", bright[i][j]);
    fprintf(f, "\n");
  }
}

void zoom(int **matrix, Pgm *pgm)
{
  int cols = pgm->cols / 2, lines = pgm->lines / 2;
  int c = 0, l = 0;
  int img[4][4];
  int image[8][8] = {12, 12, 9, 43, 76, 23, 87, 12,
                     12, 11, 9, 23, 76, 43, 87, 12,
                     32, 13, 9, 99, 76, 12, 87, 12,
                     22, 12, 9, 234, 76, 23, 87, 12,
                     42, 9, 9, 65, 76, 12, 87, 12,
                     12, 8, 9, 43, 14, 23, 87, 76,
                     1, 45, 9, 67, 76, 34, 87, 86,
                     2, 87, 9, 33, 42, 23, 87, 12};
  for (int i = 0; i < 8; i += 2)
  {
    for (int j = 0; j < 8; j += 2)
    {
      img[l][c] =
          c++;
    }
    l++;
  }
}

void polarize(int **matrix, Pgm *pgm)
{
}

void rotate(int **matrix, Pgm *pgm)
{
  printf("\nSalve rodado");
  int rotatedTemp[pgm->lines][pgm->cols];
  int rotated[pgm->cols][pgm->lines];
  int i = 0, j = 0;

  for (int i = 0; i < pgm->lines; i++)
    for (int j = 0; j < pgm->cols; j++)
    {
      rotatedTemp[i][j] = matrix[i][j];
    }
  for (i = 0; i < pgm->lines; i++)
  {
    for (j = 0; j < pgm->cols; j++)
    {
      rotated[j][pgm->lines - i - 1] = rotatedTemp[i][j];
    }
  }
  // Print the matrix
  // Tranposing the matrix*
  /*for (int i = 0; i < pgm->lines; i++)
    for (int j = i; j < pgm->cols; j++)
      swap(&rotatedTemp[i][j], &rotatedTemp[j][i]);

  // Reversing each row of the matrix
  for (int i = 0; i < pgm->lines; i++)
    for (int j = 0; j < pgm->cols / 2; j++)
      swap(&rotatedTemp[i][j], &rotatedTemp[i][pgm->cols - j - 1]);

  for (int i = 0; i < pgm->cols; i++)
    for (int j = 0; j < pgm->lines; j++)
      rotated[i][j] = rotatedTemp[i][j];

  // Print the matrix*/

  FILE *f = fopen("glassware_noisy_rotate.pgm", "wb");
  fprintf(f, "P2\n");
  fprintf(f, "%d %d\n", pgm->lines, pgm->cols);
  fprintf(f, "%d\n", pgm->highest);
  // Print the matrix
  for (i = 0; i < pgm->cols; i++) // MUDAR LINHAS E COLUNAS OBTEM RESULTADO DIFERENTE
  {
    for (j = 0; j < pgm->lines; j++)
      fprintf(f, "%d ", rotated[i][j]);
    fprintf(f, "\n");
  }
  /*for (int i = 0; i < pgm->cols; i++)
  {
    for (int j = pgm->lines - 1; j >= pgm->cols; j--)
      swap(&matrix[i][j], &matrix[j][i]);
  }

  for (int i = 0; i < pgm->lines; i++)
  {
    for (int j = 0; j < pgm->cols / 2; j++)
    {
      swap(&matrix[i][j], &matrix[i][pgm->cols - j - 1]);
    }
  }

  printf("Rotated Matrix :\n");
  for (int i = 0; i < pgm->lines; i++)
  {
    for (int j = 0; j < pgm->cols; j++)
    {
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
  FILE *f = fopen("glassware_noisy_rotate.pgm", "wb");
  fprintf(f, "P2\n");
  fprintf(f, "%d %d\n", pgm->cols, pgm->lines);
  fprintf(f, "%d\n", pgm->highest);
  for (int i = 0; i < pgm->lines - 1; i++)
  {
    for (int j = 0; j < pgm->cols - 1; j++)
      fprintf(f, "%d ", matrix[i][j]);
    fprintf(f, "\n");
  }*/
}

void swap(int *i, int *j)
{
  int temp = *i;
  *i = *j;
  *j = temp;
}
