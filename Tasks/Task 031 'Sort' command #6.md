## 'Sort' command #6

[Judge System](https://www.hackerrank.com/challenges/text-processing-sort-6/problem)

You are given a file of tab separated weather data (TSV). There is no header column in this data file.
The first five columns of this data are: (a) the name of the city (b) the average monthly temperature in Jan (in Fahreneit). (c) the average monthly temperature in April (in Fahreneit). (d) the average monthly temperature in July (in Fahreneit). (e) the average monthly temperature in October (in Fahreneit).

You need to sort this file in ascending order of the second column (i.e. the average monthly temperature in January).

#### Input Format

A text file with multiple lines of tab separated data. The first five fields have been explained above

#### Output Format

Sort the data in ascending order of the average monthly temperature in January.

#### Sample Input

````
Albany, N.Y.    22.2    46.6    71.1    49.3    38.60   136 64.4    57
Albuquerque, N.M.   35.7    55.6    78.5    57.3    9.47    60  11.0    64
Anchorage, Alaska   15.8    36.3    58.4    34.1    16.08   115 70.8    39 / 60
Asheville, N.C. 35.8    54.1    73.0    55.2    47.07   126 15.3    39
Atlanta, Ga.    42.7    61.6    80.0    62.8    50.20   115 2.1 69 / 65
Atlantic City, N.J. 32.1    50.6    75.3    55.1    40.59   113 16.2    60 / 54
Austin, Texas   50.2    68.3    84.2    70.6    33.65   85  0.9 62 / 58
Baltimore, Md.  32.3    53.2    76.5    55.4    41.94   115 21.5    53
Baton Rouge, La.    50.1    66.6    81.7    68.1    63.08   110 0.2 52 / 46
Billings, Mont. 24.0    46.1    72.0    48.1    14.77   96  56.9    69
Birmingham, Ala.    42.6    61.3    80.2    62.9    53.99   117 1.5 60
Bismarck, N.D.  10.2    43.3    70.4    45.2    16.84   96  44.3    64
Boise, Idaho    30.2    50.6    74.7    52.8    12.19   89  20.6    64
Boston, Mass.   29.3    48.3    73.9    54.1    42.53   127 42.8    52 / 66
Bridgeport, Conn.   29.9    48.9    74.0    54.7    44.15   119 26.2    55 / 49

#### Sample Output

````
Bismarck, N.D.  10.2    43.3    70.4    45.2    16.84   96  44.3    64
Anchorage, Alaska   15.8    36.3    58.4    34.1    16.08   115 70.8    39 / 60
Albany, N.Y.    22.2    46.6    71.1    49.3    38.60   136 64.4    57
Billings, Mont. 24.0    46.1    72.0    48.1    14.77   96  56.9    69
Boston, Mass.   29.3    48.3    73.9    54.1    42.53   127 42.8    52 / 66
Bridgeport, Conn.   29.9    48.9    74.0    54.7    44.15   119 26.2    55 / 49
Boise, Idaho    30.2    50.6    74.7    52.8    12.19   89  20.6    64
Atlantic City, N.J. 32.1    50.6    75.3    55.1    40.59   113 16.2    60 / 54
Baltimore, Md.  32.3    53.2    76.5    55.4    41.94   115 21.5    53
Albuquerque, N.M.   35.7    55.6    78.5    57.3    9.47    60  11.0    64
Asheville, N.C. 35.8    54.1    73.0    55.2    47.07   126 15.3    39
Birmingham, Ala.    42.6    61.3    80.2    62.9    53.99   117 1.5 60
Atlanta, Ga.    42.7    61.6    80.0    62.8    50.20   115 2.1 69 / 65
Baton Rouge, La.    50.1    66.6    81.7    68.1    63.08   110 0.2 52 / 46
Austin, Texas   50.2    68.3    84.2    70.6    33.65   85  0.9 62 / 58
````

#### Explanation

The data has been sorted in ascending order of the average monthly temperature in January (i.e, the second column). 
