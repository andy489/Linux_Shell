#!/bin/bash
# 28.sh B) smart sed + bc
#github.com/andy489

TEMP=$(mktemp)
DATA=$(mktemp)

cat | egrep "^[-+]?[0-9]+$" | sort -n > "${TEMP}"

[ "$(cat "${TEMP}" | wc -l )" -ne 0 ] || { echo -n  "\nno valid numbers"; exit 1; } 

while read NUM; do
	SUM_DIGITS= $(echo ${n} | sed -E 's/(.)/\1+/g' | sed 's/.$//' | bc)
	echo "${NUM} ${SUM_DIGITS}" >> "${DATA}"
done < <(cat "${TEMP}")

uniq -c "${DATA}" | awk '{$1=$1}1' | grep '^1' | cut -d' ' -f2,3 > "${TEMP}" 
#cat "${TEMP}"
LARGEST_SUM_DIGITS_AND_UNIQUE=$(cat "${TEMP}" | sort -t' ' -k2 | tail -1 | cut -d' ' -f2)

RES=$(cat "${TEMP}" | grep "${LARGEST_SUM_DIGITS_AND_UNIQUE}$" | sort -t' ' -k1 | tail -1 | cut -d' ' -f1)

echo -e "\n${RES}"

rm -- "${TEMP}"
rm -- "${DATA}"

exit 0
