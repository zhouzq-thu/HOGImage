//
//  main.cpp
//  HOGImage
//
//  Created by Zhi-Qiang Zhou on 7/4/19.
//  Copyright © 2019 Zhi-Qiang Zhou. All rights reserved.
//

#include "HOGImage.hpp"

int main(int argc, const char * argv[]) {
    Mat image = imread("images/teaser-original.jpg");
    HOGDescriptor hogDesc(image.size(),
                          Size(20, 20),
                          Size(10, 10),
                          Size(10, 10),
                          9);

    Mat hogImg = HOGImage(image, hogDesc, 3, 2);

    string name = "HOG Image window";
    namedWindow(name);
    imshow(name, hogImg);
    waitKey();
    imwrite("hogImg.jpg", hogImg);
}
