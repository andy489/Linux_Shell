#!/bin/bash
# Image editor: crop [ipoint faces faces_s]
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

	# echo "size_x: ${size_x}, size_y: ${size_y}, a: ${a}, b: ${b}"
	# echo "new_x: ${new_x}, new_y: ${new_y}"
	# echo "offset_x: ${offset_x}, offset_y: ${offset_y}"

	convert "${input_image}" \
		-crop "${new_x}x${new_y}+${offset_x}+${offset_y}" \
		+repage "${output_image}"
}

function faces {
	a="${1}"; b="${2}" 
	input_image="${3}"; output_image="${4}"
	
	# obtaing image size
	read size_x size_y < <(identify "${input_image}" \
		| cut -d' ' -f3 | tr x ' ')

	median_x=$(( size_x / 2 ))
	median_y=$(( size_y / 2 ))

	# echo "median_x: ${median_x}, median_y: ${median_y}"
	
	faces_cnt=$(facedetect -c "${input_image}" | wc -l)
	
	if [ "${faces_cnt}" -ne 0 ]; then
	
		# calculating median of all faces and format it to integer coordinates
		read median_x median_y < <(facedetect -c "${input_image}" \
			| awk '{coord_x+=$1; coord_y+=$2} END{printf "%.0f %d\n",coord_x/NR, coord_y/NR }')
	fi

	# echo "updated median_x: ${median_x}, updated median_y: ${median_y}"

	ipoint "${a}" "${b}" "${median_x}" "${median_y}" "${input_image}" "${output_image}"
}

function faces_s {
	# here instead of the median, we take the coordinates of the center
	# of the smallest square that covers the set of ALL faces in a pic

	a="${1}"; b="${2}"
	input_image="${3}"; output_image="${4}"

	# obtain image size
	read size_x size_y < <(identify "${input_image}" \
		| cut -d' ' -f3 | tr x ' ')

	sq_x=$(( size_x / 2 ))
	sq_y=$(( size_y / 2 ))
	
	# obtain the list of all faces in order to call the heavy "facedetect" function only once
	faces=$(facedetect -c "${input_image}")

	if [ $(echo "${faces}" | wc -l) -ne 0 ]; then

		# calculating center of wanted square
		read smallest_x latgest_x smallest_y largest_y \
		< <(echo "${faces}" \
		| awk 'BEGIN{s_x=2^15; l_x=-1; s_y=2^15; l_y=-1}
		  {if ( s_x > $1 ) { s_x = $1 }
	       	   if ( l_x < $1 ) { l_x = $1 }
		   if ( s_y > $2 ) { s_y = $2 }
		   if ( l_y < $2 ) { l_y = $2 }}
	       	      END{print s_x, l_x, s_y, l_y}')

		# echo -n "s_x: ${smallest_x}, l_x: ${largest_x}, "
		# echo	  "s_y: ${smallest_y}, l_y: ${largest_y}"

		sq_x=$(( smallest_x + (largest_x - smallest_x) / 2 ))
		sq_y=$(( smallest_y + (largest_y - smallest_y) / 2 ))
	fi
	
	# echo "updated center_sq_x: ${sq_x}, updated center_sq_y: ${sq_y}"
	
	ipoint "${a}" "${b}" "${sq_x}" "${sq_y}" "${input_image}" "${output_image}"
}

case "${1}" in
	ipoint)
		shift
		ipoint "${@}"
		exit $?
		;;
	faces)
		shift
		faces "${@}"
		exit $?
		;;
	faces_s)
		shift 
		faces_s "${@}"
		exit #?
		;;
	*)
		die "unknown subcommand: ${1}"
		;;
esac
