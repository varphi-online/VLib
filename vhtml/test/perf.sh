#!/usr/bin/env bash
set -e

sudo sysctl -w kernel.perf_event_paranoid=1

prog="$1"
shift

/usr/bin/perf record -e cycles:u -g --call-graph fp -- "$prog" "$@"
/usr/bin/perf report
