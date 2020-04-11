#!/bin/bash
# 04-b-9000 1st solution
# github.com/andy489

function count_children {
        ps --ppid "${1}" | head +2 | wc -l
}

ps -eo pid=,ppid= | while read  -d $'\n' pid_and_ppid; do
        read pid ppid < <(echo "${pid_and_ppid}")
        [ $(count_children "${pid}") -ge $(count_children "${ppid}") ] && echo "${pid}"
done 2>/dev/null
