#!/bin/bash
# 28.sh A)
# github.com/andy489

temp=$(mktemp)

cat | egrep "^[-+]?[0-9]+$" | sort -n > "$temp"

[ "$(cat "$temp" | wc -l )" -ne 0 ] || { echo -n  "\nno valid numbers numbers"; exit 1; } 

MAX="$(cat "$temp" | tail -n 1)"
MIN="$(cat "$temp" | head -n 1)"

if [ $MAX -eq $MIN ];then
    echo -e "\n$BIGGEST"
else
    ABS_MIN="$( echo $MIN | sed -E "s/-//")"
    ABS_MAX="$( echo $MAX | sed -E "s/-//")"
    if [ $ABS_MIN -eq $ABS_MAX ];then
        echo -e "\n$MIN"
        echo "$MAX"
    elif [ $ABS_MIN -lt $ABS_MAX ]; then
        echo -e "$MAX"
    else
        echo -e "\n$MIN"
    fi
fi

rm -- "${temp}"

exit 0
