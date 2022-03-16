#!/bin/bash
# 15

[ $(id -u) -eq 0 ] || exit 0

while read _USER _HOME; do
        if [ ! -d "${_HOME}" ] || sudo -u "${_USER}" [ ! -w "${_HOME}" ]; then
                echo "${_USER}"
        fi
done < <(cat /etc/passwd | awk -F : '{print $1,$6}')
