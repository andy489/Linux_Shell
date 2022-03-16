#!/bin/bash
# 05-b-4700 1st solution
# girhub.com/andy489

if [ $# -eq 1 ]; then
	DELIMITER=" "
elif [ $# -eq 2 ]; then
	DELIMITER="${2}"
else 
	echo "Invalid number of parameters"
	exit 1
fi

grep -qE '^[-+]?[0-9]+$' <(echo "${1}")

if [ ! $? -eq 0 ]; then
	echo "Invalid integer number!"
	exit 2
fi

NUM=$(echo "${1}")
SIGN=$(echo "${1}" | cut -c1)

FLAG=0
grep -q [-+] <(echo "${SIGN}") 

if [ $? -eq 0 ]; then
	NUM=$(echo "${NUM}" | cut -c2-)	
	FLAG=1
fi

REV_NUM=$(echo "${NUM}" | gsed 's/./&\n/g' | tac)

CNT=1

for i in ${REV_NUM}; do
	NEW_NUM+="${i}"
	if [ $(($CNT%3)) -eq 0 ]; then
		NEW_NUM+="${DELIMITER}"
	fi
	CNT=$((CNT+1))
done

DEL_FIRST_DELI=0

if [ $(( (${CNT}-1)%3 )) -eq 0 ]; then
	DEL_FIRST_DELI=1
fi  

[ "${FLAG}" -eq 1 ] && echo -n "${SIGN}"
REST_NUM=$(echo -e "${NEW_NUM}" | grep -o . | tac | tr -d "\n" ; echo)
if [ ${DEL_FIRST_DELI} -eq 1 ]; then
	REST_NUM=$(echo "${REST_NUM}" | cut -c2-)
fi

echo "${REST_NUM}"
