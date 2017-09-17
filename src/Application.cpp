
class Processor
{
  private:	
	int voltage;
	int frequency;
	int power;
	int cost;
  public:	
	void setvoltage(int v)
	{
		voltage=v;
	}

    void setfequency(int f)
	{
		frequency=f;
	}
    
    void setpower(int p)
	{
		power=p;
	}
    
    void setcost(int c)
	{
		cost=c;
	}


	float getexec()
	{
       return (float)cost/frequency;
    }

    float getenergy()
    {
    	return (float)(power*cost)/frequency;
    }
};
