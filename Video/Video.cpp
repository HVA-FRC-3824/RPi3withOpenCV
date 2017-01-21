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

using namespace cv;
using namespace std;

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

// Constant value to verify data structures are the same on both this side (client side) and the server side
int STRUCT_VERSION = 0;

// Serializes target attributes by broadcasting UDP packets
void serialize() {
    
    // Insert roborio name here
    const char *host = "roboRIO-3824-FRC.local";
    
    // Data structures to hold host information and the server information
    struct hostent *hp;
    struct sockaddr_in servaddr;
    
    // Creates socket object and see if socket can be created
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("cannot create socket");
    }
    
    // Fill out the server information (address, family, and port)
    memset((char*)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5800);
    
    // Look up the address of the server given its name
    hp = gethostbyname(host);
    if (!hp) {
        fprintf(stderr, "could not obtain address of %s\n", host);
    } else {
        
        if (targetType == 2) {
            bigTargetX = liftX;
        }
        
        // Data structure that contains the values to be sent to server (roborio) via UDP packet
        struct targetInformation {
            short frameNum = htons(frameIndex);
            short targetX = htons((short)bigTargetX);
            short targetY = htons((short)bigTargetY);
            short targetHeight = htons((short)bigTargetHeight);
            short dataStructVersion = htons((short)STRUCT_VERSION);
            char targetNum = (char)targetType;
        } values;
        
        // Put the host's address into the server address structure
        memcpy((void *)&servaddr.sin_addr, hp->h_addr_list[0], hp->h_length);
        
        // Sends the frame number, target type (lift or boiler), target x coordinate, target y coordinate, and target height to the server (roborio)
        if (sendto(fd, &values, sizeof(values), 0, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
            perror("sendto failed");
        }
    }
}

int main() {
    
    // Creates an object that can capture video from web cam
    VideoCapture stream1(0);
    
    // If the video stream cannot be opened, tell user
    if (!stream1.isOpened()) {
        cout << "cannot open camera";
        return 1;
    }
    
    // Raw image from camera
    Mat cameraFrameImage;
    
    // Default scalar constants for program to only detect reflective tape (H, S, V)
//    Scalar MINCOLOR = Scalar(49, 128, 76);
//    Scalar MAXCOLOR = Scalar(78, 255, 255);
    
    // Test scalar constants for program to detect more than just the reflective tape (H, S, V)
     Scalar MINCOLOR = Scalar(20, 20, 20);
     Scalar MAXCOLOR = Scalar(180, 255, 255);
    
    // Image to draw contours on
    Mat contourImg;
    
    // Image to display in window
    Mat outputImg;
    
    // Vectors holding contour and contour attributes
    vector<vector<Point> > inputContours;
    vector<vector<Point> > outputContours;
    vector<Vec4i> hierarchy;
    vector<Point> hull;
    
    int MIN_CONTOUR_WIDTH = 100;
    int MAX_CONTOUR_WIDTH = 1000;
    int MIN_CONTOUR_HEIGHT = 100;
    int MAX_CONTOUR_HEIGHT = 1000;
    int MIN_CONTOUR_AREA = 100;
    int MIN_CONTOUR_PERIMETER = 100;
    int MIN_SOLIDITY = 0;
    int MAX_SOLIDITY = 100;
    int MIN_VERTEX_COUNT = 0;
    int MAX_VERTEX_COUNT = 1000000;
    int MIN_RATIO = 0;
    int MAX_RATIO = 1000;
    
    // Write only file storage system for outputs.xml file, where matrices containing images will be stored
    FileStorage fileStorage("outputs.xml", FileStorage::WRITE);
    
    int frameStorageIndex = 0;
    
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
                                if (contour.size() > MIN_VERTEX_COUNT && contour.size() < MAX_VERTEX_COUNT)	{
                                double ratio = bb.width / bb.height;
                                    if (ratio > MIN_RATIO && ratio < MAX_RATIO) {
                                        outputContours.push_back(contour);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        for (int outputContourIndex = 0; outputContourIndex < outputContours.size(); outputContourIndex++) {
            drawContours(contourImg, outputContours, outputContourIndex, CV_RGB(255, 255, 255), 2, 8, hierarchy, 0, Point() );
        }
        
        // Variables used to identify largest contours
        int largestArea = 0;
        int secondLargestArea = 0;
        int largestContourIndex = 0;
        int secondLargestContourIndex = 0;
        
        // Rectangles that will be wrapped around the pieces of reflective tape
        Rect largestTapeRect;
        Rect secondLargestTapeRect;
        
        // Finds largest contour; makes largestTapeRect only wrap around the largest contour (target)
        for (int contourIndex = 0; contourIndex < outputContours.size(); contourIndex++) {
            double a = contourArea(outputContours[contourIndex], false);
            if (a > largestArea){
                largestArea = a;
                largestContourIndex = contourIndex;
                largestTapeRect = boundingRect( Mat(outputContours[contourIndex]) );
            } else if (a < largestArea && a > secondLargestArea) {
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
        
        smallTargetX = largestTapeRect.x;
        smallTargetY = largestTapeRect.y;
        smallTargetWidth = largestTapeRect.width;
        smallTargetHeight = largestTapeRect.height;
        
        // Dectect to see if the strips of reflective tape are for the boiler or the lift
        double expectedBoilerProportion = 2.0;
        double expectedLiftProportion = 1.0;
        double actualProportion;
        
        if (smallTargetX != 0) {
            actualProportion = bigTargetHeight/smallTargetHeight;
        }
        
        double deviationFromExpectedBoilerProportion = expectedBoilerProportion - actualProportion;
        double deviationFromExpectedLiftProportion = expectedLiftProportion - actualProportion;
        
        if (deviationFromExpectedBoilerProportion > -0.1 && deviationFromExpectedBoilerProportion < 0.1) {
            targetType = 1;
        } else if (deviationFromExpectedLiftProportion > -0.1 && deviationFromExpectedLiftProportion < 0.1) {
            targetType = 2;
            liftX = (bigTargetX + smallTargetX) / 2;
        }
        
        // Displays each image in window
        imshow("outputImage", outputImg);
        
        // Writes every tenth frame that the window is showing to outputs.xml file
        if (frameIndex % 10 == 0) {
            stringstream name;
            name << "outputImage" << frameStorageIndex;
            fileStorage << name.str() << outputImg;
            frameStorageIndex++;
        }
        
        serialize();
        
        frameIndex++;
        
        // Breaks the loop to end the program if the next image does not exist
        if (waitKey(30) >= 0)
            break;
    }
    
    // Releases (ends) the file storage system
    fileStorage.release();
    
    return 0;
}
