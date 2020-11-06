#include<iostream>
#include<bits/stdc++.h>
#include<math.h>
#include<string.h>
#include<vector>
#include<string>
#include<set>
#include<queue>
#include<utility>
#include<algorithm>
using namespace std;


// public is an access specifier to make members of the Class accessible from outside the class, other allowed values are private and protected
class lan{      //Stores its own id, adjacency list of lans (storing the ids), adjacency list of hosts(again ids) and the Designated port.
 //each adjacency list is a collection of unordered lists used to represent a finite graph and describes the set of neighbors of a vertex in the graph.
	public:
		char id; // id of the bridge
		int DP;    // designated port of the lan
		vector<int> adj_bridges; // the vector instance maintaining the list/vector of adjacent bridges in for of bridge ids 
		vector<int> hosts; // similar to the list of adjacent lans this vector stores the list/vector of the adjacent hosts
		// required for learning bridges 
		lan()
		{
			id = '\0';
			DP = -1;
		}
};


class ftable //Used to store create an entry of the Forwarding table. Stores host id and Forwarding LAN id
{
	public:
		int host; // storing the host id
		lan fport; // from the class lan making a instance of the forwarding port or lan
};


class bridge{  //Stores its own id, the root bridge's id, distance from the root, the forwarding table (as a vector of ftable objects), adjacency list of lans (storing the ids), and the root port (stored in the form of pair of lan and the bridge connected to the lan with minimum root distance).
	public:
		int id;  // the id of the bridge
		int root;   // the root bridge id
		int root_distance; // the variable to store the distance from the root
		vector<char> adj_lans;   //the adjacency list of lans
		vector<ftable> forwarding_table;  // the vector for generating forwaridng table, each is a type of 
		pair<char,int> RP; //the root port, in the form of pair of lan and the port connected to root port with minimum distance
		bridge()  //
		{
			id = -1;
			root = id;
			RP = make_pair('\0',-1);
			root_distance = -1;
		}
};


class message{ // the class for declaring variables for Storing the root id, the current distance from the root, the bridge object of the source bridge, the id of the destination bridge and the lan via which the message is sent. There is also a structure for comparing messages by destination ids
	public:
		int root; // the root id 
		int dist;  //the current distance from the root
		bridge source; // the object bridge of the source bridge
		int destination; //the id of the destination bridge
		char lan; // the lan via which the message will be sent
		message() // the default initialising
		{
			root = -1;
			dist = -1;
			destination = -1;
			lan = '\0';
		}
};

class traces{ // this class is used to store the trace objects(Timestamp, bridge activity, status and message)
	public:
		int time;
		int bridge;
		char status;
		message m;
};//
class data_packet{ //this Stores the host id of the source and destination, the lan via which the packet is sent and the destination bridge id.
public:
	int source;// the source host id
	int destination;// the destination host id
	char prev;// the lan instance via which the packet is being sent
	int bridge;// the id of the destination bridge
};
