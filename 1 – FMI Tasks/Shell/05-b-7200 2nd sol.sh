#!/bin/bash
# 05-b-7200 2nd solution
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
                gfind "${i}" -type f 2>/dev/null -printf "%s %f\n" \
                        | awk -v sz="${COUNT_FILES}" '$1 < sz {print $2}'
        else
                echo "$(basename "${i}") is not a file/directory name!"
        fi
done
