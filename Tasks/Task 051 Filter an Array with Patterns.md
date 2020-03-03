## Filter an Array with Patterns

[Judge System](https://www.hackerrank.com/challenges/bash-tutorials-filter-an-array-with-patterns/problem)

#### Objective
We now transition to some basic examples of bash scripting for the purpose of text processing and data munging. In this challenge, we practice reading and filtering an array.

#### Resources
Here's a [great tutorial](https://www.thegeekstuff.com/2010/06/bash-array-tutorial/) with useful examples related to arrays in Bash.

#### Task
You are given a list of countries, each on a new line. Your task is to read them into an array and then filter out (remove) all the names containing the letter 'a' or 'A'.

#### Input Format

The input format consists of a list of country names, each on a separate line. The only characters present in the country names will be upper or lower case characters and hyphens.

#### Output Format

From the given list, remove the names that contain 'a' or 'A' in them. If there are no names left after removing these characters, you should display a blank line.

#### Sample Input

````
Namibia
Nauru
Nepal
Netherlands
NewZealand
Nicaragua
Niger
Nigeria
NorthKorea
Norway
````

#### Sample Output
````
Niger
````

####Explanation

Niger is the only name that does not contain an 'a' or 'A'. 
