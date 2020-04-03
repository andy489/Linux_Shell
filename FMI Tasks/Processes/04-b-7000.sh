# github.com/andy489
# 04-b-7000
# Намерете колко физическа памет заемат осреднено всички процеси на потребителската група students. Внимавайте, когато групата няма нито един процес.

ps -e -g students -o rss= | awk '{++lines; m+=$1}END{OFMT="%.3f"; if(lines==0){print 0} else {print m/lines}}'
