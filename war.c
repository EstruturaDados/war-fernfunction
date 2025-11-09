// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define QTD_TERRITORIOS 5
#define TAM_NOME 64
#define TAM_COR 16
#define MISSAO_DESTRUIR_VERDE 1
#define MISSAO_CONQUISTAR_3 2

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct
{
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
Territorio *alocarMapa(size_t qtd);
void liberarMemoria(Territorio *mapa);
// Funções de interface com o usuário:
void exibirMenuPrincipal(void);
void exibirMapa(const Territorio *mapa, size_t qtd, const char *corJogador);
void exibirMissao(int missaoId);
// Funções de lógica principal do jogo:
void inicializarTerritorios(Territorio *mapa, size_t qtd);
void faseDeAtaque(Territorio *mapa, size_t qtd, const char *corJogador);
int simularAtaque(Territorio *mapa, size_t qtd, int idxAtacante, int idxDefensor, const char *corJogador);
int sortearMissao(const char *corJogador);
int verificarVitoria(const Territorio *mapa, size_t qtd, const char *corJogador, int missaoId);
// Função utilitária:
void limparBufferEntrada(void);

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main()
{
    setlocale(LC_ALL, "pt_BR.UTF-8");
    srand((unsigned)time(NULL));
    Territorio *mapa = alocarMapa(QTD_TERRITORIOS);
    if (!mapa)
        return 1;
    inicializarTerritorios(mapa, QTD_TERRITORIOS);

    char corJogador[TAM_COR];
    int opcaoCor = -1;
    const char *opcoesCores[] = {"Azul", "Vermelho", "Amarelo", "Preto", "Verde"};
    printf("Escolha sua cor:\n");
    for (int i = 0; i < 5; i++)
        printf("%d - %s\n", i + 1, opcoesCores[i]);
    printf("Opção: ");
    if (scanf("%d", &opcaoCor) != 1 || opcaoCor < 1 || opcaoCor > 5)
    {
        strcpy(corJogador, "Azul");
    }
    else
    {
        strcpy(corJogador, opcoesCores[opcaoCor - 1]);
    }
    limparBufferEntrada();

    int missaoId = sortearMissao(corJogador);
    int venceu = 0;
    int opcao = -1;
    do
    {
        exibirMapa(mapa, QTD_TERRITORIOS, corJogador);
        printf("\nSua missão: ");
        exibirMissao(missaoId);
        exibirMenuPrincipal();
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1)
        {
            limparBufferEntrada();
            opcao = -1;
        }
        limparBufferEntrada();
        switch (opcao)
        {
        case 1:
            faseDeAtaque(mapa, QTD_TERRITORIOS, corJogador);
            venceu = verificarVitoria(mapa, QTD_TERRITORIOS, corJogador, missaoId);
            if (venceu)
            {
                exibirMapa(mapa, QTD_TERRITORIOS, corJogador);
                printf("\nParabéns! Você cumpriu sua missão!\n");
                opcao = 0;
            }
            else
            {
                printf("\nPressione Enter para continuar...");
                getchar();
            }
            break;
        case 2:
            if (verificarVitoria(mapa, QTD_TERRITORIOS, corJogador, missaoId))
            {
                printf("\nCondição de vitória atingida. Você venceu!\n");
                opcao = 0;
            }
            else
            {
                printf("\nMissão ainda não cumprida.\n");
                printf("Pressione Enter para continuar...");
                getchar();
            }
            break;
        case 0:
            break;
        default:
            printf("\nOpção inválida.\n");
            printf("Pressione Enter para continuar...");
            getchar();
        }
    } while (opcao != 0);

    liberarMemoria(mapa);
    return 0;
}

// --- Implementação das Funções ---
Territorio *alocarMapa(size_t qtd)
{
    Territorio *mapa = (Territorio *)calloc(qtd, sizeof(Territorio));
    return mapa;
}

void inicializarTerritorios(Territorio *mapa, size_t qtd)
{
    const char *nomesPadrao[QTD_TERRITORIOS] = {"Alfa", "Bravo", "Charlie", "Delta", "Echo"};
    const char *coresPadrao[QTD_TERRITORIOS] = {"Verde", "Vermelho", "Azul", "Amarelo", "Preto"};
    for (size_t i = 0; i < qtd; i++)
    {
        strncpy(mapa[i].nome, nomesPadrao[i], TAM_NOME - 1);
        mapa[i].nome[TAM_NOME - 1] = '\0';
        strncpy(mapa[i].cor, coresPadrao[i], TAM_COR - 1);
        mapa[i].cor[TAM_COR - 1] = '\0';
        mapa[i].tropas = 3 + rand() % 4;
    }
}

void liberarMemoria(Territorio *mapa)
{
    free(mapa);
}

void exibirMenuPrincipal(void)
{
    printf("\nMenu:\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missão\n");
    printf("0 - Sair\n");
}

void exibirMapa(const Territorio *mapa, size_t qtd, const char *corJogador)
{
    printf("\n%-4s %-16s %-12s %-8s %-8s\n", "ID", "Território", "Cor", "Tropas", "Seu?");
    for (size_t i = 0; i < qtd; i++)
    {
        int seu = strcmp(mapa[i].cor, corJogador) == 0;
        printf("%-4zu %-16s %-12s %-8d %-8s\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas, seu ? "Sim" : "Não");
    }
}

void exibirMissao(int missaoId)
{
    if (missaoId == MISSAO_DESTRUIR_VERDE)
    {
        printf("Destruir o exército Verde.\n");
    }
    else if (missaoId == MISSAO_CONQUISTAR_3)
    {
        printf("Conquistar 3 territórios.\n");
    }
    else
    {
        printf("Indefinida.\n");
    }
}

void faseDeAtaque(Territorio *mapa, size_t qtd, const char *corJogador)
{
    int a = 0, d = 0;
    printf("\nEscolha o território atacante (1-%zu): ", qtd);
    if (scanf("%d", &a) != 1)
    {
        limparBufferEntrada();
        return;
    }
    printf("Escolha o território defensor (1-%zu): ", qtd);
    if (scanf("%d", &d) != 1)
    {
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();
    if (a < 1 || a > (int)qtd || d < 1 || d > (int)qtd || a == d)
    {
        printf("\nEscolha inválida.\n");
        return;
    }
    int conquistou = simularAtaque(mapa, qtd, a - 1, d - 1, corJogador);
    if (conquistou)
    {
        printf("\nTerritório conquistado!\n");
    }
}

int simularAtaque(Territorio *mapa, size_t qtd, int idxAtacante, int idxDefensor, const char *corJogador)
{
    if (idxAtacante < 0 || idxAtacante >= (int)qtd || idxDefensor < 0 || idxDefensor >= (int)qtd)
        return 0;
    if (strcmp(mapa[idxAtacante].cor, corJogador) != 0)
    {
        printf("\nO atacante deve ser seu território.\n");
        return 0;
    }
    if (strcmp(mapa[idxDefensor].cor, corJogador) == 0)
    {
        printf("\nVocê não pode atacar seu próprio território.\n");
        return 0;
    }
    if (mapa[idxAtacante].tropas < 2)
    {
        printf("\nTropas insuficientes para atacar (é necessário manter pelo menos 1).\n");
        return 0;
    }
    if (mapa[idxDefensor].tropas <= 0)
    {
        printf("\nO defensor não possui tropas.\n");
        return 0;
    }
    int dadoAtacante = 1 + rand() % 6;
    int dadoDefensor = 1 + rand() % 6;
    printf("\nDados: Atacante=%d x Defensor=%d\n", dadoAtacante, dadoDefensor);
    if (dadoAtacante >= dadoDefensor)
    {
        mapa[idxDefensor].tropas -= 1;
        printf("Defensor perdeu 1 tropa. Tropas defensor: %d\n", mapa[idxDefensor].tropas);
        if (mapa[idxDefensor].tropas <= 0)
        {
            strncpy(mapa[idxDefensor].cor, corJogador, TAM_COR - 1);
            mapa[idxDefensor].cor[TAM_COR - 1] = '\0';
            mapa[idxDefensor].tropas = 1;
            mapa[idxAtacante].tropas -= 1;
            printf("Território %s agora é seu. Tropas movidas: 1\n", mapa[idxDefensor].nome);
            return 1;
        }
    }
    else
    {
        mapa[idxAtacante].tropas -= 1;
        printf("Atacante perdeu 1 tropa. Tropas atacante: %d\n", mapa[idxAtacante].tropas);
    }
    return 0;
}

int sortearMissao(const char *corJogador)
{
    if (strcmp(corJogador, "Verde") == 0)
        return MISSAO_CONQUISTAR_3;
    int r = 1 + rand() % 2;
    return r == 1 ? MISSAO_DESTRUIR_VERDE : MISSAO_CONQUISTAR_3;
}

int verificarVitoria(const Territorio *mapa, size_t qtd, const char *corJogador, int missaoId)
{
    if (missaoId == MISSAO_DESTRUIR_VERDE)
    {
        for (size_t i = 0; i < qtd; i++)
            if (strcmp(mapa[i].cor, "Verde") == 0)
                return 0;
        return 1;
    }
    if (missaoId == MISSAO_CONQUISTAR_3)
    {
        int seus = 0;
        for (size_t i = 0; i < qtd; i++)
            if (strcmp(mapa[i].cor, corJogador) == 0)
                seus++;
        return seus >= 3;
    }
    return 0;
}

void limparBufferEntrada(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}