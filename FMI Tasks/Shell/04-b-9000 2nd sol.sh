#!/bin/bash
# 04-b-9000 2nd solution
# github.com/andy489

PROCESS_FILE="$(mktemp)"

ps -e -o pid=,ppid= > "${PROCESS_FILE}"

function get_children() {
	cat "${PROCESS_FILE}" | awk -v pid="${1}" '$2 == pid { print $1 }'
}

function count_children() {
	get_children "${1}" | wc -l
}

while read PID PPID; do
	[ $(count_children "${PID}") -ge $(count_children "${PPID}") ] && echo "${PID}"
done < "${PROCESS_FILE}"

rm -- "${PROCESS_FILE}"
