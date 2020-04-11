#!/bin/bash
# 05-b-7200
# github.com/andy489

for i; do
	if [ -f "${i}" ]; then
		if [ -r "${i}" ]; then
			echo "$(basename "${i}") is readable file"
		else
			echo "$(basename "${i}") is not readable file"
		fi
	elif [ -d "${i}" ]; then
		COUNT_FILES=$(find "${i}" -maxdepth 1 -type f 2>/dev/null | wc -l)
		echo "$(find "${i}" -maxdepth 1 -type f -size -"${COUNT_FILES}" 2>/dev/null |\
			 xargs basename -a)"
	else
		echo "$(basename "${i}") is not a file/directory name!"
	fi
done
