#!testenv/bin/python3

#Usage: ./analysis.py WIKIPARSED MOST_IN 
# WIKIPARSED is the parsed wikipedia file generated by the parse script
# LINKEDTO is file where we rank each page in order of most incoming links

import sys


if len(sys.argv) < 3: 
  print("Usage: ./grapher.py WIKIPARSED LINKEDTO")
  exit(1)

#open file
input_file = open(sys.argv[1], 'r')

pages = {}

current_node = ""
#iterate over all lines in the file
while True:
  line = input_file.readline().strip()
  if not line: 
    #add final set of links to the main set
    break

  #if it starts with brackets, it's a link from current_node
  if line[0] == "[":
    line = line[2:-2] #[[ = 2, ]] = 2
    if not line in pages:
      pages[line] = set()
    pages[line].add(current_node)


  #if it starts with <title>, it's a title
  #End the 
  elif line[0] == "<":
    line = line[7:-8] # <title> = 7, </title> = 8
    current_node = line

pages_sorted = sorted(pages.items(), key=lambda kv: len(kv[1]), reverse=True)
#for title, incomings in pages_sorted:
#  print(title, len(incomings))

in_file = open(sys.argv[2], 'w')
for title, incomings in pages_sorted:
  count = len(incomings)
  in_file.write(f'{title} {count}\n')
#in_file.close()




