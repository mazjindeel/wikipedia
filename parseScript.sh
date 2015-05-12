grep -Po '<title>(.*)</title>|\[\[.*?\]\]' smallsample.txt > test.txt
#http://en.wikipedia.org/wiki/Wikipedia:Namespace
#if a line starts with a namespace followed by a :, exclude it


#if there's a | between the [[]], and the first part contains a colon, ignore the record
 
#-P for pcre (perl regex) option
#o to output only the result, with each result on a different line (if multiple results)
