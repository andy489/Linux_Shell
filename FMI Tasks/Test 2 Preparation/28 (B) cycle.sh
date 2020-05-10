#!/bin/bash
# 28.sh B) cycle
#github.com/andy489

temp=$(mktemp)
data=$(mktemp)

cat | egrep "^[-+]?[0-9]+$" | sort -n > "${temp}"

[ "$(cat "$temp" | wc -l )" -ne 0 ] || { echo -n  "\nno valid numbers"; exit 1; } 

MAX_SUM=0
CUR_RES=0

while read NUM; do
    SUM=0
    NUM_COPY="$NUM"
    NUM="$( echo $NUM | sed -E "s/-//" )"
    while [ "$NUM" -ne 0 ]; do
        REM=$(expr $NUM % 10 )
        SUM=$( expr $SUM + $REM )
        NUM=$( expr $NUM / 10 )
    done
	
	 echo "${NUM_COPY} ${SUM}" >> "${data}"

done < <(cat "${temp}")

uniq -c "${data}" | awk '{$1=$1}1' | grep '^1' | cut -d' ' -f2,3 > "${temp}" 
#cat "${temp}"
largest_sum_digits_and_unique=$(cat "${temp}" | sort -t' ' -k2 | tail -1 | cut -d' ' -f2)

RES=$(cat "${temp}" | grep "${largest_sum_digits_and_unique}$" | sort -t' ' -k1 | tail -1 | cut -d' ' -f1)

echo -e "\n${RES}"

rm -- "${temp}"
rm -- "${data}"

exit 0
