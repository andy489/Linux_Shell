# !/bin/bash

readarray ary 
echo ${ary[@]/[A-Z]/.}
