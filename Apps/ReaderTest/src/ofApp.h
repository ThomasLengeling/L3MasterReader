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

enum debugConfiguration {
    INPUT_IMG = 0,
    CUT_IMG = 1,
    GRID_POS = 2,
    DEBUG_COLOR = 3,
    RELEASE = 4
};


class ofApp: public ofBaseApp
{
public:

    void setup();
	void update();
	void draw();
    void exit();

    void mouseDragged(int x, int y, int button);
    void keyPressed(int key);
    void keyReleased(int key);

    void ofScreenMarkers();

    //gui
    void setupGUI();
    void drawGUI();
    void updateGUI();

    // constants   
    const int videoDeviceId = 1;
    	
    // marker detection
    ofImage imgMarkersInfo;

    ofFbo    mFbo;

    //camera grabber
    CamCaptureRef mCamGrabber;
    ofImage mImageDetector;

    // aruco detector
    ArucoDetectorRef mArucoDetector;
    int mTotalMarkers;
    bool mRefimentDetector;
    bool mEnableCrop;

    //grid values
    GridDetectorRef  mGridDetector;
    glm::vec2 mGridLocation;
    glm::vec2  mGridStep;
    bool mCalculateGrid;
    bool mEnableGridPos; 

    //grid calculations

    int numMarkers;

    //color
    ofxDatSliderRef mGammaValue;
    ofxDatSliderRef mAlphaValue;
    ofxDatSliderRef mBetaValue;

    bool mEnableColorPros;

    debugConfiguration mConfigureMode;

};
