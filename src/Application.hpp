
class task{
  public:
  	int taskId;
  	int cost;
  	int total_vol;
};


class comm{
  public:
    int commId;	
    task source;
    task dest;
    int vol;
};

class Application{
  private:
    int N;
    vector<task> task_set;
    int E;
    vector<comm> comm_set;
  public:
    Application(string filename){
        ifstream appfile( filename );
        string line;
        char deli=' ';
        getline(appfile, line);
        istringstream tokenizer1(line); 
        string x,y;
        std::getline(tokenizer1, x, deli); // then get the tokens from it
        std::getline(tokenizer1, y);
        this->N = atoi(x.c_str());
        this->E = atoi(y.c_str());
        //int t_array[x];
    
        getline(appfile, line);
        stringstream ss(line);
        string item;
        int i=0;
    
        while (getline(ss, item, deli)) {
            task t;
            t.taskId = i;
    	    t.cost = atoi(item.c_str());
            this->addTask(t);
            i++;
        }
        string z; 
        i=0;
        while(std::getline(appfile, line)) {       // cache the line
        std::istringstream tokenizer2(line);
        comm co;
        co.commId = i;
        std::getline(tokenizer2, x, deli); // then get the tokens from it
        std::getline(tokenizer2, y, deli);
        std::getline(tokenizer2, z);
        co.source = this->getTaskSet()[atoi(x.c_str())];
        co.dest = this->getTaskSet()[atoi(y.c_str())];
        co.vol = atoi(z.c_str());
        this->addComm(co); 
        i++;
        }  
    }
    void addTask(task t){
    	task_set.push_back(t);
    }
    void setTaskvol(int i,int n){
    	this->task_set[i].total_vol=n;
    }
    void addComm(comm c){
    	comm_set.push_back(c);
    }
    vector<task> getTaskSet(){
    	return this->task_set;
    }
    vector<comm> getCommSet(){
    	return this->comm_set;
    }
    void printApp(){
    	cout<<"Application Information from the application configuration file\n";
    	cout<<"Number of tasks = "<< this->N <<'\n';
    	cout<<"Number of edges = "<< this->E <<'\n';
    	cout<<"TaskSet information :- \n";
    	for(int i=0;i<this->N;i++){
    		cout<<"my task id : "<< this->task_set[i].taskId<<'\n';
    		cout<<"my cost : "<<this->task_set[i].cost<<'\n';
    	}
    	cout<<"CommSet information :- \n";
    	for(int i=0;i<E;i++){
    		cout<<"my comm id : "<< this->comm_set[i].commId<<'\n';
    		cout<<"my source dest pair is : "<<this->comm_set[i].source.taskId<<' '<<comm_set[i].dest.taskId<<'\n';
    		cout<<"my volume is : "<< this->comm_set[i].vol<<'\n';
    	}
    } 

};
