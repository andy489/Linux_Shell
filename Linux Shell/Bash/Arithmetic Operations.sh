# !/bin/bash

read exp
echo $exp | bc -l | xargs printf "%.3f"
