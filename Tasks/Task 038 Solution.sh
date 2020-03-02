declare -a arr
i=0
while read line
do
arr[$i]=$line
((i++))
done

echo ${arr[@]:3:5}


/*
OR
*/

arr=($(cat))
echo ${arr[@]:3:5}
