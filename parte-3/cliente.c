/****************************************************************************************
 ** ISCTE-IUL: Trabalho prático 3 de Sistemas Operativos 2024/2025, Enunciado Versão 1+
 **
 ** Aluno: Nº:       Nome: 
 ** Nome do Módulo: cliente.c
 ** Descrição/Explicação do Módulo:
 **
 **
 ***************************************************************************************/

// #define SO_HIDE_DEBUG                // Uncomment this line to hide all @DEBUG statements
#include "defines.h" // Inclui definições e estruturas usadas no projeto

/*** Variáveis Globais ***/
int msgId = -1;                         // Variável que tem o ID da Message Queue
MsgContent clientRequest;               // Pedido enviado do Cliente para o Servidor
int recebeuRespostaServidor = FALSE;    // Variável que determina se o Cliente já recebeu uma resposta do Servidor

/**
 * @brief Processamento do processo Cliente.
 *        OS ALUNOS NÃO DEVERÃO ALTERAR ESTA FUNÇÃO.
 */
int main () {
    so_debug("<"); // Mensagem de debug (entrada na função)

    // c1_IniciaCliente:
    c1_1_GetMsgQueue(IPC_KEY, &msgId); // Obtém o ID da message queue
    c1_2_ArmaSinaisCliente();          // Arma sinais SIGINT e SIGALRM

    // c2_CheckinCliente:
    c2_1_InputEstacionamento(&clientRequest); // Lê dados do estacionamento do utilizador
    c2_2_EscrevePedido(msgId, clientRequest); // Envia pedido para o servidor

    c3_ProgramaAlarme(MAX_ESPERA); // Programa alarme para timeout

    // c4_AguardaRespostaServidor:
    c4_1_EsperaRespostaServidor(msgId, &clientRequest); // Espera resposta do servidor
    c4_2_DesligaAlarme(); // Desliga o alarme

    c5_MainCliente(msgId, &clientRequest); // Ciclo principal do cliente

    so_error("Cliente", "O programa nunca deveria ter chegado a este ponto!"); // Erro se chegar aqui
    so_debug(">"); // Mensagem de debug (saída da função)
    return 0; // Termina o programa
}

/**
 * @brief c1_1_GetMsgQueue Ler a descrição da tarefa C1.1 no enunciado
 * @param ipcKey (I) Identificador de IPC a ser usada para o projeto
 * @param pmsgId (O) identificador aberto de IPC
 */
void c1_1_GetMsgQueue(key_t ipcKey, int *pmsgId) {
    so_debug("< [@param ipcKey:0x0%x]", ipcKey); // Debug do parâmetro

    // tenta abrir a message queue já criada pelo servidor
    int msgid = msgget(ipcKey, 0666); // Abre a message queue com permissões 0666
    if (msgid == -1) { // Se falhar
        so_error("C1.1", "Erro ao abrir a Message Queue do projeto"); // Mensagem de erro
        exit(1); // Termina o programa
    }
    *pmsgId = msgid; // Guarda o id da message queue
    so_success("C1.1", "Message Queue aberta com sucesso"); // Mensagem de sucesso

    so_debug("> [@return *pmsgId:%d]", *pmsgId); // Debug do retorno
}

/**
 * @brief c1_2_ArmaSinaisCliente Ler a descrição da tarefa C1.2 no enunciado
 */
void c1_2_ArmaSinaisCliente() {
    so_debug("<"); // Debug entrada

    if (signal(SIGINT, c6_TrataCtrlC) == SIG_ERR) { // Arma SIGINT
        so_error("C1.2", "Erro ao armar o sinal SIGINT"); // Erro se falhar
        exit(1);
    }
    if (signal(SIGALRM, c7_TrataAlarme) == SIG_ERR) { // Arma SIGALRM
        so_error("C1.2", "Erro ao armar o sinal SIGALRM"); // Erro se falhar
        exit(1);
    }
    so_success("C1.2", "Sinais armados com sucesso"); // Sucesso

    so_debug(">"); // Debug saída
}

/**
 * @brief c2_1_InputEstacionamento Ler a descrição da tarefa C2.1 no enunciado
 * @param pclientRequest (O) pedido a ser enviado por este Cliente ao Servidor
 */
void c2_1_InputEstacionamento(MsgContent *pclientRequest) {
    so_debug("<"); // Debug entrada

    char buffer[100]; // Buffer para leitura
    int valido = 1;   // Flag de validação

    // Matrícula
    do {
        printf("Matrícula: "); // Pede matrícula
        if (!fgets(buffer, sizeof(buffer), stdin)) exit(1); // Lê input
        sscanf(buffer, "%9s", pclientRequest->msgData.est.viatura.matricula); // Copia para estrutura
        valido = strlen(pclientRequest->msgData.est.viatura.matricula) > 0; // Verifica se está preenchido
        if (!valido) printf("Campo obrigatório!\n"); // Mensagem de erro
    } while (!valido);

    // País
    do {
        printf("País: "); // Pede país
        if (!fgets(buffer, sizeof(buffer), stdin)) exit(1); // Lê input
        sscanf(buffer, "%2s", pclientRequest->msgData.est.viatura.pais); // Copia para estrutura
        valido = strlen(pclientRequest->msgData.est.viatura.pais) > 0; // Verifica se está preenchido
        if (!valido) printf("Campo obrigatório!\n"); // Mensagem de erro
    } while (!valido);

    // Categoria
    do {
        printf("Categoria: "); // Pede categoria
        if (!fgets(buffer, sizeof(buffer), stdin)) exit(1); // Lê input
        sscanf(buffer, " %c", &pclientRequest->msgData.est.viatura.categoria); // Copia para estrutura
        valido = pclientRequest->msgData.est.viatura.categoria != ' ' && pclientRequest->msgData.est.viatura.categoria != '\0'; // Valida
        if (!valido) printf("Campo obrigatório!\n"); // Mensagem de erro
    } while (!valido);

    // Nome do Condutor
    do {
        printf("Nome do Condutor: "); // Pede nome
        if (!fgets(buffer, sizeof(buffer), stdin)) exit(1); // Lê input
        buffer[strcspn(buffer, "\n")] = 0; // remove newline
        valido = strlen(buffer) > 0 && strspn(buffer, " ") != strlen(buffer); // Valida
        if (valido) strncpy(pclientRequest->msgData.est.viatura.nomeCondutor, buffer, sizeof(pclientRequest->msgData.est.viatura.nomeCondutor)-1); // Copia
        else printf("Campo obrigatório!\n"); // Mensagem de erro
    } while (!valido);

    pclientRequest->msgData.est.pidCliente = getpid(); // Guarda PID do cliente
    pclientRequest->msgData.est.pidServidorDedicado = -1; // Inicializa PID do servidor dedicado

    so_success("C2.1", "%s %s %c %s %d %d", pclientRequest->msgData.est.viatura.matricula, pclientRequest->msgData.est.viatura.pais, pclientRequest->msgData.est.viatura.categoria, pclientRequest->msgData.est.viatura.nomeCondutor, pclientRequest->msgData.est.pidCliente, pclientRequest->msgData.est.pidServidorDedicado); // Sucesso
    so_debug("> [*pclientRequest:[%s:%s:%c:%s:%d:%d]]", pclientRequest->msgData.est.viatura.matricula, pclientRequest->msgData.est.viatura.pais, pclientRequest->msgData.est.viatura.categoria, pclientRequest->msgData.est.viatura.nomeCondutor, pclientRequest->msgData.est.pidCliente, pclientRequest->msgData.est.pidServidorDedicado); // Debug saída
}

/**
 * @brief c2_2_EscrevePedido Ler a descrição da tarefa C2.2 no enunciado
 * @param msgId (I) identificador aberto de IPC
 * @param clientRequest (I) pedido a ser enviado por este Cliente ao Servidor
 */
void c2_2_EscrevePedido(int msgId, MsgContent clientRequest) {
    so_debug("< [@param msgId:%d, clientRequest:[%s:%s:%c:%s:%d:%d]]", msgId, clientRequest.msgData.est.viatura.matricula, clientRequest.msgData.est.viatura.pais, clientRequest.msgData.est.viatura.categoria, clientRequest.msgData.est.viatura.nomeCondutor, clientRequest.msgData.est.pidCliente, clientRequest.msgData.est.pidServidorDedicado); // Debug entrada

    clientRequest.msgType = MSGTYPE_LOGIN; // Define tipo de mensagem
    if (msgsnd(msgId, &clientRequest, sizeof(MsgContent) - sizeof(long), 0) == -1) { // Envia mensagem
        so_error("C2.2", "Erro ao enviar pedido para o servidor"); // Erro se falhar
        exit(1);
    }
    so_success("C2.2", "Pedido enviado com sucesso"); // Sucesso
    so_debug(">"); // Debug saída
}

/**
 * @brief c3_ProgramaAlarme Ler a descrição da tarefa C3 no enunciado
 * @param segundos (I) número de segundos a programar no alarme
 */
void c3_ProgramaAlarme(int segundos) {
    so_debug("< [@param segundos:%d]", segundos); // Debug entrada

    if (segundos <= 0) { // Valida segundos
        so_error("C3", "Valor de segundos inválido para o alarme"); // Erro se inválido
        exit(1);
    }
    alarm(segundos); // Programa alarme

    so_success("C3", "Espera resposta em %d segundos", segundos); // Sucesso

    so_debug(">"); // Debug saída
}

/**
 * @brief c4_1_EsperaRespostaServidor Ler a descrição da tarefa C4.1 no enunciado
 * @param msgId (I) identificador aberto de IPC
 * @param pclientRequest (O) mensagem enviada por um Servidor Dedicado
 */
void c4_1_EsperaRespostaServidor(int msgId, MsgContent *pclientRe   quest) {
    so_debug("< [@param msgId:%d]", msgId); // Debug entrada

    if (msgrcv(msgId, pclientRequest, sizeof(MsgContent) - sizeof(long), getpid(), 0) == -1) { // Espera mensagem do servidor
        so_error("C4.1", "Erro ao receber resposta do servidor"); // Erro se falhar
        exit(1);
    }

    // Verifica o status da resposta
    if (pclientRequest->msgData.status == CLIENT_ACCEPTED) { // Se aceite
        so_success("C4.1", "Check-in realizado com sucesso"); // Sucesso
    } else if (pclientRequest->msgData.status == ESTACIONAMENTO_TERMINADO) { // Se estacionamento terminado
        so_success("C4.1", "Não é possível estacionar"); // Mensagem
        exit(0); // Termina
    } else {
        so_error("C4.1", "Resposta inesperada do servidor"); // Erro se inesperado
        exit(1);
    }

    so_debug("> [*pclientRequest:[%s:%s:%c:%s:%d:%d]]", pclientRequest->msgData.est.viatura.matricula, pclientRequest->msgData.est.viatura.pais, pclientRequest->msgData.est.viatura.categoria, pclientRequest->msgData.est.viatura.nomeCondutor, pclientRequest->msgData.est.pidCliente, pclientRequest->msgData.est.pidServidorDedicado); // Debug saída
}

/**
 * @brief c4_2_DesligaAlarme Ler a descrição da tarefa C4.2 no enunciado
 */
void c4_2_DesligaAlarme() {
    so_debug("<"); // Debug entrada

    alarm(0); // Desliga o alarme
    so_success("C4.2", "Desliguei alarme"); // Sucesso

    so_debug(">"); // Debug saída
}

/**
 * @brief c5_MainCliente Ler a descrição da tarefa C5 no enunciado
 * @param msgId (I) identificador aberto de IPC
 * @param pclientRequest (O) mensagem enviada por um Servidor Dedicado
 */
void c5_MainCliente(int msgId, MsgContent *pclientRequest) {
    so_debug("< [@param msgId:%d]", msgId); // Debug entrada

    while (1) { // Ciclo infinito
        
        //Recebe mensagens da message queue com msgrcv, apenas aquelas cujo tipo (msgType) corresponde ao PID do cliente (ou seja, mensagens destinadas a este cliente).
        if (msgrcv(msgId, pclientRequest, sizeof(MsgContent) - sizeof(long), getpid(), 0) == -1) { // Espera mensagem
            so_error("C5", "Erro ao receber mensagem do servidor"); // Erro se falhar
            exit(1);
        }
        //Se o status for INFO_TARIFA, mostra a informação da tarifa ao utilizador e volta ao início do ciclo para esperar nova mensagem.
        if (pclientRequest->msgData.status == INFO_TARIFA) { // Se for info tarifa
            so_success("C5", "%s", pclientRequest->msgData.infoTarifa); // Mostra info
            continue; // Volta ao início do ciclo
            //Se o status for ESTACIONAMENTO_TERMINADO, mostra mensagem de término e termina o programa.
        } else if (pclientRequest->msgData.status == ESTACIONAMENTO_TERMINADO) { // Se estacionamento terminado
            so_success("C5", "Estacionamento terminado"); // Mensagem
            exit(0); // Termina
        } else {
            //se receber outro status, mostra erro e termina o programa.
            so_error("C5", "Status inesperado na mensagem"); // Erro se inesperado
            exit(1);
        }
    }

    so_debug("> [*pclientRequest:[%s:%s:%c:%s:%d:%d]]", pclientRequest->msgData.est.viatura.matricula, pclientRequest->msgData.est.viatura.pais, pclientRequest->msgData.est.viatura.categoria, pclientRequest->msgData.est.viatura.nomeCondutor, pclientRequest->msgData.est.pidCliente, pclientRequest->msgData.est.pidServidorDedicado); // Debug saída
}

/**
 * @brief  c6_TrataCtrlC Ler a descrição da tarefa C6 no enunciado
 * @param  sinalRecebido (I) número do sinal que é recebido por esta função (enviado pelo SO)
 */
void c6_TrataCtrlC(int sinalRecebido) {
    so_debug("< [@param sinalRecebido:%d, msgId:%d, clientRequest:[%s:%s:%c:%s:%d:%d]]", sinalRecebido, msgId, clientRequest.msgData.est.viatura.matricula, clientRequest.msgData.est.viatura.pais, clientRequest.msgData.est.viatura.categoria, clientRequest.msgData.est.viatura.nomeCondutor, clientRequest.msgData.est.pidCliente, clientRequest.msgData.est.pidServidorDedicado); // Debug entrada

    MsgContent terminaMsg; // Cria mensagem de término
    terminaMsg.msgType = clientRequest.msgData.est.pidServidorDedicado; // Define tipo para o servidor dedicado
    terminaMsg.msgData.status = TERMINA_ESTACIONAMENTO; // Define status de término
    terminaMsg.msgData.est = clientRequest.msgData.est; // Copia dados do estacionamento

    if (msgsnd(msgId, &terminaMsg, sizeof(MsgContent) - sizeof(long), 0) == -1) { // Envia mensagem de término
        so_error("C6", "Erro ao enviar mensagem de término ao servidor"); // Erro se falhar
        exit(1);
    }

    so_success("C6", "Cliente: Shutdown"); // Sucesso

    so_debug(">"); // Debug saída
}

/**
 * @brief  c7_TrataAlarme Ler a descrição da tarefa C7 no enunciado
 * @param  sinalRecebido (I) número do sinal que é recebido por esta função (enviado pelo SO)
 */
void c7_TrataAlarme(int sinalRecebido) {
    so_debug("< [@param sinalRecebido:%d]", sinalRecebido); // Debug entrada

    so_error("C7", "Cliente: Timeout"); // Mensagem de timeout
    exit(0); // Termina

    so_debug(">"); // Debug saída (não é executada)
}