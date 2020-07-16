#include<bits/stdc++.h>
#include <algorithm> 
#define INFI 100000 
#define max 100
using namespace std ;
int src_dest[101][2];
int delay[101];
float capacity[101];
float data[5000];
int conn_src_dest[500][2];
float bandwidth[500][3];
int SZ=0;
int forwarding_table[100][100][5];
//int path_table[][][6]
int vcid=100;
int f_it[100];             //ARRAY TO KEEP TRACK OF NO. OF ENTRIES IN AT ith ROUTER
int num_nodes;
 int num_edges;
vector<vector<vector<int>>> path2(100,vector<vector<int>>(100, vector<int>(100)));


 int dist2[100][100];


//******************//  function for finding minimum edge //******************************************
int minDistance(int dist[], bool sptSet[]) 
{ 
    // Initialize min value 
    int min = 100000, min_index; 
  
    for (int v = 0; v < num_nodes; v++) 
        if (sptSet[v] == false && dist[v] <= min) 
            min = dist[v], min_index = v; 
  
    return min_index; 
} 
//***********************// applying dijiktras algorthum for 2nd shortest path //***********************


void dijiktras(int G[100][100],int n,int startnode,int endnode)
{

int cost[max][max],distance[max],parent[max];
   int visited[max],count,mindistance,nextnode,i,j;
   for(i=0;i<n;i++)
      for(j=0;j<n;j++)
   if(G[i][j]==0)
      cost[i][j]=INFI;
   else
      cost[i][j]=G[i][j];
   for(i=0;i<n;i++) {
      distance[i]=cost[startnode][i];
      parent[i]=startnode;
      visited[i]=0;
   }
   distance[startnode]=0;
   visited[startnode]=1;
   count=1;
   while(count<n-1) {
      mindistance=INFI;
      for(i=0;i<n;i++)
         if(distance[i]<mindistance&&!visited[i]) {
         mindistance=distance[i];
         nextnode=i;
      }
      visited[nextnode]=1;
      for(i=0;i<n;i++)
         if(!visited[i])
      if(mindistance+cost[nextnode][i]<distance[i]) {
         distance[i]=mindistance+cost[nextnode][i];
         parent[i]=nextnode;
      }
      count++;
   }
   
    
     dist2[startnode][endnode]=distance[endnode];
    
      j=endnode;
      int countx=0;
      path2[startnode][endnode].push_back(j);
     do {
                                                              // tracing the whole path from end node to start
        
         j=parent[j];
          path2[startnode][endnode].push_back(j);       
         countx++;
        
      }while(j!=startnode);

     
reverse(path2[startnode][endnode].begin(),path2[startnode][endnode].end());  //reverse the path to get path from start to end
path2[startnode][endnode].resize(countx+1);    //resizing the path vector
 
}

//********************************// Utility function to save the file in an array //********************************

void read_file(char* file)
{   

SZ = 0;
FILE *ptr;
;
ptr = fopen(file, "r");

    if (ptr == NULL)
        printf("Can't open %s for reading.\n", file);
    else
    {
        while (fscanf(ptr,"%f",&data[SZ++])==1)
        {
          
        }

 
        fclose(ptr);
}

}



//**************************************************************************************************//
//**************************************** Main function //*****************************************//


int main(int argc , char* argv[])
{
 if(argc!=15)
 {
     cout<<"invalid input";
     return -1;
 }

 char* topfile=(argv[2]);
 
 read_file(topfile);
  num_nodes=data[0];
  num_edges=data[1];
 //cout<<num_nodes<<num_edges<<endl;
 int k=1;


 //**********************************Saving the topology file data************************************//
 
 
 for(int i=0;i<num_edges;i++)                            
 { k++;
     src_dest[i][0]=data[k++];                  //src_dest array save sorce and destination
     src_dest[i][1]=data[k++];
     delay[i]=data[k++];              //saving  link delay
     capacity[i]=data[k++];                //saving  link capacity
     
 }
 //**********************************Saving the connection file data *******************************//

 char* connection_file=argv[4];
 read_file(connection_file);
 int num_connections=data[0];
 int conne_it=1;
 for(int i=0;i<num_connections;i++)
 {
     conn_src_dest[i][0]=(int)data[conne_it++];
     conn_src_dest[i][1]=(int)data[conne_it++];           // saving connection requests 
     bandwidth[i][0]=data[conne_it++];                  // minimum bandwidth
     bandwidth[i][1]=data[conne_it++];                      // average bandwidth
     bandwidth[i][2]=data[conne_it];                         // maximum bandwidth
     conne_it++;              
     
 }

/* for(int i=0;i<num_connections;i++)
 {
     cout<<conn_src_dest[i][0]<<" ";               //printing connections
    cout<<conn_src_dest[i][1]<<" ";
     cout<<bandwidth[i][0]<<" ";
      cout<<bandwidth[i][1]<<" ";
       cout<<bandwidth[i][2]<<" ";
       cout<<endl;
 }
 cout<<endl<<endl;*/



 //******************************constructing the adjacency matrix***********************************//


int graph[100][100];
for(int i=0;i<num_nodes;++i)
{
    for(int j=0;j<num_nodes;++j)
    {
        graph[i][j]=100000;
        if(i==j)
        graph[i][j]=0;
    }
}
if(strcmp(argv[12],"dist")==0)
for(int i=0;i<num_edges;++i)
{
   graph[src_dest[i][0]][src_dest[i][1]]=delay[i];
   graph[src_dest[i][1]][src_dest[i][0]]=delay[i];
}
else
{
    for(int i=0;i<num_edges;++i)
{
   graph[src_dest[i][0]][src_dest[i][1]]=1;
   graph[src_dest[i][1]][src_dest[i][0]]=1;
}
}


/*                  print the graph


for(int i=0;i<num_nodes;++i)
{
    for(int j=0;j<num_nodes;++j)
    {
        cout<<graph[i][j]<<" ";
    }
    cout<<endl;
}


cout<<endl<<endl;*/



//********************Finding shortest distance using floyd warshall algorithum ****************************************//


vector<vector<int>> next(num_nodes);           // to save the next node in the path
 for (int i = 0; i < num_nodes; ++i)
  {
    for (int j = 0; j < num_nodes; ++j) 
    {
      next[i].push_back(0);
    }
    for (int j = 0; j < num_nodes; ++j) {                    //initializing the vector
      if (i != j) {
        next[i][j] = j;
      }
    }
  }

  int dist[num_nodes][num_nodes];                  // matrix for saving the minimum distances b/w every pair of nodes

  for(int i=0;i<num_nodes;++i)
  {
      for(int j=0;j<num_nodes;++j)
       dist[i][j]=graph[i][j];
  }

for (k = 0; k < num_nodes; k++)  
    {  
       
        for (int i = 0; i < num_nodes; i++)  
        {  
             
            for (int j = 0; j < num_nodes; j++)  
            {  
                  
                if (dist[i][k] != 100000 &&                             //conditions for updating distance b/w k and i 
     dist[k][j] != 100000 && dist[i][k] + dist[k][j] < dist[i][j])  
                    {
                    dist[i][j] = dist[i][k] + dist[k][j];  
                    next[i][j] = next[i][k];
                    }
            }  
        }  
    }  
  
vector<vector<vector<int>>> paths(num_nodes,vector<vector<int>>(num_nodes, vector<int>(num_nodes)));
 
 //paths vector saves the 1st shortest path

for (int i = 0; i < num_nodes; ++i)
 {
    for (int j = 0; j < num_nodes; ++j) 
    {
        if(i!=j)
        {
         int u=i;
         int num_nodes=j;
         paths[i][j].push_back(u);
  int count=0;
do {
          u = next[u][num_nodes];
          paths[i][j].push_back(u);             //Saving shortest path
          count++;
        } while (u != num_nodes);

       reverse(paths[i][j].begin(),paths[i][j].end());                 //removing junk leading junk values in vector
        paths[i][j].resize(count+1);                     
        reverse(paths[i][j].begin(),paths[i][j].end());     //saving the path in proper format
 

    }
    }
 }
/******** printing paths with shortest distances ********************************/
/*for(int i=0;i<num_nodes;++i)
{
    paths[i][i].resize(1);
}
 for(int i=0;i<num_nodes;++i)
 {  
     for(int j=0;j<num_nodes;++j)
     {  
      cout<<" path: "<<i<<"->"<<j<<"   ";
         for(int k=0;k<paths[i][j].size();++k)
         cout<<paths[i][j][k]<<"->";
     cout<<endl;
    
    // cout<<dist[i][j]<<" ";
         
     }
     cout<<endl;
 }
cout<<endl<<endl;*/

 ///////////////////////////////////////////////////////////////////////////////////////////////////////

//*****************************SECOND SHORTEST PATH****************************************************//

int temp_graph[100][100];                      //temporary graph

for(int i=0;i<num_nodes;++i)
{
    for(int j=0;j<num_nodes;++j)
    {
        temp_graph[i][j]=graph[i][j];            //initialising temp graph
    }
}

for(int i=0;i<num_nodes;++i)
{
    for(int j=0;j<num_nodes;++j)
    {     if(i!=j)
    { 
        
        for(int k=0;k<paths[i][j].size()-1;++k)            //excluding the 1st shortest path b/w i and j
        {
            temp_graph[paths[i][j][k]][paths[i][j][k+1]]=100000;
         //   cout<< paths[i][j][k]<<" "<<paths[i][j][k+1]<<" ";
        }
           
          
       dijiktras(temp_graph,num_nodes,i,j);           // to find the 2nd shortest path b/w i and j


       for(int k=0;k<num_nodes;++k)
        {
            for(int q=0;q<num_nodes;++q)
              {
                  temp_graph[k][q]=graph[k][q];          // undoing the removal of 1st shortest path b/w i and j
              }
        }
    }
    }
}

for(int i=0;i<num_nodes;++i)
{
   path2[i][i].resize(1);           // because path i to i is of zero distance

path2[i][i].push_back(i);

//reverse(path2.begin(),path2.end());
//path2[i][i].resize(1);   
vector<int>::iterator it; 
  
    it = path2[i][i].begin(); 
path2[i][i].erase(it);

}
for(int i=0;i<num_nodes;++i)
{
   paths[i][i].resize(1);           // because path i to i is of zero distance
paths[i][i].push_back(i);
//reverse(paths.begin(),paths.end());
vector<int>::iterator it; 
  
    it = paths[i][i].begin(); 
paths[i][i].erase(it);  
}
/*
for(int i=0;i<num_nodes;++i)
{
    for(int j=0;j<num_nodes;++j)
    {                                            //printing second shortest path
    /*for(int k=0;k<path2[i][j].size();++k)
    {
        cout<<path2[i][j][k]<<"->";
    }
    cout<<dist2[i][j]<<" ";
    }
    cout<<endl;
}*/

//*************************************************SAVING ROUTHING TABLE**************************************//
ofstream rout;
rout.open("routing_table.txt");
for(int src=0;src<num_nodes;++src)
{
    rout<<"For node "<<src<<endl;
    for(int dest=0;dest<num_nodes;++dest)
    {
        rout<<dest<<"   |   ";
        
  rout<< " path1 : ";


       for(int j=0;j<paths[src][dest].size();++j)
       {
         if(j!=paths[src][dest].size()-1)
          rout<<paths[src][dest][j]<<"->";         //saving 1st shortest path b/w src and dest
          else
          {
              rout<<paths[src][dest][j]<<"   ";
          }
       }


       rout<<"  path2 : ";
       for(int j=0;j<path2[src][dest].size();++j)
       {  if(j!=path2[src][dest].size()-1)
          rout<<path2[src][dest][j]<<"->";         //saving 2nd shortest path b/w src and dest
          else
          {
              rout<<path2[src][dest][j]<<"   ";
          }
          
       }

       rout<<dist[src][dest]<<","<<dist2[src][dest]<<"  |  ";
       if(path2[src][dest].size()!=0 && path2[src][dest].size()!=1 )
       rout<<path2[src][dest].size()-2;
       else
       {
           rout<<0;
       }
       
       rout<<endl<<endl;
    }
    rout<<endl<<endl<<endl;
}










//******************************************connection establishment******************************//
ofstream fout;
fout.open("path_table.txt");               //OPENING THE PATH TABLE FILE

for(int i=0;i<100;++i)
f_it[i]=0;               //INITIALIZATION OF ARRAY

for(int i=0;i<num_connections;++i)
{
    int src=conn_src_dest[i][0];
    int dest=conn_src_dest[i][1];
    float equi_bw=0;
    if(strcmp(argv[14],"0")==0)
   equi_bw= min(1.0*bandwidth[i][2],bandwidth[i][1]+(.25)*(bandwidth[i][2]-bandwidth[i][0])); 
   else
   {
       equi_bw=bandwidth[i][1];
   }
   
    // finding equivalent bandwidth using the given formula

       int index=-1;
       int pathway[paths[src][dest].size()];
       int pathway1[path2[src][dest].size()];

       for(int j=0;j<paths[src][dest].size();++j)
       {
           pathway[j]=paths[src][dest][j];         //saving 1st shortest path b/w src and dest
       }
       for(int j=0;j<path2[src][dest].size();++j)
       {
           pathway1[j]=path2[src][dest][j];         //saving 2nd shortest path b/w src and dest
       }

       
      
       int no_connection=1;        //flag for connection through path 1
       int no_connection1=1;       //flag for connection through path 2

/////////////        checking with path1 //////////////////////////////////////

  for(int q=0;q<paths[src][dest].size()-1;++q)
  {
      int src1=pathway[q];
      int dest1=pathway[q+1];
     
    for(int j=0;j<num_edges;++j)           //finding the cpacity index corresponding to the given link
    {
        if((src1==src_dest[j][0] && dest1==src_dest[j][1])  || (src1==src_dest[j][1] && dest1==src_dest[j][0]))
         {index=j;
         break;
         }
         
    }
    if(capacity[index]<equi_bw)              //checking required link condition
    {
        no_connection=0;
        break;
    }
    
  
  
  }
/////////////////////// when connection not establised with path1/ //////////////////////////////////////////////////
  if(no_connection==0)
  {
      index=-1;

      for(int q=0;q<path2[src][dest].size()-1;++q)        // tracing 2nd shortest path
  {
      int src1=pathway1[q];                   // finding next link in the path
      int dest1=pathway1[q+1];
    for(int j=0;j<num_edges;++j)          //finding the cpacity index corresponding to the given link
    {
        if((src1==src_dest[j][0] && dest1==src_dest[j][1])  || (src1==src_dest[j][1] && dest1==src_dest[j][0]))
         {index=j;
         break;
         }
         
    }
    if(capacity[index]<equi_bw)          //checking required link condition
    {
        no_connection1=0;
        break;
    }
    
  
  
  }
      if(no_connection1==0)
      {//cout<<"connection for "<<i<<" is not establised"<<endl;
      }

      else
      {
          
     // cout<<"connection  establised for "<<i<<" from path2"<<endl;
      for(int q=0;q<path2[src][dest].size()-1;++q)
   {
      int src1=pathway1[q];            //    finding next link in the path
      int dest1=pathway1[q+1];
    for(int j=0;j<num_edges;++j)            // updating link capacities in the path
    {
        if((src1==src_dest[j][0] && dest1==src_dest[j][1])  || (src1==src_dest[j][1] && dest1==src_dest[j][0]))
         { capacity[j]-=equi_bw;
           
         }
         
    }
   
   
  
      }
//****************************** UPDATING FORWARDING TABLE*************************************//

      forwarding_table[src][f_it[src]][0]=src;
  forwarding_table[src][f_it[src]][1]=-1;
  forwarding_table[src][f_it[src]][2]=-1;
  forwarding_table[src][f_it[src]][3]=pathway1[1];
  forwarding_table[src][f_it[src]][4]=++vcid;
  int start_vcid=vcid;
  int end_vcid;
  f_it[src]++;
  int flag=0;
   for(int q=1;q<path2[src][dest].size();++q)
  {
      int src1=pathway1[q];
     
  forwarding_table[src1][f_it[src1]][0]=src1;
  forwarding_table[src1][f_it[src1]][1]=pathway1[q-1];
  forwarding_table[src1][f_it[src1]][2]=vcid;
  if(q!=path2[src][dest].size()-1)
  forwarding_table[src1][f_it[src1]][3]=pathway1[q+1];
  else
  {
      flag=1;
      forwarding_table[src1][f_it[src1]][3]=-1;;
  }
  if(flag==0)
   forwarding_table[src1][f_it[src1]][4]=++vcid;
   else
   {
       forwarding_table[src1][f_it[src1]][4]=-1;
   }
   
  f_it[src1]++;
  end_vcid=vcid;
  }
  //*******************************SAVING PATH TABLE*************************************************

      fout<<i<<"  |  "<<src<<"  |  "<<dest<<"  |  ";
for(int i=0;i<paths[src][dest].size();++i)
if(i!=0)
fout<<"->"<<pathway1[i];
else
{
    fout<<pathway[i];
}

fout<<"  |   ( ";
for(int i=start_vcid;i<=end_vcid;++i)
{
    fout<<" "<<i<<" ";
}
fout<<")    |   ";
fout<<dist2[src][dest];
fout<<endl<<endl;


  }


  }

  //estab with path1/////////////////////////////////////////////

  else
  {
     // cout<<"connection  establised for "<<i<<" from path1"<<endl;
      for(int q=0;q<paths[src][dest].size()-1;++q)
  {
      int src1=pathway[q];              // finding next link in the path
      int dest1=pathway[q+1];
    for(int j=0;j<num_edges;++j)      // pdating link capacities in the path
    {
        if((src1==src_dest[j][0] && dest1==src_dest[j][1])  || (src1==src_dest[j][1] && dest1==src_dest[j][0]))
         {capacity[j]-=equi_bw;
         
         }
         
    }
    
    
  
  }
///////////////////////////// UPDATING FORWARDING TABLE /////////////////////////////////
  forwarding_table[src][f_it[src]][0]=src;
  forwarding_table[src][f_it[src]][1]=-1;
  forwarding_table[src][f_it[src]][2]=-1;
  forwarding_table[src][f_it[src]][3]=pathway[1];
  forwarding_table[src][f_it[src]][4]=++vcid;
  int start_vcid=vcid;
  int end_vcid;
  f_it[src]++;
  int flag1=0;
   for(int q=1;q<paths[src][dest].size();++q)
  {
      int src1=pathway[q];
     
  forwarding_table[src1][f_it[src1]][0]=src1;
  forwarding_table[src1][f_it[src1]][1]=pathway[q-1];
  forwarding_table[src1][f_it[src1]][2]=vcid;
  if(q!=paths[src][dest].size()-1)
  forwarding_table[src1][f_it[src1]][3]=pathway[q+1];
  else
  {
      flag1=1;
      forwarding_table[src1][f_it[src1]][3]=-1;;
  }
  if(flag1==0)
   forwarding_table[src1][f_it[src1]][4]=++vcid;
   else
   {
       forwarding_table[src1][f_it[src1]][4]=-1;
   }
   
  f_it[src1]++;
  end_vcid=vcid;
  }
  //***********************SAVING PATH TABLE***********************************************

  fout<<i<<"  |  "<<src<<"  |  "<<dest<<"  |   ";
for(int i=0;i<paths[src][dest].size();++i)
if(i!=0)
fout<<"->"<<pathway[i];
else
{
    fout<<pathway[i];
}

fout<<"  |   ( ";
for(int i=start_vcid;i<=end_vcid;++i)
{
    fout<<" "<<i<<" ";
}
fout<<")   |   ";
if(paths[src][dest].size()!=0 && paths[src][dest].size()!=1)
fout<<paths[src][dest].size()-2;
else
{
    fout<<0;
}

fout<<endl<<endl;
}





}
fout.close();

//***********************************saving forwarding table to a file*****************************//
ofstream fout1;
fout1.open("forwarding_table.txt");

for(int i=0;i<num_nodes;++i)
{
    fout1<<"for node "<<i<<endl;

 for(int j=0;j<f_it[i];++j)
 {
 fout1<<forwarding_table[i][j][0]<<" "<<forwarding_table[i][j][1]<<" "<<forwarding_table[i][j][2]<<" "<<forwarding_table[i][j][3]<<" "<<forwarding_table[i][j][4]<<endl;
 }

fout1<<endl<<endl;
}
 return 0;
}

