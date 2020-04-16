#!/bin/bash
# 18-05-2016-2
# github.com/andy489

: '
Напишете shell скрипт, който приема един позиционен параметър - число. Ако скриптът се изпълнява като root, да извежда 
обобщена информация за общото количество активна памет (RSS - resident set size, non-swapped physical memory that a task 
has used) на процесите на всеки потребител. Ако за някой потребител обобщеното число надвишава подадения параметър, да 
изпраща подходящи сигнали за прекратяване на процеса с най-много активна памет на потребителя.
	Забележка: Приемаме, че изхода в колоната RSS е число в същата мерна единица, като числото, подадено като аргумент.
'

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments"
	exit 1
fi

N="${1}"

if ! grep -qE '^[-+]?[0-9]+$' <(echo "${N}"); then
	echo "Invalid argument"
	exit 2
fi

USERS=$(ps -e -o user= | grep -v '^_' | sort | uniq)

for USER in ${USERS}; do
	USER_TOTAL_RSS=0
	while read PID RSS; do
		USER_TOTAL_RSS=$(expr $USER_TOTAL_RSS + $RSS)	
		LAST_PID="${PID}"
	done < <(ps -u "${USER}" -o pid=,rss= | sort -n -k2)
		
	echo "Total ${USER_TOTAL_RSS}" resident set size for user" ${USER}"
	
	if [ $(id -u) -ne 0 -a ${USER_TOTAL_RSS} -gt ${N} ]; then
		kill -s TERM "${LAST_PID}"
		sleep 2
		kill -s KILL "${LAST_PID}"	
	fi
done
