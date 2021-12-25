#pragma once

#include "ofMain.h"

#include "ofxCv.h"
#include "ofxOpenCv.h"

#include <opencv2/aruco.hpp>

#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "ArucoDetector.h"
#include "QRBlock.h"
#include "CamCapture.h"
#include "GridDetector.h"


#define QR_4x4_50   0
#define QR_6x6_1000 11


class ofApp: public ofBaseApp
{
public:

    void setup();
	void update();
	void draw();
    void keyReleased(int key);

    void ofScreenMarkers();

    // constants   
    const int videoDeviceId = 1;
    	
    // marker detection
    ofImage imgMarkersInfo;

    ofFbo    mFbo;

    GridDetectorRef  mGridDetector;

    // grabber
    ofVideoGrabber grabber;
    cv::Size videoSize = cv::Size(640, 480);
    ofImage imgVideoUndistort;

    //camera grabber
    CamCaptureRef mCamGrabber;
    ofImage mImageDetector;

    // aruco detector
    ArucoDetectorRef mArucoDetector;
    int mTotalMarkers;
    bool mRefimentDetector;

    //grid calculations

    int numMarkers;

};
