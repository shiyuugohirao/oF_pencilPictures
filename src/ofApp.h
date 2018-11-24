#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"

struct blobContour{
    ofPolyline polyline;
    ofColor color;
};

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);

    ofVideoGrabber cam;
    vector<ofPtr<ofxCv::ContourFinder>> finders;
    vector<ofColor> targetColors;
    vector<blobContour> contours;

    ofxPanel gui;
    ofParameter<int> trackType;
    ofParameter<float> minArea,maxArea,threshold,lineWidth, polySpace;
    ofParameter<bool> bShuffle, bFill, bShowCam,bShowDebug;
};
