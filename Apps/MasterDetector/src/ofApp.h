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
#include <map>
#include <utility>


#define NUM_CAM_INPUTS 4

#define CAM_WIDTH  1920  //1920
#define CAM_HEIGHT 1080 //1080

#define CAM_FRAMERATE 5 //10

#define MAX_MARKERS  267



enum debugConfiguration {
    INPUT_IMG = 0,
    CUT_IMG = 1,
    GRID_POS = 2,
    DEBUG_COLOR = 3,
    RELEASE = 4,
    DEBUG = 5,
    SINGLE_MODE = 6,
    PERSPECTIVE_IMG = 7
};




class ofApp: public ofBaseApp
{
public:

    void setup();
	void update();
	void draw();
    void exit();

    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void keyPressed(int key);
    void keyReleased(int key);

    void windowResized(int w, int h);

    void offScreenMarkers();

    //setup
    void setupValues();
    void setupGridInfo();
    void setupCamCalibration();
    void setupUDPConnection();
    void setupCams();
    void setupGUI();
    void setupArucoDetector();
    void setupGridDetector();

    //update
    void updateUDP();
    void updateGUI();

    //draw
    void drawGUI();


    // send commands
    ofxUDPManager mUDPConnectionTable;
    std::string   mUDPIp;
    int           mUDPPort;

    //network for Radar communication
    ofxUDPManager mUDPConnectionRadar;
    std::string   mUDPRadarIp;
    int           mUDPRadarPort;

    ofxUDPManager mUDPConnectionGrid;
    std::string   mUDPGridIp;
    int           mUDPGridPort;


    // 4 camera render
    ofFbo mFboSingle;
    std::vector<ofFbo> mFboGrid;
    ofFbo mFboFullGrid;

    //camera grabber
    std::vector < CamCaptureRef> mCamGrabber;
    int mCurrentCamId;
    ofImage mImageDetector;
    

    // aruco detector
    std::vector < ArucoDetectorRef> mArucoDetector;
    int mMaxMarkers;
    bool mRefimentAruco;
    bool mEnableCrop;

    //grid values
    std::vector < GridDetectorRef>  mGridDetector;
    glm::vec2 mGridLocation;
    glm::vec2  mGridStep;
    bool mCalculateGrid;
    bool mEnableGridPos;

    //
    std::vector<glm::vec2> mGridSizes;

    //grid calculations

    int numMarkers;

    //GUI
    bool            mDrawGUI;
    ofxDatSliderRef mGammaValue;
    ofxDatSliderRef mAlphaValue;
    ofxDatSliderRef mBetaValue;

    ofxDatButtonRef mBSingleGrid;
    ofxDatButtonRef mBFullGrid;
    ofxDatButtonRef mCamCalibration;
    ofxDatMatrixRef mBGridSelect;

    bool mEnableColorPros;

    debugConfiguration mConfigureMode;

    int mHighlightMarkerId;
    ofImage mBaseGrid;

    std::vector<ofVideoDevice> camlist;

    std::vector< std::map<int, int> > mPrevGridArea;
    std::vector< std::map<int, int> > mGridArea;

    std::map<int, int>  mPSGridArea;
    std::map<int, int>  mSGridArea;

    int   spaecialGridInter01[13];

    //perspective
    glm::vec2     mPerspInc;
    int           mPerspectiveIndex;
    glm::vec2     mCurretPerspInc;

    glm::vec2     mMousePos;
};
