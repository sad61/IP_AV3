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
  polarize(pgm.matrix, &pgm); // Polariza, seta valores para 0 / 255
  return 0;
}

void getInfo(Pgm *pgm)
{
  char buffer[512];
  int currentLine = 1, line = 2, counter = 0, total = 0, i = 0, countSpaces = 0;
  bool d = true;
  bool keepReading = true;
  FILE *f = fopen("columns.pgm", "r");
  do
  {
    fgets(buffer, MAX_LINE, f);
    if (feof(f))
    {
      keepReading = false;
      break;
    }

    if (strchr(buffer, '#') != NULL) // Ignora linha com comentário
      currentLine--;

    if (currentLine == line)
    {
      if (line == 2)
      {
        line++;
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
  fclose(f);
  printf("cols: %d, line: %d, highest: %d", pgm->cols, pgm->lines, pgm->highest);

  // pegando os valores da matriz do arquivo e salvando no struct;
}

void build(Pgm *pgm)
{
  char buffer[512];
  FILE *f = fopen("columns.pgm", "r");
  bool keepReading = true;
  int line = 0, total = 0, countSpaces = 0, i = 0;

  do
  {
    fgets(buffer, MAX_LINE, f);
    buffer[strcspn(buffer, "\n")] = 0;
    if (strchr(buffer, '#') != NULL)
      line--;
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
  f = fopen("columnsTemp.pgm", "wb");
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
  FILE *f = fopen("columns_bright.pgm", "wb");
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
}

void polarize(int **matrix, Pgm *pgm)
{
  int **polarized;
  int starterC = 0;
  int starterL = 0;
  int sum = 0;
  int average = 0;
  polarized = (int **)malloc(pgm->lines * sizeof(int *));
  for (int i = 0; i < pgm->lines; i++)
    polarized[i] = (int *)malloc(pgm->cols * sizeof(int));

  do
  {
    if (starterL >= pgm->lines || starterC >= pgm->cols)
      break;

    for (int i = starterL; i < starterL + 2; i++)
      for (int j = starterC; j < starterC + 2; j++)
        sum += matrix[i][j]; // vai passar pela matrix[0][0], [0][1], [1][0], [1][1]

    average = (sum / 4 < 128) ? 0 : 255;
    fillPolarize(polarized, starterL, starterC, average); // Vou fazer a mesma coisa que no loop de cima, só q preenchendo a matrix com a média da soma
    sum = 0;
    if (starterC + 2 >= pgm->cols) // Se cheguei no numero max de colunas
    {
      starterC = 0;  // Seto a coluna pra 0
      starterL += 2; // E agora vou scanear 2 linhas pra baixo
    }                // Primeira vez [0][0]...[1][1], Segunda: [2][0]...[3][1]
    else
      starterC += 2; // Sempre quero começar a scanear dnv 2 colunas a frente da anterior ex:
                     // primeira vez: [0][0]...[1][1], segunda vez: [0][2]...[1][3]
  } while (starterL < pgm->lines);
  FILE *f = fopen("columns_polarize.pgm", "wb");
  fprintf(f, "P2\n");
  fprintf(f, "%d %d\n", pgm->cols, pgm->lines);
  fprintf(f, "%d\n", 255);
  for (int i = 0; i < pgm->lines; i++)
  {
    for (int j = 0; j < pgm->cols; j++)
      fprintf(f, "%d ", polarized[i][j]);
    fprintf(f, "\n");
  }
}

void rotate(int **matrix, Pgm *pgm)
{
  printf("\nSalve rodado");

  int rotated[pgm->cols][pgm->lines];
  int i = 0, j = 0;

  for (i = 0; i < pgm->lines; i++)
    for (j = 0; j < pgm->cols; j++)
      rotated[j][pgm->lines - i - 1] = matrix[i][j];

  FILE *f = fopen("columns_rotate.pgm", "wb");
  fprintf(f, "P2\n");
  fprintf(f, "%d %d\n", pgm->lines, pgm->cols);
  fprintf(f, "%d\n", pgm->highest);

  for (i = 0; i < pgm->cols; i++) // MUDAR LINHAS E COLUNAS OBTEM RESULTADO DIFERENTE
  {
    for (j = 0; j < pgm->lines; j++)
      fprintf(f, "%d ", rotated[i][j]);
    fprintf(f, "\n");
  }
}

void swap(int *i, int *j)
{
  int temp = *i;
  *i = *j;
  *j = temp;
}

void fillPolarize(int **polarized, int starterL, int starterC, int average)
{
  for (int i = starterL; i < starterL + 2; i++)
    for (int j = starterC; j < starterC + 2; j++)
    {
      polarized[i][j] = average;
      // printf("\npolarized[%d][%d]: %d, average: %d", i, j, polarized[i][j], average);
    }
}