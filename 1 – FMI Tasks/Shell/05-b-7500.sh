#!/bin/bash
# 05-b-7500
# github.com/andy489

NUM=$(( (RANDOM%200)-100 ))

GUESSES_CNT=0

read -p 'Guess? ' GUESS

function validate_guess {
        grep -qE '^[+-]?[0-9]+$' <(echo "${1}")
}

while true; do
        GUESS_CNT=$((GUESS_CNT+1))
        if validate_guess "${GUESS}"; then
                if [ "${GUESS}" -gt "${NUM}" ]; then
                        echo '...smaller!'
                elif [ "${GUESS}" -lt "${NUM}" ]; then
                        echo '...bigger!'
                else
                        echo "RIGHT! Guessed ${NUM} in ${GUESS_CNT} tries!"                                    
                        exit 0
                fi
        else
                echo 'Not a valid guess!'
        fi
        read -p 'Guess? ' GUESS
done
