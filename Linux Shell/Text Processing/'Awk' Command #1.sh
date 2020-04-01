# github.com/andy489

awk '{if(length($4)==0) print "Not all scores are available for",$1;}'

# ---- OR

awk '{if(NF < 4) print "Not all scores are available for",$1;}'
