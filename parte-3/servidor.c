/****************************************************************************************
 ** ISCTE-IUL: Trabalho prático 3 de Sistemas Operativos 2024/2025, Enunciado Versão 1+
 **
 ** Aluno: Nº:       Nome: 
 ** Nome do Módulo: servidor.c
 ** Descrição/Explicação do Módulo:
 **
 **
 ***************************************************************************************/

// #define SO_HIDE_DEBUG                // Uncomment this line to hide all @DEBUG statements
#include "defines.h"

/*** Variáveis Globais ***/
int nrServidoresDedicados = 0;          // Número de servidores dedicados (só faz sentido no processo Servidor)
int shmId = -1;                         // Variável que tem o ID da Shared Memory
int msgId = -1;                         // Variável que tem o ID da Message Queue
int semId = -1;                         // Variável que tem o ID do Grupo de Semáforos
MsgContent clientRequest;               // Pedido enviado do Cliente para o Servidor
Estacionamento *lugaresEstacionamento = NULL;   // Array de Lugares de Estacionamento do parque
int dimensaoMaximaParque;               // Dimensão Máxima do parque (BD), recebida por argumento do programa
int indexClienteBD = -1;                // Índice do cliente que fez o pedido ao servidor/servidor dedicado na BD
long posicaoLogfile = -1;               // Posição no ficheiro Logfile para escrever o log da entrada corrente
LogItem logItem;                        // Informação da entrada corrente a escrever no logfile
int shmIdFACE = -1;                     // Variável que tem o ID da Shared Memory da entidade externa FACE
int semIdFACE = -1;                     // Variável que tem o ID do Grupo de Semáforos da entidade externa FACE
int *tarifaAtual = NULL;                // Inteiro definido pela entidade externa FACE com a tarifa atual do parque

/**
 * @brief  Processamento do processo Servidor e dos processos Servidor Dedicado
 *         OS ALUNOS NÃO DEVERÃO ALTERAR ESTA FUNÇÃO.
 * @param  argc (I) número de Strings do array argv
 * @param  argv (I) array de lugares de estacionamento que irá servir de BD
 * @return Success (0) or not (<> 0)
 */


int main(int argc, char *argv[]) {
    so_debug("<");

    //Iniciação do Servidor: Se encontrar algum erro, dá so_error (e.g., so_error("S1.1", "<erro>")) e termina o Servidor. 
    //  Caso contrário, dá so_success (e.g., so_success("S1.2", "<mensagem de sucesso>")).
    s1_IniciaServidor(argc, argv);

    //Ciclo 1: Funcionamento principal do processo Servidor: 
    // Em caso de qualquer erro, dá so_error e segue para o passo S4 (encerramento do Servidor).
    s2_MainServidor();

    so_error("Servidor", "O programa nunca deveria ter chegado a este ponto!");
    so_debug(">");
    return 0;
}

/**
 * @brief s1_iniciaServidor Ler a descrição da tarefa S1 no enunciado.
 *        OS ALUNOS NÃO DEVERÃO ALTERAR ESTA FUNÇÃO.
 * @param argc (I) número de Strings do array argv
 * @param argv (I) array de lugares de estacionamento que irá servir de BD
 */
void s1_IniciaServidor(int argc, char *argv[]) {
    so_debug("<");

    // Recebe como argumento do utilizador a dimensão do parque de 
    // estacionamento (dimensaoMaximaParque).
    s1_1_ObtemDimensaoParque(argc, argv, &dimensaoMaximaParque);
    // Arma os sinais a serem tratados pelo Servidor, para lidar com o atalho <CTRL+C> (ver S3), 
    // e para lidar com a finalização de cada Servidor Dedicado (ver S5).
    s1_2_ArmaSinaisServidor();
    // Cria a Message Queue (MSG) do projeto, com a KEY IPC_KEY: Se a MSG já existia, remove-a e cria 
    // de novo.
    s1_3_CriaMsgQueue(IPC_KEY, &msgId);
    // Cria o grupo de semáforos (SEM) necessários para o projeto, com a KEY IPC_KEY: Se o grupo SEM 
    // já existia, remove-o e cria um novo com quatro semáforos: dois semáforos do tipo MutEx, 
    // SEM_MUTEX_BD e SEM_MUTEX_LOGFILE, que deverão ser iniciados de forma correspondente, 
    // um semáforo do tipo Barreira, SEM_SRV_DEDICADOS, que será iniciado com o valor 0, 
    // e outro semáforo para controlar o número de lugares do parque, SEM_LUGARES_PARQUE, 
    // que será iniciado com o valor dimensaoMaximaParque.
    s1_4_CriaGrupoSemaforos(IPC_KEY, &semId);
    // Cria o parque de estacionamento (i.e., a base de dados correspondente ao array 
    // lugaresEstacionamento) com a dimensaoMaximaParque, numa Shared Memory (SHM) IPC 
    // com a KEY IPC_KEY definida em defines.h: Se a SHM já existia com o tamanho correto, 
    // apenas liga-se a ela. Caso contrário, cria a SHM, e, apenas nesse caso, inicia o array 
    // com todos os lugares disponíveis (pidCliente=DISPONIVEL).
    s1_5_CriaBD(IPC_KEY, &shmId, dimensaoMaximaParque, &lugaresEstacionamento);

    so_debug(">");
}

/**
 * @brief s1_1_ObtemDimensaoParque Ler a descrição da tarefa S1.1 no enunciado
 * @param argc (I) número de Strings do array argv
 * @param argv (I) array de lugares de estacionamento que irá servir de BD
 * @param pdimensaoMaximaParque (O) número máximo de lugares do parque, especificado pelo utilizador
 */
void s1_1_ObtemDimensaoParque(int argc, char *argv[], int *pdimensaoMaximaParque) {
    so_debug("< [@param argc:%d, argv:%p]", argc, argv);
    
    // verifica se foram passados 2 e so dois argc (o nome do porgrama mais argv[1])
    if (argc != 2) { // verifica se foram passados 2 e so dois argc (o nome do porgrama mais argv[1])
        so_error("S1.1", "Número de argumentos inválido");
        exit(1);
    }
    //
    char *endptr;
    int valor = strtol(argv[1], &endptr, 10); 

    if (*endptr != '\0' || valor <= 0) { 
        so_error("S1.1", "O valor passado não é válido. Dimensão tem que ser um nº inteiro superior a 0");
        exit(1);
    }

    *pdimensaoMaximaParque = valor;

    so_success("S1.1", "Argumento validado com sucesso");


    so_debug("> [@return *pdimensaoMaximaParque:%d]", *pdimensaoMaximaParque);
}

/**
 * @brief s1_2_ArmaSinaisServidor Ler a descrição da tarefa S1.2 no enunciado
 */
void s1_2_ArmaSinaisServidor() {
    so_debug("<");

    // armar o sinal para reagir de acordo com s3_TrataCtrlC (handler do sinal)
    if (signal(SIGINT, s3_TrataCtrlC) == SIG_ERR) {
        so_error("S1.2", "Erro ao armar o sinal SIGINT");
        exit(1);
    }

     // armar o sinal para reagir de acordo com s5_TrataTerminouServidorDedicado (handler do sinal)
    if (signal(SIGCHLD, s5_TrataTerminouServidorDedicado) == SIG_ERR) {
        so_error("S1.2", "Erro ao armar SIGCHLD");
        exit(1);
    }
    
    so_success("S1.2", "Sinais SIGINT e SIGCHLD armados com sucesso");

    so_debug(">");
}

/**
 * @brief s1_3_CriaMsgQueue Ler a descrição da tarefa s1.3 no enunciado
 * @param ipcKey (I) Identificador de IPC a ser usada para o projeto
 * @param pmsgId (O) identificador aberto de IPC
 */
void s1_3_CriaMsgQueue(key_t ipcKey, int *pmsgId) {
    so_debug("< [@param ipcKey:0x0%x]", ipcKey);

    //tenta criar a Message Queue com a chave IPC_KEY
    int msgid = msgget(ipcKey, IPC_CREAT | IPC_EXCL | 0666);

    // verifica se a criação da MSQ falhou e se essa falha se deve a MSQ já existente
     if (msgid == -1 && errno == EEXIST) {
        //a falha deveu-se ao facto de que MSQ já existir
        //obtemos o id associada à MSQ existente
        msgid = msgget(ipcKey, 0);
        // se não der erro, então apaga-se a MSQ existente
        if (msgid != -1) {
            msgctl(msgid, IPC_RMID, NULL);
            so_success("S1.3", "Message Queue já existente, eliminada com sucesso");
        }
        // tenta criar-se a MSQ novamente
        msgid = msgget(ipcKey, IPC_CREAT | IPC_EXCL | 0666);
    }
    // verifica se a criação da MSQ feita na passo anterior falhou
    if (msgid == -1) {
        so_error("S1.3", "Erro ao criar Message Queue");
        exit(1);
    }
    // guarda o id da MSQ criada na variável global msgId
    // (através do ponteiro pmsgId)
    *pmsgId = msgid;
    so_success("S1.3", "Message Queue criada com sucesso");

    so_debug("> [@return *pmsgId:%d]", *pmsgId);
}

/**
 * @brief s1_4_CriaGrupoSemaforos Ler a descrição da tarefa s1.4 no enunciado
 * @param ipcKey (I) Identificador de IPC a ser usada para o projeto
 * @param psemId (O) identificador aberto de IPC
 */
 void s1_4_CriaGrupoSemaforos(key_t ipcKey, int *psemId) {
    so_debug("< [@param ipcKey:0x0%x]", ipcKey);
    
    // tenta criar a Grupo de Semáforos com a chave IPC_KEY (no meu caso 0xa82442)
    int semid = semget(ipcKey, 4, IPC_CREAT | IPC_EXCL | 0666);
    // verifica se já existe grupo de semáfotos associado ao ipcKey criado
    // se sim, então apaga-o e cria-o novamente
    if (semid == -1 && errno == EEXIST) {
        //a falha se deveu ao facto de que grupo de semáforos já existe,
        // vai buscra o id associada ao ipcKey
        semid = semget(ipcKey, 0, 0);
        if (semid != -1) {
            // senão der erro, apaga o conjunto de semáforos existentes
            semctl(semid, 0, IPC_RMID);
            so_success("S1.4", "Grupo de semáforos JÁ EXISTENTE, eliminado com sucesso");
        }
        // após eliminar p grupo existente, cria um novo conjunto com 4 sems associado a ipcKey
        semid = semget(ipcKey, 4, IPC_CREAT | IPC_EXCL | 0666);
    }
    // se o passo anterior falhar, mostra mensagem de erro e termina o programa
    if (semid == -1) {
        so_error("S1.4", "Erro ao criar grupo de semáforos");
        exit(1);
    }

    // Inicializar cada semáforo 
    semctl(semid, SEM_MUTEX_BD, SETVAL, SEM_MUTEX_INITIAL_VALUE);        // SEM_MUTEX_BD=1
    semctl(semid, SEM_MUTEX_LOGFILE, SETVAL, SEM_MUTEX_INITIAL_VALUE);   // SEM_MUTEX_LOGFILE=1
    semctl(semid, SEM_SRV_DEDICADOS, SETVAL, 0);                         // SEM_SRV_DEDICADOS=0
    semctl(semid, SEM_LUGARES_PARQUE, SETVAL, dimensaoMaximaParque);     // SEM_LUGARES_PARQUE=imensaoMaximaParque

    *psemId = semid;
    so_success("S1.4", "Grupo de semáforos CRIADO e INICIALIZADO com sucesso");

    so_debug("> [@return *psemId:%d]", *psemId);
}

/**
 * @brief s1_5_CriaBD Ler a descrição da tarefa S1.5 no enunciado
 * @param ipcKey (I) Identificador de IPC a ser usada para o projeto
 * @param pshmId (O) identificador aberto de IPC
 * @param dimensaoMaximaParque (I) número máximo de lugares do parque, especificado pelo utilizador
 * @param plugaresEstacionamento (O) array de lugares de estacionamento que irá servir de BD
 */
void s1_5_CriaBD(key_t ipcKey, int *pshmId, int dimensaoMaximaParque, Estacionamento **plugaresEstacionamento) {
    so_debug("< [@param ipcKey:0x0%x, dimensaoMaximaParque:%d]", ipcKey, dimensaoMaximaParque);

   int shm_tamanho = dimensaoMaximaParque * sizeof(Estacionamento);

    // tenta aceder à SHM existente
    int shmId = shmget(ipcKey, 0, 0);

    if (shmId != -1) {
        // SHM existe, verificar tamanho
        struct shmid_ds info;
        if (shmctl(shmId, IPC_STAT, &info) == -1) {
            so_error("S1.5", "Erro ao obter info da SHM existente");
            exit(1);
        }
        if (info.shm_segsz != shm_tamanho) {
            // Tamanho diferente, remove e cria de novo
            if (shmctl(shmId, IPC_RMID, NULL) == -1) {
                so_error("S1.5", "Erro ao remover SHM com tamanho diferente");
                exit(1);
            }
            shmId = shmget(ipcKey, shm_tamanho, IPC_CREAT | IPC_EXCL | 0666);
            if (shmId == -1) {
                so_error("S1.5", "Erro ao criar NOVA SHM");
                exit(1);
            }

            *pshmId = shmId;
            *plugaresEstacionamento = (Estacionamento *)shmat(shmId, NEW_ADDRESS, 0);
            if (*plugaresEstacionamento == SHMAT_ERROR) {
                so_error("S1.5", "Erro ao fazer attach à nova SHM");
                exit(1);
            }
            // Inicializar todos os lugares como livres (pidCliente = 0)
            for (int i = 0; i < dimensaoMaximaParque; i++) {
                (*plugaresEstacionamento)[i].pidCliente = DISPONIVEL;
            }
            so_success("S1.5", "SHM criada DE NOVO e lugares inicializados como livres");
        } else {
            // Tamanho correto, só faz attach
            *pshmId = shmId;
            *plugaresEstacionamento = (Estacionamento *)shmat(shmId, NEW_ADDRESS, 0);
            if (*plugaresEstacionamento == SHMAT_ERROR) {
                so_error("S1.5", "Erro ao fazer attach à SHM existente");
                exit(1);
            }


            so_success("S1.5", "Ligado à SHM existente com sucesso");
        }
    } else {
        // SHM NÃO EXISTE, cria e inicializa
        shmId = shmget(ipcKey, shm_tamanho, IPC_CREAT | IPC_EXCL | 0666);
        if (shmId == -1) {
            so_error("S1.5", "Erro ao criar SHM");
            exit(1);
        }

        *pshmId = shmId;
        *plugaresEstacionamento = (Estacionamento *)shmat(shmId, NEW_ADDRESS, 0);

        if (*plugaresEstacionamento == SHMAT_ERROR) {
            so_error("S1.5", "Erro ao fazer attach à nova SHM");
            exit(1);
        }

        for (int i = 0; i < dimensaoMaximaParque; i++) {
            (*plugaresEstacionamento)[i].pidCliente = DISPONIVEL;
        }
        
        so_success("S1.5", "Sem SHM anterior. SHM criada e com array inicializado com lugares livres");
    }
    
    so_debug("> [@return *pshmId:%d, *plugaresEstacionamento:%p]", *pshmId, *plugaresEstacionamento);
}

/**
 * @brief s2_MainServidor Ler a descrição da tarefa S2 no enunciado.
 *        OS ALUNOS NÃO DEVERÃO ALTERAR ESTA FUNÇÃO
 */
 // Ciclo 1: Funcionamento principal do processo Servidor: Em caso de qualquer erro, dá so_error 
 // e segue para o passo S4 (encerramento do Servidor).
 void s2_MainServidor() {
    so_debug("<");

    while (TRUE) {
        // Lê da MSG uma mensagem do tipo MSGTYPE_LOGIN para clientRequest. 
        // Dá so_success "<matricula> <pidCliente>". 
        // Se receber um sinal, trata-o da forma esperada, e retoma o funcionamento normal.
        s2_1_LePedidoCliente(msgId, &clientRequest);
        // Cria um processo Servidor Dedicado. Na sequência, o novo processo Servidor Dedicado 
        // dá so_success "SD: Nasci com PID <pidServidorDedicado>" e segue para o passo SD7, 
        // enquanto que o processo Servidor dá so_success "Servidor: Iniciei SD <pidServidorDedicado>", e retoma o Ciclo 1 no passo S2.
        s2_2_CriaServidorDedicado(&nrServidoresDedicados);
    }

    so_debug(">");
}

/**
 * @brief s2_1_LePedidoCliente Ler a descrição da tarefa S2.1 no enunciado.
 * @param msgId (I) identificador aberto de IPC
 * @param pclientRequest (O) pedido recebido, enviado por um Cliente
 */
void s2_1_LePedidoCliente(int msgId, MsgContent *pclientRequest) {
    so_debug("< [@param msgId:%d]", msgId);

     // Lê da MSG uma mensagem do tipo MSGTYPE_LOGIN
    if (msgrcv(msgId, pclientRequest, sizeof(MsgContent) - sizeof(long), MSGTYPE_LOGIN, 0) == -1) {
        so_error("S2.1", "Erro ao ler pedido do cliente");
        s4_EncerraServidor();
        exit(1);
    }
    so_success("S2.1", "%s %d", pclientRequest->msgData.est.viatura.matricula, pclientRequest->msgData.est.pidCliente);

    //sleep(10);  // TEMPORÁRIO, os alunos deverão comentar este statement apenas
                // depois de terem a certeza que não terão uma espera ativa

    so_debug("> [@return *pclientRequest:[%s:%s:%c:%s:%d.%d]]", pclientRequest->msgData.est.viatura.matricula, pclientRequest->msgData.est.viatura.pais, pclientRequest->msgData.est.viatura.categoria, pclientRequest->msgData.est.viatura.nomeCondutor, pclientRequest->msgData.est.pidCliente, pclientRequest->msgData.est.pidServidorDedicado);
}

/**
 * @brief s2_2_CriaServidorDedicado Ler a descrição da tarefa S2.2 no enunciado
 * @param pnrServidoresDedicados (O) número de Servidores Dedicados que foram criados até então
 */
void s2_2_CriaServidorDedicado(int *pnrServidoresDedicados) {
    so_debug("<");

      pid_t pid = fork();
    if (pid < 0) {
        so_error("S2.2", "Erro ao criar Servidor Dedicado");
        s4_EncerraServidor();
        exit(1);
    }
    if (pid == 0) {
        // Processo filho: Servidor Dedicado
        so_success("S2.2", "SD: Nasci com PID %d", getpid());
        sd7_MainServidorDedicado();
        exit(0); // Termina o processo filho após completar a tarefa do Servidor Dedicado
    } else {
        // Processo pai: Servidor principal
        (*pnrServidoresDedicados)++;
        so_success("S2.2", "Servidor: Iniciei SD %d", pid);
    }

    so_debug("> [@return *pnrServidoresDedicados:%d", *pnrServidoresDedicados);
}

/**
 * @brief s3_TrataCtrlC Ler a descrição da tarefa S3 no enunciado
 * @param sinalRecebido (I) número do sinal que é recebido por esta função (enviado pelo SO)
 */
void s3_TrataCtrlC(int sinalRecebido) {
    so_debug("< [@param sinalRecebido:%d]", sinalRecebido);

    so_success("S3", "Servidor: Start Shutdown");
    s4_EncerraServidor();
    exit(0);

    so_debug(">");
}

/**
 * @brief s4_EncerraServidor Ler a descrição da tarefa S4 no enunciado
 *        OS ALUNOS NÃO DEVERÃO ALTERAR ESTA FUNÇÃO
 */
void s4_EncerraServidor() {
    so_debug("<");

    s4_1_TerminaServidoresDedicados(lugaresEstacionamento, dimensaoMaximaParque);
    s4_2_AguardaFimServidoresDedicados(nrServidoresDedicados);
    s4_3_ApagaElementosIPCeTermina(shmId, semId, msgId);

    so_debug(">");
}

/**
 * @brief s4_1_TerminaServidoresDedicados Ler a descrição da tarefa S4.1 no enunciado
 * @param lugaresEstacionamento (I) array de lugares de estacionamento que irá servir de BD
 * @param dimensaoMaximaParque (I) número máximo de lugares do parque, especificado pelo utilizador
 */
void s4_1_TerminaServidoresDedicados(Estacionamento *lugaresEstacionamento, int dimensaoMaximaParque) {
    so_debug("< [@param lugaresEstacionamento:%p, dimensaoMaximaParque:%d]", lugaresEstacionamento, dimensaoMaximaParque);

   // ENTRA NA ZONA CRÍTICA
    struct sembuf op_down = {SEM_MUTEX_BD, SEM_DOWN, NO_FLAGS};
    semop(semId, &op_down, 1);

    for (int i = 0; i < dimensaoMaximaParque; i++) {
        int pidSD = lugaresEstacionamento[i].pidServidorDedicado;
        if (pidSD > 0) {
            kill(pidSD, SIGUSR2);
        }
    }

    // SAI DA ZONA CRÍTICA
    struct sembuf op_up = {SEM_MUTEX_BD, 1, 0};
    semop(semId, &op_up, 1);

    so_success("S4.1", "SIGUSR2 enviado a todos os Servidores Dedicados");


    so_debug(">");
}

/**
 * @brief s4_2_AguardaFimServidoresDedicados Ler a descrição da tarefa S4.2 no enunciado
 * @param nrServidoresDedicados (I) número de Servidores Dedicados que foram criados até então
 */
void s4_2_AguardaFimServidoresDedicados(int nrServidoresDedicados) {
    so_debug("< [@param nrServidoresDedicados:%d]", nrServidoresDedicados);

    struct sembuf op;
    op.sem_num = SEM_SRV_DEDICADOS;
    op.sem_op = SEM_DOWN * nrServidoresDedicados; // SEM_DOWN está definido como -1
    op.sem_flg = NO_FLAGS;

    if (semop(semId, &op, 1) == -1) {
        so_error("S4.2", "Erro ao aguardar fim dos Servidores Dedicados");
        return;
    }

    so_success("S4.2", "Todos os Servidores Dedicados terminaram");

    so_debug(">");
}

/**
 * @brief s4_3_ApagaElementosIPCeTermina Ler a descrição da tarefa S4.2 no enunciado
 * @param shmId (I) identificador aberto de IPC
 * @param semId (I) identificador aberto de IPC
 * @param msgId (I) identificador aberto de IPC
 */
void s4_3_ApagaElementosIPCeTermina(int shmId, int semId, int msgId) {
    so_debug("< [@param shmId:%d, semId:%d, msgId:%d]", shmId, semId, msgId);


   // Apaga a SHM, SEM e MSG, ignorando erros
    shmctl(shmId, IPC_RMID, NULL);
    semctl(semId, 0, IPC_RMID);
    msgctl(msgId, IPC_RMID, NULL);

    so_success("S4.3", "Servidor: End Shutdown");
    exit(0);

    

    so_debug(">");
}

/**
 * @brief s5_TrataTerminouServidorDedicado Ler a descrição da tarefa S5 no enunciado
 * @param sinalRecebido (I) número do sinal que é recebido por esta função (enviado pelo SO)
 */
void s5_TrataTerminouServidorDedicado(int sinalRecebido) {
    so_debug("< [@param sinalRecebido:%d]", sinalRecebido);

    int status;
    pid_t pid;
    // Espera por todos os filhos terminados (pode haver mais do que um)
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        nrServidoresDedicados--; // Atualiza o número de SD ativos
        so_success("S5", "Servidor: Confirmo que terminou o SD %d", pid);
        // Incrementa o semáforo da barreira
        struct sembuf op = {SEM_SRV_DEDICADOS, 1, 0};
        semop(semId, &op, 1);
    }

    so_debug("> [@return nrServidoresDedicados:%d", nrServidoresDedicados);
}

// NÃO FIZ A PARTIR DAQUI
/**
 * @brief sd7_ServidorDedicado Ler a descrição da tarefa SD7 no enunciado
 *        OS ALUNOS NÃO DEVERÃO ALTERAR ESTA FUNÇÃO.
 */
void sd7_MainServidorDedicado() {
    so_debug("<");

    // sd7_IniciaServidorDedicado:
    sd7_1_ArmaSinaisServidorDedicado();
    sd7_2_ValidaPidCliente(clientRequest);
    sd7_3_GetShmFACE(KEY_FACE, &shmIdFACE);
    sd7_4_GetSemFACE(KEY_FACE, &semIdFACE);
    sd7_5_ProcuraLugarDisponivelBD(semId, clientRequest, lugaresEstacionamento, dimensaoMaximaParque, &indexClienteBD);

    // sd8_ValidaPedidoCliente:
    sd8_1_ValidaMatricula(clientRequest);
    sd8_2_ValidaPais(clientRequest);
    sd8_3_ValidaCategoria(clientRequest);
    sd8_4_ValidaNomeCondutor(clientRequest);

    // sd9_EntradaCliente:
    sd9_1_AdormeceTempoRandom();
    sd9_2_EnviaSucessoAoCliente(msgId, clientRequest);
    sd9_3_EscreveLogEntradaViatura(FILE_LOGFILE, clientRequest, &posicaoLogfile, &logItem);

    // sd10_AcompanhaCliente:
    sd10_1_AguardaCheckout(msgId);
    sd10_2_EscreveLogSaidaViatura(FILE_LOGFILE, posicaoLogfile, logItem);

    sd11_EncerraServidorDedicado();

    so_error("Servidor Dedicado", "O programa nunca deveria ter chegado a este ponto!");
    so_debug(">");
}

/**
 * @brief sd7_1_ArmaSinaisServidorDedicado Ler a descrição da tarefa SD7.1 no enunciado
 */
void sd7_1_ArmaSinaisServidorDedicado() {
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief sd7_2_ValidaPidCliente Ler a descrição da tarefa SD7.2 no enunciado
 * @param clientRequest (I) pedido recebido, enviado por um Cliente
 */
void sd7_2_ValidaPidCliente(MsgContent clientRequest) {
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.msgData.est.viatura.matricula, clientRequest.msgData.est.viatura.pais, clientRequest.msgData.est.viatura.categoria, clientRequest.msgData.est.viatura.nomeCondutor, clientRequest.msgData.est.pidCliente, clientRequest.msgData.est.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief sd7_3_GetShmFACE Ler a descrição da tarefa SD7.3 no enunciado
 * @param ipcKeyFace (I) Identificador de IPC a ser definida pela FACE
 * @param pshmIdFACE (O) identificador aberto de IPC da FACE
 */
void sd7_3_GetShmFACE(key_t ipcKeyFace, int *pshmIdFACE) {
    so_debug("< [@param ipcKeyFace:0x0%x]", ipcKeyFace);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug("> [@return *pshmIdFACE:%d]", *pshmIdFACE);
}

/**
 * @brief sd7_4_GetSemFACE Ler a descrição da tarefa SD7.4 no enunciado
 * @param ipcKeyFace (I) Identificador de IPC a ser definida pela FACE
 * @param psemIdFACE (O) identificador aberto de IPC da FACE
 */
void sd7_4_GetSemFACE(key_t ipcKeyFace, int *psemIdFACE) {
    so_debug("< [@param ipcKeyFace:0x0%x]", ipcKeyFace);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug("> [@return *psemIdFACE:%d]", *psemIdFACE);
}

/**
 * @brief sd7_5_ProcuraLugarDisponivelBD Ler a descrição da tarefa SD7.5 no enunciado
 * @param semId (I) identificador aberto de IPC
 * @param clientRequest (I) pedido recebido, enviado por um Cliente
 * @param lugaresEstacionamento (I) array de lugares de estacionamento que irá servir de BD
 * @param dimensaoMaximaParque (I) número máximo de lugares do parque, especificado pelo utilizador
 * @param pindexClienteBD (O) índice do lugar correspondente a este pedido na BD (>= 0), ou -1 se não houve nenhum lugar disponível
 */
void sd7_5_ProcuraLugarDisponivelBD(int semId, MsgContent clientRequest, Estacionamento *lugaresEstacionamento, int dimensaoMaximaParque, int *pindexClienteBD) {
    so_debug("< [@param semId:%d, clientRequest:[%s:%s:%c:%s:%d:%d], lugaresEstacionamento:%p, dimensaoMaximaParque:%d]", semId, clientRequest.msgData.est.viatura.matricula, clientRequest.msgData.est.viatura.pais, clientRequest.msgData.est.viatura.categoria, clientRequest.msgData.est.viatura.nomeCondutor, clientRequest.msgData.est.pidCliente, clientRequest.msgData.est.pidServidorDedicado, lugaresEstacionamento, dimensaoMaximaParque);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug("> [*pindexClienteBD:%d]", *pindexClienteBD);
}

/**
 * @brief  sd8_1_ValidaMatricula Ler a descrição da tarefa SD8.1 no enunciado
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 */
void sd8_1_ValidaMatricula(MsgContent clientRequest) {
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.msgData.est.viatura.matricula, clientRequest.msgData.est.viatura.pais, clientRequest.msgData.est.viatura.categoria, clientRequest.msgData.est.viatura.nomeCondutor, clientRequest.msgData.est.pidCliente, clientRequest.msgData.est.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  sd8_2_ValidaPais Ler a descrição da tarefa SD8.2 no enunciado
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 */
void sd8_2_ValidaPais(MsgContent clientRequest) {
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.msgData.est.viatura.matricula, clientRequest.msgData.est.viatura.pais, clientRequest.msgData.est.viatura.categoria, clientRequest.msgData.est.viatura.nomeCondutor, clientRequest.msgData.est.pidCliente, clientRequest.msgData.est.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  sd8_3_ValidaCategoria Ler a descrição da tarefa SD8.3 no enunciado
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 */
void sd8_3_ValidaCategoria(MsgContent clientRequest) {
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.msgData.est.viatura.matricula, clientRequest.msgData.est.viatura.pais, clientRequest.msgData.est.viatura.categoria, clientRequest.msgData.est.viatura.nomeCondutor, clientRequest.msgData.est.pidCliente, clientRequest.msgData.est.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  sd8_4_ValidaNomeCondutor Ler a descrição da tarefa SD8.4 no enunciado
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 */
void sd8_4_ValidaNomeCondutor(MsgContent clientRequest) {
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.msgData.est.viatura.matricula, clientRequest.msgData.est.viatura.pais, clientRequest.msgData.est.viatura.categoria, clientRequest.msgData.est.viatura.nomeCondutor, clientRequest.msgData.est.pidCliente, clientRequest.msgData.est.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief sd9_1_AdormeceTempoRandom Ler a descrição da tarefa SD9.1 no enunciado
 */
void sd9_1_AdormeceTempoRandom() {
    so_debug("<");

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief sd9_2_EnviaSucessoAoCliente Ler a descrição da tarefa SD9.2 no enunciado
 * @param msgId (I) identificador aberto de IPC
 * @param clientRequest (I) pedido recebido, enviado por um Cliente
 */
void sd9_2_EnviaSucessoAoCliente(int msgId, MsgContent clientRequest) {
    so_debug("< [@param msgId:%d, clientRequest:[%s:%s:%c:%s:%d:%d]]", msgId, clientRequest.msgData.est.viatura.matricula, clientRequest.msgData.est.viatura.pais, clientRequest.msgData.est.viatura.categoria, clientRequest.msgData.est.viatura.nomeCondutor, clientRequest.msgData.est.pidCliente, clientRequest.msgData.est.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief sd9_3_EscreveLogEntradaViatura Ler a descrição da tarefa SD9.3 no enunciado
 * @param logFilename (I) O nome do ficheiro de Logfile (i.e., FILE_LOGFILE)
 * @param clientRequest (I) pedido recebido, enviado por um Cliente
 * @param pposicaoLogfile (O) posição do ficheiro Logfile mesmo antes de inserir o log desta viatura
 * @param plogItem (O) registo de Log para esta viatura
 */
void sd9_3_EscreveLogEntradaViatura(char *logFilename, MsgContent clientRequest, long *pposicaoLogfile, LogItem *plogItem) {
    so_debug("< [@param logFilename:%s, clientRequest:[%s:%s:%c:%s:%d:%d]]", logFilename, clientRequest.msgData.est.viatura.matricula, clientRequest.msgData.est.viatura.pais, clientRequest.msgData.est.viatura.categoria, clientRequest.msgData.est.viatura.nomeCondutor, clientRequest.msgData.est.pidCliente, clientRequest.msgData.est.pidServidorDedicado);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug("> [*pposicaoLogfile:%ld, *plogItem:[%s:%s:%c:%s:%s:%s]]", *pposicaoLogfile, plogItem->viatura.matricula, plogItem->viatura.pais, plogItem->viatura.categoria, plogItem->viatura.nomeCondutor, plogItem->dataEntrada, plogItem->dataSaida);
}

/**
 * @brief  sd10_1_AguardaCheckout Ler a descrição da tarefa SD10.1 no enunciado
 * @param msgId (I) identificador aberto de IPC
 */
void sd10_1_AguardaCheckout(int msgId) {
    so_debug("< [@param msgId:%d]", msgId);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief  sd10_1_1_TrataAlarme Ler a descrição da tarefa SD10.1.1 no enunciado
 * @param  sinalRecebido (I) número do sinal que é recebido por esta função (enviado pelo SO)
 */
void sd10_1_1_TrataAlarme(int sinalRecebido) {
    so_debug("< [@param sinalRecebido:%d]", sinalRecebido);

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

    sd11_1_LibertaLugarViatura(semId, lugaresEstacionamento, indexClienteBD);
    sd11_2_EnviaTerminarAoClienteETermina(msgId, clientRequest);

    so_debug(">");
}

/**
 * @brief sd11_1_LibertaLugarViatura Ler a descrição da tarefa SD11.1 no enunciado
 * @param semId (I) identificador aberto de IPC
 * @param lugaresEstacionamento (I) array de lugares de estacionamento que irá servir de BD
 * @param indexClienteBD (I) índice do lugar correspondente a este pedido na BD (>= 0), ou -1 se não houve nenhum lugar disponível
 */
void sd11_1_LibertaLugarViatura(int semId, Estacionamento *lugaresEstacionamento, int indexClienteBD) {
    so_debug("< [@param semId:%d, lugaresEstacionamento:%p, indexClienteBD:%d]", semId, lugaresEstacionamento, indexClienteBD);

    // Substituir este comentário pelo código da função a ser implementado pelo aluno

    so_debug(">");
}

/**
 * @brief sd11_2_EnviaTerminarAoClienteETermina Ler a descrição da tarefa SD11.2 no enunciado
 * @param msgId (I) identificador aberto de IPC
 * @param clientRequest (I) pedido recebido, enviado por um Cliente
 */
void sd11_2_EnviaTerminarAoClienteETermina(int msgId, MsgContent clientRequest) {
    so_debug("< [@param msgId:%d, clientRequest:[%s:%s:%c:%s:%d:%d]]", msgId, clientRequest.msgData.est.viatura.matricula, clientRequest.msgData.est.viatura.pais, clientRequest.msgData.est.viatura.categoria, clientRequest.msgData.est.viatura.nomeCondutor, clientRequest.msgData.est.pidCliente, clientRequest.msgData.est.pidServidorDedicado);

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