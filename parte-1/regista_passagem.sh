#!/bin/bash
# SO_HIDE_DEBUG=1                   ## Uncomment this line to hide all @DEBUG statements
# SO_HIDE_COLOURS=1                 ## Uncomment this line to disable all escape colouring
. so_utils.sh                       ## This is required to activate the macros so_success, so_error, and so_debug

#####################################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos 2024/2025, Enunciado Versão 1
##
## Aluno: Nº: 82442   Nome: Carolina Salvador
## Nome do Módulo: S1. Script: regista_passagem.sh
## Descrição/Explicação do Módulo:
##
## SCRIPT VALIDADO NO VALIDADOR. TUDO OK
##
#####################################################################################

## Este script é invocado quando uma viatura entra/sai do estacionamento Park-IUL. Este script recebe todos os dados por argumento, na chamada da linha de comandos, incluindo os <Matrícula:string>, <Código País:string>, <Categoria:char> e <Nome do Condutor:string>.

## S1.1. Valida os argumentos passados e os seus formatos:
## • Valida se os argumentos passados são em número suficiente (para os dois casos exemplificados), assim como se a formatação de cada argumento corresponde à especificação indicada. O argumento <Categoria> pode ter valores: L (correspondente a Ligeiros), P (correspondente a Pesados) ou M (correspondente a Motociclos);
## • A partir da indicação do argumento <Código País>, valida se o argumento <Matrícula> passada cumpre a especificação da correspondente <Regra Validação Matrícula>;
## • Valida se o argumento <Nome do Condutor> é o “primeiro + último” nomes de um utilizador atual do Tigre;
## • Em caso de qualquer erro das condições anteriores, dá so_error S1.1 <descrição do erro>, indicando o erro em questão, e termina. Caso contrário, dá so_success S1.1.

if [[ ! -f "estacionamentos.txt" ]]; then
    touch estacionamentos.txt
fi


if [[ ! -f "paises.txt" ]]; then
    so_error S1.1 "Ficheiro paises.txt não existe"
    exit 1
fi

if [[ ! -r "paises.txt" ]]; then
    so_error S1.1 "Ficheiro paises.txt SEM PERMISSÃO de leitura"
    exit 1
fi


if [[ $# -ne 4 ]]; then
    if [[ $# -ne 1 || "$1" != */* ]]; then
        so_error S1.1 "Número de argumentos inválido"
        echo "Número de argumentos: $#"
        exit 1
    else 
        #so_success S1.1 "Número de argumentos válido - 1 arg"
        flag=saida
        pais=$(echo "$1" | awk -F'/' '{print $1}')
        matricula=$(echo "$1" | awk -F'/' '{print $2}')
        #echo "País: $pais"
        #echo "Matrícula: $matricula"
        
    fi
else # se for IGUAL a 4
    flag=entrada
    matricula="$1"
    pais="$2"
    categoria="$3"
    nome="$4"

    if [[ ! "$categoria" =~ ^(L|P|M)$ ]]; then
        so_error S1.1 "Categoria inválida. Só são admitidos os valores: L, P e M"   
        exit 1
    fi

    num_nomes=$(echo "$nome" | wc -w)
    primeiro_nome=$(echo "$nome" | awk '{print $1}')
    ultimo_nome=$(echo "$nome" | awk '{print $2}')
    #echo "$num_nomes"

    if [[ "$num_nomes" -ne 2 || ! $(cut -d: -f5 _etc_passwd | awk -F ',' '{print $1}' | grep -E "^${primeiro_nome}.*${ultimo_nome}$") ]]; then
        so_error S1.1 "Nome do condutor inválido ou formato incorreto (deve conter apenas primeiro e último nome)"
        exit 1
    #else 
     #   so_success S1.1 "Nome Válido: 1º Nome: $primeiro_nome / 2º Nome: $ultimo_nome"
    fi

fi

if [[ -f paises.txt ]]; then
    if ! grep -q "^$pais###" paises.txt; then
        so_error S1.1 "Linha NÃO ENCONTRADA"
        exit 1
    else
        regra=$(cat paises.txt | grep "^$pais###" | awk -F '###' '{print $3}')
        
        if ! [[ "$matricula" =~ $regra ]]; then
            so_error S1.1 "Matrícula $matricula não segue a formatação correta"
            exit 1
        else
            so_success S1.1 "Número de Campos Correto"
        fi

    fi
else
    so_error S1.1 "ficheiro não existe"
    exit 1
fi


## S1.2. Valida os dados passados por argumento para o script com o estado da base de dados de estacionamentos especificada no ficheiro estacionamentos.txt:
## • Valida se, no caso de a invocação do script corresponder a uma entrada no parque de estacionamento, se ainda não existe nenhum registo desta viatura na base de dados;
## • Valida se, no caso de a invocação do script corresponder a uma saída do parque de estacionamento, se existe um registo desta viatura na base de dados;
## • Em caso de qualquer erro das condições anteriores, dá so_error S1.2 <descrição do erro>, indicando o erro em questão, e termina. Caso contrário, dá so_success S1.2.

# TO DO --> DONE



matricula_formatada=$(echo $matricula | sed 's/[^A-Z0-9]//g')

num_campos=$(cat estacionamentos.txt | grep "^$matricula_formatada" | tail -n1 | awk -F ':' '{print NF}')

if [ "$flag" == "entrada" ]; then
    if  grep -q "^$matricula_formatada" estacionamentos.txt; then # verificar se não existe registo na base de dados
        if [[ "$num_campos" -ne 6 ]]; then  # verificar se o último registo da matrícula possui uma saída (6 argumento). Se não, então existe ainda um carro por sair
            so_error S1.2 "A viatura com a matrícula $matricula JÁ DEU ENTRADA"
            exit 1
        else
            so_success S1.2 "Viatura encontra-se válida para dar entrda"
        fi
    else
        so_success S1.2 "Viatura encontra-se válida para dar entrda"
    fi

fi

# TO DO ---> REFORMULAR VALIDAÇÃO   
if [ "$flag" == "saida" ]; then
    if  grep -q "^$matricula_formatada" estacionamentos.txt; then #verificar se existe regito na base de dados
        #se existir, verifica se o último registo sobre a matrícula tem valor no campo $6, ou seja, já deu saída
        if [[ "$num_campos" -eq 6 ]]; then
            so_error S1.2 "Saída Já efetuada"
            exit 1
        else
            so_success S1.2 "Viatura encontra-se VÁLIDA para sair"
        fi
    else
        so_error S1.2 "A viatura com matrícula $matricula AINDA NÃO SE ENCONTRA NO SISTEMA"
        exit 1
    fi
fi

## S1.3. Atualiza a base de dados de estacionamentos especificada no ficheiro estacionamentos.txt:
## • Remova do argumento <Matrícula> passado todos os separadores (todos os caracteres que não sejam letras ou números) eventualmente especificados;
## • Especifique como data registada (de entrada ou de saída, conforme o caso) a data e hora do sistema Tigre;
## • No caso de um registo de entrada, crie um novo registo desta viatura na base de dados;
## • No caso de um registo de saída, atualize o registo desta viatura na base de dados, registando a data de saída;
## • Em caso de qualquer erro das condições anteriores, dá so_error S1.3 <descrição do erro>, indicando o erro em questão, e termina. Caso contrário, dá so_success S1.3.



if [[ ! "$matricula_formatada" =~ ^[A-Z0-9]+$ ]]; then # verifica regex 
    so_error S1.3 "Matrícula formatada inválida!"
    exit 1
fi

data_registada=$(date "+%Y-%m-%dT%Hh%M")

if [ "$flag" == "entrada" ]; then

    if [[ ! -w "estacionamentos.txt" ]]; then
        so_error S1.3 "Ficheiro estacionamentos.txt SEM PERMISSÃO de escrita"
        exit 1
    fi

    echo "$matricula_formatada:$pais:$categoria:$primeiro_nome $ultimo_nome:$data_registada" >> estacionamentos.txt #linha nova ADICIONADA
    linha=$(cat estacionamentos.txt | tail -n1)
    so_success S1.3 "Nova ENTRADA em estacionamentos.txt - $linha"
else
    if [ "$flag" == "saida" ]; then

        posicao_linha=$(grep -n "^$matricula_formatada:" estacionamentos.txt | tail -n 1 | cut -d: -f1)

        if [[ ! -w "estacionamentos.txt" ]]; then
            so_error S1.3 "Ficheiro estacionamentos.txt SEM PERMISSÃO de escrita"
            exit 1
        fi

        sed -i "${posicao_linha}s/$/:$data_registada/" estacionamentos.txt #linha existente EDITADA
        linha=$(grep "^$matricula_formatada:" estacionamentos.txt | tail -n 1)
        so_success S1.3 "A viatura com matrícula $matricula saiu - $linha"
    else
       so_error S1.3 "Erro na atualização da BD estacionamentos.txt"
    fi
fi

## S1.4. Lista todos os estacionamentos registados, mas ordenados por saldo:
## • O script deve criar um ficheiro chamado estacionamentos-ordenados-hora.txt igual ao que está no ficheiro estacionamentos.txt, com a mesma formatação, mas com os registos ordenados por ordem crescente da hora (e não da data) de entrada das viaturas.
## • Em caso de qualquer erro das condições anteriores, dá so_error S1.4 <descrição do erro>, indicando o erro em questão, e termina. Caso contrário, dá so_success S1.4.

# TO DO NO FINAL ---> Verificar se se consegue abrir o file 
sort -t ":" -k5.12,5.16 estacionamentos.txt > estacionamentos-ordenados-hora.txt
so_success S1.4 "BD estacionamentos-ordenados-hora.txt CRIADA COM SUCEESO"
