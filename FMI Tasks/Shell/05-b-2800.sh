#!/bin/bash
# 05-b-2800   first
# github.com/andy489
if [ $# -ne 1 ]; then
	echo "Not one parameter!"
	exit 1
elif [[ "${1}" =~ ^[[:alnum:]]+$ ]]; then
	echo "True"
	exit 0
else
	echo "False"
	exit 1
fi	

#!/bin/bash
# 05-b-2800   second
if [ $# -ne 1 ]; then
	echo "Not one parameter!"
	exit 1
elif echo "${1}" | grep -qE '^[[:alnum:]]+$'; then
	echo "True"
	exit 0
else
	echo "False"
	exit 1
fi	
