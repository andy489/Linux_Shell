#!/bin/bash
# 05-b-4200 1st solution
# github.com/andy489

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

if [ ! -r $1 ]; then
	echo "Error, file not readable"
	exit 2
fi

MAX_CNT=0
CUR_CNT=0

DATA_FIRST_WAY=$(cat "$1" | grep -o "." | egrep "({|})" | tr "\n" " ")
DATA_SECOND_WAY=$(cat "$1" | sed 's/[^{}]/\n/g' | sed '/^ *$/d')

DATA=${DATA_SECOND_WAY}

for i in $DATA; do
	if [ $i = "{" ]; then
		CUR_CNT=$((CUR_CNT+1))
		if [ $CUR_CNT -gt $MAX_CNT ]; then
			MAX_CNT=$CUR_CNT
		fi
	else
		CUR_CNT=$((CUR_CNT-1))
	fi
done

echo "The deepest nesting is ${MAX_CNT} levels"
