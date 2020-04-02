# github.com/andy489
# 04-b-6100
# Изведете имената на потребителите, които имат поне 2 процеса, чиято команда е vim

ps -eo user,comm | awk '$2~/vim$/' | cut -d' ' -f1 | uniq -d
