#!/bin/bash
# 05-b-4700 2nd solution
# girhub.com/andy489

if [ $# -eq 1 ]; then
	DELIMITER=" "
elif [ $# -eq 2 ]; then
	DELIMITER="${2}"
else 
	echo "Invalid number of parameters"
	exit 1
fi

grep -qE '^[-+]?[0-9]+$' <(echo "${1}")

if [ ! $? -eq 0 ]; then
	echo "Invalid integer number!"
	exit 2
fi

NUM=$(echo "${1}" | grep -o .)
SIGN=$(echo "${NUM}" | head -1)

FLAG=0
grep -q [-+] <(echo "${SIGN}")

if [ $? -eq 0 ]; then
	NUM=$(echo "${NUM}" | tail -n +2)
	FLAG=1
fi

[ "${FLAG}" -eq 1 ] && echo -n "${SIGN}"

echo "${NUM}" | tac | xargs -n3 | tr -d " " | tr "\n" "${DELIMITER}" |\
	grep -o . | tac | tail -n +2 | tr -d "\n" ; echo
