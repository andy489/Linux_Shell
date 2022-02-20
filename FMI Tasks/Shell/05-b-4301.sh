#!/bin/bash
# 05-b-4301

if [ $# -ne 3 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

ADDRESS_FILE="${1}"

if [ ! -f "${ADDRESS_FILE}" ]; then
	echo "First argument is not a file!"
	exit 2
elif [ ! -r "${ADDRESS_FILE}" ]; then
	echo "Address book file is not readable!"
	exit 3
elif [ ! -w "${ADDRESS_FILE}" ]; then
	echo "Address book file is not writable!"
	exit 4
fi

NAME="${2}"
NICK="${3}"

USERNAME=$(cat ~velin/passwd.txt | awk -v name="${NAME}" -F ':' '($5 == name){print $1}')

#if [ $(wc -l "${USERNAME}") -ne 1 ]; then
#	echo "Do the bonus thing"
#fi

echo "${NICK} ${USERNAME}" >> "${ADDRESS_FILE}"
