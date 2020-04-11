#!/bin/bash
# 04-b-9000   second
# github.com/andy489

PROCESS_FILE="$(mktemp)"

ps -e -o pid=,ppid= > "${PROCESS_FILE}"

function get_children {
	cat "${PROCESS_FILE}" | awk -v pid="{1}" '$2 == pid { print $1 }'
}

function count_children {
	get_children "${1}" | wc -l
}

while read -d $'\n' LINE; do
	read -d ',' pid ppid < <(echo "${LINE}")
	[ $(count_children "${pid}") -ge $(count_children "${ppid}") ] &&\
		echo "${pid}"
done < "${PROCESS_FILE}"

rm "${PROCESS_FILE}"
