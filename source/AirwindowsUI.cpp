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
    g.setColour(juce::Colours::darkgrey);
    g.setFont(23.0f); g.drawText(textScore, 7, 2, displayWidth-8, 24, juce::Justification::bottomLeft);

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
            g.setColour(juce::Colour(255.0f, 0, 0));
            g.fillRect((float)count*dx, (200.0f - peakL)*dy, dx, dataA[count]*dataA[count]*120.0f*dy);
            if (slewL > 197.0f) {
                slewL -= 197.0f;
                g.fillRect((float)count*dx, (400.0f - slewL)*dy, dx, slewL*dy);
            } else {
                g.setColour(juce::Colours::black);
                g.fillRect((float)count*dx, (400.0f - slewL)*dy, dx, dy);
            }
            g.setColour(juce::Colours::black);
            g.fillRect((float)count*dx, (400.0f + meterZeroL)*dy, dx, dy);

        } else if (peakL > 1.0f) {
            float psDotHypeL = (11.0f * sqrt(dataA[count] * dataB[count])) / (fabs(((peakL*((hype+6.0f)/7.0f))-slewL) * (7.0f/meterZeroL) )+1.0f);
            float psDotVibeL = sin(pow(fmin(dataC[count]*8.5f,6.18f) / (fabs(((peakL*((hype+4.0f)/5.0f))-slewL) * (7.0f/meterZeroL) )+1.0f),1.618f)*0.13f) * 3.141592f;
            float psDotSizeL = ((psDotVibeL*(1.0f-hype))+(psDotHypeL*hype))*(1.0+(sin(hype*3.141592f)*0.25f));
            if (count > dataPosition-2 && count < dataPosition) {
                momentaryScoreL[count] = ((psDotSizeL*140.0f*(1.0f-hype))+(psDotSizeL*peakL*hype)); //increase score
                maxScore += momentaryScoreL[count]; //increase score
            }
            
            g.setColour(juce::Colours::black);
            if (psDotSizeL > 1.0f) g.setColour(juce::Colour(0, 0, 255));
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
                if (psDotSizeL < 1.0) {
                    if (slewL > peakL) g.setColour(juce::Colour(255, 0, 0));
                    else g.setColour(juce::Colour(0, 234.0f, 0));
                    g.fillRect((float)count*dx, (400.0f - slewL)*dy, 2.0f*dotWidth*dx, 2.0f*dotHeight*dy);
                } else g.fillRect((float)count*dx, (400.0f - slewL)*dy, psDotSizeL*dotWidth*dx, psDotSizeL*dotHeight*dy);
                
            }
            if (psDotSizeL < 1.0) {
                g.setColour(juce::Colour(0, 0, 0));
                g.fillRect((float)count*dx, (400.0f + meterZeroL)*dy, 16.0f*dataA[count]*dataA[count]*dotWidth*dx, 16.0f*dataA[count]*dataA[count]*dotHeight*dy);
            } else g.fillRect((float)count*dx, (400.0f + meterZeroL)*dy, 16.0f*dataA[count]*dataA[count]*dotWidth*dx, 16.0f*dataA[count]*dataA[count]*dotHeight*dy);
        }
 
        if (peakR > 197.0f) {
            peakR = 197.0;
            g.setColour(juce::Colour(255.0f, 0, 0));
            g.fillRect((float)count*dx, (200.0f - peakR)*dy, dx, dataB[count]*dataB[count]*120.0f*dy);
            if (slewR > 197.0f) {
                slewR -= 197.0f;
                g.fillRect((float)count*dx, (400.0f - slewR)*dy, dx, slewR*dy);
            } else {
                g.setColour(juce::Colours::black);
                g.fillRect((float)count*dx, (400.0f - slewR)*dy, dx, dy);
            }
            g.setColour(juce::Colours::black);
            g.fillRect((float)count*dx, (400.0f + meterZeroR)*dy, dx, dy);

        } else if (peakR > 1.0f) {
            float psDotHypeR = (11.0f * sqrt(dataB[count] * dataA[count])) / (fabs(((peakR*((hype+6.0f)/7.0f))-slewR) * (7.0f/meterZeroR) )+1.0f);
            float psDotVibeR = sin(pow(fmin(dataD[count]*8.5f,6.18f) / (fabs(((peakR*((hype+4.0f)/5.0f))-slewR) * (7.0f/meterZeroR) )+1.0f),1.618f)*0.13f) * 3.141592f;
            float psDotSizeR = ((psDotVibeR*(1.0f-hype))+(psDotHypeR*hype))*(1.0+(sin(hype*3.141592f)*0.25f));
            if (count > dataPosition-2 && count < dataPosition) {
                momentaryScoreR[count] = ((psDotSizeR*140.0f*(1.0f-hype))+(psDotSizeR*peakR*hype)); //increase score
                maxScore += momentaryScoreR[count]; //increase score
  }
            
            g.setColour(juce::Colours::black);
            if (psDotSizeR > 1.0f) g.setColour(juce::Colour(0, 0, 255));
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
               if (psDotSizeR < 1.0) {
                   if (slewR > peakR) g.setColour(juce::Colour(255.0f, 0, 0));
                   else g.setColour(juce::Colour(0, 234.0f, 0));
                   g.fillRect((float)count*dx, (400.0f - slewR)*dy, 2.0f*dotWidth*dx, 2.0f*dotHeight*dy);
               } else g.fillRect((float)count*dx, (400.0f - slewR)*dy, psDotSizeR*dotWidth*dx, psDotSizeR*dotHeight*dy);
               
            }
            if (psDotSizeR < 1.0) {
                g.setColour(juce::Colour(0, 0, 0));
                g.fillRect((float)count*dx, (400.0f + meterZeroL)*dy, 16.0f*dataB[count]*dataB[count]*dotWidth*dx, 16.0f*dataB[count]*dataB[count]*dotHeight*dy);
            } else g.fillRect((float)count*dx, (400.0f + meterZeroR)*dy, 16.0f*dataB[count]*dataB[count]*dotWidth*dx, 16.0f*dataB[count]*dataB[count]*dotHeight*dy);
            //done with peak, slew, zero cross
        }
        
        if (count > dataPosition-2 && count < dataPosition) {
            hitScore[count] = sqrt(maxScore); lingerScore += (hitScore[count]); lingerScore *= 0.5f;
            maxScore = fmax(maxScore-lingerScore,0.0f);
        }
                
        if ((hitScore[count]*0.0602) > highestScore) highestScore = (int)(hitScore[count]*0.0602);
        //calibrated so, at default size and conditions, if you break into the upper RMS meter you break into B, then A etc.
        if (highestScore > 38) highestScore = 38;
        switch (highestScore) {
            case 0:
                textScore = juce::String("FF"); break;
            case 1:
                textScore = juce::String("FF"); break;
            case 2:
                textScore = juce::String("FF"); break;
            case 3:
                textScore = juce::String("FF"); break;
            case 4:
                textScore = juce::String("FE"); break;
            case 5:
                textScore = juce::String("FD"); break;
            case 6:
                textScore = juce::String("FC"); break;
            case 7:
                textScore = juce::String("FB"); break;
            case 8:
                textScore = juce::String("FA - ultimate chill"); break;
            case 9:
                textScore = juce::String("EF - ultimate chill"); break;
            case 10:
                textScore = juce::String("EE - ultimate chill"); break;
            case 11:
                textScore = juce::String("ED - even more chill"); break;
            case 12:
                textScore = juce::String("EC - even more chill"); break;
            case 13:
                textScore = juce::String("EB - even more chill"); break;
            case 14:
                textScore = juce::String("EA - even more chill"); break;
            case 15:
                textScore = juce::String("DF - Deepest Vibe"); break;
            case 16:
                textScore = juce::String("DE - Deep Vibe"); break;
            case 17:
                textScore = juce::String("DD - Deep Vibe"); break;
            case 18:
                textScore = juce::String("DC - Deep Vibe"); break;
            case 19:
                textScore = juce::String("DB - Deep Vibe"); break;
            case 20:
                textScore = juce::String("DA - Deep Vibe"); break;
            case 21:
                textScore = juce::String("CF - Comfortable"); break;
            case 22:
                textScore = juce::String("CE - Comfortable"); break;
            case 23:
                textScore = juce::String("CD - Comfortable"); break;
            case 24:
                textScore = juce::String("CC - Comfortable"); break;
            case 25:
                textScore = juce::String("CB - Comfortable"); break;
            case 26:
                textScore = juce::String("CA - Comfortable"); break;
            case 27:
                textScore = juce::String("BF - Biggest"); break;
            case 28:
                textScore = juce::String("BE - Biggest"); break;
            case 29:
                textScore = juce::String("BD - Biggest"); break;
            case 30:
                textScore = juce::String("BC - Biggest"); break;
            case 31:
                textScore = juce::String("BB - Biggest"); break;
            case 32:
                textScore = juce::String("BA - Biggest"); break;
            case 33:
                textScore = juce::String("AF - Attention"); break;
            case 34:
                textScore = juce::String("AE - Attention"); break;
            case 35:
                textScore = juce::String("AD - Attention"); break;
            case 36:
                textScore = juce::String("AC - Attention"); break;
            case 37:
                textScore = juce::String("AB - Attention"); break;
            case 38:
                textScore = juce::String("AA - Attention"); break;
        }
        //we are building the ability to assign a letter score
        
        float drawScore = pow(hitScore[count],2.0)*0.001f;
        smoothMomentary = (smoothMomentary*0.93f) + (fmax(((momentaryScoreL[count]+momentaryScoreR[count])*0.00155f),0.0f)*0.07f);
        g.setColour(juce::Colour(0, smoothMomentary*smoothMomentary*265, 0));
        if (smoothMomentary > 0.9) g.setColour(juce::Colour(0, smoothMomentary*110, 255));
        if (hitScore[count] > 1.0) g.fillRect((float)count*dx, (400.0f-drawScore)*dy, 1.1f*dotWidth*dx, fmin(hitScore[count]*0.013f,5.0f)*dotHeight*dy);
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
