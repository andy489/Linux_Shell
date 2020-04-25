#!/bin/bash
# 19.sh
# github.com/andy489

num=""

if [ $# -eq 2 ]; then
	num="${2}"
elif [ $# -gt 2 ]; then
	echo "Invalid number of arguments!"
	exit 1
elif [ $# -eq 0 ]; then
	echo "Invalid number of arguments!"
	exit 2
fi

dir="${1}"

if [ ! -d "${dir}" ]; then
	echo "Not a valid directory name!"
	exit 3
elif [ ! -r "${dir}" ]; then
	echo "Directory is not readable!"
	exit 4
fi

function validate_num {
	egrep -q '^[0-9]+$' <(echo "${1}")
}

function count_hardlinks {
	find "${1}" -samefile "${2}" 2>/dev/null | wc -l
}

function is_broken_symlink {
	file "${1}" | grep -q 'broken'
}

if [ -n "${num}" ]; then
	if validate_num "${num}" ; then
		
		while read f; do
			if [ $(count_hardlinks "${dir}" "${f}") -ge "${num}" ]; then
				echo "${f}"
			fi
		done < <(find "${dir}" -type f ! -name '.*' 2>/dev/null)
	else
		echo "Invalid number argument!"
		exit 5
	fi
else
	while read l; do
		if is_broken_symlink "${l}"; then
			echo "${l}"
		fi
	done < <(find "${dir}" -type l 2>/dev/null)
fi
