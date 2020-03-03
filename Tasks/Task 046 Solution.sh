read n
arr=($(cat)) 
arr=${arr[*]}
printf "%.3f" $(echo $((${arr// /+}))/$n | bc -l)
