# github.com/andy489

#!/bin/bash

read x
read y
printf "%s\n" $x{+,-,*,/}"($y)" | bc
