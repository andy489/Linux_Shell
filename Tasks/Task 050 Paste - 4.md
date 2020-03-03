## Paste - 4

[Judge System](https://www.hackerrank.com/challenges/paste-4/problem)

Given a CSV file where each row contains the name of a city and its state separated by a comma, your task is to restructure the file in such a way, that three consecutive rows are folded into one, and separated by tab.

#### Input Format

You are given a CSV file where each row contains the name of a city and its state separated by a comma.

#### Output Format

Restructure the file in such a way, that every group of three consecutive rows are folded into one, and separated by tab.

#### Sample Input

````
Albany, N.Y.
Albuquerque, N.M.
Anchorage, Alaska
Asheville, N.C.
Atlanta, Ga.
Atlantic City, N.J.
Austin, Texas
Baltimore, Md.
Baton Rouge, La.
Billings, Mont.
Birmingham, Ala.
Bismarck, N.D.
Boise, Idaho
Boston, Mass.
Bridgeport, Conn.

#### Sample Output

````
Albany, N.Y.    Albuquerque, N.M.   Anchorage, Alaska
Asheville, N.C. Atlanta, Ga.    Atlantic City, N.J.
Austin, Texas   Baltimore, Md.  Baton Rouge, La.
Billings, Mont. Birmingham, Ala.    Bismarck, N.D.
Boise, Idaho    Boston, Mass.   Bridgeport, Conn.
````

#### Explanation

The given input file has been reshaped as required.
