#!/bin/bash
# 14.sh
# github.com/andy489

[ $# -eq 1 ] || { echo "Invalid number of arguments. Usage: $0 <number>"; exit 1; }

[ $(id -u) -eq 0 ] || exit 0

N="${1}"

if ! grep -qE '^[-+]?[0-9]+$' <(echo "${N}"); then
	echo "Invalid argument"
	exit 3
fi

USERS=$(mktemp)
ps -e -o user= | sort | uniq > "${USERS}"

while read _USER; do
	USER_TOTAL_RSS=0
	while read PID RSS; do
		USER_TOTAL_RSS=$(expr $USER_TOTAL_RSS + $RSS)	
		LAST_PID="${PID}"
	done < <(ps -u "${_USER}" -o pid=,rss= | sort -n -k2)
		
	echo "Total ${USER_TOTAL_RSS} resident set size for user ${_USER}"
	
	if [ ${USER_TOTAL_RSS} -gt ${N} ]; then
		kill -s TERM "${LAST_PID}"
		sleep 1
		kill -s KILL "${LAST_PID}"	
	fi
done < "${USERS}"
rm -- "${USERS}"
