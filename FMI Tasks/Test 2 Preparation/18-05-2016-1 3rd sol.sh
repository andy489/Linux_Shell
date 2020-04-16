#!/bin/bash
# 18-05-2016-1 1st solution
# github.com/andy489

: '
Напишете shell скрипт, който по подаден един позиционен параметър, ако този параметър е директория, намира всички symlink-ове в нея и под-директориите ѝ с несъществъваш destination.
'

if [ $# -ne 1 -o ! -d "${1}" ]; then
        exit 1
fi

find "${1}" -type l | xargs ls -l | while read foo; do
        SRC=$(echo "${foo}" | tr -s ' ' | cut -d' ' -f9)
        DST=$(echo "${foo}" | tr -s ' ' | cut -d' ' -f11)

        if [ ! -e $(dirname "${SRC}")/${DST} ]; then
                echo "${SRC}"
        fi
done
