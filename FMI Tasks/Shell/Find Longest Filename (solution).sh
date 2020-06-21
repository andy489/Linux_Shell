#!/bin/bash

[ $# -eq 1 ] || { echo "Invalid number of arguments. Usage: $0 <dir_path>"; exit 1;}

DIR_PATH="${1}"

[ -d "${DIR_PATH}" ] || { echo "Invalid directory name"; exit 2; }
[ -r "${DIR_PATH}" ] || { echo "Directory is not readable; exit 3"; }

LENGTH_FILENAME=$(mktemp)

gfind "${DIR_PATH}" -type f -printf "%f\n" 2>/dev/null |\
			awk '{printf "%s %s\n", length($0),$0}' |\
			sort -n -t' ' -k1 > "${LENGTH_FILENAME}"

MAXLENGHT="$(cat "${LENGTH_FILENAME}" | tail -1 | cut -d' ' -f1)"
#echo "MAX LENGHT: ${MAXLENGHT}"

grep '^30 ' "${LENGTH_FILENAME}" | cut -d' ' -f2-

rm -- "${LENGTH_FILENAME}"
