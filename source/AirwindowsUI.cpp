// AirwindowsUI by Chris Johnson
// Initial seed code for the meter created by Paul Walker on 8/23/21.
#include "AirwindowsUI.h"

void AirwindowsMeter::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::white); //blank screen before doing anything, unless our draw covers the whole display anyway
    //this is probably quick and optimized
    
    float dx = (float)getWidth()/1200.0f; //width of individual pixel on this display object
    float dy = (float)getHeight()/600.0f; //height of individual pixel on this display object
    float dotWidth = 1200.0f/(float)getParentWidth(); //scale single pixel to roughly the right size
    float dotHeight = 600.0f/(float)getParentHeight(); //scale single pixel to roughly the right size

    g.setColour(juce::Colours::lightgrey);
    g.fillRect(0.0,  60.0f*dy, (float)getWidth(),1.0); // -6dB markings
    g.fillRect(0.0, 100.0f*dy, (float)getWidth(),1.0); //-12dB markings
    g.fillRect(0.0, 130.0f*dy, (float)getWidth(),1.0); //-18dB markings
    g.fillRect(0.0, 150.0f*dy, (float)getWidth(),1.0); //-24dB markings
    g.fillRect(0.0, 165.0f*dy, (float)getWidth(),1.0); //-30dB markings
    g.fillRect(0.0, 175.0f*dy, (float)getWidth(),1.0); //-36dB markings

    g.fillRect(0.0, 260.0f*dy, (float)getWidth(),1.0); // -6dB markings for slew
    g.fillRect(0.0, 300.0f*dy, (float)getWidth(),1.0); //-12dB markings
    g.fillRect(0.0, 330.0f*dy, (float)getWidth(),1.0); //-18dB markings
    g.fillRect(0.0, 350.0f*dy, (float)getWidth(),1.0); //-24dB markings
    g.fillRect(0.0, 365.0f*dy, (float)getWidth(),1.0); //-30dB markings
    g.fillRect(0.0, 375.0f*dy, (float)getWidth(),1.0); //-36dB markings

    g.fillRect(0.0, 403.0f*dy, (float)getWidth(),1.0); //20k markings
    g.fillRect(0.0, 415.0f*dy, (float)getWidth(),1.0); //2k markings
    g.fillRect(0.0, 458.0f*dy, (float)getWidth(),1.0); //200hz markings
    g.fillRect(0.0, 593.0f*dy, (float)getWidth(),1.0); //20hz markings

    g.setColour(juce::Colours::black);
    g.fillRect(0.0,  200.0f*dy, (float)getWidth(),1.0); // border with slew meter
    g.fillRect(0.0,  400.0f*dy, (float)getWidth(),1.0); // border with zero cross meter

    for (int count = 0; count < dataPoints; ++count) //count through all the points in the array
    {
        g.setColour(juce::Colours::grey);
        g.fillRect((float)dataPosition*dx, 0.0f, 1.0f, (float)getHeight()); //the moving line
        g.setColour(juce::Colours::black);

        float peakL = dataC[count] * 200.0f;
        float peakR = dataD[count] * 200.0f;
        float slewL = sqrt(dataE[count])*275.0f;
        if (slewL > 197.0f) slewL = 197.0;
        float slewR = sqrt(dataF[count])*275.0f;
        if (slewR > 197.0f) slewR = 197.0;
        float meterZeroL = (sqrt(dataG[count])*6.0f)-6.0f;
        if (meterZeroL > 198.0f) meterZeroL = 198.0f;
        float meterZeroR = (sqrt(dataH[count])*6.0f)-6.0f;
        if (meterZeroR > 198.0f) meterZeroR = 198.0f;
        
        if (dataA[count] > dataB[count]) {
            //draw grey to R point, then green to L
            float h = dataB[count] * 200.0f; //height of this data point
            float setR = 1.0f-dataB[count];
            float setG = 1.0f-dataB[count];
            float setB = 1.0f-dataB[count];
            g.setColour(juce::Colour::fromFloatRGBA (setR, setG, setB, 1.0f));
            g.fillRect((float)count*dx, (200.0f - h)*dy, dx, h*dy);
            float greyPoint = h;
            h = dataA[count] * 200.0f; //height of this data point
            g.setColour(juce::Colour::fromFloatRGBA (0.0, 1.0, 0.0, 1.0f)); //L is pure green
            g.fillRect((float)count*dx, (200.0f - h)*dy, dx, (h-greyPoint)*dy);
            g.setColour(juce::Colours::black); //outline in black
            float extent = fabs(h-lastLOutline)+1.0f;
            if (extent > h) extent = h;
            g.fillRect((float)count*dx, (200.0f - h)*dy, dx, extent*dy);
            g.setColour(juce::Colour::fromFloatRGBA (0.9f, 0.9f, 0.9f, 1.0f));
            g.fillRect((float)count*dx, (400.0f - h)*dy, dx, h*dy);
            lastLOutline = h;
        } else {
            //draw grey to L point, then red to R
            float h = dataA[count] * 200.0f; //height of this data point
            float setR = 1.0f-dataA[count];
            float setG = 1.0f-dataA[count];
            float setB = 1.0f-dataA[count];
            g.setColour(juce::Colour::fromFloatRGBA (setR, setG, setB, 1.0f));
            g.fillRect((float)count*dx, (200.0f - h)*dy, dx, h*dy);
            float greyPoint = h;
            h = dataB[count] * 200.0f; //height of this data point
            g.setColour(juce::Colour::fromFloatRGBA (1.0, 0.0, 0.0, 1.0f)); //R is pure red
            g.fillRect((float)count*dx, (200.0f - h)*dy, dx, (h-greyPoint)*dy);
            g.setColour(juce::Colours::black); //outline in black
            float extent = fabs(h-lastROutline)+1.0f;
            if (extent > h) extent = h;
            g.fillRect((float)count*dx, (200.0f - h)*dy, dx, extent*dy);
            g.setColour(juce::Colour::fromFloatRGBA (0.9f, 0.9f, 0.9f, 1.0f));
            g.fillRect((float)count*dx, (400.0f - h)*dy, dx, h*dy);
            lastROutline = h;
        } //done with RMS chunk and sound density

        
        if (peakL > 197.0f) {
            peakL = 197.0;
            g.setColour(juce::Colour(255, 0, 0));
            g.fillRect((float)count*dx, (200.0f - peakL)*dy, 2.0f*dx, 2.0f*dy);
            g.fillRect((float)count*dx, (400.0f - slewL)*dy, dx, dy);
            g.fillRect((float)count*dx, (400.0f + meterZeroL)*dy, dx, dy);

        } else if (peakL != 0.0f) {
            float psDotSizeL = 2.5f / (fabs((peakL-slewL)*0.08f)+1.0f);
            g.setColour(juce::Colours::black);
            juce::uint8 blueSpot = (juce::uint8)fmin((psDotSizeL-1.5f)*618.0f,255.0f);
            if (psDotSizeL > 1.5f) g.setColour(juce::Colour(0, 0, blueSpot));
            else if (psDotSizeL < 1.0f) psDotSizeL = 1.0f;
            g.fillRect((float)count*dx, (200.0f - peakL)*dy, psDotSizeL*dotWidth*dx, psDotSizeL*dotHeight*dy);
            g.fillRect((float)count*dx, (400.0f - slewL)*dy, psDotSizeL*dotWidth*dx, psDotSizeL*dotHeight*dy);
            g.fillRect((float)count*dx, (400.0f + meterZeroL)*dy, psDotSizeL*dotWidth*dx, psDotSizeL*dotHeight*dy);
        }
        
        
        if (peakR > 197.0f) {
            peakR = 197.0;
            g.setColour(juce::Colour(255, 0, 0));
            g.fillRect((float)count*dx, (200.0f - peakR)*dy, 2.0f*dx, 2.0f*dy);
            g.fillRect((float)count*dx, (400.0f - slewR)*dy, dx, dy);
            g.fillRect((float)count*dx, (400.0f + meterZeroR)*dy, dx, dy);

        } else if (peakR != 0.0f) {
            float psDotSizeR = 2.5f / (fabs((peakR-slewR)*0.08f)+1.0f);
            g.setColour(juce::Colours::black);
            juce::uint8 blueSpot = (juce::uint8)fmin((psDotSizeR-1.5f)*618.0f,255.0f);
            if (psDotSizeR > 1.5f) g.setColour(juce::Colour(0, 0, blueSpot));
            else if (psDotSizeR < 1.0f) psDotSizeR = 1.0f;
            g.fillRect((float)count*dx, (200.0f - peakR)*dy, psDotSizeR*dotWidth*dx, psDotSizeR*dotHeight*dy);
            g.fillRect((float)count*dx, (400.0f - slewR)*dy, psDotSizeR*dotWidth*dx, psDotSizeR*dotHeight*dy);
            g.fillRect((float)count*dx, (400.0f + meterZeroR)*dy, psDotSizeR*dotWidth*dx, psDotSizeR*dotHeight*dy);
            //done with peak, slew, zero cross
        }
    }
}
