#!/bin/bash
# 16.sh

[ $# -eq 3 ] || { echo "Invalid number of arguments"; exit 1; }

FILE="${1}"; KEY_1="${2}"; KEY_2="${3}"

[ -f "${FILE}" -a -r "${FILE}" ] || { echo "Not file or not readable"; exit 2; }

[ -n "${KEY_1}" -a -n "${KEY_2}" ] || { echo "There is a key with length 0"; exit 3; }

VALUE_1=$( grep "${KEY_1}" "${FILE}" | cut -d'=' -f2 | awk '{$1=$1}1' | tr ' ' '\n' | sort | uniq)

VALUE_2=$( grep -vxF -f <(echo "${VALUE_1}") <(grep "${KEY_2}" "${FILE}" \
	| cut -d'=' -f2 | awk '{$1=$1}1' | tr ' ' '\n' | sort | uniq) )

NEW_VALUE_1=$(echo "${VALUE_1}" | tr '\n' ' ')
NEW_VALUE_2=$(echo "${VALUE_2}" | tr '\n' ' ')

sed -i -e "s/^${KEY_1}=.*/${KEY_1}=${NEW_VALUE_1}/; s/^${KEY_2}=.*/${KEY_2}=${NEW_VALUE_2}/" "${FILE}"
