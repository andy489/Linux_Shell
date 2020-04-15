#!/bin/bash
# 05-b-7200
# github.com/andy489

for i; do
        if [ -f "${i}" ]; then
                if [ -r "${i}" ]; then
                        echo "$(basename "${i}") is readable file"
                else
                        echo "$(basename "${i}") is not readabl file"
                fi
        elif [ -d "${i}" ]; then
                COUNT_FILES=$(find "${i}" -type f 2>/dev/null | wc -l | awk '{$1=$1}1')
                        echo "$(find "${i}" -type f -size -"${COUNT_FILES}"c 2>/dev/null |\
                        xargs basename -a)"
        else
                echo "$(basename "${i}") is not a file/directory name!"
        fi
done
