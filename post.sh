#!/bin/bash

clear

echo "Processing the output files in CMAT"

cmat -l << quit
comp $1/group1.txt
8192 8192
4

128 128
$1/group1
comp $1/group2.txt
8192 8192
4

128 128
$1/group2
comp $1/group3.txt
8192 8192
4

128 128
$1/group3
comp $1/group4.txt
8192 8192
4

128 128
$1/group4
comp $1/group5.txt
8192 8192
4

128 128
$1/group5
comp $1/group6.txt
8192 8192
4

128 128
$1/group6
comp $1/group7.txt
8192 8192
4

128 128
$1/group7
comp $1/group8.txt
8192 8192
4

128 128
$1/group8
comp $1/group9.txt
8192 8192
4

128 128
$1/group9
comp $1/group10.txt
8192 8192
4

128 128
$1/group10
comp $1/group11.txt
8192 8192
4

128 128
$1/group11
comp $1/group12.txt
8192 8192
4

128 128
$1/group12

exit 
quit


