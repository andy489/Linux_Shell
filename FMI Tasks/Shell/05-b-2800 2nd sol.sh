#!/bin/bash
# 05-b-2800 2nd solution

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments!"
	exit 1
elif echo "${1}" | grep -qE '^[[:alnum:]]+$'; then
	echo "True"
	exit 0
else
	echo "False"
	exit 1
fi	
