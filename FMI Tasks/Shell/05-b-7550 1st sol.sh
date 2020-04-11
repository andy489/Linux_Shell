#!/bin/bash
# 05-b-7550 1st solution
# github.com/andy489

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

USER="${1}"

if [ $(id -u "${USER}") -eq 1 ]; then
	echo "Invalid username!"
	exit 2
fi
 
PS_CNT=0

while read -d $'\n' PID; do
	kill -15 "${PID}"
	kill -9 "${PID}"
	CNT=(($CNT+1))
done < <(ps -u "${USER}" -o pid=)

echo "Total: ${PS_CNT} killed processes"
