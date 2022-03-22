#pragma once
/*

Thomas Sanchez Lengeling
March, 2019

Living Line

*/

#include "ofMain.h"

#include <algorithm>
#include <iostream>
#include <vector>

#include <opencv2/calib3d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

// addons
#include "ofxCv.h"
#include "ofxOpenCv.h"

#include "GuiExtension.h"

class CamCapture;
typedef std::shared_ptr<CamCapture> CamCaptureRef;

class CamCapture {
public:
    CamCapture(glm::vec2 dims);
    ~CamCapture();

    static CamCaptureRef create(glm::vec2 dims) {
        return std::make_shared<CamCapture>(dims);
    }

    //setup video
    void setupCam(int id, int ftps = 30);
    void setupVideo(std::string name);

    //cam
    ofVideoGrabber    & getCam(){return mCam;}
    ofVideoPlayer     & getVideo(){return mVideoInput;}

    int getCamId(){return mCamId;}

    //setup GUI
    void setupGUISwap(float x, float y);
    void drawGUISwap();
    void updateGUISwap();

    //draw update images
    bool updateImage();
    void drawImage(int x, int y, int w, int h);
    void drawImage(int x, int y);

    ofPixels  & getImgPixels();

    //gamma, alpha an beta correction
    void setGamma(float  gm){mGamma = gm;}
    void setAlpha(float   alpha){mAlpha = alpha;}
    void setBeta(float  beta){mBeta = beta;}

    float getGamma(){return mGamma;}
    float getAlpha() { return mAlpha; }
    float getBeta() { return mBeta; }

    // clean Image
    void adjustGamma(cv::Mat &img);

    void toogleCrop(){mActivateCrop =! mActivateCrop;}
    void toogleCam(){mActivateCam =! mActivateCam;}

    void cropImg(cv::Mat & inputVideo);

    void drawCropImg();
    void drawCropRoi();
    void drawPerspectiveImg();

    void resetCrop();

    cv::Mat & getCropMat(){return mCropMat;}

    void setCropUp(glm::vec2 up);
    void setCropDown(glm::vec2 down);

    void setCropDisp(glm::vec2 dis) { mDisp = dis; }

    glm::vec2 getCropUp(){return mCornerUp;}
    glm::vec2 getCropDown(){return mCornerDown;}
    glm::vec2 getCropDisp(){return mDisp;}
    cv::Rect getROI() { return mRoi; }

    void setId(int id){mId = id;}

    void updateDetectImg(ofImage & img){mVidImg = img;}
    ofImage & getImg(){return mVidImg;}

    ofTexture & getFboTexture(){return mFboResolution.getTexture();}

    //Perspective Transform
    void calculatePerspective(cv::Mat& inputVideo);
    //get cornes for transformation
    void updateCorners(glm::vec2 corner);

    bool isEnablePerspective() { return mCalculatedPerspec; }
    void enablePerspective() { mCalculatedPerspec = true; }

    cv::Mat& getPerspecMat() { return mPerspectiveMat; }

    glm::vec2 getPerspecDim() { return mPrespecDim; }

    void resetPerspective();

    void setInputPersp(glm::vec2 pos, int index);
    void addInputPersp(glm::vec2 pos, int index);

    glm::vec2 getInputPersp(int index);


private:
    //input stream
    ofVideoGrabber      mCam;
    int                 mFps;
    int                 mCamId;
    ofPixels            mPixs;

    glm::vec2           mDim;

    //Resolution of the I
    ofFbo               mFboResolution;

    //swap camId
    ofxDatMatrixRef     mSwapCamId;

    //video player
    ofVideoPlayer       mVideoInput;
    std::string         mVideoName;

    //input image
    ofImage             mVidImg;

    //acative streaming
    bool mActivateCam;

    //crop of image
    bool      mActivateCrop;
    cv::Rect  mRoi;
    glm::vec2 mCornerUp;
    glm::vec2 mCornerDown;
    glm::vec2 mDisp;

    // Perspective Transformation
    bool        mCalculatedPerspec;  //can calculate perspective
    cv::Mat     mPerspectiveMat;

    //perspective points
    cv::Point2f mInputQuad[4]; //corners
    int         cornerIndex;
    glm::vec2   mPrespecDim;
    bool        mResetPerspec;

    //crop Mat
    cv::Mat   mCropMat;


    //id of the Image
    int mId;

    //img correction
    float mGamma;
    float mAlpha;
    float  mBeta;
};
