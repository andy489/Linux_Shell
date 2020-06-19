#!/bin/bash
# 24.sh
# github.com/andy489

[ $# -eq 0 ] || { echo "Invalid number of arguments. Usage: $0"; exit 1; }

#[ $(id -u) -eq 0 ] || { echo "Script ${0} is not executed as root."; exit 1; }

TOTAL_ROOT_RSS="$(ps -u "root" -o rss= | awk '{s+=$1}END{print s}')"

while read USER_UID _HOME; do

	[ "${USER_UID}" -ne 0 ] || continue
	
	[ ! -d "${_HOME}" ] || [ "$(stat -c "%u" "${_HOME}")" != "${USER_UID}" ] || [ "$(stat -c "%A" "${_HOME}"| cut -c3)" != "w" ] || continue

	# echo "${_USER} ${_HOME}"

	TOTAL_USER_RSS="$(ps -u "${USER_UID}" -o rss= | awk '{s+=$1}END{print s}')"
	
	[ -n "${TOTAL_USER_RSS}" ] || TOTAL_USER_RSS=0
	
	if [ "${TOTAL_ROOT_RSS}" -gt "${TOTAL_USER_RSS}" ]; then
		killall -u "${USER_UID}" -m .
		sleep 2
		killall -u "${USER_UID}" -KILL -m .
	fi

done < <(cut -d':' -f3,6 /etc/passwd)
