# !/bin/bash
arr=($(cat))
echo ${arr[@]/*[aA]*/}
