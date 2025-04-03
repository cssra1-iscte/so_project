/****************************************************************************************
 ** ISCTE-IUL: Trabalho prático 2 de Sistemas Operativos 2024/2025, Enunciado Versão 2+
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
 * @brief Processamento do processo Cliente
 */
int main () {
    // Exemplos de chamadas às funções do Cliente:
    // c1_IniciaCliente();
    // c1_1_ValidaFifoServidor(FILE_REQUESTS);
    // c1_3_ArmaSinaisCliente();
    // c2_CheckinCliente();
    // c2_1_InputEstacionamento(&clientRequest);
    // FILE *fFifoServidor;
    // c2_2_AbreFifoServidor(FILE_REQUESTS, &fFifoServidor);
    // c2_3_EscrevePedido(fFifoServidor, clientRequest);
    // c3_ProgramaAlarme(MAX_ESPERA);
    // c4_EsperaRespostaServidor();
    // c4_1_DesligaAlarme();
    // c4_2_InputEsperaCheckout();
    // c5_EncerraCliente();
    // c5_1_EnviaSigusr1AoServidor(clientRequest);
    // c5_2_EsperaRespostaServidorETermina();
}

/**
 * @brief  c1_IniciaCliente    Ler a descrição da tarefa C1 no enunciado
 * @return Success  (TRUE or FALSE)
 */
int c1_IniciaCliente() {
    int success = FALSE;
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  c1_1_ValidaFifoServidor      Ler a descrição da tarefa C1.1 no enunciado
 * @param  filenameFifoServidor (I) O nome do FIFO do servidor (i.e., FILE_REQUESTS)
 * @return Success  (TRUE or FALSE)
 */
int c1_1_ValidaFifoServidor(char *filenameFifoServidor) {
    int success = FALSE;
    so_debug("< [@param filenameFifoServidor:%s]", filenameFifoServidor);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  c1_3_ArmaSinaisCliente      Ler a descrição da tarefa C1.3 no enunciado
 * @return Success  (TRUE or FALSE)
 */
int c1_3_ArmaSinaisCliente() {
    int success = FALSE;
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  c2_CheckinCliente      Ler a descrição da tarefa C2 no enunciado
 * @return Success  (TRUE or FALSE)
 */
int c2_CheckinCliente() {
    int success = FALSE;
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  c2_1_InputEstacionamento      Ler a descrição da tarefa C2.1 no enunciado
 * @param  pclientRequest (O) pedido a ser enviado por este Cliente ao Servidor
 * @return Success  (TRUE or FALSE)
 */
int c2_1_InputEstacionamento(Estacionamento *pclientRequest) {
    int success = FALSE;
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug("> [*pclientRequest:[%s:%s:%c:%s:%d:%d]]", pclientRequest->viatura.matricula, pclientRequest->viatura.pais, pclientRequest->viatura.categoria, pclientRequest->viatura.nomeCondutor, pclientRequest->pidCliente, pclientRequest->pidServidorDedicado);
    return success;
}

/**
 * @brief  c2_2_AbreFifoServidor      Ler a descrição da tarefa C2.2 no enunciado
 * @param  filenameFifoServidor (I) O nome do FIFO do servidor (i.e., FILE_REQUESTS)
 * @param  pfFifoServidor (O) descritor aberto do ficheiro do FIFO do servidor
 * @return Success  (TRUE or FALSE)
 */
int c2_2_AbreFifoServidor(char *filenameFifoServidor, FILE **pfFifoServidor) {
    int success = FALSE;
    so_debug("< [@param filenameFifoServidor:%s]", filenameFifoServidor);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug("> [*pfFifoServidor:%p]", *pfFifoServidor);
    return success;
}

/**
 * @brief  c2_3_EscrevePedido      Ler a descrição da tarefa C2.3 no enunciado
 * @param  fFifoServidor (I) descritor aberto do ficheiro do FIFO do servidor
 * @param  clientRequest (I) pedido a ser enviado por este Cliente ao Servidor
 * @return Success  (TRUE or FALSE)
 */
int c2_3_EscrevePedido(FILE *fFifoServidor, Estacionamento clientRequest) {
    int success = FALSE;
    so_debug("< [@param fFifoServidor:%p, clientRequest:[%s:%s:%c:%s:%d:%d]]", fFifoServidor, clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  c3_ProgramaAlarme      Ler a descrição da tarefa C3 no enunciado
 * @param  segundos (I) número de segundos a programar no alarme
 * @return Success  (TRUE or FALSE)
 */
int c3_ProgramaAlarme(int segundos) {
    int success = FALSE;
    so_debug("< [@param segundos:%d]", segundos);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  c4_EsperaRespostaServidor      Ler a descrição da tarefa C4 no enunciado
 * @return Success  (TRUE or FALSE)
 */
int c4_EsperaRespostaServidor() {
    int success = FALSE;
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  c4_1_DesligaAlarme      Ler a descrição da tarefa C4.1 no enunciado
 * @return Success  (TRUE or FALSE)
 */
int c4_1_DesligaAlarme() {
    int success = FALSE;
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  c4_2_InputEsperaCheckout      Ler a descrição da tarefa C4.2 no enunciado
 * @return Success  (TRUE or FALSE)
 */
int c4_2_InputEsperaCheckout() {
    int success = FALSE;
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  c5_EncerraCliente      Ler a descrição da tarefa C5 no enunciado
 * @return Success  (TRUE or FALSE)
 */
int c5_EncerraCliente() {
    int success = FALSE;
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  c5_1_EnviaSigusr1AoServidor      Ler a descrição da tarefa C5.1 no enunciado
 * @param  clientRequest (I) pedido a ser enviado por este Cliente ao Servidor
 * @return Success  (TRUE or FALSE)
 */
int c5_1_EnviaSigusr1AoServidor(Estacionamento clientRequest) {
    int success = FALSE;
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  c5_2_EsperaRespostaServidorETermina      Ler a descrição da tarefa C5.2 no enunciado
 * @return Success  (TRUE or FALSE)
 */
int c5_2_EsperaRespostaServidorETermina() {
    int success = FALSE;
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  c6_TrataSigusr1      Ler a descrição da tarefa C6 no enunciado
 * @param  sinalRecebido (I) número do sinal que é recebido por esta função (enviado pelo SO)
 */
void c6_TrataSigusr1(int sinalRecebido) {
    so_debug("< [@param sinalRecebido:%d]", sinalRecebido);

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
 * @return Success  (TRUE or FALSE)
 */
void c9_TrataAlarme(int sinalRecebido) {
    so_debug("< [@param sinalRecebido:%d]", sinalRecebido);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}