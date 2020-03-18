**Задача 4.**

```sh
cat "$(awk 'BEGIN{cnt1='"$(wc -l < Bonnie)"'; \ 
cnt2='"$(wc -l < LP)"'} END {if (cnt1 > cnt2) {print "Bonnie"} else {print "LP"}}')" |\
cut -d ' ' -f4- | sort -t '"' | cat > Bonnie.songs
```
