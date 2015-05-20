#include <stdio.h>
#include <iostream>
#include <fstream>
#define p printf
#define s scanf
#include <stdlib.h>

float *input_values1;  // input values array
float *input_values2;  // input values array
float *diff1;  // input values array
float *diff2;
float *tim; //time array
int *pulse;
int array_size; //size of arrays
int count=0;

using namespace std;

int main()
{
int plf,j,k,i=0;
float t1d,t2d,t3d,t1s,t2s,t3s,t;
int heap_alloc;
int window=25;
float base1,base2, rad1, rad2;
ifstream image_file("Doj.csv", std::ios_base::in); ///reading the input file containing readius vs  time
ofstream image_filee("Doj1.csv");
image_file.seekg(0,std::ios::end); //go to end of file to find size
heap_alloc=image_file.tellg()/2; //find the size of file and divide it by two to allocate size for heap arrays input values, output values and tim	
//cout<< image_file.tellg()/(2*sizeof(float));
image_file.seekg(0,std::ios::beg); //go to the beginning of file
input_values1=(float*)malloc(2*heap_alloc); //allocation of memory
input_values2=(float*)malloc(2*heap_alloc); //allocation of memory
diff1=(float*)malloc(2*heap_alloc); //allocation of memory
diff2=(float*)malloc(2*heap_alloc);
pulse=(int*)malloc(2*heap_alloc); //allocation of memory
tim=(float*)malloc(2*heap_alloc); //time
while(image_file>>tim[i])
{
		image_file>>input_values1[i];
		//cout<<input_values1[i];
		image_file>>input_values2[i];
		//cout<<input_values2[i];
		image_file>>pulse[i];
		//cout<<pulse[i];
		i++;
}
array_size=i;
base1=input_values1[0];
base2=input_values2[0];
for(i=0;i<=array_size;i++){
diff1[i]=input_values1[i]-base1;
diff2[i]=input_values2[i]-base2;
image_filee<<tim[i]<<"\t"<<diff1[i]<<"\t"<<diff2[i]<<"\n"; //writing output values
}

image_file.close(); //close input file
image_filee.close(); //close output file
//getchar();
}
