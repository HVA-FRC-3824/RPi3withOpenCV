#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace std;
char key;
int main()
{
	    cvNamedWindow("Camera_Output", 1);    //Create window
		    CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);  //Capture using any camera connected to your system
			    while(1){ //Create infinte loop for live streaming

					        IplImage* frame = cvQueryFrame(capture); //Create image frames from capture
							        cvShowImage("Camera_Output", frame);   //Show image frames on created window
									        key = cvWaitKey(10);     //Capture Keyboard stroke
											        if (char(key) == 27){
														            break;      //If you hit ESC key loop will break.
																	        }
													    }
				    cvReleaseCapture(&capture); //Release capture.
					    cvDestroyWindow("Camera_Output"); //Destroy Window
						    return 0;
}

