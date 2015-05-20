#include <stdio.h>
#include <iostream>
#include <fstream>
#define p printf
#define s scanf
#include <stdlib.h>

int working_window=5; //size of working window
float work_window1[5]; //working window array
float work_window2[5]; //working window array
float *input_values1;  // input values array
float *input_values2;  // input values array
float *output_values1; //output values array
float *output_values2; //output values array
float *tim; //time array
int array_size; //size of arrays
int count=0;

using namespace std;

int main()
{
int i=1;
int j,k;
int temp;
int flag=0;
int heap_alloc;
ifstream image_file("radtime.csv", std::ios_base::in); ///reading the input file containing readius vs  time
ofstream image_filee("radtime1.csv"); //creating file for the output after applying filter
image_file.seekg(0,std::ios::end); //go to end of file to find size
heap_alloc=image_file.tellg()/2; //find the size of file and divide it by two to allocate size for heap arrays input values, output values and tim
//cout<< image_file.tellg()/(2*sizeof(float));
image_file.seekg(0,std::ios::beg); //go to the beginning of file
input_values1=(float*)malloc(2*heap_alloc); //allocation of memory
input_values2=(float*)malloc(2*heap_alloc); //allocation of memory
output_values1=(float*)malloc(2*heap_alloc); //allocation of memory
output_values2=(float*)malloc(2*heap_alloc); //allocation of memory
tim=(float*)malloc(2*heap_alloc); //time
while(image_file>>tim[i])
{
		image_file >>input_values1[i];
		image_file >>input_values2[i];
		//cout << working_window[i] << " "; //read values
		i++;
}
input_values1[0]=input_values1[1]; //resolving boundary issues in median filter
input_values2[0]=input_values2[1]; //resolving boundary issues in median filter
//cout<<i;
array_size=i;
for(i=0;i<array_size;i++)
{
	for(j=0;j<working_window;j++)
	{
		work_window1[j]=input_values1[i-(working_window/2)+j]; //asssign value for work window
		work_window2[j]=input_values2[i-(working_window/2)+j]; //asssign value for work window
	}	
	for(j=0;j<working_window;j++)
	{
		for(k=1;k<working_window;k++)
		{
			if(work_window1[k-1]>work_window1[k]) //bubble sort implementation for sorting work window
			{
				temp=work_window1[k-1];
				work_window1[k-1]=work_window1[k];
				work_window1[k]=temp;
			}
		}	
	}
	for(j=0;j<working_window;j++)
	{
		for(k=1;k<working_window;k++)
		{
			if(work_window2[k-1]>work_window2[k]) //bubble sort implementation for sorting work window
			{
				temp=work_window2[k-1];
				work_window2[k-1]=work_window2[k];
				work_window2[k]=temp;
			}
		}	
	}
	input_values1[i]=work_window1[working_window/2]; //updating input values
	input_values2[i]=work_window2[working_window/2]; //updating input values
	output_values1[i]=work_window1[working_window/2]; //setting output values
	output_values2[i]=work_window2[working_window/2]; //setting output values
	//cout<<output_values[i]<<" ";
	image_filee<<tim[i]<<"\t"<<output_values1[i]<<"\t"<<output_values2[i]<<"\n"; //writing output values
	count++;
}
//cout<<"\n";
//cout<<count;
image_file.close(); //close input file
image_filee.close(); //close output file
getchar();
}

 





