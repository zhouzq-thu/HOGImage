//
//  HOGImage.hpp
//  HOGImage
//
//  Created by Zhi-Qiang Zhou on 7/4/19.
//  Copyright © 2019 Zhi-Qiang Zhou. All rights reserved.
//

#ifndef HOGImage_hpp
#define HOGImage_hpp

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/**
 HOG visualization.

 @param srcImg input image, usually the window of HOG
 @param hog an instance of cv::HOGDescriptor
 @param imgScale image scale
 @param vecScale scale for lines to plot
 @param drawRect specify draw rectangles or not
 @return HOG image
 */
Mat HOGImage(Mat srcImg, HOGDescriptor &hog,
             int imgScale = 1, float vecScale = 1.0,
             bool drawRect = true);

#endif /* HOGImage_hpp */
