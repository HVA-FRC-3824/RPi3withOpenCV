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

// Values to be posted to Network Tables

// See if the target is the boiler, lift, or neither; 1 = boiler 2 = lift 3 = neither
int targetType = 0;

int frameIndex = 0;

double bigTargetX = 0;
double bigTargetY = 0;
double bigTargetWidth = 0;
double bigTargetHeight = 0;

double smallTargetX = 0;
double smallTargetY = 0;
double smallTargetWidth = 0;
double smallTargetHeight = 0;

double liftX = 0;

double bigTargetXDec = 0.0;
double bigTargetYDec = 0.0;
double bigTargetHeightDec = 0.0;

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
        
        bigTargetXDec = (bigTargetX - (int)bigTargetX) * 10;
        bigTargetYDec = (bigTargetY - (int)bigTargetY) * 10;
        bigTargetHeightDec = (bigTargetHeight - (int)bigTargetHeight) * 10;
        
        // Data structure that contains the values to be sent to server (roborio) via UDP packet
        struct targetInformation {
            short frameNum = htons(frameIndex);
            short targetXInt = htons((short)bigTargetX);
            short targetYInt = htons((short)bigTargetY);
            short targetHeightInt = htons((short)bigTargetHeight);
            short targetXDecimal = htons((short)bigTargetXDec);
            short targetYDecimal = htons((short)bigTargetYDec);
            short targetHeightDecimal = htons((short)bigTargetHeightDec);
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
    
    // Vector holding contours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    // Variables used to identify largest contours
    int largestArea = 0;
    int secondLargestArea = 0;
    int largestContourIndex = 0;
    int secondLargestContourIndex = 0;
    
    // Rectangles that will be wrapped around the pieces of reflective tape
    Rect largestTapeRect;
    Rect secondLargestTapeRect;
    
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
        findContours(contourImg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
        
        // Finds largest contour; makes largestTapeRect only wrap around the largest contour (target)
        for (int contourIndex = 0; contourIndex < contours.size(); contourIndex++) {
            double a = contourArea(contours[contourIndex], false);
            if (a > largestArea){
                largestArea = a;
                largestContourIndex = contourIndex;
                largestTapeRect = boundingRect( Mat(contours[contourIndex]) );
            } else if (a < largestArea && a > secondLargestArea) {
                secondLargestArea = a;
                secondLargestContourIndex = contourIndex;
                secondLargestTapeRect = boundingRect( Mat(contours[contourIndex]) );
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
        double actualProportion = bigTargetHeight/smallTargetHeight;
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
