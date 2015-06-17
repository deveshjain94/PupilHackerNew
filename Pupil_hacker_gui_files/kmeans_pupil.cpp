//faults- number of clusers not independent; value of threshold; dimensions for cropping

#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <fstream>
#include <string.h>
#include <highgui.h>
using namespace std;
using namespace cv;

char patdat[50], patdat1[50], fname[100];
ifstream infile("../Data/data.csv");		//Takes input from Pupil_Hacker_Gui for reading patient data and stores in the csv file	 


void video_rec(char fname[100])
{
	
	int fps=30;
	
	
	VideoCapture cap(1); // open the video camera on the device
	Mat image;
	cap>>image;
	int framewidth=image.cols; 		
	int frameheight=image.rows;
	VideoWriter video(fname,CV_FOURCC('M','J','P','G'),fps,cvSize(framewidth,frameheight));  //CV_FOURCC('M','J','P','G') is a motion-jpeg codec

	while(1)
	{
	 if (!cap.isOpened())  // if not success, exit program
    {
        cout << "ERROR: Cannot open the video file" << endl;
       
    }
cap>>image;
	

	  /*Define Frame Height and Width for the Video record. The frameheight and width should be same as the input images for proper video   
   decoding when being played*/
	

	/*Define VideoiWriter object for storing the video*/
    
	 video<<image; 
	namedWindow("Input",WINDOW_NORMAL);		//Create a image window for displaying the output frames
        imshow("Input",image); 		//Live feed

	    char key;
	    key = waitKey(1); 	//Capture Keyboard stroke
            if (char(key) == 27)
		{
		  break; 		//If you hit ESC key loop will break and code will terminate
		}
	}

}

int main()
{
infile>>patdat;			//reading patients first name
		
infile>>patdat1;		//reading patients last name
strcpy(fname, "../Videos/Raw/");	//declaring path for storing video
  	strcat(fname, patdat);		//appending patients first name on the video
  	strcat(fname, " ");		//appending patients name on the video
  	strcat(fname, patdat1);		//appending patients last name on the video
	strcat(fname,"_raw");
	strcat(fname,".avi");
	
video_rec(fname);
double radius, pi=3.14;
double len, area,p;
char key;
int attempts=15;
int histSize = 256;		//no of bins in the histogram
float range[] = { 0, 255 } ;	//range of intensity values
const float* histRange = { range };
int hist_w = 512; int hist_h = 400;	//width and height of window to display the histogram
int bin_w = cvRound( (double) hist_w/histSize );	//bin_w=2
Mat image_RGB, image, image_gray, grad_x, grad_y, grad, labels, centers, hist, histImage, edge;
vector<Point> approx;
RotatedRect temp;


ofstream image_file("../Data/radtime.csv"); 	//csv file initialization. Data obtained from the image file written onto the csv file
image_file<<"\t"<< "left eye"<< "\n";
VideoCapture capture(fname); // open the saved video
capture>>image_RGB;			//Extract a frame and store in image matrix. 
while(1)
{
	
	capture>>image_RGB;				//Extract a frame and store in image matrix. 
	image_RGB=image_RGB(Rect(0,100,230,230));	//crop left eye in vid_centOn_res
	//GaussianBlur(image_RGB, image,Size(9,9),0);
	cvtColor( image_RGB, image_gray, CV_BGR2GRAY );
	normalize(image_gray, image_gray, 0, 255, NORM_MINMAX);	//Increases contrast in the image by a process called linear stretching
	//Sample matrix as input to kmeans function
	Mat samples(image_RGB.rows * image_RGB.cols, 3, CV_32F);
	//loop reorders the data from the image from a (rows,cols,3) matrix to an (rows*cols,3) matrix (one row per pixel-R,G,B).
  	for( int y = 0; y < image_RGB.rows; y++ )
   	 for( int x = 0; x < image_RGB.cols; x++ )
      	  for( int z = 0; z < 3; z++)
		//samples – Floating-point matrix of input samples, one row per sample
        	samples.at<float>(y + x*image_RGB.rows, z) = image_RGB.at<Vec3b>(y,x)[z];

//kmeans(const Mat& samples,int clusterCount,Mat& labels,TermCriteria termcrit,int attempts, int flags, Mat* centers);
double comp = kmeans(samples,4, labels,TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 1000, 0.0001), attempts, KMEANS_PP_CENTERS,centers);
//cout<<"compactness of the result is: "<<comp<<endl;


	// The loop replaces each pixel in the image with the corresponding cluster center for visualization
	Mat new_image( image_RGB.size(), image_RGB.type() );
  	for( int y = 0; y < image_RGB.rows; y++ )
    	 for( int x = 0; x < image_RGB.cols; x++ )
   	 { 
      		int cluster_idx = labels.at<int>(y + x*image_RGB.rows,0);
      		new_image.at<Vec3b>(y,x)[0] = centers.at<float>(cluster_idx, 0);
      		new_image.at<Vec3b>(y,x)[1] = centers.at<float>(cluster_idx, 1);
     		new_image.at<Vec3b>(y,x)[2] = centers.at<float>(cluster_idx, 2);
    	}

	//{
	cvtColor( new_image, new_image, CV_BGR2GRAY );
	threshold(new_image,edge,100, 255, THRESH_BINARY_INV); //Using an opencv function to threshold the image
	//}
	//Canny(new_image, edge, 80,60,5, true); 
	
	Mat drawing = image_RGB.clone();
	vector<vector<Point> > contours;	//Defining Contour variable as a vector-list of list of points
	findContours(edge.clone(), contours, CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
	for( int i=0; i<contours.size(); i++)
	{
		len = arcLength(contours[i], true); 
		area=contourArea(contours[i], false);	
		if(  len>100  and len <170 and area>600 and area< 1600)
		{
		approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
		if(approx.size()>=8 && approx.size()< 15)
		{
		radius= area/pi;
		radius= sqrt(radius);
		cout<< "radius : "<< (radius*2.54*10)/96<<" mm "<<endl;
		p=(radius*2.54*10)/96;
		image_file<<"\t"<<p<<"\n"; 
		temp = fitEllipse(Mat(contours[i]));
		//drawContours( drawing, contours, i, (0,0,255) , 1 );
		ellipse(drawing, temp, Scalar(0,0,255), 1, 8);
		}
		
		}
	}

	//For saving processed video
	int fps=30;
	char fname1[100];
	strcpy(fname1, "../Videos/Processed/");	//declaring path for storing video
  	strcat(fname1, patdat);		//appending patients first name on the video
  	strcat(fname1, " ");		//appending patients name on the video
  	strcat(fname1, patdat1);		//appending patients last name on the video
	strcat(fname1,"_processed");
	strcat(fname1,".avi");	
	int framewidth=image_RGB.cols; 		
	int frameheight=image_RGB.rows;
	VideoWriter video(fname1,CV_FOURCC('M','J','P','G'),fps,cvSize(framewidth,frameheight));  //CV_FOURCC('M','J','P','G') is a motion-jpeg codec
	video<<drawing;

	

	namedWindow("label",WINDOW_NORMAL);		
        imshow("label",new_image); 
	namedWindow("binary",WINDOW_NORMAL);		
        imshow("binary",edge); 
	namedWindow("contours",WINDOW_NORMAL);		
        imshow("contours",drawing); 

	key = waitKey(100); 	//Capture Keyboard stroke
    	if (char(key) == 27)
	{
	        break; 		//If you hit ESC key loop will break and code will terminate

	}
	
}
image_file.close(); 		//close csv file
infile.close();	    		//close data file
}
