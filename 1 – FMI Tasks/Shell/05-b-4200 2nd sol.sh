#!/bin/bash
# 05-b-4200 2nd solution

[ $# -eq 1 ] || exit 1
[ ! -r $1 ] || exit 2

F=${1}

DATA=$(cat $F | sed -e 's/[^{}]//g' | tr -d '\n' | tr -d ' ')
while grep -qsF '{}{' <(echo ${DATA}); do
	DATA=$(echo "${DATA}" | sed -e 's/{}{/{/')
done

ALLCHARS=$(echo ${DATA} | tr -d '}' | wc -c)
echo "The deepest nesting is $((--ALLCHARS)) levels"
