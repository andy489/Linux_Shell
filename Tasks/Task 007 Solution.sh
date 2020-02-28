read char
echo -e 'YES\nNO\n' | grep -i $char

/*
OR
*/

read char
if [ $char == 'Y' -o $char == 'y' ]
then
    echo 'YES'
else
    echo 'NO'
fi
