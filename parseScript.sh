#!/bin/bash

#Author: Mazin Jindeel
#Date: 5/15
#Functionality: Search for titles and links in wikipedia dataset and output to file
#arguments: first is path of uncompressed wikipedia dataset, second is output file

#input file path
input="$1"
#output file path
output="$2"
#search for titles or links in articles
grep -Po '<title>(.*)</title>|\[\[.*?\]\]' $input > $output
#-P for pcre (perl regex) option
#o to output only the result, with each result on a different line (if multiple results)
