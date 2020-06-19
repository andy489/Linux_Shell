#!/bin/bash
# 24.sh
# github.com/andy489

[ $# -eq 0 ] || { echo "Invalid number of arguments. Usage: $0"; exit 1; }

#[ $(id -u) -eq 0 ] || { echo "Script ${0} is not executed as root."; exit 1; }

TOTAL_ROOT_RSS="$(ps -u "root" -o rss= | awk '{s+=$1}END{print s}')"

while read _USER _HOME; do

	[ "${_USER}" != "root" ] || continue
	
	[ ! -d "${_HOME}" ] || [ "$(stat -c "%U" "${_HOME}")" != "${_USER}" ] || [ "$(stat -c "%A" "${_HOME}"| cut -c3)" != "w" ] || continue

	# echo "${_USER} ${_HOME}"

	TOTAL_USER_RSS="$(ps -u "${_USER}" -o rss= | awk '{s+=$1}END{print s}')"
	
	[ -n "${TOTAL_USER_RSS}" ] || TOTAL_USER_RSS=0
	
	if [ "${TOTAL_ROOT_RSS}" -gt "${TOTAL_USER_RSS}" ]; then
		killall -u "${_USER}" -m .
		sleep 2
		killall -u "${_USER}" -KILL -m .
	fi

done < <(cut -d':' -f1,6 /etc/passwd)
