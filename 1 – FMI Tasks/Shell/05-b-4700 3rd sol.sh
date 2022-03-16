#!/bin/bash
# 05-b-4700 3rd solution
# github.com/andy489

case $# in
1) N="${1}"; D=" "
;;
2) N="${1}"; D="${2}"
;;
*) echo "Invalid number of arguments!"; exit 1
;;
esac
	
if ! grep -qE '^[-+]?[0-9]+$' <(echo "${N}"); then
	echo "Invalid number!"
	exit 2	
fi

echo "${N}" | rev | sed -E "s/(.{3})/\1$D/g" | rev \
	| sed -e "s/^$D//" \
	| sed -E "s/^([-+])$D/\1/"
