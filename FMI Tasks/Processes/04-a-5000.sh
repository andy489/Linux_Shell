# github.com/andy489
# 04-a-5000
# Намерете командите на 10-те най-стари процеси в системата.

ps -eo cmd= --sort=start_time | head
