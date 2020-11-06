// header file bridge.h includes all the neccessary standard library and header files 
#include "my_bridge.h"
using namespace std;
struct compare_dest_id{ // compares messages by destination id
    bool operator()(message const &msg1, message const &msg2){
    	if(msg1.dstn < msg2.dstn) return true; 
		//the second object destination id found to be higher and returns true on comparing
		// used when implementing the protocol and at the point when we need to break the ties
    	else 
		return false; //destination id of first is founded farther/higher and thus returns false
    }
};
// typedef is a standard method, and is used to give data type a new name. 
typedef set<message,compare_dest_id> messageSet;
// message comparer simultaneously compares the messages based on the destination id
// send message is a helper function for checking the connection of bridges and lans when all the bridge send their config messages
messageSet Bridge_Config_msg(message m, vector<bridge> b_net, vector<lan> l_net){
	messageSet messages;
	int root = m.msg_root;
	int d = m.msg_dist;
	bridge source = m.source;
	// takes source 
	for(int i=0; i<b_net.size();i++){
		// iterates over the whole bridges network and matches the bridge
		if(source.bridge_id == b_net[i].bridge_id){
			// check if bridge is same as the source id
			for(int j=0; j<b_net[i].conn_lans.size(); j++){
				// loops over the adjacency or connected list of the lans to the bridge
				for(int k=0; k<l_net.size(); k++){
					// now full looping over the lan network to find the desired lan
					if(b_net[i].conn_lans[j] == l_net[k].lan_id){
						for(int p=0; p<l_net[k].conn_bridges.size();p++){
							// the destination bridges are selected by finding the 
							if(l_net[k].conn_bridges[p] != source.bridge_id)
							// bridges in the adjacency list of the lans and choosing those which is not the source itself
							{
								message ms;
								ms.msg_root = root;
								ms.msg_dist = d;
								ms.source = source;
								ms.dstn = l_net[k].conn_bridges[p];
								// the update in the received message list is the destination bridge id along with
								//cout<<"destination "<<ms.destination<<endl;
								ms.lan = l_net[k].lan_id;
								// the lan through which both the bridges are connected
								//cout<<"ms.lan"<<ms.lan<<endl;
								messages.insert(ms);
								// insert function is a method of set datatype to enter a element into the set
							}
						}}}}}}
	return messages;
}

message configuration_update(message m,vector<bridge>& b_net){
	// the call by reference method is used on the bridge network
	// call by reference method of passing arguments to a function copies the address of an argument into the formal parameter
	// in simple words the changes to the bridge network inside the function will be automatically applied globally
	bridge source = m.source;
	int destination = m.dstn;
	int root = m.msg_root;
	int bridge_dist = m.msg_dist;
	char lan = m.lan;

	message update_msg;

	for(int i=0; i<b_net.size();i++)
	{
		if(destination == b_net[i].bridge_id)
		{
			bridge b = b_net[i];
			if(root < b.root_id){
			// rule 1 applied: analysis of the received message 
			// the root in the message is found to be better than the own root id 
			// the first rule of implementing stp is applied
			// bridge acknowledges the other source bridge as the root bridge 
				update_msg.msg_root = root;
				// root update, distance update 
				update_msg.msg_dist = bridge_dist+1;
				update_msg.source=b_net[i];
				b_net[i].root_id = root;
				b_net[i].root_port = make_pair(lan,source.bridge_id);
				b_net[i].root_distance = bridge_dist+1;
				//cout<<"rule 1 "<<endl;
			}
			else if(root == b.root_id && bridge_dist+1 < b_net[i].root_distance)
			{
				//rule 2 applied: It identifies a root with an equal ID
				update_msg.msg_root = root;
				update_msg.msg_dist = bridge_dist+1;
				// now as to break the tie here we make the use of the distance to the root parameter
				update_msg.source=b_net[i];
				b_net[i].root_id = root;
				b_net[i].root_port = make_pair(lan,source.bridge_id);
				// if distance to the root is found to be shorter than own configured value
				// updates its parameter accordingly
				//cout<<"rule 2 "<<endl;
				b_net[i].root_distance = bridge_dist+1;
			}
			else if (root == b.root_id && bridge_dist+1 == b_net[i].root_distance && source.bridge_id<b_net[i].root_port.second)
			{
				//rule 3 applied: The root ID and distance are equal
				// since both are equal for this case the id of sending bridge is used
				//cout<<"rule 3 "<<endl;
				update_msg.msg_root = root;
				update_msg.msg_dist = bridge_dist+1;
				update_msg.source=b_net[i];
				// ties are breaked by analysing the source id of the sending bridge
				// if its smaller than parameter update takes place
				b_net[i].root_id = root;				
				b_net[i].root_port= make_pair(lan,source.bridge_id);
				b_net[i].root_distance = bridge_dist+1;
			}
			else
			{
				// in all other cases bridge will the bridge stops generating configuration messages on its own and 
				update_msg.msg_root = -1;
				//instead only forwards configuration messages from other bridges,
				// also it adds 1 to the distance field as the distance from the sending bridge to the source bridge is also one hop
                //cout<<"rule 4"<<endl;
				update_msg.msg_dist = bridge_dist+1;
				update_msg.source=b_net[i];
			}
		}
	}

	return update_msg;
}


int main(){
	// inputs start here 
	// starts by taking trace as a input
	int trace;
	cin>>trace;
	// the plausible values of trace is either 0 or 1
	int num_bridges;
	cin >> num_bridges;
	// takes the number of bridges which will be there in the network
	vector<bridge> b_net;
	// whole list of bridges are stored as a list/vector of bridges
	// this vector corresponds the practical implementation of a network
	vector<char> inp_lan;
	// vector of inputted lans are used to store only the unique elements and thus 
	// even when same lan characters are passed multiple time only one copy is stored
	// we used if condition to prevent multiple entry
	string line;
	getline(cin,line);
	// getline is a standard library function that is used to read a string or a line from an input stream
	int iter=0;
	while(iter<num_bridges){
	bridge b;
	string line;
	getline(cin,line);
		for(int j=0; j<line.size(); j++){
		if(j==1){
			b.bridge_id = (int) line[j] - 48;
			// bridge assumes itself a root bridge
			b.root_id = b.bridge_id;
		}
		if(j>2){
			if(line[j] != ' ') {
				// only inputs the characters, empty spaces are rejected 
				//conn_lans which is the list of lans connected to the bridges is updated with the lan
				b.conn_lans.push_back(line[j]);
				// the whole set of lans updated with the lan
				// if the same lan is inserted only one instance will be stored
				if(!count(inp_lan.begin(), inp_lan.end(),line[j]))
				inp_lan.push_back(line[j]);
			}
		}
		}
		sort(b.conn_lans.begin(),b.conn_lans.end());
		b_net.push_back(b);
		// push_back is a vector method to enter elements into a vector at last position
		iter++;
	}
    // inputting of all the data done here 
    
	vector<lan> l_net;
	// decalring vector to store all the lans to form a lan network similar to bridge network
	while(!inp_lan.empty())
	{
		char c = *inp_lan.begin();
		// access the first lan element 
		lan l;
		l.lan_id = c;
		// stores lan id as alphabetical characters
		for(int i=0; i<b_net.size();i++)
		// iteration over the all bridge netowork
			for(int j=0; j<b_net[i].conn_lans.size(); j++)
			// for each bridges looks up over its adjacency lan vector and iterates over it
				if (b_net[i].conn_lans[j] == c) 
				// matches whether the selected lan in the inp_lan is the part of bridge adjacency lans or not
				// and if yes
				// pushes the bridge id of the bridge to the adjacency bridges vector of the lan class
				l.conn_bridges.push_back(b_net[i].bridge_id);
		l_net.push_back(l);
		// adds the lan to the l_net
		inp_lan.erase(inp_lan.begin());
		//removes the lan from the set as it has been included in the l_net 
		// and to avoid any repeated addition
		// the initial designated ports are defaultly set to -1
	}
    // list of the sent and received messages by the bridges claiming their configuration time to time
	vector<message> sent_msg,recv_msg;
	// initialising the vector of messages
	// sent_msg abbreviates sent_messages which are advertised by the bridges about the configuration
	// recv_msg abbreviates received configured messages which are sent by the other bridges
	vector<traces> all_traces;
	// stores all the trace objects 
	int timestamp = 0;
	// the timestamp of the activities
	// timestamp is kind of a unit interval multiplier, values changes at step of 1
	// At t=0  default initialisation of the network will be implmented and 
	// all bridges will send their advertisements and then as time progresses will behave according to the protocol.
	int initial=1;
	// initial = 1 implies that whether the sent messsages are the default initialised one
    // practical implementation of initial stage
    // all bridges assumes themselves as root bridges
	iter=0;
	while(iter<b_net.size()){
		message m;
		// the message instance
		// all the bridges configures themselves as the root bridges
		m.msg_root = b_net[iter].bridge_id;
		// initialises its own bridge id as the root id
		m.msg_dist=0;
		// and thus takes the distance of the source bridge as zero
		m.source=b_net[iter];
		// the source is the bridge itself as per the initial protocol strategy
		sent_msg.push_back(m);
		iter++;
	}
	// implementation of the spanning tree protocol starts here 
	// iteration over the whole sent message of bridges about their configuration
	while(!sent_msg.empty()){
		if(initial != 1){
			// since for first step, when the values are initialised 
			// the condition remains false
			// on other steps old config messages are deleted 
			// and new are generated as the bridges receive the configuration message from others
			while(!sent_msg.empty()){
				sent_msg.erase(sent_msg.begin());
			}
		}
		// will be true only when there are update message for the configuration to be updated
		// the received message list which includes the parameters by which the bridges compares its configuration
		// by following the rules of protocol each bridge makes a change/updates its configuration
		// finally a hierarchy is form which is the spanning tree/ without any loops 
		while(!recv_msg.empty()){
			message m = recv_msg.front();
			message to_be_published = configuration_update(m, b_net);
			if(to_be_published.msg_root != -1){
				sent_msg.push_back(to_be_published);
			}
			// traces of the received messages from the otheer bridges
			traces t;
			t.timestamp = timestamp;
			t.bridge = m.dstn;
			t.action = 'r';
			// the action here is r as this time the message is being receieved 
			t.m = m;
			if(trace==1){
			cout<<t.timestamp;
			cout<<" ";
			cout<<"B";
			cout<<" ";
			cout<<t.bridge;
			cout<<" ";
			cout<<t.action;
			cout<<" (B";
			cout<<t.m.msg_root<<" "<<t.m.msg_dist<<" B"<<t.m.source.bridge_id<<")";
			cout<<endl;
			} 
			all_traces.push_back(t);
			recv_msg.erase(recv_msg.begin());
			//cout<<"receive trace added"<<endl;
		}
		vector<message> temp;
		while(!sent_msg.empty()){
			message m = sent_msg.front();
			// front takes out the first message instance from the sent messages vector
			messageSet recv_msg_by_set = Bridge_Config_msg(m, b_net, l_net);
			// sends the first message instance from the sent_msg queue
			sent_msg.erase(sent_msg.begin());
			// pop_back function removes the end messages from the sent config msg list
			while(!recv_msg_by_set.empty()){
				// recv_msg results are being stored in the recv_msg message queue
				recv_msg.push_back(*recv_msg_by_set.begin());
				recv_msg_by_set.erase(recv_msg_by_set.begin());
			}
			// traces are to implemented and plotted only when the value of trace is one 
			// otherwise only the final output will be printed
			traces t;
			// traces definition in the header file
			t.timestamp = timestamp;
			// timestamp shows the time elapsed in the protocol
			t.bridge = m.source.bridge_id;
			// for the trace the bridge activity is stored as the source which generates the msg
			t.action = 's';
			// s denotes the sent config message and r denotes the received config message
			t.m = m;
			all_traces.push_back(t);
			if (trace==1){
		    cout<<t.timestamp<<" B"<<t.bridge<<" "<<t.action<<" (B"<<t.m.msg_root<<" "<<t.m.msg_dist;
			cout<<" B"<<t.m.source.bridge_id<<")";
		    cout<<endl;
		}
			temp.push_back(m);
		}
		while(!temp.empty()){
			sent_msg.push_back(temp.front());
			temp.erase(temp.begin());
		}
		timestamp++;
		initial=0;
	}
	//Protocol Implemented
	// the stp has been formed, and all the bridges are in agreement on which ports are in use for the spanning tree. 
	// Only those required ports may be used for forwarding data packets in the extended LAN.
    iter=0;
	while(iter<l_net.size()){
		// final loop over whole lan network
		int min_val=l_net[iter].conn_bridges[0];
		for(int i=1; i<l_net[iter].conn_bridges.size(); i++){
			if(min_val>l_net[iter].conn_bridges[i]){
				min_val=l_net[iter].conn_bridges[i];
			}
		}
		l_net[iter].dsgnted_port=min_val;
		// for every iteration value the designated port is selected to the lowest id bridge
		iter++;
	}
	// loop to display the final output 
	iter=0;
	while(iter<b_net.size()){
		// prints the bridge on the line for e.g B1, B2..
		cout<<"B"<<b_net[iter].bridge_id<<":";
		// display of output bridges as root/designated or null port
		for(int j=0; j<b_net[iter].conn_lans.size(); j++){
			int flag=0;
			char root_check = b_net[iter].conn_lans[j];
			// this line prints the lan number or character
			cout<<" "<<root_check<<"-";
			// using the root port instance of bridge 
			// the bridge whose root port matches with the root_check variable is the root port
			if(root_check == b_net[iter].root_port.first) {cout<<"RP";flag=1;}
			int iterj=0;
			// for the designated ports denoted as DP
			while(iterj<l_net.size()){
				if(root_check==l_net[iterj].lan_id && b_net[iter].bridge_id==l_net[iterj].dsgnted_port && flag==0)
					{cout<<"DP";
					flag=1;
					break;}
					iterj++;
			}
			// finally the remained ports which are of no use as per the spanning tree protocol are discarded
			// these are called as null ports denoted by NP
			if(!flag) {
				cout<<"NP";
				b_net[iter].conn_lans.erase(remove(b_net[iter].conn_lans.begin(), b_net[iter].conn_lans.end(), root_check), b_net[iter].conn_lans.end());
				j--;
			}
		}
		cout<<endl;
		iter++;
	}
}
