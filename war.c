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
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#ifdef _WIN32
#include <windows.h>
#endif

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define MAX_STRING_NOME 30
#define MAX_STRING_COR 10

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[MAX_STRING_NOME];
    char cor[MAX_STRING_COR];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
Territorio* alocarTerritorios(int quantidade);
void liberarMemoria(Territorio* mapa);
// Funções de interface com o usuário:
void exibirMapa(const Territorio* mapa, int quantidade);
void exibirStatusTerritorios(Territorio* atacante, Territorio* defensor);
// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio* mapa, int quantidade);
void atacar(Territorio* atacante, Territorio* defensor);
// Função utilitária:
void limparBufferEntrada();

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // Define o locale para o sistema (mais portátil)
    setlocale(LC_ALL, "");

    // No Windows, force o console para UTF-8 para exibir acentuação corretamente
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif
    int numTerritorios;
    
    // Inicializa a semente para números aleatórios
    srand(time(NULL));
    
    printf("=============================================\n");
    printf("BEM-VINDO AO JOGO WAR!\n");
    printf("=============================================\n");
    printf("Digite a quantidade de territórios desejada: ");
    scanf("%d", &numTerritorios);
    limparBufferEntrada();
    
    // Validação da entrada
    if (numTerritorios <= 0) {
        printf("Erro: A quantidade de territórios deve ser maior que zero!\n");
        return 1;
    }
    
    // Aloca memoria dinamicamente para o vetor de territórios
    Territorio* mapa = alocarTerritorios(numTerritorios);
    if (mapa == NULL) {
        printf("Erro: Falha ao alocar memória para os territórios!\n");
        return 1;
    }

    printf("=============================================\n");
    printf("Vamos cadastrar os %i territórios do nosso mundo:\n", numTerritorios);
    
    // Cadastra os territórios iniciais.
    for (int i = 0; i < numTerritorios; i++) {
        printf("--- Cadastrando o Território %i ---\n",i+1);
        printf("Digite o nome do território: ");
        fgets(mapa[i].nome, MAX_STRING_NOME, stdin);
        // Remove o '\n' deixado pelo fgets
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';
        printf("Digite a cor do exército: ");
        fgets(mapa[i].cor, MAX_STRING_COR, stdin);
        // Remove o '\n' deixado pelo fgets
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';
        
        // Validação do número de tropas
        do {
            printf("Digite o número de tropas (mínimo 1): ");
            scanf("%d", &mapa[i].tropas);
            limparBufferEntrada();
            
            if (mapa[i].tropas < 1) {
                printf("Erro: Um território deve ter no mínimo 1 exército!\n");
            }
        } while (mapa[i].tropas < 1);
    }

    printf("=============================================\n");
    printf("        MAPA DO MUNDO - ESTADO ATUAL\n");
    printf("=============================================\n");

    // Exibe o estado atual dos territórios cadastrados.
    for (int j = 0; j < numTerritorios; j++) {
        printf("\n--- Território %i ---\n",j+1);
        printf("- Nome: %s", mapa[j].nome);
        printf("- Cor do exército: %s", mapa[j].cor);
        printf("- Número de tropas: %d\n", mapa[j].tropas);
    }
    
    // Executa a fase de ataque
    printf("\n=============================================\n");
    printf("        INICIANDO FASE DE ATAQUE\n");
    printf("=============================================\n");
    faseDeAtaque(mapa, numTerritorios);
    
    // Libera a memória alocada
    liberarMemoria(mapa);
    
    return 0;
}

// --- Implementação das Funções ---

// alocarTerritorios():
// Aloca dinamicamente a memória para um vetor de territórios usando calloc.
// Recebe como parâmetro a quantidade de territórios a serem alocados.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
Territorio* alocarTerritorios(int quantidade) {
    Territorio* mapa = (Territorio*)calloc(quantidade, sizeof(Territorio));
    if (mapa == NULL) {
        fprintf(stderr, "Erro: Não foi possível alocar memória para %d territórios!\n", quantidade);
        return NULL;
    }
    printf("Memória alocada com sucesso para %d territórios!\n", quantidade);
    return mapa;
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
// Recebe como parâmetro um ponteiro para o vetor de territórios alocado dinamicamente.
void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
        printf("\nMemória liberada com sucesso!\n");
    }
}

// exibirStatusTerritorios():
// Exibe o estado atual de dois territórios (atacante e defensor) em forma de texto.
// Mostra nome, dono (cor), e número de tropas de cada território.
void exibirStatusTerritorios(Territorio* atacante, Territorio* defensor) {
    printf("\n--- STATUS DOS TERRITÓRIOS ---\n");
    printf("Território: %s\n", atacante->nome);
    printf("  Dono: %s\n", atacante->cor);
    printf("  Tropas: %d\n\n", atacante->tropas);
    
    printf("Território: %s\n", defensor->nome);
    printf("  Dono: %s\n", defensor->cor);
    printf("  Tropas: %d\n", defensor->tropas);
}

// exibirMapa():
// Exibe todos os territórios disponíveis no mapa com seus índices, donos e tropas.
// Utiliza const para garantir que apenas lê os dados sem modificá-los.
void exibirMapa(const Territorio* mapa, int quantidade) {
    printf("\n--- MAPA DO MUNDO DISPONÍVEL ---\n");
    for (int i = 0; i < quantidade; i++) {
        printf("[%d] %s - Dono: %s | Tropas: %d\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// atacar():
// Simula um ataque entre dois territórios usando números aleatórios como dados de batalha.
// Recebe dois ponteiros: atacante (quem está atacando) e defensor (quem está sendo atacado).
// O resultado do ataque depende de dados aleatórios (1-6) rolados por ambos os lados.
// O perdedor perde uma tropa, e a batalha continua até que um lado desista ou seja vencido.
// Se o atacante vencer, o território defensor muda de dono (cor do exército) e suas tropas são atualizadas.
void atacar(Territorio* atacante, Territorio* defensor) {
    if (atacante == NULL || defensor == NULL) {
        printf("Erro: Territórios inválidos para o ataque!\n");
        return;
    }
    
    if (atacante->tropas <= 0) {
        printf("Erro: %s não possui tropas para atacar!\n", atacante->nome);
        return;
    }
    
    if (defensor->tropas <= 0) {
        printf("Erro: %s não possui tropas para se defender!\n", defensor->nome);
        return;
    }
    
    printf("\n=============================================\n");
    printf("        SIMULAÇÃO DE BATALHA\n");
    printf("=============================================\n");
    printf("ATACANTE: %s (%s) - Tropas: %d\n", atacante->nome, atacante->cor, atacante->tropas);
    printf("DEFENSOR: %s (%s) - Tropas: %d\n\n", defensor->nome, defensor->cor, defensor->tropas);
    
    // Exibe o status inicial dos territórios
    exibirStatusTerritorios(atacante, defensor);
    
    int rodada = 1;
    
    while (atacante->tropas > 0 && defensor->tropas > 0) {
        printf("--- RODADA %d ---\n", rodada);
        
        // Gera números aleatórios entre 1 e 6 para simular dados
        int dadoAtacante = (rand() % 6) + 1;
        int dadoDefensor = (rand() % 6) + 1;
        
        printf("%s rolou: %d\n", atacante->nome, dadoAtacante);
        printf("%s rolou: %d\n", defensor->nome, dadoDefensor);
        
        // Compara os resultados
        if (dadoAtacante > dadoDefensor) {
            defensor->tropas--;
            printf("→ %s venceu a rodada! %s perde 1 tropa.\n", atacante->nome, defensor->nome);
        } else if (dadoDefensor > dadoAtacante) {
            atacante->tropas--;
            printf("→ %s venceu a rodada! %s perde 1 tropa.\n", defensor->nome, atacante->nome);
        } else {
            printf("→ Empate! Ninguém perde tropas.\n");
        }
        
        printf("Estado: %s (%d tropas) vs %s (%d tropas)\n\n", 
               atacante->nome, atacante->tropas, 
               defensor->nome, defensor->tropas);
        
        rodada++;
    }
    
    // Determina o vencedor e realiza as mudanças necessárias
    printf("=============================================\n");
    if (atacante->tropas > 0) {
        printf("✓ %s venceu a batalha e conquistou %s!\n", atacante->nome, defensor->nome);
        printf("  Tropas de %s: %d\n", atacante->nome, atacante->tropas);
        
        // Muda o dono do território defensor
        strcpy(defensor->cor, atacante->cor);
        
        // O território conquistado fica com 1 tropa do atacante (deixa uma guarnição)
        defensor->tropas = 1;
        
        // O atacante perde uma tropa por deixar no território conquistado
        atacante->tropas--;
        
        printf("\n→ Novo dono de %s: %s (cor: %s)\n", defensor->nome, atacante->nome, defensor->cor);
        printf("  Tropas em %s: %d (guarnição)\n", defensor->nome, defensor->tropas);
        printf("  Tropas restantes em %s: %d\n", atacante->nome, atacante->tropas);
    } else {
        printf("✓ %s manteve sua defesa e venceu a batalha!\n", defensor->nome);
        printf("  Tropas restantes: %d\n", defensor->tropas);
    }
    printf("=============================================\n");
    
    // Exibe o status final dos territórios após a batalha
    printf("\n");
    exibirStatusTerritorios(atacante, defensor);
    printf("\n");
}

// faseDeAtaque():
// Gerencia a seleção de territórios para ataque.
// Permite que o jogador escolha um território atacante e outro defensor.
// Valida as escolhas e executa o ataque entre os territórios selecionados.
void faseDeAtaque(Territorio* mapa, int quantidade) {
    int indiceAtacante, indiceDefensor;
    
    // Exibe o mapa disponível
    exibirMapa((const Territorio*)mapa, quantidade);
    
    // Seleciona o território atacante
    printf("\n--- SELEÇÃO DO TERRITÓRIO ATACANTE ---\n");
    printf("Digite o número do território que deseja atacar (1-%d): ", quantidade);
    scanf("%d", &indiceAtacante);
    limparBufferEntrada();
    
    // Validação do território atacante
    if (indiceAtacante < 1 || indiceAtacante > quantidade) {
        printf("Erro: Número de território inválido!\n");
        return;
    }
    
    indiceAtacante--; // Converte para índice baseado em 0
    
    if (mapa[indiceAtacante].tropas <= 0) {
        printf("Erro: O território '%s' não possui tropas para atacar!\n", mapa[indiceAtacante].nome);
        return;
    }
    
    // Seleciona o território defensor
    printf("\n--- SELEÇÃO DO TERRITÓRIO DEFENSOR ---\n");
    printf("Digite o número do território a defender (1-%d): ", quantidade);
    scanf("%d", &indiceDefensor);
    limparBufferEntrada();
    
    // Validação do território defensor
    if (indiceDefensor < 1 || indiceDefensor > quantidade) {
        printf("Erro: Número de território inválido!\n");
        return;
    }
    
    indiceDefensor--; // Converte para índice baseado em 0
    
    // Verifica se o jogador selecionou o mesmo território
    if (indiceAtacante == indiceDefensor) {
        printf("Erro: Um território não pode atacar a si mesmo!\n");
        return;
    }
    
    if (mapa[indiceDefensor].tropas <= 0) {
        printf("Erro: O território '%s' não possui tropas para se defender!\n", mapa[indiceDefensor].nome);
        return;
    }
    
    // Executa o ataque entre os territórios selecionados
    printf("\n");
    atacar(&mapa[indiceAtacante], &mapa[indiceDefensor]);
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}