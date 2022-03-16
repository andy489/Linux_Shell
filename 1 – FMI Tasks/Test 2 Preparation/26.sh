#!/bin/bash
# 26.sh

[ $# -eq 2 ] || { echo "Usage: ${0} <stenograph.txt> <dir>"; exit 1; }

STENOGRAPH="${1}"
DIR="${2}"

if [ ! -f "${STENOGRAPH}" ]; then
	echo "Invalid file name"
	exit 2
elif [ ! -r "${STENOGRAPH}" ]; then 
	echo "File not readable"
	exit 3
fi

if [ ! -d "${DIR}" ]; then
	echo "Invalid directory name"
	exit 4
elif [ ! -w "${DIR}" ]; then
	echo "Directory is not readable"
	exit 5
fi

[ "$(find "${DIR}" -mindepth 1 | wc -l)" -eq 0 ] || { echo "Dir is not empty"; exit 6; }

CNT=1

touch "${DIR}/dict.txt"

while read LINE; do
	NAME_FAMILY="$(cut -d':' -f1 < <(echo "${LINE}") | sed -E "s/\(.*\)//" | awk '$1=$1')"
	NUMBER="$(cat "${DIR}/dict.txt" | fgrep "${NAME_FAMILY}" | cut -d';' -f2 )"
    
	if [ -z "$NUMBER" ]; then
		# number does not exist -> add to dict.txt
		echo "${NAME_FAMILY};${CNT}" >> "${DIR}/dict.txt"
		NUMBER="${CNT}"
		CNT=$( expr "${CNT}" + 1 )
	fi
	echo "${LINE}" | cut -d':' -f2 >> "${DIR}/${NUMBER}.txt"
done < "${STENOGRAPH}"

exit 0
