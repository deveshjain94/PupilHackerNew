/*Libraries Used*/
#include <stdlib.h>
#include <iostream>
#include <fstream>
#define p printf
#define s scanf
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include "hpdf.h"
#include <string.h>
#include <time.h>

//#ifndef HPDF_NOPNGLIB

jmp_buf env;
time_t rawtime;
struct tm * timeinfo;
float *input_values1;   // input values array
float *input_values2;   // input values array
float *diff1;  		// input values array
float *diff2;
float *tim; 		//time array
int *pulseOD;
int *pulseOS;
char patdat[50];
int array_size; 	//size of arrays
int count=0;
int textsize1=15;

#ifdef HPDF_DLL
void  __stdcall
#else
void
#endif
error_handler  (HPDF_STATUS   error_no,			//Error Handling
                HPDF_STATUS   detail_no,
                void         *user_data)
{
    printf ("ERROR: error_no=%04X, detail_no=%u\n", (HPDF_UINT)error_no,
                (HPDF_UINT)detail_no);
    longjmp(env, 1);
}

HPDF_Doc  pdf;

static const int PAGE_WIDTH = 420;	//Declaring Page Heights
static const int PAGE_HEIGHT = 400;
static const int CELL_WIDTH = 50;
static const int CELL_HEIGHT = 19;
static const int CELL_HEADER = 10;
int col[]={40,150,207,265,322,380};

void
draw_graph (HPDF_Page   page1);

using namespace std;

void
draw_graph (HPDF_Page   page1)
{
HPDF_Font font1;
font1 = HPDF_GetFont (pdf, "Arial", NULL);  //Declaring Font for template 
    char buf[50];
    int i;
    int y;
    
    /* Draw 16 X 15 cells */
  
    /* Draw vertical lines. */
    HPDF_Page_SetLineWidth (page1, 0.5);

    HPDF_Page_MoveTo (page1, col[0], PAGE_HEIGHT-37);		//Column 1
    HPDF_Page_LineTo(page1, col[0], 40);
    HPDF_Page_Stroke(page1);
    	
    HPDF_Page_MoveTo (page1, col[1], PAGE_HEIGHT-37);		//Column 2
    HPDF_Page_LineTo(page1, col[1], 40);	
    HPDF_Page_Stroke(page1);
    	
    HPDF_Page_MoveTo (page1, col[2], PAGE_HEIGHT-37-CELL_HEIGHT*2); //Column 3
    HPDF_Page_LineTo(page1, col[2], 40);
    HPDF_Page_Stroke(page1);	
    	
    HPDF_Page_MoveTo (page1, col[3], PAGE_HEIGHT-37);		//Column 4
    HPDF_Page_LineTo(page1, col[3], 40);
    HPDF_Page_Stroke(page1);
    
    HPDF_Page_MoveTo (page1, col[4], PAGE_HEIGHT-37-CELL_HEIGHT*2); //Column 5
    HPDF_Page_LineTo(page1, col[4], 40);
    HPDF_Page_Stroke(page1);
    
    HPDF_Page_MoveTo (page1, col[5], PAGE_HEIGHT-37);		//Column 6
    HPDF_Page_LineTo(page1, col[5], 40);
    HPDF_Page_Stroke(page1);   
   
     /* Draw horizontal lines. */
    for (i = 0; i <= 4; i++) {
    y = i * 3*CELL_HEIGHT + 40;

        HPDF_Page_MoveTo (page1, 40, y);
        HPDF_Page_LineTo (page1, PAGE_WIDTH - 40, y);
        HPDF_Page_Stroke (page1);
    }
        HPDF_Page_MoveTo (page1, 40, y+CELL_HEIGHT*2);
        HPDF_Page_LineTo (page1, PAGE_WIDTH - 40, y+CELL_HEIGHT*2);
        HPDF_Page_Stroke (page1);
        
        HPDF_Page_MoveTo (page1, 40, 363);
        HPDF_Page_LineTo (page1, PAGE_WIDTH - 40, 363);
        HPDF_Page_Stroke (page1);
        
        HPDF_Page_MoveTo (page1, col[1], 325);
        HPDF_Page_LineTo (page1, PAGE_WIDTH - 40, 325);
        HPDF_Page_Stroke (page1);        
        
        /*Between the Table Lines*/
        
    for (i = 0; i <= 3; i++) {			//Radius Row division into two sections OD
       int y = i * 3*CELL_HEIGHT + 69;
        HPDF_Page_MoveTo (page1, col[1], y);
        HPDF_Page_LineTo (page1, col[2], y);
        HPDF_Page_Stroke (page1);
    }
    
    for (i = 0; i <= 3; i++) {			//Radius Row division into two sections OS
       int y = i * 3*CELL_HEIGHT + 69;
        HPDF_Page_MoveTo (page1, col[3], y);
        HPDF_Page_LineTo (page1, col[4], y);
        HPDF_Page_Stroke (page1);
    }
    
    for (i = 0; i <= 10; i++) {			//Time row division into three sections OD
       int y = i*CELL_HEIGHT + 59;
        HPDF_Page_MoveTo (page1,col[2], y);
        HPDF_Page_LineTo (page1, col[3], y);
        HPDF_Page_Stroke (page1);
    }
    
    for (i = 0; i <= 10; i++) {			//Time row division into three sections OS
       int y = i*CELL_HEIGHT + 59;
        HPDF_Page_MoveTo (page1, col[4], y);
        HPDF_Page_LineTo (page1, col[5], y);
        HPDF_Page_Stroke (page1);
    }
    
    /*Writing Baseline and Stimulus tabs*/
        HPDF_Page_BeginText (page1);	
        HPDF_Page_MoveTextPos (page1, col[2]-10, 337);		//Printing OD
        HPDF_Page_ShowText (page1, "OD");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1, col[4]-10, 337);		//Printing OS
        HPDF_Page_ShowText (page1,"OS");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);				//Printing Baseline
        HPDF_Page_MoveTextPos (page1, col[1]-83, 282);
        HPDF_Page_ShowText (page1,"Baseline");
        HPDF_Page_EndText (page1);
                   
    for (i=0; i<4;i++){
        HPDF_Page_SetFontAndSize (page1, font1, textsize1);	//Printing Stimulus -
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1, col[1]-92, 62+57*i);
        HPDF_Page_ShowText (page1,"Stimulus - ");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);				//Printing stimulus no.
        snprintf(buf, 5, "%X", 4-i);
        HPDF_Page_MoveTextPos (page1, col[1]-22, 62+57*i);
        HPDF_Page_ShowText (page1,buf);
        HPDF_Page_EndText (page1);
        
        HPDF_Page_SetFontAndSize (page1, font1, 10);		//Printing Percentage symbol OD
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1, col[2]-14, 52+57*i);
        HPDF_Page_ShowText (page1,"%");
        HPDF_Page_EndText (page1);
        
        //HPDF_Page_SetFontAndSize (page, font1, 10);
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1, col[4]-14, 52+57*i);	//Printing Percentage symbol OS
        HPDF_Page_ShowText (page1,"%");
        HPDF_Page_EndText (page1);
     }
        HPDF_Page_SetFontAndSize (page1, font1, 9);		//Radius left column OD
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,col[2]-50, 312);
        HPDF_Page_ShowText (page1,"Radius (px)");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);				//Time Left column OD
        HPDF_Page_MoveTextPos (page1,col[3]-50, 312);
        HPDF_Page_ShowText (page1,"Time (ms)");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);			//Radius right column OS
        HPDF_Page_MoveTextPos (page1,col[4]-50, 312);
        HPDF_Page_ShowText (page1,"Radius (px)");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_BeginText (page1);                 //Time right column OS
        HPDF_Page_MoveTextPos (page1,col[5]-50, 312);
        HPDF_Page_ShowText (page1,"Time (ms)");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_SetFontAndSize (page1, font1, 12);  //Printing zero in baseline left OD
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,col[3]-31, 283);
        HPDF_Page_ShowText (page1,"0");
        HPDF_Page_EndText (page1);
        
        HPDF_Page_SetFontAndSize (page1, font1, 12);  //Printing zero in baseline right OS
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,col[5]-31, 283);
        HPDF_Page_ShowText (page1,"0");
        HPDF_Page_EndText (page1);
               
}

int main (int argc, char **argv)
{
char times[100];
HPDF_Image image;
HPDF_Image image1;

time (&rawtime);
timeinfo = localtime (&rawtime);

pdf = HPDF_NewEx (error_handler, NULL, NULL, 0, NULL);
    if (!pdf) {
        printf ("error: cannot create PdfDoc object\n");
        return 1;
    }

    if (setjmp(env)) {
        HPDF_Free (pdf);
        return 1;
    }
    char fname[256];
    HPDF_Font font;

	font = HPDF_GetFont (pdf, "Arial", NULL);
	
	HPDF_Page page0 = HPDF_AddPage (pdf);
        HPDF_Page_SetWidth (page0, PAGE_WIDTH);
        HPDF_Page_SetHeight (page0, PAGE_HEIGHT);

	/*Appending Image*/
	image1 = HPDF_LoadJpegImageFromFile (pdf, "../Data/logo.jpg");
	HPDF_Page_DrawImage (page0, image1, 40,200,380,160);
	
	HPDF_Page_SetFontAndSize (page0, font, 20);
	HPDF_Page_BeginText (page0);
        HPDF_Page_MoveTextPos (page0,50, 170);
        HPDF_Page_ShowText (page0,"Patient's Name   :");
        HPDF_Page_EndText (page0);
        
        HPDF_Page_BeginText (page0);
        HPDF_Page_MoveTextPos (page0,50, 140);
        HPDF_Page_ShowText (page0,"Patient's Age      :");
        HPDF_Page_EndText (page0);
        
        HPDF_Page_BeginText (page0);
        HPDF_Page_MoveTextPos (page0,50, 110);
        HPDF_Page_ShowText (page0,"Patient's Gender :");
        HPDF_Page_EndText (page0);
	
	HPDF_Page page1 = HPDF_AddPage (pdf);
        HPDF_Page_SetWidth (page1, PAGE_WIDTH);
        HPDF_Page_SetHeight (page1, PAGE_HEIGHT);
        
        HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);
        

float min, sumOS=0, sumOD=0;
int mink,minj;
int j=0;
int k=0;
int i=0;
int i1,count,plf1=0;
float t1d,t2d,t3d,t1s,t2s,t3s;
int t=0;
float decOD,decOS;
int heap_alloc;
int window=15;
float base1,base2;
int age;
char gend[10];

ifstream image_file("../Data/radtime.csv", std::ios_base::in); ///reading the input file containing readius vs  time
image_file.seekg(0,std::ios::end); //go to end of file to find size
heap_alloc=image_file.tellg()/2;

ifstream image_file1("../Data/data.csv",std::ios_base::in); //Reading the patient's data file

image_file.seekg(0,std::ios::beg); //go to the beginning of file
input_values1=(float*)malloc(2*heap_alloc); //allocation of memory
input_values2=(float*)malloc(2*heap_alloc); //allocation of memory
diff1=(float*)malloc(2*heap_alloc); //allocation of memory
diff2=(float*)malloc(2*heap_alloc);
pulseOD=(int*)malloc(2*heap_alloc); //allocation of memory
pulseOS=(int*)malloc(2*heap_alloc); //allocation of memory
tim=(float*)malloc(2*heap_alloc);

i=0;
char patdat1[20];
image_file1.seekg(0,std::ios::beg);
image_file1>>patdat;
image_file1>>patdat1;

strcat(patdat," ");
strcat(patdat, patdat1);

image_file1>>age;
image_file1>>gend;

	HPDF_Page_SetFontAndSize (page0, font, 20);
	HPDF_Page_BeginText (page0);
        HPDF_Page_MoveTextPos (page0,220,170);
        HPDF_Page_ShowText (page0,patdat);
        HPDF_Page_EndText (page0);
        
        snprintf(times, 5, "%d",age);
        
        HPDF_Page_BeginText (page0);
        HPDF_Page_MoveTextPos (page0,220,140);
        HPDF_Page_ShowText (page0,times);
        HPDF_Page_EndText (page0);
        
        HPDF_Page_BeginText (page0);
        HPDF_Page_MoveTextPos (page0,220,110);
        HPDF_Page_ShowText (page0,gend);
        HPDF_Page_EndText (page0);
i=0;
while(image_file>>tim[i]){
		image_file>>input_values2[i]; //Data for OD
		//cout<<input_values1[i];
		image_file>>input_values1[i]; //Data for OS
		//cout<<input_values2[i];
		image_file>>pulseOS[i];	     //Pulse OD
		image_file>>pulseOD[i];      //Pulse OS
		//cout<<pulse[i];
		i++;
}

array_size=i;
for (i=0;i<50;i++){
  sumOS=input_values1[i]+sumOS;
  sumOD=input_values2[i]+sumOD;
}
base1=sumOS/50;
base2=sumOD/50;

	HPDF_Page_SetFontAndSize (page1, font, 12);
	snprintf(times, 5, "%F", base1);

	HPDF_Page_BeginText (page1);			//Baseline OS
        HPDF_Page_MoveTextPos (page1,col[4]-40, 283);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);
        
        snprintf(times, 5, "%F", base2);

	HPDF_Page_BeginText (page1);			//Baseline OD
        HPDF_Page_MoveTextPos (page1,col[2]-40, 283);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);
plf1=0;
for(count=0;count<4;count++){
for(i=0;i<array_size;i++){ //Finding when the pulseOS or pulseOD has gone high
  if(pulseOS[plf1] || pulseOD[plf1]){
     t=tim[plf1];
     cout<<t<<"\n";
     j=plf1;
     k=plf1;
     break;
  }plf1++;
}
i1=4;
switch(pulseOD[plf1]){
case 40 : {i1=3;
	   break;}
case 45 : {i1=2;
	   break;}
default : break;
}

switch(pulseOS[plf1]){
case 40 : {i1=1;
	   break;}
case 45 : {i1=0;
	   break;}
default : break;
}

plf1=plf1+10;

/*Finding time T1 and radius for OS*/
for(i=0;i<=window;i++){
  diff1[j]=input_values1[j]-base1;
  if(abs(diff1[j])>=3){
     break;   
  }
  j++;
}
//cout<<j<<"\n";
min=10000;
minj=j;
for (i=0;i<=10;i++){
  if(input_values1[j]<min){
  
    min=input_values1[j];
    minj=j;
    }j++;
}
t1s=tim[minj]-t;

snprintf(times, 5, "%F", t1s);				//Printing T1s

	HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,col[5]-42, 255-i1*57);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);
        
        snprintf(times, 5, "%F", input_values1[minj]);	//Printing radius for stimulus 1 OS

	HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,col[4]-39, 250-i1*57);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);
        	
        decOS=(base1-input_values1[minj])*100/base1;	// Percentage Decrease OS
        snprintf(times, 5, "%F", decOS);
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,col[4]-39, 222-57*i1);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);

/*Finding time T1 for OD*/
for(i=0;i<=window;i++){
  diff2[k]=input_values2[k]-base2;
  if(abs(diff2[k])>=3){
     break;
  }
  k++;
}
//cout<<k<<"\n";
min=10000;
mink=k;
for (i=0;i<=10;i++){
  if(input_values2[k]<min){
    min=input_values2[k];
    mink=k;
    }  k++;
}

t1d=tim[mink]-t;
cout<<t1d<<"\n";
snprintf(times, 5, "%F", t1d);				//Printing t1d
	HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,col[3]-43, 255-i1*57);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);
        
        snprintf(times, 5, "%F", input_values2[mink]);		//Printing radius for stimulus 1 OD

	HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,col[2]-40, 250-i1*57);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);

	decOD=(base2-input_values2[mink])*100/base2;	// Percentage Decrease OD
        snprintf(times, 5, "%F", decOD);
        HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,col[2]-40, 222-i1*57);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);

/*Finding T2 for OS*/
k=mink;
j=minj;

for(i=0;i<=10;i++){			//Finding the section which is less than baseline by 5 OS
  diff1[j]=input_values1[j]-base1;
  if(abs(diff1[j])<=3){
     break;
  }
  j++;
}

t1d=tim[mink];
t1s=tim[minj];
min=0;					//Finding the max value in a window of 10 surrounding that OS
minj=j;
for (i=0;i<=20;i++){
  if(input_values1[j]>min){
    min=input_values1[j];
    minj=j;
    }j++;
}
t2s=tim[minj]-t1s;
cout<<t2s<<"\n";
snprintf(times, 5, "%F", t2s);

	HPDF_Page_BeginText (page1);			//Printing t2s
        HPDF_Page_MoveTextPos (page1,col[5]-42, 235-i1*57);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);

/*Finding T2 for OD*/
for(i=0;i<=20;i++){
  diff2[k]=input_values2[k]-base2;	//Finding the section which is less than baseline by 5 OD
  if(abs(diff2[k])<=4){
     break;
  }
  k++;
}

min=0;
mink=k;
for (i=0;i<=20;i++){
  if(input_values2[k]>min){			//Finding the max value in a window of 10 surrounding that OD
    min=input_values2[k];
    mink=k;
    }k++;
}
	t2d=tim[mink]-t1d;
	cout<<t2d<<"\n";
	snprintf(times, 5, "%F", t2d);				//Printing t2d
	HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,col[3]-43, 235-i1*57);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);
        
/*T3 for OS*/
	t3s=tim[minj]-t;
	snprintf(times, 5, "%F", t3s);				//Printing t3s
	HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,col[5]-42, 215-i1*57);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);
        
/*T3 for OD*/
	t3d=tim[mink]-t;
	snprintf(times, 5, "%F", t3d);				//Printing t3d
	HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,col[3]-43, 215-i1*57);
        HPDF_Page_ShowText (page1,times);
        HPDF_Page_EndText (page1);
}
/*Creating New Page*/
HPDF_Page page2 = HPDF_AddPage (pdf);
HPDF_Page_SetWidth (page2, PAGE_WIDTH);
HPDF_Page_SetHeight (page2, PAGE_HEIGHT);

/*Appending Graph*/
image = HPDF_LoadPngImageFromFile (pdf, "../Data/output.png");
HPDF_Page_DrawImage (page2, image, 30,20,360,360);

/*Saving File in pdf*/
strcpy (fname, argv[0]);
strcat (fname, ".pdf");
    
        HPDF_Page_SetFontAndSize (page1, font, textsize1);
        draw_graph (page1);					//Drawing the Graph

	
	HPDF_Page_SetFontAndSize (page0, font, 10);
	HPDF_Page_BeginText (page0);
        HPDF_Page_MoveTextPos (page0,300, 380);
        HPDF_Page_ShowText (page0,asctime(timeinfo));
        HPDF_Page_EndText (page0);
        
        HPDF_Page_SetFontAndSize (page1, font, 10);
	HPDF_Page_BeginText (page1);
        HPDF_Page_MoveTextPos (page1,300, 380);
        HPDF_Page_ShowText (page1,asctime(timeinfo));
        HPDF_Page_EndText (page1);
        
        HPDF_Page_SetFontAndSize (page2, font, 10);
	HPDF_Page_BeginText (page2);
        HPDF_Page_MoveTextPos (page2,300, 380);
        HPDF_Page_ShowText (page2,asctime(timeinfo));
        HPDF_Page_EndText (page2);


    /* save the document to a file */
    HPDF_SaveToFile (pdf, patdat);

    /* clean up */
    HPDF_Free (pdf);
    image_file.close(); //close input file

    return 0;
}

