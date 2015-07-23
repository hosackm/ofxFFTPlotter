#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    samples = std::vector<float>(512);
    stream.setup(this, 2, 1, 48000, 512, 4);
    plotter = new FFTPlotter(2048, 48000);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    plotter->draw(100, 100, 400, 300);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    for(int i = 0; i < bufferSize; ++i)
    {
        //samples[i] = input[i];
        plotter->addSamples(input, bufferSize, nChannels);
    }
}

void ofApp::audioOut(float *output, int bufferSize, int nChannels){
//    for(int i = 0; i < bufferSize; i++)
//    {
//        output[2 * i] = samples[i];
//        output[2 * i + 1] = samples[i];
//    }
}

ofApp::~ofApp(){
    delete plotter;
}
