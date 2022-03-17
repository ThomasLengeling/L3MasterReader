#include "ofApp.h"

int main(){
    ofGLFWWindowSettings settings;
    settings.setSize(1920, 1080);
    settings.windowMode = OF_WINDOW;
    settings.setPosition(glm::vec2(0, 0));
    settings.setGLVersion(4, 1); // programmable renderer
    //settings.resizable = false;
  //  settings.decorated = true;

    auto mainWindow = ofCreateWindow(settings);
    auto mainApp = make_shared<ofApp>();

    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
}
