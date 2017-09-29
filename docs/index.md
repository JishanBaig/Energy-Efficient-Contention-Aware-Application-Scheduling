
## Introduction

Now a days due to the smaller size of the processors, one MPSoC contains several number of processors. Previously on chip buses were used to communicate between them but now Network on chip is used to make proper communication between them. 
Energy consumption and execution time both are very critical issues regarding the application scheduling. Till now most of the work done in this area was more focused towards processors energy consumption during the execution of the application tasks. Less amount of work has been done in communication energy dissipation due task communication dependencies. Communication is done through the NoC links on an on chip processor network. This plays significant amount of role in total energy dissipation during application DAG execution. Modern processors are actually improved with dynamic voltage and frequency scaling technique (DVFS). In this technique, processors’ voltage or frequency level can be changed dynamically so that it can be set up according to its utilization. Using this technique energy dissipation during task execution can be saved effectively. But communication energy dissipation can not be ignored because of its significant value. So including processors and NoC communication link energy dissipation , we aim to minimize total energy consumption during complete application DAG execution. With this problem we need to solve the problem of contention over both processors and communication links. Contention implies to a situation where no two tasks can be assigned to the same processor at the same time and no two communications can have a common NoC link between them. Contention is a very serious problem and it can lead to wrong execution of the application graph. That’s why we also need to take care of contention situation. To solve this problem, we use application mapping and later application scheduling problem with optimal voltage and frequency assignment. Following figure shows general NoC architecture. As you can see, it’s a two dimensional 3X3 mesh network. Every processor has its own router to communicate with nearby other processors through NoC links which are bidirectional links.


Fig.1 - A 3X3 NoC Architecture

Assumptions:
1.	All Processors are homogeneous type and consists of DVFS technique (dynamic voltage and Frequency Scaling ).
2.	The frequency used by NoC link may differ to each other.
3.	The application graph will be basically a directed acyclic graph(DAG).
Issues to be solved:
1.	All the Tasks will be efficiently mapped to the processors on MPSoCs.
2.	Finding optimal voltage values for the processors so that corresponding tasks can be scheduled without  much energy consumption.
3.	Finding optimal frequency values for the communications or their corresponding NoC links that are to be included in the complete route for that communication.
4.	Scheduling the application graph such that every task and every communication will be in their dependency order given by the application DAG.
5.	There should not be any kind of contention present while finding the scheduling order of application DAG.
6.	With the consideration of all of the above issues, our aim is to minimize the total energy consumption and the total finish time/scheduling time of the application graph.

Example:
Due to the mapping, contention among the processors or communication links may increase, which may lead to underutilization of available resources and ultimately it will lead to increase in the total finish time of the application graph.Here is the example to show the effect of mapping on the total energy consumption and total finish time of the application. As shown in 2(b) and 2(c) two mappings are applied for the application. 
For mapping 1, energy consumption for the links is as follows : 
10 + 20 + (15*2) = 60
 For mapping 2, energy consumption for the links is as follows : 
20 + 15 + (10*2) = 55


Fig.2 - (a) shows the application graph having tasks A,B and C with weights 10, 20 and 10 respectively,having communication links A to B with weight 10, A to C with weight 15 and B to C with weight 20. (b) & (c) shows two different kind of mapping of tasks to processors. (d) & (e) shows scheduling applied on the above mentioned mappings.

Thus as you can see that energy consumption for mapping 2 is less than the energy consumption for mapping 1. Fig. 2(d) and 2(e) shows the scheduling applied to the possible mappings on the application graph. Scheduling 1 is shown for mapping 1 and scheduling 2 is shown for mapping 2. In mapping 1 task B can not be executed concurrently with AB or AC communication links. But in mapping 2, task B and communication AC can be done concurrently. So mapping 2 utilizes the resources more efficiently and saves the time using concurrent execution. Thus mapping plays an important role in saving energy and minimizing time. 

Related Work and Our Contributions
Various types of genetic algorithms have been applied in the past research work to get the optimal values for the processor parameters. Various application mapping techniques have been provided in [1]. For many past algorithms, the consideration for optimization was mainly the overall application finish time. Energy consumption was not considered in such algorithms. Energy aware task allocation problem is addressed by [2]. The problem regarding contention is addressed by [3] and it also provides solution to minimize the weighted distance.
How our work differs?
1.	We consider both application mapping and application scheduling problems with Contention awareness and time and energy minimization.
2.	We propose genetic algorithm and evolutionary programming techniques to find the optimal voltage and optimal frequency values.
3.	We provide comparison between genetic algorithm and evolutionary programming over the course of  finding optimal voltage frequency values for all the processors and communication links.
We solve all the above issues in the following steps. To solve the application mapping problem which is modeled as Quadratic Integer Programming (QBP) problem, we will apply Relaxation based Iterative rounding (RIRAM) algorithm. To solve application scheduling problem with contention awareness, we apply modified version of earliest time first scheduling algorithm known as earliest time first greatest b-level first scheduling (ETFGBF) algorithm. Then to find optimal voltage and frequency levels, we apply genetic algorithm and evolutionary programming algorithm over a number of generations on initially chosen samples. 



System Model and Problem definition 
Processor Model
There are M processors on the MPSoC system as shown in fig.1 which has M = 9.Each processor has its own processor id. As we know that all the processor are of DVFS type. Every processor also has voltage and frequency attribute. The execution time and the energy consumption can be calculated using voltage and frequency of the processor and the cost of the task that is allocated to the processor.  
Time = cost / frequency
And its energy consumption = (power * cost) / frequency
Every processor has all the above attributes. It also consists its position i.e. coordinates in two dimensional NoC having mesh architecture.
NoC Link Model  
It is the communication link between two nearby processors.it contains processor ids of its source and destination processors. Task to task communication becomes processor to processor communication after task to processor mapping. This processor to processor communication link consists of one or more than one NoC links. Due to the xy routing there are only some fix NoC links are to be followed for a processor to processor communication. In xy routing distance in x direction is covered first then distance in y direction is covered. That’s why it fixes the NoC links to be followed.
Application Model
Application contains a set of tasks and set of communication links between them. The application can be considered as directed acyclic graph G(T,E), where T represents task set and E represents Comm set of application graph. Task set contains |T| number of tasks and Comm set contains |E| number of communication links. Each task in the task set has its task id , total cost (number of CPU cycles needed by task to execute completely) and total volume (it’s the summation of the total communication volume coming into it and the total volume going out of it). Each comm in the comm set has its comm id,  source task, destination task and communication volume. Precedence order must be followed such that a task will execute after all of its predecessor tasks are over and all the communication to that task is completed.
Transformed Application Model
Transformed application G*(T+E,E*) is constructed by applying transformation on the original application graph. In transformed application every node and communication link becomes an unified node. Every node has its node id and istask boolean variable ( to know that it’s a task or not ). Every node contains it’s fromnode ids and tonode ids to find out it’s parent s and children. If node is of communication type node then it will also have list of all the NoC links it needs to trace for the source task to destination communication. Every node also has b-level value. The  b-level value is the time period between the start of its execution and end of the exit nodes execution. The b-level values plays an important role in ETFGBF scheduling algorithm. Every node also contains its ready time and finish time information which will be used in ETFGBF scheduling algorithm.


Sample Model
Samples are required to give input to the ETFGBF scheduling algorithm in order to find the optimal voltage and frequency values for the processors and communication links between them. It basically contains voltage values for processors and frequency values for the communication links. Using these values ETFGBF algorithm provides minimized time and minimized energy consumption values , those are used to calculate fitness value for the given sample. This fitness values decides the selection of the given sample in the corresponding generation. If the fitness value is more, there are more chances of getting it selected for going into next generation or becoming parent for creating next generation.
Problem Definition
Given a MPSoC system having network on chip mesh architecture among the processors of similar kind, all the processors can be operated on the sample voltage level set we provide in the sample. All the processors are DVFS technique compatible. Application is given as an application DAG which contains all the task and communication link information. Our aim is to do task to processor mapping and task - communication scheduling with minimum energy consumption and minimum total finish time with contention awareness. 
Energy Efficient Application Mapping 
Application mapping implies to the mapping of the application tasks to the MPSoC processors such that the energy consumption of all the communication links can be minimized that will eventually lead to minimization of overall energy consumption. The total energy consumption for communications including router and link energy consumption can be formulated as follows:    
Here some constants are required from the architecture itself e.g. per bit energy consumption for router and links. The total weight / volume of data to be transferred and the manhattan distance (in terms of NoC links travelled) is also required to calculate weighted distance. Thus for all the communication links ( in this case |E| number of communication edges) , energy consumption is calculated. Our aim to minimize this total energy consumption. Constants are architecture and hardware dependent. Frequency is provided by the input sample. The weighted distance can be minimized by finding appropriate mapping to the processors so that frequently used communication distance will be as minimum as possible. All the processors reside on chip arranged in mesh based architecture. It is a two dimensional architecture. Very processor has its two dimensional x-y coordinates. Using their xy coordinates (i, j), the manhattan distance between all pair of processors can be calculated according to the following formula. 
L(MXM) matrix is used to store all manhattan distance values for all M processors. X(NXM) matrix contains initial probability mappings of N tasks to M processors. LD(NXN) matrix contains distance between all task pairs given a mapping X.

In mapping, every task is allocated a processor. Thus task to task communication becomes processor to processor communication . if ith task communicates with jth task then then X[i][j] = 1 and all other X[i]s and X[j]s = 0. We use a communication matrix C(NXN) which contains the total volume of the communication between ith task and jth task or else it contains 0. Thus if we do cell by cell multiplication of LD and C matrix, we get the weighted communication matrix containing weighted communication between all NXN task pairs. Our aim is find a mappings which can minimize the overall sum of matrix opt = LD*C.



This is a kind of binary quadratic programming problem.it comes under NP Hard class of problems. To solve this problem we use relaxation based iterative rounding algorithm. In this algorithm we try to relax the constraint mentioned in equation 12, which can only take binary values. But we consider as it can take any real value between 0 to 1. Thus we used initial X matrix containing mapping probabilities.

We allocate tasks to processors according to these mappings in iterative fashion.for this purpose we calculate priorities for all the tasks. we calculate total volume for every task in the application graph. total volume is defined as the sum of total incoming volume into the task and total outgoing volume from the task.

Thus total volume is sorted in nonincreasing order and high priority will be given to the task with highest total volume.thus in every iteration processor is assigned to a task and that task is then removed for consideration in next iterations. all the mappings are stored in map(NXM) matrix.

As shown in above equation i1 is selected in first iteration thus for the next iteration it is removed from the optimization problem. Iteratively every task will be selected for the mapping and at the end of the algorithm,  every row in the mp matrix will have exactly one value as set. The index of that set value represents the processor id on which the task is allocated for the execution.


![](.\Picture3.png)

Fig.3 - Flowchart for RIRAM algorithm

Energy Efficient Contention Aware Application Scheduling
after mapping is done, we need to schedule the tasks and decide the order of task and communication execution order.order is decide in such a way that there should not be any contention among tasks and communication links.the order must follow the dependency order provided by application DAG. the finish time of last exit node will be the total finish time of complete application.the original application graph contains tasks and communication nodes. to implement our modified ETF scheduling algorithm the original application graph is changed into a transformed application graph that contains unified nodes. every task and communication is changed as unified node in transformed graph.the cost of task node becomes weight of corresponding unified node and the volume of communication node becomes weight of its corresponding unified node.the deadline of the application should also be taken care of by scheduling algorithm.thus the total finish time of the application must be less than or equal to the 
deadline of the application. 

The total energy consumption also will be calculated by the scheduling algorithm. sample values will be given to the ETFGBF algorithm that contains voltage values for the processors and frequency values for the communication links. thus both energy consumption and total finish time of the application will be calculated by the scheduling algorithm.the modified earliest time first algorithm is used for this purpose. earliest time first scheduling policy takes scheduling decision on the basis of earliest ready time among all the ready tasks. it does not consider b- level values of the application nodes.but in our algorithm, we first use earliest ready time (task which has least ready time value), then greatest b-level value to select the unified node to schedule. It means one extra parameter is considered for the scheduling decision.
The b-level is defined as the time period between execution start time of node and finish time of the exit node. the maximum value is taken as b-level value of the unified node. The b-level values for application nodes are calculated using a recursive algorithm. It starts from the exit nodes. For the exit nodes its execution time becomes its b- level value. A unified node may have many paths to different exit nodes. The maximum of all of them is considered as the b-level value for the unified node.

Fig. - 4  (a) original application DAG with task nodes with their costs and with communication links with their volume requirements.(b) shows the result of the mapping done by relaxation based iterative rounding algorithm.every task is allocated to corresponding processor.(c) shows the transformed application DAG that contains unified nodes converted from task nodes and communication links.(d) shows the results obtained from ETFGBF scheduling algorithm. It shows ready time and finish time of all the nodes present in the transformed application DAG.
The problem of contention is also resolved by this algorithm. It checks for the contention situation between two communication nodes.for this, it checks for the common NoC links present between the scheduled communication node and all other communication node. If there is any noc link which is common then the other communication node is added as a child to the scheduled communication node. Because it’s basically causing dependency. Because of this they can not be executed concurrently. They have to schedule one after another. For the path selection xy routing is used which prefers x over y. Thus every communication link has fixed Noc links to travell with its communication volume. Figure 5 shows an example of the complete execution of ETFGBF scheduling algorithm. All tasks and communications ready and finish times are calculated. As we can see communication link  from 1 to 4 have contention with the communication link 1 to 2. That’s why they can not execute concurrently and 1 to 4 is scheduled after 1 to 2 communication. 


![](.\Picture5.png)
Fig.5 - Setting up for ETFGBF algorithm

The total completion time for the application execution in the example is 216. ETFGBF algorithm also calculates total energy consumption by the application (including processor’s and communication) according to the voltage and frequency assignment. Fig. 5 and 6 shows the complete flow chart for the ETFGBF algorithm.  


![](.\Picture6.png)

Fig. 6 - ETFGBF Scheduling algorithm

Genetic Algorithm for Voltage and Frequency Assignment 
Genetic algorithms are used to find the best solution in the solution search space. A fitness value is calculated in every generation for every sample. The samples having the best fitness values are chosen for the next generation and also chosen to become parent to produce new samples for next generation. Our aim is to find optimal value of voltage and frequency to assign to the processors and communication links respectively. For this purpose we take many sample values and for all of them we execute ETFGBF scheduling algorithm. A sample contains voltage levels to assign to processors and then frequency values to assign to communication links. Thus a sample in our case looks like the following.
 
The sample size is the sum of total number of task nodes and total number of communication links i.e. |T|+|E|. First |T| values are for voltages and next |E| values are for frequencies. We find application completion time and energy consumption with the given sample’s frequency and voltage assignment. Then the fitness value can be calculated as a function of obtained energy and completion time. Fitness value implies that how good the sample values are. Thus using genetic algorithm, the sample with the better fitness value can be obtained over a number of generations. Only samples with the better fitness value will go to the next generation. The fitness value of a sample s can be calculated by using the following formula.

If the application finish time is less than or equal to deadline then fitness value depends only on energy consumption. It will be inversely proportional to to the energy consumption. But if the application finish time is greater than the deadline then its fitness value should be very less which decreases the chances of getting it selected for the next generation. That’s why fitness value is inversely proportional to the squared application finish time. Thus using the above formula only the best samples will get more chances of getting selected. In an iteration top two samples with best fitness values are chosen. Suppose the following two samples are selected.
Using the above samples , further more samples are created. For the sample creation process two techniques are followed. First technique is crossover. In crossover technique, a random locus is chosen within the range of sample size. Through that locus to the end of sample, all the elements are swapped between these two samples as shown. Thus new two samples have been created. Created samples may have better fitness values than their parents. That’s why these newly created samples included into the sample set for next generation. 

The second technique to create new samples from best fitness samples is mutation. In mutation, for each parent sample, one random locus is generated and the value at that position is replaced with complement of the current value. Thus two more samples are created. We have a total of four newly created samples with two best samples for this generation. All these six samples go into the next generation. The whole process repeats itself until the predefined number of generations are over or we get the sample with the required fitness value.  

The complete process is given in algorithm 3. The application graph (T, E) and sample solution assignment is given to the ETFGBF algorithm. It will return the application finish time and the energy consumption after execution. The crossover used in GAVFA algorithm is single point crossover. Single point crossover means there is only one locus , that is used to swap the elements of the samples. In mutation process, a maximum value MAX_VAL ( 1000 is used in this case) is required to complement the values present on the randomly chosen locus. The normalization of frequency and voltage values is required because of this randomization in genetic algorithm so that unexpected results won’t come and the number of generations taken to get a good fitness value is not more. Randomization is involved in the complete sample values in the corresponding sample. Every element in the sample has equal probability of getting selected for the complementation in the mutation process. The effect of crossover is more, because more elements are involved in the process. Mutation does not affect the sample value and thus the fitness value of the sample. The following flowchart shows the complete process for GAVFA algorithm
.


![](.\Picture7.png)

Fig.7 - GAVFA algorithm

Evolutionary Programming for Voltage and Frequency Assignment
Only mutation, no crossover is applied in evolutionary programming technique. Thus only complementation of the element at randomly chosen locus is done to create new sample. All the other steps are same as in GAVFA algorithm. The following flowchart shows the complete process of evolutionary programming (EPVFA).  


![](.\Picture8.png)

Fig. 8 - EPVFA algorithm

Analysis and Comparison
Initial application mapping algorithm solves the binary quadratic problem as a convex optimization problem. For every task in N tasks in the sorted volume set, it runs the number of tasks i.e. N times to set the corresponding row in the map matrix. Thus its overall time is O(N^2). Then application scheduling algorithm require transformation and b-level of all the unified nodes. Transformation of the original application graph takes O(N+E) time. For b-level calculation, a recursive algorithm is used on the transformed DAG. It may traverse all other nodes for every node b-level calculation in worst case. Thus it takes O(N+E) time. 
In ETFGBF scheduling algorithm step 6 takes linear time, step 7 takes O((N+E)^2) time because for every children all of its parents are checked and step 8 takes O((sqrt(M))(N+E)^2)) time in worst case. Here sqrt(M) is basically the number of NoC links two communication links may have in common. All the above steps are done for every unified node in the transformed graph. Thus the overall time complexity for the ETFGBF scheduling will be O((sqrt(M))(N+E)^3) in worst case. The overall model implementation time will be the time taken by genetic algorithm or evolutionary programming (both will take same time asymptotically). For genetic algorithm, the number of samples for which ETFGBF scheduling algorithm is applied, are constant. Suppose genetic algorithm runs for P number of generations. Application mapping algorithm will run initially to map the tasks on processors. In every generation, scheduling algorithm will run constant number of times. For calculating fitness values and sorting, it takes O((N+E)log(N+E)) time. For the crossover and mutation , it takes O(N+E) time in worst case. Thus the overall time for the model will be  P*(O((N+E)log(N+E))+O((sqrt(M))(N+E)^3)) which is equal to O(P*(sqrt(M))*(N+E)^3). Where P is the number of generations, M is the number of processors, N is the number of tasks and E is the number of communication links.
Comparison
Both genetic algorithm and evolutionary programming take asymptotically same time. But on computing machine evolutionary programming takes less time. Because evolutionary programming does not do crossover between the best samples in a generation. It only does mutations to create new samples.Genetic algorithm does both crossovers and mutations. So for the same number of generations , evolutionary programming takes less time than genetic algorithm with significant amount of time difference.if we increase the number of generations, this time difference increae significantly.the following plot shows the significant time difference between these two algorithms.


![](.\Picture9.png)

Fig. 9 - Time comparison plot between GAVFA and EPVFA algorithms 

Key observation is that if we consider same time for both the algorithms then evolutionary programming will able to run more for number of generations.that means it will get chance to converge fitness value more number of times. Thus fitness value obtained by evolutionary programming will be more than by genetic algorithm. Thus we can get more fit sample, in our case which is the optimal voltage and frequency assignment.
Conclusion and Future Scope
The solution model presented in this paper works efficiently for simple and complex application graphs. It works for mesh architecture of any number of processors. For the future scope, the number of assumptions taken into consideration can be removed and model can be made more complex. The model presented in this paper can be tested for wide variety of complex real life graphs. To check the contention among communication links, xy routing is considered to make the problem simple. But in real life situations, this constraint is not feasible. Many complex routing algorithms can be used in place of xy routing. The algorithm can also be provided to find the contention in that situation. It will make contention awareness more difficult to achieve. The complexity of ETFGBF algorithm may increase by adding this feature. For the relaxation based iterative rounding algorithm, initial mappings of tasks to processors are required. These initial mappings should also be taken by applying some heuristic. The same way for genetic algorithm and evolutionary programming the initial samples are required. These initial samples affect the convergence rate towards the actual solution. Thus initial must be closer to the actual solution. For this purpose benchmark frequency and voltage assignment can be used.

References
1.	P.K. Sahu, S. Chattopadhyay, A survey on application mapping strategies for network-on-chip design, J. Syst. Archit. 59 (1) (2013).
2.	J. Huang, C. Buckl, A. Raabe, A. Knoll, Energy-aware task allocation for network-on-chip based heterogeneous multiprocessor systems, in: Proc. 19th Euromicro International Conference on Parallel, Distributed and Network- Based Processing, 2011, pp. 447–454.
3.	C.-L. Chou, R. Marculescu, Contention-aware application mapping for network-on-chip communication architectures, in: Proc. IEEE International Conference on Computer Design, 2008, pp. 164–169.
4.	Gao, Wei. "Comparison study of genetic algorithm and evolutionary programming." Machine Learning and Cybernetics, 2004. Proceedings of 2004 International Conference on. Vol. 1. IEEE, 2004.
Li, Dawei, and Jie Wu. "Energy-efficient contention-aware application mapping and scheduling on NoC-based MPSoCs." Journal of Parallel and Distributed Computing 96 (2016): 1-11.
