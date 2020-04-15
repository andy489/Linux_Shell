#!/bin/bash
# 05-b-9100.sh
# github.com/andy489

if [ $# -ne 2 ]; then
	  echo "Invalid number of arguments!"
	  exit 1
fi

S="${1}" # S=SOURCE
D="${2}" # D=DESTINATION

if [ ! -d "${S}" ]; then
	  echo "Invalid source directory!"
	  exit 2
elif [ ! -r "${S}" ]; then
	  echo "Source directory is not readable!"
	  exit 3
fi

if [ ! -d "${D}" ]; then
      echo "Invalid destination directory!"
      exit 4
elif [ ! -r "${S}" ]; then
    exho "Destination directory is not readable!"
	  exit 5
fi

while read EXT; do
	mkdir -p "${D}/${EXT}"
	gfind "${S}" -type f -name "*.${EXT}" 2>/dev/null -print0 | xargs -0 -I {} mv {} "${D}/${EXT}"
done < <(gfind "${S}" -type f -name "*.*" -printf "%f\n" \
	| rev | cut -d '.' -f1 | rev | sort | uniq )
