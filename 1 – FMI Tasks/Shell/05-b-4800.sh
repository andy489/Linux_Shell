#!/bin/bash
# 05-b-4800.sh
# github.com/andy489

if [ $# -ne 2 ]; then
        echo "Invalid number of arguments!"
        exit 1
fi

SEARCHED_FILE="${1}"
DIR_PATH="${2}"

if [ ! -f "${SEARCHED_FILE}" ]; then
        echo "First argument is not a file!"
        exit 2
elif [ ! -r "${SEARCHED_FILE}" ]; then
        echo "File is not readable!"
        exit 3
fi

if [ ! -d "${DIR_PATH}" ]; then
        echo "Second argument is not a directory!"
        exit 4
elif [ ! -r "${DIR_PATH}" ]; then
        echo "Directory is not readable!"
        exit 5
fi

SEARCHED_HASH=$(md5sum "${SEARCHED_FILE}" | awk '{print $1}')

CNT=0

while read CUR_HASH FILE_PATH; do
        if [ "${CUR_HASH}" = "${SEARCHED_HASH}" ]; then
                echo "$(basename ${FILE_PATH})"
                CNT=$(( ++CNT ))
        fi
done < <(find "${DIR_PATH}" -type f 2>/dev/null -print0 \
                | xargs -0 -I {} md5sum {})

if [ $CNT -eq 0 ]; then
        echo "~No copies found."
else
        echo "~Found total $CNT duplicates."
fi
