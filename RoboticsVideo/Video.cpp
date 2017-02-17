#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>
#include <sys/types.h>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include "cmdProcessor.hpp"

using namespace cv;
using namespace std;

#define TRUE (1)
#define FALSE (!TRUE)

// Values to be sent over network
// See if the target is the boiler, lift, or neither; 1 = boiler 2 = lift 3 = neither
int targetType = 0;

int frameIndex = 0;

int bigTargetX = 0;
int bigTargetY = 0;
int bigTargetWidth = 0;
int bigTargetHeight = 0;

int smallTargetX = 0;
int smallTargetY = 0;
int smallTargetWidth = 0;
int smallTargetHeight = 0;

int liftX = 0;

short frameWidth;
short frameHeight;

#define DO_TRACES 0
// Data structures to hold host information and the server information

const char *host = "roboRIO-3824-FRC.local";    // Insert roborio name here
struct hostent *hp;
struct sockaddr_in servaddr;
int fd;

// Constant value to verify data structures are the same on both this side (client side) and the server side
int STRUCT_VERSION = 0;

void sig_handler(int signo)
{
    cout << "received signal\n";
}


void destructNetwork() {
    close(fd);
}

void initNetwork() {
    
    hp = NULL;

    // Creates socket object and see if socket can be created
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("cannot create socket");
    }
    
    // Fill out the server information (address, family, and port)
    memset((char*)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5800);
    
    // Look up the address of the server given its name
    while(!hp)
    {
        hp = gethostbyname(host);
        if (!hp) {
            fprintf(stderr, "could not obtain address of %s\n", host);
        }
    }
    
    // Put the host's address into the server address structure
    memcpy((void *)&servaddr.sin_addr, hp->h_addr_list[0], hp->h_length);
}

void dump_buf(unsigned char *buf, int len)
{
    unsigned char *b = buf;
    for (int i=0; i<len; i++)
    {
        printf("%02X ", *b++);
    }
    
    printf("\n");
}

// Serializes target attributes by broadcasting UDP packets
void serialize() {
    
    if (!hp) {
        fprintf(stderr, "could not obtain address of %s\n", host);
    } else {
        
        if (targetType == 2) {
            bigTargetX = liftX;
        }
        
        // Data structure that contains the values to be sent to server (roborio) via UDP packet
        struct __attribute__((packed)) targetInformation {
            unsigned char  dataStructVersion = (unsigned char) STRUCT_VERSION;
            unsigned char  targetNum = (unsigned char) targetType;
            unsigned short frameNum = htons((unsigned short) frameIndex);
            unsigned short targetX = htons((unsigned short)bigTargetX);
            unsigned short targetY = htons((unsigned short)bigTargetY);
	    unsigned short targetWidth = htons((short)bigTargetWidth);
            unsigned short targetHeight = htons((short)bigTargetHeight);
            unsigned short width = htons((short) frameWidth);
            unsigned short height = htons((short) frameHeight);
        } values;
        
#if (DO_TRACES == 1)
        dump_buf((unsigned char *) &values, sizeof(values));
#endif
        
        // Sends the frame number, target type (lift or boiler), target x coordinate, target y coordinate, and target height to the server (roborio)
        if (sendto(fd, &values, sizeof(values), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
            perror("sendto failed");
        }
    }
}

int main() {
    char headless;
    char *pDisplay;

    cmdProcessor * cmdServer;
    
    cmdServer = cmdProcessor::getInstance();
    cmdServer->start();
    
    // Creates an object that can capture video from web cam
    cout << "Creating capture stream\n";
    VideoCapture stream1(0);
    
    // If the video stream cannot be opened, tell user
    if (!stream1.isOpened()) {
        cout << "cannot open camera\n";
        return 1;
    }

    // determine if we're running headless or not
    cout << "Attempting to open an output window\n";
    pDisplay = getenv("DISPLAY");
    if(pDisplay != NULL)
    {
	
	cout << "Running with windowing environment\n";
        headless = FALSE;
    }
    else
    {
	cout << "Running HEADLESS\n";
        headless = TRUE;
    }
    cout << "headless test complete\n";
    
    initNetwork();
    
    // Raw image from camera
    Mat cameraFrameImage;
    
    // Default scalar constants for program to only detect reflective tape (H, S, V)
    Scalar MINCOLOR = Scalar(50, 100, 60);
    Scalar MAXCOLOR = Scalar(100, 255, 255);
    
    // Test scalar constants for program to detect more than just the reflective tape (H, S, V)
//    Scalar MINCOLOR = Scalar(20, 20, 20);
//    Scalar MAXCOLOR = Scalar(180, 255, 255);
    
    // Image to draw contours on
    Mat contourImg;
    
    // Image to display in window
    Mat outputImg;
    
    // Vectors holding contour and contour attributes
    vector<vector<Point> > inputContours;
    vector<vector<Point> > outputContours;
    vector<Vec4i> hierarchy;
    vector<Point> hull;
    
    int MIN_CONTOUR_WIDTH = 0;
    int MAX_CONTOUR_WIDTH = 1000000;
    int MIN_CONTOUR_HEIGHT = 0;
    int MAX_CONTOUR_HEIGHT = 1000000;
    int MIN_CONTOUR_AREA = 0;
    int MIN_CONTOUR_PERIMETER = 0;
    int MIN_SOLIDITY = 0;
    int MAX_SOLIDITY = 1000000;
    int MIN_VERTEX_COUNT = 0;
    int MAX_VERTEX_COUNT = 1000000;

    // Write only file storage system for outputs.xml file, where matrices containing images will be stored
//    FileStorage fileStorage("outputs.xml", FileStorage::WRITE);
    
//    int frameStorageIndex = 0;

    //----------------------------------
    // Get one frame from the camera so we can get information about the frame
    // that only needs to be retrieved one time
    stream1.read(cameraFrameImage);
    
    // Get the frame size
    frameWidth = (short) stream1.get(CV_CAP_PROP_FRAME_WIDTH);
    frameHeight = (short) stream1.get(CV_CAP_PROP_FRAME_HEIGHT);
    
    //----------------------------------
    // Displays video feed and writes every tenth frame to outputs.xml
    while (true) {
        // Latest image from video feed
        stream1.read(cameraFrameImage);
        
        // Blurs image from camera to make colors run together
        blur(cameraFrameImage, cameraFrameImage, Size(10,10));
        
        // Converts image from BGR to HSV
        cvtColor(cameraFrameImage, cameraFrameImage, CV_BGR2HSV);
        
        // Makes the output image only show elements that are in between the specified HSV min and max constants
        inRange(cameraFrameImage, MINCOLOR, MAXCOLOR, outputImg);
        
        // Makes the contour image only show elements that are in between the specified HSV min and max constants
        inRange(cameraFrameImage, MINCOLOR, MAXCOLOR, contourImg);
        
        //Finds the contours on the contour image
        findContours(contourImg, inputContours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        
	outputContours.clear();

	for (vector<Point> contour: inputContours) {
	    Rect bb = boundingRect(contour);
            if (bb.width > MIN_CONTOUR_WIDTH && bb.width < MAX_CONTOUR_WIDTH) {
                if (bb.height > MIN_CONTOUR_HEIGHT && bb.height < MAX_CONTOUR_HEIGHT) {
                    double area = contourArea(contour);
                    if (area > MIN_CONTOUR_AREA) {
                        if (arcLength(contour, true) > MIN_CONTOUR_PERIMETER) {
                            convexHull(Mat(contour, true), hull);
                            double solid = 100 * area / contourArea(hull);
                            if (solid > MIN_SOLIDITY && solid < MAX_SOLIDITY) {
                                if (contour.size() > MIN_VERTEX_COUNT && contour.size() < MAX_VERTEX_COUNT) {
				    outputContours.push_back(contour);
                                }
                            }
                        }
                    }
                }
            }
        }


        // Variables used to identify largest contours
        int largestArea = 0;
        int secondLargestArea = 0;
        int largestContourIndex = -1;
        int secondLargestContourIndex = -1;
        
        // Rectangles that will be wrapped around the pieces of reflective tape
        Rect largestTapeRect;
        Rect secondLargestTapeRect;
	
        // Finds largest contour; makes largestTapeRect only wrap around the largest contour (target)
        for (int contourIndex = 0; contourIndex < outputContours.size(); contourIndex++) {
            double a = contourArea(outputContours[contourIndex], false);
            if (a > largestArea) {
                largestArea = a;
                largestContourIndex = contourIndex;
                largestTapeRect = boundingRect( Mat(outputContours[contourIndex]) );
            }
        }
	
	for (int contourIndex = 0; contourIndex < outputContours.size(); contourIndex++) {
	    double a = contourArea(outputContours[contourIndex], false);
	    if (a < largestArea && a > secondLargestArea) {
		secondLargestArea = a;
		secondLargestContourIndex = contourIndex;
		secondLargestTapeRect = boundingRect( Mat(outputContours[contourIndex]) );
	    }
	}
        
        // Displays white rectangle around largest contour (target)
        rectangle(outputImg, largestTapeRect.tl(), largestTapeRect.br(), CV_RGB(255, 255, 255), 2, 8, 0);
        rectangle(outputImg, secondLargestTapeRect.tl(), secondLargestTapeRect.br(), CV_RGB(255, 255, 255), 2, 8, 0);
        
        // Sets global values that will be posted to Network Tables
        bigTargetX = largestTapeRect.x;
        bigTargetY = largestTapeRect.y;
        bigTargetWidth = largestTapeRect.width;
        bigTargetHeight = largestTapeRect.height;
        
        smallTargetX = secondLargestTapeRect.x;
        smallTargetY = secondLargestTapeRect.y;
        smallTargetWidth = secondLargestTapeRect.width;
        smallTargetHeight = secondLargestTapeRect.height;
        
        // Dectect to see if the strips of reflective tape are for the boiler or the lift
        double expectedBoilerProportion = 2.0;
        double expectedLiftProportion = 1.0;
        double actualProportion = 0;
        
        if (smallTargetHeight != 0) {
            actualProportion = (double)bigTargetHeight/(double)smallTargetHeight;
        }
        
        double deviationFromExpectedBoilerProportion = expectedBoilerProportion - actualProportion;
        double deviationFromExpectedLiftProportion = expectedLiftProportion - actualProportion;
        
	targetType = 0;
	
        if (deviationFromExpectedBoilerProportion > -0.1 && deviationFromExpectedBoilerProportion < 0.1) {
            targetType = 1;
        } else if (deviationFromExpectedLiftProportion > -0.1 && deviationFromExpectedLiftProportion < 0.1) {
            targetType = 2;
            liftX = (bigTargetX + smallTargetX) / 2;
        } else {
	    targetType = 0;
	}

        // If we're running with windowing system active, displays each image in window
	if(!headless)
	{
            imshow("outputImage", outputImg);
	}
        
        // Writes every tenth frame that the window is showing to outputs.xml file
//        if (frameIndex % 10 == 0) {
//            stringstream name;
//            name << "outputImage" << frameStorageIndex;
//            fileStorage << name.str() << outputImg;
//            frameStorageIndex++;
//        }
        
        serialize();
        
        frameIndex++;
        
        // Breaks the loop to end the program if the next image does not exist
        waitKey(30);
    }

//    fileStorage.release();
    
    return 0;
}
