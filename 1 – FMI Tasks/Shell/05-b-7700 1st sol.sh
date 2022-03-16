#!/bin/bash
# 05-b-7700 1st solution
# github.com/andy489

if [ $# -ne 2 ]; then
        echo "Invalid number of arguments!"
        exit 1
fi

DIR_PATH="${1}"
NUM="${2}"

if [ ! -d "${DIR_PATH}" ]; then
        echo "Invalid directory name!"
        exit 2
elif [ ! -r "${DIR_PATH}" ]; then
        echo "Directory is not readable!"
        exit 3
fi

function validate_num {
        grep -qE '^[-+]?[0-9]+$' <(echo ${NUM})
}

if validate_num "${NUM}"; then
        find "${DIR_PATH}" -maxdepth 1 -type f 2>/dev/null -printf "%s\n" \
                | awk -v sz="${NUM}" '$1 > sz {SUM+=$1} END {print SUM}'
else
        echo "Second argument is an invalid integer number!"
        exit 4
fi
