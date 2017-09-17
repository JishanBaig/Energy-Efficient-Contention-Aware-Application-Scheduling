
class Sample{
  private:
  	
  	int numPros;
  	int numComms;
  	float energy;
  	float time;
  	float fitness_val;
  public:
  	vector<float> values;
  	int sampleId;
    Sample(int x, int y,int z ){
    	this->sampleId=x;
    	this->numPros=y;
    	this->numComms=z;
    }	
    void setOuts(float en, float tm){
    	this->energy = en;
    	this->time = tm;
    	this->fitness_val = 1000000.0/this->energy;
    }
    float getfitness_val(){
        return this->fitness_val;
    }
    bool operator==(const int& nd ) const { return this->sampleId == nd;}
    void setId(int x){
    	this->sampleId = x;
    }
};