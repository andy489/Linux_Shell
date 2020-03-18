**Задача 11.** Вие сте асистент по ОС. На първото упражнение казвате на студентите да си напишат данните на лист, взимате го и им правите акаунти. След упражнението обаче, забравяте да вземете листа със себе си - сещате се половин час по-късно, когато трябва да въведете имената на студентите в  таблица, но за зла беда в стаята вече няма ни помен от листа (вероятно иззет от спешния отряд на GDPR-полицията)

Сещате се, че в началото на упражнението UNIX-часовниът е показвал 155116800, а в края 1551176100.

Напишете команда, която изкарва с таб факултетните номера и имената на потребителите от специалност СИ, чиито home директории са променили статуса си (status change time) в зададения времеви интервал.

Приемете, че всички потребители от СИ имат home директории под /home/SI.

*Примерен изход:*

```
62198   Ivaylo Georgiev
62126   Victoria Georgieva
62009   Denitsa DObreva
62208   Trayana Nedelcheva
```

*Няколко реда от /etc/passwd за справка:*

```
s62136:x:1302:503:Alexander Ignatow, SI, 2, 2:/home/KN/s62136:/bin/bash
s62171:x:1031:504:Deivid Metanov:/home/SI/s62171:/bin/bash
s62126:x:1016:504:Victoria Georgieva:/home/SI/s62126:/bin/bash
s62009:x:1170:504:Denitsa DObreva,SI,3,3:/home/SI/s62009:/bin/bash
s62196:x:1221:504:Elena Tuparova,SI,2,1:/home/SI/s62196:/bin/bash
```


**Решение:**

Първи начин:

```sh
find /home/SI -maxdepth 1 -type d - newerct '@1551168000' -not -newerct '@1551176100' \
-exec grep -F :{}: /etc/passwd \; | cut -d':' -f1,5 | cut -c2- | cut -d',' -f1 | tr ':' '\t'
```

Втори начин:

```sh
find /home/SI -maxdepth 1 -type d -printf "%C@ %p\n" |\
awk '$1 > 1551168000 && $1 < 1551176100 {for(i=2;i<NF;++i)printf "$i ", printf "$NF\n"}' \
...further process
```

Трети начин:

```sh
egrep '^([^:]*:[^:]*){4}:/home/SI/' /etc/passwd | cut -d':' -f6 | xargs gstat -c "%Z %n\n" |\
egrep "$(echo {1551168000..1551176100} | tr ' ' '|')" | cut -d' ' -f2 | grep -f - /et/passwd | tr ':' '\t' | cut -f1,5 | cut -c2-  
```
