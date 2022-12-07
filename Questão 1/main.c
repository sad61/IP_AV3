#include <stdio.h>
#define MAX_FUNC 21
#define MAX_DEPT 6


//Esses valores servirão para auxiliar no manuseio
//dos dados dos arquivos
int numeroDeFuncionarios = 0;
int numeroDeDepartamentos = 0;

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

typedef struct{
    funcionario trabalhador;
    struct Node* proximo;   
} Node;

typedef struct{
    Node* head;
} Lista;

Node* adicionaNo(Lista *lista);

int main() {


    //Declaração das principais variáveis do programa
    FILE *funcArq = fopen("funcionarios.dat", "r");
    FILE *departArq = fopen("departamentos.dat", "r");

    funcionario funcionarios[MAX_FUNC];
    departamento departamentos[MAX_DEPT];

    char lixo[1000];
    int operacao;


    int i;


    //Leitura do conteúdo lixo da primeira linha de ambos os arquivos
    fscanf(funcArq, "%[^\n]", lixo);
    fscanf(departArq, "%[^\n]", lixo);
    

    //lendo os dados do arquivo funcionarios.dat para o vetor 'funcionarios'
    for (i = 0; i < MAX_FUNC; i++) {
        if (feof(funcArq)) {
            // -1 na linha será a forma de identificar que o funcionário não existe
            funcionarios[i].linha = -1;
        } else {
            fscanf(funcArq, "%d %d", &funcionarios[i].linha, &funcionarios[i].numFunc);
            fscanf(funcArq, "%d %d", &funcionarios[i].nivel, &funcionarios[i].departamento);
            fscanf(funcArq, "%d ", &funcionarios[i].proximo);
            numeroDeFuncionarios = i+1;
        }
    }

    //lendo os dados do arquivo funcionarios.dat para o vetor 'funcionarios'
    for (i = 0; i < MAX_DEPT; i++) {
        if (feof(departArq)) {
            // -1 na linha será a forma de identificar que o funcionário não existe
            departamentos[i].codDepto = -1;
        } else {
            fscanf(departArq, "%d", &departamentos[i].codDepto);
            fscanf(departArq, " %s", &departamentos[i].nomeDepto);
            fscanf(departArq, "%d", &departamentos[i].inicio);
            numeroDeDepartamentos = i+1;
        }
    }


    //Gerando e preenchendo a lista endaceada 'listaDeFuncionarios'
    //para permitir melhor manipulação dos dados
    Lista listaDeFuncionarios;
    listaDeFuncionarios.head = NULL;

    for (int i = 0; funcionarios[i].linha != -1 && i < MAX_FUNC; i++) {
        Node* noAuxiliar = adicionaNo(&listaDeFuncionarios);

        noAuxiliar->trabalhador.linha = funcionarios[i].linha;
        noAuxiliar->trabalhador.numFunc = funcionarios[i].numFunc;
        noAuxiliar->trabalhador.nivel = funcionarios[i].nivel;
        noAuxiliar->trabalhador.departamento = funcionarios[i].departamento;
        noAuxiliar->trabalhador.proximo = funcionarios[i].proximo;
    }


    //printando na tela a lista de funcionários atual
    printf("\n| %s\t", "linha");
    printf("| %s\t", "numFunc");
    printf("| %s\t", "nivel");
    printf("| %s\t", "departamento");
    printf("| %s\t", "proximo");
    printf("\n");
    for (int i = 0; funcionarios[i].linha != -1; i++) {
        printf("| %d\t", funcionarios[i].linha);
        printf("| %d\t\t", funcionarios[i].numFunc);
        printf("| %d\t", funcionarios[i].nivel);
        printf("| %d\t\t", funcionarios[i].departamento);
        printf("| %d\t", funcionarios[i].proximo);
        printf("\n");
    }


    //printando na tela a lista de departamentos
    printf("\n| %s\t", "codDepto");
    printf("| %s\t", "nomeDepto");
    printf("| %s\t", "inicio");
    printf("\n");
    for (int i = 0; departamentos[i].codDepto != -1; i++) {
        printf("| %d\t", departamentos[i].codDepto);
        printf("| %s\t\t", departamentos[i].nomeDepto);
        printf("| %d\t", departamentos[i].inicio);
        printf("\n");
    }


    do {
        int numeroDoFuncionario, nivelSalarial;
        int numeroDoNovoDepartamento, numeroDoDepartamentoAnterior;

        printf("\nSelecione o tipo de operacao desejada:\n"
               "\tdigite '0' para encerrar o programa\n"
               "\tdigite '1' para admitir novo funcionario\n"
               "\tdigite '2' para demitir um funcionario\n"
               "\tdigite '3' para mudar um funcionario de departamento\n"
               "\tdigite '4' para consultar todos os funcionarios de um departamento\n"
               "\tdigite '5' para consultar um funcionario especifico\n"
               "Operacao escolhida: ");
        scanf("%d", &operacao);

        switch (operacao)
        {
            case 0:
                break;
            case 1:
                if (numeroDeFuncionarios < MAX_FUNC-1)
                {
                    admitir();
                    numeroDeFuncionarios++;
                }
                else
                {
                    printf("\nO numero maximo de funcionarios ja foi alcancado\n");
                }
                
                break;
            case 2:
                //demitir();
                break;
            case 3:
                //mudarDeDepartamento();
                break;
            case 4:
                //consultarFuncionariosDeDepartamento();
                break;
            case 5:
                //consultarFuncionarioEspecifico();
                break;
            default:
                printf("\nOpcao invalida, selecione uma das opcoes disponiveis!\n");
        }
    }while (operacao != 0);
    
    printf("\nPROGRAMA ENCERRADO\n\n");

    return 0;
}

Node* adicionaNo(Lista *lista){
    Node* node = malloc(sizeof(Node));
    node->proximo = NULL;

    if (lista->head == NULL) {
        lista->head = node;
    } 
    else 
    {
        Node* noAuxiliar = lista->head;

        while (noAuxiliar->proximo != NULL)
        {
            noAuxiliar = noAuxiliar->proximo;
        }

        noAuxiliar->proximo = node;
    }

    return node;
}