awk '{
average = ($2 + $3 + $4) / 3
if (average >= 80) {
  grade = "A"
} else if (average >= 60) {
  grade = "B"
} else if (average >= 50) {
  grade = "C"
} else {
  grade = "FAIL"
}
print $0, ":", grade
}'

/*
OR
*/

awk '{
    s=($2+$3+$4)/3;
    print $0,":",(s>=80?"A":(s>=60?"B":(s>=50?"C":"FAIL")))
}'
