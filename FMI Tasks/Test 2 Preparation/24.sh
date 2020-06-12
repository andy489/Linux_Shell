#!/bin/bash
# 24.sh
# github.com/andy489

#[ $(id -u) -eq 0 ] || { echo "Script ${0} is not executed as root."; exit 1; }

total_root_rss="$(ps -u "root" -o rss= | awk '{s+=$1}END{print s}')"

while read _USER _HOME; do

	[ "${_USER}" != "root" ] || continue
	
	[ ! -d "${_HOME}" ] || [ "$(stat -c "%U" "${_HOME}")" != "${_USER}" ] || [ "$(stat -c "%A" "${_HOME}"| cut -c3)" != "w" ] || continue

	# echo "${_USER} ${_HOME}"

	total_user_rss="$(ps -u "${_USER}" -o rss= | awk '{s+=$1}END{print s}')"
	
	if [ "${total_root_rss}" -gt "${total_user_rss}" ]; then
		killall -u "${_USER}" -m .
		sleep 2
		killall -u "${_USER}" -KILL -m .
	fi

done < <(cut -d':' -f1,6 /etc/passwd)
