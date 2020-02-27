for num in {1..99..2}
do
    echo $num
done

OR

seq 1 2 99

OR

for ((i=1; i<100; i=i+2))
do
    echo $i
done

OR

num=1
while [ $num -le 99 ]
do
    echo $num
    num=$((num+2))
done
