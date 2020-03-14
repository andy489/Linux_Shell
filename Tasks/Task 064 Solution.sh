read size
tr ' ' '\n' | sort -n | uniq -c | awk '$1==1 {print $2}'

/*
OR
*/

read size
echo $(($(sed 's/ /^/g')))
