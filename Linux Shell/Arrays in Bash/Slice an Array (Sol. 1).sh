# github.com/andy489

#!/bin/bash

declare -a arr
i=0
while read line
do
arr[$i]=$line
((i++))
done

echo ${arr[@]:3:5}
