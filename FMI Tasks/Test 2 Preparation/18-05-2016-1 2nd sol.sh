#!/bin/bash
# 18-05-2016 2nd solution
# github.com/andy489

: '
Напишете shell скрипт, който по подаден един позиционен параметър, ако този параметър е директория, 
намира всички symlink-ове в нея и под-директориите ѝ с несъществъваш destination.
'

if [ $# -ne 1 -o ! -d "${1}" ]; then
        exit 1
fi

for i in $(find "${1}" -type l); do
        DST=$(stat -c %N "${i}" | cut -d "'" -f4)
        if [ ! -e $(dirname "${i}")/${DST} ]; then
                echo "${i}"
        fi
done
