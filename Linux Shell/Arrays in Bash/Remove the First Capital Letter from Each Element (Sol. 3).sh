# github.com/andy489

#!/bin/bash

readarray ary 
echo ${ary[@]/[A-Z]/.}
