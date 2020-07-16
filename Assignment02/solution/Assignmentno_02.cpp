
#include<bits/stdc++.h>
#include <algorithm> 
using namespace std;
int no_of_ports=8;
int buffer_size=4;
double packetgenprob=0.5;

vector<double> sdv;

int k_drop=4;
int maxtimeslot=10000;


//  packet data structure FOR INQ AND KOUQ
class packet
{public :
  int out_port_no;
  int in_port_no;
  double timming;
  packet(int o,int in,double t)
  {
    out_port_no=o;
    in_port_no=in;
    timming=t;
  }
};

// USED IN ISLIP ONLY
typedef struct packet1 {
    bool is_generated; 
    int input_port; //input port
    int output_port; //output port
    double gen_tym_of_packet; //packet generation time
    double comp_tym_of_transmission; //completion time of transmission
    
}
packet1;



bool mycompr(packet p1, packet p2)         // to sort the vector according to time                                                 
{                                              //used in KOUQ
    return (p1.timming < p2.timming); 
} 



int main(int args,char* argv[])
{  


no_of_ports=atoi(argv[1]);
buffer_size=atoi(argv[2]);
packetgenprob=atof(argv[3]);
k_drop=atoi(argv[5]);

maxtimeslot=atoi(argv[7]);












if(strcmp(argv[4],"ISLIP")!=0)
{






vector<packet> qp[no_of_ports];                         //used as a output queue in KOUQ and input queue in INQ
double link_util_count[no_of_ports][no_of_ports];           //to maintain link utilization of each link     
int total_packet_generated=0;                              //total packet generated
int port_array[no_of_ports];                            //used for to store randomly generated o/p port for ith i/p port
double average_pkt_delay=0;                    
double avg_link_util=0;
double link_util=0;


double delay=0;
vector<double> std_pd;                             //standard deviation calculation



if(strcmp(argv[4],"INQ")==0)
{

vector<packet> output_buffer;
vector<queue<packet>> contention;

///////////////////////////////////////////   INQ   //////////////////////////////////////////////////////////////
 
  

for(int tym=0;tym<maxtimeslot;++tym)
{

for(int i=0;i<no_of_ports;++i)
 {
     queue<packet> q;
     contention.push_back(q);
 }


/*********************************   PACKET GENERATION PHASE    **********************************************/
for(int i=0;i<no_of_ports;++i)
  {
   float probability = rand()%100;
            probability /= 100;
  if(probability>=packetgenprob)    // to generate the packet according to the give probabilty of packet genertaion
 {}
  else
  {
    //srand(time(0));
     int number=rand()%(no_of_ports);
                                          //generate port number
      packet pt(number,i,tym);       
      if(contention[i].size()<=buffer_size)
      contention[i].push(pt);  
    

       }
  }

/******************************************** SCHEDULING PHASE  ****************************************/

vector<int> chose_port;
for(int i=0;i<no_of_ports;++i)
{
int index=-1;

for(int j=0;j<no_of_ports;++j)
{
    if(contention[j].size()!=0)
{
    if(contention[j].front().out_port_no==i)
    {

       index=j;
       break;
    }
}
}

if(index!=-1)
{  
   
    output_buffer.push_back(contention[index].front());
   contention[index].pop();
}


}

/******************************************   TRANSMISSION PHASE    ***************************************/



double std_delay=0;
int count=0;

for(int i=0;i<output_buffer.size();++i)
{

      average_pkt_delay=average_pkt_delay+tym-output_buffer[i].timming+1;  //summing dealys in this tym slot
    
   std_delay+=tym-output_buffer[i].timming+1;
   total_packet_generated++;
       count++;
     avg_link_util++;
      
    
  
}

if(count>0)
{std_delay/=count;
  std_pd.push_back(std_delay);

}
 output_buffer.clear();       //clearing output buffer

// calculating avg pkt delay in this time slot
 }


  
}



///////////////////////////////////////////   KOUQ   //////////////////////////////////////////////////////////////
 
else if(strcmp(argv[4],"KOUQ")==0)
{
  


vector<packet> contention[no_of_ports];



  long double drop_probability=0;
  int count_drop=0;

for(int tym=0;tym<maxtimeslot;++tym)
{

long double per_tym_drop=0;

/*********************************   PACKET GENERATION PHASE    **********************************************/


for(int i=0;i<no_of_ports;++i)
  {
    int num=(rand()%100+1 );             // to generate a nummber between 1 to 10
  //srand(time(0));
  if(num>(100*packetgenprob))    // to generate the packet according to the give probabilty of packet genertaion
  port_array[i]=-1;
  else
  {
    //srand(time(0));
      port_array[i]=rand()%(no_of_ports);            //generate port number
      packet pt(port_array[i],i,tym);       
      contention[ port_array[i]].push_back( pt);       // to keep track for contention on the generated o/p port
                 //incrementing link util

       }
   
  
  
  }

/******************************************** SCHEDULING PHASE  ****************************************/


for(int i=0;i<no_of_ports;++i)
{


  if(contention[i].size()!=0 && contention[i].size()<=k_drop)     //if contention is less than given k value
  {
       sort(contention[i].begin(),contention[i].end(),mycompr);        //sort the contention vector acc to timing

         int j=0;
         while(qp[i].size()<=buffer_size && j<contention[i].size())      //check for buffer size and contention size
         {
          
          qp[i].push_back(contention[i][j]);               //push in the output buffer queue
          j++;
         }
       
   
    }
    else if(contention[i].size()>k_drop)              // if contention is greater than k
    {
    
      per_tym_drop++;                              
      vector<packet> temp;

// randomly selecting k packets and saveing in temp vector
      for(int j=0;j<k_drop ;++j)
      {
        int random_packet=rand()%contention[i].size();
     
       temp.push_back(contention[i][random_packet]);

    contention[i].erase(contention[i].begin()+random_packet,contention[i].begin()+random_packet+1);
      }
// sort the randomly selected k values
      sort(temp.begin(),temp.end(),mycompr);
    for(int j=0;j<k_drop && qp[i].size()<=buffer_size;++j)
     qp[i].push_back(temp[j]);

    }
    
    contention[i].clear();

}


/******************************************   TRANSMISSION PHASE    ***************************************/
double std_delay=0;

if(tym>2)
{

int count=0;                     //to keep track to no. of pkt transmitted in this tym slot

for(int i=0;i<no_of_ports;++i)
{
  if(qp[i].size()!=0)
  {  
    
      average_pkt_delay+=tym-qp[i][0].timming+1; // summing pkt delay in this tym slot
     std_delay+=tym-qp[i][0].timming+1;
      count++;
     qp[i].erase(qp[i].begin(),qp[i].begin()+1);     
     avg_link_util++;         //transmitting the packet
     total_packet_generated++;
   
  }

  
}
// calculating avg pkt delay in this time slot
if(count>0)
{std_delay/=count;

 std_pd.push_back(std_delay);                            //saving for std deviation calculation

}


}
  
}




}



average_pkt_delay=average_pkt_delay/total_packet_generated;
// calculating std deviation for pkt delay
double sd=0;
for(int i=0;i<std_pd.size();++i)
{
  sd+=(std_pd[i]-delay)*(std_pd[i]-delay);
}
sd/=(total_packet_generated-1);
sd=sqrt(sd);


//calculating avg linkutilization


avg_link_util=(avg_link_util/(maxtimeslot*no_of_ports));

ofstream fout1;
fout1.open(argv[6],std::ios_base::app);
// printing the value to file
fout1<<"N\t"<<"p\t"<<"Queue type\t"<<"Avg PD\t\t"<<"Std Dev of PD\t\t"<<"Avg link utilisation\t"<<endl;
fout1<<no_of_ports<<"\t"<<packetgenprob<<"\t"<<argv[4]<<"\t\t"<<average_pkt_delay<<"\t\t"<<sd<<"\t\t"<<avg_link_util<<endl;
fout1.close();


}

else if(strcmp(argv[4],"ISLIP")==0)
{
//declaration
     int no_of_trsmtted_paket1 = 0; // store number of transmitted paket1 overall
    int no_of_genrted_packt = 0; //store number of paket1s generated
    double drop_prob = 0; //store drop paket1s
    int delay = 0; 
     int accpet_pointer_array[no_of_ports] = { 
        0
    };
    int grant_pointer_array[no_of_ports] = { 
        0
    };
    vector < int > array_for_delay; //vector to calculate standard deviation
    queue < packet1 > input_queue[no_of_ports][no_of_ports];
   
    srand(time(0));
    for (int t = 0; t < maxtimeslot; t++) {
     
        int req_ip_port[no_of_ports][no_of_ports];

         bool output_port_flag[no_of_ports] = { 
            false
        };
        bool ip_port_flag[no_of_ports] = { 
            false
        };
        int grant_req[no_of_ports] = { 
            -1
        };
        int accpet_req[no_of_ports] = { 
            -1
        };
       
        //generating paket1 for each input port for each time slotss
        for (int input_port = 0; input_port < no_of_ports; input_port++) {



                 packet1 p;
                double time = t + (double)((rand() % 10 + 1) / 1000.0); //generation time btwn t+0.001 to t+0.01
                int output_port = rand() % no_of_ports; //selecting o/p port randomly
               
               
               double random_num = ((double) rand() / (RAND_MAX)); //generating  a random number. btw 0 and 1
                if (random_num >= .5)
              p.is_generated = false;
             else {
              p.is_generated = true; 
             }
          
             p.input_port = input_port;
             p.gen_tym_of_packet = time;
             p.output_port = output_port;



           
            if (p.is_generated) { // insert paket1 in queue if generated
                no_of_genrted_packt++; // count of generated paket1s
                input_queue[input_port][p.output_port].push(p); 
            }
        }
        int connectionn = 0, iterations = 0; 
        do {
            connectionn = 0;
           
          
            for (int input_port = 0; input_port < no_of_ports; input_port++) {
                for (int output_port = 0; output_port < no_of_ports; output_port++) {
                    if (ip_port_flag[input_port] || output_port_flag[output_port]) { 
                        req_ip_port[input_port][output_port] = -1;
                        continue;
                    }
                    if (!input_queue[input_port][output_port].empty()) { // Check whether queue has paket1 or not
                        req_ip_port[input_port][output_port] = 1; 
                      
                    } else req_ip_port[input_port][output_port] = -1; 
                }
            }

            //Grant Request phase 
           
            for (int output_port = 0; output_port < no_of_ports; output_port++) {
                if (output_port_flag[output_port]) continue; //Check if output-port established connection
                int i = 0, input_port = grant_pointer_array[output_port];
                for (; i < no_of_ports; i++) { 
                    if (req_ip_port[input_port][output_port] == 1) break; 
                    input_port = (input_port + 1) % no_of_ports;
                }
                if (i < no_of_ports) grant_req[output_port] = input_port; 
                else grant_req[output_port] = -1;
            }

            //Accept-phase
            
            for (int input_port = 0; input_port < no_of_ports; input_port++) {
                if (ip_port_flag[input_port]) continue; //Check  input-port is bussy or not
                int i = 0, output_port = accpet_pointer_array[input_port];
                for (; i < no_of_ports; i++) { //Checking which port granted request
                    if (grant_req[output_port] == input_port) break; 
                    output_port = (output_port + 1) % no_of_ports;
                }
                if (i < no_of_ports) {
                    connectionn++; 
                    no_of_trsmtted_paket1++; //Counting  number of paket1s that are  transmitted
                  
                    output_port_flag[output_port] = true; // Mark outport as established connection
                    ip_port_flag[input_port] = true; // Mark inputport as establishe connection
                    packet1 p = input_queue[input_port][output_port].front(); 
                    p.comp_tym_of_transmission = t + 1; 
                    delay += (int) p.comp_tym_of_transmission - (int) p.gen_tym_of_packet; //calculating total delay
                    array_for_delay.push_back((int) p.comp_tym_of_transmission - (int) p.gen_tym_of_packet); 
                    input_queue[input_port][output_port].pop(); //removing  paket1 from the  queue
                    if( iterations == 0 ){
                        accpet_pointer_array[input_port] = (output_port+1)%no_of_ports;
                        grant_pointer_array[output_port] = (input_port+1)%no_of_ports;
                    }
                }
            }
            iterations++;
        } while (connectionn > 0);
    }


    //  link utilisation calculation
    float link_utilization = no_of_trsmtted_paket1 / (no_of_ports * maxtimeslot * 1.0);

    float Averagedelay = delay / (no_of_trsmtted_paket1 * 1.0); // average delay calculation
    //  Standard-Deviation calculation
    float stabdard_deviation = 0;
    for (int i = 0; i < array_for_delay.size(); i++) {
        stabdard_deviation += (array_for_delay[i] - Averagedelay) * (array_for_delay[i] - Averagedelay); 
    }
    stabdard_deviation = sqrt(stabdard_deviation / array_for_delay.size());
    
  //printing the data to file 
    ofstream fout1;
fout1.open(argv[6],std::ios_base::app);

fout1<<"N\t"<<"p\t"<<"Queue type\t"<<"Avg PD\t\t"<<"Std Dev of PD\t\t"<<"Avg link utilisation\t"<<endl;
fout1<<no_of_ports<<"\t"<<packetgenprob<<"\t"<<argv[4]<<"\t\t"<<Averagedelay<<"\t\t"<<stabdard_deviation<<"\t\t"<<link_utilization<<endl;
fout1.close();

}





return 0;

}

