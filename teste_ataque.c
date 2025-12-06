// Programa de teste para a função atacar()
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STRING_NOME 30
#define MAX_STRING_COR 10

typedef struct {
    char nome[MAX_STRING_NOME];
    char cor[MAX_STRING_COR];
    int tropas;
} Territorio;

// Protótipo da função
void atacar(Territorio* atacante, Territorio* defensor);
void exibirStatusTerritorios(Territorio* atacante, Territorio* defensor);

// Implementação da função exibirStatusTerritorios
void exibirStatusTerritorios(Territorio* atacante, Territorio* defensor) {
    printf("\n--- STATUS DOS TERRITÓRIOS ---\n");
    printf("Território: %s\n", atacante->nome);
    printf("  Dono: %s\n", atacante->cor);
    printf("  Tropas: %d\n\n", atacante->tropas);
    
    printf("Território: %s\n", defensor->nome);
    printf("  Dono: %s\n", defensor->cor);
    printf("  Tropas: %d\n", defensor->tropas);
}

// Implementação da função atacar
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

int main() {
    srand(time(NULL));
    
    // Cria dois territórios para teste
    Territorio territorio1;
    strcpy(territorio1.nome, "Reino da Luz");
    strcpy(territorio1.cor, "Amarelo");
    territorio1.tropas = 10;
    
    Territorio territorio2;
    strcpy(territorio2.nome, "Reino das Sombras");
    strcpy(territorio2.cor, "Preto");
    territorio2.tropas = 8;
    
    printf("===========================================\n");
    printf("TESTE DA FUNÇÃO ATACAR()\n");
    printf("===========================================\n\n");
    
    // Realiza o ataque
    atacar(&territorio1, &territorio2);
    
    printf("Estado final:\n");
    printf("- %s (cor: %s): %d tropas\n", territorio1.nome, territorio1.cor, territorio1.tropas);
    printf("- %s (cor: %s): %d tropas\n", territorio2.nome, territorio2.cor, territorio2.tropas);
    
    return 0;
}
