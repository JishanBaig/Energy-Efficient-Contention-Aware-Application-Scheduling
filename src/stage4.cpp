using namespace std;
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <functional>
#include <math.h>
#include <fstream>
#include <sstream>
#include <limits>
#include <iterator>
#include "Processor.hpp"
#include "NoC.hpp"
#include "Application.hpp"
#include "transformedApplication.hpp"
#include "Sample.hpp"
#include "stage1.cpp"
#include "stage2.cpp"
#define MAX_VAL 1000


vector<Processor> getProcessorsReady(){
	string filename="Processor.txt";
	cout<<"Reading the configuration file and making processors ready.\n";
	string line;
	vector<Processor> Proc_vector;
	ifstream appfile( filename );
    std::getline(appfile, line);       // cache the line
    int M=atoi(line.c_str());
    for(int i=0 ; i < M ; i++ ){
		Processor p(i , sqrt(M));
		//p.getPos().x<<'\n';
		Proc_vector.push_back(p);
    }
    for(int i = 0; i < M ; i++){
		cout<<"my processor id is : "<< Proc_vector[i].getpid() <<'\n'; 
		cout<<"my x = "<< Proc_vector[i].getPos().x <<" my y = "<< Proc_vector[i].getPos().y <<'\n';
    }
    int i=0;
    char deli=' ';
    string x,y;
    while(std::getline(appfile, line)) {       // cache the line
    std::istringstream tokenizer2(line);
    std::getline(tokenizer2, x, deli);
    std::getline(tokenizer2, y);
    Proc_vector[i].setfrequency(atoi(x.c_str()));
	Proc_vector[i].setpower(atoi(y.c_str()));
    i++;  
    }
    cout<<"Processors are ready.\n";
    return Proc_vector;
}

void printProcessors(vector<Processor> Proc_vector){
    //testApp.printApp();
    cout<<"Here are the Processors' Properties.\n";
    for(int i=0;i<Proc_vector.size();i++){
	    cout<<"my processor id is : "<< Proc_vector[i].getpid() <<'\n';
		cout<<"my energy is = "<< Proc_vector[i].getenergy() << '\n';
		cout<<"my execution time is = "<< Proc_vector[i].getexec() <<'\n';
    }
}    


vector<vector <int> > getmeL(vector<Processor> Proc_vector){
	vector<vector <int>> L (Proc_vector.size(), std::vector<int>(Proc_vector.size()));
	for(int i=0;i<Proc_vector.size();i++){
		for(int j=0;j<Proc_vector.size();j++){
			int y = abs( Proc_vector[i].getPos().x - Proc_vector[j].getPos().x ) + abs( Proc_vector[i].getPos().y - Proc_vector[j].getPos().y );
			L[i][j] = y;
		}
	}
	cout<<"L matrix : M*M \n";
	for(int i=0;i<Proc_vector.size();i++){
		for(int j=0;j<Proc_vector.size();j++){
			cout<< L[i][j] <<" ";
		}
	    cout<<"\n";	
	}
	return L;
}

int compare2 (const void * a, const void * b)
{
  Sample*c= ( Sample* )a;
  Sample*d= ( Sample* )b;
  float e = c->getfitness_val();
  float f = d->getfitness_val();
  return f>e;
}

int main()
{
	//
    //N = 8 tasks and M = 9 processors.
    //X[N*M] ith task allocated to jth processor.
    //L[M*M] keeps he manhattan distance between the ith and jth processors.
    
    vector<Processor> Proc_vector = getProcessorsReady();
    //printProcessors(Proc_vector);
    //
    string testAppfilename="Application.txt";
    Application testApp(testAppfilename);
    //testApp.printApp();
    
    cout<<"Energy Efficient Application Mapping :\n";
    vector<vector <bool>> map = EEAM(Proc_vector,testApp);//stage1
    cout<<"Energy Efficient Application Mapping Ends Here.\n";
    cout<<"\n\n";
    //
    cout<<"Genetic algorithm Setting starts from here.";
    int numSamples,numPros,numComms;
    ifstream sampleFile("inmap.txt");
    string line;
    char deli=' ';
    getline(sampleFile, line);
    istringstream tokenizer1(line); 
    string x,y,z;
    std::getline(tokenizer1, x, deli); // then get the tokens from it
    std::getline(tokenizer1, y, deli); 
    std::getline(tokenizer1, z);
    numSamples = atoi(x.c_str());
    numPros = atoi(y.c_str());
    numComms = atoi(z.c_str());
    cout<<"numSamples : "<<numSamples<<", numPros : "<<numPros<<", numComms : "<<numComms<<'\n';    
    vector<vector <string>> stringSamples (numSamples, std::vector<string>());
    int i=0;
    while(std::getline(sampleFile, line)){
        istringstream iss(line);
        copy(istream_iterator<string>(iss),istream_iterator<string>(),back_inserter(stringSamples[i]));
        i++;
    }
    vector<vector <float>> Samples (numSamples, std::vector<float>(numPros+numComms));
    cout<<"Storing all the samples in Sample vector.\n";
    for(int i = 0; i < stringSamples.size();i++){
        for(int j=0; j < stringSamples[0].size(); j++){
           Samples[i][j]=atof(stringSamples[i][j].c_str());
        }
    }
    cout<<"printing all initial samples.\n";
    for(int i = 0; i < Samples.size();i++){
        for(int j=0; j < Samples[0].size(); j++){
           cout<<Samples[i][j]<<" ";
        }
        cout<<'\n';
    }
    vector<Sample> all_samples;
    for(int i = 0; i < Samples.size();i++){
        Sample s(i,numPros,numComms);
        for(int j=0; j < Samples[0].size(); j++){
           s.values.push_back(Samples[i][j]);
        }
        all_samples.push_back(s);
    }
    int numGenerations = 10;
    Sample winning_sample(MAX_VAL,numPros,numComms);
    vector<float> gen_fitness_values;
    while(numGenerations--){
        //getting energy and time from ETFGBF Algorithm.
        for(int i = 0; i < all_samples.size();i++){
            cout<<"ETFGBF settings starts from here.\n";
            out output = ETFGBF(testApp, map, all_samples[i], Proc_vector);
            cout<<"ETFGBF algorithm ends here.\n";
            all_samples[i].setOuts(output.energy, output.time);
        }
        //Printing fitness value for all the samples.
        for(int i = 0; i < all_samples.size();i++){
            cout<<"my sample id is : "<<all_samples[i].sampleId<<'\n';
            cout<<"my fitness is : "<<all_samples[i].getfitness_val()<<'\n';
        }
        //sorting all_samples according to the fitness values.
        qsort (all_samples.data(),all_samples.size() , sizeof(Sample),compare2);
        //sort (all_samples.begin(), all_samples.end(), compare2);
        //Printing fitness value for all the samples.
        cout<<"After Sorting based on Fitness Values.\n";
        for(int i = 0; i < all_samples.size();i++){
            cout<<"my sample id is : "<<all_samples[i].sampleId<<'\n';
            cout<<"my fitness is : "<<all_samples[i].getfitness_val()<<'\n';
        }
        //selecting top 2 solutions out of it.
        cout<<"Best Fitness Value is : "<<all_samples[0].getfitness_val()<<'\n';
        winning_sample = all_samples[0];
        gen_fitness_values.push_back(all_samples[0].getfitness_val());
        while(all_samples.size()>2){
            int x = all_samples[all_samples.size()-1].sampleId;
            all_samples.erase(std::remove(all_samples.begin(), all_samples.end(), x), all_samples.end());
        }
        //vector<Sample> selected_samples;
        //selected_samples.push_back(all_samples[0]);//0
        //selected_samples.push_back(all_samples[1]);//1
        all_samples.push_back(all_samples[0]);//2
        all_samples.push_back(all_samples[1]);//3
        all_samples.push_back(all_samples[0]);//4
        all_samples.push_back(all_samples[1]);//5
        //setting sampleids for all samples.
        for(int i = 0;i < 6;i++){
            all_samples[i].setId(i);
        }
        /*//Crossover.
        int Crossover_locus = rand()%(numPros+numComms);
        int temp;
        for(int i=Crossover_locus+1;i<all_samples[0].values.size();i++){
            temp = all_samples[2].values[i];
            all_samples[2].values[i] = all_samples[3].values[i];
            all_samples[3].values[i] = temp ;
        }*/
        //Mutation.
        int Mutation_locus = rand()%(numPros+numComms);
        all_samples[4].values[Mutation_locus] = MAX_VAL - all_samples[4].values[Mutation_locus];
        all_samples[5].values[Mutation_locus] = MAX_VAL - all_samples[5].values[Mutation_locus];
    }
    cout<<"All Fitness Values Over the Generations :\n";
    for(int i=0;i<gen_fitness_values.size();i++){
        cout<<gen_fitness_values[i]<<' ';
    }
    cout<<"\n";
    cout<<"Winning Sample Values :\n";
    for(int i=0;i<winning_sample.values.size();i++){
        cout<<winning_sample.values[i]<<' ';
    }
    return 0;
}

   