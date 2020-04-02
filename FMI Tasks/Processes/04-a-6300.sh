# github.com/andy489
# 04-a-6300
# Изведете командата на най-стария процес

ps -eo cmd= --sort=start_time | head -1
