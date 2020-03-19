**Задача 3.** Даден ви е ASCII текстов файл (например /etc/services). Отпечатайте хистограма на N-те (например 10) най-често срещани думи.

**Решение:**

Първи начин (tr)

```sh
tr -cs "[:alpha:]" "\n" < /etc/passwd | sort -n | uniq -c |sort -bnr -t' ' -k1 | head
```

Изход:

```
  99 var
  99 bin
  98 usr
  97 false
  67 empty
  23 Daemon
  20 db
  14 Server
  12 Service
   9 Kerberos
```

Втори начин (awk)

```sh
awk 'BEGIN{FS="[^a-zA-Z]+"}{for(i=1;i<=NF;++i)print $i}' /etc/passwd |sed '/^ *$/d' |\
sort -n | uniq -c | sort -bnr -t' ' -k1 | head
```

Изход:

```
  99 var
  99 bin
  98 usr
  97 false
  67 empty
  23 Daemon
  20 db
  14 Server
  12 Service
   9 Kerberos
```

Трети начин:

```sh
grep -o -E '\w+' /etc/passwd | sort | uniq -c | sort -rn | head
```
