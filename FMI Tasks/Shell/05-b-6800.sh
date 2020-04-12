#!/bin/bash
# 05-b-6800
# github.com/andy489

[ $# -eq 0 ] &&  exit 0

FIRST_ARG="${1}"
FLAG_INCLUDE_HIDDEN="not set"

if [ "${FIRST_ARG}" = -a ]; then
        FLAG_INCLUDE_HIDDEN="-a"
        shift 1
fi

[ $# -eq 0 ] && exit 0

DIR_PATH="${1}"

if [ ! -d "${DIR_PATH}" ]; then
        echo "Invalid directory name!"
        exit 1
elif [ ! -r "${DIR_PATH}" ]; then
        echo "Directory is not readable!"
        exit 2
fi

if [ "${FLAG_INCLUDE_HIDDEN}" = -a ]; then
        echo "Including hidden files:"
        while read -d $'\n' i; do
                BASE_NAME=$(basename ${i})
                if [ -f "${i}" ]; then
                        eval $(stat -s "${i}")
                        echo "${BASE_NAME} (${st_size} bytes)"
                elif [ -d "${i}" ]; then
                        ENTRIES=$(find "${i}" | wc -l | awk '{$1=$1}1')
                        echo "${BASE_NAME} (${ENTRIES} entries)"
                fi
        done < <(find . -maxdepth 1 -mindepth 1)
else
        echo "Not incuding hidden files:"
        for i in "${DIR_PATH}"/*; do
                BASE_NAME=$(basename "${i}")
                if [ -f "${i}" ]; then
                        eval $(stat -s "${i}")
                        echo "${BASE_NAME} (${st_size} bytes)"
                elif [ -d "${i}" ]; then
                        ENTRIES=$(find "${i}" ! -name '.*' | wc -l | awk '{$1=$1}1')
                        echo "${BASE_NAME} (${ENTRIES} entries)"
                fi
        done
fi
