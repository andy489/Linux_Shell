#!/bin/bash
# Image editor: crop [ipoint median_faces rect_faces] 
# github.com/andy489

set -euo pipefail

function msg {
    echo "${@}" >&2
}

function die {
    msg "${@}"
    exit 1
}

function left_1d {
    in_size="${1}"
    out_size="${2}"
    int_x="${3}"

    mid=$(( in_size / 2 ))

    if [[ $(( int_x - mid )) -le 0 ]]; then
        echo 0
        return
    fi

    if [[ $(( int_x + mid )) -ge "${out_size}" ]]; then
        echo $(( out_size - in_size ))
        return
    fi

    echo $(( int_x - mid ))
}

function image_size {
    identify "${1}" | cut -d ' ' -f 3 | tr x ' '
}

function ipoint {
    a="${1}"; b="${2}"; x="${3}"; y="${4}"; in_file="${5}"; out_file="${6}"

    # ensure that a > b
    if [[ "${a}" -lt "${b}" ]]; then
        read a b < <(echo "${b} ${a}")
    fi

    # obtain image size
    read size_x size_y < <(image_size "${in_file}")

    # swap ratio if working on a portrait image
    if [[ "${size_x}" -lt "${size_y}" ]]; then
        read a b < <(echo "${b} ${a}")
    fi

    # calculate size of crop window
    new_x="${size_x}"
    new_y="$(( (size_x * b) / a ))"
    if [[ ${new_y} -gt ${size_y} ]]; then
        new_x="$(( (size_y * a) / b ))"
        new_y="${size_y}"
    fi

    offset_x=$(left_1d "${new_x}" "${size_x}" "${x}")
    offset_y=$(left_1d "${new_y}" "${size_y}" "${y}")

    convert "${in_file}" \
        -crop "${new_x}x${new_y}+${offset_x}+${offset_y}" \
        +repage "${out_file}"
}

function median_faces {
    # obtain image size
    read size_x size_y < <(image_size "${1}")

    facedetect -c "${1}" | awk -v sx=${size_x} -v sy=${size_y} '
    BEGIN {
        count = 0; x_sum = 0; y_sum = 0;
    }

    { count += 1; x_sum += $1; y_sum += $2; }

    END {
        if (count == 0) {
            print int(sx / 2 + 0.5), int(sy / 2 + 0.5);
        } else {
            print int(x_sum / count + 0.5), int(y_sum / count + 0.5)
        }
    }
    '
}

function rect_faces {
    # obtain image size
    read size_x size_y < <(image_size "${1}")

    facedetect -c "${1}" | awk -v sx=${size_x} -v sy=${size_y} '
    BEGIN {
        left = 2^15; right = -1; bot = -1; top = 2^15;
    }

    {
        left  = left  < $1 ? left  : $1;
 	right = right > $1 ? right : $1;
	bot   = bot   > $2 ? bot   : $2;
	top   = top   < $2 ? top   : $2;
    }

    END {
        if ( NR == 0) {
            print int(sx / 2 + 0.5), int(sy / 2 + 0.5);
        } else {
            print int((right + left) / 2 + 0.5), int((bot + top) / 2 + 0.5)
        }
    }
    '
}

function median_faces_crop {
    a="${1}"; b="${2}"; in_file="${3}"; out_file="${4}"

    read point_x point_y < <(median_faces "${in_file}")

    ipoint "${a}" "${b}" "${point_x}" "${point_y}" "${in_file}" "${out_file}"
}

function rect_faces_crop {
    a="${1}"; b="${2}"; in_file="${3}"; out_file="${4}"

    read point_x point_y < <(rect_faces "${in_file}")

    ipoint "${a}" "${b}" "${point_x}" "${point_y}" "${in_file}" "${out_file}"
}

function get_centre {
    read size_x size_y < <(image_size "${1}")
    echo "$(( size_x / 2 )) $(( size_y / 2 ))"
}

function centre_crop {
    a="${1}"; b="${2}"; in_file="${3}"; out_file="${4}"

    read point_x point_y < <(get_centre "${in_file}")

    ipoint "${a}" "${b}" "${point_x}" "${point_y}" "${in_file}" "${out_file}"
}


case "${1}" in
    ipoint)
        shift
        ipoint "${@}"
        exit $?
        ;;
    median_faces)
        shift
        median_faces_crop "${@}"
        exit $?
        ;;
    rect_faces)
        shift
        rect_faces_crop "${@}"
        exit $?
        ;;
    centre)
        shift
        centre_crop "${@}"
        exit $?
        ;;
    *)
        die "unknown subcommand: ${1}"
        ;;
esac
