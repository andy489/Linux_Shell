# github.com/andy489

#!/bin/bash

arr=($(cat))
echo ${arr[@]/*[aA]*/}
