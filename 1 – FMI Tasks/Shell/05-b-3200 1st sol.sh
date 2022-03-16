#!/bin/bash
# 05-b-3200 1st solution

read -p "Enter full directory path: " DIR_PATH

BASE_NAME=$(basename ${DIR_PATH})

echo "Count of directories in '${BASE_NAME}' is: \
$(find "${DIR_PATH}" -mindepth 1 -maxdepth 1 -type d 2>/dev/null |\
wc -l)"

echo "Count of files in '${BASE_NAME}' is: \
$(find "${DIR_PATH}" -mindepth 1 -maxdepth 1 -type f 2>/dev/null |\
wc -l)" 
