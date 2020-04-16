#!/bin/bash
# 05-b-4300.sh
# github.com/andy489

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

USERNAME="${1}"

if ! cut -d ':' -f1 /etc/passwd | fgrep -q "^${USERNAME}$"; then
	echo "Invalid username!"
	exit 2
fi

until who -u | awk '{print $1} | fgrep -q "^${USERNAME}$"; do
	sleep 1
done

echo "User ${USERNAME} logged in"
