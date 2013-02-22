//Color Predicates

#include<cv.h>
#include<highgui.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#include"/home/ameya/cvblob/cvBlob/cvblob.h"

using namespace cvb;
using namespace std;

IplImage *img, *imghsv, *imgthresh, *labels, *imglab, *scribble, *imgthresh_median, *img_cam;
int h, s, v, l, l_track = 190, min_area = 6000, store_position[100][2] = {0}, vol_test=100, line_length;
static int posX=0, posY=0, lastX=0, lastY=0;
unsigned int posID=0, lastID=0; 
double slope;

CvMemStorage* storage = cvCreateMemStorage(0);
CvSeq* lines = 0;

int flag=0;
int main()
{
	FILE *fp = fopen("cp3.bin", "r");
	int i,j, cp[256][256];
	for(i=0;i<256;i++)
		fread(cp[i], sizeof(int), 256, fp);
	fclose(fp);



	CvCapture* capture = 0;
	capture = cvCaptureFromCAM(0);
	
	CvBlobs blobs;
	
//	cvNamedWindow("Image");
//	cvNamedWindow("thresh");
		
	if(!capture)
	{
		printf("no camera");
		return -1;
	}
	
	CvScalar Data, Data_lab;
	CvTracks hand_track;

	scribble = cvCreateImage(cvSize((int)(640*0.7), (int)(480*0.7)),8,1);
	
while(true)
{	

  img_cam=cvQueryFrame(capture);
	img = cvCreateImage( cvSize((int)((img_cam->width*70)/100) , (int)((img_cam->height*70)/100) ),img_cam->depth, img_cam->nChannels );
	cvResize(img_cam, img);
	imghsv = cvCreateImage(cvGetSize(img),8,3);
	imglab = cvCreateImage(cvGetSize(img),8,3);
	imgthresh=cvCreateImage(cvGetSize(img), 8, 1);
	imgthresh_median=cvCreateImage(cvGetSize(img), 8, 1);
	labels=cvCreateImage(cvGetSize(img), IPL_DEPTH_LABEL, 1);
//	cvCreateTrackbar("l value", "thresh", &l_track, 255, NULL);
//	cvCreateTrackbar("min_area", "thresh", &min_area, 100000, NULL);
//	cvCreateTrackbar("vol", "track", &vol_test, 1024, NULL);
	cvCvtColor(img,imghsv,CV_BGR2HSV);
	cvCvtColor(img,imglab,CV_BGR2Lab);
	for(i=0;i<img->height;i++)
	{
		for(j=0;j<img->width;j++)
		{
			Data = cvGet2D(imghsv, i, j);
			Data_lab = cvGet2D(imglab, i, j);
			l = Data_lab.val[0];
			h = Data.val[0];
			s = Data.val[1];
			
			if(cp[h][s] == 1 && l<l_track)
				cvSet2D(imgthresh, i, j, cvRealScalar(255));
			else
				cvSet2D(imgthresh, i, j, cvScalarAll(0));
		}
	}
	cvSmooth(imgthresh, imgthresh_median, CV_MEDIAN, 9, 9);
	cvDilate(imgthresh_median, imgthresh_median);
	//cvErode(imgthresh_median, imgthresh_median);
//	cvSmooth(imgthresh_median, imgthresh_median, CV_BLUR, 4, 7);
	unsigned int result = cvLabel(imgthresh_median, labels, blobs);
		
//	cvRenderBlobs(labels, blobs, img, img);
		
	cvFilterByArea(blobs, min_area, 500000);
	
	cvUpdateTracks(blobs, hand_track, 200., 5);
	cvRenderTracks(hand_track, img, img, CV_TRACK_RENDER_ID|CV_TRACK_RENDER_BOUNDING_BOX);
		
/*	for(CvBlobs::const_iterator it=blobs.begin(); it!=blobs.end(); ++it)
	{
		double moment10 = it->second->m10;
		double moment01 = it->second->m01;
		double area = it->second->area;
		
		posX = moment10/area;
		posY = moment01/area;
		printf("(%d, %d)\n", posX, posY);
	}
*/	
	for(CvTracks::const_iterator jt=hand_track.begin(); jt!=hand_track.end(); ++jt)
	{

			posX = jt->second->centroid.x;
			posY = jt->second->centroid.y;
			posID = jt->second->id;
			
			if(posX>0 && posY>0 && store_position[posID][0]>0 && store_position[posID][1]>0);
			{
				cvLine(scribble, cvPoint(posX, posY), cvPoint(store_position[posID][0], store_position[posID][1]), cvScalar(255), 10);
			}
			
			printf("(%d, %d)\n", posX, posY);
			store_position[posID][0] = posX;
			store_position[posID][1] = posY;
	}
	
	
/*	if(abs(posX-lastX)>=0 && abs(posY-lastY)>=0 && posX>0 &&posY>0 && lastX>0 && lastY>0);
	{
		cvLine(scribble, cvPoint(posX, posY), cvPoint(lastX, lastY), cvScalar(0,255,255), 5);
	}
	
	lastX = posX;
	lastY = posY;*/
	
	      lines = cvHoughLines2( scribble, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 80, 100, 10);
        for( i = 0; i < lines->total; i++ )
        {
            CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
            line_length=sqrt((line[1].x - line[0].x)*(line[1].x - line[0].x) + (line[1].y - line[0].y)*(line[1].y - line[0].y));
            if(line[1].x != line[0].x)
              slope = abs((line[1].y-line[0].y)/(line[1].x-line[0].x));
            if (line_length>150 && line[0].x!=0 && line[1].x!=444 && abs(line[1].y-line[0].y)<=6 && flag==0)
            {
              system("gnome-terminal -x sh -c \"vlc -I rc c.mp4\"");
              printf("(%d, %d) and (%d, %d) \n", line[0].x, line[0].y, line[1].x, line[1].y);
              printf("detected line length = %d\n", line_length);
              printf( "slope = %lf\n", slope);
              scribble = cvCreateImage(cvSize((int)(640*0.7), (int)(480*0.7)),8,1);
              flag=1;
              sleep(10);
              break;
            }
            if(line_length>50 && line[0].y!=0 && line[1].y!=444 && slope>5.0 && flag==1)
            {
              system("killall vlc");
              printf("(%d, %d) and (%d, %d) \n", line[0].x, line[0].y, line[1].x, line[1].y);
              printf("detected line length = %d\n", line_length);
              printf( "slope = %lf\n", slope);
              scribble = cvCreateImage(cvSize((int)(640*0.7), (int)(480*0.7)),8,1);
              sleep(10);
		          flag=0;
              break;
            }
/*            else if(line_length>=150 && vol_test<1024 && line[0].x!=0 && line[1].x!=444)
            {
              vol_test = vol_test + 40;
              system("notify-send -t 100  \"Ameya\"");
              printf("(%d, %d) and (%d, %d) \n", line[0].x, line[0].y, line[1].x, line[1].y);
              printf("detected line length = %d\n", line_length);
              scribble = cvCreateImage(cvSize((int)(640*0.7), (int)(480*0.7)),8,1);
              sleep(10);
              break;
            }
	    else if(vol_test>1020)
		vol_test=0;*/
        }
	
	
	
	cvShowImage("track", scribble);
	
	cvShowImage("thresh", imgthresh_median);
	cvShowImage("Image", img);
	
	char c = cvWaitKey(15);
	cvReleaseBlobs(blobs);
	cvReleaseTracks(hand_track);
	cvReleaseImage(&imgthresh_median);
	cvReleaseImage(&img);
//	cvReleaseImage(&img_cam);
	cvReleaseImage(&imghsv);
	cvReleaseImage(&imglab);
	if (c==27)
		break;
}
		
	cvReleaseCapture(&capture);
	return 0;
	
}



