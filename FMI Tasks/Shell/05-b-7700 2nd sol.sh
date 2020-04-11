
#!/bin/bash
# 05-b-7700 2nd solution
# github.com/andy489

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

DIR_PATH="${1}"
NUM="${2}"

if [ ! -d "${DIR_PATH}" ]; then
	echo "Invalid directory name!"
	exit 2
elif [ ! -r "${DIR_PATH}" ]; then
	echo "Directory is not readable!"
	exit 3
fi

function validate_num {
	grep -qE '^[-+]?[0-9]+$' <(echo ${NUM})
}

if validate_num "${NUM}"; then
	ALL_SIZES=$(find "${DIR_PATH}" -type f 2>/dev/null -size -"${NUM}"c -printf "%s\n")
	# echo "${ALL_SIZES}"
	TOTAL_SIZE=0
	for i in ${ALL_SIZES}; do
		(( TOTAL_SIZE+="${i}" ))
	done
	echo "${TOTAL_SIZE}"
else
	echo "Second argument is an invalid integer number!"
	exit 4
fi
