//VitualCanvas Logistic Regression

#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <math.h>

using namespace cv;
using namespace std;

int main()
{
	Mat frame;
	VideoCapture cap(1);
	
	if(cap.isOpened() == 0)
	{
		cout<<"\n Camera not initialized";
		return -1;
	}
	int i,j;
	int bgr[3];
	char key='1';
	float value;
    float intensity,i_n,i_e,q;
	while(key!=27)
	{
		cap >> frame;
		//frame = imread("test8d.jpg");
		Mat img_thresh=Mat::zeros(frame.size(), CV_8UC1);
		Mat frame2 = Mat::zeros(frame.size(), CV_8UC3);

        Mat img_hsv;
        
        cvtColor( frame, img_hsv, CV_BGR2HSV );
        /*
        Mat hsv_split[3];
		
		split( img_hsv, hsv_split);
		
		//imshow( " Value channel", value[2]); 
		
		equalizeHist( hsv_split[2], hsv_split[2]);
		
		//imshow("Modified", value[2]);
		
		merge( hsv_split, 3, frame2);
		
		cvtColor(frame2,frame2, CV_HSV2BGR);
		
		//imshow(" Modified2" , frame2);
		* */
		for(i=0;i<frame.rows;i++)
		{
			for(j=0;j<frame.cols;j++)
			{
				bgr[0]=frame.at<Vec3b>(i,j)[0];
				bgr[1]=frame.at<Vec3b>(i,j)[1];
				bgr[2]=frame.at<Vec3b>(i,j)[2];
                
                            				

				value=(-3.934570 * (bgr[0]/130))+(1.168743 * (bgr[1]/130)) + (5.294436 * (bgr[2]/130)) - 4.474383;

                
				
				if(value < 0)
				{
					img_thresh.at<uchar>(i,j) = (uchar)0;
				}
				else
				{
					img_thresh.at<uchar>(i,j)=(uchar)255;
				}
			}
		}
		/*
		medianBlur(img_thresh,img_thresh,7);
        
        dilate(img_thresh,img_thresh, NULL, Point(-1,-1),1, BORDER_CONSTANT,morphologyDefaultBorderValue());
        erode(img_thresh,img_thresh,NULL, Point(-1,-1),1, BORDER_CONSTANT,morphologyDefaultBorderValue());
		*/
		
		
		imshow("video",frame);
		
		imshow("thresh",img_thresh);
		
		
		key=waitKey(33);
	}
	
	return 0;
	
}
		
		
			
		
		
		
	
