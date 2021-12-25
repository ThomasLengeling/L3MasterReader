#include "ofApp.h"

using namespace glm;
using namespace ofxCv;


void ofApp::setup()
{
  
    mArucoDetector = ArucoDetector::create();
    mArucoDetector->setupCalibration(10, 20);

    // init video grabber
    //get back a list of devices.
    // Print available devices
    ofLog(OF_LOG_NOTICE) << "Cam devices:";
    ofVideoGrabber mVideoGrabber;
    mVideoGrabber.listDevices();

    mCamGrabber = CamCapture::create(glm::vec2(1920, 1080));
    mCamGrabber->setId(0);
    mCamGrabber->setupCam(0, 30);


    // oF app
    ofSetVerticalSync(false);
    ofSetFrameRate(60);
    ofSetBackgroundAuto(false);
    ofBackground(0);

    //detector
    mGridDetector = GridDetector::create(glm::vec2(10, 20));
    mGridDetector->setId(0);

    mGridDetector->setMaxMarkers(300);
    mGridDetector->setupCleaner();
    mGridDetector->setupBlocks();

    //ofscren renderer
    mRefimentDetector = true;

    ofFbo fboTemp;
    fboTemp.allocate(1920, 1080, GL_RGB);
    fboTemp.begin();
    ofClear(0, 0, 0);
    fboTemp.end();
   // ofSetWindowShape(videoSize.width, videoSize.height);
}


void ofApp::update()
{
    cv::Mat imageCopy;

    //update frame
    bool newFrame = mCamGrabber->updateImage();
    if (newFrame) {
        ofPixels pixelsImg = mCamGrabber->getImgPixels();

        //crop
        if (pixelsImg.getHeight() > 0) {
            cv::Mat input = ofxCv::toCv(pixelsImg).clone();
            mCamGrabber->cropImg(input);
            cv::Mat copMat = mCamGrabber->getCropMat();
            copMat.copyTo(imageCopy);
        }

        //send img to the aruco detecotor
        if (!imageCopy.empty()) {
            mArucoDetector->detectMarkers(imageCopy, mRefimentDetector);

            mImageDetector = mArucoDetector->getOfImg();

            numMarkers = mArucoDetector->getNumMarkers();

            mCamGrabber->updateDetectImg(mImageDetector);

            mGridDetector->generateMarkers(mArucoDetector->getTagIds(), mArucoDetector->getBoard());
            mGridDetector->updateCleaner();

        }
    }

    ofScreenMarkers();

}
//--------------------------------------------------------------------------- 
void ofApp::ofScreenMarkers() {
    mFbo.begin();

    ofSetColor(255);
    mImageDetector.draw(0, 0);
    mGridDetector->drawMarkers();
    mFbo.end();
      
}

void ofApp::draw()
{    
    ofSetColor(0, 0, 0, 255);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());


    ofSetColor(255);
    //mImageDetector.draw(0, 0, 640, 480);
    mCamGrabber->drawImage(640, 0, 640, 480);

    mFbo.draw(0, 0);

    ofSetColor(255);
    
        ofDrawBitmapStringHighlight("Markers: " + ofToString(numMarkers), 20, ofGetHeight() - 130);
   ofDrawBitmapStringHighlight("app fps: " + ofToString(ofGetFrameRate(), 0), 20, ofGetHeight() - 100);
  // ofDrawBitmapStringHighlight("cam fps: " + ofToString(grabber.getGrabber(, 0), 20, ofGetHeight() - 40);
}




void ofApp::keyReleased(int key)
{
    switch (key)
    {
    case 'a':
 
        break;

    case 't':
                 
        break;

    case 's':
       
        break;

    case 'u':
        break;

    default:
        break;
    } 
}