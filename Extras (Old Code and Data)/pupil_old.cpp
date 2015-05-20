//OpenCV Libraries
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
//#include "gnuplot_i.hpp"
using namespace std;
using namespace cv;


VideoCapture capture(1); //initialization of camerad device object

ofstream image_file("../Data/radtime.csv"); //csv file initialization
int USB = open( "/dev/ttyACM0", O_RDWR| O_NONBLOCK | O_NDELAY );

void signalhandler(int signum) //signal to close the program
{
cout<<"Done Recording"<<endl;
image_file.close();
exit(0);
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata) //function to fing the BGR intensities of pixels in image
{
    Mat img = *((Mat*)userdata); // 1st cast , then deref

    if  ( event == EVENT_LBUTTONDOWN )
    {
        Vec3b pixel = img.at<Vec3b>(y,x); // y,x here !
        cout << "Left button clicked - position (" << x << ", " << y << ")" << endl;
        cout << "color (" << int(pixel[0])  << ", " << int(pixel[1])   << ", " << int(pixel[2])  << ")" << endl;
    }
}

int main()
{
//Gnuplot g1("lines");
std::vector<double> xVec, yVec; //initialization of vectors
char key;
int x,y;
double area; 			//area of contour
int isColor = 1;
int fps = 10;			//FPS of recorded video
boost::posix_time::ptime t1,t0;		  //Time Variable
float radius[2]; 		//Radius for the overlay circle
//thresholds for each channel
int redmax1=120;
int redmin=0;
int greenmax1=120;
int greenmin=0;
int bluemax1=120;
int bluemin=0;
int redmax2=120;
int greenmax2=120;
int bluemax2=120;
int bluemax3;
int count;
int xcentre;
int ycentre;
float rad;
float pi=3.14; 		//pi value
int j=0; 		//counter for radii
Mat image; 		//Input image matrix
char flag[1];
char uflag=0;
int n;
int mom;
Scalar m1,m2,var1,var2;
capture.set(CV_CAP_PROP_FRAME_WIDTH,1280); 	//set resolution of image
capture.set(CV_CAP_PROP_FRAME_HEIGHT,960);
radius[0]=0; 					//intialize radii of pupils
radius[1]=0;
//VideoCapture capture(0); //Capture using any camera connected to your system
if ( USB < 0 )
{
cout << "Error " << errno << " opening " << "/dev/ttyACM0" << ": " << strerror (errno) << endl;
}
struct termios tty;
memset (&tty, 0, sizeof tty);
cfsetospeed (&tty, B9600);
cfsetispeed (&tty, B9600);
memset (&flag, '\0', sizeof flag);

/* Setting other Port Stuff */
tty.c_cflag     &=  ~PARENB;        	// Make 8n1
tty.c_cflag     &=  ~CSTOPB;
tty.c_cflag     &=  ~CSIZE;
tty.c_cflag     |=  CS8;
tty.c_cflag     &=  ~CRTSCTS;       	// no flow control
tty.c_lflag     =   0;          	// no signaling chars, no echo, no canonical processing
tty.c_oflag     =   0;                  // no remapping, no delays
tty.c_cc[VMIN]      =   0;              // read doesn't block
tty.c_cc[VTIME]     =   5;              // 0.5 seconds read timeout

tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
tty.c_iflag     &=  ~(IXON | IXOFF | IXANY);// turn off s/w flow ctrl
tty.c_lflag     &=  ~(ICANON | ECHO | ECHOE | ISIG); // make raw
tty.c_oflag     &=  ~OPOST;              // make raw

/* Flush Port, then applies attributes */
tcflush( USB, TCIFLUSH );
if ( tcsetattr ( USB, TCSANOW, &tty ) != 0)
{
cout << "Error " << errno << " from tcsetattr" << endl;
}
capture>>image; 			//capture an image
//Extract a frame and store in the 'image' matrix
//Define Frame Height and Width
int framewidth=image.cols; 		//measure image size
int frameheight=image.rows;
signal(SIGINT,signalhandler); 		//intialize signal handling to end program

//Define VideoiWriter object for storing the video
VideoWriter video("capture.avi",CV_FOURCC('M','J','P','G'),fps,cvSize(framewidth,frameheight));
//initializing csv file for plotting data
//measuring time T-Zero
t0 = boost::posix_time::microsec_clock::local_time();
//Mat roiMat;
//Loop for Processing
Rect roi1=Rect(100,100,250,500);
Rect roi2=Rect(830,100,250,500);

n = write( USB,"s", sizeof (char) );

while(1)
{
capture>>image;
GaussianBlur(image,image,Size(9,9),0,0,BORDER_DEFAULT);
Mat roimat1= image(roi1);
Mat roimat2=image(roi2);
meanStdDev(roimat1,m1,var1,Mat());
meanStdDev(roimat2,m2,var2,Mat());
bluemax1=m1[0]-var1[0]+15;
greenmax1=m1[1]-var1[1]+15;
redmax1=m1[2]-var1[2]+15;
bluemax2=m2[0]-var2[0];
greenmax2=m2[1]-var2[1];
redmax2=m2[2]-var2[2];
//cout<<"blue"<<bluemax2<<"\n";
//cout<<"green"<<greenmax2<<"\n";
//cout<<"red"<<redmax2<<"\n";
/*if(bluemax1>180)
{
bluemax1=m1[0]-var1[0]-15;
greenmax1=m1[1]-var1[1]-15;
redmax1=m1[2]-var1[2]-15;
}*/
/*if(bluemax2>180)
{
bluemax2=m2[0]-var2[0]-15;
greenmax2=m2[1]-var2[1]-15;
redmax2=m2[2]-var2[2]-15;
}*/

	 //Create image frames from capture
	Mat output = Mat::zeros(image.size(),CV_8UC1);	//Output Matrix of the same size as input image
	std::vector<std::vector<cv::Point> > contours;	//Defining Contour variable as a vector
//Thresholding the RGB image and converting into binary image
//pupil 1 only region of interest
	for(y=100;y<400;y++)
	{
		for(x=200;x<400;x++)
		{
			if(((int)image.at<Vec3b>(y,x)[0]>0)&&((int)image.at<Vec3b>(y,x)[0]<bluemax1)&&((int)image.at<Vec3b>(y,x)[1]>0)&&((int)image.at<Vec3b>(y,x)[1]<greenmax1)&&((int)image.at<Vec3b>(y,x) [2]>0)&&((int)image.at<Vec3b>(y,x)[2]<redmax1))
			{
				output.at<uchar>(y,x)=255;
			}
		}
	}
	//pupil 2 only region of interest
	for(y=100;y<450;y++)
	{
		for(x=930;x<1180;x++)
		{
			if(((int)image.at<Vec3b>(y,x)[0]>0)&&((int)image.at<Vec3b>(y,x)[0]<bluemax2)&&((int)image.at<Vec3b>(y,x)[1]>0)&&((int)image.at<Vec3b>(y,x)[1]<greenmax2)&&((int)image.at<Vec3b>(y,x) [2]>0)&&((int)image.at<Vec3b>(y,x)[2]<redmax2))
			{
				output.at<uchar>(y,x)=255;
			}
		}
	}
findContours(output.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_NONE); //find contours
	for (int i = 0; i < contours.size(); i++)
	{
    area = cv::contourArea(contours[i]);    				//area of contours
    cv::Rect rect = cv::boundingRect(contours[i]);  			//creating bounding box for contour
                        						//radius of circle
     //selecting contours based on area and height-width ratio of bounding box
    	if (area>=100 && area<=8000 && std::abs(1 - ((double)rect.width /(double)rect.height)) <= 0.25 )
    	{
    		//cout<<area<<"\n";
    		rad= rect.width/2;
    		 sqrt(area)/rad;
    		radius[j]= (float)sqrt(area/pi); //Calculation of radius based on area of contour
      	//impose circle of the found radius over the contour
        	cv::circle(image, cv::Point(rect.x + radius[j], rect.y + radius[j]), radius[j], Scalar(0,0,255), 2);
        	j++;
        //cout<<j;
    	}
	 }

namedWindow("Input",1);
imshow("Input",image); //BGR image
namedWindow("output",1);
imshow("output",output);  //Show image frames on created window
setMouseCallback("Input", CallBackFunc, &image);
j=0; //reset radius array counter
//xVec.push_back((float)t1);
//yVec.push_back((radius));
//g = Gnuplot::
/*g1.reset_plot();
g1.set_grid();
g1.set_style("points").plot_xy(x,y,"user-defined points 2d");*/

video<<image; // Save frame in video
n = read( USB,flag, sizeof flag );
//cout<<n<<"\n";
/* Error Handling */
t1=boost::posix_time::microsec_clock::local_time();
boost::posix_time::time_duration diff = t1 - t0;
//cout<<td<<" "<<CLOCKS_PER_SEC<<"\n";
 cout << flag[0]<<"\n";
   if(flag[0]=='0')
   {
   	image_file<<diff.total_milliseconds()<<"\t"<<radius[1]<<"\t"<<radius[0]<<"\t"<<0<<"\t"<<0<<"\n"; //insert the time and radius of pupil in the csv file
	 }
	 else if(flag[0]=='1')
	 {
	 	image_file<<diff.total_milliseconds()<<"\t"<<radius[1]<<"\t"<<radius[0]<<"\t"<<40<<"\t"<<0<<"\n"; //insert the time and radius of pupil in the csv file
	 }
	 else if(flag[0]=='2')
	 {
	 	image_file<<diff.total_milliseconds()<<"\t"<<radius[1]<<"\t"<<radius[0]<<"\t"<<0<<"\t"<<40<<"\n"; //insert the time and radius of pupil in the csv file
	 }
	 else if(flag[0]=='3')
	 {
	 	image_file<<diff.total_milliseconds()<<"\t"<<radius[1]<<"\t"<<radius[0]<<"\t"<<40<<"\t"<<0<<"\n"; //insert the time and radius of pupil in the csv file
	 }
	 else if(flag[0]=='4')
	 {
	 	image_file<<diff.total_milliseconds()<<"\t"<<radius[1]<<"\t"<<radius[0]<<"\t"<<0<<"\t"<<40<<"\n"; //insert the time and radius of pupil in the csv file
	 }
	 else if(flag[0]=='5')
	 {
	 	image_file<<diff.total_milliseconds()<<"\t"<<radius[1]<<"\t"<<radius[0]<<"\t"<<40<<"\t"<<0<<"\n"; //insert the time and radius of pupil in the csv file
	 }
	 else if(flag[0]=='6')
	 {
	 	image_file<<diff.total_milliseconds()<<"\t"<<radius[1]<<"\t"<<radius[0]<<"\t"<<0<<"\t"<<40<<"\n"; //insert the time and radius of pupil in the csv file
	 }
	 else if(flag[0]=='7')
	 {
	 	image_file<<diff.total_milliseconds()<<"\t"<<radius[1]<<"\t"<<radius[0]<<"\t"<<40<<"\t"<<0<<"\n"; //insert the time and radius of pupil in the csv file
	 }
	 else if(flag[0]=='8')
	 {
	 	image_file<<diff.total_milliseconds()<<"\t"<<radius[1]<<"\t"<<radius[0]<<"\t"<<0<<"\t"<<40<<"\n"; //insert the time and radius of pupil in the csv file
	 }
	 else
	 {
	 	image_file<<diff.total_milliseconds()<<"\t"<<radius[1]<<"\t"<<radius[0]<<"\t"<<0<<"\t"<<0<<"\n"; //insert the time and radius of pupil in the csv file
	 }
key = waitKey(100); //Capture Keyboard stroke
	if (char(key) == 27)
	{
	
		break; //If you hit ESC key loop will break.
	} 
//capture.release();

}
image_file.close(); //close csv file
image_file.close();
system("gnuplot ../plot.ps");
system("sh ../report.sh");
 //close csv file
return 0;
}
