# Find the most interconnected wikipedia pages
### Maz Jindeel - 10/18

# What are the most linked to Wikipedia pages?
We examine the wikipedia archive to see which pages are the most linked to by other wikipedia pages. At a high level, this is done by sticking wikipedia pages into a digraph and seeing which nodes have the most incoming edges. 

# Results

## Most Linked To Pages
||Page|Links
|---|----------------|--------|
|1|[United States](https://en.wikipedia.org/wiki/United_States)|403095|
|2|[Animal](https://en.wikipedia.org/wiki/Animal)|179816|
|3|[France](https://en.wikipedia.org/wiki/France)|149450|
|4|[Arthropod](https://en.wikipedia.org/wiki/Arthropod)|134051|
|5|[India](https://en.wikipedia.org/wiki/India)|131814|
|6|[World War II](https://en.wikipedia.org/wiki/World_War_II)|128344|
|7|[Insect](https://en.wikipedia.org/wiki/Insect)|127139|
|8|[Germany](https://en.wikipedia.org/wiki/Germany)|126493|
|9|[Canada](https://en.wikipedia.org/wiki/Canada)|123462|
|10|[United Kingdom](https://en.wikipedia.org/wiki/United_Kingdom)|116515|


* Find the top 1000 in [results/linkedto_top_thousand.txt](results/linkedto_top_thousand.txt).
* Find the top 1000000 in [results/linkedto_top_million.txt](results/linkedto_top_million.txt).

# Instructions:

* These steps may take a while, as the data is quite large. 
* Ideally you will have around 100GB of hard drive space free. The analysis will
  peak around 130GB of RAM, so the more you can use the better. 

1. Download dataset
    * download the wikipedia dataset in XML
      [here](http://en.wikipedia.org/wiki/Wikipedia:Database_download#English-language_Wikipedia). 
    * Note: you will need at about 12GB free on your disk to complete this step. 

2. Uncompress the data. 
	* `bunzip2 enwiki-20170820-pages-articles.xml.bz2`
	* Note: Unzipped file is 59GB. 

3. run parseScript.sh to extract just titles and links.
    * `./ parseScript.sh enwiki-20180820-pages-articles.xml graph_input.txt`
    * output is ~10GB
	* Links to namespaces are removed, because they are not considered pages

4. Analyze the data
	* This requires a great deal of RAM (over 16gb) or a ton of swap space.
	* Install the dependencies (we only need networkx), so `pip3 install networkx`
	* Run the command `./analysis.py graph_input.txt results/linkedto.txt`
	* See files linkedto.txt and the output of the analysis.

# Future work / TODO
## Performance: 
* This thing is particularly memory hungry - some care could be taken to improve the performance. This project peaked around 130gb of RAM. Thankfully I have access to a 500GB RAM machine - doing this with swap files would be painful!
* There are still some namespaces remaining in the results. I removed the obvious ones, but some remain as a result of typos or simply that the namespace is too small for us to care. 

## [Bacon distance](https://en.wikipedia.org/wiki/Six_Degrees_of_Kevin_Bacon).
Would be nice to have!

