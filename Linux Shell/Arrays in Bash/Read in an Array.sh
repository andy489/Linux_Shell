# github.com/andy489

#!/bin/bash

declare -a array
i=0
while read line
do
array[$i]=$line
i+=1
done
echo ${array[@]}

# ---- OR

arr=($(cat))
echo ${arr[@]}
