//
//  FFTPlotter.h
//  FFTPlotter
//
//  Created by Matthew Hosack on 7/22/15.
//
//

#ifndef __FFTPlotter__FFTPlotter__
#define __FFTPlotter__FFTPlotter__

#include "fftw3.h"
#include <vector>

class FFTPlotter {
public:
    FFTPlotter(unsigned int fftlength, unsigned int sr);
    ~FFTPlotter();
    void addSamples(const float *input, const int bufferSize, const int nChannels);
    void draw(const unsigned x, const unsigned y, const unsigned w, const unsigned h);
    void setLength(unsigned int n);

private:
    void processFFT();
    void processHanning();
    
    double *buffer;
    double *bins;
    fftw_complex *fftw_out;

    int count;

    unsigned int fftlength;
    unsigned int samplerate;
    fftw_plan plan;
};

#endif /* defined(__FFTPlotter__FFTPlotter__) */
