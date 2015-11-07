#include <iostream>
#include <stdio.h>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
//#include "opencv2/nonfree/nonfree.hpp"

#include "opencv2/xfeatures2d.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include <sys/stat.h>


using namespace cv;
using namespace std;
using namespace cv::xfeatures2d;


template<class KPMatcher>
struct SURFMatcher
{
    KPMatcher matcher;
    template<class T>
    void match(const T& in1, const T& in2, std::vector<cv::DMatch>& matches)
    {
        matcher.match(in1, in2, matches);
    }
};

struct SURFDetector
{
    Ptr<Feature2D> surf;
    SURFDetector(double hessian = 400.0)
    {
        surf = SURF::create(hessian);
    }
    template<class T>
    void operator()(const T& in, const T& mask, std::vector<cv::KeyPoint>& pts, T& descriptors, bool useProvided = false)
    {
        surf->detectAndCompute(in,mask, pts, descriptors, useProvided);
    }
};

int main(){
    //Mat reference_object = imread("reference.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    Mat reference_object = imread("reference.jpg",1); 

    //VideoCapture cap(0); // open the video camera no. 0
    //if (!cap.isOpened())  // if not success, exit program
    //{
        //cout << "Cannot open the video cam" << endl;
        //return -1;
    //}
    Mat image, image_orig;
    //cap.read(image);
    
    image_orig = imread("1.jpg", 1);

    image = image_orig;
    //cvtColor(image_orig, image, CV_RGB2GRAY);

    Mat des1, des2;
    SURFDetector surf; 
    std::vector<KeyPoint> keypoints_object, keypoints_scene;
    std::vector<DMatch> matches;

    surf(reference_object, Mat(), keypoints_object, des1);
    surf(image, Mat(), keypoints_scene, des2);
    
    SURFMatcher<BFMatcher> matcher;
    matcher.match(des1, des2, matches);

    double max_dist = 0; double min_dist = 100;

  //-- Quick calculation of max and min distances between keypoints
    for( int i = 0; i < des1.rows; i++ )
    { double dist = matches[i].distance;
        if( dist < min_dist ) min_dist = dist;
        if( dist > max_dist ) max_dist = dist;
    }

    printf("-- Max dist : %f \n", max_dist );
    printf("-- Min dist : %f \n", min_dist );
    
    std::vector< DMatch > good_matches;

    for( int i = 0; i < des1.rows; i++ )
    { if( matches[i].distance < 3*min_dist )
        { good_matches.push_back( matches[i]); }
    }
   

    Mat img_matches;
    drawMatches(reference_object, keypoints_object, image, keypoints_scene,
               good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
               vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

    
    //-- Localize the object
    std::vector<Point2f> obj;
    std::vector<Point2f> scene;

    for( int i = 0; i < (int)good_matches.size(); i++ )
    {
    //-- Get the keypoints from the good matches
        obj.push_back( keypoints_object[ good_matches[i].queryIdx ].pt );
        scene.push_back( keypoints_scene[ good_matches[i].trainIdx ].pt );
    }

    Mat H = findHomography( obj, scene, CV_RANSAC );

    //-- Get the corners from the image_1 ( the object to be "detected" )
    std::vector<Point2f> obj_corners(4);
    obj_corners[0] = cvPoint(0,0); obj_corners[1] = cvPoint( reference_object.cols, 0 );
    obj_corners[2] = cvPoint( reference_object.cols, reference_object.rows ); obj_corners[3] = cvPoint( 0, reference_object.rows );
    std::vector<Point2f> scene_corners(4);

    perspectiveTransform( obj_corners, scene_corners, H);

    //-- Draw lines between the corners (the mapped object in the scene - image_2 )
    line( img_matches, scene_corners[0] + Point2f( reference_object.cols, 0), scene_corners[1] + Point2f( reference_object.cols, 0), Scalar(0, 255, 0), 4 );
    line( img_matches, scene_corners[1] + Point2f( reference_object.cols, 0), scene_corners[2] + Point2f( reference_object.cols, 0), Scalar( 0, 255, 0), 4 );
    line( img_matches, scene_corners[2] + Point2f( reference_object.cols, 0), scene_corners[3] + Point2f( reference_object.cols, 0), Scalar( 0, 255, 0), 4 );
    line( img_matches, scene_corners[3] + Point2f( reference_object.cols, 0), scene_corners[0] + Point2f( reference_object.cols, 0), Scalar( 0, 255, 0), 4 );

    //-- Show detected matches
    
    //-- Drow lines on original corners
    line( image, scene_corners[0] , scene_corners[1] , Scalar(0, 255, 0), 4 );
    line( image, scene_corners[1] , scene_corners[2] , Scalar( 0, 255, 0), 4 );
    line( image, scene_corners[2] , scene_corners[3] , Scalar( 0, 255, 0), 4 );
    line( image, scene_corners[3] , scene_corners[0] , Scalar( 0, 255, 0), 4 );
    

    imshow( "Good Matches & Object detection", img_matches );
    imshow("object in orginal image", image_orig);

    waitKey(0);
    return 0;
}

