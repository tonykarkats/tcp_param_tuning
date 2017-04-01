#!/bin/bash

FOLDER=$1

mkdir -p $FOLDER/run_pasted
for f in $FOLDER/run_1/*
do
	filename=`basename $f`
	echo $filename
	paste $FOLDER/run_*/$filename > $FOLDER/run_pasted/$filename
done

./produce_avg.py run_pasted
rm -rf run_pasted/
