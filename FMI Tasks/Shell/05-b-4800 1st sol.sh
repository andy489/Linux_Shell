#!/bin/bash
# 05-b-4800.sh 1st solution
# github.com/andy489

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

FILE_PATH="${1}"
DIR_PATH="${2}"

if [ ! -f "${FILE_PATH}" ]; then
	echo "First argument is not a file!"
	exit 2
elif [ ! -r "${FILE_PATH}" ]; then
	echo "File is not readable!"
	exit 3
fi

if [ ! -d "${DIR_PATH}" ]; then
	echo "Second argument is not a directory!"
	exit 4
elif [ ! -r "${DIR_PATH}" ]; then
	echo "Directory is not readable!"
	exit 5
fi

FILE_HASH="$(md5sum "${FILE_PATH}" | cut -d' ' -f1)"
CNT=0

while read -d $'\0' FILE; do
	CUR_HASH="$(md5sum "${FILE}" | cut -d' ' -f1)"
	if [ "${CUR_HASH}" = "${FILE_HASH}" ]; then
		CNT=$(($CNT+1))
		echo $(basename "${FILE}")
	fi
done < <(find "${DIR_PATH}" -type f 2>/dev/null -print0)

if [ $CNT -eq 0 ]; then
	echo "~No copies found."
else
	echo "~Found total $CNT duplicates."
fi
