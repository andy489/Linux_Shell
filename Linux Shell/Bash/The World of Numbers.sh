# github.com/andy489

#! /bin/bash

read x
read y
printf "%s\n" $x{+,-,*,/}"($y)" | bc

# ---- OR

read x
read y
for i in {+,-,"*",/}
do
    var=$(((x)$i(y)))
    echo $var
done
