//
//  main.cpp
//  HOGImage
//
//  Created by Zhi-Qiang Zhou on 7/4/19.
//  Copyright © 2019 Zhi-Qiang Zhou. All rights reserved.
//

#include <iostream>

#include "HOGImage.hpp"

int main(int argc, const char * argv[]) {
    Mat image = imread("images/teaser-original.jpg");
    HOGDescriptor hogDesc(image.size(),
                          Size(20, 20),
                          Size(10, 10),
                          Size(10, 10),
                          9);

    vector<float> desc;
    string name = "HOG window";
    namedWindow(name);
    imshow(name, HOGImage(image, hogDesc, 5, 3));
    waitKey();
}
