#!/bin/bash
# 13.sh solution 1
# github.com/andy489
  
[ $# -eq 1 ] || { 
  echo "Invalid number of arguments. Usage: ${0} <dirname>" >&2
  exit 1
}

D="${1}"

[ -d "${D}" -a -r "${D}" ] || { 
  echo "Not directory or not readable." >&2
  exit 2 
}

find "${d}" -type l -printf "%Y %p\n" 2>/dev/null | grep -e "^[NL]" | cut -c3-
