#!/bin/bash

# ANGEL GARCIA MALAGON

for i in `ls test/`;do
  out="$i.out"
  ./bin/cse test/$i &> output/$out

done
