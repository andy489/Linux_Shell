# !/bin/bash

readarray arr
echo ${arr[@]/[A-Z]/.}
