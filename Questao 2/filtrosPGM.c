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

void getInfo(Pgm *pgm, char *fileName);
void build(Pgm *pgm, char *fileName);
void brighten(int **matrix, Pgm *pgm, char *fileName);
void zoom(int **matrix, Pgm *pgm, char *fileName);
void polarize(int **matrix, Pgm *pgm, char *fileName);
void rotate(int **matrix, Pgm *pgm, char *fileName);
void fillPolarized(int **matrix, int starterL, int starterC, int average);
void writeFile(int **matrix, int lines, int cols, int highest, char *fileName);

int main(int argc, char **argv) // Por simplicidade usarei o .pgm usado como exemplo.
{
  Pgm pgm;

  // Removendo o pegando só a parte antes do . do nome do arquivo
  char *fileType;
  char fileName[50];

  strcpy(fileName, argv[1]);
  fileType = strstr(fileName, ".pgm");

  *fileType = '\0';

  getInfo(&pgm, fileName);// Vai pegar as dimensions da matrix e o maior valor.

  pgm.matrix = (int **)malloc(pgm.lines * sizeof(int *));
  for (int i = 0; i < pgm.lines; i++)
    pgm.matrix[i] = (int *)malloc(pgm.cols * sizeof(int));

  build(&pgm, fileName);                // Construo a matriz
  brighten(pgm.matrix, &pgm, fileName); // Clareia
  rotate(pgm.matrix, &pgm, fileName);   // Roda
  zoom(pgm.matrix, &pgm, fileName);     // Zoom out
  polarize(pgm.matrix, &pgm, fileName); // Polariza, seta valores para 0 / 255
  return 0;
}

void getInfo(Pgm *pgm, char *fileName)
{
  char buffer[512];
  int currentLine = 1, line = 2;
  bool d = true;
  bool keepReading = true;
  char tempName[32];
  strcpy(tempName, fileName);
  printf("\ngetInfo tepName: %s\n", tempName);
  FILE *f = fopen(strcat(tempName, ".pgm"), "r");
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

void build(Pgm *pgm, char *fileName)
{
  char buffer[512];
  bool keepReading = true;
  char tempName[32];
  strcpy(tempName, fileName);
  printf("\nbuild tempName: %s, fileName: %s", tempName, fileName);
  int line = 0, total = 0, j = 0, i = 0;
  FILE *f = fopen(strcat(tempName, ".pgm"), "rb");
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
        // printf("values: %d, cols: %d, pgm->matrix[%d][%d], total: %d\n", atoi(values), pgm->cols, i, j, total);
        pgm->matrix[i][j] = atoi(values);
        if (i == pgm->lines - 1 && j == pgm->cols - 1)
          break;
        j++;
        if (j == pgm->cols)
        {
          j = 0;
          i++;
        }
        values = strtok(NULL, " ");
      }
    }
    line++;
  } while (keepReading);
  fclose(f);
}

void brighten(int **matrix, Pgm *pgm, char *fileName)
{
  int cols = pgm->cols, lines = pgm->lines;
  int **bright;
  int highest = 0;
  char tempName[32];
  strcpy(tempName, fileName);

  bright = (int **)malloc(lines * sizeof(int *));
  for (int i = 0; i < lines; i++)
    bright[i] = (int *)malloc(cols * sizeof(int));

  for (int i = 0; i < lines; i++)
    for (int j = 0; j < cols; j++)
    {
      // printf("\n Brighten matrix[%d][%d]: %d", i, j, matrix[i][j]);
      if (matrix[i][j] + BRIGHTEN > 255)
      {
        bright[i][j] = matrix[i][j] + (255 - matrix[i][j]);
      }
      else
        bright[i][j] = matrix[i][j] + BRIGHTEN;
      highest = (highest < bright[i][j]) ? bright[i][j] : highest;
    }
  
  //Removendo o excesso do nome do arquivo, normalmente o '.acsii'
  char *fileType = strchr(tempName, '.');

  if (fileType != NULL)
  {
     *fileType = '\0';
  }

  writeFile(bright, lines, cols, highest, (char *)strcat(tempName, "1.pgm"));
}

void zoom(int **matrix, Pgm *pgm, char *fileName)
{
  int **zoomed;
  int lines = pgm->lines / 2, cols = pgm->cols / 2;
  int starterC = 0;
  int starterL = 0;
  int sum = 0;
  int average = 0;
  int highest = 0;
  int i, j, k = 0, z = 0;
  char tempName[32];
  strcpy(tempName, fileName);

  zoomed = (int **)malloc(lines * sizeof(int *));
  for (i = 0; i < lines; i++)
    zoomed[i] = (int *)malloc(cols * sizeof(int));
  do
  {
    if (starterL >= pgm->lines || starterC >= pgm->cols)
      break;
    for (i = starterL; i < starterL + 2; i++) // Mesma ideia do polarize();
      for (j = starterC; j < starterC + 2; j++)
        sum += matrix[i][j];

    average = sum / 4;
    sum = 0;
    highest = (highest < average) ? average : highest;
    k = (starterL + 1) / 2; // Ao dividir por 2 ele converte pro inteiro mais proximo
    z = (starterC + 1) / 2; // Isso as vezes fazia com que 2 numeros diferentes, divididos dessem o mesmo resultado
    zoomed[k][z] = average; // 318 / 2 = 159, 319 / 2 = 319
                            // Somando 1 antes de dividir resolve esse problema
    if (starterC + 2 >= pgm->cols)
    {
      starterC = 0;
      starterL += 2;
    }
    else
      starterC += 2;
  } while (starterL < pgm->lines);

  //Removendo o excesso do nome do arquivo, normalmente o '.acsii'
  char *fileType = strchr(tempName, '.');

  if (fileType != NULL)
  {
     *fileType = '\0';
  }

  writeFile(zoomed, lines, cols, highest, (char *)strcat(tempName, "2.pgm"));
}

void polarize(int **matrix, Pgm *pgm, char *fileName)
{
  int **polarized;
  int cols = pgm->cols, lines = pgm->lines;
  int starterC = 0;
  int starterL = 0;
  int sum = 0;
  int average = 0;
  int highest = 0;
  char tempName[32];
  strcpy(tempName, fileName);

  polarized = (int **)malloc(lines * sizeof(int *));
  for (int i = 0; i < lines; i++)
    polarized[i] = (int *)malloc(cols * sizeof(int));
  do
  {
    if (starterL >= pgm->lines || starterC >= pgm->cols)
      break;

    for (int i = starterL; i < starterL + 2; i++)
      for (int j = starterC; j < starterC + 2; j++)
        sum += matrix[i][j]; // vai passar pela matrix[0][0], [0][1], [1][0], [1][1]

    highest = (highest < average) ? average : highest;
    average = (sum / 4 < 128) ? 0 : 255;
    fillPolarized(polarized, starterL, starterC, average); // Vou fazer a mesma coisa que no loop de cima, só q preenchendo a matrix com a média da soma
    sum = 0;
    if (starterC + 2 >= cols) // Se cheguei no numero max de colunas
    {
      starterC = 0;  // Seto a coluna pra 0
      starterL += 2; // E agora vou scanear 2 linhas pra baixo
    }                // Primeira vez [0][0]...[1][1], Segunda: [2][0]...[3][1]
    else
      starterC += 2; // Sempre quero começar a scanear dnv 2 colunas a frente da anterior ex:
                     // primeira vez: [0][0]...[1][1], segunda vez: [0][2]...[1][3]
  } while (starterL < lines);

  //Removendo o excesso do nome do arquivo, normalmente o '.acsii'
  char *fileType = strchr(tempName, '.');

  if (fileType != NULL)
  {
     *fileType = '\0';
  }

  writeFile(polarized, lines, cols, highest, (char *)strcat(tempName, "3.pgm"));
}

void rotate(int **matrix, Pgm *pgm, char *fileName)
{
  int cols = pgm->cols, lines = pgm->lines;
  int **rotated;
  int highest = pgm->highest;
  int i = 0, j = 0;
  char tempName[32];
  strcpy(tempName, fileName);

  rotated = (int **)malloc(cols * sizeof(int *));
  for (int i = 0; i < cols; i++)
    rotated[i] = (int *)malloc(lines * sizeof(int));

  for (i = 0; i < lines; i++)
    for (j = 0; j < cols; j++)
      rotated[j][lines - i - 1] = matrix[i][j];
  
  //Removendo o excesso do nome do arquivo, normalmente o '.acsii'
  char *fileType = strchr(tempName, '.');

  if (fileType != NULL)
  {
     *fileType = '\0';
  }
  
  writeFile(rotated, cols, lines, highest, (char *)strcat(tempName, "4.pgm"));
}

void fillPolarized(int **polarized, int starterL, int starterC, int average)
{
  for (int i = starterL; i < starterL + 2; i++)
    for (int j = starterC; j < starterC + 2; j++)
    {
      polarized[i][j] = average;
      // printf("\npolarized[%d][%d]: %d, average: %d", i, j, polarized[i][j], average);
    }
}

void writeFile(int **matrix, int lines, int cols, int highest, char *fileName)
{
  FILE *f = fopen(fileName, "wb");
  fprintf(f, "P2\n");
  fprintf(f, "%d %d\n", cols, lines);
  fprintf(f, "%d\n", highest);
  for (int i = 0; i < lines; i++)
  {
    for (int j = 0; j < cols; j++)
      fprintf(f, "%d ", matrix[i][j]);
    fprintf(f, "\n");
  }
}