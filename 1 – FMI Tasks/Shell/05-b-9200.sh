#!/bin/bash
# 05-b-9200
# github.com/andy489

echo -e "Usage:\n-r [ args ] deletes recursively files in given args\n"

[ $# -eq 0 ] && exit 0

FIRST_ARG="${1}"
FLAG_RECUR=0

if [ "${FIRST_ARG}" = -r ]; then
	FLAG_RECUR=1
	shift 1
fi

export LOGFILE=$(mktemp)

for i; do
	if [ -d "${i}" ]; then
		DIR_CONTENTS=$(find "${i}" -mindepth 1 | wc -l | awk '{$1=$1}1')
		if [ "${DIR_CONTENTS}" -eq 0 ]; then

			echo "[$(date +"Y-%m-%d %H:%M:%S")] Removed directory ${i}/" >> "${LOGFILE}"
			rm -di "${i}" # rmdir "${i}"
		
		elif [ "${FLAG_RECUR}" -eq 1 ]; then
			
			echo "[$(date +"Y-%m-%d %H:%M:%S")] Removed directory recursively ${i}/" >> "${LOGFILE}"
			rm -Rdi "${i}"
		
		fi
	elif [ -f "${i}" ]; then
		echo "[$(date +"%Y-%m-%d %H:%M:%S")] Removed file ${i}" >> "${LOGFILE}"
		rm -di ${i}
	fi
done

echo "Log file info:"
cat ${LOGFILE}
