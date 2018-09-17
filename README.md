#Find the most interconnected wikipedia pages
###Author: Mazin Jindeel
###Date: 10/18

#What are the most linked to Wikipedia pages?
We examine the wikipedia archive to see which pages are the most linked to by other wikipedia pages. We also generate a list of pages with the most outgoi


#Results

##Most Linked To Pages
1. United States, 403383
2. Animal, 179831
3. France, 149506
4. Arthropod, 134052
5. India, 131925
6. World War II, 128570
7. Insect, 127142
8. Germany, 126540
9. Canada, 123521
10. United Kingdom, 116583

* Find the top 1000 in [results/linkedto_top_thousand](results/linkedto_top_thousand).
* Find the top 1000000 in [results/linkedto_top_million](results/linkedto_top_million).

#Instructions:

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

#Future work / TODO
## Performance: 
* This thing is particularly memory hungry - some care could be taken to improve the performance. This project peaked around 130gb of RAM. Thankfully I have access to a 500GB RAM machine - doing this with swap files would be painful!
* There are still some namespaces remaining in the results. I removed the obvious ones, but some remain as a result of typos or simply that the namespace is too small for us to care. 

## [Bacon distance](https://en.wikipedia.org/wiki/Six_Degrees_of_Kevin_Bacon).
Would be nice to have!

