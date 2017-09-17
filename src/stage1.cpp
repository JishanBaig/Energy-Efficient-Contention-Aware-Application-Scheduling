

int compare (const void * a, const void * b)
{
  task*c= ( task* )a;
  task*d= ( task* )b;
  return (  d->total_vol - c->total_vol);
}



vector<vector <bool>> EEAM(vector<Processor> Proc_vector, Application testApp){
	//Finding L matrix;
    cout<<"Finding L matrix.\n";
    vector<vector <int>> L (Proc_vector.size(), std::vector<int>(Proc_vector.size()));
	for(int i=0;i<Proc_vector.size();i++){
		for(int j=0;j<Proc_vector.size();j++){
			int y = abs( Proc_vector[i].getPos().x - Proc_vector[j].getPos().x ) + abs( Proc_vector[i].getPos().y - Proc_vector[j].getPos().y );
			L[i][j] = y;
		}
	}
	cout<<"Printing L matrix : M*M \n";
	for(int i=0;i<Proc_vector.size();i++){
		for(int j=0;j<Proc_vector.size();j++){
			cout<< L[i][j] <<" ";
		}
	    cout<<"\n";	
	}
    //C matrix.
	vector<vector <int>> C (testApp.getTaskSet().size(), std::vector<int>(testApp.getTaskSet().size(),0));
    cout<<"C matrix : N*N \n";
    for(int i=0;i<testApp.getTaskSet().size();i++){
		for(int j=0;j<testApp.getTaskSet().size();j++){
			cout<< C[i][j] <<" ";
		}
	    cout<<"\n";	
	}
	cout<<"updated C matrix : N*N \n";
    for(int i = 0;i<testApp.getCommSet().size();i++){
    	C[testApp.getCommSet()[i].source.taskId][testApp.getCommSet()[i].dest.taskId] = testApp.getCommSet()[i].vol;
    }
    for(int i=0;i<testApp.getTaskSet().size();i++){
		for(int j=0;j<testApp.getTaskSet().size();j++){
			cout<< C[i][j] <<" ";
		}
	    cout<<"\n";	
	}
    //Finding task total Volume vector.
    cout<<"Finding task total Volume vector.\n";
	int sum;
	for(int i=0; i < testApp.getTaskSet().size();i++){
		sum=0;
		for(int j=0; j < testApp.getTaskSet().size();j++){
			sum = sum + C[i][j];
		}	
		for(int j=0; j < testApp.getTaskSet().size();j++){
			sum = sum  + C[j][i];
			
		} 
		testApp.setTaskvol(i,sum);
		

	}
	for(int i=0;i<testApp.getTaskSet().size();i++){
		cout<<"My task id is : "<< testApp.getTaskSet()[i].taskId <<'\n';
		cout<<"My total_vol is : "<< testApp.getTaskSet()[i].total_vol<<'\n';
	}
	cout<<'\n';
	vector<task> sorted_task_set = testApp.getTaskSet();
	//Sorting the task set according to the total volume.
	cout<<"sorted_task_set with sort based on total volume : \n";
	qsort ( &sorted_task_set[0],sorted_task_set.size() , sizeof(task),compare);
    for(int i=0;i<sorted_task_set.size();i++){
		cout<<"My task id is : "<< sorted_task_set[i].taskId <<'\n';
		cout<<"My total_vol is : "<< sorted_task_set[i].total_vol<<'\n';
	}

    //X mapping matrix;
    cout<<"Assumed X matrix.\n";
    vector<vector <float>> X (testApp.getTaskSet().size(), std::vector<float>(Proc_vector.size()));
    X = {{0.12,0.13,0.94,0.44,0.33,0.65,0.76,0.02,0.81}, 
         {0.32,0.65,0.38,0.54,0.64,0.94,0.34,0.43,0.92},
         {0.68,0.99,0.54,0.35,0.44,0.32,0.43,0.77,0.44},
         {0.45,0.32,0.44,0.79,0.69,0.08,0.51,0.12,0.16}, 
         {0.65,0.26,0.29,0.54,0.76,0.14,0.88,0.76,0.96}, 
         {0.65,0.01,0.87,0.23,0.13,0.25,0.96,0.33,0.43},
         {0.22,0.34,0.22,0.67,0.86,0.45,0.56,0.17,0.44},
         {0.21,0.35,0.69,0.67,0.23,0.45,0.56,0.79,0.21}}; 
    cout<<"X matrix : N*M\n";
    for(int i=0;i<testApp.getTaskSet().size();i++){
		for(int j=0;j<Proc_vector.size();j++){
			cout<< X[i][j] <<" ";
		}
	    cout<<"\n";	
	}
    //XT mapping matrix.
    vector<vector <float>> XT (Proc_vector.size(), std::vector<float>(testApp.getTaskSet().size(),0.0));
    cout<<"XT matrix : M*N\n";
    for(int i=0;i<testApp.getTaskSet().size();i++){
		for(int j=0;j<Proc_vector.size();j++){
			XT[j][i]=X[i][j];
		}
	}
	for(int i=0;i<Proc_vector.size();i++){
		for(int j=0;j<testApp.getTaskSet().size();j++){
			cout<< XT[i][j] <<" ";
		}
	    cout<<"\n";	
	}
    //Given X, L' Matrix
    cout<<"LD Matrix.\n";
    vector<vector <float>> mul (testApp.getTaskSet().size(), std::vector<float>(Proc_vector.size(),0));
    for(int i = 0; i < X.size(); ++i){
        for(int j = 0; j < L.size(); ++j){
            for(int k = 0; k < X[0].size(); ++k){
                mul[i][j] += X[i][k] * L[k][j];
            }
        }    
    }
    vector<vector <float>> LD (testApp.getTaskSet().size(), std::vector<float>(testApp.getTaskSet().size(),0));
    for(int i = 0; i < mul.size(); ++i){
        for(int j = 0; j < XT.size(); ++j){
            for(int k = 0; k < mul[0].size(); ++k){
                LD[i][j] += mul[i][k] * XT[k][j];
            }
        }
    }
    //done LD = X*L*XT
    cout<<"Printing LD matrix : N*N \n";
    for(int i=0;i<testApp.getTaskSet().size();i++){
		for(int j=0;j<testApp.getTaskSet().size();j++){
			cout<< LD[i][j] <<" ";
		}
	    cout<<"\n";	
	}

	
	//Getting the optimization function opt = (LD)(C) ready.
	cout<<"Getting the optimization function i.e. total weighted distance of all communications ready.\n";
    vector<vector <float>> opt (testApp.getTaskSet().size(), std::vector<float>(testApp.getTaskSet().size(),0));
    for(int i = 0; i < LD.size(); ++i){
        for(int j = 0; j < C.size(); ++j){
            opt[i][j] += LD[i][j] * C[i][j];
        }    
    }
    cout<<"Printing opt matrix : N*N \n";
    for(int i=0;i<opt.size();i++){
		for(int j=0;j<opt[0].size();j++){
			cout<< opt[i][j] <<" ";
		}
	    cout<<"\n";	
	}
    cout<<"Summing all the values of optimization matrix.\n";
    int opt_val=0;
    for(int i = 0; i < opt.size(); ++i){
        for(int j = 0; j < opt[0].size(); ++j){
            opt_val += opt[i][j];
        }    
    }
    cout<<"Optimization function value = "<<opt_val<<"\n";
    //RIRAM starts from here.
    cout<<"map matrix.\n";
    vector<vector <bool>> map (testApp.getTaskSet().size(), std::vector<bool>(Proc_vector.size()));
    for( int i=0; i < sorted_task_set.size(); i++ ){
        int p = sorted_task_set[i].taskId;
        float max = X[p][0],max_pos=0;
        for(int j=0;j<X[0].size();j++){
        	if(X[p][j]>max){
        		max=X[p][j];
        		//cout<<"max="<<max<<" X[p][j] = "<<X[p][j]<<'\n';
        		max_pos=j;
        		//cout<<"max_pos = "<<max_pos<<'\n';
        	}
        }
        cout<<"task with taskId : "<< p <<" is assigned to processor with pid: "<< max_pos <<'\n';
        for(int k=0;k<X[0].size();k++){
            X[p][k]=0;
        }
        X[p][max_pos]=1;
    }

    for(int i=0;i<testApp.getTaskSet().size();i++){
		for(int j=0;j<Proc_vector.size();j++){
			map[i][j]=X[i][j];
		}
	}
    cout<<"map matrix : N*M\n";
    for(int i=0;i<testApp.getTaskSet().size();i++){
		for(int j=0;j<Proc_vector.size();j++){
			cout<< map[i][j] <<" ";
		}
	    cout<<"\n";	
	}
	return map;
}