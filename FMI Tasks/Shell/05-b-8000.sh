#!/bin/bash
# 05-b-8000
# github.com/andy489

: '
This script calculates the proportion of
Resident Set Size (memory allocated to a process in RAM)
and Virtual Memory Size (memory that a process can access)
for every specific user process
'

if [ $# -ne 1 ]; then
        echo "Invalid number of arguments"
        exit 1
fi

USER="${1}"

if ! id "${USER}" &>/dev/null ; then
        echo "Invalid user!"
        exit 2
fi

ps -u "${USER}" -o pid,rss,vsz | tail -n +2 | while read PID RSS VSZ; do
        if [ $VSZ -eq 0 ]; then
                PROPORTION="inf"
        else
                PROPORTION=$(echo "scale=4; $RSS / $VSZ" | bc)
        fi
        echo "${VSZ} ${PID} consume ${PROPORTION} of RSS/VSZ memory"
done | sort -rn | cut -d' ' -f2-
