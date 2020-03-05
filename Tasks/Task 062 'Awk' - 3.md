## 'Awk' - 3

[Judge System](https://www.hackerrank.com/challenges/awk-3/problem)

#### An Introduction to 'awk'

There are a lot of quick tricks which may be accomplished with the 'awk' interpreter. Among other things, awk may be used for a lot of text-munging and data-processing tasks which require some quick scripting work.

Examples with awk

[Print Examples](https://www.thegeekstuff.com/2010/01/awk-introduction-tutorial-7-awk-print-examples/)<br> 
[Conditionals with Awk](https://www.thegeekstuff.com/2010/02/awk-conditional-statements/)

#### Task

You are provided a file with four space-separated columns containing the scores of students in three subjects. The first column, contains a single character (A-Z) - the identifier of the student. The next three columns have three numbers (each between 0 and 100, both inclusive) which are the scores of the students in English, Mathematics and Science respectively.

Your task is to identify the performance grade for each student. If the average of the three scores is 80 or more, the grade is 'A'. If the average is 60 or above, but less than 80, the grade is 'B'. If the average is 50 or above, but less than 60, the grade is 'C'. Otherwise the grade is 'FAIL'.

#### Input Format

There will be no more than 10 rows of data.<br>
Each line will be in the format:<br>
[Identifier][Score in English][Score in Math][Score in Science]

#### Output Format

For each row of data, append a space, a colon, followed by another space, and the grade. Observe the format showed in the sample output.

#### Sample Input

```
A 25 27 50
B 35 37 75
C 75 78 80
D 99 88 76
```

#### Sample Output

```
A 25 27 50 : FAIL
B 35 37 75 : FAIL
C 75 78 80 : B
D 99 88 76 : A
```

#### Explanation

A scored an average less than <img src="https://latex.codecogs.com/svg.latex?\Large&space;50{\;}\Rightarrow"> FAIL Same for B C scored an average between <img src="https://latex.codecogs.com/svg.latex?\Large&space;60"> and <img src="https://latex.codecogs.com/svg.latex?\Large&space;80{\;}\Rightarrow"> B 
D scored an average between <img src="https://latex.codecogs.com/svg.latex?\Large&space;80"> and <img src="https://latex.codecogs.com/svg.latex?\Large&space;90{\;}\Rightarrow"> A
