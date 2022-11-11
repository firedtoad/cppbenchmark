#!/usr/bin/env bash
BENCHES=$(find ./bin -type f -executable )
CPU=$(grep -m 1 'model name' /proc/cpuinfo | awk '{print $6"_"$7}')
CORES=$(($(grep -c ^processor /proc/cpuinfo)))
RESULTS_DIR=results/$CPU
mkdir -p "$RESULTS_DIR"
COMMANDS=""
CWD=$(pwd)
for bench in $BENCHES; do
    RESULT_FILE=$RESULTS_DIR/$(basename "$bench")
    # shellcheck disable=SC2116
    COMMANDS+="cd $CWD && $bench > $RESULT_FILE.txt 2>&1 \n"
done
echo -e "$COMMANDS" |xargs -I CMD  --max-procs="$CORES" bash -c CMD

