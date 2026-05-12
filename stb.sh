#!/usr/bin/env bash
set -euo pipefail

usage() {
	printf "Creates a stb-style header file out of a series of implementation files.\n"
	printf '\tUsage: %s <output> <impl_macro> <header> [<file> | --text <text>]...\n' \
		"$0"
}

if [[ "${1:-}" == "-h" || "${1:-}" == "--help" ]]; then
	usage
	exit 0
fi

if (( $# < 4 )); then
	usage >&2
	exit 1
fi

out=$1
impl_macro=$2
header=$3
shift 3

cat -- "$header" > "$out"
printf '#ifdef %s\n' "$impl_macro" >> "$out"

while (( $# > 0 )); do
	case "$1" in
		--text)
			if (( $# < 2 )); then
				printf '%s: --text requires an argument\n' "$0" >&2
				exit 1
			fi
			printf '%b\n' "$2" >> "$out"
			shift 2
			;;
		*)
			cat -- "$1" >> "$out"
			shift
			;;
	esac
done

printf '#endif /* %s */\n' "$impl_macro" >> "$out"

sed -i "/#include \"$header\"/d" "$out"
