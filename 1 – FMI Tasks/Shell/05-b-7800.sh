#!/bin/bash
# 05-b-7800 
# github.com/andy489

if [ $# -ne 1 ]; then
        echo "Invalid number of arguments"
        exit 1
fi

DIR_PATH="${1}"

if [ ! -d "${DIR_PATH}" ]; then
        echo "Invalid directory name!"
        exit 2
elif [ ! -r "${DIR_PATH}" ]; then
        echo "Directory is not readable!"
        exit 3
fi

CNT_X=0

while read -d $'\n' LINE; do
        if [ -x "${LINE}" ]; then
                (( CNT_X+=1 ))
        fi
done < <(gfind "${DIR_PATH}" 2>/dev/null)

echo "Number of executable files is ${CNT_X}"

