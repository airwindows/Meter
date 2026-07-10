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
        float psDotSizeL = 0.0f;
        float psDotSizeR = 0.0f;
        float slewDotSizeL = 0.0f;
        float slewDotSizeR = 0.0f;
        float bassDotSizeL = 0.0f;
        float bassDotSizeR = 0.0f;
        float peakL = dataC[count] * 200.0f;
        float peakR = dataD[count] * 200.0f;
        float slewL = sqrt(dataE[count])*300.0f;
        float slewR = sqrt(dataF[count])*300.0f;
        float meterZeroL = (sqrt(dataG[count])*6.0f)-6.0f;
        if (meterZeroL > 192.0f) meterZeroL = 192.0f;
        float bassL = fmin((sqrt(meterZeroL)*16.2f)-20.0f,199.0f);
        float meterZeroR = (sqrt(dataH[count])*6.0f)-6.0f;
        if (meterZeroR > 192.0f) meterZeroR = 192.0f;
        float bassR = fmin((sqrt(meterZeroR)*16.2f)-20.0f,199.0f);
        //begin draw dots on meters L
        if (peakL > 196.0f) {
            g.setColour(juce::Colour(255, 0, 0));
            g.fillRect((float)count, 1.0f, fmin(sqrt(peakL-196.0f),196.0f), (fmin((peakL-196.0f),196.0f)*vS));
        } //peak is clipping!
        else if (peakL > 1.0f) { //peak isn't clipping, but is not literally zero so there's something here to work with
            psDotSizeL = (8.3144112499811f * sqrt(dataA[count] * dataB[count])) / (fabs(((peakL*((6.6180339887f)/7.0f))-slewL) * (7.0f/meterZeroL) )+1.0f);
            psDotSizeL += sin(pow(fmin(dataC[count]*8.5f,6.18f) / (fabs(((peakL*((4.6180339887f)/5.0f))-slewL) * (7.0f/meterZeroL) )+1.0f),1.618f)*0.13f) * 1.467577515170776f;
            slewDotSizeL = (sin(0.1618f/psDotSizeL)*6.18f)+(sqrt(slewL)*0.1618f);
            bassDotSizeL = meterZeroL*0.1f*dataA[count];
            
            if (count < dataPosition && count > dataPosition-2) {
                outputR += psDotSizeL * sqrt(fmax(psDotSizeL,1.0f)) * 0.2f;
                outputG += slewL * slewDotSizeL * sqrt(fmax(slewDotSizeL,1.0f)) * 0.0009f;
                outputB += bassDotSizeL * sqrt(fmax(bassDotSizeL,1.0f)) * 13.0f / (bassL+0.666f); //RGB backdrop, L version
            }
            
            if (psDotSizeL > 1.0f) g.setColour(juce::Colour::fromFloatRGBA(fmin((slewL-peakL)/256.0f,0.0f), fmin((peakL-slewL)/256.0f,0.0f), 1.0f, 1.0f));
            else if (slewL > peakL) g.setColour(juce::Colour::fromFloatRGBA(fmin((180.0f+(slewL-peakL))/256.0f,1.0f), 0.0f, 0.0f, 1.0f));
            else g.setColour(juce::Colour::fromFloatRGBA(0.0f, ((255.0f-(peakL-slewL))/256.0f), 0.0f, 1.0f)); //set COLOR
            if (psDotSizeL > 1.0f) g.fillRect((float)count, (float)((200.0f - peakL)*vS), (float)fmax(psDotSizeL,2.0), (float)(fmax(psDotSizeL,dataC[count]*5.0f)*vS)); //draw blue dots
            else if (slewL > peakL) g.fillRect((float)count, (float)((200.0f - peakL)*vS), 2.0f, (float)(3.0*vS));
            else g.fillRect((float)count, (float)((200.0f - peakL)*vS), (float)(9.0f*dataA[count]), (float)((9.0f*dataA[count])*vS)); //draw red or green dots
            if (slewL > 194.0f) g.fillRect((float)count, (float)((400.0f-(sqrt(slewL-194.0f)*1.618f))*vS), 1.618f, (float)(sqrt(slewL-194.0f)*1.618f)*vS);
            else g.fillRect((float)count, (float)((400.0f-slewL)*vS), slewDotSizeL, (float)(slewDotSizeL*vS)); //draw slew
            g.fillRect((float)count, ((400.0f+bassL)*vS), bassDotSizeL, bassDotSizeL*vS); //zero cross subs
        } //end draw dots on meters L
        //begin draw dots on meters R
        if (peakR > 197.0f) {
            g.setColour(juce::Colour(255, 0, 0));
            g.fillRect((float)count, 1.0f, fmin(sqrt(peakR-196.0f),196.0f), (fmin((peakR-196.0f),196.0f)*vS));
        } //peak is clipping!
        else if (peakR > 1.0f) { //peak isn't clipping, but is not literally zero so there's something here to work with
            psDotSizeR = (8.3144112499811f * sqrt(dataB[count] * dataA[count])) / (fabs(((peakR*((6.6180339887f)/7.0f))-slewR) * (7.0f/meterZeroR) )+1.0f);
            psDotSizeR += sin(pow(fmin(dataD[count]*8.5f,6.18f) / (fabs(((peakR*((4.6180339887f)/5.0f))-slewR) * (7.0f/meterZeroR) )+1.0f),1.618f)*0.13f) * 1.467577515170776f;
            slewDotSizeR = (sin(0.1618f/psDotSizeR)*6.18f)+(sqrt(slewR)*0.1618f);
            bassDotSizeR = meterZeroR*0.1f*dataB[count];
            
            if (count < dataPosition && count > dataPosition-2) {
                outputR += psDotSizeR * sqrt(fmax(psDotSizeR,1.0f)) * 0.2f;
                outputG += slewR * slewDotSizeR * sqrt(fmax(slewDotSizeR,1.0f)) * 0.0009f;
                outputB += bassDotSizeR * sqrt(fmax(bassDotSizeR,1.0f)) * 13.0f / (bassR+0.666f);
                backR[count] = storeR;
                backG[count] = storeG;
                backB[count] = storeB; //RGB backdrop for text
            }
            
            if (psDotSizeR > 1.0f) g.setColour(juce::Colour::fromFloatRGBA(fmin((slewR-peakR)/256.0f,0.0f), fmin((peakR-slewR)/256.0f,0.0f), 1.0f, 1.0f));
            else if (slewR > peakR) g.setColour(juce::Colour::fromFloatRGBA(fmin((180.0f+(slewR-peakR))/256.0f,1.0f), 0.0f, 0.0f, 1.0f));
            else g.setColour(juce::Colour::fromFloatRGBA(0.0f, ((255.0f-(peakR-slewR))/256.0f), 0.0f, 1.0f)); //set COLOR
            if (psDotSizeR > 1.0f) g.fillRect((float)count, (float)((200.0f - peakR)*vS), (float)fmax(psDotSizeR,2.0), (float)(fmax(psDotSizeR,dataD[count]*5.0f)*vS)); //draw blue dots
            else if (slewR > peakR) g.fillRect((float)count, (float)((200.0f - peakR)*vS), 2.0f, (float)(3.0*vS));
            else g.fillRect((float)count, (float)((200.0f - peakR)*vS), (float)(9.0f*dataB[count]), (float)((9.0f*dataB[count])*vS)); //draw red or green dots
            if (slewR > 194.0f) g.fillRect((float)count, (float)((400.0f-(sqrt(slewR-194.0f)*1.618f))*vS), 1.618f, (float)(sqrt(slewR-194.0f)*1.618f)*vS);
            else g.fillRect((float)count, (float)((400.0f-slewR)*vS), slewDotSizeR, (float)(slewDotSizeR*vS)); //draw slew
            g.fillRect((float)count, ((400.0f+bassR)*vS), bassDotSizeR, bassDotSizeR*vS); //zero cross subs
        } //end draw dots on meters R
        g.setColour(juce::Colour::fromFloatRGBA(backR[count], backG[count], backB[count], 1.0f)); //set backdrop colour
        g.fillRect((float)(count)-0.25f, 182.5f*vS, 1.5f, 19.5f*vS);
        
        unsigned long bintracker = (unsigned long)(peakL * (0.005f*(float)totalBins));//converts 0-200 to 0-bin number for textscore bins
        if (bintracker > 0 && bintracker <= totalBins) peakTrack[bintracker] += psDotSizeL * sqrt(fmax(psDotSizeL,1.0f)) * 0.2f;
        bintracker = (unsigned long)(peakR * (0.005f*(float)totalBins));
        if (bintracker > 0 && bintracker <= totalBins) peakTrack[bintracker] += psDotSizeR * sqrt(fmax(psDotSizeR,1.0f)) * 0.2f; //peak textscore bins
        
        bintracker = (unsigned long)(slewL * (0.005f*(float)totalBins));
        if (bintracker > 0 && bintracker <= totalBins) slewTrack[bintracker] += slewL * slewDotSizeL * sqrt(fmax(slewDotSizeL,1.0f)) * 0.0009f;
        bintracker = (unsigned long)(slewR * (0.005f*(float)totalBins));
        if (bintracker > 0 && bintracker <= totalBins) slewTrack[bintracker] += slewR * slewDotSizeR * sqrt(fmax(slewDotSizeR,1.0f)) * 0.0009f; //slew textscore bins
        
        bintracker = (unsigned long)(bassL * (0.005f*(float)totalBins));
        if (bintracker > 0 && bintracker <= totalBins) bassTrack[bintracker] += bassDotSizeL * sqrt(fmax(bassDotSizeL,1.0f)) * 13.0f / (bassL+0.666f);
        bintracker = (unsigned long)(bassR * (0.005f*(float)totalBins));
        if (bintracker > 0 && bintracker <= totalBins) bassTrack[bintracker] += bassDotSizeR * sqrt(fmax(bassDotSizeR,1.0f)) * 13.0f / (bassR+0.666f); //bass textscore bins
        //bins are for reinforcing score of a dispersed cloud of dot positions rather than maxing out a position
        
        float maxPeakBin = 0.0f;
        float maxSlewBin = 0.0f;
        float maxBassBin = 0.0f;
        for (unsigned long binscale = 0; binscale < totalBins; ++binscale) {
            maxPeakBin = fmax(peakTrack[binscale], maxPeakBin);
            maxSlewBin = fmax(slewTrack[binscale], maxSlewBin);
            maxBassBin = fmax(bassTrack[binscale], maxBassBin);
        } //now each holds the highest value of the lot
        for (unsigned long binscale = 0; binscale < totalBins; ++binscale) {
            peakTrack[binscale] = fmax(peakTrack[binscale] - (maxPeakBin*0.012f), 0.0f); //adding decimal place and the '2' is the only change
            slewTrack[binscale] = fmax(slewTrack[binscale] - (maxSlewBin*0.01f), 0.0f);  //between 0.2.3 and 0.2.4 in terms of the algorithm:
            bassTrack[binscale] = fmax(bassTrack[binscale] - (maxBassBin*0.01f), 0.0f);  //finetunes the '+' balance meter's performance
        } //bins fall off at fixed speed, not converging on 0
        
        float peakScore = 0.0;
        float slewScore = 0.0;
        float bassScore = 0.0;
        for (unsigned long binscale = 0; binscale < totalBins; ++binscale) {
            peakScore += peakTrack[binscale];
            slewScore += slewTrack[binscale];
            bassScore += bassTrack[binscale];
        }
        if (sqrt(peakScore * 100.0f)*0.26f > peaksGrade) peaksGrade = sqrt(peakScore * 100.0f)*0.26f;
        if (sqrt(slewScore * 100.0f)*0.26f > slewGrade) slewGrade = sqrt(slewScore * 100.0f)*0.26f;
        if (sqrt(bassScore * 100.0f)*0.26f > bassGrade) bassGrade = sqrt(bassScore * 100.0f)*0.26f;
        
        if (dataPosition == count) {
            double scoreHue = 5.0-((sqrt(cumulative)/sqrt(duration))*5.25);
            if (scoreHue > 0.69) scoreHue = 0.69;
            if (scoreHue < 0.0) scoreHue += 1.0;
            dispHue[count] = (float)scoreHue;
            dispSat[count] = 1.0f-fmax(abs(peaksGrade-slewGrade)*0.12f,0.0f);
            if (peaksGrade < 0.01) dispSat[count] = 0.0;
            float cumulativeLoudness = sqrt(dataA[count]+dataB[count]+0.01f); //whole rating scaled by RMS of each channel
            //in this way, it'll keep the same rating for anything that was wholly consistent in loudness,
            //but when RMS rises it will be paying proportionally more attention to the balance.
            //that means on varying tracks it'll care more about the main part than intros/outros.
            cumulative += dispSat[count]*cumulativeLoudness; //each time, add what the saturation of the hit intensity bar was
            duration += cumulativeLoudness; //and the total duration we'll be dividing by. End result higher is better.
            //again, all this is now scaled by how loud it is: it's weighing the loud parts more heavily,
            //but continues to need there to be a balance regardless.
        }
        //saturation is an even better guide to balance than the + and will hint at how close you're getting
        g.setColour(juce::Colour::fromHSV(dispHue[count], dispSat[count], sqrt(dispSat[count]), 1.0f));
        if (dispSat[count] == 0.0) g.setColour(juce::Colours::white);
        g.fillRect((float)(count)-0.25f, 401.0f*vS, 1.5f, 19.0f*vS);
        //draw that bar that shows color of the text score at any given point
        //note: the bar will freak out if you're pushing levels to loudenation levels as you're forcing the
        //measurements inside the meter to saturate and overdrive. Keeping max color means keeping balance there too.
        
        if (count == (unsigned long)dataPosition-1) { //only update text score display more infrequently
            if (peaksGrade < 1) peaksGrade = 1;
            if (peaksGrade > 26) peaksGrade = 26;
            switch ((int)peaksGrade) {
                case 1:
                    rating = juce::String("Z"); break;
                case 2:
                    rating = juce::String("Y"); break;
                case 3:
                    rating = juce::String("X"); break;
                case 4:
                    rating = juce::String("W"); break;
                case 5:
                    rating = juce::String("V"); break;
                case 6:
                    rating = juce::String("U"); break;
                case 7:
                    rating = juce::String("T"); break;
                case 8:
                    rating = juce::String("S"); break;
                case 9:
                    rating = juce::String("R"); break;
                case 10:
                    rating = juce::String("Q"); break;
                case 11:
                    rating = juce::String("P"); break;
                case 12:
                    rating = juce::String("O"); break;
                case 13:
                    rating = juce::String("N"); break;
                case 14:
                    rating = juce::String("M"); break;
                case 15:
                    rating = juce::String("L"); break;
                case 16:
                    rating = juce::String("K"); break;
                case 17:
                    rating = juce::String("J"); break;
                case 18:
                    rating = juce::String("I"); break;
                case 19:
                    rating = juce::String("H"); break;
                case 20:
                    rating = juce::String("G"); break;
                case 21:
                    rating = juce::String("F"); break;
                case 22:
                    rating = juce::String("E"); break;
                case 23:
                    rating = juce::String("D"); break;
                case 24:
                    rating = juce::String("C"); break;
                case 25:
                    rating = juce::String("B"); break;
                case 26:
                    rating = juce::String("A"); break;
            } //this is our letter score, incorporating all the measurements
            
            if (slewGrade < 1) slewGrade = 1;
            if (slewGrade > 26) slewGrade = 26;
            switch ((int)slewGrade) {
                case 1:
                    sparkle = juce::String("Z"); break;
                case 2:
                    sparkle = juce::String("Y"); break;
                case 3:
                    sparkle = juce::String("X"); break;
                case 4:
                    sparkle = juce::String("W"); break;
                case 5:
                    sparkle = juce::String("V"); break;
                case 6:
                    sparkle = juce::String("U"); break;
                case 7:
                    sparkle = juce::String("T"); break;
                case 8:
                    sparkle = juce::String("S"); break;
                case 9:
                    sparkle = juce::String("R"); break;
                case 10:
                    sparkle = juce::String("Q"); break;
                case 11:
                    sparkle = juce::String("P"); break;
                case 12:
                    sparkle = juce::String("O"); break;
                case 13:
                    sparkle = juce::String("N"); break;
                case 14:
                    sparkle = juce::String("M"); break;
                case 15:
                    sparkle = juce::String("L"); break;
                case 16:
                    sparkle = juce::String("K"); break;
                case 17:
                    sparkle = juce::String("J"); break;
                case 18:
                    sparkle = juce::String("I"); break;
                case 19:
                    sparkle = juce::String("H"); break;
                case 20:
                    sparkle = juce::String("G"); break;
                case 21:
                    sparkle = juce::String("F"); break;
                case 22:
                    sparkle = juce::String("E"); break;
                case 23:
                    sparkle = juce::String("D"); break;
                case 24:
                    sparkle = juce::String("C"); break;
                case 25:
                    sparkle = juce::String("B"); break;
                case 26:
                    sparkle = juce::String("A"); break;
            } //this is our two letter score, incorporating all the measurements
            
            if (bassGrade < 1) bassGrade = 1;
            if (bassGrade > 26) bassGrade = 26;
            switch ((int)bassGrade) {
                case 1:
                    rumble = juce::String("Z"); break;
                case 2:
                    rumble = juce::String("Y"); break;
                case 3:
                    rumble = juce::String("X"); break;
                case 4:
                    rumble = juce::String("W"); break;
                case 5:
                    rumble = juce::String("V"); break;
                case 6:
                    rumble = juce::String("U"); break;
                case 7:
                    rumble = juce::String("T"); break;
                case 8:
                    rumble = juce::String("S"); break;
                case 9:
                    rumble = juce::String("R"); break;
                case 10:
                    rumble = juce::String("Q"); break;
                case 11:
                    rumble = juce::String("P"); break;
                case 12:
                    rumble = juce::String("O"); break;
                case 13:
                    rumble = juce::String("N"); break;
                case 14:
                    rumble = juce::String("M"); break;
                case 15:
                    rumble = juce::String("L"); break;
                case 16:
                    rumble = juce::String("K"); break;
                case 17:
                    rumble = juce::String("J"); break;
                case 18:
                    rumble = juce::String("I"); break;
                case 19:
                    rumble = juce::String("H"); break;
                case 20:
                    rumble = juce::String("G"); break;
                case 21:
                    rumble = juce::String("F"); break;
                case 22:
                    rumble = juce::String("E"); break;
                case 23:
                    rumble = juce::String("D"); break;
                case 24:
                    rumble = juce::String("C"); break;
                case 25:
                    rumble = juce::String("B"); break;
                case 26:
                    rumble = juce::String("A"); break;
            } //this is our letter score, incorporating all the measurements
            
            totalPackage = juce::String("Z");
            double allMatch = ((sqrt(cumulative)/sqrt(duration))*28.28);
            if (allMatch < 1.001) allMatch = 1.001;
            if (allMatch > 28) allMatch = 28;
            switch ((int)allMatch) {
                case 1:
                    totalPackage = juce::String("Z"); break;
                case 2:
                    totalPackage = juce::String("Y"); break;
                case 3:
                    totalPackage = juce::String("X"); break;
                case 4:
                    totalPackage = juce::String("W"); break;
                case 5:
                    totalPackage = juce::String("V"); break;
                case 6:
                    totalPackage = juce::String("U"); break;
                case 7:
                    totalPackage = juce::String("T"); break;
                case 8:
                    totalPackage = juce::String("S"); break;
                case 9:
                    totalPackage = juce::String("R"); break;
                case 10:
                    totalPackage = juce::String("Q"); break;
                case 11:
                    totalPackage = juce::String("P"); break;
                case 12:
                    totalPackage = juce::String("O"); break;
                case 13:
                    totalPackage = juce::String("N"); break;
                case 14:
                    totalPackage = juce::String("M"); break;
                case 15:
                    totalPackage = juce::String("L"); break;
                case 16:
                    totalPackage = juce::String("K"); break;
                case 17:
                    totalPackage = juce::String("J"); break;
                case 18:
                    totalPackage = juce::String("I"); break;
                case 19:
                    totalPackage = juce::String("H"); break;
                case 20:
                    totalPackage = juce::String("G"); break;
                case 21:
                    totalPackage = juce::String("F"); break;
                case 22:
                    totalPackage = juce::String("E"); break;
                case 23:
                    totalPackage = juce::String("D"); break;
                case 24:
                    totalPackage = juce::String("C"); break;
                case 25:
                    totalPackage = juce::String("B"); break;
                case 26:
                    totalPackage = juce::String("A"); break;
                case 27:
                    totalPackage = juce::String("+A"); break;
                case 28:
                    totalPackage = juce::String("++A"); break;
            } //this is our letter score, incorporating all the measurements
            if (dataPosition == count) dispSat[count] = 1.0f-fmax(abs(peaksGrade-slewGrade)*0.12f,0.0f);
        }
    }
    
    float scaleFont = (sqrt(vS*61.8f)*1.618f);
    if (scaleFont > 12.0f) {
        g.setFont(scaleFont*1.618f);
        g.setColour(juce::Colours::white);
        g.drawText("tone color", 8, (int)(194.0f*vS)-(int)(scaleFont-1.0f), displayWidth/3, 32, juce::Justification::topLeft);
        g.drawText("seek white balance", ((displayWidth*2)/3)-11, (int)(194.0f*vS)-(int)(scaleFont-1.0f), displayWidth/3, 32, juce::Justification::topRight);
        g.drawText("hit intensity", 8, (int)(412.0f*vS)-(int)(scaleFont-1.0f), displayWidth/2, 32, juce::Justification::topLeft);
        g.drawText("boost color", (displayWidth/2)-11, (int)(412.0f*vS)-(int)(scaleFont-1.0f), displayWidth/2, 32, juce::Justification::topRight);
        g.drawText(totalPackage+"-"+rating+sparkle+rumble, 8, (int)(194.0f*vS)-(int)(scaleFont-1.0f), displayWidth-20, 32, juce::Justification::centredTop);
        //underdrawing in white for areas prone to get covered up with dots
        g.setColour(juce::Colours::black);
        g.drawText("tone color", 7, (int)(194.0f*vS)-(int)(scaleFont), displayWidth/3, 32, juce::Justification::topLeft);
        g.drawText("seek white balance", ((displayWidth*2)/3)-12, (int)(194.0f*vS)-(int)(scaleFont), displayWidth/3, 32, juce::Justification::topRight);
        g.drawText("hit intensity", 7, (int)(412.0f*vS)-(int)(scaleFont), displayWidth/2, 32, juce::Justification::topLeft);
        g.drawText("boost color", (displayWidth/2)-12, (int)(412.0f*vS)-(int)(scaleFont), displayWidth/2, 32, juce::Justification::topRight);
        g.drawText(totalPackage+"-"+rating+sparkle+rumble, 7, (int)(194.0f*vS)-(int)(scaleFont), displayWidth-20, 32, juce::Justification::centredTop);
        g.setFont(scaleFont*1.1f);
        g.drawText("loudness: "+rating, 7, (int)(3*vS), displayWidth/3, 32, juce::Justification::topLeft);
        g.drawText("peaks", (displayWidth/2)-12, (int)(3*vS), displayWidth/2, 32, juce::Justification::topRight);
        g.drawText("detail: "+sparkle, 7, (int)(203*vS), displayWidth/2, 32, juce::Justification::topLeft);
        g.drawText("slews", (displayWidth/2)-12, (int)(203*vS), displayWidth/2, 32, juce::Justification::topRight);
        g.drawText("fullness: "+rumble, 7, (int)(423*vS), displayWidth/2, 32, juce::Justification::topLeft);
        g.drawText("zero cross", (displayWidth/2)-12, (int)(423*vS), displayWidth/2, 32, juce::Justification::topRight);
        g.setFont(scaleFont);
        g.drawText("-6 dB", 7, (int)(60.0f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("-12 dB", 7, (int)(101.02f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("-18 dB", 7, (int)(130.02f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("-24 dB", 7, (int)(150.2f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("-30 dB", 7, (int)(164.9f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("-36 dB", 7, (int)(175.2f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("900 Hz", 7, (int)(460.0f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("210 Hz", 7, (int)(501.02f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("100 Hz", 7, (int)(530.02f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("60 Hz", 7, (int)(550.2f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("45 Hz", 7, (int)(564.9f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("35 Hz", 7, (int)(575.2f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("30 Hz", 7, (int)(582.5f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
    } else {
        g.setFont(scaleFont*6.18f);
        g.setColour(juce::Colours::white); //underdrawing in white for areas prone to get covered up with dots
        g.drawText(totalPackage+"-"+rating+sparkle+rumble, 0, 0, displayWidth, displayHeight, juce::Justification::centred);
        g.setColour(juce::Colours::black);
        g.drawText(totalPackage+"-"+rating+sparkle+rumble, 0, 0, displayWidth, displayHeight, juce::Justification::centred);
    }
    
    g.setColour(juce::Colours::grey);
    g.fillRect((int)dataPosition, 0, 1, (int)(599.0f*vS)); //the moving line
    g.fillRect(0, (int)(182.5*vS), getWidth(), 2);
    g.setColour(juce::Colours::darkgrey);
    g.fillRect(0, (int)(201.0f*vS), getWidth(), 2); // outline backdrop color line
    g.fillRect(0, (int)(400.0f*vS), getWidth(), 2);
    g.setColour(juce::Colours::black);
    g.fillRect(0, (int)(420.0f*vS), getWidth(), 2); // outline score color line
    
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.382f));
    g.fillRect(0, 0, getWidth(), 2);
    g.fillRect(0, 0, 2, getHeight());
    
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.618f));
    g.fillRect(2, getHeight()-2, getWidth(), 2);
    g.fillRect(getWidth()-2, 2, 2, getHeight()-2);
}
