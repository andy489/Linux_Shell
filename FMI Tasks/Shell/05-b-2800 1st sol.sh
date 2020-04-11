#!/bin/bash
# 05-b-2800 1st solution
# github.com/andy489

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments!"
	exit 1
elif [[ "${1}" =~ ^[[:alnum:]]+$ ]]; then
	echo "True"
	exit 0
else
	echo "False"
	exit 1
fi	
