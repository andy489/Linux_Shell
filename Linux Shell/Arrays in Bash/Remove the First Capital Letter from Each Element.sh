# github.com/andy489

#!/bin/bash

arr=($(sed 's/^[A-Z]/./')) 
echo ${arr[@]}

# ---- OR

readarray arr
echo ${arr[@]/[A-Z]/.}

# ---- OR

readarray ary 
echo ${ary[@]/[A-Z]/.}

# ---- OR

sed 's/^[A-Z]/./' | paste -sd' '
