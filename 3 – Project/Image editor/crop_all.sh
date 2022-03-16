#!/bin/bash
# github.com/andy489

tempdir="$(mktemp -d)"

function cleanup {
	if [ -d "${tempdir}" ]; then
		rm -dR "${tempdir}"
	fi
}

trap cleanup EXIT

cdir="$(dirname "${0}")"

set -euo pipefail

function msg {
    echo "${@}" >&2
}

function die {
    msg "${@}"
    exit 1
}

function crop {
    "${cdir}"/crop.sh "${@}"
}

[ $# -eq 4 ] || die "Invalid number of arguments! Usage: $0 <a> <b> <in_dir> <out_dir>"

function validate_numbers {
    grep -q '[^0-9]' <(echo -e "${1}\n${2}")
}

a="${1}"; b="${2}"; in_dir="${3}"; out_dir="${4}"

validate_numbers "${a}" "${b}" && die "Invalid proportions! Usage: $0 <a> <b> <in_dir> <out_dir>"

[ -d "${in_dir}" ]  || die "Invalid input directory! Usage: $0 <a> <b> <in_dir> <out_dir>"
[ -d "${out_dir}" ] || die "Invalid output directory! Usage: $0 <a> <b> <in_dir> <out_dir>"
[ -r "${in_dir}" ]  || die "Not readable input directory! Usage: $0 <a> <b> <in_dir> <out_dir>"
[ -w "${out_dir}" ] || die "Not writable input directory! Usage: $0 <a> <b> <in_dir> <out_dir>"

function process_image {
    img="${1}"
    base="$(basename "${img}")"

    img_centre="${tempdir}/centre_${base}"
    img_median_faces="${tempdir}/median_faces_${base}"
    img_rect_faces="${tempdir}/rect_faces_${base}"
	img_user_coords="${tempdir}/user_coords_${base}"
	
    crop centre "${a}" "${b}" "${img}" "${img_centre}"
    crop median_faces "${a}" "${b}" "${img}" "${img_median_faces}"
    crop rect_faces "${a}" "${b}" "${img}" "${img_rect_faces}"
	
	function image_size {
    	identify "${1}" | cut -d ' ' -f 3 | tr x ' '
	}

	read size_x size_y < <(image_size "${img}")

    msg "preview: $(pw "${tempdir}"/*)"
    msg "choose:"
    msg "  (1) centre"
    msg "  (2) median_faces"
    msg "  (3) rect_faces"
    msg "  (4) enter coordinates manually"
    read choice

	while validate_numbers "${choice}" "1" || test "${choice}" -gt 4 || test "${choice}" -eq 0
    do
        read -p "choose: " choice
    done

    case "${choice}" in
        1) result="${img_centre}" ;;
        2) result="${img_median_faces}" ;;
        3) result="${img_rect_faces}" ;;
		4) 
			while true; do
					read -p "Enter interesting point coordinates in interval [0;${size_x}], [0;${size_y}]: " ix iy

					if ! [[ ( "${ix}" -ge 0 ) && ( "${ix}" -le "${size_x}" ) && \
							( "${iy}" -ge 0 ) && ( "${iy}" -le "${size_y}" ) ]]; then
						
						msg "coordinates are out of bounds"
						continue
					fi

					crop ipoint "${a}" "${b}" "${ix}" "${iy}" "${img}" "${img_user_coords}"
					msg "preview for coordinates ${ix}, ${iy}: $(pw "${img_user_coords}")"
					msg "is this ok?:"
					msg "  (y) yes"
					msg "  (n) no"
					read choice

					while [[ ( "${choice}" != y ) && ( "${choice}" != n ) ]]; do
						read -p "choose: " choice
					done

					case "${choice}" in
						y) result="${img_user_coords}"; break ;;
						n) continue ;;
					esac
			done
		;;
    esac

    cp -v "${result}" "${out_dir}"/"${base}"
    rm -rf "${tempdir}"/*
}

mkdir -p "${out_dir}"

function img_file_check {
	grep "JPEG image" <(file $"{1}")
}

for img in "${in_dir}"/*; do
	fgrep -q "JPEG image data" <(file "${img}") && {
  		msg "processing image ${img}";
   		process_image "${img}";
	}
done
