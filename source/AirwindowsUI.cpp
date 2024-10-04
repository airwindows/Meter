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

    g.fillRect(0.0, 403.0f*dy, (float)getWidth(),1.0); //20k markings
    g.fillRect(0.0, 415.0f*dy, (float)getWidth(),1.0); //2k markings
    g.fillRect(0.0, 458.0f*dy, (float)getWidth(),1.0); //200hz markings
    
    g.setColour(juce::Colours::grey);
    g.setFont(18.0f);
    g.drawText("blue-loud", 8, 3, displayWidth-15, 18, juce::Justification::bottomLeft);
    g.drawText("peak loudness", 0, 3, displayWidth-15, 18, juce::Justification::bottomRight);
    g.drawText("-6 dB", 0, 50, displayWidth-15, 18, juce::Justification::bottomRight);
    g.drawText("-12 dB", 0, 90, displayWidth-15, 18, juce::Justification::bottomRight);
    g.drawText("-18 dB", 0, 120, displayWidth-15, 18, juce::Justification::bottomRight);
    g.drawText("-24 dB", 0, 140, displayWidth-15, 18, juce::Justification::bottomRight);
    g.drawText("-30 dB", 0, 155, displayWidth-15, 18, juce::Justification::bottomRight);
    g.drawText("clip", 0, 180, displayWidth-15, 18, juce::Justification::bottomRight);
    g.drawText("red-bright", 8, 200, displayWidth-15, 18, juce::Justification::bottomLeft);
    g.drawText("slew brightness", 0, 200, displayWidth-15, 18, juce::Justification::bottomRight);
    g.drawText("overslew", 0, 380, displayWidth-15, 18, juce::Justification::bottomRight);
    g.drawText("green-dark", 8, 400, displayWidth-15, 18, juce::Justification::bottomLeft);
    g.drawText("zero cross bass", 0, 400, displayWidth-15, 18, juce::Justification::bottomRight);

    g.setColour(juce::Colours::black);
    g.fillRect(0.0, 593.0f*dy, (float)getWidth(),1.0); //20hz markings is also border with zero cross meter
    g.fillRect(0.0,  200.0f*dy, (float)getWidth(),1.0); // border with slew meter
    g.fillRect(0.0,  400.0f*dy, (float)getWidth(),1.0); // border with zero cross meter

    for (int count = 0; count < fmin(displayWidth,2000); ++count) //count through all the points in the array
    {
        g.setColour(juce::Colours::black);
        float psDotSizeL = 0.0;
        float psDotSizeR = 0.0; //define these here so we can use them with the evenness meter
        float peakL = dataC[count] * 200.0f;
        float peakR = dataD[count] * 200.0f;
        float slewL = sqrt(dataE[count])*300.0f;
        float slewR = sqrt(dataF[count])*300.0f;
        float meterZeroL = (sqrt(dataG[count])*6.0f)-6.0f;
        if (meterZeroL > 192.0f) meterZeroL = 192.0f;
        float meterZeroR = (sqrt(dataH[count])*6.0f)-6.0f;
        if (meterZeroR > 192.0f) meterZeroR = 192.0f;
        
        if (peakL > 197.0f) { //Peak is clipping!
            g.setColour(juce::Colour(255, 0, 0));
            g.fillRect((float)count*dx, 176.0f*dy, dx, 24.0f*dy);
        } else if (peakL > 1.0f) {
            float psDotHypeL = (11.0f * sqrt(dataA[count] * dataB[count])) / (fabs(((peakL*((hype+6.0f)/7.0f))-slewL) * (7.0f/meterZeroL) )+1.0f);
            float psDotVibeL = sin(pow(fmin(dataC[count]*8.5f,6.18f) / (fabs(((peakL*((hype+4.0f)/5.0f))-slewL) * (7.0f/meterZeroL) )+1.0f),1.618f)*0.13f) * 3.141592f;
            psDotSizeL = ((psDotVibeL*(1.0f-hype))+(psDotHypeL*hype))*(1.0f+(sin(hype*3.141592f)*0.25f));
            if (count > dataPosition-2 && count < dataPosition) {
                momentaryScoreL[count] = ((psDotSizeL*140.0f*(1.0f-hype))+(psDotSizeL*peakL*hype)); //increase score
                maxScore += momentaryScoreL[count]; //increase score
            }
            if (psDotSizeL > 1.0f) {
                g.setColour(juce::Colour((juce::uint8)fmin((slewL-peakL),0.0f), (juce::uint8)fmin((peakL-slewL),0.0f), 255)); midPeaks += 1.0f; //set COLOR for blue dots, when loud peaks are happening
                g.fillRect((float)count*dx, (200.0f - peakL)*dy, fmax(psDotSizeL,1.618f)*dotWidth*dx, fmax(psDotSizeL,1.618f)*dotHeight*dy);
            } else {
                if (slewL > peakL) {
                    g.setColour(juce::Colour((juce::uint8)fmin(180.0f+(slewL-peakL),255.0f), 0, 0)); brightPeaks += 1.0f; //set COLOR for red dots, when bright peaks are happening
                } else {
                    g.setColour(juce::Colour(0, (juce::uint8)(255.0f-(peakL-slewL)), 0)); darkPeaks += 1.0f;   //set COLOR for green dots, when dark peaks are happening
                }
                g.fillRect((float)count*dx, (200.0f - peakL)*dy, 1.618f*dotWidth*dx, 1.618f*dotHeight*dy);
           }
            if (slewL > 197.0f) {
                g.fillRect((float)count*dx, (399.0f - (sqrt(slewL-197.0f)*1.618f))*dy, 1.618f*dotWidth*dx, sqrt(slewL-197.0f)*1.618f*dy);
            } else {
                if (psDotSizeL < 1.0) {
                    g.fillRect((float)count*dx, (400.0f - slewL)*dy, 1.618f*dotWidth*dx, 1.618f*dotHeight*dy);
                } else {
                    g.fillRect((float)count*dx, (400.0f - slewL)*dy, fmax(psDotSizeL,1.618f)*dotWidth*dx, fmax(psDotSizeL,1.618f)*dotHeight*dy);
                }
            }
            g.fillRect((float)count*dx, (400.0f + meterZeroL)*dy, fmax(8.0f*dataA[count],1.618f)*dotWidth*dx, fmax(8.0f*dataA[count],1.618f)*dotHeight*dy);
        }
        
        if (peakR > 197.0f) { //Peak is clipping!
            g.setColour(juce::Colour(255, 0, 0));
            g.fillRect((float)count*dx, 176.0f*dy, dx, 24.0f*dy);
        } else if (peakR > 1.0f) {
            float psDotHypeR = (11.0f * sqrt(dataB[count] * dataA[count])) / (fabs(((peakR*((hype+6.0f)/7.0f))-slewR) * (7.0f/meterZeroR) )+1.0f);
            float psDotVibeR = sin(pow(fmin(dataD[count]*8.5f,6.18f) / (fabs(((peakR*((hype+4.0f)/5.0f))-slewR) * (7.0f/meterZeroR) )+1.0f),1.618f)*0.13f) * 3.141592f;
            psDotSizeR = ((psDotVibeR*(1.0f-hype))+(psDotHypeR*hype))*(1.0f+(sin(hype*3.141592f)*0.25f));
            if (count > dataPosition-2 && count < dataPosition) {
                momentaryScoreR[count] = ((psDotSizeR*140.0f*(1.0f-hype))+(psDotSizeR*peakR*hype)); //increase score
                maxScore += momentaryScoreR[count]; //increase score
            }
            if (psDotSizeR > 1.0f) {
                g.setColour(juce::Colour((juce::uint8)fmin((slewR-peakR),0.0f), (juce::uint8)fmin((peakR-slewR),0.0f), 255)); midPeaks += 1.0f; //set COLOR for blue dots, when loud peaks are happening
                g.fillRect((float)count*dx, (200.0f - peakR)*dy, fmax(psDotSizeR,1.618f)*dotWidth*dx, fmax(psDotSizeR,1.618f)*dotHeight*dy);
            } else {
                if (slewR > peakR) {
                    g.setColour(juce::Colour((juce::uint8)fmin(180.0f+(slewR-peakR),255.0f), 0, 0)); brightPeaks += 1.0f; //set COLOR for red dots, when bright peaks are happening
                } else {
                    g.setColour(juce::Colour(0, (juce::uint8)(255.0f-(peakR-slewR)), 0)); darkPeaks += 1.0f;   //set COLOR for green dots, when dark peaks are happening
                }
                g.fillRect((float)count*dx, (200.0f - peakR)*dy, 1.618f*dotWidth*dx, 1.618f*dotHeight*dy);
            }
            if (slewR > 197.0f) {
                g.fillRect((float)count*dx, (399.0f - (sqrt(slewR-197.0f)*1.618f))*dy, 1.618f*dotWidth*dx, sqrt(slewR-197.0f)*1.618f*dy);
           } else {
               if (psDotSizeR < 1.0) {
                   g.fillRect((float)count*dx, (400.0f - slewR)*dy, 1.618f*dotWidth*dx, 1.618f*dotHeight*dy);
               } else {
                   g.fillRect((float)count*dx, (400.0f - slewR)*dy, fmax(psDotSizeR,1.618f)*dotWidth*dx, fmax(psDotSizeR,1.618f)*dotHeight*dy);
               } //Drawing slew dots, R
               
            }
            g.fillRect((float)count*dx, (400.0f + meterZeroR)*dy, fmax(8.0f*dataB[count],1.618f)*dotWidth*dx, fmax(8.0f*dataB[count],1.618f)*dotHeight*dy);
            //done with peak, slew, zero cross
        }
               
        int peakLTracker = (int)(peakL * (0.005f*(float)peakBins));
        int peakRTracker = (int)(peakR * (0.005f*(float)peakBins));//converts 0-200 to 0-bin number for bins
        if (peakLTracker > 0 && peakLTracker <= peakBins) peakTrack[peakLTracker] += (psDotSizeL * sqrt(maxScore) * 0.0002);
        if (peakRTracker > 0 && peakRTracker <= peakBins) peakTrack[peakRTracker] += (psDotSizeR * sqrt(maxScore) * 0.0002);
        //and we've incremented each bin we hit by Dot Size, to incorporate our peak intensity stuff
        evennessScore *= 0.98f;
        //this is slowing the response, in theory, and scaling the whole thing by making it fall back
        for (int binscale = 0; binscale < peakBins; ++binscale) {
            evennessScore += fmin(peakTrack[binscale],fmax(peakLTracker,peakRTracker)*0.06f);
            //larger min for more intense curve boosting
            //this causes high bins to clamp and stay active until the energy subsides.
            peakTrack[binscale] *= 0.98f;
        }
        
        if (count > dataPosition-2 && count < dataPosition) {
            peakScore[count] = evennessScore;
            loudScore[count] = sqrt(maxScore); lingerScore += (loudScore[count]);
            lingerScore *= 0.5f;
            maxScore = fmax(maxScore-lingerScore,0.0f);
            hitScore[count] = (((highestPeakScore - (highestScore*0.618033988749894f))+26.0f)*0.9f) - (fabs((brightPeaks-darkPeaks)/((brightPeaks+midPeaks+darkPeaks)*0.01f))*0.6f);
        }
                
        if ((loudScore[count]*0.0602) > highestScore) highestScore = (int)(loudScore[count]*0.0602);
        if (pow(peakScore[count],2)*0.000387 > highestPeakScore) highestPeakScore = (int)(pow(peakScore[count],2)*0.000387);
        float hypeScore = (highestPeakScore - (highestScore*0.618033988749894f))+24.0f;
        if (highestScore > 38) highestScore = 38;
        if (highestPeakScore > 49) highestPeakScore = 49;
        textScore = juce::String("Novelty: ");
        switch (highestPeakScore) {
            case 0:
                textScore = textScore + juce::String("FF-Stable"); break;
            case 1:
                textScore = textScore + juce::String("FF-Stable"); break;
            case 2:
                textScore = textScore + juce::String("FF-Stable"); break;
            case 3:
                textScore = textScore + juce::String("FF-Stable"); break;
            case 4:
                textScore = textScore + juce::String("FE-Stable"); break;
            case 5:
                textScore = textScore + juce::String("FD-Stable"); break;
            case 6:
                textScore = textScore + juce::String("FC-Stable"); break;
            case 7:
                textScore = textScore + juce::String("FB-Stable"); break;
            case 8:
                textScore = textScore + juce::String("FA-Stable"); break;
            case 9:
                textScore = textScore + juce::String("EF-Steady"); break;
            case 10:
                textScore = textScore + juce::String("EE-Steady"); break;
            case 11:
                textScore = textScore + juce::String("ED-Steady"); break;
            case 12:
                textScore = textScore + juce::String("EC-Steady"); break;
            case 13:
                textScore = textScore + juce::String("EB-Steady"); break;
            case 14:
                textScore = textScore + juce::String("EA-Steady"); break;
            case 15:
                textScore = textScore + juce::String("DF-Evolving"); break;
            case 16:
                textScore = textScore + juce::String("DE-Evolving"); break;
            case 17:
                textScore = textScore + juce::String("DD-Evolving"); break;
            case 18:
                textScore = textScore + juce::String("DC-Evolving"); break;
            case 19:
                textScore = textScore + juce::String("DB-Evolving"); break;
            case 20:
                textScore = textScore + juce::String("DA-Evolving"); break;
            case 21:
                textScore = textScore + juce::String("CF-Variations"); break;
            case 22:
                textScore = textScore + juce::String("CE-Variations"); break;
            case 23:
                textScore = textScore + juce::String("CD-Variations"); break;
            case 24:
                textScore = textScore + juce::String("CC-Variations"); break;
            case 25:
                textScore = textScore + juce::String("CB-Variations"); break;
            case 26:
                textScore = textScore + juce::String("CA-Variations"); break;
            case 27:
                textScore = textScore + juce::String("BF-Contrasts"); break;
            case 28:
                textScore = textScore + juce::String("BE-Contrasts"); break;
            case 29:
                textScore = textScore + juce::String("BD-Contrasts"); break;
            case 30:
                textScore = textScore + juce::String("BC-Contrasts"); break;
            case 31:
                textScore = textScore + juce::String("BB-Contrasts"); break;
            case 32:
                textScore = textScore + juce::String("BA-Contrasts"); break;
            case 33:
                textScore = textScore + juce::String("AF-Startling"); break;
            case 34:
                textScore = textScore + juce::String("AE-Startling"); break;
            case 35:
                textScore = textScore + juce::String("AD-Startling"); break;
            case 36:
                textScore = textScore + juce::String("AC-Startling"); break;
            case 37:
                textScore = textScore + juce::String("AB-Startling"); break;
            case 38:
                textScore = textScore + juce::String("AA-Startling"); break;
            case 39:
                textScore = textScore + juce::String("AA-Startling"); break;
            case 40:
                textScore = textScore + juce::String("AA-Startling"); break;
            case 41:
                textScore = textScore + juce::String("AA-Startling"); break;
            case 42:
                textScore = textScore + juce::String("AA-Startling"); break;
            case 43:
                textScore = textScore + juce::String("AA-Startling"); break;
            case 44:
                textScore = textScore + juce::String("AA-Startling"); break;
            case 45:
                textScore = textScore + juce::String("AA-Absurd"); break;
            case 46:
                textScore = textScore + juce::String("AA-Absurd"); break;
            case 47:
                textScore = textScore + juce::String("AA-Absurd"); break;
            case 48:
                textScore = textScore + juce::String("AA-Absurd"); break;
            case 49:
                textScore = textScore + juce::String("AA-Absurd"); break;
        }
        textScore = textScore + juce::String("  Loudness: ");
        switch (highestScore) {
            case 0:
                textScore = textScore + juce::String("FF-Quiet"); break;
            case 1:
                textScore = textScore + juce::String("FF-Quiet"); break;
            case 2:
                textScore = textScore + juce::String("FF-Quiet"); break;
            case 3:
                textScore = textScore + juce::String("FF-Quiet"); break;
            case 4:
                textScore = textScore + juce::String("FE-Quiet"); break;
            case 5:
                textScore = textScore + juce::String("FD-Quiet"); break;
            case 6:
                textScore = textScore + juce::String("FC-Quiet"); break;
            case 7:
                textScore = textScore + juce::String("FB-Quiet"); break;
            case 8:
                textScore = textScore + juce::String("FA-Quiet"); break;
            case 9:
                textScore = textScore + juce::String("EF-Quiet"); break;
            case 10:
                textScore = textScore + juce::String("EE-Quiet"); break;
            case 11:
                textScore = textScore + juce::String("ED-Soft"); break;
            case 12:
                textScore = textScore + juce::String("EC-Soft"); break;
            case 13:
                textScore = textScore + juce::String("EB-Soft"); break;
            case 14:
                textScore = textScore + juce::String("EA-Soft"); break;
            case 15:
                textScore = textScore + juce::String("DF-Spacious"); break;
            case 16:
                textScore = textScore + juce::String("DE-Spacious"); break;
            case 17:
                textScore = textScore + juce::String("DD-Spacious"); break;
            case 18:
                textScore = textScore + juce::String("DC-Spacious"); break;
            case 19:
                textScore = textScore + juce::String("DB-Relaxed"); break;
            case 20:
                textScore = textScore + juce::String("DA-Relaxed"); break;
            case 21:
                textScore = textScore + juce::String("CF-Relaxed"); break;
            case 22:
                textScore = textScore + juce::String("CE-Relaxed"); break;
            case 23:
                textScore = textScore + juce::String("CD-Popular"); break;
            case 24:
                textScore = textScore + juce::String("CC-Popular"); break;
            case 25:
                textScore = textScore + juce::String("CB-Popular"); break;
            case 26:
                textScore = textScore + juce::String("CA-Popular"); break;
            case 27:
                textScore = textScore + juce::String("BF-Strong"); break;
            case 28:
                textScore = textScore + juce::String("BE-Strong"); break;
            case 29:
                textScore = textScore + juce::String("BD-Strong"); break;
            case 30:
                textScore = textScore + juce::String("BC-Strong"); break;
            case 31:
                textScore = textScore + juce::String("BB-Loud"); break;
            case 32:
                textScore = textScore + juce::String("BA-Loud"); break;
            case 33:
                textScore = textScore + juce::String("AF-Loud"); break;
            case 34:
                textScore = textScore + juce::String("AE-Loud"); break;
            case 35:
                textScore = textScore + juce::String("AD-Aggro"); break;
            case 36:
                textScore = textScore + juce::String("AC-Aggro"); break;
            case 37:
                textScore = textScore + juce::String("AB-Aggro"); break;
            case 38:
                textScore = textScore + juce::String("AA-Aggro"); break;
        }
        //we are building the ability to assign a letter score
        float totalPeaks = (brightPeaks + midPeaks + darkPeaks)*0.01f;
        textScore = textScore + "  Tone: Bright " + juce::String((int)(brightPeaks/totalPeaks)) + "% ";
        textScore = textScore + "Loud " + juce::String((int)(midPeaks/totalPeaks)) + "% ";
        textScore = textScore + "Dark " + juce::String((int)(darkPeaks/totalPeaks)) + "%";
        
        //next, generate hypescore minus fabs(brightPeaks-darkPeaks)* some scaling factor, and do another case statement with just a two letter score to sort by
        int finalGrade = (int)((hypeScore*0.9f)-(fabs((brightPeaks-darkPeaks)/totalPeaks)*0.6f));
        if (highestGrade < finalGrade) highestGrade = finalGrade;
        if (highestGrade < 0) highestGrade = 0;
        if (highestGrade > 38) highestGrade = 38;
         switch (highestGrade) {
            case 0:
                rating = juce::String("  "); break;
            case 1:
                rating = juce::String("FF"); break;
            case 2:
                rating = juce::String("FF"); break;
            case 3:
                rating = juce::String("FF"); break;
            case 4:
                rating = juce::String("FE"); break;
            case 5:
                rating = juce::String("FD"); break;
            case 6:
                rating = juce::String("FC"); break;
            case 7:
                rating = juce::String("FB"); break;
            case 8:
                rating = juce::String("FA"); break;
            case 9:
                rating = juce::String("EF"); break;
            case 10:
                rating = juce::String("EE"); break;
            case 11:
                rating = juce::String("ED"); break;
            case 12:
                rating = juce::String("EC"); break;
            case 13:
                rating = juce::String("EB"); break;
            case 14:
                rating = juce::String("EA"); break;
            case 15:
                rating = juce::String("DF"); break;
            case 16:
                rating = juce::String("DE"); break;
            case 17:
                rating = juce::String("DD"); break;
            case 18:
                rating = juce::String("DC"); break;
            case 19:
                rating = juce::String("DB"); break;
            case 20:
                rating = juce::String("DA"); break;
            case 21:
                rating = juce::String("CF"); break;
            case 22:
                rating = juce::String("CE"); break;
            case 23:
                rating = juce::String("CD"); break;
            case 24:
                rating = juce::String("CC"); break;
            case 25:
                rating = juce::String("CB"); break;
            case 26:
                rating = juce::String("CA"); break;
            case 27:
                rating = juce::String("BF"); break;
            case 28:
                rating = juce::String("BE"); break;
            case 29:
                rating = juce::String("BD"); break;
            case 30:
                rating = juce::String("BC"); break;
            case 31:
                rating = juce::String("BB"); break;
            case 32:
                rating = juce::String("BA"); break;
            case 33:
                rating = juce::String("AF"); break;
            case 34:
                rating = juce::String("AE"); break;
            case 35:
                rating = juce::String("AD"); break;
            case 36:
                rating = juce::String("AC"); break;
            case 37:
                rating = juce::String("AB"); break;
            case 38:
                rating = juce::String("AA"); break;
        }

        float drawScore = pow(loudScore[count],3.0f)*0.00000028f;
        if (drawScore > 70) drawScore = 70;
        g.setColour(juce::Colour(0, 0, 220)); //line for loudness is blue
        if (loudScore[count] > 1.0) g.fillRect((float)count*dx, (664.0f-drawScore)*dy, dx, sqrt(drawScore/7.0f)*dy);
        drawScore = 0.0;
        
        if (peakScore[count] > 1.0) drawScore = pow(peakScore[count],2.0f)*0.00069f;
        if (drawScore > 70) drawScore = 70;
        g.setColour(juce::Colour(240, 64, 0)); //line for variety is redorange
        if (peakScore[count] > 1.0) g.fillRect((float)count*dx,(664.0f-drawScore)*dy, dx, sqrt(drawScore/7.0f)*dy);
        drawScore = 0.0;
        
        if (hitScore[count] > 1.0) drawScore = pow(hitScore[count],2.0f)*0.05f;
        if (drawScore > 70) drawScore = 70;
        g.setColour(juce::Colour(0, 130, 0)); //line for letter score is a dark green
        if (hitScore[count] > 1.0) g.fillRect((float)count*dx,(664.0f-drawScore)*dy, dx, sqrt(drawScore/7.0f)*dy);
    }
    g.setFont(18.0f);
    g.setColour(juce::Colours::darkgrey);
    g.drawText(textScore, 7, 178, displayWidth-8, 18, juce::Justification::centredBottom);
    g.setFont(48.0f);
    g.setColour(juce::Colours::black);
    g.drawText(rating, 7, 198, displayWidth-8, 48, juce::Justification::centredBottom);

    g.setColour(juce::Colours::grey);
    g.fillRect((float)dataPosition*dx, 0.0f, 1.0f, (float)getHeight()); //the moving line

    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.382f));
    g.fillRect(0, 0, getWidth(), linewidth);
    g.fillRect(0, 0, linewidth, getHeight());
     
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.618f));
    g.fillRect(linewidth, getHeight()-linewidth, getWidth(), linewidth);
    g.fillRect(getWidth()-linewidth, linewidth, linewidth, getHeight()-linewidth);
}
