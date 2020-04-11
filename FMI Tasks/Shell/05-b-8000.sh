#!/bin/bash
# 05-b-8000
# github.com/andy489

if [ $# -ne 1 ]; then
        echo "Invalid number of arguments"
        exit 1
fi

USER="${1}"

function validate_user {
        id "${1}" 1>/dev/null 2>/dev/null
}

ANSWER=""

if validate_user "${USER}"; then
        while read -d $'\n' LINE; do
                read RSS VSZ COMM
                PROPORTION=0
                if [ VSZ = 0 ]; then
                        PROPORTION="inf"
                else
                        PROPORTION=$(echo "$(bc <<< "scale=2; $RSS / $VSZ")")
                fi
                ANSWER+="${VSZ} Process: ${COMM}, RSS/VSZ=${PROPORTION}\n"
        done < <(ps -u "${USER}" -o rss=,vsz=,comm=)
else
        echo "Invalid user!"
        exit 2
fi

echo -e "${ANSWER}" | sort -rn -t' ' -k1 | cut -d' ' -f2-
