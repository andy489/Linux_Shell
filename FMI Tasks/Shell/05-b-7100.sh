#!/bin/bash
# 05-b-7100
# github.com/andy489

if [ $# -ne 2 ]; then
        echo "Invalid number of arguments!"
        exit 1
fi

DIR_PATH="${1}"
NUM="${2}"

if [ ! -d "${DIR_PATH}" ]; then
        echo "First argument is not a directory"
        exit 2
elif [ ! -r "${DIR_PATH}" ]; then
        echo "Directory is not readable!"
        exit 3
fi

function validate_num {
        grep -qE '^[+-]?[0-9]+$' <(echo "${1}")
}

if validate_num "${NUM}" ; then
	#find . -type f 2>/dev/null -size +"${NUM}"c -printf "%f\n"
        while read SIZE FILE_PATH; do
              	if validate_num "${SIZE}"; then
                        if [ "${SIZE}" -gt "${NUM}" ]; then
                                echo "${FILE_PATH}"
                        fi
                fi
        done < <(find "${DIR_PATH}" -type f 2>/dev/null -printf "%s %p\n")          
else
        echo "Second argument is not an integer!"
        exit 4
fi
