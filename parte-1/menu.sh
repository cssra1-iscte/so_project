#!/bin/bash
# SO_HIDE_DEBUG=1                   ## Uncomment this line to hide all @DEBUG statements
# SO_HIDE_COLOURS=1                 ## Uncomment this line to disable all escape colouring
. so_utils.sh                       ## This is required to activate the macros so_success, so_error, and so_debug

#####################################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos 2024/2025, Enunciado Versão 1
##
## Aluno: Nº:  82442    Nome: Carolina Salvador
## Nome do Módulo: S4. Script: menu.sh
## Descrição/Explicação do Módulo:
## v2.0
## ATUALIZAÇÃO - ficheiro corrido no validador com sucesso
#####################################################################################

## Este script invoca os scripts restantes, não recebendo argumentos.
## Atenção: Não é suposto que volte a fazer nenhuma das funcionalidades dos scripts anteriores. O propósito aqui é simplesmente termos uma forma centralizada de invocar os restantes scripts.
## S4.1. Apresentação:
## S4.1.1. O script apresenta (pode usar echo, cat ou outro, sem “limpar” o ecrã) um menu com as opções abaixo indicadas.

## S4.2. Validações:
## S4.2.1. Aceita como input do utilizador um número. Valida que a opção introduzida corresponde a uma opção válida. Se não for, dá so_error <opção> (com a opção errada escolhida), e volta ao passo S4.1 (ou seja, mostra novamente o menu). Caso contrário, dá so_success <opção>.
## S4.2.2. Analisa a opção escolhida, e mediante cada uma delas, deverá invocar o sub-script correspondente descrito nos pontos S1 a S3 acima. No caso das opções 1 e 4, este script deverá pedir interactivamente ao utilizador as informações necessárias para execução do sub-script correspondente, injetando as mesmas como argumentos desse sub-script:
## S4.2.2.1. Assim sendo, no caso da opção 1, o script deverá pedir ao utilizador sucessivamente e interactivamente os dados a inserir:


## Este script não deverá fazer qualquer validação dos dados inseridos, já que essa validação é feita no script S1. Após receber os dados, este script invoca o Sub-Script: regista_passagem.sh com os argumentos recolhidos do utilizador. Após a execução do sub-script, dá so_success e volta ao passo S4.1.
## S4.2.2.2. No caso da opção 2, o script deverá pedir ao utilizador sucessivamente e interactivamente os dados a inserir:
##  Este script não deverá fazer qualquer validação dos dados inseridos, já que essa validação é feita no script S1. Após receber os dados, este script invoca o Sub-Script: regista_passagem.sh com os argumentos recolhidos do utilizador. Após a execução do sub-script, dá so_success e volta ao passo S4.1.
## S4.2.2.3. No caso da opção 3, o script invoca o Sub-Script: manutencao.sh. Após a execução do sub-script, dá so_success e volta para o passo S4.1.
## S4.2.2.4. No caso da opção 4, o script deverá pedir ao utilizador as opções de estatísticas a pedir, antes de invocar o Sub-Script: stats.sh. Se uma das opções escolhidas for a 8, o menu deverá invocar o Sub-Script: stats.sh sem argumentos, para que possa executar TODAS as estatísticas, caso contrário deve respeitar a ordem.
## Após a execução do Sub-Script: stats.sh, dá so_success e volta para o passo S4.1.


## Apenas a opção 0 (zero) permite sair deste Script: menu.sh. Até escolher esta opção, o menu deverá ficar em ciclo, permitindo realizar múltiplas operações iterativamente (e não recursivamente, ou seja, não deverá chamar o Script: menu.sh novamente). 

while true; do
    echo ""
    echo "MENU:"
    echo "1: Regista passagem – Entrada estacionamento"
    echo "2: Regista passagem – Saída estacionamento"
    echo "3: Manutenção"
    echo "4: Estatísticas"
    echo "0: Sair"
    echo ""
    echo -n "Opção: "
    read opcao
    echo "_"
    echo ""

    if [[ "$opcao" =~ ^[0-4]$ ]]; then
        so_success S4.2.1 "$opcao"
    fi

    case $opcao in
        1) 
            echo "Regista passagem de Entrada estacionamento:"
            read -p "Indique a matrícula da viatura: " matricula
            read -p "Indique o código do país de origem da viatura: " codigo_pais
            read -p "Indique a categoria da viatura [L(igeiro)|P(esado)|M(otociclo)]: " categoria
            read -p "Indique o nome do condutor da viatura: " nome
            echo ""
            ./regista_passagem.sh "$matricula" "$codigo_pais" "$categoria" "$nome" # sem "aspas" a variável $nome faz com que o numero de argumentos lidos seja > 4
            so_success S4.3
            ;;
        2) 
            echo "Regista passagem de Saída estacionamento:"
            read -p "Indique a matrícula da viatura: " matricula
            read -p "Indique o código do país de origem da viatura: " codigo_pais
            echo ""
            ./regista_passagem.sh "$codigo_pais/$matricula"
            so_success S4.4
            ;;


        3) 
            ./manutencao.sh
            so_success S4.5
            ;;
        4) 
            echo "Estatísticas:"
            echo "1: matrículas e condutores cujas viaturas estão ainda estacionadas no parque"
            echo "2: top3 das matrículas das viaturas que passaram mais tempo estacionadas"
            echo "3: tempos de estacionamento de ligeiros e pesados agrupadas por país"
            echo "4: top3 das matrículas das viaturas que estacionaram mais tarde num dia"
            echo "5: tempo total de estacionamento por utilizador"
            echo "6: matrículas e tempo total de estacionamento delas, agrupadas por país da matrícula"
            echo "7: top3 nomes de condutores mais compridos"
            echo "8: todas as estatísticas anteriores, na ordem numérica indicada"
            echo ""
            read -p "Indique quais as estatísticas a incluir, opções separadas por espaço: " opcoes
            echo "_"
            echo ""

            if [[ -z "$opcoes" ]]; then
                so_error S4.6 "tente novamente"
            else
            
                if [[ $opcoes =~ (^|[[:space:]])8($|[[:space:]]) ]]; then
                    ./stats.sh
                else
                    num_args=$(echo $opcoes | wc -w)
                    echo "Número de args: $num_args"
                    ./stats.sh $opcoes
                fi
                so_success S4.6
            
            fi
            ;;
        0)
            
            echo "A Sair do Menu.."
            echo ""
            exit 0
            ;;

        *)
            so_error S4.2.1 "$opcao"
            echo ""

    esac

done