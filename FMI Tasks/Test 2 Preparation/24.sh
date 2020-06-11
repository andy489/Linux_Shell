#!/bin/bash
# 24.sh
# github.com/andy489

#[ $(id -u) -eq 0 ] || { echo "Script $0 is not executed as root."; exit 1; }

OLD_IFS="${IFS}"
IFS=":"

total_root_rss="$(ps -u "root" -o rss= | awk '{s+=$1}END{print s}')"

while read USER HOME; do

	[ "${USER}" != "root" ] || continue
	# stat --format, if not macOS
	[ ! -d "${HOME}" ] || [ "$(stat -c "%U" "${HOME}")" != "${USER}" ] || [ ! "$(stat -c "%A" "${HOME}"| cut -c3)" = "w" ] || continue

	# echo "${USER} ${HOME}"

	total_user_rss=$(ps -u "${USER}" -o rss= | awk '{s+=$1}END{print s}')
	
	if [ "${total_root_rss}" -gt "${total_user_rss}"  ]; then
		killall -u "${USER}"
		sleep 2
		killall -u "${USER}" -s SIGKILL
	fi

done < <(cat /etc/passwd | cut -d':' -f1,6 )

IFS="${OLD_IFS}"
