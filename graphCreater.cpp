#include <iostream>
#include <utility>
#include <algorithm>
#include <map>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

int main()
{
    typedef boost::adjacency_list<boost::vecS,boost::vecS,boost::bidirectionalS, std::string> Graph;
    typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
    typedef boost::graph_traits<Graph>::edge_descriptor Edge;
    Graph graph;

    std::string str1 = "str1", str2 = "str2", str3 = "str3", str4 = "str4";
    //how to store vertices in a hashmap?
    Vertex v1 = boost::add_vertex(str1, graph);
    Vertex v2 = boost::add_vertex(str2, graph);

    
    //std::unordered_map <std::string, Vertex*  
    Edge edge;
    bool edgeExists;
    //check if edge already exists, don't want duplicate edges
    boost::tie(edge, edgeExists) = boost::edge(v1, v2, graph);
    if(!edgeExists)
        boost::add_edge(v1, v2, graph);

    

    /*
    //create typedef for Graph type
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS, std::string> Graph;
    
    typedef std::pair<int, int> Edge;

    Graph graph();
    Edge edge("6", "9"); 
    boost::add_edge(edge.first, edge.second, graph);
    //std::cout << "first: " << 
    */
}
