//BackgroundSubtraction

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
   
	Mat frame;
	Mat mask;
	VideoCapture cap(0);
	vector<Point> hull;
    Mat prev=Mat::ones(640,480,CV_32FC1);
	if(cap.isOpened() == 0)
	{
		cout<<"\n Camera not initialized";
		return -1;
	}
	char c = '1';
	bool update = true;
	BackgroundSubtractorMOG bog;
	Mat frame2;
    int i=0,j;
    Mat mask2;
	while(c!=27)
	{   
        i++;
		cap >> frame;
		mask=Mat::zeros(frame.size(),frame.type());
		bog( frame, mask, 0);
		
		frame2.create(frame.size(),frame.type());
		
		
		/*
		if(c == 'u' || c == 'U' )
			update = 0;
		else if(c == 'n'|| c == 'N')
			update = 1;
		*/	
		/*
		frame2.create(frame.size(), frame.type());
		frame2=Scalar::all(0);
        mask2 = Mat::zeros(frame.size(),frame.type());
		frame.copyTo(frame2, mask);
        mask.convertTo(mask2, CV_32FC1, 1, 0);
        */
        //cout<<"\n Mask.type"<<(int)mask2.type()<<"\n Mask.size()"<<mask2.size().width<<" "<<mask2.size().height	;
        
        //hull[0]=Point(12,12);
        //if(i>330)
        frame2=Mat::zeros(frame.size(),frame.type());
        frame.copyTo(frame2,mask);
        vector<Point2f> points;
        for(i=0;i<mask.rows;i++)
        {
            for(j=0;j<mask.cols;j++)
            {
                if(mask.at<uchar>(i,j) == (uchar)255)
                {
                    points.push_back(Point((float)i,(float)j));
                }
             }
        }
        //cout<<points;
        if(points.size()!=0)
        {
           // convexHull(points,hull,false, true);
        }
       /*
        if(hull.size()!=0)
        {
             for(i=0;i<hull.size();i++)
             {
                 circle(frame, hull[i], 3, Scalar(255,0,0), 1, 8, 0);
             }
        }
        */
		//imshow("foreground",frame);
        //imshow("Mask2",mask2);	
        
		imshow("Video",frame);
		imshow("Mask",mask);
        imshow("frame2",frame2);
        c=waitKey(33);
        
		
	}
	
	return 0;
}
