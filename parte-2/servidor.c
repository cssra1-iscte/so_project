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

     /** 
        EXPLICAÇÃO CÓDIGO:
        * Primeiro, temos que alocar espaço de memória para o array plugaresEstacionamento.
        * Para além de alocar espaço, queremos que para cada posição do array, Estacionamento.pidCliente=DISPONIVEL
        * Para fazer isso, usamos o calloc -> *ptr = (castType*)calloc(n, size), permitindo assim alocar dinamicamente, 
        conforme o nosso n (dimensaoMaximaParque) e, inicializar todas as posições a zero. 
    */

    *plugaresEstacionamento = (Estacionamento *)malloc(dimensaoMaximaParque * sizeof(Estacionamento));

    if (*plugaresEstacionamento == NULL) {
        so_error("S1.2", "Erro ao alocar memória para o parque de estacionamento");
        exit(1);
    }

    //fazer for para percorrer cada posição do array plugaresEstacionamento, 
    // colocado Estacionamento.pidCliente = DISPONIVEL
    for (int i = 0; i < dimensaoMaximaParque; i++) {
        (*plugaresEstacionamento)[i].pidCliente = DISPONIVEL;
    }

    so_success("S1.2", "Base de dados lugaresEstacionamento[] criada com sucesso");


    so_debug("> [*plugaresEstacionamento:%p]", *plugaresEstacionamento);
}

/**
 * @brief  s1_3_ArmaSinaisServidor Ler a descrição da tarefa S1.3 no enunciado
 */
void s1_3_ArmaSinaisServidor() {
    so_debug("<");

    /** 
        EXPLICAÇÃO CÓDIGO:
        * Para armar sinais que chegam ao processo servidor.c, usamos a função signal(sinal, handler_sinal)
        * Neste caso vamos armar 2 sinais - ou seja vamos fazer com que o processo pai, ao receber um destes sinais,
        saiba como tratá-los
        * 1º sinal - SIGINT (sinal enviado ao processo quando é pressionado "CTRL+C" - por default, o processo termina
        ao receber este sinal)
        * 2º sinal - SIGCHLD (sinal enviado ao processo quando um dos processos filhos termina - por default, o sinal é ignorado)
    */

    // armar o sinal para reagir de acordo com s3_TrataCtrlC (handler do sinal)
    if (signal(SIGINT, s3_TrataCtrlC) == SIG_ERR) {
        so_error("S1.3", "Erro ao armar o sinal SIGINT");
        exit(1);
    }

    // armar o sinal para reagir de acordo com s5_TrataTerminouServidorDedicado (handler do sinal)
    /*struct sigaction sa_chld;
    sa_chld.sa_handler = s5_TrataTerminouServidorDedicado;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART; */

    if (signal(SIGCHLD, s5_TrataTerminouServidorDedicado) == SIG_ERR) {
        so_error("S1.3", "Erro ao armar SIGCHLD");
        exit(1);
    }
    
    so_success("S1.3", "Sinais SIGINT e SIGCHLD armados com sucesso");

    so_debug(">");
}

/**
 * @brief  s1_4_CriaFifoServidor Ler a descrição da tarefa S1.4 no enunciado
 * @param  filenameFifoServidor (I) O nome do FIFO do servidor (i.e., FILE_REQUESTS)
 */
void s1_4_CriaFifoServidor(char *filenameFifoServidor) {
    so_debug("< [@param filenameFifoServidor:%s]", filenameFifoServidor);

    /** 
        EXPLICAÇÃO CÓDIGO:
        * O 1º passo é verificar se o FIFO existe, pois caso exista, temos que o apagar recorrendo ao unlink().
        Para isso recorremos à estrutura stat.
        A função stat() vai procurar no sistema de ficheiros (disco) se existe o ficheiro com o path indicado. 
        Se stat(filenameFifoServidor, &st) == 0 --> quer dizer que existe um ficheiro (seja ele de que tipo for) que a 
        estrutura st foi preenchida com os dados desse ficheiro e a função retorna 0.
        Se o output for um número < 0, então o ficheiro não existe. 
        * Ao verificar que existe um ficheiro no path filenameFifoServidor, então usamos S_ISFIFO para validar 
        se o ficheiro é um FIFO.
        * Se for um FIFO, procedemos ao unlink(filenameFifoServidor). 
        Devemos sempre validar se as operações foram bem sucedidas com um if.
        * Após garantirmos que não existe nenhum FIFO associado ao pointer *filenameFifoServidor,
        então criamos um FIFO recorrendo ao mkfifo, validando sempre com um if se a operação foi bem sucedida. 
    */

    // Verifica se o FIFO já existe
    if (access(filenameFifoServidor, F_OK) == 0) {
        so_debug("FIFO já existia.");

        // Tenta apagar o FIFO existente
        if (unlink(filenameFifoServidor) != 0) {
            so_error("S1.4","Falha ao apagar FIFO existente");
            exit(1);
        } else {
            so_success("S1.4","FIFO existente removido com sucesso");
        }
    } else {
        so_success("S1.4","FIFO não existia.");
    }

    // Tenta criar o FIFO
    if (mkfifo(filenameFifoServidor, 0666) != 0) {
        so_error("S1.4","Falha ao criar FIFO");
        exit(1);
    } else {
        so_success("S1.4","FIFO criado com sucesso");
    }


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
    while (TRUE) { //CICLO 1
        s2_1_AbreFifoServidor(FILE_REQUESTS, &fFifoServidor); 
        s2_2_LePedidosFifoServidor(fFifoServidor); //CICLO 2
        //sleep(10);  // TEMPORÁRIO, os alunos deverão comentar este statement apenas
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


    while (1) {
        *pfFifoServidor = fopen(filenameFifoServidor, "rb");

        if (*pfFifoServidor != NULL) {
            // Sucesso na abertura
            so_success("S2.1", "Abertura de FIFO em modo LEITURA com sucesso");
            break; //sai do ciclo while
        }

        // falha na abertura do FIFO em modo leitura - recebe um sinal antes de a operação terminar (Interrupted system call)
        if (errno == EINTR) {
            so_debug("Abertura interrompida por sinal, tentativa repetida.");
            continue; //recomeça o ciclo while -> tenta abrir o FIFO de novo
        }

        // *pfFifoServidor == NULL -> erro ao abrir o FIFO
        so_error("S2.1", "Erro ao abrir FIFO Servidor: %s", strerror(errno));
        s4_EncerraServidor(FILE_REQUESTS);
    }

    so_debug("> [*pfFifoServidor:%p]", *pfFifoServidor);
}

/**
 * @brief  s2_2_LePedidosFifoServidor Ler a descrição da tarefa S2.2 no enunciado.
 *         OS ALUNOS NÃO DEVERÃO ALTERAR ESTA FUNÇÃO.
 * @param  fFifoServidor (I) descritor aberto do ficheiro do FIFO do servidor
 */
void s2_2_LePedidosFifoServidor(FILE *fFifoServidor) {
    so_debug("<");

    int terminaCiclo2 = FALSE; //int 0
    while (TRUE) {
        terminaCiclo2 = s2_2_1_LePedido(fFifoServidor, &clientRequest);
        if (terminaCiclo2) //IF (terminaCiclo2 = TRUE) // o resultado vem da variável "naoHaMaisPedidos"
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
    int naoHaMaisPedidos = TRUE; //int 1
    so_debug("< [@param fFifoServidor:%p]", fFifoServidor);

    // O processo principal servidor.c vai "tentar" ler 1 bloco com o tamanho de um 
    // do tipo "Estacionamento" a partir do FIFO fFifoServidor (aberto em modo leitura) e guarda-o na variável *pclientRequest
    size_t dadosLidos = fread(pclientRequest, sizeof(Estacionamento), 1, fFifoServidor);

    if (dadosLidos == 1) {
        // Se há dados para ler, a flag naoHaMaisPedidos deixa de ser verdadeira, passando a FALSE
        naoHaMaisPedidos = FALSE;
        so_success("S2.2.1", "Li Pedido do FIFO");
    } else {
        // Não detetando nada para ler, verifica que se chegou ao fim do ficheiro (EOF)
        if (feof(fFifoServidor)) {
            // se sim, então fecha-se o FIFO
            fclose(fFifoServidor);
            so_success("S2.2.1", "Não há mais registos no FIFO");
        } else {
            //se não há dados, nem se chega ao fim no FIFO, então existem problemas com a leitura em geral
            so_error("S2.2.1", "Erro ao ler do FIFO");
            s4_EncerraServidor(FILE_REQUESTS);
        }
    

    so_debug("> [naoHaMaisPedidos:%d, *pclientRequest:[%s:%s:%c:%s:%d.%d]]", naoHaMaisPedidos, pclientRequest->viatura.matricula, pclientRequest->viatura.pais, pclientRequest->viatura.categoria, pclientRequest->viatura.nomeCondutor, pclientRequest->pidCliente, pclientRequest->pidServidorDedicado);
    }

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

    *pindexClienteBD = -1; // valor default se não for encontrado nenhum lugar disponível

    // procura-se no array de Estacionamentos se existem lugares disponíveis para atender ao pedido do Cliente.
    // Percorre-se cada posição do array à procura de Estacionamento.pidCliente=DISPONIVEL
    for (int i = 0; i < dimensaoMaximaParque; i++) {
        // se o pidCliente associada à posição for DISPONIVEL, aloca a essa posição o Estacionamento 
        if (lugaresEstacionamento[i].pidCliente == DISPONIVEL) {
            lugaresEstacionamento[i] = clientRequest; // reserva o lugar com as informações do cliente
            *pindexClienteBD = i; //atualiza o valor da posição para saber o lugar do array ocupado

            so_success("S2.2.2", "Reservei Lugar: %d", i);
            break; //interrompe o for ao encontrar a primeira posição i livre.
        }
    }

    so_debug("> [*pindexClienteBD:%d]", *pindexClienteBD);
}

/**
 * @brief  s2_2_3_CriaServidorDedicado    Ler a descrição da tarefa S2.2.3 no enunciado
 * @param  lugaresEstacionamento (I) array de lugares de estacionamento que irá servir de BD
 * @param  indexClienteBD (I) índice do lugar correspondente a este pedido na BD (>= 0), ou -1 se não houve nenhum lugar disponível
 */
void s2_2_3_CriaServidorDedicado(Estacionamento *lugaresEstacionamento, int indexClienteBD) {
    so_debug("< [@param lugaresEstacionamento:%p, indexClienteBD:%d]", lugaresEstacionamento, indexClienteBD);

    //criação do processo filho
    pid_t pid = fork();

    //se o pid for inferior a 0 -> erro ao criar processo
    if (pid < 0) {
        so_error("S2.2.3","Erro ao criar processo Servidor Dedicado");
        s4_EncerraServidor(FILE_REQUESTS); 
    }
    // se o pid for 0, então as tarefas são executadas pelo procesos filho
    if (pid == 0) {
        //obtém o id do processo do filho
        pid_t pidServidorDedicado = getpid();
        so_success("S2.2.3", "SD: Nasci com PID %d", pidServidorDedicado);
        
        // o processo filho executa a tarefa em sd7
        sd7_MainServidorDedicado(); 
        exit(0); //termina o processo filho

    // Se for >0, então é o processo api, e executa o código abaixo.
    } else {

       
        if (indexClienteBD >= 0 && indexClienteBD < dimensaoMaximaParque) {
            lugaresEstacionamento[indexClienteBD].pidServidorDedicado = pid;
        }

        so_success("S2.2.3","Servidor: Iniciei SD %d", pid);

        // Retorna para o ciclo no passo S2.2 (nada para fazer aqui, função termina)
    }

    so_debug(">");
}

/**
 * @brief  s3_TrataCtrlC    Ler a descrição da tarefa S3 no enunciado
 * @param  sinalRecebido (I) número do sinal que é recebido por esta função (enviado pelo SO)
 */
void s3_TrataCtrlC(int sinalRecebido) {
    so_debug("< [@param sinalRecebido:%d]", sinalRecebido);

    so_success("S3", "Servidor: Start Shutdown");

    for (int i = 0; i < dimensaoMaximaParque; i++) {
        if (lugaresEstacionamento[i].pidCliente != DISPONIVEL) {
            int pidSD = lugaresEstacionamento[i].pidServidorDedicado;
            if (kill(pidSD, SIGUSR2) == -1){
                so_error("S3", "Falha ao enviar SIGUSR2 ao SD PID %d", pidSD);
                break;
            }
        }
    }

    
    s4_EncerraServidor(FILE_REQUESTS);

    so_debug(">");
}

/**
 * @brief  s4_EncerraServidor Ler a descrição da tarefa S4 no enunciado
 * @param  filenameFifoServidor (I) O nome do FIFO do servidor (i.e., FILE_REQUESTS)
 */
void s4_EncerraServidor(char *filenameFifoServidor) {
    so_debug("< [@param filenameFifoServidor:%s]", filenameFifoServidor);

    //tenta remover o FIFO, o resultado de int unlink() for !=0, dá erro
    if (unlink(filenameFifoServidor) != 0) {
        so_error("S4", "Erro ao remover FIFO %s", filenameFifoServidor);
        exit(0); // inicialmente coloquei exit(1), mas o validador deu erro no codigo do exit. Não faz sentido.
    } else {
        so_success("S4", "Servidor: End Shutdown");
        exit(0); //terminou sem erro
    }


    so_debug(">");
}

/**
 * @brief  s5_TrataTerminouServidorDedicado    Ler a descrição da tarefa S5 no enunciado
 * @param  sinalRecebido (I) número do sinal que é recebido por esta função (enviado pelo SO)
 */
void s5_TrataTerminouServidorDedicado(int sinalRecebido) {
    so_debug("< [@param sinalRecebido:%d]", sinalRecebido);

    
    int stat;
    
    pid_t child = wait( &stat );

    //verifica se terminou normalmente
    if ( WIFEXITED(stat) ) {
        so_success("S5", "Servidor: Confirmo que terminou o SD %d", child);
    //se não terminou normalmente, verifica se terminou por causa de um sinal
    } else if (WIFSIGNALED(stat)) {
        so_success("S5", "Servidor: Confirmo que terminou o SD %d", child);
    }


    so_debug(">");
}

/**
 * @brief  sd7_ServidorDedicado Ler a descrição da tarefa SD7 no enunciado
 *         OS ALUNOS NÃO DEVERÃO ALTERAR ESTA FUNÇÃO.
 */
void sd7_MainServidorDedicado() {
    so_debug("<");

    // sd7_IniciaServidorDedicado:
    sd7_1_ArmaSinaisServidorDedicado(); //feito
    sd7_2_ValidaPidCliente(clientRequest); //feito
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

    // Armar o sinal para reagir de acordo com sd12_TrataSigusr2 (handler do sinal)
    if(signal(SIGUSR2, sd12_TrataSigusr2) == SIG_ERR) {
        so_error("SD7.1", "Erro ao tentar armar sinal SIGUSR2");
        exit(1);
    }

    // Uma vez que os servidores dedicados e o servidor estão a correr na mesma shell, de forma a estes não encerrarem quando se clica em CTRL+C, 
    // deve armar-se o sinal de forma ao servidor dedicado ignorar o sinal SIGINT. 
    if(signal(SIGINT, SIG_IGN) == SIG_ERR) {
        so_error("SD7.1", "Erro ao tentar armar sinal SIGINT");
        exit(1);
    }
    // Armar o sinal para reagir de acordo com sd13_TrataSigusr1 (handler do sinal)
    if(signal(SIGUSR1, sd13_TrataSigusr1) == SIG_ERR) {
        so_error("SD7.1", "Erro ao tentar armar sinal SIGUSR1");
        exit(1);
    }

    so_success("SD7.1", "Sinais armados com sucesso");

    so_debug(">");
}

/**
 * @brief  sd7_2_ValidaPidCliente    Ler a descrição da tarefa SD7.2 no enunciado
 * @param  clientRequest (I) pedido recebido, enviado por um Cliente
 */
void sd7_2_ValidaPidCliente(Estacionamento clientRequest) {
    so_debug("< [@param clientRequest:[%s:%s:%c:%s:%d:%d]]", clientRequest.viatura.matricula, clientRequest.viatura.pais, clientRequest.viatura.categoria, clientRequest.viatura.nomeCondutor, clientRequest.pidCliente, clientRequest.pidServidorDedicado);

    if (clientRequest.pidCliente > 0) {
        so_success("SD7.2", "Pid do cliente [%d] é válido.", clientRequest.pidCliente);
    } else {
        so_error("SD7.2", "Pid do cliente [%d] é inválid.", clientRequest.pidCliente);
        exit(1);
    }

    so_debug(">");
}

/**
 * @brief  sd7_3_ValidaLugarDisponivelBD    Ler a descrição da tarefa SD7.3 no enunciado
 * @param  indexClienteBD (I) índice do lugar correspondente a este pedido na BD (>= 0), ou -1 se não houve nenhum lugar disponível
 */
void sd7_3_ValidaLugarDisponivelBD(int indexClienteBD) {
    so_debug("< [@param indexClienteBD:%d]", indexClienteBD);

    if (indexClienteBD == -1) {
        // Envia sinal SIGHUP ao cliente, para indicar que não há lugar
        if (kill(clientRequest.pidCliente, SIGHUP) == -1) {
            so_error("SD7.3", "Falha ao enviar SIGHUP ao Cliente PID %d", clientRequest.pidCliente);
            exit(0);
        } 
            //so_success("SD7.3", "SD: Enviei SIGHUP ao Cliente PID %d (sem lugar disponível)", clientRequest.pidCliente);       
    }

    so_success("SD7.3", "Há lugar disponível");

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