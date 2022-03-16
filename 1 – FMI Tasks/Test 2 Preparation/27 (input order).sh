#!/bin/bash
# 27.sh 2nd sol

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

result=$(mktemp)

while read LINE; do
    # exists in b.csv or it has to be added
    CUTLINE="$( echo "$LINE" | cut -d',' -f2- )"
    if egrep -q ",${CUTLINE}$" "${result}" ; then
        continue;
    else # add
        echo "${LINE}" >> "${result}"
    fi

done < <( cat "${a}" | sort -t',' -nk1)
# now result contains what we want to include

while read LINE; do
	if cat "${result}" | egrep -q "^${LINE}$"; then
		echo "${LINE}" >> "${b}"
	fi
done < "${a}"

rm -- "${result}"

exit 0
