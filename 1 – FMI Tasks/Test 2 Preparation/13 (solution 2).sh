#!/bin/bash
# 13.sh 2nd solution

[ $# -eq 1 ] || { echo "Invalid number of arguments. Usage: $0 <dirname>"; exit 1; }

DIRNAME="${1}"

[ -d "${DIRNAME}" -a -r "${DIRNAME}" ] || { echo "Not directory or not readable"; exit 2; }

find "${DIRNAME}" -type l -print0 2>/dev/null \
	| xargs -0 -I {} file {} \
	| fgrep "broken" | cut -d':' -f1
