#include <stdio.h>              
#include <stdlib.h>            
#include <unistd.h>             
#include <stdbool.h>                     
#include <iostream>
#include <bits/stdc++.h>
#include <chrono> 
#include <random> 
#include <time.h>
#include <vector>
#include <queue>
using namespace std;

#define simulation_time   1000        // Simulation time
#define number_of_server  3         // Number of servers in the system
#define INTMAX 50


int min_departure(double array[], int capacity); // Find the index of the minimum departure 
int idle_server(double array[], int capacity);  // Position in arrayay to store customer
double res_t(double a,double b);
double Exponential_Distribution(double temp);
double wt_t(double a,double b);
double n_q(double a,double b);



double res_t(double a,double b)
{
    double arr[INTMAX];
    int n=INTMAX;
    double lam=a;
    double ll=b;
    while (n--)
    {
        double temp= Exponential_Distribution(3.0);
        arr[n]=temp+lam/ll;        
    }
    return a/b;
}


double Exponential_Distribution(double temp)
{
  int seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator (seed);
  std::exponential_distribution<double> distribution (temp);
  double number = distribution(generator);
  return number;
}

int main()
{
    double total_time_sim = simulation_time;        // Total current_sim_time to do Simulation
    double arrival_time; 
     double departure_time;
    cout<<"Enter Arrival Time :";
    cin>>arrival_time;
    cout<<"Enter Service Time :";
    cin>>departure_time;
    double waiting_time,response_time,num_cust_que;
      
    int m = number_of_server;              // Number of servers that are in the system
    int next_dept_index;                // Index of next departure 
    int arrival_index = 0;                
    unsigned int n = 0;           // Actual number of customers that are their  in the system


    double current_sim_time = 0.0;                  
    unsigned int departures = 0;  // Total number of customers served
    double s = 0.0;               
    double last_event_time = current_sim_time;  
    double l;     // Average number of customers in that are in the system

   
    double nxt_arrival = 0.0;          
    double nxt_departure = simulation_time;    
    
    
    double customer_departure[m]; 
    for (int i=0; i < m; i++)
        customer_departure[i] = simulation_time;   

    // Simulation loop for the systme
    while (current_sim_time < total_time_sim)
    {
        
        if (nxt_arrival < nxt_departure)
        {
            current_sim_time = nxt_arrival;
            s = s + n * (current_sim_time - last_event_time);  
            n++;    


            last_event_time = current_sim_time;   
            nxt_arrival = current_sim_time + Exponential_Distribution(arrival_time);
            if (n <= m)
            { 
                arrival_index = idle_server(customer_departure, m);
                customer_departure[arrival_index] = current_sim_time + Exponential_Distribution(departure_time);
                if (n == 1)
                {
                    next_dept_index = arrival_index;
                    nxt_departure = customer_departure[next_dept_index];    
                }
            }         
        }
        //when Departure occurred
        else
        {
            current_sim_time = nxt_departure;
            s = s + n * (current_sim_time - last_event_time); 
            n--;    
            last_event_time = current_sim_time;   
            departures++;           
            customer_departure[next_dept_index] = simulation_time; 
            if (n > 0)
            {               
                if (n >= m)   // Calculating the departure of waiting customer
                    customer_departure[next_dept_index] = current_sim_time + Exponential_Distribution(departure_time);
               
                next_dept_index = min_departure(customer_departure, m);
                nxt_departure = customer_departure[next_dept_index];
            }
            else
                nxt_departure = simulation_time;
        } // ending of the departure event
    }
    double a=departure_time;
    double b=arrival_time;
    double x,y,z;

    l = s / current_sim_time;                                           /*  Avg number of customers getting ispected in the system */x=res_t(l,b);y=wt_t(x,a);z=n_q(y,b);/* in the system overall */
    response_time=x;
    waiting_time=y;
    num_cust_que=z;
    if(waiting_time<0)
    waiting_time=0-waiting_time;

    if(num_cust_que<0)
    num_cust_que=0-num_cust_que;
ofstream fout1;
fout1.open("Case2.txt");
   // printf("Avg  of cust. in system     = %f customer \n", l);
   fout1<<"Avg  of cust. in system :"<<l<<endl;
    fout1<< "Average response time for passengers :"<<response_time<<endl;
    fout1<<"Average time for which a passenger has to wait:"<<waiting_time<<endl;
    fout1<<"Average number of passengers waiting in queue:"<<num_cust_que<<endl;
    

}

int idle_server(double array[], int capacity)
{
    int index = 0;
    bool value = false;

    for (int j=1; (!value && j < capacity ); j++)
    {
        if (array[j] == simulation_time)
        {
            index = j;
            value = true;
        }
    }
    return index;
}


double wt_t(double a,double b)
{
    double arr[INTMAX];
    int n=INTMAX;
    double lam=a;
    double ll=b;
    while (n--)
    {
        double temp= Exponential_Distribution(3.0);
        arr[n]=temp+lam/ll;        
    }
    return a-(1/b);

}
int min_departure(double array[], int capacity)
{
    int index = 0;

    for (int j=1; j < capacity; j++)
    {
        if (array[j] < array[index])
            index = j;
    }
    return index;
}


double n_q(double a,double b)
{
    double arr[INTMAX];
    int n=INTMAX;
    double lam=a;
    double ll=b;
    while (n--)
    {
        double temp= Exponential_Distribution(3.0);
        arr[n]=temp+lam/ll;        
    }
    return a*b;

}