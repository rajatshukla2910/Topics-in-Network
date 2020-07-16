#include <chrono>
#include <thread>
#include <random>
#include<iostream>
#include<bits/stdc++.h>
#include <time.h>
using namespace std;




double Exponential_Distribution(double temp)     // arrival 
{
  int seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator (seed);
  std::exponential_distribution<double> distribution (temp);
  double number = distribution(generator);
  return number;
}


int main()
{
long double  arrival_rate;
queue<double>Queue0; //initializing three queues
queue<double>Queue1;
queue<double>Queue2;


srand(time(NULL));
cout<<"ENter Arrival Rate : ";
cin>>arrival_rate;       //taking value of arrival rate 
long double service_rate;
cout<<"ENter Service Rate : ";//taking value of service time
cin>>service_rate;


double nextarival_time  = Exponential_Distribution(arrival_rate); //storing the interarrival time
int customer_serviced[3]={0,0,0};
long int total_customer[3]={0,0,0};
long double waiting_time[3]={0,0,0};
   // time of next arrival
double nextdeparture_time[3] = {INT_MAX , INT_MAX ,  INT_MAX,};

double total_wait_time[3]={0.0,0.0,0.0};
double averagewaiting_time[3]={0.0,0.0,0.0};
double l = arrival_rate;
double m= service_rate;
int random_value;
double a,b,c;

 while (nextarival_time<1000) {

        // arrival of customer
   random_value=rand()%3;// gererating the random value for passenger in which of the three queue he can go
             
     switch(random_value)
     {

         case 0:
                           //arrival of customer to queue0
            if (nextarival_time <= nextdeparture_time[0]) 
            {
              
                if (Queue0.empty()) nextdeparture_time[0] =  Exponential_Distribution(service_rate)+ nextarival_time ;
                Queue0.push(nextarival_time);   
                

                      
                nextarival_time = Exponential_Distribution(arrival_rate/3)+ nextarival_time ;
            }

            // departure of customer from queue0
            else
             {
                double wait = nextdeparture_time[0] - Queue0.front();
               Queue0.pop();
                total_wait_time[0] = total_wait_time[0] + wait;
                customer_serviced[0]++;
                 if (Queue0.empty()) 
                 nextdeparture_time[0] = INT_MAX;
                
                else  
                
                 nextdeparture_time[0] = Exponential_Distribution(service_rate)+ nextdeparture_time[0];
               
            }
             break;


       case 1 :

                 //arrival of customer to queue1
            if (nextarival_time <= nextdeparture_time[1]) 
            {
               
                if (Queue1.empty()) nextdeparture_time[1] =  Exponential_Distribution(service_rate) + nextarival_time ;
                Queue1.push(nextarival_time);   
                  

                     
                nextarival_time = Exponential_Distribution(arrival_rate/3)+ nextarival_time ;
            }

            // departure of customer from  queue1
            else
             {
                    
                double wait = nextdeparture_time[1] - Queue1.front();
               Queue1.pop();
                total_wait_time[1] = total_wait_time[1] + wait;
                customer_serviced[1]++;
                 if (Queue1.empty()) 
                 nextdeparture_time[1] = INT_MAX;
                
                else  
                
                 nextdeparture_time[1] = Exponential_Distribution(service_rate)+ nextdeparture_time[1];
               
            }


           break;

    case  2:

              //arrival of customer to queue2
            if (nextarival_time <= nextdeparture_time[2]) 
            {
               
                if (Queue2.empty()) nextdeparture_time[2] =  Exponential_Distribution(service_rate) + nextarival_time ;
                Queue2.push(nextarival_time);   
                

                     
                nextarival_time = Exponential_Distribution(arrival_rate/3)+ nextarival_time ;
            }

            // departure of customer from queue2
            else
             {
                   
                double wait = nextdeparture_time[2] - Queue2.front();
               Queue2.pop();
                total_wait_time[2] = total_wait_time[2] + wait;
                customer_serviced[2]++;
                 if (Queue2.empty()) 
                 nextdeparture_time[2] = INT_MAX;
                
                else  
                
                 nextdeparture_time[2] = Exponential_Distribution(service_rate)+ nextdeparture_time[2];
               
            }

             break;

        }
 }
 double totalwaiting_time =0;
 double customerserviced =0;
 double average_response_time;
 for (int i = 0; i < 3; i++)
 {
     
     totalwaiting_time+=total_wait_time[i];//computing the total waiting time of customer
    customerserviced+=customer_serviced[i];


 }

average_response_time =totalwaiting_time/customerserviced ;//computing the average response time of customer
double z=average_response_time;

ofstream fout1;
fout1.open("Case1.txt");
/*cout<<"customer_serviced : "<<customer_serviced<<endl<<"total_wait_time:"<<total_wait_time<<endl;<<avrage w8ing time in queue*/a=z*l;b=(l/m)*z;c=b*l;

fout1<<"Average number of passengers getting inspected: "<<a<<endl;
fout1<<"Average response time for passengers  : "<<average_response_time<<endl;
fout1<<"Average time for which a passenger has to wait : "<<b<<endl;
fout1<<"Average number of passengers waiting in queue: "<<c<<endl;
return 0;

}