#!/bin/sh
set -eu

MATRIX_BIN="${1:-}"

if [ -z "$MATRIX_BIN" ]; then
  echo "ERROR: Matrix binary path not provided" >&2
  exit 1
fi

if [ ! -x "$MATRIX_BIN" ]; then
  echo "ERROR: Matrix binary is not executable or not found: $MATRIX_BIN" >&2
  exit 1
fi

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
DATA_DIR="$SCRIPT_DIR/data"
ANSW_DIR="$SCRIPT_DIR/answ"

found_any=false

for in_file in "$DATA_DIR"/*.txt; do
  [ -e "$in_file" ] || continue
  found_any=true

  name=$(basename "$in_file")
  exp_file="$ANSW_DIR/$name"

  if [ ! -f "$exp_file" ]; then
    echo "ERROR: expected answer file not found: $exp_file" >&2
    exit 1
  fi

  if ! "$MATRIX_BIN" < "$in_file" | diff -u "$exp_file" - >/dev/null; then
    echo "test $in_file failed" >&2
    exit 1
  fi
done

if [ "$found_any" = false ]; then
  echo "ERROR: no input files found in $DATA_DIR" >&2
  exit 1
fi

echo "all tests passed successfully"
