#!/bin/bash
# 05-b-4300
# github.com/andy489

if [ $# -ne 2 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

ADDRESS_BOOK="${1}"
NICKNAME="${2}"

if [ ! -f "${ADDRESS_BOOK}" ]; then
	echo "Invalid file argument!"
	exit 2
elif [ ! -r "${ADDRESS_BOOK}" ]; then
	echo "File is not readable!"
	exit 3
fi

if ! fgrep -wq "${NICKNAME}" "${ADDRESS_BOOK}"; then
        echo "Nickname ${NICKNAME} is not in the address book!"
        exit 4
fi

USERNAME=$(fgrep -w "${NICKNAME}" "${ADDRESS_BOOK}" | head -1 | awk '{print $2}')

if ! id -u "${USERNAME}" &>/dev/null; then
	echo "Invalid username!"
	exit 5
elif ! who | awk '{print $1}' | fgrep -qE "^${USERNAME}$"; then
	echo "User ${USERNAME} is not logged in!"
	exit 6
fi

read -p "Enter message: " MSG

echo "${MSG}" | write "${USERNAME}"
