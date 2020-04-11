#!/bin/bash
# github.com/andy489

n=$1 
  
for((r=1; r<=n; ++r)) 
do
    for((j=0; j<n-r; ++j))
    do	 
      	echo -n " "
    done
    	
    for((k=0; k<r; ++k))
    do
        echo -n "*"
    done 

    echo	
done
