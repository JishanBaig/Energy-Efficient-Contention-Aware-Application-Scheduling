class out{
  public:
    float energy;
    float time;
};

vector<NoC_link> calculating_NoC_links(node nd, vector<node> ns){
	vector<NoC_link> nls;
	// x-direction movement.
	if(ns[nd.src].pid %3==0){
		int i = ns[nd.src].pid ,j;
	    while( (i-ns[nd.dest].pid) < -2 ){
            j=i+3;
            NoC_link nl;
            nl.st = i;
            nl.end = j;
            nls.push_back(nl);
            i=j;
            if(i==ns[nd.dest].pid)
               return nls;
        }
        while( (i-ns[nd.dest].pid) > 0 ){
            j=i-3;
            NoC_link nl;
            nl.st = i;
            nl.end = j;
            nls.push_back(nl);
            i=j;
            if(i==ns[nd.dest].pid)
               return nls;
        }
        //y-direction movement.
        while(i+1 <= ns[nd.dest].pid){
            j=i+1;
            NoC_link nl;
            nl.st = i;
            nl.end = j;
            nls.push_back(nl);
            i=j;
            if(i==ns[nd.dest].pid)
       	        return nls;
	    }

	}
	else if(ns[nd.src].pid %3==2){
		int i = ns[nd.src].pid ,j;
	    while( (i-ns[nd.dest].pid) < 0 ){
            j=i+3;
            NoC_link nl;
            nl.st = i;
            nl.end = j;
            nls.push_back(nl);
            i=j;
            if(i==ns[nd.dest].pid)
               return nls;
        
	    }
	    //y-direction movement.
        while(i-1 >= ns[nd.dest].pid){
            j=i-1;
            NoC_link nl;
            nl.st = i;
            nl.end = j;
            nls.push_back(nl);
            i=j;
            if(i==ns[nd.dest].pid)
       	        return nls;
	    }
    }//end of else if.
	else{ //(ns[nd.src].pid %3==1)
		int i = ns[nd.src].pid ,j;
	    while( (i-ns[nd.dest].pid) < -1 ){
            j=i+3;
            NoC_link nl;
            nl.st = i;
            nl.end = j;
            nls.push_back(nl);
            i=j;
            if(i==ns[nd.dest].pid)
               return nls;
        }
        while( (i-ns[nd.dest].pid) > 1 ){
            j=i-3;
            NoC_link nl;
            nl.st = i;
            nl.end = j;
            nls.push_back(nl);
            i=j;
            if(i==ns[nd.dest].pid)
               return nls;
        }
        //y-direction movement.
        while(i+1 <= ns[nd.dest].pid){
            j=i+1;
            NoC_link nl;
            nl.st = i;
            nl.end = j;
            nls.push_back(nl);
            i=j;
            if(i==ns[nd.dest].pid)
       	        return nls;
	    }

	}//else part ends.
	return nls;
}


out ETFGBF(Application testApp,vector<vector <bool>> map, Sample sample, vector<Processor> Proc_vector){
	transformedApplication  transformed_testApp(testApp);
    cout<<" assigningProcessors starts here.\n";
    for(int i=0;i<testApp.getTaskSet().size();i++){
    	int m = 0;
    	for(int j=0;j<map[0].size();j++){
    		if(map[i][j]==1)
    			m=j;
    	}
        transformed_testApp.assigningProcessors(i,m);
    }
    cout<<"assigningNoC_links starts here.\n";
    for(int i=testApp.getTaskSet().size(); i < transformed_testApp.getNodeSet().size();i++){
    	vector<NoC_link> nc_vec = calculating_NoC_links(transformed_testApp.getNodeSet()[i],transformed_testApp.getNodeSet());
    	for(int j=0; j < nc_vec.size(); j++){
    		transformed_testApp.assigningNoC_links(i,nc_vec[j]);
    	}
    }
    //transformed_testApp.printit();
    //calculating links to travell for communication node.
    //cout<<"Calculating NoC links to travell for communication node.\n";
    //vector<NoC_link> calculatin_NoC_links(node nd);
    cout<<"ETFGBF settings starts from here.\n";
    cout<<"Already calculated  execution time and b-level of each unified node.\n";
    cout<<"Initialized the ready times of entry nodes.\n";
    cout<<"Labeling entry nodes as ready.\n";
    for(int i=0;i<transformed_testApp.getentrynodeids().size();i++){
    	transformed_testApp.setnodeentry(transformed_testApp.getentrynodeids()[i]);
    }
    int Unscheduled = transformed_testApp.getNodeSet().size();
    vector<node> readyset;
    cout<<"Putting all entry nodes in readyset\n";
    for(int i=0;i<transformed_testApp.getentrynodeids().size();i++){
    	readyset.push_back(transformed_testApp.getNodeSet()[transformed_testApp.getentrynodeids()[i]]);
    }
    cout<<"Initially readyset size : "<<readyset.size()<<'\n';
    
    while(Unscheduled!=0){
    	int ert = std::numeric_limits<int>::max();
    	node nd=readyset[0];
    	cout<<"readyset size : "<<readyset.size()<<'\n';
    	for(int i=0;i < readyset.size();i++){
    		cout<<readyset[i].nodeId<<' ';
    	}
    	cout<<'\n';
        for(int i=0;i<readyset.size();i++){
        	if(ert > readyset[i].ready_time)
        		ert = readyset[i].ready_time;
        }
        vector<node> ert_vector;
        for(int i=0;i<readyset.size();i++){
        	if(ert == readyset[i].ready_time)
        		ert_vector.push_back(readyset[i]);
        }
        node choosen_node;
        if(ert_vector.size()==1)
        	choosen_node=ert_vector[0];
        else{
        	int hbl = 0;
    	    node nd = ert_vector[0];
            for(int i=0;i<ert_vector.size();i++){
        	    if(hbl < ert_vector[i].b_level)
        		    hbl = ert_vector[i].b_level;
            }
            vector<node> hbl_vector;
            for(int i=0;i<ert_vector.size();i++){
        	    if(hbl = ert_vector[i].b_level)
        		    hbl_vector.push_back(ert_vector[i]);
            }
            if(hbl_vector.size()==1)
            	choosen_node = hbl_vector[0];
            else
            	choosen_node = hbl_vector[0];//randomly choosen.
        }//else ends.step 6 is done.
        cout<<"the choosen node for sceduling is : "<< choosen_node.nodeId<<'\n';
        
        transformed_testApp.setscheduled(choosen_node.nodeId);
        transformed_testApp.setfinishtime(choosen_node.nodeId);

        for(int i=0;i<choosen_node.toNode.size();i++){
            int k = choosen_node.toNode[i], flag=0;
            for(int j=0;j<transformed_testApp.getNodeSet()[k].fromNode.size();j++){
            	int l = transformed_testApp.getNodeSet()[k].fromNode[j];
            	if(transformed_testApp.getNodeSet()[l].isscheduled == 0)
            		flag=1;
            }
            if(flag==0 && transformed_testApp.getNodeSet()[k].isscheduled==0){//means all parents of k are scheduled.
            	cout<<"setting ready for node Id : "<<k<<'\n';
                transformed_testApp.setready(k);
                readyset.push_back(transformed_testApp.getNodeSet()[k]);
                int max_finish_time=0;
                for(int j=0;j<transformed_testApp.getNodeSet()[k].fromNode.size();j++){
            	    int l = transformed_testApp.getNodeSet()[k].fromNode[j];
            	    if(max_finish_time < transformed_testApp.getNodeSet()[l].finish_time)
            		    max_finish_time= transformed_testApp.getNodeSet()[l].finish_time;

                }
                cout<<"max_finish_time : "<<max_finish_time<<'\n'; 
                transformed_testApp.setreadytime(k, max_finish_time);

            } 
        }//step 7 done.

        if(choosen_node.isTask==0)
            transformed_testApp.takecareofContention(choosen_node.nodeId);//step 8 ends here.
        Unscheduled=Unscheduled-1;
        //erasing from ready set.
        int itoe=0;
        for(int i=0;i<readyset.size();i++){
        	if(readyset[i].nodeId==choosen_node.nodeId){
        		itoe=i;
        		//cout<<"fdhdhdhhfhdddddddddddddfdddddd : "<<itoe<<'\n';
        		break;
        	}
        }
        //cout<<"itoe : "<<itoe<<'\n';
        cout<<"readyset size before erase: "<<readyset.size()<<'\n';
        //node  &p=&choosen_node;
        //auto pred = [](node& v) { return v.nodeId == choosen_node.nodeId; };
        //readyset.erase( remove_if(readyset.begin(), readyset.end(), my_predicate), readyset.end() );
        //readyset.erase( std::remove_if( readyset.cbegin(), readyset.cend(),pred ),readyset.cend());
        //auto it =itoe;
        //readyset.erase(readyset.cbegin() + it);
        //const node ndc=choosen_node;
        int x= choosen_node.nodeId;
        readyset.erase(std::remove(readyset.begin(), readyset.end(), x), readyset.end());
        cout<<"readyset size after erase: "<<readyset.size()<<'\n';
    	for(int i=0;i < readyset.size();i++){
    		cout<<readyset[i].nodeId<<' ';
    	}
        cout<<'\n';	
         cout<<'\n';	
    }//end of while loop.
    

    int max_finish_time_all=0;
    for(int i=0;i<transformed_testApp.getNodeSet().size();i++){
    	if(max_finish_time_all < transformed_testApp.getNodeSet()[i].finish_time)
    		max_finish_time_all = transformed_testApp.getNodeSet()[i].finish_time;
    }
    cout<<"Maximal Finish Time of all the nodes : "<<max_finish_time_all<<'\n';
    cout<<'\n';
    cout<<"Printing Earliest Time First Greatest b-level First scheduling output for transformed_testApp : "<<"\n";
    //transformed_testApp.printit();
    //Algorithm 2 ends here.
    cout<<"calculating energy for the provided sample\n";
    cout<<"Processors energy\n";
    for(int i=0;i<transformed_testApp.getNodeSet().size();i++){
        if(transformed_testApp.getNodeSet()[i].isTask == 1){
            int id = transformed_testApp.getNodeSet()[i].pid;
            Proc_vector[id].setvoltage(sample.values[id]);
            Proc_vector[id].setcost(transformed_testApp.getNodeSet()[i].weight);
        }
    }
    float Pros_energy=0;
    for(int i=0;i<Proc_vector.size();i++){
        Pros_energy = Pros_energy + Proc_vector[i].getenergy();
    }
    cout<<"Total Processors energy = "<<Pros_energy<<'\n';
    //
    cout<<"communication Energy.\n";
    float er = 0.005 , el = 0.005;
    float Comm_energy=0;
    for(int i=0;i<transformed_testApp.getNodeSet().size();i++){
        if(transformed_testApp.getNodeSet()[i].isTask == 0){
            Comm_energy = Comm_energy + (er + el * sample.values[i])*transformed_testApp.getNodeSet()[i].weight* transformed_testApp.getNodeSet()[i].NoC_links.size() + transformed_testApp.getNodeSet()[i].weight * er ;
        }
    }
    cout<<"Total communication energy = "<<Comm_energy<<'\n';
    float total_energy = Pros_energy + Comm_energy;
    out output;
    output.energy = total_energy;
    output.time = max_finish_time_all; 
    return output;
}