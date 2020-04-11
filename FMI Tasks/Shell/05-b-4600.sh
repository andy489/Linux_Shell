#!/bin/bash
# 05-b-4600
# github.com/andy489

if [ $# -ne 3 ]; then
	#echo "Invalid number of arguments!"
	exit 4
fi

grep -vqE '^[+-]?[0-9]+$' <(echo "${1}"; echo "${2}"; echo "${3}")

if [ $? -eq 0 ]; then
	#echo "At least one of the arguments is not an integer!"
	exit 3
fi

if [ $2 -gt $3 ]; then
	#echo "Reversed intervals"
	exit 2
fi

if [ $1 -lt $2 ] || [ $1 -gt $3 ]; then
	#echo "Not in interval"
	exit 1
fi

if [ $1 -ge $2 ] &&  [ $1 -le $3 ]; then
	#echo "In interval"
	exit 0
fi
