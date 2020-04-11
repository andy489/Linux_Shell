#!/bin/bash
# 05-b-4400.sh
# github.com/andy489

if [ $# -eq 0 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

FIRST_DIR_PATH="${1}"

if [ ! -d "${FIRST_DIR_PATH}" ]; then
	echo "First argument is not a directory!"
	exit 2
fi

SECOND_DIR_PATH=""

if [ $# -eq 2 ]; then
	SECOND_DIR_PATH="${2}"
else
	if [ $# -eq 1 ]; then
		SECOND_DIR_PATH="target-$(date +'%Y-%m-%d-%H-%M')"
		mkdir "${SECOND_DIR_PATH}"
	else
		echo "Too many arguments!"
		exit 3
	fi
fi

if [ ! -d "${SECOND_DIR_PATH}" ]; then
	echo "Second file is not a directory!"
	exit 3
fi

echo "Source directory: ${FIRST_DIR_PATH}"
echo "Destination directory: ${SECOND_DIR_PATH}"
echo -e "\nChanged files in the last 45 mins in Source directory:\n"

find "${FIRST_DIR_PATH}" -type f -mmin -45 -exec echo {} \;

find "${FIRST_DIR_PATH}" -type f -mmin -45 -exec cp {} "${SECOND_DIR_PATH}" \;

#find "${FIRST_DIR_PATH}" -type f -mmin -45 | while read FILE; do
#	cp "${FILE}" "${SECOND_DIR_PATH}"
#done

echo -e "\nSuccessfully copying files\n"
