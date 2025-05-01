/****************************************************************************************
 ** ISCTE-IUL: Trabalho prático 2 de Sistemas Operativos 2024/2025, Enunciado Versão 4+
 **
 ** Aluno: Nº:       Nome: 
 ** Nome do Módulo: cliente.c
 ** Descrição/Explicação do Módulo:
 **
 **
 ***************************************************************************************/

// #define SO_HIDE_DEBUG                // Uncomment this line to hide all @DEBUG statements
#include "common.h"

/*** Variáveis Globais ***/
Estacionamento clientRequest;           // Pedido enviado do Cliente para o Servidor
int recebeuRespostaServidor = FALSE;    // Variável que determina se o Cliente já recebeu uma resposta do Servidor

/**
 * @brief  Processamento do processo Cliente.
 *         OS ALUNOS NÃO DEVERÃO ALTERAR ESTA FUNÇÃO.
 */
int main () {
    so_debug("<");

    // c1_IniciaCliente:
    c1_1_ValidaFifoServidor(FILE_REQUESTS);
    c1_2_ArmaSinaisCliente();

    // c2_CheckinCliente:
    c2_1_InputEstacionamento(&clientRequest);
    FILE *fFifoServidor;
    c2_2_AbreFifoServidor(FILE_REQUESTS, &fFifoServidor);
    c2_3_EscrevePedido(fFifoServidor, clientRequest);

    c3_ProgramaAlarme(MAX_ESPERA);

    // c4_AguardaRespostaServidor:
    c4_1_EsperaRespostaServidor();
    c4_2_DesligaAlarme();
    c4_3_InputEsperaCheckout();

    c5_EncerraCliente();

    so_error("Cliente", "O programa nunca deveria ter chegado a este ponto!");
    so_debug(">");
}

/**
 * @brief  c1_1_ValidaFifoServidor Ler a descrição da tarefa C1.1 no enunciado
 * @param  filenameFifoServidor (I) O nome do FIFO do servidor (i.e., FILE_REQUESTS)
 */
void c1_1_ValidaFifoServidor(char *filenameFifoServidor) {
    so_debug("< [@param filenameFifoServidor:%s]", filenameFifoServidor);

    struct stat st;
    // verifica se o ficheiro já existe
    if (stat(filenameFifoServidor, &st) == 0) {
        // verifica se, existindo, se é de organização FIFO
        if (!S_ISFIFO(st.st_mode)) {
            so_error("C1.1", "O ficheiro existe mas não é do TIPO FIFO");
            exit(1);
        }
        
        so_success("C1.1", "Ficheiro FIFO existe");

    } else {
        so_error("C1.1", "Ficheiro não existe. É necessário correr o servidor primeiro");
        exit(1);
    }
    

    so_debug(">");
}

/**
 * @brief  c1_2_ArmaSinaisCliente Ler a descrição da tarefa C1.3 no enunciado
 */
void c1_2_ArmaSinaisCliente() {
    so_debug("<");

    // arma sinal SIGUSR1 - serve para o Servidor Dedicado indicar que o check-in foi concluído com sucesso
    //não dá para usar o signal porque o manipulador de sinal tem que ter a assinatura estendida
    struct sigaction sa;
    sa.sa_sigaction = c6_TrataSigusr1; // Define o manipulador de sinal
    sa.sa_flags = SA_SIGINFO;          // Indica que o manipulador usa a assinatura estendida
    sigemptyset(&sa.sa_mask);          // Limpa a máscara de sinais
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        so_error("C1.2", "Erro ao armar SIGUSR1");
        exit(1);
    }

    // Arma sinal SIGHUP - Check-in não teve sucesso ou estacionamento terminou
    if (signal(SIGHUP, c7_TrataSighup) == SIG_ERR) {
        so_error("C1.2", "Erro ao armar SIGHUP");
        exit(1);
    }

    // Arma sinal SIGINT - Atalho <CTRL+C>
    if (signal(SIGINT, c8_TrataCtrlC) == SIG_ERR) {
        so_error("C1.2", "Erro ao armar SIGINT");
        exit(1);
    }

    // Arma sinal SIGALRM - Timeout no check-in
    if (signal(SIGALRM, c9_TrataAlarme) == SIG_ERR) {
        so_error("C1.2", "Erro ao armar SIGALRM");
        exit(1);
    }

    so_success("C1.2", "Armação de sinais com sucesso");

    so_debug(">");
}

/**
 * @brief  c2_1_InputEstacionamento Ler a descrição da tarefa C2.1 no enunciado
 * @param  pclientRequest (O) pedido a ser enviado por este Cliente ao Servidor
 */
void c2_1_InputEstacionamento(Estacionamento *pclientRequest) {
    so_debug("<");

    printf("Park-IUL: Check-in Viatura\n----------------------------\n");

    // Matrícula
    printf("Introduza a matrícula da viatura: ");
    so_gets(pclientRequest->viatura.matricula, sizeof(pclientRequest->viatura.matricula));
    if (strlen(pclientRequest->viatura.matricula) == 0 || strspn(pclientRequest->viatura.matricula, " ") == strlen(pclientRequest->viatura.matricula)) {
        so_error("C2.1", "Campo Matrícula inválido");
        exit(1);
    }

    // País
    printf("Introduza o país da viatura: ");
    so_gets(pclientRequest->viatura.pais, sizeof(pclientRequest->viatura.pais));
    if (strlen(pclientRequest->viatura.pais) == 0 || strspn(pclientRequest->viatura.pais, " ") == strlen(pclientRequest->viatura.pais)) {
        so_error("C2.1", "Campo País inválido");
        exit(1);
    }

    // Categoria
    char categoriaTemp[4];
    printf("Introduza a categoria da viatura: ");
    so_gets(categoriaTemp, sizeof(categoriaTemp));
    if (strlen(categoriaTemp) == 0 || strspn(categoriaTemp, " ") == strlen(categoriaTemp)) {
        so_error("C2.1", "Campo Categoria inválido");
        exit(1);
    }
    pclientRequest->viatura.categoria = categoriaTemp[0];

    // Nome do condutor
    printf("Introduza o nome do condutor: ");
    so_gets(pclientRequest->viatura.nomeCondutor, sizeof(pclientRequest->viatura.nomeCondutor));
    if (strlen(pclientRequest->viatura.nomeCondutor) == 0 || strspn(pclientRequest->viatura.nomeCondutor, " ") == strlen(pclientRequest->viatura.nomeCondutor)) {
        so_error("C2.1", "Campo Nome do Condutor inválido");
        exit(1);
    }

    // Preenchimento dos campos adicionais
    pclientRequest->pidCliente = getpid();
    pclientRequest->pidServidorDedicado = -1;

    // Sucesso
    so_success("C2.1", "%s %s %c %s %d %d",
        pclientRequest->viatura.matricula,
        pclientRequest->viatura.pais,
        pclientRequest->viatura.categoria,
        pclientRequest->viatura.nomeCondutor,
        pclientRequest->pidCliente,
        pclientRequest->pidServidorDedicado
    );

    so_debug("> [*pclientRequest:[%s:%s:%c:%s:%d:%d]]", pclientRequest->viatura.matricula, pclientRequest->viatura.pais, pclientRequest->viatura.categoria, pclientRequest->viatura.nomeCondutor, pclientRequest->pidCliente, pclientRequest->pidServidorDedicado);
}

/**
 * @brief  c2_2_AbreFifoServidor Ler a descrição da tarefa C2.2 no enunciado
 * @param  filenameFifoServidor (I) O nome do FIFO do servidor (i.e., FILE_REQUESTS)
 * @param  pfFifoServidor (O) descritor aberto do ficheiro do FIFO do cleinye
 */
void c2_2_AbreFifoServidor(char *filenameFifoServidor, FILE **pfFifoServidor) {
    so_debug("< [@param filenameFifoServidor:%s]", filenameFifoServidor);

    while (1) {
        *pfFifoServidor = fopen(filenameFifoServidor, "wb");

        if (*pfFifoServidor != NULL) {
            // Sucesso na abertura
            so_success("C2.2", "Abertura de FIFO em modo ESCRITA com sucesso");
            break; //sai do ciclo while
        }

        // falha na abertura do FIFO em modo leitura - recebe um sinal antes de a operação terminar (Interrupted system call)
        if (errno == EINTR) {
            so_debug("C2.2", "Abertura interrompida por sinal, tentativa repetida.");
            continue; //recomeça o ciclo while -> tenta abrir o FIFO de novo
        }

        // *pfFifoServidor == NULL -> erro ao abrir o FIFO
        so_error("C2.2", "Erro ao abrir FIFO Servidor: %s", strerror(errno));
        exit(1);
    }

    so_debug("> [*pfFifoServidor:%p]", *pfFifoServidor);
}

/**
 * @brief  c2_3_EscrevePedido Ler a descrição da tarefa C2.3 no enunciado
 * @param  fFifoServidor (I) descritor aberto do ficheiro do FIFO do servidor
 * @param  clientRequest (I) pedido a ser enviado por este Cliente ao Servidor
 */
void c2_3_EscrevePedido(FILE *fFifoServidor, Estacionamento clientRequest) {
    so_debug("< [@param fFifoServidor:%p, clientRequest:[%s:%s:%c:%s:%d:%d]]", fFifoServidor, clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado);

    // Tenta escrever no FIFO
    size_t elementosEscritos = fwrite(&clientRequest, sizeof(Estacionamento), 1, fFifoServidor);

    // Fecha o FIFO 
    fclose(fFifoServidor);

    // Valida a escrita
    if (elementosEscritos != 1) {
        so_error("C2.3", "Erro ao escrever no FIFO do servidor");
        exit(1);
    } else {
        so_success("C2.3", "FIFO ecsrito com sucesso");
    }

    so_debug(">");
}

/**
 * @brief  c3_ProgramaAlarme Ler a descrição da tarefa C3 no enunciado
 * @param  segundos (I) número de segundos a programar no alarme
 */
void c3_ProgramaAlarme(int segundos) {
    so_debug("< [@param segundos:%d]", segundos);

    alarm(segundos); // Define alarme
    so_success("C3", "Espera resposta em %d segundos", segundos);

    so_debug(">");
}

/**
 * @brief  c4_1_EsperaRespostaServidor Ler a descrição da tarefa C4 no enunciado
 */
void c4_1_EsperaRespostaServidor() {
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  c4_2_DesligaAlarme Ler a descrição da tarefa C4.1 no enunciado
 */
void c4_2_DesligaAlarme() {
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  c4_3_InputEsperaCheckout Ler a descrição da tarefa C4.2 no enunciado
 */
void c4_3_InputEsperaCheckout() {
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  c5_EncerraCliente      Ler a descrição da tarefa C5 no enunciado
 */
void c5_EncerraCliente() {
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  c5_1_EnviaSigusr1AoServidor      Ler a descrição da tarefa C5.1 no enunciado
 * @param  clientRequest (I) pedido a ser enviado por este Cliente ao Servidor
 */
void c5_1_EnviaSigusr1AoServidor(Estacionamento clientRequest) {
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  c5_2_EsperaRespostaServidorETermina      Ler a descrição da tarefa C5.2 no enunciado
 */
void c5_2_EsperaRespostaServidorETermina() {
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  c6_TrataSigusr1 Ler a descrição da tarefa C6 no enunciado
 * @param  sinalRecebido (I) número do sinal que é recebido por esta função (enviado pelo SO)
 * @param  siginfo (I) informação sobre o sinal
 * @param  context (I) contexto em que o sinal foi chamado
 */
 void c6_TrataSigusr1(int sinalRecebido, siginfo_t *siginfo, void *context) {
    so_debug("< [@param sinalRecebido:%d, siginfo:%p, context:%p]", sinalRecebido, siginfo, context);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  c7_TrataSighup      Ler a descrição da tarefa C7 no enunciado
 * @param  sinalRecebido (I) número do sinal que é recebido por esta função (enviado pelo SO)
 */
void c7_TrataSighup(int sinalRecebido) {
    so_debug("< [@param sinalRecebido:%d]", sinalRecebido);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  c8_TrataCtrlC      Ler a descrição da tarefa c8 no enunciado
 * @param  sinalRecebido (I) número do sinal que é recebido por esta função (enviado pelo SO)
 */
void c8_TrataCtrlC(int sinalRecebido) {
    so_debug("< [@param sinalRecebido:%d]", sinalRecebido);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  c9_TrataAlarme      Ler a descrição da tarefa c9 no enunciado
 * @param  sinalRecebido (I) número do sinal que é recebido por esta função (enviado pelo SO)
 */
void c9_TrataAlarme(int sinalRecebido) {
    so_debug("< [@param sinalRecebido:%d]", sinalRecebido);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}