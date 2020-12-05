/**********************************************************************/
/*                              tempo.c                               */
/*                                                                    */
/* Autor: Felipe Yudi Miyoshi Nakamoto                                */
/* Disciplina: Sistemas Distribuídos (UFPR - Ciência da Computação)   */
/* Professor: Elias P. Duarte Jr.                                     */
/* Data da Última Modificação: 26/11/2020                             */
/* Descrição: Nosso primeiro programa de simulação. Vamos simular N   */
/* nodos, cada um conta o "tempo" independentemente. Um Exemplo       */
/* simples e significativo para captar o "espírito" da simulação      */
/*                                                                    */
/**********************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "smpl.h"

// Vamos definir os EVENTOS
#define test 1
#define fault 2
#define recovery 3

/* Vamos definir o descritor do processo */
typedef struct {
  int id;  // identificador de facility do SMPL
           // outras variáveis locais dos processos são declaradas aqui!
} TipoProcesso;

TipoProcesso *processo;

/* log_header(int n)
   n: numero de processos na execução
 * Imprime o cabeçalho inicial para o log do programa */
void log_header(int n) {
  printf("**********************************************************\n");
  printf("*                Início do Log - tempo.c                 *\n");
  printf("**********************************************************\n");
  printf("*  Número de processos: %d                                *\n", n);
  printf("**********************************************************\n\n");
}

/* log_footer()
 * Imprime o rodapé final para o log do programa */
void log_footer() {
  printf("\n**********************************************************\n");
  printf("*                Fim do Log - tempo.c                    *\n");
  printf("**********************************************************\n");
}

int main(int argc, char *argv[]) {
  static int N;      // número de processos
  static int token;  // indica o processo que está executando
  static int event, r, i;
  static char fa_name[5];

  if (argc != 2) {
    puts("Uso correto: tempo <número de processos>");
    exit(1);
  }

  N = atoi(argv[1]);

  smpl(0, "Um Exemplo de Simulação");
  reset();
  stream(1);
  log_header(N);

  // inicializar processos
  processo = (TipoProcesso *)malloc(sizeof(TipoProcesso) * N);

  for (i = 0; i < N; i++) {
    memset(fa_name, '\0', 5);
    sprintf(fa_name, "%d", i);
    // facility é a unidade que vai ser simulada
    processo[i].id = facility(fa_name, 1);
  }

  // vamos fazer o escalonamento inicial de eventos

  // nossos processos vão executar testes em intervalos de testes
  // o intervalo de testes vai ser de 30 unidades de tempo
  // a simulação começa no tempo 0 (zero) e vamos escalonar o primeiro teste de
  // todos os processos para o tempo 30.0
  for (i = 0; i < N; i++) {
    schedule(test, 30.0, i);
  }
  schedule(fault, 31.0, 1);
  schedule(recovery, 61.0, 1);

  // agora vem o loop principal do simulador
  while (time() < 150.0) {
    cause(&event, &token);
    switch (event) {
      case test:
        // se o processo está falho, não testa
        if (status(processo[token].id) != 0) {
          break;
        }
        printf(" [INFO] O processo %d vai testar no tempo %4.1f\n", token,
               time());
        schedule(test, 30.0, token);
        break;
      case fault:
        // reserva a facility, configura o status como diferente de 0
        r = request(processo[token].id, token, 0);
        if (r != 0) {
          puts(" [ERROR] Não foi possível falhar o processo.");
          exit(1);
        }
        printf(" [INFO] O processo %d falhou no tempo %4.1f\n", token, time());
        break;
      case recovery:
        // libera a facility, configura o status novamente como 0
        release(processo[token].id, token);
        printf(" [INFO] O processo %d recuperou no tempo %4.1f\n", token,
               time());
        schedule(test, 30.0, token);
        break;
    }
  }
  log_footer();
  return 0;
}