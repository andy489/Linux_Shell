arr=($(sed 's/^[A-Z]/./')) 
echo ${arr[@]}

/*
OR
*/

readarray arr
echo ${arr[@]/[A-Z]/.}

/*
OR
*/

readarray ary 
echo ${ary[@]/[A-Z]/.}

/* 
OR (without array)
*/

sed 's/^[A-Z]/./' | paste -sd' '
