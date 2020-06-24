#!/bin/bash
# 04-b-9000 1st solution
# github.com/andy489

function count_children() {
        ps --ppid "${1}" | head +2 | wc -l
}

ps -eo pid=,ppid= | while read PID PPID; do
        [ $(count_children "${PID}") -ge $(count_children "${PPID}") ] && echo "${PID}"
done 2>/dev/null
