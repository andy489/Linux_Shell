## 'Uniq' command #3

[Judge System](https://www.hackerrank.com/challenges/text-processing-in-linux-the-uniq-command-3/problem)

Given a text file, count the number of times each line repeats itself (only consider consecutive repetions). Display the count and the line, separated by a space. There shouldn't be leading or trailing spaces. Please note that the uniq -c command by itself will generate the output in a different format.

This time, compare consecutive lines in a **case insensitive** manner. So, if a line **X** is followed by case variants, the output should count all of them as the same (but display only the form X in the second column).

So, as you might observe in the case below: aa, AA and Aa are all counted as instances of 'aa'.

#### Sample Input

````
00
00
01
01
00
00
02
02
03
aa
AA
Aa
````

#### Sample Output

````
2 00
2 01
2 00
2 02
1 03
3 aa 
````

#### Explanation

````
00 is repeated twice
01 is repeated twice
00 is repeated twice
02 is repeated twice
03 occurs once
aa is repeated thrice (if we ignore case - AA, Aa are the same as 'aa')
````
