#!/bin/bash
# 16.sh
# github.com/andy489

[ $# -eq 3 ] || { echo "Invalid number of arguments"; exit 1; }

FILE="${1}"; KEY_1="${2}"; KEY_2="${3}"

[ -f "${FILE}" -a -r "${FILE}" ] || { echo "Not file ot not readable"; exit 2; }
[ -n "${KEY_1}" -a -n "${KEY_2}" ] || { echo "There is a key with length 0"; exit 3; }

VAL_1="$(cat "${FILE}" | fgrep "${KEY_1}=" | awk -F'=' '{print $2}')"
VAL_2="$(cat "${FILE}" | fgrep "${KEY_2}=" | awk -F'=' '{print $2}')"

[ -n "${VAL_2}" ] && exit 0

NEW_VAL=""
for WORD in ${VAL_2}; do
	if fgrep -qv "${WORD}" <(echo "${VAL_1}"); then
		NEW_VAL+="${WORD} "
	fi
done

sed -i -E "s/${KEY_2}=${VAL_2}/${KEY_2}=${NEW_VAL}/" "${FILE}"

exit #?	
