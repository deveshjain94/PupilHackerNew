/*#########################################################################################################################################
Pupil Hacker Code
Authors : Suyash Yeotikar, Devesh Jain
Ver 1.0

This code uses OpenCV for pupil detection and keeping track of pupil size while various light stimulus is given sequentually to both the eyes. It then generates an automated report along with a graph for quick interpretation of the data.

The code is divided into different sections:
1. Declaration and Initialisation - Libraries and various variables used are initialised here. 
				    Csv for writing the data and reading patient data is initialised here.
				    Comm Port for Arduino communication is declared here.
				    Signal handler for closing file operations on exiting the program (signalhandler).
				    Debbuging prog - Finding the RGB intensities of a point in the vid frame for debugging is written here.
				    
2. Main Program Segment - Declaring and initialising more variables.
			  Setting up Baud rate and other parameters for serial communication with arduinos.
			  Extracting a single frame for calculating the frameheight/width for initialising the video recorder.
			  Defining the region of interest for both eyes separately.
			  Sending a serial pusle to arduino to start the blink code in the arduino.
			  The loop then starts which gives out the tracked pupil and stores the parameters.
			  
2.1 The while loop - Capturing incoming frames and finding mean and std deviation on the specifed regions of interest. The regions of 	 
                     interest is fixed due to the hardware construction and is the region where one can essentially see the eye in the 
                     camera feed.
                     
                     Calculating the threshold using this mean and std deviation data to make thresholding robust and indiffernt to 
                     changes in light conditions and other external parameters.
                     
                     Thresholding using threshold values in a nested for loop and generating a binary output image with only pupil shown.
                     
		     Find contours (regions of places in the binary image which can be closed and formed into a single object if it has a 
		     proper boundary).
	
		     Selection of the contour by checking whehter it is a circle (pupil) by using the bounding box property.
		     
		     Imposing a red circle around the pupil in the original image frame.
		     
		     Creating windows and displaying each frame. Storing it in the video as well.
		     
		     Calculate the Time taken for each iteration and store it in a csv file along with the obtained radius values. This 
		     storing is done according to the input being recieved about the state of the LEDs. For different Stimuluses it stores 
		     using different flags for distinguishin one from another as it can be used to generate a proper report with synced 
		     data from arduino.
		     
		     Calling the plot.ps postscript and the report.sh shell script for generation of the graph and reports respectively 
		     from the generated csv

Contact for more details : deveshjain94@gmail.com, +91 9553720585
##########################################################################################################################################*/

/*OpenCV Libraries*/
#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <time.h>
#include <fstream>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <cstdlib>
#include <math.h>
#include <ctime>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace std;
using namespace cv;

char patdat[50];
char fname[200];
char patdat1[50];
VideoCapture capture(1);

ifstream infile("../Data/data.csv");		//csv file for reading patient data
ofstream image_file("../Data/radtime.csv"); 	//csv file initialization

int USB = open( "/dev/ttyACM0", O_RDWR| O_NONBLOCK | O_NDELAY );	//Opening communication with the arduino for checking LED status

void signalhandler(int signum){ 					//Signal to close the program
  cout<<"Done Recording"<<endl;
  image_file.close();
  exit(0);
}

//This part of the code gives the RGB value of any point on the video picture along with the pixel location (only used for debugging)
void CallBackFunc(int event, int x, int y, int flags, void* userdata){ //function to fing the BGR intensities of pixels in image

    Mat img = *((Mat*)userdata); 					// 1st cast , then deref

    if  ( event == EVENT_LBUTTONDOWN ){
    
        Vec3b pixel = img.at<Vec3b>(y,x); 				// y,x here !
        cout << "Left button clicked - position (" << x << ", " << y << ")" << endl;
        cout << "color (" << int(pixel[0])  << ", " << int(pixel[1])   << ", " << int(pixel[2])  << ")" << endl;
    }
}

/*Main code segment begins here*/
int main(){
  infile>>patdat;			//reading patients first name 		
  infile>>patdat1;		//reading patients last name
  strcpy(fname, "../Videos/");	//declaring path for storing video
  strcat(fname, patdat);		//appending patients first name on the video
  strcat(fname, " ");		//appending patients name on the video
  strcat(fname, patdat1);		//appending patients last name on the video
  std::vector<double> xVec, yVec; //initialization of vectors
  char key;
  int x,y;
  double area; 			//area of contour
  int isColor = 1;
  int fps = 10;			//FPS of recorded video
  boost::posix_time::ptime t1,t0;	//Time Variable for recording keeping track of time passed since execution of program
  float radius[2]; 		//Radius for the overlay circle

  //Initialising thresholds for each channel. Separate thresholds are initialised for both the eye regions

  int redmax1=120;	//Red threshold for left eye 
  int greenmax1=120;	//Green threshold for left eye
  int bluemax1=120;	//Blue threshold for left eye

  int redmax2=120;	//Red threshold for right eye 
  int greenmax2=120;	//Green threshold for right eye
  int bluemax2=120;	//Blue threshold for right eye

  int greenmin=0;	
  int redmin=0;		//minimum values
  int bluemin=0;

  int count;
  float rad;
  float pi=3.14; 				//pi value
  int j=0; 					//counter for radii		
  char flag[1];
  char uflag=0;
  int n;

  Mat image; 					//Input image matrix object created

  Scalar m1,m2,var1,var2;			//variables defined for finding the threshold value dynamically using mean and std dev.

  capture.set(CV_CAP_PROP_FRAME_WIDTH,1280); 	//set resolution of image
  capture.set(CV_CAP_PROP_FRAME_HEIGHT,960);

  radius[0]=0; 					//intialize radii of pupils
  radius[1]=0;

  /* Error handling in case Arduino does not respond to the serial communication request*/
  if ( USB < 0 ){
    cout << "Error " << errno << " opening " << "/dev/ttyACM0" << ": " << strerror (errno) << endl;
  }

  /*Setting the baud rate and other parameters for serial communication with arduino*/
  struct termios tty;
  memset (&tty, 0, sizeof tty);
  cfsetospeed (&tty, B9600);
  cfsetispeed (&tty, B9600);
  memset (&flag, '\0', sizeof flag);

  /* Setting other Port Stuff for Arduino */
  tty.c_cflag     &=  ~PARENB;        	// Make 8n1
  tty.c_cflag     &=  ~CSTOPB;
  tty.c_cflag     &=  ~CSIZE;
  tty.c_cflag     |=  CS8;
  tty.c_cflag     &=  ~CRTSCTS;       	// no flow control
  tty.c_lflag     =   0;          	// no signaling chars, no echo, no canonical processing
  tty.c_oflag     =   0;                  // no remapping, no delays
  tty.c_cc[VMIN]      =   0;              // read doesn't block
  tty.c_cc[VTIME]     =   5;              // 0.5 seconds read timeout

  tty.c_cflag     |=  CREAD | CLOCAL;     	// turn on READ & ignore ctrl lines
  tty.c_iflag     &=  ~(IXON | IXOFF | IXANY);	// turn off s/w flow ctrl
  tty.c_lflag     &=  ~(ICANON | ECHO | ECHOE | ISIG); // make raw
  tty.c_oflag     &=  ~OPOST;              	// make raw

  /* Flush Port, then applies attributes */
  tcflush( USB, TCIFLUSH );
  if ( tcsetattr ( USB, TCSANOW, &tty ) != 0){
    cout << "Error " << errno << " from tcsetattr" << endl;
  }  

  capture>>image; 			//Extract a frame and store in image matrix

  /*Define Frame Height and Width for the Video record. The frameheight and width should be same as the input images for proper video   
   decoding when being played*/
  int framewidth=image.cols; 		
  int frameheight=image.rows;

  signal(SIGINT,signalhandler); 		//intialize signal handling to end program
  strcat(fname,".avi");			//Video format for storing the video

  /*Define VideoiWriter object for storing the video*/
  VideoWriter video(fname,CV_FOURCC('M','J','P','G'),fps,cvSize(framewidth,frameheight));

  t0 = boost::posix_time::microsec_clock::local_time();	//Getting absolute time at this point and storing in t0

  Rect roi1=Rect(0,250,300,350);	//Region of Interest 1 for Left eye
  Rect roi2=Rect(1000,250,280,350);	//Region of Interest 2 for Right eye

  n = write( USB,"s", sizeof (char) );	//Sends a serial variable 's' while hints the arduino program to start the blink code. This was 					done to sync with the arduino and giving the control of the arduino program in the cpp file here.
  /*Loop for Processing*/
  while(1)
  {
    capture>>image;						//Capture and store frame in image matrix variable
    GaussianBlur(image,image,Size(9,9),0,0,BORDER_DEFAULT);	//Gaussian blur to remove some noise
    Mat roimat1= image(roi1);				//Selecting the left eye region of interest
    Mat roimat2= image(roi2);				//Selecting the right eye region of interest
    meanStdDev(roimat1,m1,var1,Mat());			//Taking mean and std deviation of the left eye region values
    meanStdDev(roimat2,m2,var2,Mat());			//Taking mean and std deviation of the right eye region values

    /*Setting the threshold for left eye using the mean and std deviation. This gives a dynamic value for thresold independ of lighting   
    conditions and whoever wears it (obviously subject to the artificial light conditions of having IR LEDs)*/
    bluemax1=m1[0]-var1[0]-10;				
    greenmax1=m1[1]-var1[1]-10;
    redmax1=m1[2]-var1[2]-10;

    /*Setting the threshold for right eye using the mean and std deviation as above*/
    bluemax2=m2[0]-var2[0]-10;
    greenmax2=m2[1]-var2[1]-10;
    redmax2=m2[2]-var2[2]-10;
  
    Mat output = Mat::zeros(image.size(),CV_8UC1);	//Output Matrix of the same size as input image
    std::vector<std::vector<cv::Point> > contours;	//Defining Contour variable as a vector
	
    /*Thresholding the RGB image and converting into binary image*/
	
    //Pupil 1 (left eye) only region of interest specified by coordinates
    for(y=250;y<600;y++){
  	for(x=0;x<230;x++){
		if(((int)image.at<Vec3b>(y,x)[0]>0)&&((int)image.at<Vec3b>(y,x)[0]<bluemax1)&&((int)image.at<Vec3b>(y,x)[1]>0)&& ((int)image.at<Vec3b>(y,x)[1]<greenmax1)&&((int)image.at<Vec3b>(y,x) [2]>0)&&((int)image.at<Vec3b>(y,x)[2]<redmax1)){
				output.at<uchar>(y,x)=255;	//output binary image
		}
	}
    }

    //Pupil 2 (right eye) only region of interest specified by coordinates
    for(y=250;y<600;y++){
	for(x=1080;x<1280;x++){
		if(((int)image.at<Vec3b>(y,x)[0]>0)&&((int)image.at<Vec3b>(y,x)[0]<bluemax2)&&((int)image.at<Vec3b>(y,x)[1]>0)&&((int)image.at<Vec3b>(y,x)[1]<greenmax2)&&((int)image.at<Vec3b>(y,x) [2]>0)&&((int)image.at<Vec3b>(y,x)[2]<redmax2)){
				output.at<uchar>(y,x)=255;	//output binary image
		}
	}
    }

    findContours(output.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_NONE); //find contours in the binary thresholded image
    for (int i = 0; i < contours.size(); i++){
        area = cv::contourArea(contours[i]);    				//area of contours
        cv::Rect rect = cv::boundingRect(contours[i]);  			//creating bounding box for contour
          
       /*Selecting contours based on area and height-width ratio of bounding box (For deciding whether the connected area is a circle*/
        if (area>=100 && area<=8000 && std::abs(1 - ((double)rect.width /(double)rect.height)) <= 0.3 ){
    	    rad = rect.width/2;		//calculating radius using half the width of the bounding box
       	    radius[j]= (float)sqrt(area/pi); //Calculation of radius based on area of contour, either this radius or above rad can be used
    	  
    	    /*Impose circle of the found radius over the contour*/
            cv::circle(image, cv::Point(rect.x + radius[j], rect.y + radius[j]), radius[j], Scalar(0,0,255), 2);
            j++;
        }
    }
  
    j=0; 				//reset radius array counter
    namedWindow("Input",1);		//Create a image window for displaying the output frames
    imshow("Input",image); 		//BGR image with circle superimposed 
    namedWindow("output",1);		//Create a image window for displaying the output binary frames
    imshow("output",output);  		//Show binary output frame on created window
    setMouseCallback("Input", CallBackFunc, &image);

    video<<image; 				// Save frame in video
    n = read( USB,flag, sizeof flag );	//Reads the LED status to know which LED is ON using serial port

    t1=boost::posix_time::microsec_clock::local_time();	//Gives absolute time at this point and storing in t1
    boost::posix_time::time_duration diff = t1 - t0;	//Taking time difference between the time the code started and this point

    cout << flag[0]<<"\n";			//Outputs the LED state on the terminal for debuggin
 
    /*insert data in CSV file depending upon the character sent in flag by arduino serially (depending on which LED is on) */
 
    if(flag[0]=='0'){	//No LEDs are blinking
  	  //insert the time and radius of pupil in the csv file
   	image_file<<diff.total_milliseconds()<<"\t"<<radius[1]<<"\t"<<radius[0]<<"\t"<<0<<"\t"<<0<<"\n"; 
     }
     else if(flag[0]=='1'){ //Stimulus 1
	image_file<<diff.total_milliseconds()<<"\t"<<radius[1]<<"\t"<<radius[0]<<"\t"<<40<<"\t"<<0<<"\n"; 
     }
     else if(flag[0]=='2'){	//Stimulus 2
	image_file<<diff.total_milliseconds()<<"\t"<<radius[1]<<"\t"<<radius[0]<<"\t"<<0<<"\t"<<40<<"\n"; 
     }
     else if(flag[0]=='3'){ //Stimulus 3
	image_file<<diff.total_milliseconds()<<"\t"<<radius[1]<<"\t"<<radius[0]<<"\t"<<40<<"\t"<<0<<"\n"; 
     }
     else if(flag[0]=='4'){	//Stimulus 4
	image_file<<diff.total_milliseconds()<<"\t"<<radius[1]<<"\t"<<radius[0]<<"\t"<<0<<"\t"<<40<<"\n"; 
     }
     else if(flag[0]=='5'){	//Stimulus 5
	image_file<<diff.total_milliseconds()<<"\t"<<radius[1]<<"\t"<<radius[0]<<"\t"<<40<<"\t"<<0<<"\n"; 
     }
     else if(flag[0]=='6'){	//Stimulus 6
	image_file<<diff.total_milliseconds()<<"\t"<<radius[1]<<"\t"<<radius[0]<<"\t"<<0<<"\t"<<40<<"\n"; 
     }
     else if(flag[0]=='7'){	//Stimulus 7
	image_file<<diff.total_milliseconds()<<"\t"<<radius[1]<<"\t"<<radius[0]<<"\t"<<40<<"\t"<<0<<"\n"; 
     }
     else if(flag[0]=='8'){	//Stimulus 8
	image_file<<diff.total_milliseconds()<<"\t"<<radius[1]<<"\t"<<radius[0]<<"\t"<<0<<"\t"<<40<<"\n"; 
     }
     else{			//No LEDs are ON
	image_file<<diff.total_milliseconds()<<"\t"<<radius[1]<<"\t"<<radius[0]<<"\t"<<0<<"\t"<<0<<"\n"; 
     }
	 
    key = waitKey(100); 	//Capture Keyboard stroke
    if (char(key) == 27){
	  break; 		//If you hit ESC key loop will break and code will terminate
    } 
  }

  image_file.close(); 		//close csv file
  infile.close();	    		//close data file
  system("gnuplot ../plot.ps");	//run the plot.ps postscript for generating the plot from the csv
  system("sh ../report.sh");	//run the report.sh shell script which forms the report using the data generated above

  return 0;
}
