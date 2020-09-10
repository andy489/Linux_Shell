# github.com/andy489

#!/bin/bash

arr=($(cat | grep -iv 'a'))
echo ${arr[*]}
