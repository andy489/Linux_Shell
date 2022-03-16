#!/bin/bash
# github.com/andy489

FILE="tmp.txt"

if [ -f "${FILE}" ]; then
	RUN_INDX="$(cat "${FILE}")"
else
	RUN_INDX=0
fi

RUN_INDX="$(expr $RUN_INDX + 1)"
echo "${RUN_INDX}" > "${FILE}"

case "${RUN_INDX}" in
	1) sleep 3; exit 2 ;;
	2) sleep 3; exit 3 ;;
	3) sleep 0.5; exit 0 ;;
	4) sleep 1; exit 1 ;;
	5) sleep 1; exit 0 ;;
	6) sleep 1; exit 1 ;;
	7) sleep 1; exit 1 ;;
	*) echo "this should never be written" >&2; sleep 1; exit 1 ;;
esac
