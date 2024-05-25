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
    g.setColour(juce::Colours::lightgrey);
    g.setFont(18.0f); g.drawText(textScore, 7, 2, 200, 20, juce::Justification::bottomLeft);

    for (int count = 0; count < fmin(displayWidth,2000); ++count) //count through all the points in the array
    {
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

        } else if (peakL > 1.0f) {
            float psDotSizeL = (11.0f * sqrt(dataA[count] * dataB[count])) / (fabs((peakL-slewL) * (7.0f/meterZeroL) )+1.0f);
            if (count > dataPosition-2 && count < dataPosition) maxScore += (psDotSizeL*peakL); //increase score
            g.setColour(juce::Colours::black);
            if (psDotSizeL > 1.0f) g.setColour(juce::Colour(0, 0, 255.0f));
            else psDotSizeL = sqrt(psDotSizeL);
            g.fillRect((float)count*dx, (200.0f - peakL)*dy, psDotSizeL*dotWidth*dx, psDotSizeL*dotHeight*dy);
            if (slewL > 197.0f) {
                slewL -= 197.0f;
                juce::uint8 greySpike = (juce::uint8)fabs((dataA[count] * 690.0f)-slewL);
                g.setColour(juce::Colour(greySpike, greySpike, greySpike));
                g.fillRect((float)count*dx, (400.0f - slewL)*dy, psDotSizeL*dotWidth*dx, slewL*dy);
                //slew is so high we're drawing the spike that cuts across the RMS grey shadow
                if (psDotSizeL > 1.0f) g.setColour(juce::Colour(0, 0, 255.0f));
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

        } else if (peakR > 1.0f) {
            float psDotSizeR = (11.0f * sqrt(dataB[count] * dataA[count])) / (fabs((peakR-slewR) * (7.0f/meterZeroR) )+1.0f);
            if (count > dataPosition-2 && count < dataPosition) maxScore += (psDotSizeR*peakR); //increase score
            g.setColour(juce::Colours::black);
            if (psDotSizeR > 1.0f) g.setColour(juce::Colour(0, 0, 255.0f));
            else psDotSizeR = sqrt(psDotSizeR);
            g.fillRect((float)count*dx, (200.0f - peakR)*dy, psDotSizeR*dotWidth*dx, psDotSizeR*dotHeight*dy);
            if (slewR > 197.0f) {
                slewR -= 197.0f;
                juce::uint8 greySpike = (juce::uint8)fabs((dataB[count] * 690.0f)-slewR);
                g.setColour(juce::Colour(greySpike, greySpike, greySpike));
                g.fillRect((float)count*dx, (400.0f - slewR)*dy, psDotSizeR*dotWidth*dx, slewR*dy);
                //slew is so high we're drawing the spike that cuts across the RMS grey shadow
                if (psDotSizeR > 1.0f) g.setColour(juce::Colour(0, 0, 255.0f));
           } else {
                g.fillRect((float)count*dx, (400.0f - slewR)*dy, psDotSizeR*dotWidth*dx, psDotSizeR*dotHeight*dy);
            }
            g.fillRect((float)count*dx, (400.0f + meterZeroR)*dy, psDotSizeR*dotWidth*dx, psDotSizeR*dotHeight*dy);
            //done with peak, slew, zero cross
        }
        
        if (count > dataPosition-2 && count < dataPosition) {
            hitScore[count] = sqrt(maxScore); lingerScore += (hitScore[count]); lingerScore *= 0.5f;
            maxScore = fmax(maxScore-lingerScore,0.0f);
        }
        if ((hitScore[count]*0.0602) > highestScore) highestScore = (int)(hitScore[count]*0.0602);
        //calibrated so, at default size and conditions, if you break into the upper RMS meter you break into AA etc.
        if (highestScore > 39) highestScore = 39;
        switch (highestScore) {
            case 0:
                textScore = juce::String("Z"); break;
            case 1:
                textScore = juce::String("Y"); break;
            case 2:
                textScore = juce::String("X"); break;
            case 3:
                textScore = juce::String("W"); break;
            case 4:
                textScore = juce::String("V"); break;
            case 5:
                textScore = juce::String("U"); break;
            case 6:
                textScore = juce::String("T"); break;
            case 7:
                textScore = juce::String("S"); break;
            case 8:
                textScore = juce::String("R"); break;
            case 9:
                textScore = juce::String("Q"); break;
            case 10:
                textScore = juce::String("P"); break;
            case 11:
                textScore = juce::String("O"); break;
            case 12:
                textScore = juce::String("N"); break;
            case 13:
                textScore = juce::String("M"); break;
            case 14:
                textScore = juce::String("L"); break;
            case 15:
                textScore = juce::String("K"); break;
            case 16:
                textScore = juce::String("J"); break;
            case 17:
                textScore = juce::String("I"); break;
            case 18:
                textScore = juce::String("H"); break;
            case 19:
                textScore = juce::String("G"); break;
            case 20:
                textScore = juce::String("F"); break;
            case 21:
                textScore = juce::String("E"); break;
            case 22:
                textScore = juce::String("D"); break;
            case 23:
                textScore = juce::String("C"); break;
            case 24:
                textScore = juce::String("B"); break;
            case 25:
                textScore = juce::String("A"); break;
            case 26:
                textScore = juce::String("A"); break;
            case 27:
                textScore = juce::String("AA"); break;
            case 28:
                textScore = juce::String("AA"); break;
            case 29:
                textScore = juce::String("AA"); break;
            case 30:
                textScore = juce::String("AA"); break;
            case 31:
                textScore = juce::String("AAA"); break;
            case 32:
                textScore = juce::String("AAA"); break;
            case 33:
                textScore = juce::String("AAA"); break;
            case 34:
                textScore = juce::String("AAA"); break;
            case 35:
                textScore = juce::String("AAA"); break;
            case 36:
                textScore = juce::String("AAA"); break;
            case 37:
                textScore = juce::String("AAA"); break;
            case 38:
                textScore = juce::String("AAA"); break;
            case 39:
                textScore = juce::String("AAAA"); break;
        }
        //we are building the ability to assign a letter score
        
        float lineDotSize = 0.25+sqrt(dataA[count]*dataB[count]); //below the RMS level, line small
        float drawScore = pow(hitScore[count],2.0)*0.001f;
        if (drawScore > lineDotSize) lineDotSize *= 2.0f; //bumps up as it gets bigger
        if ((drawScore > peakL) && (drawScore > peakR)) lineDotSize *= 2.0f;
        g.setColour(juce::Colours::blue);
        if (hitScore[count] > 1.0) g.fillRect((float)count*dx, (400.0f-drawScore)*dy, lineDotSize*dotWidth*dx, lineDotSize*dotHeight*dy);
        }
    
    g.setColour(juce::Colours::grey);
    g.fillRect((float)dataPosition*dx, 0.0f, 1.0f, (float)getHeight()); //the moving line

    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.382f));
    g.fillRect(0, 0, getWidth(), linewidth);
    g.fillRect(0, 0, linewidth, getHeight());
     
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.618f));
    g.fillRect(linewidth, getHeight()-linewidth, getWidth(), linewidth);
    g.fillRect(getWidth()-linewidth, linewidth, linewidth, getHeight()-linewidth);
}
