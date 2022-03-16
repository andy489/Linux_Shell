#!/bin/bash
# 21.sh
  
[ $(id -u) -eq 0 ] || { echo "Script $0 not executed as root, nothing done!"; exit 1; }

for U in $(ps -e -o user= | grep -v "^_" | sort | uniq); do
        TOTAL_RSS=0
        PS_CNT=0

        while read CPID CRSS; do
                PS_CNT=$(expr ${PS_CNT} + 1 )
                TOTAL_RSS=$(expr ${TOTAL_RSS} + ${CRSS})
                MAX_RSS=${CRSS}
                MAX_RSS_PID=${CPID}
        done < <(ps -u "${U}" -o pid=,rss= | sort -n -k 2)

        if [ ${PS_CNT} -eq 0 ]; then
                continue
        fi

        AVG_RSS=$(expr ${TOTAL_RSS} '/' ${PS_CNT})
        echo "${U} ${PS_CNT} ${TOTAL_RSS}"

        if [ ${MAX_RSS} -gt $(expr ${AVG_RSS} '*' 2) ]; then
                echo "killing process ${MAX_RSS_PID}"
                kill -s SIGTERM ${MAX_RSS_PID}             
                sleep 2                                    
                kill -s SIGKILL ${MAX_RSS_PID}
        fi
done
