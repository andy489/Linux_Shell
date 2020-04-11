#!/bin/bash
# 05-b-7000
# github.com/andy489

if [ $# -eq 0 ]; then
        echo "Invalid number of arguments!"
        exit 1
fi

read -p "Insert searched string: " STRING

while [ $# -ne 0 ]; do
        FILE_NAME="${1}"
        if [ ! -f "${FILE_NAME}" ]; then
                echo "Invalid file name given as argument!"
        elif [ ! -r "${FILE_NAME}" ]; then
                echo "File is not readable!"
        else
                CUR_MATCHES=$(grep -oc "${STRING}" "${FILE_NAME}")
                echo "In file \"${FILE_NAME}\", ${CUR_MATCHES} rows contains string \"${STRING}\""
        fi
        shift 1
done
