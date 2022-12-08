#include <stdio.h>
#include <Windows.h>
#include <unistd.h>
#define MAX_FUNC 21
#define MAX_DEPT 5


//Esses valores servirão para auxiliar no manuseio
//dos dados dos arquivos
int numeroDeFuncionarios = 0;

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

void mostrarTabelaFunc(Lista *lista);
void mostrarTabelaDpto(departamento departamentos[]);
Node* adicionaNo(Lista *lista);
Node* admitir(Lista *Lista, int numeroDoFuncionario);
Node* reorganizaDepartamento(Lista *Lista, Node* novoFuncionario, departamento departamentos[]);
Node* acharFuncionarioEspecifico(Lista *lista, int numeroDoFuncionario);
Node* get(Lista *lista, int iDoFuncionario);
Node* funcionariosDeDepartamento(Lista *lista, int inicioDoDpto);
Node* mudarDeDepartamento(Lista *lista, Node* funcEscolhido, int departamentoAntigo, int departamentoNovo, departamento departamentos[]);
Node* demitir(Lista *lista, Node* funcDemitido, departamento departamentos[]);


int main() {


    //Declaração das principais variáveis do programa
    FILE *funcArq = fopen("funcionarios.dat", "r");
    FILE *departArq = fopen("departamentos.dat", "r");

    Lista listaDeFuncionarios;
    listaDeFuncionarios.head = NULL;
    departamento departamentos[MAX_DEPT];

    char lixo[1000];
    int operacao;


    int i;


    //Leitura do conteúdo lixo da primeira linha de ambos os arquivos
    fscanf(funcArq, "%[^\n]", lixo);
    fscanf(departArq, "%[^\n]", lixo);
    

    //lendo os dados do arquivo funcionarios.dat para o vetor 'funcionarios'
    while (!feof(funcArq)) {
        Node* noAuxiliar = adicionaNo(&listaDeFuncionarios);
        fscanf(funcArq, "%d %d", &noAuxiliar->trabalhador.linha, &noAuxiliar->trabalhador.numFunc);
        fscanf(funcArq, "%d %d", &noAuxiliar->trabalhador.nivel, &noAuxiliar->trabalhador.departamento);
        fscanf(funcArq, "%d ", &noAuxiliar->trabalhador.proximo);
        numeroDeFuncionarios++;
    }

    //lendo os dados do arquivo funcionarios.dat para o vetor 'funcionarios'
    for (i = 0; i < MAX_DEPT; i++) {
        if (feof(departArq)) {
            // -1 na linha será a forma de identificar que o funcionário não existe
            departamentos[i].codDepto = -1;
        } else {
            fscanf(departArq, "%d", &departamentos[i].codDepto);
            fscanf(departArq, "%s", &departamentos[i].nomeDepto);
            fscanf(departArq, "%d", &departamentos[i].inicio);
        }
    }

    mostrarTabelaFunc(&listaDeFuncionarios);
    mostrarTabelaDpto(departamentos);


    do {
        int numeroDoFunc, iDoInicio, dptoEscolhido;
        int departamentoAntigo, departamentoNovo;
        Lista listaDoDepartamento;
        Node* funcDaBusca, funcAuxiliar;
        Sleep(500);
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
                    printf("\nPor favor, insira os seguintes dados do novo funcionario:\n"
                           "\tNumero do funcionario: ");
                    scanf("%d", &numeroDoFunc);

                    Node* jaExiste = acharFuncionarioEspecifico(&listaDeFuncionarios, numeroDoFunc);
                    if (jaExiste != NULL) {
                        printf("\nFuncionario ja cadastrado\n");
                        Sleep(3000);
                        break;
                    }

                    Node* novoFuncionario = admitir(&listaDeFuncionarios, numeroDoFunc);
                    reorganizaDepartamento(&listaDeFuncionarios, novoFuncionario, departamentos);
                    numeroDeFuncionarios++;

                    printf("\nOperacao escolhida = admissao\n"
                           "Funcionario admitido:"
                           "\tNumero = %d\n"
                           "\tNivel salarial = %d\n"
                           "\tDepartamento = %d\n"
                           "\tProximo no departamento = %d\n"
                           "Segue lista atualizada: \n", novoFuncionario->trabalhador.numFunc,
                           novoFuncionario->trabalhador.nivel, novoFuncionario->trabalhador.departamento);
                    Sleep(6500);
                }
                else
                {
                    printf("\nO numero maximo de funcionarios ja foi alcancado\n");
                }
                
                break;
            case 2:
                if (numeroDeFuncionarios != 0) {
                    printf("\nInforme o funcionario de interesse (numFunc do funcionario): ");
                    scanf("%d", &numeroDoFunc);

                    funcDaBusca = acharFuncionarioEspecifico(&listaDeFuncionarios, numeroDoFunc);

                    if (funcDaBusca == NULL) {
                        printf("\nFuncionario nao encontrado!\n");
                        Sleep(3000);
                        break;
                    }

                    demitir(&listaDeFuncionarios, funcDaBusca, departamentos);

                    numeroDeFuncionarios--;

                    printf("\nOpcao escolhida = demitir\n"
                           "Funcionario escolhido = %d\n"
                           "Funcionario demitido com sucesso\n"
                           "Segue tabela atualizada:\n", numeroDoFunc);
                    Sleep(5000);
                }
                else
                {
                    printf("\nNao ha funcionarios para serem demitidos\n");
                    Sleep(3000);
                }
                break;
            case 3:
                printf("\nInforme o funcionario de interesse (numFunc do funcionario): ");
                scanf("%d", &numeroDoFunc);

                funcDaBusca = acharFuncionarioEspecifico(&listaDeFuncionarios, numeroDoFunc);

                if (funcDaBusca == NULL) {
                    printf("\nFuncionario nao encontrado!");
                    break;
                }
                
                departamentoAntigo = funcDaBusca->trabalhador.departamento;

                printf("\nO departamento atual eh: %d\n", departamentoAntigo);
                printf("\nPor favor, informe o novo: ");
                scanf("%d", &departamentoNovo);

                while (departamentoNovo < 1 || departamentoNovo >= MAX_DEPT) {
                    printf("\nEscolha um departamento valido!\n");
                    Sleep(1000);
                    mostrarTabelaDpto(departamentos);
                    printf("\nOpcao: ");
                    scanf("%d", &departamentoNovo);
                }

                mudarDeDepartamento(&listaDeFuncionarios, funcDaBusca, departamentoAntigo, departamentoNovo, departamentos);
                break;
            case 4:
                if (numeroDeFuncionarios != 0) {

                    printf("\nQual departamento de interesse (informe o codDpto): ");
                    scanf("%d", &dptoEscolhido);

                    while (dptoEscolhido < 1 || dptoEscolhido >= MAX_DEPT) {
                        printf("\nEscolha um departamento valido!\n");
                        Sleep(1000);
                        mostrarTabelaDpto(departamentos);
                        printf("\nOpcao: ");
                        scanf("%d", &dptoEscolhido);
                    }

                    iDoInicio = departamentos[dptoEscolhido-1].inicio;

                    listaDoDepartamento.head = funcionariosDeDepartamento(&listaDeFuncionarios, iDoInicio);
                    
                    printf("\nOperacao escolhida = visualizar funcionarios de um departamento\n"
                           "departamento escolhido = %d\n", dptoEscolhido);
                    Sleep(4000);

                    if (listaDoDepartamento.head == NULL) {
                        printf("O departamento esta vazio\n");
                    }
                    else
                    {
                        mostrarTabelaFunc(&listaDoDepartamento);
                    }
                }
                else
                {
                    printf("\nNao existe nenhum funcionario contradado\n");
                }
                Sleep(8000);
                break;
            case 5:
                printf("\nPor favor, informe o numero do funcionario que procura: ");
                scanf("%d", &numeroDoFunc);

                funcDaBusca = acharFuncionarioEspecifico(&listaDeFuncionarios, numeroDoFunc);

                printf("\nOperacao escolhida = consultar funcionario\n"
                       "numero do funcionario que se busca = %d", numeroDoFunc);

                if (funcDaBusca == NULL) {
                    printf("\nFuncionario nao encontrado\n");
                }
                else
                {
                    printf("\n| %s\t", "linha");
                    printf("| %s\t", "numFunc");
                    printf("| %s\t", "nivel");
                    printf("| %s\t", "departamento");
                    printf("| %s\t", "proximo");
                    printf("\n");

                    printf("| %d\t", funcDaBusca->trabalhador.linha);
                    printf("| %d\t\t", funcDaBusca->trabalhador.numFunc);
                    printf("| %d\t", funcDaBusca->trabalhador.nivel);
                    printf("| %d\t\t", funcDaBusca->trabalhador.departamento);
                    printf("| %d\t", funcDaBusca->trabalhador.proximo);
                    printf("\n");
                }

                Sleep(5000);

                break;
            default:
                printf("\nOpcao invalida, selecione uma das opcoes disponiveis!\n");
        }
        mostrarTabelaFunc(&listaDeFuncionarios);
        mostrarTabelaDpto(departamentos);

    }while (operacao != 0);

    atualizaArquivoDeFuncionarios(&listaDeFuncionarios);
    atualizaArquivoDeDepartamentos(departamentos);
    
    printf("\nPROGRAMA ENCERRADO, ARQUIVOS ATUALIZADOS\n\n");

    return 0;
}

void mostrarTabelaFunc(Lista *lista) {

    printf("\n| %s\t", "linha");
    printf("| %s\t", "numFunc");
    printf("| %s\t", "nivel");
    printf("| %s\t", "departamento");
    printf("| %s\t", "proximo");
    printf("\n");
    Sleep(100);

    for (Node* funcAtual = lista->head; funcAtual != NULL; funcAtual = funcAtual->proximo) {
        printf("| %d\t", funcAtual->trabalhador.linha);
        printf("| %d\t\t", funcAtual->trabalhador.numFunc);
        printf("| %d\t", funcAtual->trabalhador.nivel);
        printf("| %d\t\t", funcAtual->trabalhador.departamento);
        printf("| %d\t", funcAtual->trabalhador.proximo);
        printf("\n");
        Sleep(100);
    }
}

void mostrarTabelaDpto(departamento departamentos[]) {

    printf("\n| %s\t", "codDepto");
    printf("| %s\t", "nomeDepto");
    printf("| %s\t", "inicio");
    printf("\n");
    Sleep(100);

    for (int i = 0; i < MAX_DEPT-1; i++) {
        printf("| %d\t\t", departamentos[i].codDepto);
        printf("| %s\t", departamentos[i].nomeDepto);
        printf("| %d\t", departamentos[i].inicio);
        printf("\n");
        Sleep(100);
    }
}

Node* get(Lista *lista, int iDoFuncionario) {
    Node* funcAtual = lista->head;

    if (iDoFuncionario < 0) {
        return NULL;
    }

    for (int i = 0; i < iDoFuncionario; i++) {
        funcAtual = funcAtual->proximo;
    }

    return funcAtual;
}

Node* adicionaNo(Lista *lista) {
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

Node* acharFuncionarioEspecifico(Lista *lista, int numeroDoFuncionario) {
    Node* noAuxiliar = lista->head;

    while (noAuxiliar != NULL && noAuxiliar->trabalhador.numFunc != numeroDoFuncionario) {
        noAuxiliar = noAuxiliar->proximo;
    }

    return noAuxiliar;
}

Node* funcionariosDeDepartamento(Lista *lista, int inicioDoDpto) {

    if (inicioDoDpto == -1) {
        return NULL;
    }

    Lista funcDpto;
    funcDpto.head = NULL;

    Node* noDoDpto = get(lista, inicioDoDpto);
    Node* noAuxiliar = funcDpto.head;

    while (noDoDpto->trabalhador.proximo != -1) {
        noAuxiliar = adicionaNo(&funcDpto);

        noAuxiliar->trabalhador.linha = noDoDpto->trabalhador.linha;
        noAuxiliar->trabalhador.numFunc = noDoDpto->trabalhador.numFunc;
        noAuxiliar->trabalhador.nivel = noDoDpto->trabalhador.nivel;
        noAuxiliar->trabalhador.departamento = noDoDpto->trabalhador.departamento;
        noAuxiliar->trabalhador.proximo = noDoDpto->trabalhador.proximo;

        noDoDpto = get(lista, noDoDpto->trabalhador.proximo);
    }

    noAuxiliar = adicionaNo(&funcDpto);

    noAuxiliar->trabalhador.linha = noDoDpto->trabalhador.linha;
    noAuxiliar->trabalhador.numFunc = noDoDpto->trabalhador.numFunc;
    noAuxiliar->trabalhador.nivel = noDoDpto->trabalhador.nivel;
    noAuxiliar->trabalhador.departamento = noDoDpto->trabalhador.departamento;
    noAuxiliar->trabalhador.proximo = noDoDpto->trabalhador.proximo;

    return funcDpto.head;
}

Node* admitir(Lista *lista, int numeroDoFuncionario) {
    Node* novoNode = adicionaNo(lista);
    novoNode->trabalhador.linha = numeroDeFuncionarios;
    novoNode->trabalhador.proximo = -1;

    novoNode->trabalhador.numFunc = numeroDoFuncionario;

    printf("\tnivel salarial do funcionario: ");
    scanf("%d", &novoNode->trabalhador.nivel);

    printf("\tdepartamento do funcionario: ");
    scanf("%d", &novoNode->trabalhador.departamento);

    while (novoNode->trabalhador.departamento >= MAX_DEPT || novoNode->trabalhador.departamento < 1) {
        printf("\nNumero de departamento invalido!\n");
        printf("\tdepartamento do funcionario: ");
        scanf("%d", &novoNode->trabalhador.departamento);
    }

    printf("Novo funcionario adicionado\n");

    return novoNode;
}

Node* reorganizaDepartamento(Lista *lista, Node* novoFuncionario, departamento departamentos[]) {
    int iDoDepartamento = novoFuncionario->trabalhador.departamento-1;

    int iDoFuncionario = departamentos[iDoDepartamento].inicio;
    int iDoProxfunc;

    if (iDoFuncionario == -1) {
        departamentos[iDoDepartamento].inicio = novoFuncionario->trabalhador.linha;
        return novoFuncionario;
    }

    Node* funcAtual = get(lista, iDoFuncionario);
    Node* proxFunc;

    if (funcAtual->trabalhador.numFunc > novoFuncionario->trabalhador.numFunc) {
        departamentos[iDoDepartamento].inicio = novoFuncionario->trabalhador.linha;
        novoFuncionario->trabalhador.proximo = funcAtual->trabalhador.linha;
        return novoFuncionario;
    }

    iDoProxfunc = funcAtual->trabalhador.proximo;

    while (iDoProxfunc != -1) {
        funcAtual = get(lista, iDoFuncionario);
        proxFunc = get(lista, iDoProxfunc);

        if (funcAtual->trabalhador.numFunc <= novoFuncionario->trabalhador.numFunc &&
            novoFuncionario->trabalhador.numFunc <= proxFunc->trabalhador.numFunc)
        {
            funcAtual->trabalhador.proximo = novoFuncionario->trabalhador.linha;
            novoFuncionario->trabalhador.proximo = proxFunc->trabalhador.linha;
            return novoFuncionario;   
        }
        else
        {
            iDoFuncionario = iDoProxfunc;
            iDoProxfunc = proxFunc->trabalhador.proximo;
        }
    }

    proxFunc->trabalhador.proximo = novoFuncionario->trabalhador.linha;
    novoFuncionario->trabalhador.proximo = -1;
    return novoFuncionario;
}

Node* mudarDeDepartamento(Lista *lista, Node* funcEscolhido, int departamentoAntigo, int departamentoNovo, departamento departamentos[]) {
    
    Lista listaDoDepartamento;
    listaDoDepartamento.head = funcionariosDeDepartamento(lista, departamentos[departamentoAntigo-1].inicio);
    Node* noAuxiliar = listaDoDepartamento.head;
    Node* nodeParaAlterar;

    if (noAuxiliar->trabalhador.numFunc != funcEscolhido->trabalhador.numFunc) {
        while (noAuxiliar->trabalhador.proximo != funcEscolhido->trabalhador.linha) {
            noAuxiliar = noAuxiliar->proximo;
        }

        int indiceDoNoh = noAuxiliar->trabalhador.linha;

        noAuxiliar->trabalhador.proximo = funcEscolhido->trabalhador.proximo;

        nodeParaAlterar = get(lista, noAuxiliar->trabalhador.linha);
        
        nodeParaAlterar->trabalhador.proximo = noAuxiliar->trabalhador.proximo;
    }
    else
    {
        departamentos[departamentoAntigo-1].inicio = funcEscolhido->trabalhador.proximo;
    }

    funcEscolhido->trabalhador.departamento = departamentoNovo;

    reorganizaDepartamento(lista, funcEscolhido, departamentos);

    return funcEscolhido;
}

Node* demitir(Lista *lista, Node* funcDemitido, departamento departamentos[]) {
    Node* noAuxiliar;
    int depart = funcDemitido->trabalhador.departamento-1;

    // Reorganizando o departamento para existir sem o funcionário demitido
    if (departamentos[depart].inicio == funcDemitido->trabalhador.linha) {
        departamentos[depart].inicio = funcDemitido->trabalhador.proximo;
    }
    else
    {
        noAuxiliar = lista->head;

        while (noAuxiliar->trabalhador.proximo != funcDemitido->trabalhador.linha) {
            noAuxiliar = noAuxiliar->proximo;
        }

        noAuxiliar->trabalhador.proximo = funcDemitido->trabalhador.proximo;
    }

    //Efetivamente demitindo o funcionário
    noAuxiliar = lista->head;

    if (noAuxiliar == funcDemitido) {
        lista->head = funcDemitido->proximo;
        funcDemitido->proximo = NULL;
        return funcDemitido;
    }

    while (noAuxiliar->proximo != funcDemitido) {
        noAuxiliar = noAuxiliar->proximo;
    }

    noAuxiliar->proximo = funcDemitido->proximo;
    funcDemitido->proximo = NULL;

    //Atualizando todos os valores das listas
    noAuxiliar = lista->head;
    int linhaDeletada = funcDemitido->trabalhador.linha;

    while (noAuxiliar != NULL) {
        if (noAuxiliar->trabalhador.linha > linhaDeletada) {
            noAuxiliar->trabalhador.linha--;
        }
        if (noAuxiliar->trabalhador.proximo > linhaDeletada) {
            noAuxiliar->trabalhador.proximo--;
        }
        noAuxiliar = noAuxiliar->proximo;
    }

    for (int i = 0; departamentos[i].codDepto != -1 && i < MAX_DEPT-1; i++) {
        if (departamentos[i].inicio > linhaDeletada) {
            departamentos[i].inicio--;
        }
    }

    return funcDemitido;
}

void atualizaArquivoDeFuncionarios(Lista *lista) {
    FILE *f = fopen("funcionarios.dat", "w");
    if (f == NULL) {
        printf("\nNao foi possivel abrir o arquivo\n");
        Sleep(3000);
        return;
    }

    fprintf(f, "%s ", "linha");
    fprintf(f, "%s ", "numFunc");
    fprintf(f, "%s ", "nivel");
    fprintf(f, "%s ", "departamento");
    fprintf(f, "%s", "proximo");
    fprintf(f, "\n");
    
    for (Node* funcAtual = lista->head; funcAtual != NULL; funcAtual = funcAtual->proximo) {
        fprintf(f, "%d ", funcAtual->trabalhador.linha);
        fprintf(f, "%d ", funcAtual->trabalhador.numFunc);
        fprintf(f, "%d ", funcAtual->trabalhador.nivel);
        fprintf(f, "%d ", funcAtual->trabalhador.departamento);
        fprintf(f, "%d", funcAtual->trabalhador.proximo);
        fprintf(f, "\n");
        }

    fclose(f);

    return;
}

void atualizaArquivoDeDepartamentos(departamento departamentos[]) {
    FILE *f = fopen("departamentos.dat", "w");
    if (f == NULL) {
        printf("\nNao foi possivel abrir o arquivo\n");
        Sleep(3000);
        return;
    }

    fprintf(f, "%s ", "codDepto");
    fprintf(f, "%s ", "nomeDepto");
    fprintf(f, "%s ", "inicio");
    fprintf(f, "\n");
    
    for (int i = 0; i < MAX_DEPT-1; i++) {
        fprintf(f, "%d ", departamentos[i].codDepto);
        fprintf(f, "%s ", departamentos[i].nomeDepto);
        fprintf(f, "%d", departamentos[i].inicio);
        fprintf(f, "\n");
        }
}