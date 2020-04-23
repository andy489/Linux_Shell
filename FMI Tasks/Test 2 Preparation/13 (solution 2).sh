#!/bin/bash
# 13.sh (solution 2)
# github.com/andy489

[ $# -eq 1 ] || (echo "Invalid number of arguments"; exit 1)

d="${1}"

[ -d "${d}" -a -r "${d}" ] || (echo "Not directory or not readable"; exit 2)

find "${di}" -type l 2>/dev/null -print0 \
	| xargs -0 -I {} file {} \
	| fgrep "broken"
