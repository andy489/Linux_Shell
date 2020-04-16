#!/bin/bash
# 18-05-2016-3
# github.com/andy489

:'
Напишете shell скрипт който, ако се изпълнява от root, проверява кои потребители на системата 
нямат homedir или не могат да пиат в него.
'

[ $(id -u) -eq 0 ] || exit 1

while read USER HOME; do
        if [ ! -d "${HOME}" ] || sudo -u "${USER}" [ ! -w "${HOME}" ]; then
                echo "${USER}"
        fi
done < <(cat /etc/passwd | awk -F : '{print $1,$6}')
