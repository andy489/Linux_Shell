#!/bin/bash
# 28.sh A) cycle

TEMP=$(mktemp)

cat | egrep "^[-+]?[0-9]+$" | sort -n > "${TEMP}"

[ "$(awk 'END{print NR}' "${TEMP}")" -ne 0 ] || { echo -n "\nno valid numbers"; exit 1; } 

MAX="$(tail -1 "${TEMP}")"
MIN="$(head -1 "${TEMP}")"

if [ "${MAX}" -eq "${MIN}" ];then
    echo -e "\n${MAX}"
else
    ABS_MIN="$(sed -E "s/-//" <(echo "${MIN}"))"
    ABS_MAX="$(sed -E "s/-//" <(echo "${MAX}"))"
    if [ "${ABS_MIN}" -eq "${ABS_MAX}" ];then
        echo -e "\n${MIN}"
        echo "${MAX}"
    elif [ "${ABS_MIN}" -lt "${ABS_MAX}" ]; then
        echo -e "${MAX}"
    else
        echo -e "\n${MIN}"
    fi
fi

rm -- "${TEMP}"

exit 0
