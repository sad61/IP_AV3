#include <stdio.h>
#define MAX_FUNC 21
#define MAX_DEPT 6

typedef struct {
    int linha;
    int numFunc;
    int nivel;
    int departamento;
    int proximo;
} funcionario;

typedef struct {
    int codDepto;
    char nomeDepto[30];
    int inicio;
} departamento;

int main() {
    FILE *funcArq = fopen("funcionarios.dat", "r");
    FILE *departArq = fopen("departamentos.dat", "r");

    funcionario funcionarios[MAX_FUNC];
    departamento departamentos[MAX_DEPT];

    char lixo[1000];

    int i;

    fscanf(funcArq, "%[^\n]", lixo);
    fscanf(departArq, "%[^\n]", lixo);
    
    for (i = 0; i < MAX_FUNC; i++) {
        if (feof(funcArq)) {
            // -1 na linha será a forma de identificar que o funcionário não existe
            funcionarios[i].linha = -1;
        } else {
            fscanf(funcArq, "%d %d", &funcionarios[i].linha, &funcionarios[i].numFunc);
            fscanf(funcArq, "%d %d", &funcionarios[i].nivel, &funcionarios[i].departamento);
            fscanf(funcArq, "%d ", &funcionarios[i].proximo);
        }
    }

    for (i = 0; i < MAX_DEPT; i++) {
        if (feof(departArq)) {
            // -1 na linha será a forma de identificar que o funcionário não existe
            departamentos[i].codDepto = -1;
        } else {
            fscanf(departArq, "%d", &departamentos[i].codDepto);
            fscanf(departArq, " %s", &departamentos[i].nomeDepto);
            fscanf(departArq, "%d", &departamentos[i].inicio);
        }
    }
    
    return 0;
}