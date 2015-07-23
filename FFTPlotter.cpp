//
//  FFTPlotter.cpp
//  FFTPlotter
//
//  Created by Matthew Hosack on 7/22/15.
//
//
#include "ofGraphics.h"
#include <cmath>
#define NEXTPOW2(x) ( pow(2, ceil(log(x)/log(2))) )

#include "FFTPlotter.h"

FFTPlotter::FFTPlotter(unsigned int fftlength, unsigned int sr){
    /* Force power of 2 length */
    this->setLength(fftlength);
    samplerate = sr;

    /* Audio/FFT Buffers */
    buffer = new double[fftlength];
    bins = new double[fftlength / 2 + 1];
    fftw_out = new fftw_complex[fftlength];
    count = 0;
    
    /* FFTW plan */
    plan = fftw_plan_dft_r2c_1d(
        fftlength
        ,buffer
        ,fftw_out
        ,FFTW_ESTIMATE);
}

FFTPlotter::~FFTPlotter(){
    delete [] buffer;
    delete [] bins;
}

void FFTPlotter::addSamples(const float *input, const int bufferSize, const int nChannels){
    for (int i = 0; i < bufferSize; ++i) {
        buffer[count++] = input[i];
    }

    /* processFFT */
    if (count == fftlength) {
        this->processFFT();
    }
}

void FFTPlotter::draw(const unsigned x, const unsigned y, const unsigned w, const unsigned h){
    ofDrawBitmapString("Draw is being called", 10, 10);
    
    ofPushStyle();
    ofPushMatrix();

    ofTranslate(x, y, 0);
    
    int nBins = fftlength / 2 + 1;
    float binWidth = w / (float)nBins;
    
    ofSetColor(20, 255, 20);
    
    double mag;
    for (int i = 0; i < nBins; ++i) {
        mag = bins[i];
        ofRect(binWidth * i, h - (mag / 100.), binWidth, mag / 100. * h);
    }

    ofPopStyle();
    ofPopMatrix();
}

void FFTPlotter::setLength(unsigned int n)
{
    fftlength = NEXTPOW2(n);
}

void FFTPlotter::processFFT(){
    /* Window off samples and run fftw */
    this->processHanning();
    fftw_execute(plan);
    
    /* Convert to magnitude */
    double real, imag;
    
    for (int i = 0; i < fftlength / 2; ++i) {
        real = fftw_out[i][0];
        imag = fftw_out[i][1];
        bins[i] = 20.0f * log(sqrt(real*real + imag*imag));
    }
    
    /* move back to first index */
    count = 0;
}

void FFTPlotter::processHanning(){
    int N = fftlength - 1;
    
    for (int i = 0; i < fftlength; ++i) {
        double mult = 0.5 * (1.0 - cos(2 * M_PI * i / N));
        buffer[i] = mult * buffer[i];
    }
}
