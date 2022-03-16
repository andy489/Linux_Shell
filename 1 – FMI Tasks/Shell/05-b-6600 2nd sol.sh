#!/bin/bash
# 05-b-6600 2nd solution
# github.com/andy489

[ $# -eq 1 ] || exit 1

[ -d "${1}" ] || exit 2

[ -r "${1}" ] || exit 3

while read -d $'\n' LINE ; do 
	find "${1}" -maxdepth 1 -type f -exec md5sum {} \; \
		| awk -v FIRST="${LINE}" '$1 == FIRST {print $2}' \
		| sort | tail -n +2 | xargs -I{} rm {}
done < <( find "${1}" -maxdepth 1 -type f -exec md5sum {} \; \
	| awk '{ print $1}' | sort | uniq -d )
