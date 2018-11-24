#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

void ofApp::setup() {
    ofBackground(255);
    cam.setup(640, 480);

    gui.setup();
    gui.setPosition(650, 10);
    gui.add(minArea.set("minArea", 3, 0, 30));
    gui.add(maxArea.set("maxArea", 50, 0, 300));
    gui.add(threshold.set("Threshold", 12, 0, 255));
    gui.add(trackType.set("Track Type", 0, 0,4));
    gui.add(bShuffle.set("shuffle", false));
    gui.add(bFill.set("fill", false));
    gui.add(bShowCam.set("showCam", false));
    gui.add(bShowDebug.set("showDebug", false));
    gui.add(lineWidth.set("lineWidth", 3, 0.1, 10));
    gui.add(polySpace.set("polySpace", 10, 0.1, 30));

    targetColors.resize(5);
    for(auto &c:targetColors) c=ofColor(ofRandom(255),ofRandom(255),ofRandom(255));
    //setup finders
    for(int i=0;i<targetColors.size(); i++){
        ofPtr<ofxCv::ContourFinder> f = make_shared<ofxCv::ContourFinder>();
        f->setMinAreaRadius(minArea);
        f->setMaxAreaRadius(maxArea);
        f->setThreshold(threshold);
        f->setTargetColor(targetColors[i], TrackingColorMode((int)trackType));
        f->findContours(cam);

        finders.push_back(f);
    }
}

void ofApp::update() {
    ofSetWindowTitle(ofToString(ofGetFrameRate(),2));

    cam.update();
    if(cam.isFrameNew()) {
        contours.clear();
        for(int i=0;i<targetColors.size(); i++){
            auto f = finders[i];
            f->setMinAreaRadius(minArea);
            f->setMaxAreaRadius(maxArea);
            f->setThreshold(threshold);
            f->setTargetColor(targetColors[i], TrackingColorMode((int)trackType));
            f->findContours(cam);

            for(auto &p:f->getPolylines()){
                blobContour b;
                b.polyline = p.getResampledBySpacing(polySpace);
                glm::vec2 c = p.getCentroid2D();
                b.color = cam.getPixels().getColor(c.x,c.y);
                contours.push_back(b);
            }
        }
    }
}

void ofApp::draw() {
    ofSetColor(255);
    if(bShowCam) cam.draw(0, 0);

    ofPushStyle();
    (bFill) ? ofFill() : ofNoFill();
    ofSetLineWidth(lineWidth);
    for(auto &c:contours){
        ofSetColor(c.color);
        auto points = c.polyline.getVertices();
        if(bShuffle) random_shuffle(points.begin(), points.end()-1);
        ofBeginShape();
        for(auto &p:points) ofCurveVertex(p);
        ofEndShape();
    }
    ofPopStyle();

    if(bShowDebug) for(auto &f:finders) f->draw();

    gui.draw();

    for(int i=0; i<targetColors.size(); i++){
        ofPushMatrix();
        ofTranslate(ofGetWidth()-75, 75 + 32*i);
        ofFill();
        ofSetColor(0);
        ofDrawRectangle(-1, -1, 30+2, 30+2);
        ofSetColor(targetColors[i]);
        ofDrawRectangle(0, 0, 30, 30);
        ofPopMatrix();
    }
}

void ofApp::keyPressed(int key){
    switch(key){
        case '0': targetColors[0] = cam.getPixels().getColor(mouseX,mouseY); break;
        case '1': targetColors[1] = cam.getPixels().getColor(mouseX,mouseY); break;
        case '2': targetColors[2] = cam.getPixels().getColor(mouseX,mouseY); break;
        case '3': targetColors[3] = cam.getPixels().getColor(mouseX,mouseY); break;
        case '4': targetColors[4] = cam.getPixels().getColor(mouseX,mouseY); break;
    }
}
