# !/bin/bash

array=($(cat))
Total=("${array[@]}" "${array[@]}" "${array[@]}")
echo ${Total[@]}
