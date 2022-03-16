#!/bin/bash
# 23.sh 1st sol
# github.com/andy489

[ $# -eq 2 ] || { echo "Invalid number of arguments!"; exit 1; }

dir="${1}"
str="${2}"

if [ ! -d "${dir}" ]; then
	echo "Invalid directory!"
	exit 2
elif [ ! -r "${dir}" ]; then
	echo "Directory is not readable!"
	exit 3
fi

if [ -z "${str}" ]; then
	echo "Empty string!"
	exit 4
fi

grep -E "^vmlinuz-[0-9]+\.[0-9]+\.[0-9]+-${str}$" <(find "${dir}" -maxdepth 1 -type f 2>/dev/null \
	-exec basename {} \;) \
	| sort -V -t'-' -k2 | tail -n 1
