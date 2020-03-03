## 'Sed' command #3

[Judge System](https://www.hackerrank.com/challenges/text-processing-in-linux-the-sed-command-3/problem)

[Sed](https://en.wikipedia.org/wiki/Sed) is a popular utility which enables quick parsing and transformation of text.

Here are some very simple examples of **sed** in action.

Substitute the first occurrence of 'editor' with 'tool'.

```
`$:~/hackerrank/bash/grep/grep1$` echo "My favorite programming editor is Emacs. Another editor I like is Vim." | sed -e s/editor/tool/
My favorite programming tool is Emacs. Another editor I like is Vim.
```

Substitute all the occurrences of 'editor' with 'tool'.

```
`$:~/hackerrank/bash/grep/grep1$` echo "My favorite programming editor is Emacs. Another editor I like is Vim." | sed -e s/editor/tool/g
My favorite programming tool is Emacs. Another tool I like is Vim.  
```

Substitute the second occurrence of 'editor' with 'tool'.

```
`$:~/hackerrank/bash/grep/grep1$` echo "My favorite programming editor is Emacs. Another editor I like is Vim." | sed -e s/editor/tool/2
My favorite programming editor is Emacs. Another tool I like is Vim.
```

Highlight all the occurrences of 'editor' by wrapping them up in brace brackets.

```
`$:~/hackerrank/bash/grep/grep1$` echo "My favorite programming editor is Emacs. Another editor I like is Vim." | sed -e s/editor/{&}/g
My favorite programming {editor} is Emacs. Another {editor} I like is Vim.
```

Some references for learning about **sed** have been included: 

[Sed - An Introduction and a tutorial](https://www.grymoire.com/Unix/Sed.html#uh-10a)<br>
[The TLDP Guide](http://tldp.org/LDP/abs/html/x23170.html)<br>
[Some Practical Examples](https://www.folkstalk.com/2012/01/sed-command-in-unix-examples.html)

#### Task

Given an input file, in each line, highlight all the occurrences of 'thy' by wrapping them up in brace brackets. The search should be case-insensitive.

#### Input Format

A text file will be piped to your command via STDIN.

#### Output Format

Highlight all occurrences of 'thy' as shown in the example below.

#### Sample Input

```
From fairest creatures we desire increase,
That thereby beauty's rose might never die,
But as the riper should by time decease,
His tender heir might bear his memory:
But thou contracted to thine own bright eyes,
Feed'st thy light's flame with self-substantial fuel,
Making a famine where abundance lies,
Thy self thy foe, to thy sweet self too cruel:
Thou that art now the world's fresh ornament,
And only herald to the gaudy spring,
Within thine own bud buriest thy content,
And tender churl mak'st waste in niggarding:
Pity the world, or else this glutton be,
To eat the world's due, by the grave and thee.
When forty winters shall besiege thy brow,
And dig deep trenches in thy beauty's field,
Thy youth's proud livery so gazed on now,
Will be a tattered weed of small worth held:
Then being asked, where all thy beauty lies,
Where all the treasure of thy lusty days;
To say within thine own deep sunken eyes,
Were an all-eating shame, and thriftless praise.
How much more praise deserved thy beauty's use,
If thou couldst answer 'This fair child of mine
Shall sum my count, and make my old excuse'
```

#### Sample Output

```
From fairest creatures we desire increase,
That thereby beauty's rose might never die,
But as the riper should by time decease,
His tender heir might bear his memory:
But thou contracted to thine own bright eyes,
Feed'st {thy} light's flame with self-substantial fuel,
Making a famine where abundance lies,
{Thy} self {thy} foe, to {thy} sweet self too cruel:
Thou that art now the world's fresh ornament,
And only herald to the gaudy spring,
Within thine own bud buriest {thy} content,
And tender churl mak'st waste in niggarding:
Pity the world, or else this glutton be,
To eat the world's due, by the grave and thee.
When forty winters shall besiege {thy} brow,
And dig deep trenches in {thy} beauty's field,
{Thy} youth's proud livery so gazed on now,
Will be a tattered weed of small worth held:
Then being asked, where all {thy} beauty lies,
Where all the treasure of {thy} lusty days;
To say within thine own deep sunken eyes,
Were an all-eating shame, and thriftless praise.
How much more praise deserved {thy} beauty's use,
If thou couldst answer 'This fair child of mine
Shall sum my count, and make my old excuse'
```

#### Explanation

All occurrences of 'thy' have been highlighted by wrapping them up in brace brackets {}. The search and replacement has been done regardless of case.
