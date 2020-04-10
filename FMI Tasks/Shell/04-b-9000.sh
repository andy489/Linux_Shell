#!/bin/bash
# 04-b-9000   first
# github.com/andy489

function count_children {
        ps --ppid "${1}" | head +2 | wc -l
}

ps -eo pid=,ppid= | while read  -d $'\n' pid_and_ppid; do
        read pid ppid < <(echo "${pid_and_ppid}")
        [[ $(count_children "${pid}") -ge $(count_children "${ppid}") ]] && echo "${pid}"
done 2>/dev/null

# ---- ---- ----

#!/bin/bash
# 04-b-9000   second
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
