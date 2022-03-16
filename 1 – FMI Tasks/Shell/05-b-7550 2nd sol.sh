#!/bin/bash
# 05-b-7550 2nd solution
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

PS_CNT=$(ps -u "${USER}" | wc -l)
killall -15 -u "${USER}"
killall -9 -u "${USER}"
echo "Total: ${PS_CNT} killed processes"

# run with sudo ./05-b-7550.sh user
