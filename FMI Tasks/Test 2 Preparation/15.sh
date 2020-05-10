#!/bin/bash
# 15
# github.com/andy489

[ $(id -u) -eq 0 ] || exit 0

while read _USER HOME; do
        if [ ! -d "${HOME}" ] || sudo -u "${_USER}" [ ! -w "${HOME}" ]; then
                echo "${_USER}"
        fi
done < <(cat /etc/passwd | awk -F : '{print $1,$6}')
