#!/bin/bash
# 18.sh
# github.com/andy489

user_home=$(mktemp)

cat /etc/passwd | cut -d ':' -f 1,6 | tr ':' ' ' > "${user_home}"

function find_most_recently_modified_regular_file {
	find "${1}" -type f ! -name ".*" -printf "%T@ %f\n" 2>/dev/null | sort -n -t' ' -k1 | tail -1
}

most_recent_for_all_users=$(mktemp)

while read user home; do

	[ -d "${home}" ] || continue
	[ -r "${home}" ] || continue

	cur_file="$(find_most_recently_modified_regular_file "${home}")"

	[ -n "${cur_file}" ] || continue	

	echo "${user} ${cur_file}" >> "${most_recent_for_all_users}"		

done < "${user_home}"

cat "${most_recent_for_all_users}" | sort -n -t' ' -k2 | tail -1 | cut -d' ' -f1,3-

rm -- "${most_recent_for_all_users}"
rm -- "${user_home}"
