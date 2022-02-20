# !/bin/bash

arr=($(sed 's/^[A-Z]/./')) 
echo ${arr[@]}
