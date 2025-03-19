#!/bin/bash
# SO_HIDE_DEBUG=1                   ## Uncomment this line to hide all @DEBUG statements
# SO_HIDE_COLOURS=1                 ## Uncomment this line to disable all escape colouring
. so_utils.sh                       ## This is required to activate the macros so_success, so_error, and so_debug
# Atualiza o crontab com as configurações do ficheiro cron.def
#crontab cron.def
#####################################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos 2024/2025, Enunciado Versão 1
##
## Aluno: Nº:  82442     Nome: Carolina Salvador
## Nome do Módulo: S2. Script: manutencao.sh
## Descrição/Explicação do Módulo:
## v2.0
## ATUALIZAÇÃO - Ficheiro praticamente corrigido. Dei o meu melhor e aprendi bastante! Finalmente percebi o cron :)
#####################################################################################

## Este script não recebe nenhum argumento, e permite realizar a manutenção dos registos de estacionamento. 

## S2.1. Validações do script:
## O script valida se, no ficheiro estacionamentos.txt:
## • Todos os registos referem códigos de países existentes no ficheiro paises.txt;
## • Todas as matrículas registadas correspondem à especificação de formato dos países correspondentes;
## • Todos os registos têm uma data de saída superior à data de entrada;
## • Em caso de qualquer erro das condições anteriores, dá so_error S2.1 <descrição do erro>, indicando o erro em questão, e termina. Caso contrário, dá so_success S2.1.

## S2.2. Processamento:
## • O script move, do ficheiro estacionamentos.txt, todos os registos que estejam completos (com registo de entrada e registo de saída), mantendo o formato do ficheiro original, para ficheiros separados com o nome arquivo-<Ano>-<Mês>.park, com todos os registos agrupados pelo ano e mês indicados pelo nome do ficheiro. Ou seja, os registos são removidos do ficheiro estacionamentos.txt e acrescentados ao correspondente ficheiro arquivo-<Ano>-<Mês>.park, sendo que o ano e mês em questão são os do campo <DataSaída>. 
## • Quando acrescentar o registo ao ficheiro arquivo-<Ano>-<Mês>.park, este script acrescenta um campo <TempoParkMinutos> no final do registo, que corresponde ao tempo, em minutos, que durou esse registo de estacionamento (correspondente à diferença em minutos entre os dois campos anteriores).
## • Em caso de qualquer erro das condições anteriores, dá so_error S2.2 <descrição do erro>, indicando o erro em questão, e termina. Caso contrário, dá so_success S2.2.
## • O registo em cada ficheiro arquivo-<Ano>-<Mês>.park, tem então o formato:
## <Matrícula:string>:<Código País:string>:<Categoria:char>:<Nome do Condutor:string>: <DataEntrada:AAAA-MM-DDTHHhmm>:<DataSaída:AAAA-MM-DDTHHhmm>:<TempoParkMinutos:int>
## • Exemplo de um ficheiro arquivo-<Ano>-<Mês>.park, para janeiro de 2025:

# 1º de tudo, valida se paises.txt existe ou se existindo, tem permissões de escrita. 
# Caso um destes casos seja verdadeiro, o script não pode ser efetuado


if [[ ! -f paises.txt || ! -r paises.txt ]]; then
        so_error S2.1 "Ficheiro paises.txt não existe ou não tem permissões de escrita"
        exit 1
    fi

# ficheiro paises.txt existe, começamos a efetuar as validações
# Valida se o ficheiro estacionamentos.txt existe para efetuar as validações do seu conteúdo (ler o ficheiro). 
# Se não existir, dá so_success "O ficheiro estacionamentos.txt ainda não existe"
if [[ -f estacionamentos.txt ]]; then 
    # se existe, então validar as suas permissões de leitura, se não tiver o script termina
    if [[ ! -r estacionamentos.txt ]]; then
        so_error S2.1 "Ficheiro estacionamentos.txt existe, mas não tem permissões de leitura"
        exit 1
    fi
    # caso exista e tenha permissões de leitura, verifica se está vazio
    if [[ ! -s estacionamentos.txt ]]; then
        so_success S2.1 "Ficheiro estacionamentos.txt existe, mas está vazio"
        exit 0
    fi

    # tem permissões de leitura -> avançar
    #extrai os codigos dos paises para o ficheiro paises_autoriz.txt
    awk -F '###' '{print $1}' paises.txt > paises_autoriz.txt 
    #permite colocar todos os codigos de paises_autoriz.txt ordenados por "codigo | codigo | ..."
    codigos_validos=$(paste -sd '|' paises_autoriz.txt) 
    #conta o número de casos inválidos no ficheiro estacionamentos.txt
    codigos_invalidos=$(cut -d':' -f2 "estacionamentos.txt" | grep -vE "^($codigos_validos)$" | wc -l) 
    #para visualizar na consola o número de casos inválidos
    echo "Nº de códigos inválidos: $codigos_invalidos" 
    
    #---

    #sabendo o número de casos inválidos, valida se são diferentes de 0, ou seja, se existem (se for =0, não há erros)
    if [[ $codigos_invalidos != 0 ]]; then
        so_error S2.1 "Existem codigos invalidos"
        exit 1
    fi
    
    rm paises_autoriz.txt
    
    num_linhas_file=$(cat estacionamentos.txt | wc -l)
    

    #não havendo erros com os codigos dos paises, analisamos linha a linha no file estacionamentos.txt 

    #iniciar um contador para obter todas as linhas do ficheiro sem saida 
    count=0 
    #se cada matricula associado ao codigo, segue a regex em paises.txt.
    while IFS=":" read -r matricula codigo categoria nome entrada saida; do
    
        #extrai a regex de paises.txt associado ao codigo da linha em análise
        regex=$(grep "^$codigo###" paises.txt | awk -F '###' '{print $3}')

        #verifica se a matricula segue a formatacao segundo a regex, se não seguir, dá error e sai do script
        if [[ ! "$matricula" =~ $regex ]]; then
            so_error S2.1 "Matrícula inválida para o país $codigo: $matricula"
            exit 1
        fi
        #verifica se a linha em análise possui o campo saída, se não possuir, passa para a linha seguinte no while.
        if [[ -n "$saida" ]]; then
        #extrai os valores, em segundos, (formato UNIX), tanto da data de entrada como da saída
            entrada_unix=$(date -d "$(echo "$entrada" | sed 's/T/ /; s/h/:/')" +%s) # tempo em segundos
            saida_unix=$(date -d "$(echo "$saida" | sed 's/T/ /; s/h/:/')" +%s) # tempo em segundos 

            # valida se o valor correspondente à entrada (em formato unix) é superior à saida (em formato unix). 
            #Se der true, quer dizer que a data de entrada é posterior à data de saida, logo é uma linha inválida e o script encerraa.
            if [[ "$entrada_unix" -ge "$saida_unix" ]]; then
                so_error S2.1 "Ficheiro com datas INVÁLIDAS"
                exit 1
            fi

            #tratando-se de uma linha válida, procedemos à fase de processamento.
            #obtemos o ano e o mes da data de saida (campo: saida)
            ano_saida=$(echo "$saida" | awk -F '-' '{print $1}')
            mes_saida=$(echo "$saida" | awk -F '-' '{print $2}')
            
            # criamos a variável ficheiro, que guarda o nome do ficheiro .park do mes e ano de saida
            ficheiro="arquivo-$ano_saida-$mes_saida.park"
            
            #obter a duração em minutos
            duracao_park=$(( (saida_unix - entrada_unix) / 60 ))

             #caso o ficheiro tenha o \r, este é eliminado. Ocorre se o ficheiro tiver sido criado no Windows. 
            #Se não tiver \r, fica tudo igual
            sed -i 's/\r$//' estacionamentos.txt

            if [[ -f $ficheiro ]]; then
                if [[ ! -r $ficheiro || ! -w $ficheiro ]]; then
                    so_error S2.2 "Ficheiro $ficheiro sem permissões de leitura e/ou escrita"
                    exit 1
                fi

                #verificar se a linha que queremos adicionar já existe no ficheiro $ficheiro
                if grep -q "^$matricula:$codigo:$categoria:$nome:$entrada:$saida" $ficheiro; then
                    so_error S2.2 "Detetado registo JÁ GUARDADO num ficheiro .park . POR FAVOR VALIDAR SITUAÇÃO"
                    exit 1
                fi

                #adiciona a linha ao ficheiro .park pertencente a esse ano e mes
                echo "$matricula:$codigo:$categoria:$nome:$entrada:$saida:$duracao_park" >> "$ficheiro"

            else 
                #cria ficheiro com a linha 
                echo "$matricula:$codigo:$categoria:$nome:$entrada:$saida:$duracao_park" > "$ficheiro"
            fi

            #para remover a linha do ficheiro estacionamentos.txt, temos que verificar se existe permissões de escrita.
            if [[ ! -w estacionamentos.txt ]]; then
                so_error S2.2 "Ficheiro estacionamentos.txt existe, mas não tem permissões de escrita"
                exit 1
            fi
            
           
            #tendo permissões de escrita, retira a linha com o campo $saida do ficheiro estacionamentos.txt
            sed -i "/^$matricula:$codigo:$categoria:$nome:$entrada:$saida$/d" estacionamentos.txt
        
        else
            #dado que a linha não possui saída, incrementa uma unidade ao count 
            ((count++))
        fi

    done < estacionamentos.txt

    #se o número de linhas sem saída for igual ao total de linhas que o ficheiro estacionamentos.txt inicialmente tinha, dispara o 
    if [[ $count -eq $num_linhas_file ]]; then
        echo "Nº de linhas inicial ficheiro = $num_linhas_file"
        so_success S2.1 "Ficheiro validado corretamente"
        so_success S2.2 "O ficheiro estacionamentos.txt ainda não tem SAIDAS"
        
    else
        echo "Nº de linhas inicial ficheiro = $num_linhas_file"
        so_success S2.1 "Ficheiro validado corretamente"
        so_success S2.2 "Manutenção realizada com sucesso"
    fi

else
    so_success S2.1 "Ficheiro estacionamentos.txt ainda não existe"
    so_success S2.2 "O script manutencao.sh não criou qualquer ficheiro"
fi


