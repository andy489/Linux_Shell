#!/bin/bash
# 05-b-3200 2nd solution
# github.com/andy489

read -p "Enter full directory path: " DIR_PATH

BASE_NAME=$(basename ${DIR_PATH})
FILES_CNT=0
DIRS_CNT=0

for i in "${DIR_PATH}"/*; do
	if [ -f "${i}" ]; then
		FILES_CNT=$((FILES_CNT+=1))
	elif [ -d "${i}" ]; then
		DIRS_CNT=$((DIRS_CNT+=1))
 	fi
#	echo ${i}
done

echo "Count of directories in '${BASE_NAME}' is: ${DIRS_CNT}"
echo "Count of files in '${BASE_NAME}' is: ${FILES_CNT}"
