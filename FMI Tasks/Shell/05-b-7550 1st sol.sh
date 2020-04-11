#!/bin/bash
# 05-b-7550 1st solution
# github.com/andy489
: '
Do not use kill -9
It does not give the process a chance to cleanly:
1) shut down socket connections
2) clean up temp files
3) inform its children that it is going away
4) reset its terminal characteristics
	and so on and so on.
Generally, send 15, and wait a second or two, and if that does not work, send 2, 
and if that does not work send 1. If that doesnt't REMOVE THE BINARY because
the program is badly behaved!
'
if [ $# -ne 1 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

USER="${1}"

if [ $(id -u "${USER}") -eq 1 ]; then
	echo "Invalid username!"
	exit 2
fi
 
PS_CNT=0

while read -d $'\n' PID; do
	kill -15 "${PID}"
	sleep 1
	kill -9 "${PID}"
	CNT=(($CNT+1))
done < <(ps -u "${USER}" -o pid=)

echo "Total: ${PS_CNT} killed processes"
