#!/bin/bash
# 05-b-3300
# github.com/andy489

if [ ! $# -eq 3 ]; then
	echo "Invalid number of argumrnts!"
	exit 1
fi

FILE_1="${1}"
FILE_2="${2}"

if [ ! -r "${FILE_1}" ] || [ ! -r ${FILE_2} ]; then      
 	echo "First two files are not both readable."
	exit 2
fi

paste "${FILE_1}" "${FILE_2}" | sort > "${3}"
# paste -d "\n" "${FILE_1}" "${FILE_2}" | sort > "${3}"
