#!/bin/bash
# 25.sh
# github.com/andy489

[ $# -eq 1 ] || { echo "Usage: ${0} <LOGDIR>"; exit 1; }

LOGDIR="${1}"

if [ ! -d "${LOGDIR}" ];then
  echo "Invalid directory name"
  exit 2
elif [ ! -r "${LOGDIR}" ]; then
  echo "Directory not readable"
  exit 3
fi

RECORDS=$(mktemp)

while read FRIEND; do

  LINES="$(find "${LOGDIR}" -mindepth 4 -maxdepth 4 -type f \
    | fgrep "$FRIEND" \
    | xargs -I {} wc -l {} \
    | awk '{print $1}' \
    | awk '{sum += $1}END{print sum}')"

  echo "$LINES $FRIEND" >> "${RECORDS}"

done < <(gfind "${LOGDIR}" -mindepth 3 -maxdepth 3 -type d \
        | cut -d '/' -f 4 | sort | uniq)

cat "${RECORDS}" | sort -rn -k1 | head -n 3

rm -- "${RECORDS}"

exit 0
