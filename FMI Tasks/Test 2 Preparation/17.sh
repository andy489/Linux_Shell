#!/bin/bash
# 17.sh
# github.com/andy489

[ $# -eq 1 ] || { echo "Invalid number of arguments"; exit 1; }

FOO="${1}"

$(id -u "${FOO}" &>/dev/null) || { echo "Invalid username!"; exit 2; }
[ $(id -u) -eq 0 ] || exit 0 

# If we are here, we are sure that script is executed as root.

PS=$(mktemp)
UNIQUE_USERS=$(mktemp)

ps -e -o user=,pid=,etimes= | grep -v '^_' | sort -t' ' -k1 > "${PS}"

FOO_PS_CNT=$( awk -v "foo=${FOO}" '{ if ($1 == foo) ++cnt } END {print cnt}' "${PS}" )
cat "${PS}" | cut -d ' ' -f 1 | uniq > "${UNIQUE_USERS}"


echo "a)"
while read USER; do
    CUR_CNT=$(grep -c "${USER}" "${PS}")

    if [ -z "${FOO_PS_CNT}" ]; then
        FOO_PS_CNT=0
    fi

    if [ "${CUR_CNT}" -gt "${FOO_PS_CNT}" ]; then
        echo "${USER}"
    fi

done < "${UNIQUE_USERS}"
echo

echo "b)"
        AVG=$(awk '{total += $3}END{print int(total/NR)}' "${PS}")
echo

while read PID ETIMES; do
    if [ "${ETIMES}" -gt $( echo "${AVG} * 2" | bc ) ]; then
        kill -15 PID
        sleep 2
        kill -9 PID
    fi
done < <( cat "${PS}" | fgrep "^${FOO}")

rm -- "${PS}"
rm -- "${UNIQUE_USERS}"
