# !/bin/bash

arr=($(cat | grep -iv 'a'))
echo ${arr[*]}
