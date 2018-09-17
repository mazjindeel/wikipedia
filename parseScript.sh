#!/bin/bash

#Author: Mazin Jindeel
#Date: 5/15
#Search for titles and links in wikipedia dataset and output to file
#arguments: first is path of uncompressed wikipedia dataset, second is output file
#Usage - ./parseScript.sh wikipedia.xml output.txt

if [ -z $1 ] || [ -z $2 ] 
then
	echo "Error - Missing Arguments"
	echo "Usage: ./parseScript.sh wikipedia.xml output.txt"
	exit 1
fi

#uncompressed wikipedia path
wikipedia_uncompressed="$1"

#output file path
output="$2"
#search for titles <title>.*</title> or links [[link]] in articles, output matches
#then remove all the namespaces (may need to revisit this list to remove even more of the namespaces)
#then remove all the renamings ([a|b] is just a link to a
#then output to the output file

grep -Po '<title>(.*)</title>|\[\[.*?\]\]' $wikipedia_uncompressed | grep -Ev '\[\[.*(34|User|WP|Project|File|Image|MediaWiki|Template|Help|Category|Portal|Book|Draft|Education|Program|TimedText|Module|Topic|User|talk|Talk|WT|Special|Media|Project|Talk|Image|talk|File|talk|Wikipedia|Talk|m|mw|wikt|CAT|H|MOS|P|T|MP|Wikipedia|Template|Module|toollabs|bugzilla|tools|template|wikipedia):.*\]\]' | sed -E 's/\|[^]]*//g' > $output



