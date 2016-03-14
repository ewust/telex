#!/bin/bash

for f in `ls *.gnuplot`;
do
    cat $f | awk -F'\t' '{print $5}' | sort -n | ../../eval/cdf.py > $f.cdf
    gnuplot -e "set term png; set output '$f.png'; \
                set xrange[0:1500]; set yrange[0:1]; plot './$f.cdf' with lines;"
done

