# github.com/andy489

read x
read y
for i in {+,-,"*",/}
do
    var=$(((x)$i(y)))
    echo $var
done
