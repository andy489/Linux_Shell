#!/bin/bash
# 13.sh solution 1
# github.com/andy489
  
[ $# -eq 1 ] || { echo "Invalid number of arguments. Usage: ${0} <dirname>"; exit 1; }

d="${1}"

[ -d "${d}" -a -r "${d}" ] || { echo "Not directory or not readable"; exit 2; }

find "${d}" -type l -printf "%Y %p\n" 2>/dev/null | grep -e "^[^NL]" | cut -c3-
