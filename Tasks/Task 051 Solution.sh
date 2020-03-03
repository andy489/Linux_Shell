arr=($(cat))
echo ${arr[@]/*[aA]*/}

/*
OR
*/

arr=($(cat | grep -iv 'a'))
echo ${arr[*]}
