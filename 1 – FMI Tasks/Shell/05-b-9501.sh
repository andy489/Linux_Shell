#!/bin/bash
# 05-b-9501
# github.com/andy489

if [ $# -ne 1 ]; then
	echo "Invalid number of arguments!"
	exit 1
fi

SC="${1}" #S = starting color 

if ! grep -qE '^-[rgbx]$' <(echo "${SC}"); then
	echo "Invalid color argument!"
	exit 2
fi

case "${SC}" in
-r) COUNT=0
;;
-g) COUNT=1
;;
-b) COUNT=2
;;
-x) COUNT=3
;;
esac

if [ ${COUNT} -eq 3 ]; then

while read LINE; do
echo "${LINE}"
done

exit 0

fi

function change_color {  
N=${1}
STR=${2}
REM=$((${N} % 3))

case "${REM}" in
0)
echo -e "\033[0;31m${STR}"
;;
1)
echo -e "\033[0;32m${STR}"
;;
2)
echo -e "\033[0;34m${STR}"
;;
esac
}

while read LINE; do 
change_color ${COUNT} ${LINE}
COUNT=$(( COUNT + 1 ))
done

# Usage: cat file.txt | ./05-b-9501 -r
