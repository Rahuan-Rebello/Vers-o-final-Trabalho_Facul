#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 50
#define MAX_GAMES 5

//Código desenvolvido por Pedro e Rahuan.

typedef struct
{
    int id;
    char nome[SIZE];
    char apelido[SIZE];
    int idade;
    char email[SIZE];
    char senha[SIZE];
    float fundos;                         // Adicionando fundos à conta
    char jogosComprados[MAX_GAMES][SIZE]; // Lista de jogos comprados
    int totalJogosComprados;
} Conta_Epic;

typedef struct
{
    Conta_Epic *contas;   // Ponteiro para o array dinâmico de contas
    int totalCadastrados; // Número atual de contas cadastradas
    int capacidade;       // Capacidade máxima do array
} GerenciadorContas;

typedef struct
{
    int id;
    char titulo[SIZE];
    float preco;
} Game;

Game catalogo[MAX_GAMES] = {
    {1, "The Witcher 3", 150.0},
    {2, "GTA V", 100.0},
    {3, "Cyberpunk 2077", 200.0},
    {4, "Minecraft", 50.0},
    {5, "FIFA 24", 180.0}};

void listarCatalogo() // Função para listar o catálogo
{
    printf("\nCatálogo de Games:\n");
    for (int i = 0; i < MAX_GAMES; i++)
    {
        printf("%d. %s - R$%.2f\n", catalogo[i].id, catalogo[i].titulo, catalogo[i].preco);
    }
}

void adicionarFundos(Conta_Epic *conta) // Função para adicionar os fundos à conta
{
    float valor;
    printf("Digite o valor para adicionar aos fundos: ");
    scanf("%f", &valor);
    getchar();

    if (valor > 0)
    {
        conta->fundos += valor;
        printf("Fundos adicionados com sucesso! Saldo atual: R$%.2f\n", conta->fundos);
    }
    else
    {
        printf("Valor inválido!\n");
    }
}

void comprarJogo(Conta_Epic *conta)
{
    int idJogo;
    listarCatalogo();
    printf("Escolha o ID do jogo para comprar: ");
    scanf("%d", &idJogo);
    getchar();

    if (idJogo < 1 || idJogo > MAX_GAMES)
    {
        printf("Jogo não encontrado.\n");
        return;
    }

    Game jogoSelecionado = catalogo[idJogo - 1];
    if (conta->fundos >= jogoSelecionado.preco)
    {
        if (conta->totalJogosComprados < MAX_GAMES)
        {
            conta->fundos -= jogoSelecionado.preco;
            strcpy(conta->jogosComprados[conta->totalJogosComprados++], jogoSelecionado.titulo);
            printf("Jogo '%s' comprado com sucesso! Fundos restantes: R$%.2f\n", jogoSelecionado.titulo, conta->fundos);
        }
        else
        {
            printf("Limite de jogos comprados atingido.\n");
        }
    }
    else
    {
        printf("Fundos insuficientes para comprar '%s'. Saldo: R$%.2f\n", jogoSelecionado.titulo, conta->fundos);
    }
}

int inicializarGerenciador(GerenciadorContas *gerenciador) // Função para inicializar o gerenciador de contas
{
    gerenciador->capacidade = 2; // Capacidade inicial
    gerenciador->totalCadastrados = 0;
    gerenciador->contas = (Conta_Epic *)malloc(gerenciador->capacidade * sizeof(Conta_Epic));
    if (!gerenciador->contas)
    {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }
}

// Função para redimensionar o array de contas
int redimensionarGerenciador(GerenciadorContas *gerenciador)
{
    gerenciador->capacidade *= 2; // Dobra a capacidade
    gerenciador->contas = (Conta_Epic *)realloc(gerenciador->contas, gerenciador->capacidade * sizeof(Conta_Epic));
    if (!gerenciador->contas)
    {
        printf("Erro ao realocar memoria.\n");
        return 1;
    }
}

// Função para cadastrar uma nova conta
void cadastrarConta(GerenciadorContas *gerenciador)
{
    if (gerenciador->totalCadastrados == gerenciador->capacidade)
    {
        redimensionarGerenciador(gerenciador);
    }

    Conta_Epic *novaConta = &gerenciador->contas[gerenciador->totalCadastrados];

    printf("Informe o numero da conta (deve ser maior que 100): ");
    scanf("%d", &novaConta->id);
    getchar();

    while (novaConta->id < 100)
    {
        printf("Numero da conta deve ser maior que 100: ");
        scanf("%d", &novaConta->id);
        getchar();
    }

    printf("Informe o Nome do Usuario: ");
    fgets(novaConta->nome, SIZE, stdin);
    novaConta->nome[strcspn(novaConta->nome, "\n")] = 0;

    printf("Informe o Apelido do Usuario: ");
    fgets(novaConta->apelido, SIZE, stdin);
    novaConta->apelido[strcspn(novaConta->apelido, "\n")] = 0;

    printf("Informe o email do Usuario: ");
    fgets(novaConta->email, SIZE, stdin);
    novaConta->email[strcspn(novaConta->email, "\n")] = 0;

    printf("Informe a senha do Usuario: ");
    fgets(novaConta->senha, SIZE, stdin);
    novaConta->senha[strcspn(novaConta->senha, "\n")] = 0;

    gerenciador->totalCadastrados++;
    printf("Usuario cadastrado com sucesso!\n");
}

// Função para listar todas as contas
void listarContas(GerenciadorContas *gerenciador)
{
    printf("\nContas Cadastradas:\n");
    for (int i = 0; i < gerenciador->totalCadastrados; i++)
    {
        Conta_Epic *conta = &gerenciador->contas[i];
        if (conta->id != 0)
        {
            printf("Numero da Conta: %d\n", conta->id);
            printf("Nome: %s\n", conta->nome);
            printf("Apelido: %s\n", conta->apelido);
            printf("Email: %s\n", conta->email);
            printf("Senha: ");
            for (int j = 0; j < (int)strlen(conta->senha); j++)
            {
                printf("*");
            }
            printf("\nJogos Comprados: ");
            if (conta->totalJogosComprados > 0)
            {
                for (int j = 0; j < conta->totalJogosComprados; j++)
                {
                    printf("%s%s", conta->jogosComprados[j], j < conta->totalJogosComprados - 1 ? ", " : "");
                }
            }
            else
            {
                printf("Nenhum jogo comprado");
            }
            printf("\n------------------------------\n");
            printf("\n\n");
        }
    }
}

// Função para excluir contas
void excluirConta(GerenciadorContas *gerenciador)
{
    int idExcluir;
    char confirmacao;

    printf("Informe o numero da conta a ser excluída: ");
    scanf("%d", &idExcluir);
    getchar();

    for (int i = 0; i < gerenciador->totalCadastrados; i++)
    {
        while (gerenciador->contas[i].id != idExcluir)
        {
            printf("\nDigite um id valido!");
            printf("\nInforme o numero da conta a ser excluída: ");
            scanf("%d", &idExcluir);
            getchar();
        }

        if (gerenciador->contas[i].id == idExcluir)
        {
            printf("Conta encontrada!\nNome: %s\n", gerenciador->contas[i].nome);
            printf("Confirmar exclusão? (S/N): ");
            scanf(" %c", &confirmacao);
            getchar();

            while (confirmacao != 'S' && confirmacao != 's' && confirmacao != 'n' && confirmacao != 'N')
            {
                printf("\n Digite um uma opcao valida!");
                printf("Confirmar exclusão? (S/N): ");
                scanf(" %c", &confirmacao);
            }

            if (confirmacao == 'S' || confirmacao == 's')
            {
                // Desloca as contas para preencher o espaço vazio
                for (int j = i; j < gerenciador->totalCadastrados - 1; j++)
                {
                    gerenciador->contas[j] = gerenciador->contas[j + 1];
                }
                gerenciador->totalCadastrados--;
                printf("\nConta excluída com sucesso.\n");
            }
            else if (confirmacao == 'N' || confirmacao == 'n')
            {
                printf("Exclusão cancelada.\n");
            }

            return;
        }
    }
}

// Função para ordenar contas alfabeticamente pelo nome
void ordenarContasPorNome(GerenciadorContas *gerenciador)
{
    int i, j;
    for (i = 0; i < gerenciador->totalCadastrados - 1; i++)
    {
        for (j = 0; j < gerenciador->totalCadastrados - i - 1; j++)
        {
            if (strcmp(gerenciador->contas[j].nome, gerenciador->contas[j + 1].nome) > 0)
            {
                // Troca as posições se o nome anterior for "maior" que o seguinte
                Conta_Epic temp = gerenciador->contas[j];
                gerenciador->contas[j] = gerenciador->contas[j + 1];
                gerenciador->contas[j + 1] = temp;
            }
        }
    }
    printf("Contas ordenadas alfabeticamente por nome.\n");
}

// Função para salvar as contas ordenadas em um arquivo .txt
void contasOrdenadasEmArquivo(GerenciadorContas *gerenciador, const char *arquivo)
{
    ordenarContasPorNome(gerenciador); // Ordena antes de salvar

    FILE *arquivoEpic = fopen(arquivo, "w"); // Abre o arquivo para escrita
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    for (int i = 0; i < gerenciador->totalCadastrados; i++)
    {
        Conta_Epic *conta = &gerenciador->contas[i];
        fprintf(arquivoEpic, "Número da Conta: %d\n", conta->id);
        fprintf(arquivoEpic, "Nome: %s\n", conta->nome);
        fprintf(arquivoEpic, "Apelido: %s\n", conta->apelido);
        fprintf(arquivoEpic, "Email: %s\n", conta->email);
        fprintf(arquivoEpic, "Senha: %s\n", conta->senha);
        fprintf(arquivoEpic, "\nJogos Comprados: ");
            if (conta->totalJogosComprados > 0)
            {
                for (int j = 0; j < conta->totalJogosComprados; j++)
                {
                    fprintf(arquivoEpic, "%s%s", conta->jogosComprados[j], j < conta->totalJogosComprados - 1 ? ", " : "");
                }
            }
            else
            {
                fprintf(arquivoEpic, "Nenhum jogo comprado");
            }
        fprintf(arquivoEpic, "\n------------------------------\n");
    }

    fclose(arquivoEpic); // Fecha o arquivo
    printf("Contas ordenadas salvas no arquivo '%s' com sucesso!\n", arquivo);
}

// Função para liberar a memória alocada
void liberarGerenciador(GerenciadorContas *gerenciador)
{
    free(gerenciador->contas);
}

int main()
{
    setlocale(LC_ALL, "Portuguese");

    GerenciadorContas gerenciador;
    inicializarGerenciador(&gerenciador);

    int escolha;

    do
    {

        printf("*********************************************************\n");
        printf("*                                                       *\n");
        printf("* EEEEE  PPPP   III  CCC                                *\n");
        printf("* E      P   P   I  C   C                               *\n");
        printf("* EEEE   PPPP    I  C                                   *\n");
        printf("* E      P       I  C   C                               *\n");
        printf("* EEEEE  P      III  CCC                                *\n");
        printf("*                                                       *\n");
        printf("*********************************************************\n");
        printf("*                                                       *\n");
        printf("* 1 - Cadastrar Conta na Epic                           *\n");
        printf("* 2 - Listar Contas                                     *\n");
        printf("* 3 - Excluir Conta Epic                                *\n");
        printf("* 4 - Adicionar fundos a sua carteira                   *\n");
        printf("* 5 - Comprar jogo                                      *\n");
        printf("* 6 - Listar contas em arquivo (em oredem alfabética)  *\n");
        printf("* 7 - Sair                                              *\n");
        printf("*                                                       *\n");
        printf("*********************************************************\n");
        printf("Entre com a opcao desejada: ");
        scanf("%d", &escolha);
        getchar();

        switch (escolha)
        {
        case 1:
            cadastrarConta(&gerenciador);
            break;
        case 2:
            listarContas(&gerenciador);
            break;
        case 3:
            excluirConta(&gerenciador);
            break;
        case 4:
        {
            int id;
            printf("Informe o ID da conta: ");
            scanf("%d", &id);
            getchar();
            for (int i = 0; i < gerenciador.totalCadastrados; i++)
            {
                while (gerenciador.contas[i].id != id)
                {
                    printf("\nDigite um id valido!");
                    printf("\nInforme o numero da conta a ser adicionada os fundos: ");
                    scanf("%d", &id);
                    getchar();
                }
                if (gerenciador.contas[i].id == id)
                {
                    adicionarFundos(&gerenciador.contas[i]);
                    break;
                }
            }
            break;
        }
        case 5:
        {

            int id;
            printf("Informe o ID da conta: ");
            scanf("%d", &id);
            getchar();
            for (int i = 0; i < gerenciador.totalCadastrados; i++)
            {
                while (gerenciador.contas[i].id != id)
                {
                    printf("\nDigite um id valido!");
                    printf("\nInforme o numero da conta primeiramente: ");
                    scanf("%d", &id);
                    getchar();
                }
                if (gerenciador.contas[i].id == id)
                {
                    comprarJogo(&gerenciador.contas[i]);
                    break;
                }
            }

            break;
        }
        case 6:
            contasOrdenadasEmArquivo(&gerenciador, "contas_Epic_ordenadas.txt");
            break;
        case 7:
            printf("\nAte logo! :)\n");
            break;
        default:
            printf("Por gentileza, digite uma opção valida: ");
            scanf("%i", &escolha);
            getchar();
        }
    } while (escolha != 7);

    liberarGerenciador(&gerenciador);
    return 0;
}
