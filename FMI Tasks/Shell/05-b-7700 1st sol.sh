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
        find "${DIR_PATH}" -type f 2>/dev/null -printf "%s\n" \
                | awk -v sz="${NUM}" '{if($1>sz) sum+=$1} END {print sum}'
else
        echo "Second argument is an invalid integer number!"
        exit 4
fi
