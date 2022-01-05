#include "ofApp.h"

using namespace glm;
using namespace ofxCv;


void ofApp::setup()
{
  
    mArucoDetector = ArucoDetector::create();
    mArucoDetector->setupCalibration(13, 11);

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
    mGridDetector = GridDetector::create(glm::vec2(13, 11));
    mGridDetector->setId(0);

    mGridDetector->setMaxMarkers(300);
    mGridDetector->generateGridPos();
    mGridDetector->setupBlocks();
    mGridDetector->setupCleaner();

    mGridLocation.x = 863;
    mGridLocation.y = 257;
    mGridStep.x = 65;
    mGridStep.y = 65;


    //ofscren renderer
    mRefimentDetector = true;

    //GUI
    setupGUI();
    mEnableColorPros = false;


    mFbo.allocate(1920, 1080, GL_RGB);
    mFbo.begin();
    ofClear(0, 0, 0);
    mFbo.end();
   // ofSetWindowShape(videoSize.width, videoSize.height);

    mConfigureMode = INPUT_IMG;
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
            cv::Mat copyColor;
            cv::Mat input = ofxCv::toCv(pixelsImg).clone();
            mCamGrabber->cropImg(input);
            cv::Mat copMat = mCamGrabber->getCropMat();
            

            if (mConfigureMode == DEBUG_COLOR || mEnableColorPros) {
                copMat.copyTo(copyColor);
                mCamGrabber->setGamma(mGammaValue->getValue());
                mCamGrabber->setAlpha(mAlphaValue->getValue());
                mCamGrabber->setBeta(mBetaValue->getValue());

                // calculate Gamma
                mCamGrabber->adjustGamma(copyColor);

                copyColor.copyTo(imageCopy);
            }
            else {
                copMat.copyTo(imageCopy);
            }

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

    //network and grid calculations
    mGridDetector->cleanGrid();

    //send a single grid
    if (mGridDetector->isDoneCleaner()) {
        std::string udpMsg = "i ";
        udpMsg += mGridDetector->getUDPMsg();
        //mUDPConnectionTable.Send(udpMsg.c_str(), udpMsg.length());
    }

    mGridDetector->resetCleaner();

    ofScreenMarkers();

    if (mCalculateGrid) {
        mGridDetector->generateGridPos(mGridLocation.x, mGridLocation.y, mGridStep.x, mGridStep.y);
        ofLog(OF_LOG_NOTICE) << mGridLocation.x << " " << mGridLocation.y << " " << mGridStep.x<< " " << mGridStep.y;
        mCalculateGrid = false;
    }

    updateGUI();
}
//--------------------------------------------------------------------------- 
void ofApp::ofScreenMarkers() {
    mFbo.begin();
    ofSetColor(0, 0, 0, 255);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());

    switch (mConfigureMode) {
    case INPUT_IMG:
        ofSetColor(255);
        mImageDetector.draw(0, 0);
        break;

    case CUT_IMG:
        if (mConfigureMode == CUT_IMG) {
            mCamGrabber->drawCropImg();
            mCamGrabber->drawCropRoi();
        }
        break;
    case DEBUG_COLOR:
    {
        ofSetColor(255);
        mImageDetector.draw(0, 0);
    }
    break;
    case GRID_POS:
    {
        ofSetColor(255);


        mImageDetector.draw(0, 0);
        mGridDetector->drawMarkers();

        ofSetColor(255);

        float sqsize = 36;
        float sqspace = 5;

        glm::vec2 dim = mGridDetector->getDim();
        int spaceX = dim.x * (sqsize + sqspace);
        int spaceY = dim.y * (sqsize + sqspace);

        int camWidth = mCamGrabber->getROI().width;

        mGridDetector->drawDetectedGridIn(camWidth + 30, 20, sqsize, sqspace);
    }
        break;

    case RELEASE:
    {
        ofSetColor(255);
        mImageDetector.draw(0, 0);
        float sqsize = 36;
        float sqspace = 5;

        glm::vec2 dim = mGridDetector->getDim();
        int spaceX = dim.x * (sqsize + sqspace);
        int spaceY = dim.y * (sqsize + sqspace);

        //cut RIO
        int camWidth = mCamGrabber->getROI().width;

        mGridDetector->drawDetectedGridIn(camWidth + 30, 20, sqsize, sqspace);
    }
        break;

    }
    
    
    mFbo.end();
      
}

void ofApp::draw()
{    
    ofSetColor(0, 0, 0, 255);
    ofRect(0, 0, ofGetWidth(), ofGetHeight());


    ofSetColor(255);
    mCamGrabber->drawImage(0, 0, 640, 360);
    mImageDetector.draw(0, 640, 640, 360);

    mFbo.draw(0, 0);
    
    drawGUI();

    mGridDetector->recordGrid();

    ofSetColor(255);
    ofDrawBitmapStringHighlight("Markers: " + ofToString(numMarkers), 20, ofGetHeight() - 130);
    ofDrawBitmapStringHighlight("app fps: " + ofToString(ofGetFrameRate(), 0), 20, ofGetHeight() - 100);
  // ofDrawBitmapStringHighlight("cam fps: " + ofToString(grabber.getGrabber(, 0), 20, ofGetHeight() - 40);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
    if (mConfigureMode == GRID_POS) {
        mGridDetector->setGridPos(glm::vec2(x, y));
    }

    //crop input camera information
    if (mConfigureMode == CUT_IMG) {
        {
            float distUp = ofDist(mCamGrabber->getCropUp().x, mCamGrabber->getCropUp().y, x, y);
            if (distUp >= 0.0 && distUp <= 40) {
                mCamGrabber->setCropUp(glm::vec2(x, y));
            }

            float distDown =
                ofDist(mCamGrabber->getCropDown().x, mCamGrabber->getCropDown().y, x, y);
            if (distDown >= 0.0 && distDown <= 40) {
                mCamGrabber->setCropDown(glm::vec2(x, y));
            }
        }
    }

}
//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    switch (key)
    {
    case '1':
        mConfigureMode = INPUT_IMG;
        break;
    case '2':
        mConfigureMode = CUT_IMG;
        break;
    case '3':
        mConfigureMode = GRID_POS;
        mGridDetector->enableDebugGrid(true);
        break;
    case '4':
        mConfigureMode = DEBUG_COLOR;
        mEnableColorPros = true;
        break;
    case '5':
        mConfigureMode = RELEASE;
        break;

    case 'a':
        mArucoDetector->toggleMarkerInfo();
        break;

    case 't':
                 
        break;

    case 'r':
        mRefimentDetector = !mRefimentDetector;
        break;

    case 's':
       
        break;
    case 'c':
        mEnableColorPros = !mEnableColorPros;
        break;
    case 'g':
     
        break;

    case 'u':
        break;





    default:
        break;
    } 



}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    int stepInc = 1;
    if (ofGetKeyPressed(OF_KEY_TAB))
        stepInc = 10;

    if (key == OF_KEY_UP) {
        mGridLocation.y += stepInc;
        mCalculateGrid = true;
    }
    else if (key == OF_KEY_DOWN) {
        mGridLocation.y -= stepInc;
        mCalculateGrid = true;
    }
    else if (key == OF_KEY_LEFT) {
        mGridLocation.x -= stepInc;
        mCalculateGrid = true;
    }
    else if (key == OF_KEY_RIGHT) {
        mGridLocation.x += stepInc;
        mCalculateGrid = true;
    }

     if (key == OF_KEY_UP && ofGetKeyPressed(OF_KEY_SHIFT)) {
        mGridStep.y -= stepInc;
        mCalculateGrid = true;
    }
    else if (key == OF_KEY_DOWN && ofGetKeyPressed(OF_KEY_SHIFT)) {
        mGridStep.y += stepInc;
        mCalculateGrid = true;
    }

    else if (key == OF_KEY_LEFT && ofGetKeyPressed(OF_KEY_SHIFT)) {
        mGridStep.x -= stepInc;
        mCalculateGrid = true;
    }
    else if (key == OF_KEY_RIGHT && ofGetKeyPressed(OF_KEY_SHIFT)) {
        mGridStep.x += stepInc;
        mCalculateGrid = true;
    }
}
//--------------------------------------------------------------
void ofApp::setupGUI() {
    int sliderStartX = 20;
    mGammaValue = ofxDatSlider::create();
    mGammaValue->slider =
        new ofxDatGuiSlider(mGammaValue->ofParam.set("gamma", 0.65, 0, 2));
    mGammaValue->slider->setWidth(390, .4);
    mGammaValue->slider->setPosition(sliderStartX, 110);
    mGammaValue->slider->onSliderEvent(
        [&](ofxDatGuiSliderEvent v) { mGammaValue->ofParam = v.value; });

    //alpha
    mAlphaValue = ofxDatSlider::create();
    mAlphaValue->slider =
        new ofxDatGuiSlider(mAlphaValue->ofParam.set("alpha", 0.65, 0, 3));
    mAlphaValue->slider->setWidth(390, .4);
    mAlphaValue->slider->setPosition(sliderStartX, 160);
    mAlphaValue->slider->onSliderEvent(
        [&](ofxDatGuiSliderEvent v) { mAlphaValue->ofParam = v.value; });

    //beta
    mBetaValue = ofxDatSlider::create();
    mBetaValue->slider =
        new ofxDatGuiSlider(mBetaValue->ofParam.set("beta", 50, 0, 255));
    mBetaValue->slider->setWidth(390, .4);
    mBetaValue->slider->setPosition(sliderStartX, 210);
    mBetaValue->slider->onSliderEvent(
        [&](ofxDatGuiSliderEvent v) { mBetaValue->ofParam = v.value; });

}
//--------------------------------------------------------------
void ofApp::drawGUI() {

    switch (mConfigureMode) {
    case DEBUG_COLOR:
        mGammaValue->draw();
        mAlphaValue->draw();
        mBetaValue->draw();
        break;
    }
}
//--------------------------------------------------------------
void ofApp::updateGUI() {
    switch (mConfigureMode) {
    case DEBUG_COLOR:
        mGammaValue->update();
        mAlphaValue->update();
        mBetaValue->update();
        break;
    }
}

//--------------------------------------------------------------
void ofApp::exit() {

}