/****************************************************************************************
 ** ISCTE-IUL: Trabalho prático 2 de Sistemas Operativos 2024/2025, Enunciado Versão 2+
 **
 ** Aluno: Nº:       Nome: 
 ** Nome do Módulo: servidor.c
 ** Descrição/Explicação do Módulo:
 **
 **
 ***************************************************************************************/

// #define SO_HIDE_DEBUG                // Uncomment this line to hide all @DEBUG statements
#include "common.h"

/*** Variáveis Globais ***/
Estacionamento clientRequest;           // Pedido enviado do Cliente para o Servidor
Estacionamento *lugaresEstacionamento;  // Array de Lugares de Estacionamento do parque
int dimensaoMaximaParque;               // Dimensão Máxima do parque (BD), recebida por argumento do programa
int indexClienteBD;                     // Índice do cliente que fez o pedido ao servidor/servidor dedicado na BD
long posicaoLogfile;                    // Posição no ficheiro Logfile para escrever o log da entrada corrente
LogItem logItem;                        // Informação da entrada corrente a escrever no logfile

/**
 * @brief Processamento do processo Servidor e dos processos Servidor Dedicado
 */
int main () {
    // Exemplos de chamadas às funções do Servidor e do Servidor Dedicado:
    // s1_IniciaServidor();
    // s1_2_CriaBD(dimensaoMaximaParque, &lugaresEstacionamento);
    // s1_3_ArmaSinaisServidor();
    // s1_4_CriaFifoServidor(FILE_REQUESTS);
    // s2_MainServidor();
    // FILE *fFifoServidor;
    // s2_1_AbreFifoServidor(FILE_REQUESTS, &fFifoServidor);
    // s2_2_LePedidosFifoServidor(fFifoServidor);
    // s2_2_1_LePedido(fFifoServidor, &clientRequest);
    // s2_2_2_ProcuraLugarDisponivelBD(clientRequest, lugaresEstacionamento, dimensaoMaximaParque, &indexClienteBD);
    // s2_2_3_CriaServidorDedicado(lugaresEstacionamento, indexClienteBD);
    // s4_EncerraServidor(FILE_REQUESTS);
    // sd7_IniciaServidorDedicado();
    // sd7_1_ArmaSinaisServidorDedicado();
    // sd7_2_ValidaPidCliente(clientRequest);
    // sd7_3_ValidaLugarDisponivelBD(indexClienteBD);
    // sd8_ValidaPedidoCliente(clientRequest);
    // sd8_1_ValidaMatricula(clientRequest);
    // sd8_2_ValidaPais(clientRequest);
    // sd8_3_ValidaCategoria(clientRequest);
    // sd8_4_ValidaNomeCondutor(clientRequest);
    // sd9_EntradaCliente();
    // sd9_1_AdormeceTempoRandom();
    // sd9_2_EnviaSigusr1AoCliente(clientRequest);
    // sd9_3_EscreveLogEntradaViatura(FILE_LOGFILE, clientRequest, &posicaoLogfile, &logItem);
    // sd10_AguardaCheckout();
    // sd10_2_EscreveLogSaidaViatura(FILE_LOGFILE, posicaoLogfile, logItem);
    // sd11_EncerraServidorDedicado();
    // sd11_1_LibertaLugarViatura(lugaresEstacionamento, indexClienteBD);
    // sd11_2_EnviaSighupAoClienteETermina(clientRequest);
}

/**
 * @brief  s1_IniciaServidor    Ler a descrição da tarefa S1 no enunciado
 * @return Success  (TRUE or FALSE)
 */
int s1_IniciaServidor() {
    int success = FALSE;
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  s1_2_CriaBD      Ler a descrição da tarefa S1.2 no enunciado
 * @param  dimensaoMaximaParque (I) número máximo de lugares do parque, especificado pelo utilizador
 * @param  plugaresEstacionamento (O) array de lugares de estacionamento que irá servir de BD
 * @return Success  (TRUE or FALSE)
 */
int s1_2_CriaBD(int dimensaoMaximaParque, Estacionamento **plugaresEstacionamento) {
    int success = FALSE;
    so_debug("< [@param dimensaoMaximaParque:%d]", dimensaoMaximaParque);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug("> [*plugaresEstacionamento:%p]", *plugaresEstacionamento);
    return success;
}

/**
 * @brief  s1_3_ArmaSinaisServidor  Ler a descrição da tarefa S1.3 no enunciado
 * @return Success  (TRUE or FALSE)
 */
int s1_3_ArmaSinaisServidor() {
    int success = FALSE;
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  s1_4_CriaFifoServidor    Ler a descrição da tarefa S1.4 no enunciado
 * @param  filenameFifoServidor (I) O nome do FIFO do servidor (i.e., FILE_REQUESTS)
 * @return Success  (TRUE or FALSE)
 */
int s1_4_CriaFifoServidor(char *filenameFifoServidor) {
    int success = FALSE;
    so_debug("< [@param filenameFifoServidor:%s]", filenameFifoServidor);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  s2_MainServidor      Ler a descrição da taref S2 no enunciado
 * @return Success  (TRUE or FALSE)a
 */
int s2_MainServidor() {
    int success = FALSE;
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  s2_1_AbreFifoServidor    Ler a descrição da tarefa S2.1 no enunciado
 * @param  filenameFifoServidor (I) O nome do FIFO do servidor (i.e., FILE_REQUESTS)
 * @param  pfFifoServidor (O) descritor aberto do ficheiro do FIFO do servidor
 * @return Success  (TRUE or FALSE)
 */
int s2_1_AbreFifoServidor(char *filenameFifoServidor, FILE **pfFifoServidor) {
    int success = FALSE;
    so_debug("< [@param filenameFifoServidor:%s]", filenameFifoServidor);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug("> [*pfFifoServidor:%p]", *pfFifoServidor);
    return success;
}

/**
 * @brief  s2_2_LePedidosFifoServidor    Ler a descrição da tarefa S2.2 no enunciado
 * @param  fFifoServidor (I) descritor aberto do ficheiro do FIFO do servidor
 * @return Success  (TRUE or FALSE)
 */
int s2_2_LePedidosFifoServidor(FILE *fFifoServidor) {
    int success = FALSE;
    so_debug("< [@param fFifoServidor:%p]", fFifoServidor);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  s2_2_1_LePedido    Ler a descrição da tarefa S2.2.1 no enunciado
 * @param  fFifoServidor (I) descritor aberto do ficheiro do FIFO do servidor
 * @param  pclientRequest (O) pedido recebido, enviado por um Cliente
 * @return Success  (TRUE or FALSE)
 */
int s2_2_1_LePedido(FILE *fFifoServidor, Estacionamento *pclientRequest) {
    int success = FALSE;
    so_debug("< [@param fFifoServidor:%p]", fFifoServidor);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug("> [*pclientRequest:[%s:%s:%c:%s:%d.%d]]", pclientRequest->viatura.matricula, pclientRequest->viatura.pais, pclientRequest->viatura.categoria, pclientRequest->viatura.nomeCondutor, pclientRequest->pidCliente, pclientRequest->pidServidorDedicado);
    return success;
}

/**
 * @brief  s2_2_2_ProcuraLugarDisponivelBD  Ler a descrição da tarefa S2.2.2 no enunciado
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 * @param  lugaresEstacionamento (I) array de lugares de estacionamento que irá servir de BD
 * @param  dimensaoMaximaParque (I) número máximo de lugares do parque, especificado pelo utilizador
 * @param  pindexClienteBD (O) índice do lugar correspondente a este pedido na BD (>= 0), ou -1 se não houve nenhum lugar disponível
 * @return Success  (TRUE or FALSE)
 */
int s2_2_2_ProcuraLugarDisponivelBD(Estacionamento clientRequest, Estacionamento *lugaresEstacionamento, int dimensaoMaximaParque, int *pindexClienteBD) {
    int success = FALSE;
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d], lugaresEstacionamento:%p, dimensaoMaximaParque:%d]", clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado, lugaresEstacionamento, dimensaoMaximaParque);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug("> [*pindexClienteBD:%d]", *pindexClienteBD);
    return success;
}

/**
 * @brief  s2_2_3_CriaServidorDedicado    Ler a descrição da tarefa S2.2.3 no enunciado
 * @param  lugaresEstacionamento (I) array de lugares de estacionamento que irá servir de BD
 * @param  indexClienteBD (I) índice do lugar correspondente a este pedido na BD (>= 0), ou -1 se não houve nenhum lugar disponível
 * @return Success  (TRUE or FALSE)
 */
int s2_2_3_CriaServidorDedicado(Estacionamento *lugaresEstacionamento, int indexClienteBD) {
    int success = FALSE;
    so_debug("< [@param lugaresEstacionamento:%p, indexClienteBD:%d]", lugaresEstacionamento, indexClienteBD);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  s3_TrataCtrlC    Ler a descrição da tarefa S3 no enunciado
 * @param  sinalRecebido (I) número do sinal que é recebido por esta função (enviado pelo SO)
 */
void s3_TrataCtrlC(int sinalRecebido) {
    so_debug("< [@param sinalRecebido:%d]", sinalRecebido);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  s4_EncerraServidor    Ler a descrição da tarefa S4 no enunciado
 * @param  filenameFifoServidor (I) O nome do FIFO do servidor (i.e., FILE_REQUESTS)
 * @return Success  (TRUE or FALSE)
 */
int s4_EncerraServidor(char *filenameFifoServidor) {
    int success = FALSE;
    so_debug("< [@param filenameFifoServidor:%s]", filenameFifoServidor);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  s5_TrataTerminouServidorDedicado    Ler a descrição da tarefa S5 no enunciado
 * @param  sinalRecebido (I) número do sinal que é recebido por esta função (enviado pelo SO)
 */
void s5_TrataTerminouServidorDedicado(int sinalRecebido) {
    so_debug("< [@param sinalRecebido:%d]", sinalRecebido);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  sd7_IniciaServidorDedicado    Ler a descrição da tarefa SD7 no enunciado
 * @return Success  (TRUE or FALSE)
 */
int sd7_IniciaServidorDedicado() {
    int success = FALSE;
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  sd7_1_ArmaSinaisServidorDedicado    Ler a descrição da tarefa SD7.1 no enunciado
 * @return Success  (TRUE or FALSE)
 */
int sd7_1_ArmaSinaisServidorDedicado() {
    int success = FALSE;
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  sd7_2_ValidaPidCliente    Ler a descrição da tarefa SD7.2 no enunciado
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 * @return Success  (TRUE or FALSE)
 */
int sd7_2_ValidaPidCliente(Estacionamento clientRequest) {
    int success = FALSE;
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  sd7_3_ValidaLugarDisponivelBD    Ler a descrição da tarefa SD7.3 no enunciado
 * @param  indexClienteBD (I) índice do lugar correspondente a este pedido na BD (>= 0), ou -1 se não houve nenhum lugar disponível
 * @return Success  (TRUE or FALSE)
 */
int sd7_3_ValidaLugarDisponivelBD(int indexClienteBD) {
    int success = FALSE;
    so_debug("< [@param indexClienteBD:%d]", indexClienteBD);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  sd8_ValidaPedidoCliente    Ler a descrição da tarefa SD8 no enunciado
 * @return Success  (TRUE or FALSE)
 */
int sd8_ValidaPedidoCliente() {
    int success = FALSE;
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  sd8_1_ValidaMatricula    Ler a descrição da tarefa SD8.1 no enunciado
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 * @return Success  (TRUE or FALSE)
 */
int sd8_1_ValidaMatricula(Estacionamento clientRequest) {
    int success = FALSE;
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  sd8_2_ValidaPais    Ler a descrição da tarefa SD8.2 no enunciado
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 * @return Success  (TRUE or FALSE)
 */
int sd8_2_ValidaPais(Estacionamento clientRequest) {
    int success = FALSE;
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  sd8_3_ValidaCategoria    Ler a descrição da tarefa SD8.3 no enunciado
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 * @return Success  (TRUE or FALSE)
 */
int sd8_3_ValidaCategoria(Estacionamento clientRequest) {
    int success = FALSE;
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  sd8_4_ValidaNomeCondutor    Ler a descrição da tarefa SD8.4 no enunciado
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 * @return Success  (TRUE or FALSE)
 */
int sd8_4_ValidaNomeCondutor(Estacionamento clientRequest) {
    int success = FALSE;
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  sd9_EntradaCliente    Ler a descrição da tarefa SD9 no enunciado
 * @return Success  (TRUE or FALSE)
 */
int sd9_EntradaCliente() {
    int success = FALSE;
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  sd9_1_AdormeceTempoRandom    Ler a descrição da tarefa SD9.1 no enunciado
 * @return Success  (TRUE or FALSE)
 */
int sd9_1_AdormeceTempoRandom() {
    int success = FALSE;
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  sd9_2_EnviaSigusr1AoCliente    Ler a descrição da tarefa SD9.2 no enunciado
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 * @return Success  (TRUE or FALSE)
 */
int sd9_2_EnviaSigusr1AoCliente(Estacionamento clientRequest) {
    int success = FALSE;
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  s1_IniciaServidor    Ler a descrição da tarefa S1 no enunciado
 * @param  logFilename (I) O nome do ficheiro de Logfile (i.e., FILE_LOGFILE)
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 * @param  pposicaoLogfile (O) posição do ficheiro Logfile mesmo antes de inserir o log desta viatura
 * @param  plogItem (O) registo de Log para esta viatura
 * @return Success  (TRUE or FALSE)
 */
int sd9_3_EscreveLogEntradaViatura(char *logFilename, Estacionamento clientRequest, long *pposicaoLogfile, LogItem *plogItem) {
    int success = FALSE;
    so_debug("< [@param logFilename:%s, clientRequest:[%s:%s:%c:%s:%d:%d]]", logFilename, clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug("> [*pposicaoLogfile:%ld, *plogItem:[%s:%s:%c:%s:%s:%s]]", *pposicaoLogfile, plogItem->viatura.matricula, plogItem->viatura.pais, plogItem->viatura.categoria, plogItem->viatura.nomeCondutor, plogItem->dataEntrada, plogItem->dataSaida);
    return success;
}

/**
 * @brief  sd10_AguardaCheckout    Ler a descrição da tarefa SD10 no enunciado
 * @return Success  (TRUE or FALSE)
 */
int sd10_AguardaCheckout() {
    int success = FALSE;
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  sd10_2_EscreveLogSaidaViatura    Ler a descrição da tarefa SD10.2 no enunciado
 * @param  logFilename (I) O nome do ficheiro de Logfile (i.e., FILE_LOGFILE)
 * @param  posicaoLogfile (I) posição do ficheiro Logfile mesmo antes de inserir o log desta viatura
 * @param  logItem (I) registo de Log para esta viatura
 * @return Success  (TRUE or FALSE)
 */
int sd10_2_EscreveLogSaidaViatura(char *logFilename, long posicaoLogfile, LogItem logItem) {
    int success = FALSE;
    so_debug("< [@param logFilename:%s, posicaoLogfile:%ld, logItem:[%s:%s:%c:%s:%s:%s]]", logFilename, posicaoLogfile, logItem.viatura.matricula, logItem.viatura.pais, logItem.viatura.categoria, logItem.viatura.nomeCondutor, logItem.dataEntrada, logItem.dataSaida);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  sd11_EncerraServidorDedicado    Ler a descrição da tarefa SD11 no enunciado
 * @return Success  (TRUE or FALSE)
 */
int sd11_EncerraServidorDedicado() {
    int success = FALSE;
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  sd11_1_LibertaLugarViatura    Ler a descrição da tarefa SD11.1 no enunciado
 * @param  lugaresEstacionamento (I) array de lugares de estacionamento que irá servir de BD
 * @param  indexClienteBD (I) índice do lugar correspondente a este pedido na BD (>= 0), ou -1 se não houve nenhum lugar disponível
 * @return Success  (TRUE or FALSE)
 */
int sd11_1_LibertaLugarViatura(Estacionamento *lugaresEstacionamento, int indexClienteBD) {
    int success = FALSE;
    so_debug("< [@param lugaresEstacionamento:%p, indexClienteBD:%d]", lugaresEstacionamento, indexClienteBD);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  sd11_2_EnviaSighupAoClienteETerminaSD    Ler a descrição da tarefa SD11.2 no enunciado
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 * @return Success  (TRUE or FALSE)
 */
int sd11_2_EnviaSighupAoClienteETermina(Estacionamento clientRequest) {
    int success = FALSE;
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
    return success;
}

/**
 * @brief  sd12_TrataSigusr2    Ler a descrição da tarefa SD12 no enunciado
 * @param  sinalRecebido (I) número do sinal que é recebido por esta função (enviado pelo SO)
 */
void sd12_TrataSigusr2(int sinalRecebido) {
    so_debug("< [@param sinalRecebido:%d]", sinalRecebido);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}