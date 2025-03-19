#!/bin/bash
# SO_HIDE_DEBUG=1                   ## Uncomment this line to hide all @DEBUG statements
# SO_HIDE_COLOURS=1                 ## Uncomment this line to disable all escape colouring
. so_utils.sh                       ## This is required to activate the macros so_success, so_error, and so_debug

#####################################################################################
## ISCTE-IUL: Trabalho prático de Sistemas Operativos 2024/2025, Enunciado Versão 1
##
## Aluno: Nº: 82442      Nome: Carolina Salvador
## Nome do Módulo: S3. Script: stats.sh
## Descrição/Explicação do Módulo:
## v2.0
## ATUALIZAÇÃO - ficheiro corrido no validador com sucesso
#####################################################################################

## Este script obtém informações sobre o sistema Park-IUL, afixando os resultados das estatísticas pedidas no formato standard HTML no Standard Output e no ficheiro stats.html. Cada invocação deste script apaga e cria de novo o ficheiro stats.html, e poderá resultar em uma ou várias estatísticas a serem produzidas, todas elas deverão ser guardadas no mesmo ficheiro stats.html, pela ordem que foram especificadas pelos argumentos do script.

## S3.1. Validações:
## O script valida se, na diretoria atual, existe algum ficheiro com o nome arquivo-<Ano>-<Mês>.park, gerado pelo Script: manutencao.sh. Se não existirem ou não puderem ser lidos, dá so_error S3.1 <descrição do erro>, indicando o erro em questão, e termina. Caso contrário, dá so_success S3.1.


if [[ ! -f paises.txt || ! -f estacionamentos.txt ]]; then
    so_error S3.1 "Não existem ficheiros para efetuar as estatisticas"
    exit 1
fi
if [[ ! -r paises.txt || ! -r estacionamentos.txt ]]; then
    so_error S3.1 "Ficheiros encontrados, mas sem permissão de leitura"
    exit 1
fi

regex="^arquivo-[0-9]{4}-(0[1-9]|1[0-2])\.park$"

if ! ls | grep -E -q "$regex"; then
    so_error S3.1 "Não existem arquivos de manutencão válidos"
    exit 1
fi

ls | grep -E "$regex" > temp_file.txt

while IFS= read -r file; do
    if [[ ! -r "$file" ]]; then
        so_error S3.1 "FICHEIRO $file Sem permissões de leitura"
        exit 1
    fi
done < "temp_file.txt"

rm temp_file.txt # para evitar que se alterem as permissões de leitura futuras


## S3.2. Estatísticas:
## Cada uma das estatísticas seguintes diz respeito à extração de informação dos ficheiros do sistema Park-IUL. Caso não haja informação suficiente para preencher a estatística, poderá apresentar uma lista vazia.
## S3.2.1.  Obter uma lista das matrículas e dos nomes de todos os condutores cujas viaturas estão ainda estacionadas no parque, ordenados alfabeticamente por nome de condutor:
## <h2>Stats1:</h2>
## <ul>
## <li><b>Matrícula:</b> <Matrícula> <b>Condutor:</b> <Nome do Condutor></li>
## <li><b>Matrícula:</b> <Matrícula> <b>Condutor:</b> <Nome do Condutor></li>
## ...
## </ul>

stats1() {
    echo "<h2>Stats1:</h2>" >> stats.html
    echo "<ul>" >> stats.html
    
   
    awk -F':' '$6 == "" {print $1, $4}' estacionamentos.txt | sort -k2 > "aux_file.txt"
   
    while read -r matricula condutor; do

        echo "<li><b>Matrícula:</b> $matricula <b>Condutor:</b> $condutor</li>" >> stats.html

    done < "aux_file.txt"

    rm aux_file.txt

    echo "</ul>" >> stats.html
}



## S3.2.2. Obter uma lista do top3 das matrículas e do tempo estacionado das viaturas que já terminaram o estacionamento e passaram mais tempo estacionadas, ordenados decrescentemente pelo tempo de estacionamento (considere apenas os estacionamentos cujos tempos já foram calculados):
## <h2>Stats2:</h2>
## <ul>
## <li><b>Matrícula:</b> <Matrícula> <b>Tempo estacionado:</b> <TempoParkMinutos></li>
## <li><b>Matrícula:</b> <Matrícula> <b>Tempo estacionado:</b> <TempoParkMinutos></li>
## <li><b>Matrícula:</b> <Matrícula> <b>Tempo estacionado:</b> <TempoParkMinutos></li>
## </ul>

stats2() {
    echo "<h2>Stats2:</h2>" >> stats.html
    echo "<ul>" >> stats.html

    cat arquivo-*.park | awk -F ':' '{soma[$1] += $NF} END {for (matricula in soma) print matricula " " soma[matricula]}' > aux_file.txt

    cat aux_file.txt | sort -k2,2nr | head -3 > outro_aux_file.txt

    while read -r matricula tempo; do

        echo "<li><b>Matrícula:</b> $matricula <b>Tempo estacionado:</b> $tempo</li>" >> stats.html

    done < "outro_aux_file.txt"

    rm aux_file.txt
    rm outro_aux_file.txt

    echo "</ul>" >> stats.html
    

}

## S3.2.3. Obter as somas dos tempos de estacionamento das viaturas que não são motociclos, agrupadas pelo nome do país da matrícula (considere apenas os estacionamentos cujos tempos já foram calculados):
## <h2>Stats3:</h2>
## <ul>
## <li><b>País:</b> <Nome País> <b>Total tempo estacionado:</b> <ΣTempoParkMinutos></li>
## <li><b>País:</b> <Nome País> <b>Total tempo estacionado:</b> <ΣTempoParkMinutos></li>
## ...
## </ul>

stats3() {
    echo "<h2>Stats3:</h2>" >> stats.html
    echo "<ul>" >> stats.html

    cat arquivo-*.park | awk -F':' '$3 != "M"' |  awk -F ':' '{soma[$2] += $NF} END {for (pais in soma) print pais " " soma[pais]}' > aux_file.txt

    while read -r pais soma; do

        nome_pais=$(cat paises.txt | grep "$pais" | awk -F '###' '{print $2}')

        echo "<li><b>País:</b> $nome_pais <b>Total tempo estacionado:</b> $soma</li>" >> stats.html

    done < "aux_file.txt"

    rm aux_file.txt
    
    # escrever código para extrair coisas 

    echo "</ul>" >> stats.html
}


## S3.2.4. Listar a matrícula, código de país e data de entrada dos 3 estacionamentos, já terminados ou não, que registaram uma entrada mais tarde (hora de entrada) no parque de estacionamento, ordenados crescentemente por hora de entrada:
## <h2>Stats4:</h2>
## <ul>
## <li><b>Matrícula:</b> <Matrícula> <b>País:</b> <Código País> <b>Data Entrada:</b> <DataEntrada></li>
## <li><b>Matrícula:</b> <Matrícula> <b>País:</b> <Código País> <b>Data Entrada:</b> <DataEntrada></li>
## <li><b>Matrícula:</b> <Matrícula> <b>País:</b> <Código País> <b>Data Entrada:</b> <DataEntrada></li>
## </ul>

stats4() {
    echo "<h2>Stats4:</h2>" >> stats.html
    echo "<ul>" >> stats.html
    
   cat arquivo-*.park estacionamentos.txt | awk -F ':' '{print $1":"$2":"$5}' | sort -t ":" -k3.12,5.16 | tail -n3  > aux_file.txt

    while IFS=":" read -r matricula pais data_entrada soma; do

        nome_pais=$(cat paises.txt | grep "$pais" | awk -F '###' '{print $2}')

        echo "<li><b>Matrícula:</b> $matricula <b>País:</b> $pais <b>Data Entrada:</b> $data_entrada</li>" >> stats.html

    done < "aux_file.txt"

    rm aux_file.txt
    
    echo "</ul>" >> stats.html
}

## S3.2.5. Tendo em consideração que um utilizador poderá ter várias viaturas, determine o tempo total, medido em dias, horas e minutos gasto por cada utilizador da plataforma (ou seja, agrupe os minutos em dias e horas).
## <h2>Stats5:</h2>
## <ul>
## <li><b>Condutor:</b> <NomeCondutor> <b>Tempo  total:</b> <x> dia(s), <y> hora(s) e <z> minuto(s)</li>
## <li><b>Condutor:</b> <NomeCondutor> <b>Tempo  total:</b> <x> dia(s), <y> hora(s) e <z> minuto(s)</li>
## ...
## </ul>

stats5() {
    echo "<h2>Stats5:</h2>" >> stats.html
    echo "<ul>" >> stats.html
    
    cat arquivo-*.park | awk -F ':' '{soma[$4] += $NF} END {for (nome in soma) print nome ":" soma[nome]}' > aux_file.txt

    while IFS=":" read -r nome tempo; do

        dias_tempo=$(( $tempo / 1440 )) # 1 dia tem 1440 minutos

        horas_tempo=$(( ($tempo % 1440) / 60 )) #com o resto que sobrou dos dias, contar as horas

        minutos_tempo=$(( $tempo % 60 )) #resto da hora

        echo "<li><b>Condutor:</b> $nome <b>Tempo total:</b> $dias_tempo dia(s), $horas_tempo hora(s) e $minutos_tempo minuto(s)</li>" >> stats.html

    done < "aux_file.txt"

    rm aux_file.txt

    # escrever código para extrair coisas 

    echo "</ul>" >> stats.html
}

## S3.2.6. Liste as matrículas das viaturas distintas e o tempo total de estacionamento de cada uma, agrupadas pelo nome do país com um totalizador de tempo de estacionamento por grupo, e totalizador de tempo global.
## <h2>Stats6:</h2>
## <ul>
## <li><b>País:</b> <Nome País></li>
## <ul>
## <li><b>Matrícula:</b> <Matrícula> <b> Total tempo estacionado:</b> <ΣTempoParkMinutos></li>
## <li><b>Matrícula:</b> <Matrícula> <b> Total tempo estacionado:</b> <ΣTempoParkMinutos></li>
## ...
## </ul>
## <li><b>País:</b> <Nome País></li>
## <ul>
## <li><b>Matrícula:</b> <Matrícula> <b> Total tempo estacionado:</b> <ΣTempoParkMinutos></li>
## <li><b>Matrícula:</b> <Matrícula> <b> Total tempo estacionado:</b> <ΣTempoParkMinutos></li>
## ...
## </ul>
## ...
## </ul>

stats6() {
    echo "<h2>Stats6:</h2>" >> stats.html
    echo "<ul>" >> stats.html
    
    cat arquivo-*.park | awk -F ':' '{soma[$2] += $NF} END {for (pais in soma) print pais " " soma[pais]}' > aux_file.txt

    while read -r pais soma; do
        
        nome_pais=$(cat paises.txt | grep "$pais" | awk -F '###' '{print $2}')

        echo "<li><b>País:</b> $nome_pais <b>Total tempo estacionado:</b> $soma</li>" >> stats.html
        echo "<ul>" >> stats.html

        cat arquivo-*.park | grep "$pais" | awk -F ':' '{soma[$1] += $NF} END {for (matricula in soma) print matricula " " soma[matricula]}' > outro_aux_file.txt

        while read -r matricula soma_tempo; do

            echo "<li><b>Matrícula:</b> $matricula <b> Total tempo estacionado:</b> $soma_tempo</li>" >> stats.html

        done < "outro_aux_file.txt"
        
        echo "</ul>" >> stats.html
        
    done < "aux_file.txt"
    
    rm aux_file.txt
    rm outro_aux_file.txt
    # escrever código para extrair coisas 

    echo "</ul>" >> stats.html
}


## S3.2.7. Obter uma lista do top3 dos nomes mais compridos de condutores cujas viaturas já estiveram estacionadas no parque (ou que ainda estão estacionadas no parque), ordenados decrescentemente pelo tamanho do nome do condutor:
## <h2>Stats7:</h2>
## <ul>
## <li><b> Condutor:</b> <Nome do Condutor mais comprido></li>
## <li><b> Condutor:</b> <Nome do Condutor segundo mais comprido></li>
## <li><b> Condutor:</b> <Nome do Condutor terceiro mais comprido></li>
## </ul>

stats7() {
    echo "<h2>Stats7:</h2>" >> stats.html
    echo "<ul>" >> stats.html

    cat arquivo-*.park estacionamentos.txt | awk -F ':' '{print $4}' | awk '{print length($0), $0}' | sort -nr | head -3 > aux_file.txt

    while read -r carct nome; do

        echo "<li><b> Condutor:</b> $nome</li>" >> stats.html

    done < "aux_file.txt" 
    
    rm aux_file.txt

    echo "</ul>" >> stats.html
}

## S3.3. Processamento do script:
## S3.3.1. O script cria uma página em formato HTML, chamada stats.html, onde lista as várias estatísticas pedidas.
## O ficheiro stats.html tem o seguinte formato:
## <html><head><meta charset="UTF-8"><title>Park-IUL: Estatísticas de estacionamento</title></head>
## <body><h1>Lista atualizada em <Data Atual, formato AAAA-MM-DD> <Hora Atual, formato HH:MM:SS></h1>
## [html da estatística pedida]
## [html da estatística pedida]
## ...
## </body></html>
## Sempre que o script for chamado, deverá:
## • Criar o ficheiro stats.html.
## • Preencher, neste ficheiro, o cabeçalho, com as duas linhas HTML descritas acima, substituindo os campos pelos valores de data e hora pelos do sistema.
## • Ciclicamente, preencher cada uma das estatísticas pedidas, pela ordem pedida, com o HTML correspondente ao indicado na secção S3.2.
## • No final de todas as estatísticas preenchidas, terminar o ficheiro com a última linha “</body></html>”

## Reformular



data_hora=$(date "+%Y-%m-%d %H:%M:%S")

echo "<html><head><meta charset=\"UTF-8\"><title>Park-IUL: Estatísticas de estacionamento</title></head>" > stats.html # tudo o que estava em stats.html é eliminado e esta linha é adicionada
echo "<body><h1>Lista atualizada em $data_hora</h1>" >> stats.html

if [[ $# -eq 0 ]]; then
    stats1
    stats2
    stats3
    stats4
    stats5
    stats6
    stats7

else

    for arg in "$@"; do
        case "$arg" in
            1) 
                stats1 
                ;;
            2) 
                stats2 
                ;;
            3)
                stats3 
                ;;
            4) 
                stats4 
                ;;
            5) 
                stats5 
                ;;
            6) 
                stats6 
                ;;
            7) 
                stats7 
                ;;
            *)
                so_error S3.1 "Entrada inválida"
                exit 1
            
        esac
    done
fi

echo "</body></html>" >> stats.html
so_success S3.1 "Validações corretas"
so_success S3.3
