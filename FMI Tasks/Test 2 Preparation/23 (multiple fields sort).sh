#!/bin/bash
# 23.sh 2nd sol
# github.com/andy489

[ $# -eq 2 ] || { echo "Usage: ${0} <dir> <str>"; exit 1; }

dir="$1"
str="$2"

if [ ! -d "${dir}" ]; then
   echo "Invalid directory!"
   exit 2
elif [ ! -r "${dir}" ]; then
   echo "Directory is not readable!"
   exit 3
fi

[ -n "${str}" ] || { echo "empty string"; exit 4; }

temp=$(mktemp)
gfind "${dir}" -maxdepth 1 -type f -printf "%f\n" 2>/dev/null \
   | egrep "^vmlinuz-[0-9]+\.[0-9]+\.[0-9]+-${str}$" > "${temp}"

searched="$(cat "${temp}" | cut -d'-' -f2 | sort -t'.' -n -k 1,1 -k 2,2 -k 3,3 | tail -1)"

fgrep "${searched}" "${temp}"

rm -- "${temp}"

exit 0
