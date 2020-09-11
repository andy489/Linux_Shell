# github.com/andy489

read X
read Y
if [ "$X" -gt "$Y" ]; then
echo "X is greater than Y"
elif [ "$X" -lt "$Y" ]; then
echo "X is less than Y"
elif [ "$X" -eq "$Y" ]; then
echo "X is equal to Y"
fi
