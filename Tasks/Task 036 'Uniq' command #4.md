## 'Uniq' command #4

[Judge System](https://www.hackerrank.com/challenges/text-processing-in-linux-the-uniq-command-4/problem)

Given a text file, display only those lines which are **not** followed or preceded by identical replications.

#### Sample Input

````
A00
a00
01
01
00
00
02
02
A00
03
aa
aa
aa
````

#### Sample Output

````
A00
a00
A00
03
````

#### Explanation

The comparison is case sensitive, so the first instance of "A00" and "a00" are considered different, hence unique.

The next instance of A00 is succeeded and preceded by different lines, so that is also included in the output.

The same holds true for 03 - it is succeeded and preceded by different lines, so that is also included in the output. 


