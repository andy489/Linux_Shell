# github.com/andy489

read x
read y
if (($x > $y)); 
then
    echo X is greater than Y;
elif (($x < $y)); 
then
    echo X is less than Y;
else
    echo X is equal to Y;
fi
