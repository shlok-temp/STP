#include<iostream>
#include<bits/stdc++.h>
#include<math.h>
#include<string.h>
#include<vector>
#include<string>
#include<set>
#include<algorithm>
#include<queue>
#include<utility>
//
using namespace std;
// all class instances are public as the program on running will change the corresponding values related to the class instance
// public is an access specifier to make members of the Class accessible from outside the class, other allowed values are private and protected
// declaration of a network objects using classes which store the corresponding required elements
// lan instance
class lan{
	public:
	char lan_id; // own id of the lan
	int dsgnted_port;    // designated port of the lan, the port that has the lowest Path Cost on a particular Local Area Network 
	vector<int> conn_bridges; // the vector instance maintaining the list/vector of adjacent bridges in for of bridge ids 
	vector<int> conn_hosts; // this vector stores the list/vector of the adjacent/connected hosts with the specified lan
	// required for learning bridges 
	lan()
	{// default initialisation of a lan with dummy id and a designated port
	// the initiaslisation of values helps to put some conditions to realise whether the things are initialised to still uninitialised.
		lan_id = '\0';
		dsgnted_port = -1;
	}
};
// lan class implementation over
// forwarding table instance used in learning bridges simulation
class for_tab{//Used to store create an entry of the Forwarding table. Stores host id and Forwarding LAN id
		public:
		int host_id; // storing the host id from which the sending message have been sourced
		lan fport; // from the class lan making a instance of the forwarding port or lan
		// general structure in a forwarding table is host_id and the corresponding port on which the message have been forwarded to
};
// the bridge instance in the bridge network 
class bridge{  
	public: 
	int bridge_id;  // the id of the bridge
	int root_id;   // the root bridge id
	int root_distance; // the variable to store the distance from the root
	vector<char> conn_lans;   //the connected list of lans with the bridge
	vector<for_tab> forwarding_table;  // the vector for generating forwaridng table, each is a type of ftable class 
	pair<char,int> root_port; //the root port, in the form of pair of lan(which is inputted as a character) and the port connected to root port with minimum distance
	bridge()  // default declaration of the bridge class
	{
		// bridge_id is set to -1 unless the input is taken and value is assigned
		bridge_id = -1;
		root_id = bridge_id;// assumes itself a root only 
		root_port = make_pair('\0',-1); //Constructs a pair object with its first element set to x and its second element set to y.
		// root distance is taken as -1 only for initalising values 
		root_distance = -1;
	}
};
// message instance 
class message{ // the class for declaring variables for Storing the root id, the current distance from the root, the bridge object of the source bridge, the id of the destination bridge and the lan via which the message is sent. There is also a structure for comparing messages by destination ids
	public:
	int msg_root; // the root id 
	int msg_dist;  //the current distance from the root
	bridge source; // the object bridge of the source bridge
	int dstn; //the id of the destination bridge
	char lan; // the lan via which the message will be sent
	message() // the default initialising
	{  
		msg_root = -1;
		msg_dist = -1;
		dstn = -1;
		lan = '\0';
	}
};
// traces are conditional
// only when the value of trace is set to 1 they will be shown
// otherwise the final output will be displayed
class traces{ // this class is used to store the trace objects(Timestamp, bridge activity, action and message)
	public:
	message m; 
	int timestamp;  // specifies how many unit time intervals have been elapsed
	int bridge; // the bridge id 
	char action; // whether the message has been sent on the port by the bridge or received by the bridge
};
