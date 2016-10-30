#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    
    // Image the window will display
    Mat imageToOpen;
    
    // Read only file storage system from outputs.xml file, where matrices of images are stored
    FileStorage fs("outputs.xml", FileStorage::READ);
    
    int idx = 0;
    
    // Iterate through written images
    while(true) {
        
        // Makes the image the window will display the correct written image from the array of matrices in outputs.xml (for all the ones that exist)
        if (!fs["outputImage" + to_string(idx)].empty()) {
            fs["outputImage" + to_string(idx)] >> imageToOpen;
        }
        
        // Displays each image in window
        imshow("VideoRecording", imageToOpen);
        
        idx++;
        
        // Breaks the loop to end the method and recording if the next image does not exist
        if (waitKey(30) >= 0) {
            break;
        }
    }
    
    // Releases (ends) the file storage system
    fs.release();
    
    return 0;
}