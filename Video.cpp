#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    
    // Creates an object that can capture video from web cam
    VideoCapture stream1(0);
    
    // If the video stream cannot be opened, tell user
    if (!stream1.isOpened()) {
        cout << "cannot open camera";
    }
    
    // Scalar constants for program to only detect reflective tape
    Scalar MINCOLOR = Scalar(180, 180, 180);
    Scalar MAXCOLOR = Scalar(255, 255, 255);
    
    // Write only file storage system for outputs.xml file, where matrices of images will be stored
    FileStorage fileStorage("outputs.xml", FileStorage::WRITE);
    
    int index = 0;
    int i = 0;
    
    // Displays video feed and writes every tenth frame to outputs.xml
    while (true) {
        // Latest image from vido feed
        Mat cameraFrameImage;
        stream1.read(cameraFrameImage);
        
        // Image to display in window
        Mat outputImage;
        
        // Makes the output image only include reflective tape
        inRange(cameraFrameImage, MINCOLOR, MAXCOLOR, outputImage);
        
        // Displays each image in window
        imshow("outputImage", outputImage);
        
        // Writes every tenth frame that the window is showing to outputs.xml file
        if (index % 10 == 0) {
            fileStorage << "outputImage" + to_string(i) << outputImage;
            i++;
        }
        
        index++;
        
        // Breaks the loop to end the program if the next image does not exist
        if (waitKey(30) >= 0)
            break;
    }
    
    // Releases (ends) the file storage system
    fileStorage.release();
    
    return 0;
}