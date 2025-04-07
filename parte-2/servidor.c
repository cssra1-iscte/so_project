/****************************************************************************************
 ** ISCTE-IUL: Trabalho prático 2 de Sistemas Operativos 2024/2025, Enunciado Versão 4+
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
 * @brief  Processamento do processo Servidor e dos processos Servidor Dedicado
 *         OS ALUNOS NÃO DEVERÃO ALTERAR ESTA FUNÇÃO.
 * @param  argc (I) número de Strings do array argv
 * @param  argv (I) array de lugares de estacionamento que irá servir de BD
 * @return Success (0) or not (<> 0)
 */
int main(int argc, char *argv[]) {
    so_debug("<");

    s1_IniciaServidor(argc, argv);
    s2_MainServidor();

    so_error("Servidor", "O programa nunca deveria ter chegado a este ponto!");
    so_debug(">");
}

/**
 * @brief  s1_iniciaServidor Ler a descrição da tarefa S1 no enunciado.
 *         OS ALUNOS NÃO DEVERÃO ALTERAR ESTA FUNÇÃO.
 * @param  argc (I) número de Strings do array argv
 * @param  argv (I) array de lugares de estacionamento que irá servir de BD
 */
void s1_IniciaServidor(int argc, char *argv[]) {
    so_debug("<");

    s1_1_ObtemDimensaoParque(argc, argv, &dimensaoMaximaParque);
    s1_2_CriaBD(dimensaoMaximaParque, &lugaresEstacionamento);
    s1_3_ArmaSinaisServidor();
    s1_4_CriaFifoServidor(FILE_REQUESTS);

    so_debug(">");
}

/**
 * @brief  s1_1_ObtemDimensaoParque Ler a descrição da tarefa S1.1 no enunciado
 * @param  argc (I) número de Strings do array argv
 * @param  argv (I) array de lugares de estacionamento que irá servir de BD
 * @param  pdimensaoMaximaParque (O) número máximo de lugares do parque, especificado pelo utilizador
 */
void s1_1_ObtemDimensaoParque(int argc, char *argv[], int *pdimensaoMaximaParque) {
    so_debug("< [@param argc:%d, argv:%p]", argc, argv);

    /** 
        EXPLICAÇÃO CÓDIGO:
        * Verificar se o user escreveu 1 e só 1 argumentos (argc tem que ser igual a 2)
        * Verificar se é um inteiro. Para tal, recorremos ao método strtol(const char* str, char** endptr, int base) 
        para converter a string, passada como argumento, num valor inteiro longo. 
        * A função strtol para de ler o valor de "str", ao tetor um dígito diferente de "int", devolvendo esse mesmo valor. 
        de acordo com a base escolhida (neste caso vamos escolher base 10)
        * O ponteiro especial **endptr, permite guardar o conteúdo da posição a seguir ao último dígito onde parou o "str". 
        * Se o valor guardado no ponteiro *endptr não for "\0", quer dizer que o valor da String passada como argumento não
        era um inteiro, logo dá erro. 
    */

    if (argc != 2) { // verifica se foram passados 2 e so dois argc (o nome do porgrama mais argv[1])
        so_error("S1.1", "Número de argumentos inválido");
        exit(1);
    }

    char *endptr;
    int valor = strtol(argv[1], &endptr, 10); 

    if (*endptr != '\0' || valor <= 0) { 
        so_error("S1.1", "O valor passado não é válido. Dimensão tem que ser um nº inteiro superior a 0");
        exit(1);
    }

    *pdimensaoMaximaParque = valor;

    so_success("S1.1", "Argumento validado com sucesso");

    so_debug("> [@param +pdimensaoMaximaParque:%d]", *pdimensaoMaximaParque);
}

/**
 * @brief  s1_2_CriaBD Ler a descrição da tarefa S1.2 no enunciado
 * @param  dimensaoMaximaParque (I) número máximo de lugares do parque, especificado pelo utilizador
 * @param  plugaresEstacionamento (O) array de lugares de estacionamento que irá servir de BD
 */
void s1_2_CriaBD(int dimensaoMaximaParque, Estacionamento **plugaresEstacionamento) {
    so_debug("< [@param dimensaoMaximaParque:%d]", dimensaoMaximaParque);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug("> [*plugaresEstacionamento:%p]", *plugaresEstacionamento);
}

/**
 * @brief  s1_3_ArmaSinaisServidor Ler a descrição da tarefa S1.3 no enunciado
 */
void s1_3_ArmaSinaisServidor() {
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  s1_4_CriaFifoServidor Ler a descrição da tarefa S1.4 no enunciado
 * @param  filenameFifoServidor (I) O nome do FIFO do servidor (i.e., FILE_REQUESTS)
 */
void s1_4_CriaFifoServidor(char *filenameFifoServidor) {
    so_debug("< [@param filenameFifoServidor:%s]", filenameFifoServidor);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  s2_MainServidor Ler a descrição da tarefa S2 no enunciado.
 *         OS ALUNOS NÃO DEVERÃO ALTERAR ESTA FUNÇÃO, exceto depois de
 *         realizada a função s2_1_AbreFifoServidor(), altura em que podem
 *         comentar o statement sleep abaixo (que, neste momento está aqui
 *         para evitar que os alunos tenham uma espera ativa no seu código)
 */
void s2_MainServidor() {
    so_debug("<");

    FILE *fFifoServidor;
    while (TRUE) { 
        s2_1_AbreFifoServidor(FILE_REQUESTS, &fFifoServidor);
        s2_2_LePedidosFifoServidor(fFifoServidor);
        sleep(10);  // TEMPORÁRIO, os alunos deverão comentar este statement apenas
                    // depois de terem a certeza que não terão uma espera ativa
    }

    so_debug(">");
}

/**
 * @brief  s2_1_AbreFifoServidor Ler a descrição da tarefa S2.1 no enunciado
 * @param  filenameFifoServidor (I) O nome do FIFO do servidor (i.e., FILE_REQUESTS)
 * @param  pfFifoServidor (O) descritor aberto do ficheiro do FIFO do servidor
 */
void s2_1_AbreFifoServidor(char *filenameFifoServidor, FILE **pfFifoServidor) {
    so_debug("< [@param filenameFifoServidor:%s]", filenameFifoServidor);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug("> [*pfFifoServidor:%p]", *pfFifoServidor);
}

/**
 * @brief  s2_2_LePedidosFifoServidor Ler a descrição da tarefa S2.2 no enunciado.
 *         OS ALUNOS NÃO DEVERÃO ALTERAR ESTA FUNÇÃO.
 * @param  fFifoServidor (I) descritor aberto do ficheiro do FIFO do servidor
 */
void s2_2_LePedidosFifoServidor(FILE *fFifoServidor) {
    so_debug("<");

    int terminaCiclo2 = FALSE;
    while (TRUE) {
        terminaCiclo2 = s2_2_1_LePedido(fFifoServidor, &clientRequest);
        if (terminaCiclo2)
            break;
        s2_2_2_ProcuraLugarDisponivelBD(clientRequest, lugaresEstacionamento, dimensaoMaximaParque, &indexClienteBD);
        s2_2_3_CriaServidorDedicado(lugaresEstacionamento, indexClienteBD);
    }

    so_debug(">");
}

/**
 * @brief  s2_2_1_LePedido Ler a descrição da tarefa S2.2.1 no enunciado
 * @param  fFifoServidor (I) descritor aberto do ficheiro do FIFO do servidor
 * @param  pclientRequest (O) pedido recebido, enviado por um Cliente
 * @return TRUE se não conseguiu ler um pedido porque o FIFO não tem mais pedidos.
 */
int s2_2_1_LePedido(FILE *fFifoServidor, Estacionamento *pclientRequest) {
    int naoHaMaisPedidos = TRUE;
    so_debug("< [@param fFifoServidor:%p]", fFifoServidor);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug("> [naoHaMaisPedidos:%d, *pclientRequest:[%s:%s:%c:%s:%d.%d]]", naoHaMaisPedidos, pclientRequest->viatura.matricula, pclientRequest->viatura.pais, pclientRequest->viatura.categoria, pclientRequest->viatura.nomeCondutor, pclientRequest->pidCliente, pclientRequest->pidServidorDedicado);
    return naoHaMaisPedidos;
}

/**
 * @brief  s2_2_2_ProcuraLugarDisponivelBD Ler a descrição da tarefa S2.2.2 no enunciado
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 * @param  lugaresEstacionamento (I) array de lugares de estacionamento que irá servir de BD
 * @param  dimensaoMaximaParque (I) número máximo de lugares do parque, especificado pelo utilizador
 * @param  pindexClienteBD (O) índice do lugar correspondente a este pedido na BD (>= 0), ou -1 se não houve nenhum lugar disponível
 */
void s2_2_2_ProcuraLugarDisponivelBD(Estacionamento clientRequest, Estacionamento *lugaresEstacionamento, int dimensaoMaximaParque, int *pindexClienteBD) {
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d], lugaresEstacionamento:%p, dimensaoMaximaParque:%d]", clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado, lugaresEstacionamento, dimensaoMaximaParque);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug("> [*pindexClienteBD:%d]", *pindexClienteBD);
}

/**
 * @brief  s2_2_3_CriaServidorDedicado    Ler a descrição da tarefa S2.2.3 no enunciado
 * @param  lugaresEstacionamento (I) array de lugares de estacionamento que irá servir de BD
 * @param  indexClienteBD (I) índice do lugar correspondente a este pedido na BD (>= 0), ou -1 se não houve nenhum lugar disponível
 */
void s2_2_3_CriaServidorDedicado(Estacionamento *lugaresEstacionamento, int indexClienteBD) {
    so_debug("< [@param lugaresEstacionamento:%p, indexClienteBD:%d]", lugaresEstacionamento, indexClienteBD);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
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
 * @brief  s4_EncerraServidor Ler a descrição da tarefa S4 no enunciado
 * @param  filenameFifoServidor (I) O nome do FIFO do servidor (i.e., FILE_REQUESTS)
 */
void s4_EncerraServidor(char *filenameFifoServidor) {
    so_debug("< [@param filenameFifoServidor:%s]", filenameFifoServidor);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
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
 * @brief  sd7_ServidorDedicado Ler a descrição da tarefa SD7 no enunciado
 *         OS ALUNOS NÃO DEVERÃO ALTERAR ESTA FUNÇÃO.
 */
void sd7_MainServidorDedicado() {
    so_debug("<");

    // sd7_IniciaServidorDedicado:
    sd7_1_ArmaSinaisServidorDedicado();
    sd7_2_ValidaPidCliente(clientRequest);
    sd7_3_ValidaLugarDisponivelBD(indexClienteBD);

    // sd8_ValidaPedidoCliente:
    sd8_1_ValidaMatricula(clientRequest);
    sd8_2_ValidaPais(clientRequest);
    sd8_3_ValidaCategoria(clientRequest);
    sd8_4_ValidaNomeCondutor(clientRequest);

    // sd9_EntradaCliente:
    sd9_1_AdormeceTempoRandom();
    sd9_2_EnviaSigusr1AoCliente(clientRequest);
    sd9_3_EscreveLogEntradaViatura(FILE_LOGFILE, clientRequest, &posicaoLogfile, &logItem);

    // sd10_AcompanhaCliente:
    sd10_1_AguardaCheckout();
    sd10_2_EscreveLogSaidaViatura(FILE_LOGFILE, posicaoLogfile, logItem);

    sd11_EncerraServidorDedicado();

    so_error("Servidor Dedicado", "O programa nunca deveria ter chegado a este ponto!");

    so_debug(">");
}

/**
 * @brief  sd7_1_ArmaSinaisServidorDedicado    Ler a descrição da tarefa SD7.1 no enunciado
 */
void sd7_1_ArmaSinaisServidorDedicado() {
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  sd7_2_ValidaPidCliente    Ler a descrição da tarefa SD7.2 no enunciado
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 */
void sd7_2_ValidaPidCliente(Estacionamento clientRequest) {
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  sd7_3_ValidaLugarDisponivelBD    Ler a descrição da tarefa SD7.3 no enunciado
 * @param  indexClienteBD (I) índice do lugar correspondente a este pedido na BD (>= 0), ou -1 se não houve nenhum lugar disponível
 */
void sd7_3_ValidaLugarDisponivelBD(int indexClienteBD) {
    so_debug("< [@param indexClienteBD:%d]", indexClienteBD);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  sd8_1_ValidaMatricula Ler a descrição da tarefa SD8.1 no enunciado
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 */
void sd8_1_ValidaMatricula(Estacionamento clientRequest) {
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  sd8_2_ValidaPais Ler a descrição da tarefa SD8.2 no enunciado
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 */
void sd8_2_ValidaPais(Estacionamento clientRequest) {
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  sd8_3_ValidaCategoria Ler a descrição da tarefa SD8.3 no enunciado
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 */
void sd8_3_ValidaCategoria(Estacionamento clientRequest) {
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  sd8_4_ValidaNomeCondutor Ler a descrição da tarefa SD8.4 no enunciado
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 */
void sd8_4_ValidaNomeCondutor(Estacionamento clientRequest) {
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  sd9_1_AdormeceTempoRandom Ler a descrição da tarefa SD9.1 no enunciado
 */
void sd9_1_AdormeceTempoRandom() {
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  sd9_2_EnviaSigusr1AoCliente Ler a descrição da tarefa SD9.2 no enunciado
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 */
void sd9_2_EnviaSigusr1AoCliente(Estacionamento clientRequest) {
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  sd9_3_EscreveLogEntradaViatura Ler a descrição da tarefa SD9.3 no enunciado
 * @param  logFilename (I) O nome do ficheiro de Logfile (i.e., FILE_LOGFILE)
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 * @param  pposicaoLogfile (O) posição do ficheiro Logfile mesmo antes de inserir o log desta viatura
 * @param  plogItem (O) registo de Log para esta viatura
 */
void sd9_3_EscreveLogEntradaViatura(char *logFilename, Estacionamento clientRequest, long *pposicaoLogfile, LogItem *plogItem) {
    so_debug("< [@param logFilename:%s, clientRequest:[%s:%s:%c:%s:%d:%d]]", logFilename, clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug("> [*pposicaoLogfile:%ld, *plogItem:[%s:%s:%c:%s:%s:%s]]", *pposicaoLogfile, plogItem->viatura.matricula, plogItem->viatura.pais, plogItem->viatura.categoria, plogItem->viatura.nomeCondutor, plogItem->dataEntrada, plogItem->dataSaida);
}

/**
 * @brief  sd10_1_AguardaCheckout Ler a descrição da tarefa SD10.1 no enunciado
 */
void sd10_1_AguardaCheckout() {
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  sd10_2_EscreveLogSaidaViatura Ler a descrição da tarefa SD10.2 no enunciado
 * @param  logFilename (I) O nome do ficheiro de Logfile (i.e., FILE_LOGFILE)
 * @param  posicaoLogfile (I) posição do ficheiro Logfile mesmo antes de inserir o log desta viatura
 * @param  logItem (I) registo de Log para esta viatura
 */
void sd10_2_EscreveLogSaidaViatura(char *logFilename, long posicaoLogfile, LogItem logItem) {
    so_debug("< [@param logFilename:%s, posicaoLogfile:%ld, logItem:[%s:%s:%c:%s:%s:%s]]", logFilename, posicaoLogfile, logItem.viatura.matricula, logItem.viatura.pais, logItem.viatura.categoria, logItem.viatura.nomeCondutor, logItem.dataEntrada, logItem.dataSaida);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  sd11_EncerraServidorDedicado Ler a descrição da tarefa SD11 no enunciado
 *         OS ALUNOS NÃO DEVERÃO ALTERAR ESTA FUNÇÃO.
 */
void sd11_EncerraServidorDedicado() {
    so_debug("<");

    sd11_1_LibertaLugarViatura(lugaresEstacionamento, indexClienteBD);
    sd11_2_EnviaSighupAoClienteETermina(clientRequest);

    so_debug(">");
}

/**
 * @brief  sd11_1_LibertaLugarViatura Ler a descrição da tarefa SD11.1 no enunciado
 * @param  lugaresEstacionamento (I) array de lugares de estacionamento que irá servir de BD
 * @param  indexClienteBD (I) índice do lugar correspondente a este pedido na BD (>= 0), ou -1 se não houve nenhum lugar disponível
 */
void sd11_1_LibertaLugarViatura(Estacionamento *lugaresEstacionamento, int indexClienteBD) {
    so_debug("< [@param lugaresEstacionamento:%p, indexClienteBD:%d]", lugaresEstacionamento, indexClienteBD);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  sd11_2_EnviaSighupAoClienteETerminaSD Ler a descrição da tarefa SD11.2 no enunciado
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 */
void sd11_2_EnviaSighupAoClienteETermina(Estacionamento clientRequest) {
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
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

/**
 * @brief  sd13_TrataSigusr1    Ler a descrição da tarefa SD13 no enunciado
 * @param  sinalRecebido (I) número do sinal que é recebido por esta função (enviado pelo SO)
 */
void sd13_TrataSigusr1(int sinalRecebido) {
    so_debug("< [@param sinalRecebido:%d]", sinalRecebido);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}