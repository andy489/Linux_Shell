grep 's/thy/{&}/gi'

/*
OR
*/

grep 's/[tT][hH][yY]/{&}/g'

/*
OR
*/

sed 's/\(thy\)/{\1}/gi'
