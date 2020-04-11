#!/bin/bash
# 05-b-6600 1st solution
# github.com/andy489

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

DIR_PATH="${1}"

if [ ! -d "${DIR_PATH}" ]; then
	echo "Argument is not directory!"
	exit 2
fi

if [ ! -r "${DIR_PATH}" ]; then
	echo "Directory has no read permissions!"
	exit 3
fi

HASHES=$(mktemp)

find "${DIR_PATH}" -type f -maxdepth 1 -exec md5sum {} 2>/dev/null \; |\
	sort > "${HASHES}"

cat "${HASHES}" | cut -d' ' -f1 | uniq -d | while read DUPLICATED_HASH; do
	grep "${DUPLICATED_HASH}" "${HASHES}" \
		| awk '{print $2}' \
		| tail -n +2 \
		| xargs rm --
done

rm "${HASHES}"
