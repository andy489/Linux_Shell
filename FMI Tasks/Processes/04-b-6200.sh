# github.com/andy489
# 04-b-6200
# Изведете имената на потребителите, които не са логнати в момента, но имат живи процеси

grep -vFxf <(who | cut -d' ' -f1 | sort | uniq) <(ps -eo user= | sort | uniq)

comm -23 <(ps -eo user= | sort | uniq) <(who | cut -d' ' -f1 | sort | uniq)
