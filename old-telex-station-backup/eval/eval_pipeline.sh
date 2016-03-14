#!/bin/bash

python ../eval/evaluate_it.py untagged single
newdir="single_results_`date +'%b%d.%H:%M'`.brocketship_untagged "
mkdir $newdir
mv *.out *.gnuplot $newdir

python ../eval/evaluate_it.py tagged single
newdir="single_results_`date +'%b%d.%H:%M'`.brocketship_tagged "
mkdir $newdir
mv *.out *.gnuplot $newdir

python ../eval/evaluate_it.py untagged dual
newdir="dual_results_`date +'%b%d.%H:%M'`.brocketship_untagged "
mkdir $newdir
mv *.out *.gnuplot $newdir

python ../eval/evaluate_it.py tagged dual
newdir="dual_results_`date +'%b%d.%H:%M'`.brocketship_tagged "
mkdir $newdir
mv *.out *.gnuplot $newdir

python ../eval/evaluate_it.py tagged dualflip
newdir="dualflip_results_`date +'%b%d.%H:%M'`.brocketship_tagged "
mkdir $newdir
mv *.out *.gnuplot $newdir
