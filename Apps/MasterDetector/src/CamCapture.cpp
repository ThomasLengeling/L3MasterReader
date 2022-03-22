#include "CamCapture.h"

using namespace std;
using namespace cv;


//----------------------------------------------------------------------------
CamCapture::~CamCapture(){
  mCam.close();
  cout<<"cam closed "<<mId<<std::endl;
}

//----------------------------------------------------------------------------
CamCapture::CamCapture(glm::vec2 dims) {
  mDim = dims;
  mGamma = 0.65;
  mActivateCrop = false;
  mActivateCam = true;
  mFps = 30;

  mCornerUp = glm::vec2(0, 0);
  mCornerDown = glm::vec2(dims.x, dims.y);
  mDisp = glm::vec2(23, 23);

  mFboResolution.allocate(mDim.x, mDim.y, GL_RGBA);
  mPixs.allocate(mDim.x, mDim.y, OF_PIXELS_RGBA);

  // clean start with Fbos
  mFboResolution.begin();
  ofClear(0, 0, 0, 255);
  mFboResolution.end();

  //perspective
  cornerIndex = 0;
  mCalculatedPerspec = false;
  mInputQuad[0] = cv::Point2f(20, 20);
  mInputQuad[1] = cv::Point2f(700, 20);
  mInputQuad[2] = cv::Point2f(700, 800);
  mInputQuad[3] = cv::Point2f(20, 800);
}

//-----------------------------------------------------------------------------
void CamCapture::setupCam(int id, int fps) {
  mCamId = id;
  mFps = fps;

  mCam.setDeviceID(mCamId);
  mCam.setVerbose(true);
  mCam.setDesiredFrameRate(mFps);
  mCam.initGrabber(mDim.x, mDim.y);
  mCam.setUseTexture(true);

  ofLog(OF_LOG_NOTICE) << "loaded Cam: " << mCamId << " " << mId << " "
                       << mDim.x << " " << mDim.y << "  " << mCam.getWidth()
                       << " " << mCam.getHeight()<<" fps: "<<mFps<<std::endl;
}

//-----------------------------------------------------------------------------
void CamCapture::setupGUISwap(float x, float y) {
  mSwapCamId = ofxDatMatrix::create();
  mSwapCamId->matrix =new ofxDatGuiMatrix("Cam Selector: " + to_string(mId), 4, true);
  mSwapCamId->matrix->setRadioMode(true);
  mSwapCamId->matrix->setOpacity(0.7);
  mSwapCamId->matrix->setWidth(390, .4);
  mSwapCamId->matrix->setPosition(x, y);
  mSwapCamId->matrix->onMatrixEvent([&](ofxDatGuiMatrixEvent v) {
    ofLog(OF_LOG_NOTICE) << "Id: " << mCamId << " New Index: " << v.child<< std::endl;
    mCam.close();

    mCamId = v.child;
    mCam.setDeviceID(mCamId);
    // mCam.setVerbose(true);
    mCam.setDesiredFrameRate(mFps);
    mCam.initGrabber(mDim.x, mDim.y);
  });
}

//-----------------------------------------------------------------------------
void CamCapture::setupVideo(std::string name) {
  // mVideoName = name;
  // mVideoInput.load(mVideoName);
  // mVideoInput.play();

  ofLog(OF_LOG_NOTICE) << "loaded Video: " << mVideoName << " " << mId;
}

//-----------------------------------------------------------------------------
bool CamCapture::updateImage() {
  bool newFrame = false;
  if (mActivateCam) {
    mCam.update();
    newFrame = mCam.isFrameNew();
    if (newFrame) {
      // mFboResolution.begin();
      // mCam.draw(0, 0, mDim.x, mDim.y);
      // mFboResolution.end();
      // ofLog(OF_LOG_NOTICE) << " " << newFrame << " ";
    }
  } else {
    // mVideoInput.update();
    // newFrame = mVideoInput.isFrameNew();
  }
  return newFrame;
}

//-----------------------------------------------------------------------------
ofPixels & CamCapture::getImgPixels() {
  // return (mActivateCam) ? mCam.getPixels() : mVideoInput.getPixels();
  return mCam.getPixels();
}

//-----------------------------------------------------------------------------
void CamCapture::setCropUp(glm::vec2 up) {
  mCornerUp = up;
}

//-----------------------------------------------------------------------------
void CamCapture::setCropDown(glm::vec2 down) {
  mCornerDown = down;
}

//-----------------------------------------------------------------------------
void CamCapture::drawImage(int x, int y, int w, int h) {
  if (mActivateCam) {
    mCam.draw(x, y, w, h);
  } else {
    mVideoInput.draw(x, y, w, h);
  }
}

//-----------------------------------------------------------------------------
void CamCapture::drawImage(int x, int y) {
  if (mActivateCam) {
    mCam.draw(x, y, mDim.x, mDim.y);
  } else {
    mVideoInput.draw(x, y, mDim.x, mDim.y);
  }
}

//-----------------------------------------------------------------------------
void CamCapture::drawGUISwap() {
  // draw GUI
  mSwapCamId->draw();
}

//-----------------------------------------------------------------------------
void CamCapture::updateGUISwap() {
  // update GUI
  mSwapCamId->update();
}

//-----------------------------------------------------------------------------
void CamCapture::adjustGamma(cv::Mat &img) {
  if (!img.empty()) {

    cv::Mat imgGamma;
    img.convertTo(imgGamma, -1, mAlpha, mBeta);

    cv::Mat lookUpTable(1, 256, CV_8U);
    unsigned char *p = lookUpTable.ptr();
    for (int i = 0; i < 256; i++) {
      p[i] = saturate_cast<unsigned char>(pow(i / 255.0, mGamma) * 255.0);
    }
    cv::LUT(imgGamma, lookUpTable, imgGamma);

    imgGamma.copyTo(img);
  }


}
//-----------------------------------------------------------------------------
void CamCapture::resetCrop() {

  mCornerUp = glm::vec2(0, 0);
  mCornerDown = glm::vec2(mDim.x, mDim.y);
  mDisp = glm::vec2(5, 5);

}
//-----------------------------------------------------------------------------
void CamCapture::cropImg(cv::Mat &inputVideo) {

  mRoi.x = abs(mCornerUp.x);
  mRoi.y = abs(mCornerUp.y);
  mRoi.width = abs(mCornerDown.x - mCornerUp.x);//  mLength.x;// +mDisp.x;
  mRoi.height = abs(mCornerDown.y - mCornerUp.y);  //mLength.y;// +mDisp.y;


  //ofLog(OF_LOG_NOTICE) << mRoi.x << " " << mRoi.y << " - " << mRoi.width << " " << mRoi.height;
  // Copy the data into new matrix
  if (mRoi.width <= mDim.x && mRoi.height <= mDim.y &&
      (mRoi.x >= 0 && mRoi.y >= 0 && mRoi.width <= mDim.x &&  mRoi.height <= mDim.y)) {
      cv::Mat cutMat(inputVideo, mRoi);
      cutMat.copyTo(mCropMat);
  }
  else {
      inputVideo.copyTo(mCropMat);
      ofLog(OF_LOG_NOTICE) << "error crop dims Dims";
  }

}
//-----------------------------------------------------------------------------
void CamCapture::drawCropRoi() {
  if (mActivateCrop) {
    ofSetColor(0, 200, 125, 50);
  } else {
    ofSetColor(0, 220, 195, 155);
  }

  ofBeginShape();
  ofVertex(mCornerUp);
  ofVertex(glm::vec2(mCornerDown.x, mCornerUp.y));
  ofVertex(mCornerDown);
  ofVertex(glm::vec2(mCornerUp.x, mCornerDown.y));
  ofEndShape();

  ofSetColor(255, 50, 50, 155);

  ofDrawCircle(mCornerDown.x- 30, mCornerDown.y-30, 30, 30);
  ofDrawCircle(mCornerUp.x +30, mCornerUp.y+30, 30, 30);
}
//-----------------------------------------------------------------------------
void CamCapture::drawCropImg() {
  ofImage imgCut;
  ofxCv::toOf(mCropMat, imgCut.getPixels());
  imgCut.update();

  ofSetColor(200, 200);
  imgCut.draw(mCornerUp.x, mCornerUp.y, abs(mCornerDown.x - mCornerUp.x), abs(mCornerDown.y - mCornerUp.y));

  //ofxCv::drawMat(mCropMat, 0, 200, 200, 200);
  //imgCut.draw(0, 400, 200, 200);
}

//-----------------------------------------------------------------------------
void CamCapture::drawPerspectiveImg() {
    ofImage imgPerspec;
    ofxCv::toOf(mPerspectiveMat, imgPerspec.getPixels());
    imgPerspec.update();

    imgPerspec.draw(0, 0);
}

//-----------------------------------------------------------------------------
void CamCapture::updateCorners(glm::vec2 corner) {
    //Input Quadilateral or Image plane coordinates
    mInputQuad[cornerIndex] = cv::Point2f(corner.x, corner.y);
    ofLog(OF_LOG_NOTICE) << "added point " << cornerIndex << " " << corner.x << ", " << corner.y << std::endl;

    cornerIndex++;
    if (cornerIndex >= 4) {
        cornerIndex = 0;
        ofLog(OF_LOG_NOTICE) << "new calculated perspective done";
        mCalculatedPerspec = true;
    }
}
//-----------------------------------------------------------------------------
void CamCapture::calculatePerspective(cv::Mat& inputVideo) {
    // Output Quadilateral or World plane coordinates
    ofPoint tl(mInputQuad[0].x, mInputQuad[0].y);
    ofPoint tr(mInputQuad[1].x, mInputQuad[1].y);
    ofPoint br(mInputQuad[2].x, mInputQuad[2].y);
    ofPoint bl(mInputQuad[3].x, mInputQuad[3].y);

    float widthA = sqrt(pow(br.x - bl.x, 2) + pow(br.y - bl.y, 2));
    float widthB = sqrt(pow(tr.x - tl.x, 2) + pow(tr.y - tl.y, 2));
    float maxWidth = glm::max(widthA, widthB);

    float heightA = sqrt(pow(tr.x - br.x, 2) + pow(tr.y - br.y, 2));
    float heightB = sqrt(pow(tl.x - bl.x, 2) + pow(tl.y - bl.y, 2));
    float maxHeight = glm::max(heightA, heightB);

    cv::Point2f outputQuad[4];
    outputQuad[0] = cv::Point2f(0, 0);
    outputQuad[1] = cv::Point2f(maxWidth - 1, 0);
    outputQuad[2] = cv::Point2f(maxWidth - 1, maxHeight - 1);
    outputQuad[3] = cv::Point2f(0, maxHeight - 1);

    mPrespecDim.x = maxWidth;
    mPrespecDim.y = maxHeight;


    //ofLog(OF_LOG_NOTICE) << "Perspective " << mPrespecDim.x << " " << mPrespecDim.y<< std::endl;

    // Lambda Matrix
    Mat lambda = getPerspectiveTransform(mInputQuad, outputQuad);
    cv::Size matSize(maxWidth, maxHeight);

    // Apply the Perspective Transform just found to the src image
    warpPerspective(inputVideo, mPerspectiveMat, lambda, matSize);
}
//-----------------------------------------------------------------------------
void CamCapture::setInputPersp(glm::vec2 pos, int index) {
    mInputQuad[index] = cv::Point2f(pos.x, pos.y);
}
//-----------------------------------------------------------------------------
void CamCapture::addInputPersp(glm::vec2 point, int index) {
    mInputQuad[index] += cv::Point2f(point.x, point.y);
    ofLog(OF_LOG_NOTICE) << "set point " << index << " " << mInputQuad[index].x << " " << mInputQuad[index].y;
}

//-----------------------------------------------------------------------------
glm::vec2 CamCapture::getInputPersp(int index) {
    return glm::vec2(mInputQuad[index].x, mInputQuad[index].y);
}

//-----------------------------------------------------------------------------
void CamCapture::resetPerspective() {
    mCalculatedPerspec = true;
    cornerIndex = 0;

    mInputQuad[0] = cv::Point2f(0, 0);
    mInputQuad[1] = cv::Point2f(mDim.x - 1, 0);
    mInputQuad[2] = cv::Point2f(mDim.x - 1, mDim.y - 1);
    mInputQuad[3] = cv::Point2f(0, mDim.y - 1);
}
