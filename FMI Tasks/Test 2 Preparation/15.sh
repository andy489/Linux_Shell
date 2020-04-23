#!/bin/bash
# 15
# github.com/andy489

[ $(id -u) -eq 0 ] || exit 1

while read USER HOME; do
        if [ ! -d "${HOME}" ] || sudo -u "${USER}" [ ! -w "${HOME}" ]; then
                echo "${USER}"
        fi
done < <(cat /etc/passwd | awk -F : '{print $1,$6}')
