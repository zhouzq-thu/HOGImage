//
//  HOGImage.cpp
//  HOGImage
//
//  Created by Zhi-Qiang Zhou on 7/4/19.
//  Copyright © 2019 Zhi-Qiang Zhou. All rights reserved.
//
//  Ref: http://www.juergenbrauer.org/old_wiki/doku.php?id=public:hog_descriptor_computation_and_visualization

#include "HOGImage.hpp"

Mat HOGImage(Mat srcImg, HOGDescriptor &hog,
             int imgScale, float vecScale,
             bool drawRect)
{
    Mat hogImg;
    resize(srcImg, hogImg, Size(srcImg.cols * imgScale, srcImg.rows * imgScale));

    if (srcImg.channels() == 1) {
        cvtColor(hogImg, hogImg, COLOR_GRAY2BGR);
    } else {
        cvtColor(srcImg, srcImg, COLOR_BGR2GRAY);
    }


    // compute HOG
    vector<float> descriptors;
    hog.compute(srcImg, descriptors);

    /* Ref: Fast Calculation of Histogram of Oriented Gradient Feature
     *      by Removing Redundancy in Overlapping Block
     */
    // count in the window
    int numCellsX = hog.winSize.width / hog.cellSize.width;
    int numCellsY = hog.winSize.height / hog.cellSize.height;
    int numBlocksX = (hog.winSize.width - hog.blockSize.width
                      + hog.blockStride.width) / hog.blockStride.width;
    int numBlocksY = (hog.winSize.height - hog.blockSize.height
                      + hog.blockStride.height) / hog.blockStride.height;

    // count in the block
    int numCellsInBlockX = hog.blockSize.width / hog.cellSize.width;
    int numCellsInBlockY = hog.blockSize.height / hog.cellSize.height;

    int sizeGrads[] = {numCellsY, numCellsX, hog.nbins};
    Mat gradStrengths(3, sizeGrads, CV_32F, Scalar(0));
    Mat cellUpdateCounter(numCellsY, numCellsX, CV_32S, Scalar(0));

    float *desPtr = &descriptors[0];
    for (int bx = 0; bx < numBlocksX; bx++) {
        for (int by = 0; by < numBlocksY; by++) {
            for (int cx = 0; cx < numCellsInBlockX; cx++) {
                for (int cy = 0; cy < numCellsInBlockY; cy++) {
                    int cellX = bx + cx;
                    int cellY = by + cy;
                    int *cntPtr = &cellUpdateCounter.at<int>(cellY, cellX);
                    float *gradPtr = &gradStrengths.at<float>(cellY, cellX, 0);
                    (*cntPtr)++;
                    for (int bin = 0; bin < hog.nbins; bin++) {
                        float *ptr = gradPtr + bin;
                        *ptr = (*ptr * (*cntPtr - 1) + *(desPtr++)) / (*cntPtr);
                    }
                }
            }
        }
    }

    const float PI = 3.1415927;
    const float radRangePerBin = PI / hog.nbins;
    const float maxVecLen = min(hog.cellSize.width,
                                hog.cellSize.height) / 2  * vecScale;

    for (int cellX = 0; cellX < numCellsX; cellX++) {
        for (int cellY = 0; cellY < numCellsY; cellY++) {
            Point2f ptTopLeft = Point2f(cellX * hog.cellSize.width,
                                        cellY * hog.cellSize.height);
            Point2f ptCenter = ptTopLeft + Point2f(hog.cellSize) / 2;
            Point2f ptBottomRight = ptTopLeft + Point2f(hog.cellSize);

            if (drawRect) {
                rectangle(hogImg,
                          ptTopLeft * imgScale,
                          ptBottomRight * imgScale,
                          CV_RGB(100, 100, 100),
                          1);
            }

            for (int bin = 0; bin < hog.nbins; bin++) {
                float gradStrength = gradStrengths.at<float>(cellY, cellX, bin);
                // no line to draw?
                if (gradStrength == 0)
                    continue;

                // draw the perpendicular line of the gradient
                float angle = bin * radRangePerBin + radRangePerBin / 2;
                float scale = gradStrength * maxVecLen;
                Point2f direction = Point2f(sin(angle), -cos(angle));
                line(hogImg,
                     (ptCenter - direction * scale) * imgScale,
                     (ptCenter + direction * scale) * imgScale,
                     CV_RGB(50, 50, 255),
                     1);
            }
        }
    }
    return hogImg;
}
