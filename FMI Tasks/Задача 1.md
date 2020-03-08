**Задаза 1.** Даден е текстов файл с име <img src="https://latex.codecogs.com/svg.latex?\Large&space;philip-j-fry.txt">. Напишегте shell script и/или серия от команди, които извеждат броя редове, съдържащи поне една четна цифра и несъдържщи малка латинска буква от <img src="https://latex.codecogs.com/svg.latex?\Large&space;a"> до <img src="https://latex.codecogs.com/svg.latex?\Large&space;w">.

Примерно съдържание на файла:
```
123abv123
123zz123
MMU_2.4
```

Примерен изход:

```
2
```

**Решение:** 

Първи начин:

```sh
awk '$0~/[02468]/ && $0~/[a-w]/ {print}' philip-j-fry.txt | wc -l
```

Втори назичин:

```sh
grep '[02468]' philip-j-fry.txt | grep -vc '[a-w]'
```

Трети начин:

```sh
sed -n '/[02468]/ p' philip-j-fry.txt | sed -n '/[a-w]/ !p' | wc -l
```
