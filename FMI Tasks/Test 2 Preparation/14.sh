#!/bin/bash
# 14.sh
# github.com/andy489

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

[ $(id -u) -eq 0 ] || exit 2

N="${1}"

if ! grep -qE '^[-+]?[0-9]+$' <(echo "${N}"); then
	echo "Invalid argument"
	exit 3
fi

USERS=$(ps -e -o user= | grep -v '^_' | sort | uniq)

for USER in ${USERS}; do
	USER_TOTAL_RSS=0
	while read PID RSS; do
		USER_TOTAL_RSS=$(expr $USER_TOTAL_RSS + $RSS)	
		LAST_PID="${PID}"
	done < <(ps -u "${USER}" -o pid=,rss= | sort -n -k2)
		
	echo "Total ${USER_TOTAL_RSS}" resident set size for user" ${USER}"
	
	if [ ${USER_TOTAL_RSS} -gt ${N} ]; then
		kill -s TERM "${LAST_PID}"
		sleep 2
		kill -s KILL "${LAST_PID}"	
	fi
done
