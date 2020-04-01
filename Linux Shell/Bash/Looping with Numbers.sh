# github.com/andy489

for num in {1..50}
do
    echo $num
done

# ---- OR

seq 1 50

# ---- OR

for ((i=1; i<=50; i=i+1))
do
    echo $i
done

# ---- OR

num=1
while [ $num -le 50 ]
do
    echo $num
    num=$((num+1))
done
