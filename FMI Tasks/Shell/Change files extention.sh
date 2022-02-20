/*
    Да се напише shell скрипт, който приема точно три аргумента - име на директория, старо файлово разширение и ново файлово
    разширение. Във всички директории с подаденото име, скрипта да поставя на всички файлове с подаденото 
    старо разширение - новото разширение.
    
    Примерно извикване:
    (change file extension)
    ./chfext "dir" "avi" "mp3"
    
    Резултат:
    renamed '/home/students/s61000/dir/film.avi' -> '/home/students/s61000/dir/film.mp3'
    renamed '/home/students/s61000/dir/mov1.avi' -> '/home/students/s61000/dir/mov1.mp3'
    renamed '/home/students/s61000/dir/mov2.avi' -> '/home/students/s61000/dir/mov2.mp3'
    renamed '/home/students/s61000/folder/dir/deeper.avi' -> '/home/students/s61000/folder/dir/deeper.mp3'
*/

#!/bin/bash

[ $# -eq 3 ] || {
	echo "Invalid number of arguments. Usage: ${0} <dirname> <ext1> <ext2>."
	exit 1
}

DIRNAME="${1}"
OLD="${2}"
NEW="${3}"

while read DIRPATH; do
	for FILE in ${DIRPATH}/*.${OLD}; do
		mv -v -- "${FILE}" "${FILE%${OLD}}${NEW}" 2> /dev/null
	done
done < <(find / -type d -name "${DIRNAME}" 2> /dev/null)
