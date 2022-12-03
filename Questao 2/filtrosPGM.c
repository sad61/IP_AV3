#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define MAX_LINE 8192
#define MAX_HEIGHT 512
#define MAX_WIDTH 512
#define BRIGHTEN 25

typedef struct
{
  int lines;
  int cols;
  int highest;
  int matrix[MAX_WIDTH][MAX_HEIGHT];
} Pgm;

void getInfo(Pgm *pgm);
void brighten(int matrix[MAX_WIDTH][MAX_HEIGHT], Pgm *pgm);
void zoom(int matrix[MAX_WIDTH][MAX_HEIGHT], Pgm *pgm);
void polarize(int matrix[MAX_WIDTH][MAX_HEIGHT], Pgm *pgm);
void rotate(int matrix[MAX_WIDTH][MAX_HEIGHT], Pgm *pgm);

int main(int argc, char **argv) // Por simplicidade usarei o .pgm usado como exemplo.
{
  Pgm pgm;
  getInfo(&pgm);
  brighten(pgm.matrix, &pgm);
  rotate(pgm.matrix, &pgm);
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
  /*while ((c = fgetc(f)) != EOF)
  {
    if (c == ' ')
      countSpaces++;
    if (countSpaces >= 512)
      break;
    printf("%c", c);
  }*/
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
  f = fopen("glassware_noisy.pgm", "r");
  keepReading = true;
  line = 0;
  do
  {
    fgets(buffer, MAX_LINE, f);
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
        printf("values: %d, cols: %d, pgm->matrix[%d][%d], total: %d\n", atoi(values), pgm->cols, i, countSpaces, total);
        pgm->matrix[i][countSpaces] = atoi(values);
        if (i == pgm->lines && countSpaces == pgm->cols)
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
  for (int i = 0; i < pgm->cols - 1; i++)
  {
    for (int j = 0; j < pgm->lines - 1; j++)
      fprintf(f, "%d ", pgm->matrix[i][j]);

    fprintf(f, "\n");
  }
  fclose(f);
}

void brighten(int matrix[MAX_WIDTH][MAX_HEIGHT], Pgm *pgm)
{
  for (int i = 0; i < pgm->lines - 1; i++)
    for (int j = 0; j < pgm->cols - 1; j++)
    {
      if (matrix[i][j] + BRIGHTEN > 255)
      {
        matrix[i][j] = matrix[i][j] + (255 - matrix[i][j]);
      }
      else
        matrix[i][j] = matrix[i][j] + BRIGHTEN;
    }
  FILE *f = fopen("glassware_noisy_bright.pgm", "wb");
  fprintf(f, "P2\n");
  fprintf(f, "%d %d\n", pgm->cols, pgm->lines);
  fprintf(f, "%d\n", pgm->highest);
  for (int i = 0; i < pgm->lines - 1; i++)
  {
    for (int j = 0; j < pgm->cols - 1; j++)
      fprintf(f, "%d ", matrix[i][j]);
    fprintf(f, "\n");
  }
}

void zoom(int matrix[MAX_WIDTH][MAX_HEIGHT], Pgm *pgm)
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

void polarize(int matrix[MAX_WIDTH][MAX_HEIGHT], Pgm *pgm)
{
}

void rotate(int matrix[MAX_WIDTH][MAX_HEIGHT], Pgm *pgm)
{
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
