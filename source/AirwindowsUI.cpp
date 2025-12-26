// AirwindowsUI by Chris Johnson
// Initial seed code for the meter created by Paul Walker on 8/23/21.
#include "AirwindowsUI.h"

void AirwindowsMeter::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::white); //blank screen before doing anything, unless our draw covers the whole display anyway
    //this is probably quick and optimized
    g.setColour(juce::Colours::lightgrey);
    g.fillRect(0,  60, getWidth(),1); // -6dB markings
    g.fillRect(0, 100, getWidth(),1); //-12dB markings
    g.fillRect(0, 130, getWidth(),1); //-18dB markings
    g.fillRect(0, 150, getWidth(),1); //-24dB markings
    g.fillRect(0, 160, getWidth(),1); //-30dB markings
    g.fillRect(0, 170, getWidth(),1); //-36dB markings

    g.fillRect(0, 302, getWidth(),1); //20k markings
    g.fillRect(0, 307, getWidth(),1); //2k markings
    g.fillRect(0, 328, getWidth(),1); //200hz markings
    g.fillRect(0, 367, getWidth(),1); //40hz markings
    
    g.setColour(juce::Colour(31, 31, 32));
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.8f));
    g.fillRect(0, 399, getWidth(), 99); //background that will be the chart

    g.setColour(juce::Colours::grey);
    g.setFont(16);
    g.drawText("peaks", 8, 3, displayWidth-11, 16, juce::Justification::bottomLeft);
    g.drawText("blue dot:projection of peak, red dot:brighter, green dot:darker", 0, 3, displayWidth-11, 16, juce::Justification::bottomRight);
    g.drawText("-6 dB", 0, 52, displayWidth-11, 16, juce::Justification::bottomRight);
    g.drawText("-12 dB", 0, 92, displayWidth-11, 16, juce::Justification::bottomRight);
    g.drawText("-18 dB", 0, 122, displayWidth-11, 16, juce::Justification::bottomRight);
    g.drawText("-24 dB", 0, 142, displayWidth-11, 16, juce::Justification::bottomRight);
    g.drawText("-30 dB", 0, 152, displayWidth-11, 16, juce::Justification::bottomRight);
    g.drawText("clip", 0, 182, displayWidth-11, 16, juce::Justification::bottomRight);
    
    g.drawText("slew", 8, 205, displayWidth-11, 16, juce::Justification::bottomLeft);
    g.drawText("same, arranged by frequency instead of loudness", 0, 205, displayWidth-11, 16, juce::Justification::bottomRight);
    g.drawText("overslew", 0, 282, displayWidth-11, 16, juce::Justification::bottomRight);
    
    g.drawText("zero cross", 8, 305, displayWidth-11, 16, juce::Justification::bottomLeft);
    g.drawText("lowest frequency without other sound interfering", 0, 305, displayWidth-11, 16, juce::Justification::bottomRight);
    g.drawText("200 hz", 0, 320, displayWidth-11, 16, juce::Justification::bottomRight);
    g.drawText("40 hz", 0, 359, displayWidth-11, 16, juce::Justification::bottomRight);
    g.drawText("chart", 8, 381, displayWidth-40, 16, juce::Justification::bottomLeft);
    g.drawText("blue:Sonority (blue dot density)  red:Novelty (blue dot range)  green:Intention (both plus balanced tone, minus RMS)", 8, 381, displayWidth-40, 16, juce::Justification::centredBottom);
    g.drawText("20 hz", 0, 381, displayWidth-11, 16, juce::Justification::bottomRight);

    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.618f));
    g.fillRect(0, 200, getWidth(), 1); // border with slew meter
    g.fillRect(0, 300, getWidth(), 1); // border with zero cross meter
    g.fillRect(0, 396, getWidth(), 1); //20hz markings is also border with color and charts
    
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(0, 201, getWidth(), 1); // border with slew meter
    g.fillRect(0, 301, getWidth(), 1); // border with zero cross meter
    g.fillRect(0, 397, getWidth(), 1); //20hz markings is also border with color and charts

    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.382f));
    g.fillRect(0, 202, getWidth(), 1); // border with slew meter
    g.fillRect(0, 302, getWidth(), 1); // border with zero cross meter
    g.fillRect(0, 398, getWidth(), 1); //20hz markings is also border with color and charts


    for (unsigned long count = 0; count < fmin(displayWidth,5150); ++count) //count through all the points in the array
    {
        g.setColour(juce::Colours::black);
        float psDotSizeL = 1.0f;
        float psDotSizeR = 1.0f;
        float psDotHypeL = 0.0f;
        float psDotHypeR = 0.0f;
        float psDotVibeL = 0.0f;
        float psDotVibeR = 0.0f;
        float peakL = dataC[count] * 200.0f;
        float peakR = dataD[count] * 200.0f;
        float slewL = sqrt(dataE[count])*300.0f;
        float slewR = sqrt(dataF[count])*300.0f;
        float meterZeroL = (sqrt(dataG[count])*6.0f)-6.0f;
        if (meterZeroL > 192.0f) meterZeroL = 192.0f;
        float meterZeroR = (sqrt(dataH[count])*6.0f)-6.0f;
        if (meterZeroR > 192.0f) meterZeroR = 192.0f;
        
        //begin draw dots on meters L
        if (peakL > 196.0f) {
            g.setColour(juce::Colour(255, 0, 0)); g.fillRect((int)count, 171, 1, 29);
            brightPeaks += 1.0f;
        } //peak is clipping!
        else if (peakL > 1.0f) { //peak isn't clipping, but is not literally zero so there's something here to work with
            psDotHypeL = (11.0f * sqrt(dataA[count] * dataB[count])) / (fabs(((peakL*((hype+6.0f)/7.0f))-slewL) * (7.0f/meterZeroL) )+1.0f);
            psDotVibeL = sin(pow(fmin(dataC[count]*8.5f,6.18f) / (fabs(((peakL*((hype+4.0f)/5.0f))-slewL) * (7.0f/meterZeroL) )+1.0f),1.618f)*0.13f) * 3.141592f;
            psDotSizeL = ((psDotVibeL*(1.0f-hype))+(psDotHypeL*hype))*(1.0f+(sin(hype*3.141592f)*0.25f));
            if (count == (unsigned long)dataPosition-1) {
                momentaryScoreL[count] = ((psDotSizeL*140.0f*(1.0f-hype))+(psDotSizeL*peakL*hype));
                maxScore += momentaryScoreL[count]*16.0f; //increase score
            }
            if (psDotSizeL > 1.0f) { //altering this changes the equation of what's 'loud' vs 'bright' or 'dark'
                g.setColour(juce::Colour::fromFloatRGBA(fmin((slewL-peakL)/256.0f,0.0f), fmin((peakL-slewL)/256.0f,0.0f), 1.0f, 1.0f)); midPeaks += 1.0f; //set COLOR for blue dots
                g.fillRect((int)count, (int)(200.0f - peakL), (int)fmax(psDotSizeL,2.0), (int)fmax(psDotSizeL,dataC[count]*5.0f));
            } else {
                if (slewL > peakL) {
                    g.setColour(juce::Colour::fromFloatRGBA(fmin((180.0f+(slewL-peakL))/256.0f,1.0f), 0.0f, 0.0f, 1.0f));
                    g.fillRect((int)count, (int)(200.0f - peakL), 2, 3);
                    brightPeaks += 1.0f;
                } else {
                    g.setColour(juce::Colour::fromFloatRGBA(0.0f, ((255.0f-(peakL-slewL))/256.0f), 0.0f, 1.0f));
                    g.fillRect((int)count, (int)(200.0f - peakL), (int)(9.0f*dataA[count]), (int)(9.0f*dataA[count]));
                    darkPeaks += 1.0f;
                } //set COLOR and DRAW red or green dots
           }
            if (slewL > 194.0f) g.fillRect((int)count, (int)(300.0f-fmin((slewL-194.0f)*0.5f,96.0f)), 1, (int)fmin((slewL-194.0f)*0.5f,96.0f));
            else {
                if (psDotSizeL < 1.0) {
                    if (slewL > peakL) g.fillRect((int)count, (int)(300.0f - (slewL*0.5f)), 3, 3);
                    else g.fillRect((int)count, (int)(300.0f - (slewL*0.5f)), 1, 1);
                }
                else g.fillRect((int)count, (int)(300.0f - (slewL*0.5f)), (int)fmax(psDotSizeL,2.0f), (int)fmax(psDotSizeL,2.0f)); //Drawing slew dots, L
            }
            g.fillRect((int)count, (int)(298.0f + (meterZeroL*0.5f)), (int)fmax(7.0f*dataA[count],1.0f), (int)fmax(7.0f*dataA[count],1.0f));
        } //end draw dots on meters L
        
        //begin draw dots on meters R
        if (peakR > 197.0f) {
            g.setColour(juce::Colour(255, 0, 0));g.fillRect((int)count, 171, 1, 29);
            brightPeaks += 1.0f;
        } //peak is clipping!
        else if (peakR > 1.0f) { //peak isn't clipping, but is not literally zero so there's something here to work with
            psDotHypeR = (11.0f * sqrt(dataB[count] * dataA[count])) / (fabs(((peakR*((hype+6.0f)/7.0f))-slewR) * (7.0f/meterZeroR) )+1.0f);
            psDotVibeR = sin(pow(fmin(dataD[count]*8.5f,6.18f) / (fabs(((peakR*((hype+4.0f)/5.0f))-slewR) * (7.0f/meterZeroR) )+1.0f),1.618f)*0.13f) * 3.141592f;
            psDotSizeR = ((psDotVibeR*(1.0f-hype))+(psDotHypeR*hype))*(1.0f+(sin(hype*3.141592f)*0.25f));
            if (count == (unsigned long)dataPosition-1) {
                momentaryScoreR[count] = ((psDotSizeR*140.0f*(1.0f-hype))+(psDotSizeR*peakR*hype));
                maxScore += momentaryScoreR[count]*16.0f; //increase score
            }
            if (psDotSizeR > 1.0f) { //altering this changes the equation of what's 'loud' vs 'bright' or 'dark'
                g.setColour(juce::Colour::fromFloatRGBA(fmin((slewR-peakR)/256.0f,0.0f), fmin((peakR-slewR)/256.0f,0.0f), 1.0f, 1.0f)); midPeaks += 1.0f; //set COLOR for blue dots
                g.fillRect((int)count, (int)(200.0f - peakR), (int)fmax(psDotSizeR,2.0), (int)fmax(psDotSizeR,dataD[count]*5.0f));
            } else {
                if (slewR > peakR) {
                    g.setColour(juce::Colour::fromFloatRGBA(fmin((180.0f+(slewR-peakR))/256.0f,1.0f), 0.0f, 0.0f, 1.0f));
                    g.fillRect((int)count, (int)(200.0f - peakR), 2, 3);
                    brightPeaks += 1.0f;
                } else {
                    g.setColour(juce::Colour::fromFloatRGBA(0.0f, ((255.0f-(peakR-slewR))/256.0f), 0.0f, 1.0f));
                    g.fillRect((int)count, (int)(200.0f - peakR), (int)(9.0f*dataB[count]), (int)(9.0f*dataB[count]));
                    darkPeaks += 1.0f;
                } //set COLOR and DRAW red or green dots
            }
            if (slewR > 194.0f) g.fillRect((int)count, (int)(300.0f-fmin((slewR-194.0f)*0.5f,96.0f)), 1, (int)fmin((slewR-194.0f)*0.5f,96.0f));
           else {
               if (psDotSizeR < 1.0) {
                   if (slewR > peakR) g.fillRect((int)count, (int)(300.0f - (slewR*0.5f)), 3, 3);
                   else g.fillRect((int)count, (int)(300.0f - (slewR*0.5f)), 1, 1);
               }
               else g.fillRect((int)count, (int)(300.0f - (slewR*0.5f)), (int)fmax(psDotSizeR,2.0f), (int)fmax(psDotSizeR,2.0f)); //Drawing slew dots, R
            }
            g.fillRect((int)count, (int)(298.0f + (meterZeroR*0.5f)), (int)fmax(7.0f*dataB[count],1.0f), (int)fmax(7.0f*dataB[count],1.0f));
        } //end draw dots on meters R
               
         
        if (count == (unsigned long)dataPosition-1) {
            float falloff = 0.9438f; //tweak this in one place!
            
            loudScore[count] = sqrt(maxScore)*2.0f; //trim to work with meter
            maxScore *= falloff; //how fast loudness falls off
            
            unsigned long peakLTracker = (unsigned long)(peakL * (0.005f*(float)peakBins));
            unsigned long peakRTracker = (unsigned long)(peakR * (0.005f*(float)peakBins));//converts 0-200 to 0-bin number for bins for novelty chart
            if (peakLTracker > 0 && peakLTracker <= peakBins) peakTrack[peakLTracker] += (psDotSizeL * loudScore[count]);
            if (peakRTracker > 0 && peakRTracker <= peakBins) peakTrack[peakRTracker] += (psDotSizeR * loudScore[count]);
            //and we've incremented each bin we hit by Dot Size, to incorporate our peak intensity stuff
            for (unsigned long binscale = 0; binscale < peakBins; ++binscale) {
                evennessNovelty += fmin(peakTrack[binscale],fmax((float)peakLTracker,(float)peakRTracker)*50.0f);
                //fmax causes high bins to clamp and stay active until the energy subsides.
                //it is allowing the bin number to act as the clamp limit
                peakTrack[binscale] *= falloff; //how fast each bin falls off
            }
            varietyScore[count] = sqrt(evennessNovelty);
            evennessNovelty *= falloff; //how fast novelty falls off
                      
            hitColor += pow(hue*0.5f,2.0f);
            hitColor *= falloff; //how fast hue correctness falls off
            
            evennessHype += (outGreen*hitColor);
            hypeScore[count] = fmin(sqrt(evennessHype),99.0f);
            evennessHype *= falloff; //how fast hype (final score) falls off
        }
        
        brightPeaks *= 0.996f; brightPeaksDisplay[count] = brightPeaks;
        midPeaks *= 0.996f; midPeaksDisplay[count] = midPeaks;
        darkPeaks *= 0.996f; darkPeaksDisplay[count] = darkPeaks; //make this able to track on the fly
        
        float totalPeaks = (brightPeaksDisplay[count] + midPeaksDisplay[count] + darkPeaksDisplay[count] )*0.01f;
        textScore = "bright:" + juce::String((int)(brightPeaksDisplay[count]/totalPeaks)) + "%    ";
        textScore = textScore + "sonority:" + juce::String((int)(midPeaksDisplay[count]/totalPeaks)) + "%    ";
        textScore = textScore + "dark:" + juce::String((int)(darkPeaksDisplay[count]/totalPeaks)) + "%";
        //generating metrics for bright, sonorous and dark peaks to calculate with
        
        float blueScore = 0.0f; if (loudScore[count] > 1.0f) blueScore = fmin(pow(loudScore[count],2.0f)*0.00009f,99.0f); //HERE is where to trim the blue line for loudness
        float redScore = 0.0f; if (varietyScore[count] > 1.0) redScore = fmin(pow(varietyScore[count],2.0f)*0.00083f,99.0f); //HERE is where to trim the red line for variety
        
        outGreen = pow(fmax((fmax(dataA[count],dataB[count])*-80.0f)+(redScore+blueScore),0.0f),1.618f)*0.075f;
        float scaleGreen = 90.0f + outGreen + ((redScore-blueScore)*3.0f); scaleGreen = pow(scaleGreen,1.618f)*0.0273f;
        int greenColor = (int)scaleGreen; if (greenColor < 0) greenColor = 0; if (greenColor > 255) greenColor = 255;
        
        g.setColour(juce::Colour((int)(darkPeaksDisplay[count]/(totalPeaks*0.5f)), greenColor, (int)(brightPeaksDisplay[count]/(totalPeaks*0.5f))));
        if (loudScore[count] > 0.01f) g.fillRect((int)count, 399, 2, 99);
        
        hue = juce::Colour((int)(darkPeaksDisplay[count]/(totalPeaks*0.5f)), greenColor, (int)(brightPeaksDisplay[count]/(totalPeaks*0.5f))).getHue();
        hue = hue-0.53333333f; if (hue < 0.0f) hue = fmax(1.0f+hue, 0.0f); else hue = 1.0f - hue; //our hue target
        
        g.setColour(juce::Colour((int)fmin(fmax((scaleGreen*2.2f)-230.0f,0.0f),255.0f), 255, (int)fmin(fmax((scaleGreen*2.2f)-230.0f,0.0f),255.0f)));
        //line for hype is green but goes white when it's over equally bright green
        if (hypeScore[count] > 1.0) g.fillRect((int)count,(int)(495.0f-hypeScore[count]), 2, 2);
        g.setColour(juce::Colour(0, 0, 255)); //line for loudness is blue
        if (loudScore[count] > 1.0) g.fillRect((int)count,(int)(495.0f-blueScore), 2, 2);
        g.setColour(juce::Colour(255, 0, 0)); //line for variety is red
        if (varietyScore[count] > 1.0) g.fillRect((int)count,(int)(495.0f-redScore), 2, 2);
        
        if (hypeScore[count]*0.38f > highestGrade) highestGrade += 1;
        if (highestGrade < 0) highestGrade = 0;
        if (highestGrade > 37) highestGrade = 37;
        switch (highestGrade) {
            case 0:
                rating = juce::String("FF (Calm)"); break;
            case 1:
                rating = juce::String("FF (Calm)"); break;
            case 2:
                rating = juce::String("FF (Calm)"); break;
            case 3:
                rating = juce::String("FE (Calm)"); break;
            case 4:
                rating = juce::String("FD (Calm)"); break;
            case 5:
                rating = juce::String("FC (Calm)"); break;
            case 6:
                rating = juce::String("FB (Calm)"); break;
            case 7:
                rating = juce::String("FA (Calm)"); break;
            case 8:
                rating = juce::String("EF (Calm)"); break;
            case 9:
                rating = juce::String("EE (Calm)"); break;
            case 10:
                rating = juce::String("ED (Calm)"); break;
            case 11:
                rating = juce::String("EC (Calm)"); break;
            case 12:
                rating = juce::String("EB (Calm)"); break;
            case 13:
                rating = juce::String("EA (Calm)"); break;
            case 14:
                rating = juce::String("DF (Calm)"); break;
            case 15:
                rating = juce::String("DE (Calm)"); break;
            case 16:
                rating = juce::String("DD (Calm)"); break;
            case 17:
                rating = juce::String("DC (Calm)"); break;
            case 18:
                rating = juce::String("DB (Calm)"); break;
            case 19:
                rating = juce::String("DA (Calm)"); break;
            case 20:
                rating = juce::String("CF (Serene)"); break;
            case 21:
                rating = juce::String("CE (Serene)"); break;
            case 22:
                rating = juce::String("CD (Serene)"); break;
            case 23:
                rating = juce::String("CC (Serene)"); break;
            case 24:
                rating = juce::String("CB (Serene)"); break;
            case 25:
                rating = juce::String("CA (Serene)"); break;
            case 26:
                rating = juce::String("BF (Groove)"); break;
            case 27:
                rating = juce::String("BE (Groove)"); break;
            case 28:
                rating = juce::String("BD (Powerful)"); break;
            case 29:
                rating = juce::String("BC (Powerful)"); break;
            case 30:
                rating = juce::String("BB (Forceful)"); break;
            case 31:
                rating = juce::String("BA (Forceful)"); break;
            case 32:
                rating = juce::String("AF (Legendary)"); break;
            case 33:
                rating = juce::String("AE (Legendary)"); break;
            case 34:
                rating = juce::String("AD (Transcendent)"); break;
            case 35:
                rating = juce::String("AC (Transcendent)"); break;
            case 36:
                rating = juce::String("AB (Transcendent)"); break;
            case 37:
                rating = juce::String("AA (Transcendent)"); break;
        } //this is our two letter score, incorporating all the measurements
    }
    
    g.setColour(juce::Colours::darkgrey);
    g.setFont(24);
    g.drawText(rating, 50, 174, displayWidth-70, 24, juce::Justification::centredBottom);
    g.setFont(18);
    g.drawText(textScore, 8, 280, displayWidth-40, 18, juce::Justification::centredBottom);
 
    g.setColour(juce::Colours::grey);
    g.fillRect(dataPosition, 0, 1, 399); //the moving line

    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.382f));
    g.fillRect(0, 0, getWidth(), 2);
    g.fillRect(0, 0, 2, getHeight());
     
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.618f));
    g.fillRect(2, getHeight()-2, getWidth(), 2);
    g.fillRect(getWidth()-2, 2, 2, getHeight()-2);
}
