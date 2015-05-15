#include <iostream>
#include <utility>
#include <algorithm>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

int main()
{
    //create typedef for Graph type
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS> Graph;
    
    typedef std::pair<int, int> Edge;

    Graph graph(1);
    Edge edge(6,9); 
    boost::add_edge(edge.first, edge.second, graph);
    //std::cout << "first: " << 
}
