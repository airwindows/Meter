// AirwindowsUI by Chris Johnson
// Initial seed code for the meter created by Paul Walker on 8/23/21.
#include "AirwindowsUI.h"

void AirwindowsMeter::paint(juce::Graphics &g)
{
    float vS = displayHeight/600.0f; // short for vScale: everything * this
    g.fillAll(juce::Colours::white); //blank screen before doing anything, unless our draw covers the whole display anyway
    //this is probably quick and optimized
    g.setColour(juce::Colours::lightgrey);
    g.fillRect(0,  (int)(60.0f*vS), getWidth(),1); // -6dB markings
    g.fillRect(0, (int)(101.02*vS), getWidth(),1); //-12dB markings
    g.fillRect(0, (int)(130.02f*vS), getWidth(),1); //-18dB markings
    g.fillRect(0, (int)(150.2f*vS), getWidth(),1); //-24dB markings
    g.fillRect(0, (int)(164.9f*vS), getWidth(),1); //-30dB markings
    g.fillRect(0, (int)(175.2f*vS), getWidth(),1); //-36dB markings
    g.fillRect(0, (int)(182.5f*vS), getWidth(),1); //-42dB markings

    g.fillRect(0,  (int)(260.0f*vS), getWidth(),1); // -6dB markings
    g.fillRect(0, (int)(301.02*vS), getWidth(),1); //-12dB markings
    g.fillRect(0, (int)(330.02f*vS), getWidth(),1); //-18dB markings
    g.fillRect(0, (int)(350.2f*vS), getWidth(),1); //-24dB markings
    g.fillRect(0, (int)(364.9f*vS), getWidth(),1); //-30dB markings
    g.fillRect(0, (int)(375.2f*vS), getWidth(),1); //-36dB markings
    g.fillRect(0, (int)(382.5f*vS), getWidth(),1); //-42dB markings

    g.fillRect(0,  (int)(460.0f*vS), getWidth(),1); // -6dB markings
    g.fillRect(0, (int)(501.02*vS), getWidth(),1); //-12dB markings
    g.fillRect(0, (int)(530.02f*vS), getWidth(),1); //-18dB markings
    g.fillRect(0, (int)(550.2f*vS), getWidth(),1); //-24dB markings
    g.fillRect(0, (int)(564.9f*vS), getWidth(),1); //-30dB markings
    g.fillRect(0, (int)(575.2f*vS), getWidth(),1); //-36dB markings
    g.fillRect(0, (int)(582.5f*vS), getWidth(),1); //-42dB markings

    for (unsigned long count = 0; count < fmin(displayWidth,5150); ++count) //count through all the points in the array
    {
        g.setColour(juce::Colours::black);
        float psDotSizeL = 1.0f;
        float psDotSizeR = 1.0f;
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
            g.setColour(juce::Colour(255, 0, 0));
            g.fillRect((float)count, 1.0f, fmin(sqrt(peakL-196.0f),196.0f), (fmin((peakL-196.0f),196.0f)*vS));
        } //peak is clipping!
        else if (peakL > 1.0f) { //peak isn't clipping, but is not literally zero so there's something here to work with
            psDotSizeL = (8.3144112499811f * sqrt(dataA[count] * dataB[count])) / (fabs(((peakL*((6.6180339887f)/7.0f))-slewL) * (7.0f/meterZeroL) )+1.0f);
            psDotSizeL += sin(pow(fmin(dataC[count]*8.5f,6.18f) / (fabs(((peakL*((4.6180339887f)/5.0f))-slewL) * (7.0f/meterZeroL) )+1.0f),1.618f)*0.13f) * 1.467577515170776f;
            if (count == (unsigned long)dataPosition-1) {
                momentaryScoreL[count] = ((psDotSizeL*140.0f*(1.0f-hype))+(psDotSizeL*peakL*hype));
                maxScore += momentaryScoreL[count]*16.0f; //increase score
            }
            if (psDotSizeL > 1.0f) {
                g.setColour(juce::Colour::fromFloatRGBA(fmin((slewL-peakL)/256.0f,0.0f), fmin((peakL-slewL)/256.0f,0.0f), 1.0f, 1.0f));
                midPeaks += 1.0f;
            } else if (slewL > peakL) {
                g.setColour(juce::Colour::fromFloatRGBA(fmin((180.0f+(slewL-peakL))/256.0f,1.0f), 0.0f, 0.0f, 1.0f));
                brightPeaks += 1.0f;
            } else {
                g.setColour(juce::Colour::fromFloatRGBA(0.0f, ((255.0f-(peakL-slewL))/256.0f), 0.0f, 1.0f));
                darkPeaks += 1.0f;
            } //set COLOR
            
            if (psDotSizeL > 1.0f) g.fillRect((float)count, (float)((200.0f - peakL)*vS), (float)fmax(psDotSizeL,2.0), (float)(fmax(psDotSizeL,dataC[count]*5.0f)*vS)); //draw blue dots
            else if (slewL > peakL) g.fillRect((float)count, (float)((200.0f - peakL)*vS), 2.0f, (float)(3.0*vS));
            else g.fillRect((float)count, (float)((200.0f - peakL)*vS), (float)(9.0f*dataA[count]), (float)((9.0f*dataA[count])*vS)); //draw red or green dots
            
            {
                if (slewL > 194.0f) g.fillRect((float)count, (float)((400.0f-(sqrt(slewL-194.0f)*1.618f))*vS), 1.618f, (float)(sqrt(slewL-194.0f)*1.618f)*vS);
                else g.fillRect((float)count, (float)((400.0f-slewL)*vS), (1.0f/psDotSizeL)+0.5f, (float)(((1.0f/psDotSizeL)+(sqrt(slewL)*0.1618f))*vS));
            }
            {
                g.fillRect((float)count, ((400.0f+fmin((sqrt(meterZeroL)*16.2f)-20.0f,199.0f))*vS), fmax(meterZeroL*0.1f*dataA[count],1.618f), fmax(meterZeroL*0.075f*dataA[count],1.618f)*vS);
                //zero cross subs
            }
        } //end draw dots on meters L
        
        //begin draw dots on meters R
        if (peakR > 197.0f) {
            g.setColour(juce::Colour(255, 0, 0));
            g.fillRect((float)count, 1.0f, fmin(sqrt(peakR-196.0f),196.0f), (fmin((peakR-196.0f),196.0f)*vS));
        } //peak is clipping!
        else if (peakR > 1.0f) { //peak isn't clipping, but is not literally zero so there's something here to work with
            psDotSizeR = (8.3144112499811f * sqrt(dataB[count] * dataA[count])) / (fabs(((peakR*((6.6180339887f)/7.0f))-slewR) * (7.0f/meterZeroR) )+1.0f);
            psDotSizeR += sin(pow(fmin(dataD[count]*8.5f,6.18f) / (fabs(((peakR*((4.6180339887f)/5.0f))-slewR) * (7.0f/meterZeroR) )+1.0f),1.618f)*0.13f) * 1.467577515170776f;
            if (count == (unsigned long)dataPosition-1) {
                momentaryScoreR[count] = ((psDotSizeR*140.0f*(1.0f-hype))+(psDotSizeR*peakR*hype));
                maxScore += momentaryScoreR[count]*16.0f; //increase score
            }
            if (psDotSizeR > 1.0f) {
                g.setColour(juce::Colour::fromFloatRGBA(fmin((slewR-peakR)/256.0f,0.0f), fmin((peakR-slewR)/256.0f,0.0f), 1.0f, 1.0f));
                midPeaks += 1.0f;
            } else if (slewR > peakR) {
                g.setColour(juce::Colour::fromFloatRGBA(fmin((180.0f+(slewR-peakR))/256.0f,1.0f), 0.0f, 0.0f, 1.0f));
                brightPeaks += 1.0f;
            } else {
                g.setColour(juce::Colour::fromFloatRGBA(0.0f, ((255.0f-(peakR-slewR))/256.0f), 0.0f, 1.0f));
                darkPeaks += 1.0f;
            } //set COLOR
            
            if (psDotSizeR > 1.0f) g.fillRect((float)count, (float)((200.0f - peakR)*vS), (float)fmax(psDotSizeR,2.0), (float)(fmax(psDotSizeR,dataD[count]*5.0f)*vS)); //draw blue dots
            else if (slewR > peakR) g.fillRect((float)count, (float)((200.0f - peakR)*vS), 2.0f, (float)(3.0*vS));
            else g.fillRect((float)count, (float)((200.0f - peakR)*vS), (float)(9.0f*dataB[count]), (float)((9.0f*dataB[count])*vS)); //draw red or green dots
        
            {
                if (slewR > 194.0f) g.fillRect((float)count, (float)((400.0f-(sqrt(slewR-194.0f)*1.618f))*vS), 1.618f, (float)(sqrt(slewR-194.0f)*1.618f)*vS);
                else g.fillRect((float)count, (float)((400.0f-slewR)*vS), (1.0f/psDotSizeR)+0.5f, (float)(((1.0f/psDotSizeR)+(sqrt(slewR)*0.1618f))*vS));
            }
            {
                g.fillRect((float)count, ((400.0f+fmin((sqrt(meterZeroR)*16.2f)-20.0f,199.0f))*vS), fmax(meterZeroR*0.1f*dataB[count],1.618f), fmax(meterZeroR*0.075f*dataB[count],1.618f)*vS);
                //zero cross subs
            }
        } //end draw dots on meters R
         
        if (count == (unsigned long)dataPosition-1) {
            float falloff = 0.9438f; //tweak this in one place!
            loudScore[count] = sqrt(maxScore)*2.0f; //trim to work with meter
            maxScore *= falloff; //how fast loudness falls off
            unsigned long peakLTracker = (unsigned long)(peakL * (0.005f*(float)peakBins));
            unsigned long peakRTracker = (unsigned long)(peakR * (0.005f*(float)peakBins));//converts 0-200 to 0-bin number for bins for novelty chart
            if (peakLTracker > 0 && peakLTracker <= peakBins) peakTrack[peakLTracker] += loudScore[count];
            if (peakRTracker > 0 && peakRTracker <= peakBins) peakTrack[peakRTracker] += loudScore[count];
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
        //generating metrics for bright, sonorous and dark peaks to calculate with
        float blueScore = 0.0f; if (loudScore[count] > 1.0f) blueScore = fmin(pow(loudScore[count],2.0f)*0.00009f,99.0f); //HERE is where to trim the blue line for loudness
        float redScore = 0.0f; if (varietyScore[count] > 1.0) redScore = fmin(pow(varietyScore[count],2.0f)*0.00083f,99.0f); //HERE is where to trim the red line for variety
        outGreen = pow(fmax((fmax(dataA[count],dataB[count])*-80.0f)+(redScore+blueScore),0.0f),1.618f)*0.075f;
        float scaleGreen = 90.0f + outGreen + ((redScore-blueScore)*3.0f); scaleGreen = pow(scaleGreen,1.618f)*0.0273f;
        int greenColor = (int)scaleGreen; if (greenColor < 0) greenColor = 0; if (greenColor > 255) greenColor = 255;
                
        hue = juce::Colour((juce::uint8)(darkPeaksDisplay[count]/(totalPeaks*0.5f)), (juce::uint8)greenColor, (juce::uint8)(brightPeaksDisplay[count]/(totalPeaks*0.5f))).getHue();
        hue = hue-0.53333333f; if (hue < 0.0f) hue = fmax(1.0f+hue, 0.0f); else hue = 1.0f - hue; //involved in rating setting
                
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
    
    float scaleFont = sqrt(vS*61.8f)*1.618f;
    g.setFont(scaleFont*1.618f);
    g.setColour(juce::Colours::darkgrey);
    g.drawText("peaks", 7, (int)(4*vS), displayWidth-20, 32, juce::Justification::topLeft);
    g.drawText("slew rate relative to peak energy", 7, (int)(204*vS), displayWidth-20, 32, juce::Justification::centredTop);
    g.drawText("zero cross", 7, (int)(404*vS), displayWidth-20, 32, juce::Justification::topRight);
    g.drawText(rating, 7, (int)(404*vS), displayWidth-20, 32, juce::Justification::centredTop);
    g.setFont(scaleFont);
    if (scaleFont > 8.0f) {
        g.drawText("-6 dB", 7, (int)(60.0f*vS)-7, displayWidth-20, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("-12 dB", 7, (int)(101.02f*vS)-7, displayWidth-20, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("-18 dB", 7, (int)(130.02f*vS)-7, displayWidth-20, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("-24 dB", 7, (int)(150.2f*vS)-7, displayWidth-20, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("-30 dB", 7, (int)(164.9f*vS)-7, displayWidth-20, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("-36 dB", 7, (int)(175.2f*vS)-7, displayWidth-20, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("-42 dB", 7, (int)(182.5f*vS)-7, displayWidth-20, (int)scaleFont, juce::Justification::bottomLeft);
    }//dB markings
    if (scaleFont > 8.0f) {
        g.drawText("900 Hz", 7, (int)(460.0f*vS)-7, displayWidth-20, (int)scaleFont, juce::Justification::bottomRight);
        g.drawText("210 Hz", 7, (int)(501.02f*vS)-7, displayWidth-20, (int)scaleFont, juce::Justification::bottomRight);
        g.drawText("100 Hz", 7, (int)(530.02f*vS)-7, displayWidth-20, (int)scaleFont, juce::Justification::bottomRight);
        g.drawText("60 Hz", 7, (int)(550.2f*vS)-7, displayWidth-20, (int)scaleFont, juce::Justification::bottomRight);
        g.drawText("45 Hz", 7, (int)(564.9f*vS)-7, displayWidth-20, (int)scaleFont, juce::Justification::bottomRight);
        g.drawText("35 Hz", 7, (int)(575.2f*vS)-7, displayWidth-20, (int)scaleFont, juce::Justification::bottomRight);
        g.drawText("30 Hz", 7, (int)(582.5f*vS)-7, displayWidth-20, (int)scaleFont, juce::Justification::bottomRight);
    }//zero cross markings
    
    g.setColour(juce::Colours::grey);
    g.fillRect(dataPosition, 0, 1, (int)(599.0f*vS)); //the moving line
    g.setColour(juce::Colours::lightgrey);
    g.fillRect(0, (int)(201.0f*vS), getWidth(), 2); // border with slew meter
    g.fillRect(0, (int)(401.0f*vS), getWidth(), 2); // border with zero cross meter
    
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.382f));
    g.fillRect(0, 0, getWidth(), 2);
    g.fillRect(0, 0, 2, getHeight());
     
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.618f));
    g.fillRect(2, getHeight()-2, getWidth(), 2);
    g.fillRect(getWidth()-2, 2, 2, getHeight()-2);
}
