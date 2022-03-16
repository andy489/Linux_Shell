#!/bin/bash
# 05-b-8000
# github.com/andy489

: '
This script calculates the proportion of
Resident Set Size (memory allocated to a process in RAM)
and Virtual Memory Size (memory that a process can access)
for every specific user process
'

[ $# -eq 1 ] || {
        echo "Invalid number of arguments. Usage $0 <username>."
        exit 1
}

_USER="${1}"

if ! id "${_USER}" &>/dev/null ; then
        echo "Invalid username!"
        exit 2
fi

ps -u "${_USER}" -o pid,rss,vsz | tail -n +2 | while read PID RSS VSZ; do
        if [ $VSZ -eq 0 ]; then
                PROPORTION="inf"
        else
                PROPORTION=$(echo "scale=4; $RSS / $VSZ" | bc)
        fi
        echo "${VSZ} ${PID} consume ${PROPORTION} of RSS/VSZ memory"
done | sort -rn -t' ' -k1 | cut -d' ' -f2-
