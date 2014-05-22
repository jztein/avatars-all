// USAGE: ./hairDrawer <input_img>
// e.g. ./hairDrawer kristen2.jpg

#include "hairDrawer.h"

#include <vector>

Mat getCannyEdges(Mat& srcIm, Mat& grayIm);

// grayIm is grayscale srcIm
Mat getCannyEdges(Mat& srcIm, Mat& grayIm)
{
    cout << "Canny edge detecting..." << endl;
    
    Mat detectedEdges;
    blur(grayIm, detectedEdges, Size(3,3));
    
    int lowThreshold = 20;
    //int maxThreshold;
    int ratio = 3;
    int kernelSize = 3;
    Canny(detectedEdges, detectedEdges, lowThreshold, lowThreshold*ratio, kernelSize);
    
    Mat dstIm;
    dstIm = Scalar::all(0);
    
    grayIm.copyTo(dstIm, detectedEdges);
    //imshow("Cannied im", dstIm);
    //waitKey(0);
    
    Mat filteredIm = Mat::zeros(dstIm.size(), dstIm.type());
    
    cout << srcIm.rows << ", " << srcIm.cols << endl;
    cout << detectedEdges.rows << ", " << detectedEdges.cols << endl;
    int rows = srcIm.rows;
    int cols = srcIm.cols;
    
    // box of hair
    int topMostRow = 1000000000, leftMostCol = 1000000000;
    int bottomMostRow = 0, rightMostCol = 0;
    
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            if (dstIm.at<float>(r, c) > 0.9)
            {
                
                if (leftMostCol > c)
                {
                    leftMostCol = c;
                }
                if (rightMostCol < c)
                {
                    rightMostCol = c;
                }
                if (topMostRow > r)
                {
                    topMostRow = r;
                }
                if (bottomMostRow < r)
                {
                    bottomMostRow = r;
                }
                
                dstIm.at<float>(r, c) = 1.0;
            }
                
        }
    }
    
    //imshow("filtered im", filteredIm);
    //imwrite("filteredIm_40.png", filteredIm);
    //waitKey();
    
    int ringHeight = (bottomMostRow - topMostRow) >> 2;
    int ringWidth = (rightMostCol - leftMostCol) >> 2;
    
    /*
    int innerRingLeft = leftMostCol + ringWidth;
    int innerRingRight = rightMostCol - ringWidth;
    int innerRingTop = topMostRow + ringHeight;
    int innerRingBottom = bottomMostRow - ringHeight;//*/
    //*
    int innerRingLeft = leftMostCol;// + ringWidth;
    int innerRingRight = rightMostCol;// - ringWidth;
    int innerRingTop = topMostRow;// + ringHeight;
    int innerRingBottom = bottomMostRow;// - ringHeight;//*/
    
    //*
    // assume that hair is a section with a hole in the middle (this whole is the face)
    // 
    float width = 10.0;
    width += 1.0f;
    rows = dstIm.rows;
    cols = dstIm.cols;
    int r = 0, c = 0;
    for (r = 0; r < rows; ++r)
    {
        for (c = 0; c < cols; ++c)
        {
            // JUST FOR TESTING FOR NOW
            // REPLACE with intraface markers box
            if ((c > (innerRingLeft+30) && c < (innerRingLeft+118) &&
                 r > (innerRingTop+100) && r < (innerRingBottom-80)))
            //if ((c > innerRingLeft && c < innerRingRight &&
            //     r > innerRingTop && r < innerRingBottom))
            {
                //cout << "tititit" << endl;
                dstIm.at<float>(r,c) = 0.0;
            }
            
            /*
            //cout <<filteredIm.at<float>(r,c) << " ("<< r << ", " <<  c << endl;;
            // pixels either 0.0 or 1.0
            if (dstIm.at<float>(r,c) > 0.10)
            {
                // not in ring
                if ((c > innerRingLeft && c < innerRingRight &&
                    r > innerRingTop && r < innerRingBottom))
                {
                    //cout << "tititit" << endl;
                    dstIm.at<float>(r,c) = 0.0;
                }
            }
             //*/
        }
    }
     //*/
    cout << "balaklava" << endl;
    
    //imshow("filtered hair im", dstIm);
    //imwrite("hairIm_40.png", filteredIm);
    //waitKey();
    return dstIm;
}

bool cmpMarkersHorz(Point a, Point b)
{
    return a.x < b.x;
}

bool cmpMarkersVert(Point a, Point b)
{
    return a.y < b.y;
}

void fitToHairContour(Mat& cannyIm)
{
    //imshow("canny im", cannyIm);
    //waitKey();
    
    Mat markerIm = Mat::zeros(cannyIm.rows, cannyIm.cols, cannyIm.type());
    
    ofstream svgFile("hairTest_krs1.svg");
    int svgWidth = markerIm.cols;
    int svgHeight = markerIm.rows;
    
    // init svg
    svgFile << "<!DOCTYPE html>\n<html>\n<body>\n<svg width='" << svgWidth << "' height='" << svgHeight << "' xmlns='http://www.w3.org/2000/svg' xmlns:xlink= 'http://www.w3.org/1999/xlink' viewBox='0 0 " << svgWidth << " " << svgHeight << "'> \n";
    // SVG defs
    svgFile << "<defs> \n";
    svgFile << "</defs> \n";
    // Background
    svgFile << "<rect x='0' y='0' width='" << svgWidth << "' height='" << svgHeight << "' style='fill:blue;opacity:0.1'/> \n";
    
    // find bottom leftmost white (beginning)
    // find bottom rightmost white (end)
    int rows = cannyIm.rows;
    int cols = cannyIm.cols;
    int numCircles = 0;
    
    vector<Point> markers;
    
    for (int r = 0; r < rows; r+=10)
    {
        for (int c = 0; c < cols; ++c)
        {
            if (cannyIm.at<float>(r, c) > 0.9)
            {
                markerIm.at<float>(r,c) = 1.0;
                
                Point curMarker(4*c, r);
                
                circle(markerIm, curMarker, 5, Scalar(255)); 
                svgFile << "<circle style='fill:#0000ff' cx='" << curMarker.x << "' cy='" << curMarker.y << "' r='3' />\n";
                
                if (4*c < svgWidth)
                    markers.push_back(curMarker);
                
                numCircles++;
                
            }
        }
        
    }
    
    
    size_t midPt = markers.size()/2;
    sort(markers.begin(), markers.end(), cmpMarkersHorz);
    sort(markers.begin(), markers.begin() + midPt-1, cmpMarkersVert);
    sort(markers.begin()+ midPt, markers.end(), cmpMarkersVert);
    
    // TODO change straight lines to hermite curve
    // connect markers with a line?
    for (size_t i = 1; i < markers.size(); ++i)
    {
        if (i == midPt-1)
        {
            svgFile << "<path d='M" << markers[i].x << " " << markers[i].y \
            << " L" << markers[0].x << " " << markers[0].y \
            << "' fill='transparent' stroke='red' />\n";
            continue;
        }
        svgFile << "<path d='M" << markers[i].x << " " << markers[i].y \
                << " L" << markers[i-1].x << " " << markers[i-1].y \
                << "' fill='transparent' stroke='red' />\n";
    }
    
    cout << "num circles: " << numCircles << endl;
    
    //imshow("marker im", markerIm);
    //imwrite("marker_im_thresh45.png", markerIm);
    //waitKey();
    
    // close svg
    svgFile << "</svg>\n</body>\n</html>\n";
    svgFile.close();
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cout << "USAGE: ./hairDrawer <input_img>" << endl;
        exit(1);
    }
    
    Mat inputIm = imread(argv[1]);
    if (!inputIm.data)
    {
        cout << "could not read input image: " << argv[1] << endl;
        exit(1);
    }
    
    /*
    imshow("see input image", inputIm);
    waitKey(0);
     //*/
    
    Mat grayInputIm;
    cvtColor(inputIm, grayInputIm, CV_BGR2GRAY);
    
    Mat filteredIm = getCannyEdges(inputIm, grayInputIm);
    
    fitToHairContour(filteredIm);
    
    cout << "END of func" << endl;
}
