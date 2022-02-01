#!/bin/bash

# bash script to read multiple graph6 strings in on stdin.
# Each graph is squared, converted to DIMACS format, and then fed into exactcolors's stable program.
# The output is parsed, and if the optimal clique number is less than an upper bound, the original graph6 string is written to stdout.

if [ "$1" == "" ]
then
  upperboundlimit=7
else
  upperboundlimit="$1"
fi

n=0
while read -r line
do
  tmpfile="$(mktemp /tmp/color_g6.XXXXXXXXXX)"
  echo "$line" | squareg | graph6_to_dimacs > "$tmpfile"
  cliquesize=$(stable "$tmpfile" | grep Optimal | cut -d' ' -f 4)
  if [ "$cliquesize" -lt "$upperboundlimit" ]
  then
    echo $line
  fi
  rm -f "$tmpfile"
  
  n=$(( n+1 ))
  if [ "$(( n % 1000 ))" -eq 0 ]
  then
     echo "Testing n=$n" 1>&2
  fi 
done
