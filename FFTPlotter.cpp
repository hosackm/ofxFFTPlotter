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
    /* Store Graphics */
    ofPushStyle();
    ofPushMatrix();
    
    /* Make drawing relative to x/y input parameters */
    ofTranslate(x, y);
    
    /* Calculate width of bins */
    int nBins = fftlength / 2 + 1;
    float binWidth = (float)w / (float)nBins;
    
    /* Decide maximum y axis value */
    float ylimit = 70.0;
    
    /* Cool Color */
    ofSetColor(20, 255, 20);
    
    /* temp drawing variables */
    double mag, hdraw, ydraw;
    for (int i = 0; i < nBins; ++i) {
        mag = bins[i];
        
        /* Height to draw our rectangle with (don't let it go negative) */
        hdraw = h / ylimit * mag;
        hdraw = hdraw < 0.0f ? 0.0f : hdraw;
        
        /* Y location to start rectangle at */
        ydraw = h - hdraw;
        
        /* Draw the rectangle */
        ofRect(i * binWidth, ydraw, binWidth, hdraw);
    }
    
    /* Pop Graphics */
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
    fftw_execute(this->plan);
    
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
