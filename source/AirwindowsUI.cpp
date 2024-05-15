// AirwindowsUI by Chris Johnson
// Initial seed code for the meter created by Paul Walker on 8/23/21.
#include "AirwindowsUI.h"

void AirwindowsMeter::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::white); //blank screen before doing anything, unless our draw covers the whole display anyway
    //this is probably quick and optimized
    
    auto linewidth = getWidth();
    if (getHeight() > linewidth) linewidth = getHeight();
    linewidth = (int)cbrt(linewidth/2)/2;
    float dx = 1.0f;//(float)getWidth()/displayWidth; //width of individual pixel on this display object
    float dotWidth = 1.0f;//fmax(displayWidth/(float)getParentWidth(),1.0); //scale single pixel to roughly the right size
    if (displayWidth > 2000) {
        dx = (float)getWidth()/2000.0f; //width of individual pixel on this display object
        dotWidth = fmax(2000.0f/(float)getParentWidth(),1.0f); //scale single pixel to roughly the right size
    }
    float dy = 1.0f; //(float)getHeight()/600.0f; //height of individual pixel on this display object
    float dotHeight = 1.0f; //fmax(600.0f/(float)getParentHeight(),1.0); //scale single pixel to roughly the right size
    if (displayHeight > 720) {
        dy = (float)getHeight()/720.0f; //height of individual pixel on this display object
        dotHeight = fmax(720.0f/(float)getParentHeight(),1.0f); //scale single pixel to roughly the right size
    }
    g.setColour(juce::Colours::lightgrey);
    g.fillRect(0.0,  60.0f*dy, (float)getWidth(),1.0); // -6dB markings
    g.fillRect(0.0, 100.0f*dy, (float)getWidth(),1.0); //-12dB markings
    g.fillRect(0.0, 130.0f*dy, (float)getWidth(),1.0); //-18dB markings
    g.fillRect(0.0, 150.0f*dy, (float)getWidth(),1.0); //-24dB markings
    g.fillRect(0.0, 165.0f*dy, (float)getWidth(),1.0); //-30dB markings
    g.fillRect(0.0, 175.0f*dy, (float)getWidth(),1.0); //-36dB markings

    //g.fillRect(0.0, 260.0f*dy, (float)getWidth(),1.0); // -6dB markings for slew
    //g.fillRect(0.0, 300.0f*dy, (float)getWidth(),1.0); //-12dB markings
    //g.fillRect(0.0, 330.0f*dy, (float)getWidth(),1.0); //-18dB markings
    //g.fillRect(0.0, 350.0f*dy, (float)getWidth(),1.0); //-24dB markings
    //g.fillRect(0.0, 365.0f*dy, (float)getWidth(),1.0); //-30dB markings
    //g.fillRect(0.0, 375.0f*dy, (float)getWidth(),1.0); //-36dB markings
    //I've decided not to draw these on the slew section, but if you want them they're here.
    //Slew does not really have to do with these markings, only the pale grey RMS at bottom
    //and the intensity of the actual markings tells the main story about this section, not the lines.

    g.fillRect(0.0, 403.0f*dy, (float)getWidth(),1.0); //20k markings
    g.fillRect(0.0, 415.0f*dy, (float)getWidth(),1.0); //2k markings
    g.fillRect(0.0, 458.0f*dy, (float)getWidth(),1.0); //200hz markings
    g.fillRect(0.0, 593.0f*dy, (float)getWidth(),1.0); //20hz markings

    g.setColour(juce::Colours::black);
    g.fillRect(0.0,  200.0f*dy, (float)getWidth(),1.0); // border with slew meter
    g.fillRect(0.0,  400.0f*dy, (float)getWidth(),1.0); // border with zero cross meter

    for (int count = 0; count < fmin(displayWidth,2000); ++count) //count through all the points in the array
    {
        g.setColour(juce::Colours::grey);
        g.fillRect((float)dataPosition*dx, 0.0f, 1.0f, (float)getHeight()); //the moving line
        g.setColour(juce::Colours::black);

        float peakL = dataC[count] * 200.0f;
        float peakR = dataD[count] * 200.0f;
        float slewL = sqrt(dataE[count])*300.0f;
        float slewR = sqrt(dataF[count])*300.0f;
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
            g.setColour(juce::Colour::fromFloatRGBA (0.92f, 0.92f, 0.92f, 1.0f));
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
            g.setColour(juce::Colour::fromFloatRGBA (0.92f, 0.92f, 0.92f, 1.0f));
            g.fillRect((float)count*dx, (400.0f - h)*dy, dx, h*dy);
            lastROutline = h;
        } //done with RMS chunk and sound density

        
        if (peakL > 197.0f) {
            peakL = 197.0;
            g.setColour(juce::Colour(255, 0, 0));
            g.fillRect((float)count*dx, (200.0f - peakL)*dy, 2.0f*dx, 2.0f*dy);
            if (slewL > 197.0f) {
                slewL -= 197.0f;
                g.fillRect((float)count*dx, (400.0f - slewL)*dy, dx, slewL*dy);
            } else {
                g.fillRect((float)count*dx, (400.0f - slewL)*dy, dx, dy);
            }
            g.fillRect((float)count*dx, (400.0f + meterZeroL)*dy, dx, dy);

        } else if (peakL != 0.0f) {
            float psDotSizeL = 4.2f / (fabs((peakL-slewL)*0.5f)+1.0f);
            float minSizeL = fmaxf(sqrt(42.0f/(fabs(peakL-slewL)+1.0f)),1.0f);
            g.setColour(juce::Colours::black);
            juce::uint8 blueSpot = (juce::uint8)fmin((psDotSizeL)*512.0f,255.0f);
            if (psDotSizeL > 1.0f) g.setColour(juce::Colour(0, 0, blueSpot));
            else if (psDotSizeL < minSizeL) psDotSizeL = minSizeL;
            g.fillRect((float)count*dx, (200.0f - peakL)*dy, psDotSizeL*dotWidth*dx, psDotSizeL*dotHeight*dy);
            if (slewL > 197.0f) {
                slewL -= 197.0f;
                blueSpot = (juce::uint8)fabs((dataA[count] * 690.0f)-slewL);
                g.setColour(juce::Colour(blueSpot, blueSpot, blueSpot));
                g.fillRect((float)count*dx, (400.0f - slewL)*dy, psDotSizeL*dotWidth*dx, slewL*dy);
                //slew is so high we're drawing the spike that cuts across the RMS grey shadow
            } else {
                g.fillRect((float)count*dx, (400.0f - slewL)*dy, psDotSizeL*dotWidth*dx, psDotSizeL*dotHeight*dy);
            }
            g.fillRect((float)count*dx, (400.0f + meterZeroL)*dy, psDotSizeL*dotWidth*dx, psDotSizeL*dotHeight*dy);
        }
        
        
        if (peakR > 197.0f) {
            peakR = 197.0;
            g.setColour(juce::Colour(255, 0, 0));
            g.fillRect((float)count*dx, (200.0f - peakR)*dy, 2.0f*dx, 2.0f*dy);
            if (slewR > 197.0f) {
                slewR -= 197.0f;
                g.fillRect((float)count*dx, (400.0f - slewR)*dy, dx, slewR*dy);
            } else {
                g.fillRect((float)count*dx, (400.0f - slewR)*dy, dx, dy);
            }
            g.fillRect((float)count*dx, (400.0f + meterZeroR)*dy, dx, dy);

        } else if (peakR != 0.0f) {
            float psDotSizeR = 4.2f / (fabs((peakR-slewR)*0.5f)+1.0f);
            float minSizeR = fmaxf(sqrt(42.0f/(fabs(peakR-slewR)+1.0f)),1.0f);
           g.setColour(juce::Colours::black);
            juce::uint8 blueSpot = (juce::uint8)fmin((psDotSizeR)*512.0f,255.0f);
            if (psDotSizeR > 1.0f) g.setColour(juce::Colour(0, 0, blueSpot));
            else if (psDotSizeR < minSizeR) psDotSizeR = minSizeR;
            g.fillRect((float)count*dx, (200.0f - peakR)*dy, psDotSizeR*dotWidth*dx, psDotSizeR*dotHeight*dy);
            if (slewR > 197.0f) {
                slewR -= 197.0f;
                blueSpot = (juce::uint8)fabs((dataB[count] * 690.0f)-slewR);
                g.setColour(juce::Colour(blueSpot, blueSpot, blueSpot));
                g.fillRect((float)count*dx, (400.0f - slewR)*dy, psDotSizeR*dotWidth*dx, slewR*dy);
                //slew is so high we're drawing the spike that cuts across the RMS grey shadow
            } else {
                g.fillRect((float)count*dx, (400.0f - slewR)*dy, psDotSizeR*dotWidth*dx, psDotSizeR*dotHeight*dy);
            }
            g.fillRect((float)count*dx, (400.0f + meterZeroR)*dy, psDotSizeR*dotWidth*dx, psDotSizeR*dotHeight*dy);
            //done with peak, slew, zero cross
        }
    }
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.382f));
    g.fillRect(0, 0, getWidth(), linewidth);
    g.fillRect(0, 0, linewidth, getHeight());
     
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.618f));
    g.fillRect(linewidth, getHeight()-linewidth, getWidth(), linewidth);
    g.fillRect(getWidth()-linewidth, linewidth, linewidth, getHeight()-linewidth);
}
