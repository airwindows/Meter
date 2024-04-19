// AirwindowsUI by Chris Johnson
// Initial seed code for the meter created by Paul Walker on 8/23/21.

#ifndef AIRWINDOWSUI_H
#define AIRWINDOWSUI_H
#include "PluginProcessor.h"

struct AirwindowsMeter : public juce::Component
{
    void paint(juce::Graphics &g) override;
    
    static constexpr int dataPoints = 1200;
    int dataPosition = 0;
    std::array<float, dataPoints> dataA;
    std::array<float, dataPoints> dataB;
    std::array<float, dataPoints> dataC;
    std::array<float, dataPoints> dataD;
    std::array<float, dataPoints> dataE;
    std::array<float, dataPoints> dataF;
    std::array<float, dataPoints> dataG;
    std::array<float, dataPoints> dataH;
    
    void pushA(float X) {dataA[dataPosition] = X;}
    void pushB(float X) {dataB[dataPosition] = X;}
    void pushC(float X) {dataC[dataPosition] = X;}
    void pushD(float X) {dataD[dataPosition] = X;}
    void pushE(float X) {dataE[dataPosition] = X;}
    void pushF(float X) {dataF[dataPosition] = X;}
    void pushG(float X) {dataG[dataPosition] = X;}
    void pushH(float X) {dataH[dataPosition] = X;}
    void pushIncrement(float limit) {
        dataPosition++;
        if (dataPosition >= limit) dataPosition = 0;
    }
    void resetArrays(){
        for (int count = 0; count < dataPoints; ++count) //count through all the points in the array
        {
            dataA[count] = 0.0f;
            dataB[count] = 0.0f;
            dataC[count] = 0.0f;
            dataD[count] = 0.0f;
            dataE[count] = 0.0f;
            dataF[count] = 0.0f;
            dataG[count] = 0.0f;
            dataH[count] = 0.0f;
        }
    }
    float lastLOutline = 0.0;
    float lastROutline = 0.0;
    float lastLPeak = 0.0;
    float lastRPeak = 0.0;
    float lastLSlew = 0.0;
    float lastRSlew = 0.0;
};

extern AirwindowsMeter meter;

#endif // AIRWINDOWSUI_H
