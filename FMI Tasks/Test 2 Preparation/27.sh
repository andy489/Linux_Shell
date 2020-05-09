#!/bin/bash
# 27.sh
# github.com/andy489

[ $# -eq 2 ] || { echo "Usage: ${0} <a.csv> <b.csv>"; exit 1; }

a="${1}"
b="${2}"

if [ ! -f "${a}" ] ;then
    echo "Invalid file"
    exit 2
elif [ ! -r "${a}" ]; then
	echo "File not readable"
	exit 3;
fi

( [ -e "${b}" ] || touch "${b}" ) && [ ! -w "${b}" ] && echo cannto write to file ${b} && exit 1

while read LINE; do
    # exists in b.csv or it has to be added
    CUTLINE="$( echo "$LINE" | cut -d',' -f2- )"
    if egrep -q ",${CUTLINE}$" "${b}" ; then
        continue;
    else # add
        echo "${LINE}" >> "${b}"
    fi

done < <( cat "${a}" | sort -t',' -nk1)

exit 0
