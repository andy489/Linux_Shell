#!/bin/bash
# 20.sh
# github.com/andy489
  
[ $# -eq 3 ] || { echo "Invalid number of arguments!"; exit 1; }

src="${1}"
dst="${2}"
str="${3}"

[ -d "${src}" ] || { echo "Source is not a directory!"; exit 2; }
[ -d "${dst}" ] || { echo "Destination is not a directory!"; exit 3; }
[ -r "${src}" ] || { echo "Source is not readable!"; exit 4; }
[ -w "${dst}" ] || { echo "Destination is not writable!"; exit 5; }

#[ $(id -u) -eq 0 ] || exit 0

dir_name=$( dirname "${0}" )

while read f; do
        base_name=$( basename "${f}" )
        mv -iv -- "${f}" "${dir_name}/${dst}/${base_name}"
done < <(find "${src}" -type f -name "*${str}*" 2>/dev/null)
