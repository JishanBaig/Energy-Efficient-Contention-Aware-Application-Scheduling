
class node{
  public:
  	int nodeId;
  	bool isTask;
  	int pid;
  	int node_task_Id;
  	int src;
  	int dest;
	int weight;
	vector<int> fromNode;
	vector<int> toNode;
	vector<NoC_link> NoC_links;
	int b_level;
	bool isscheduled;
	bool ready;
	int ready_time;
	int finish_time;
	bool isentry;
	bool isexit; 
	bool operator==(const int& nd ) const { return this->nodeId == nd;} 
};

class transformedApplication{
  private:
  	int N;
  	vector<node> nodeset;
  	vector<int> entrynodeids;
  	vector<int> exitnodeids;
  public:
  	transformedApplication(Application testApp){
  		this->N = testApp.getTaskSet().size() + testApp.getCommSet().size();
  		int nodeId_it = 0;
  		//scanning all the task nodes to extract information.
  		for(int i=0;i<testApp.getTaskSet().size();i++){
  			node nd;
  			nd.isTask = 1;
  			nd.node_task_Id = testApp.getTaskSet()[i].taskId;
  			nd.src=0;
  			nd.dest=0;
  			nd.nodeId = nodeId_it;
  			nodeId_it++;
  			nd.weight = testApp.getTaskSet()[i].cost;
  			nd.isscheduled = 0;
  			nd.ready=0;
  			nd.ready_time=0;
  			nd.finish_time=0;
  			nd.isentry=0;
  			nd.isexit=0;
            this->addNode(nd);
  		}
  		cout<<"number of tasks in transformedApplication : "<<nodeId_it<<'\n';
  		//scanning all the comm nodes to extract information.
  		for(int i=0;i<testApp.getCommSet().size();i++){
  			node ed;
  			ed.isTask = 0;
  			ed.src=testApp.getCommSet()[i].source.taskId;
  			ed.dest=testApp.getCommSet()[i].dest.taskId;
  			ed.nodeId = nodeId_it;
  			nodeId_it++;
  			for(int j=0;j<testApp.getTaskSet().size();j++){
  				if(this->nodeset[j].isTask==1 && this->nodeset[j].node_task_Id == ed.src){
  					this->nodeset[j].toNode.push_back(ed.nodeId);
  					ed.fromNode.push_back(this->nodeset[j].nodeId);
  				}
  				if(this->nodeset[j].isTask==1 && this->nodeset[j].node_task_Id == ed.dest){
  					this->nodeset[j].fromNode.push_back(ed.nodeId);
  					ed.toNode.push_back(this->nodeset[j].nodeId);
  				}
  			}
  			ed.weight = testApp.getCommSet()[i].vol;
  			ed.isscheduled = 0;
  			ed.ready=0;
  			ed.ready_time=0;
  			ed.finish_time=0;
  			ed.isentry=0;
  			ed.isexit=0;
            this->addNode(ed);
  		}
  		for(int i=0;i<this->N;i++){
  			if(this->nodeset[i].fromNode.size()==0){
  				this->nodeset[i].isentry = 1;
  				this->entrynodeids.push_back(this->nodeset[i].nodeId);
  			}
  			if(this->nodeset[i].toNode.size()==0){
  				this->nodeset[i].isexit = 1;
  				this->exitnodeids.push_back(this->nodeset[i].nodeId);
  			}
  		}
  		vector<node> nodeset0 = this->nodeset;
  		for(int i=0;i<this->nodeset.size();i++){
    	    this->nodeset[i].b_level = this->calculating_b_level( nodeset0, i);
        }
    }
  	void addNode(node nd){
  		nodeset.push_back(nd);
  	}
  	vector<node> getNodeSet(){
  		return this->nodeset;
  	}
  	vector<int> getexitnodeids(){
  		return exitnodeids;
  	}
  	vector<int> getentrynodeids(){
  		return entrynodeids;
  	}
  	void assigningProcessors(int i,int j){
  		if(this->nodeset[i].isTask==1)
  			nodeset[i].pid=j;
  	}
  	void assigningNoC_links(int i,NoC_link nc){
  		if(this->nodeset[i].isTask==0)
  			this->nodeset[i].NoC_links.push_back(nc);
  	}
  	void setnodeentry(int i){
  		this->nodeset[i].isentry = 1;
  	}
  	void setscheduled(int i){
  		this->nodeset[i].isscheduled=1;
  	}
  	void setready(int i){
  		this->nodeset[i].ready = 1;
  	}
  	void setfinishtime(int i){
  		this->nodeset[i].finish_time = this->nodeset[i].ready_time + this->nodeset[i].weight;
  	}
  	void setreadytime(int i, int j){
  		this->nodeset[i].ready_time = j;
  	}
  	//bool operator==(const& nd ) const { return this->getNodeSet()  nodeId == nd.nodeId;}
  	int calculating_b_level(vector<node> nodeset, int k){
        if(nodeset[k].isexit == 1)
    	    return nodeset[k].weight;
        else{
    	    int t,max=0;
    	    for(int j=0;j<nodeset[k].toNode.size();j++){
                t = calculating_b_level(nodeset, nodeset[k].toNode[j]);
                if(t>max)
            	    max=t;
    	    }
    	    return max + nodeset[k].weight;
        }
    }
    void makeChild(int m,int n){
    	this->nodeset[m].toNode.push_back(n);
    	this->nodeset[n].fromNode.push_back(m);
    	this->nodeset[n].ready_time = this->nodeset[n].ready_time > this->nodeset[m].finish_time ? this->nodeset[n].ready_time : this->nodeset[m].finish_time; 

    }
    void takecareofContention(int nid){
    	vector<int> match_vector;
    	for(int i=0;i<nodeset.size();i++){
    		if(nodeset[i].isTask==0 && i!=nid){
                for(int j=0;j<nodeset[nid].NoC_links.size();j++){
                	for(int k=0;k<nodeset[i].NoC_links.size();k++){
                		if(nodeset[nid].NoC_links[j].st == nodeset[i].NoC_links[k].st && nodeset[nid].NoC_links[j].end == nodeset[i].NoC_links[k].end  ){
                			match_vector.push_back(i);
                			break;
                		}
                	}

                }

    		}
    		
    	}
    	if(match_vector.size()==0)  return;
    	for(int i;i<match_vector.size();i++){
    		this->makeChild(nid,match_vector[i]);
    	}
    }
  	void printit(){
  		cout<< "Printing completely transformed Application Graph Properties :\n";
  		cout<<" Total Number of Nodes in transformed test Application = "<<this->N<<'\n';
  		cout<<"\n";
  		for(int i=0;i<this->N;i++){
  			cout<<"my node Id is : "<<this->nodeset[i].nodeId<<'\n';
  			cout<<"isTask : "<<this->nodeset[i].isTask<<'\n';
  			if(this->nodeset[i].isTask==1){
  				cout<<"node_task_Id : "<<this->nodeset[i].node_task_Id<<'\n';
  				cout<<"Processor assigned to me is : "<< this->nodeset[i].pid<<'\n';
  			}
  			else{
  				cout<<"src : "<<this->nodeset[i].src<<'\n';
  			    cout<<"dest : "<<this->nodeset[i].dest<<'\n';
  			}
  			cout<<"weight : "<<this->nodeset[i].weight<<'\n';
  			cout<<"my fromNode vector : ";
  			for(int k=0; k < nodeset[i].fromNode.size();k++)
                cout<<nodeset[i].fromNode[k]<<" ";
            cout<<'\n';
            cout<<"my toNode vector : ";
  			for(int k=0; k < nodeset[i].toNode.size();k++)
                cout<<nodeset[i].toNode[k]<<" ";
            cout<<'\n';
            cout<<"my NoC_links vector : ";
            for(int k=0; k < nodeset[i].NoC_links.size();k++)
                cout<<nodeset[i].NoC_links[k].st<<"-"<<nodeset[i].NoC_links[k].end<<" ";
            cout<<'\n';
            cout<<"b_level : "<<this->nodeset[i].b_level<<'\n';
            cout<<"isscheduled : "<<this->nodeset[i].isscheduled<<'\n';
            cout<<"ready : "<<this->nodeset[i].ready<<'\n';
            cout<<"ready_time : "<<this->nodeset[i].ready_time<<'\n';
            cout<<"finish_time : "<<this->nodeset[i].finish_time<<'\n';
  			cout<<"isentry : "<<this->nodeset[i].isentry<<'\n';
  			cout<<"isexit : "<<this->nodeset[i].isexit<<'\n';
  			cout<<"\n";
  		}
  		cout<<"entry node ids : ";
  		for(int k=0; k < this->entrynodeids.size();k++)
                cout<<this->entrynodeids[k]<<" ";
        cout<<'\n';
        cout<<"exit node ids : ";
  		for(int k=0; k < this->exitnodeids.size();k++)
                cout<<this->exitnodeids[k]<<" ";
        cout<<"\n\n";    

  	}

};