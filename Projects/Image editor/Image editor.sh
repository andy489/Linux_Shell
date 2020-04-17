#!/bin/bash
# Image editor
# github.com/andy489

function msg {
	echo "${@}" >&2
}

function die {
	msg "${@}"
	exit 1
}

function left_1d {
	in_size="${1}" # inner line size
	out_size="${2}"	# outter line size
	int_x="${3}" # interesting point coordinate x

	mid=$(( in_size / 2 ))
	
	left=$(( int_x - mid )) # left end of inner line
	right=$(( int_x + mid)) # right end of inner line
	
	# fixing left and right ends of inner line
	if [[ "${left}" -lt 0 ]]; then
		echo 0
		return
	fi

	if [[ "${right}" -gt "${out_size}" ]]; then
		echo $(( out_size - in_size ))
		return
	fi

	echo "${left}"
}

function ipoint {
	a="${1}"; b="${2}"; x="${3}"; y="${4}"
       	input_image="${5}"; output_image="${6}"

	# ensure that a > b
	if [ "${a}" -lt "${b}" ]; then
		read a b < <(echo "${b} ${a}")
	fi
	
	# obtain image size
	read size_x size_y < <(identify "${input_image}" \
		| cut -d' ' -f3 | tr x ' ')

	# swap ratio if working on a portrait image
	if [ "${size_x}" -lt "${size_y}" ]; then
		read a b < <(echo "${b} ${a}")
	fi

	# calculate size of crop window
	new_x="${size_x}"
	new_y="$(( (new_x * b) / a ))"

	if [ "${new_y}" -gt "${size_y}" ]; then
		new_y="${size_y}"
		new_x="$(( (new_y * a) / b ))"
	fi

	offset_x=$( left_1d "${new_x}" "${size_x}" "${x}" )
	offset_y=$( left_1d "${new_y}" "${size_y}" "${y}" )

	#echo "size_x: ${size_x}, size_y: ${size_y}, a: ${a}, b: ${b}"
	#echo "new_x: ${new_x}, new_y: ${new_y}"
	#echo "offset_x: ${offset_x}, offset_y: ${offset_y}"

	convert "${input_image}" \
		-crop "${new_x}x${new_y}+${offset_x}+${offset_y}" \
		+repage "${output_image}"
}
case "${1}" in
	ipoint)
		shift
		ipoint "${@}"
		exit $?
		;;
	*)
		die "unknown subcommand: ${1}"
		;;
esac
