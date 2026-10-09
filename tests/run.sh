#!/bin/sh
# Run every tests/cases/NAME.txt against the program given as $1.
# The program gets NAME.txt as its only argument, unless the file NAME.args
# exists: then its words are the arguments instead (the .txt file is
# ignored). An empty NAME.args means no arguments. NAME.txt is always
# the standard input of the program, so with no arguments the program
# reads the tree from stdin.
# A valid case (no NAME.args, exit status 0) is run a second time with no
# arguments and the input on stdin, and must give the same result.
# The program runs inside tests/cases, so paths in .args and in error
# messages are relative to that directory.
# Compares stdout with NAME.out, the exit status with NAME.code (default 0)
# and stderr with NAME.err (only if the file exists).

bin=${1:?usage: run.sh PROGRAM}
bin=$(cd "$(dirname "$bin")" && pwd)/$(basename "$bin")
dir=$(dirname "$0")/cases
tmp=$(mktemp -d "${TMPDIR:-/tmp}/btree-test.XXXXXX") || exit 1
trap 'rm -rf "$tmp"' EXIT
cd "$dir" || exit 1

pass=0
fail=0

# run_case LABEL NAME [ARG...]: run the program and check it against the
# expectations stored next to NAME.txt.
run_case() {
    label=$1
    name=$2
    shift 2
    ok=1

    "$bin" "$@" < "$name.txt" > "$tmp/out" 2> "$tmp/err"
    status=$?

    if ! cmp -s "$tmp/out" "$name.out"; then
        echo "  stdout differs"
        ok=0
    fi
    if [ "$status" -ne "$expected_code" ]; then
        echo "  exit status $status, expected $expected_code"
        ok=0
    fi
    if [ -f "$name.err" ] && ! cmp -s "$tmp/err" "$name.err"; then
        echo "  stderr differs"
        ok=0
    fi

    if [ "$ok" -eq 1 ]; then
        echo "PASS $label"
        pass=$((pass + 1))
    else
        echo "FAIL $label"
        fail=$((fail + 1))
    fi
}

for input in *.txt; do
    name=$(basename "$input" .txt)

    expected_code=0
    if [ -f "$name.code" ]; then
        expected_code=$(cat "$name.code")
    fi

    if [ -f "$name.args" ]; then
        set -f
        set -- $(cat "$name.args")
        set +f
        run_case "$name" "$name" "$@"
    else
        run_case "$name" "$name" "$input"
        if [ "$expected_code" -eq 0 ]; then
            run_case "$name (stdin)" "$name"
        fi
    fi
done

echo "$pass passed, $fail failed"
[ "$fail" -eq 0 ]
