
class position
{
  public:
	int x;
	int y;
};

class Processor
{
  private:
    int pid; 	
	int voltage;
	int frequency;
	int power;
	int cost;
	position pos; 
  public:
    Processor(int i, int n){
    	pid = i;
    	pos.x = floor(i / n);
    	pos.y = i - (pos.x * n);
    }
    int getpid(){
    	return this->pid;
    } 
    position getPos(){
    	return this->pos;
    }	
    void setvoltage(int v=0){
		this->voltage=v;
	}
    void setfrequency(int f){
		this->frequency=f;
	}
    void setpower(int p){
		this->power=p;
	}
    void setcost(int c=0){
		this->cost=c;
	}
    float getexec(){
        return (float)cost/frequency;  
    }
    float getenergy(){
        return (float)(0.05 * voltage * voltage * cost);         //power = capacitance(= 0.05) * (voltage)^2*frequency
    }
};


//class Processor_set
//{
//public:
//	int num_of_processors=5;
//	position Processor_ids[num_of_processors];
//power	=capacitance* (voltage)^2*frequency
//};