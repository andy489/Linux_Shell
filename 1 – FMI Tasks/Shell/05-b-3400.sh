#!/bin/bash
# 05-b-3400

read -p "Enter full path name of file: " FILE_NAME
read -p "Enter string to match: " EXP

grep -qsF "${EXP}" "${FILE_NAME}"
echo $?
