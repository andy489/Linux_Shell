**Задача 6.** Напишете серия от команди, които изтриват:<br>
а) всички файлове в текущата директория и нейните поддиректории, които са с нулева дължина;<br>
б) 5-те най-големи файла в home директорията на текущия потребител и нейните поддиректории.

**Решение:**

a)

Първи начин:

```sh
find . -maxdepth 2 2>/dev/null | xargs -I {} stat -l {} |\ 
awk '$5==0 {for i=10;i<NF;++i)printf $i" ";print $NF}' |\
xargs -I {} -rm -i {}
```

Втори начин:

```sh
gfind . -maxdepth 2 2>/dev/null -printf"%s %p\n" |\
awk '$1==0{for(i=2;i<NF;++i)printf $i" ";print $NF}' |\
xargs -I {} rm -i {}
```

Трети начин:

```sh
find . -maxdepth 2 2>/dev/null -size 0 -exec rm -i {}\;
```

б)

```sh
gfind ~ -maxdepth 2 2>/dev/null -printf"%s %p\n" |\
sort -rn -t' ' k1 | head -5 | cut -d' ' -f2- | xargs -I {} rm -i {} 
```
