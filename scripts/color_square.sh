#!/bin/bash

# bash script to read multiple graph6 strings in on stdin.
# Each graph is squared, converted to DIMACS format, and then fed into exactcolors's color program.
# The output is parsed, and if the upper bound exceeds a limit, the original graph6 string is written to stdout.

if [ "$1" == "" ]
then
  upperboundlimit=7
else
  upperboundlimit="$1"
fi

while read -r line
do
  tmpfile="$(mktemp /tmp/color_g6.XXXXXXXXXX)"
  echo "$line" | squareg | graph6_to_dimacs > "$tmpfile"
  output=$(color -l 120 "$tmpfile" | grep finished)
  LB=$(echo $output | cut -d' ' -f 5)
  UB=$(echo $output | cut -d' ' -f 8)
  if [ "$UB" -ge "$upperboundlimit" ]
  then
    echo $line
  fi
  rm -f "$tmpfile"
done
