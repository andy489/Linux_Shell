#!/bin/bash
# 05-b-9600
# github.com/andy489

[ $# -eq 0 ] && exit 1

FIRST_ARG="${1}"
FLAG=0

if [ "${FIRST_ARG}" = -r ];then
	FLAG=1
	shift 1
fi

function timestamp {
	date +"%Y-%m-%d-%H-%M-%S"
}

function archive_and_save {
	SRC=$1
	PREFIX=$2
	TIMESTAMP=$(timestamp)
	tar -czf "${BACKUP_DIR}/${PREFIX}_${TIMESTAMP}.tgz" "${SRC}"
}

function compress_and_save {
	SRC=$1
	TIMESTAMP=$(timestamp)
	gzip -c "${SRC}" > "${BACKUP_DIR}/${SRC}_${TIMESTAMP}.gz" 
}

for i; do
	if [ -d "${i}" ]; then 
		DIR_CONTENT=$(find "${i}" -mindepth 1 -maxdepth 1 2>/dev/null | wc -l)
		if [ "${DIR_CONTENT}" -eq 0 ]; then 
			archive_and_save "${i}" "empty-dir_"
			rm -i -d "${i}"
		elif [ "${FLAG}" -eq 1 ];then
			archive_and_save "${i}" "full-dir_"
			rm -i -d -R "${i}"
		fi
	elif [ -f "${i}" ]; then 
		compress_and_save "${i}"
		rm -i -- "${i}"
	fi
done
