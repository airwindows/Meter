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
        if (sqrt(peakScore * 100.0f)*0.26f > peaksGrade) peaksGrade += 1;
        if (sqrt(slewScore * 100.0f)*0.26f > slewGrade) slewGrade += 1;
        if (sqrt(bassScore * 100.0f)*0.26f > bassGrade) bassGrade += 1;
        
        if (dataPosition == (int)count) {
            int allMatch = abs(peaksGrade-slewGrade);
            if (abs(peaksGrade-bassGrade) > abs(peaksGrade-slewGrade)) allMatch = abs(peaksGrade-bassGrade);
            allMatch /= 2;
            //note that for the color measurement we have NOT clamped the grades! The color saturation can go off even on a high score
            allMatch = 26-allMatch; //we have a fourth letter that tracks which songs have ALL the parts harmoniously in balance: like Strobe or Hotel California.
            if (allMatch < 1) allMatch = 1;
            if (allMatch > 26) allMatch = 26;
            switch (allMatch) {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14://below this score the meter can just show black
                    dataI[count] = 0.94f; break;
                case 15:
                    dataI[count] = 0.87f; break;
                case 16:
                    dataI[count] = 0.71f; break;
                case 17:
                    dataI[count] = 0.62f; break;
                case 18:
                    dataI[count] = 0.55f; break;
                case 19:
                    dataI[count] = 0.48f; break;
                case 20:
                    dataI[count] = 0.42f; break;
                case 21:
                    dataI[count] = 0.36f; break;
                case 22:
                    dataI[count] = 0.28f; break;
                case 23:
                    dataI[count] = 0.23f; break;
                case 24:
                    dataI[count] = 0.17f; break;
                case 25:
                    dataI[count] = 0.11f; break;
                case 26:
                    dataI[count] = 0.001f; break;
            } //this is our letter score, incorporating all the measurements
            dataJ[count] = 1.0f-fmax(abs(peaksGrade-slewGrade)*0.12f,0.0f);
            if (peaksGrade < 2) dataJ[count] = 0.0;
        }
        //saturation is an even better guide to balance than the + and will hint at how close you're getting
        g.setColour(juce::Colour::fromHSV(dataI[count], dataJ[count], dataJ[count], 1.0f));
        g.fillRect((float)count, 401.0f*vS, 1.0f, 29.0f*vS);
        //draw that bar that shows color of the text score at any given point
        //note: the bar will freak out if you're pushing levels to loudenation levels as you're forcing the
        //measurements inside the meter to saturate and overdrive. Keeping max color means keeping balance there too.
        
        if (count == (unsigned long)dataPosition-1) { //only update text score display more infrequently
            if (peaksGrade < 1) peaksGrade = 1;
            if (peaksGrade > 26) peaksGrade = 26;
            switch (peaksGrade) {
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
            switch (slewGrade) {
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
            switch (bassGrade) {
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
            
            int allMatch = abs(peaksGrade-slewGrade);
            if (abs(peaksGrade-bassGrade) > abs(peaksGrade-slewGrade)) allMatch = abs(peaksGrade-bassGrade);
            allMatch /= 2;
            if (peaksGrade == slewGrade) allMatch -= 1;
            if (peaksGrade == bassGrade) allMatch -= 1;
            if (bassGrade == slewGrade) allMatch -= 1; //if categories perfectly balance that's a grade boost right there
            allMatch = 26-allMatch; //we have a fourth letter that tracks which songs have ALL the parts harmoniously in balance: like Strobe or Hotel California.
            if (allMatch < 1) allMatch = 1;
            if (allMatch > 26) allMatch = 26;
            
            switch (allMatch) {
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
            } //this is our letter score, incorporating all the measurements
            
            if (dataPosition == (int)count) dataJ[count] = 1.0f-fmax(abs(peaksGrade-slewGrade)*0.12f,0.0f);
            //saturation is an even better guide to balance than the + and will hint at how close you're getting
            
            switch (peaksGrade - slewGrade) {
                case -9://So this is pretty cosmetic. The functionality hasn't changed, but this is a change to presentation.
                case -8://Also, if you're sorting records as I am, you might have a special use for the '+' Meter can show:
                case -7://the purpose being to try and sort all music into hits vs. non-hits on sound texture alone.
                case -6://And so, the '+' has broadened to be the crossover zone between sparkly and loud/gutsy,
                case -5://and the meter itself now tells you which side of that you're on, regarding that 'color intensity' goal.
                case -4://You still want 'balanced' but it's more direct about what to do, to get it.
                case -3://
                case -2://this part is able to genre light sparkly stuff to the disco side
                    totalPackage = juce::String("disco ")+totalPackage; break;
                case -1:
                    totalPackage = juce::String("disco +")+totalPackage; break;//either perfect balance as in 'hit' like Stairway to Heaven
                case 0:
                    totalPackage = juce::String("hit +")+totalPackage; break; //or just a bit to either the disco or rock sides of that line
                case 1:
                    totalPackage = juce::String("rock +")+totalPackage; break;//get you a '+' regardless of what fullness is doing: i.e. a hit.
                case 2://broadening that zone for a + should help illustrate what about it seems to be worth so much in popularity.
                case 3://it also means, if you know you're pop or know you're rock, you could intentionally aim for a reading
                case 4://that still is near balanced enough to functionally work as a hit (mass appeal, sonic balance)
                case 5://but which 'reads' as the genre you insist on being seen as. In other words, if this method works for you,
                case 6://you can buy into the idea that the balance is everything for mass appeal and hit factor,
                case 7://but finetune it to convey poppiness, or rockism, through sonic cues that don't trade off too much.
                case 8://
                case 9://this part is able to genre loud blaring stuff to the rock side
                    totalPackage = juce::String("rock ")+totalPackage; break;
                default:
                    totalPackage = juce::String("rated ")+totalPackage; break;
            } //this is our extra 'balance' flavor text, comes out of the algorithm
        }
    }
    
    float scaleFont = sqrt(vS*61.8f)*1.618f;
    g.setFont(scaleFont*1.618f);
    
    g.setColour(juce::Colours::white);
    g.drawText(sparkle, 6, (int)(390*vS)-11, displayWidth-20, 32, juce::Justification::topRight);
    g.drawText(sparkle, 8, (int)(390*vS)-9, displayWidth-20, 32, juce::Justification::topRight);
    if (peaksGrade+slewGrade+bassGrade > 3) {
        g.drawText(totalPackage+"-"+rating+sparkle+rumble, 6, (int)(190*vS)-11, displayWidth-20, 32, juce::Justification::centredTop);
        g.drawText(totalPackage+"-"+rating+sparkle+rumble, 8, (int)(190*vS)-9, displayWidth-20, 32, juce::Justification::centredTop);
    }//underdrawing in white for areas prone to get covered up with dots
    
    g.setColour(juce::Colours::darkgrey);
    g.drawText("intensity (peaks)", 7, (int)(4*vS), displayWidth-20, 32, juce::Justification::topLeft);
    if (peaksGrade+slewGrade+bassGrade > 3) g.drawText(totalPackage+"-"+rating+sparkle+rumble, 7, (int)(190*vS)-10, displayWidth-20, 32, juce::Justification::centredTop);
    g.drawText(rating, 7, (int)(190*vS)-10, displayWidth-20, 32, juce::Justification::topRight);
    g.drawText("detail (slew)", 7, (int)(204*vS), displayWidth-20, 32, juce::Justification::topLeft);
    g.drawText(sparkle, 7, (int)(390*vS)-10, displayWidth-20, 32, juce::Justification::topRight);
    g.drawText("fullness (zero cross)", 7, (int)(434*vS), displayWidth-20, 32, juce::Justification::topLeft);
    g.drawText(rumble, 7, (int)(434*vS), displayWidth-20, 32, juce::Justification::topRight);
    
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
        g.drawText("900 Hz", 7, (int)(460.0f*vS)-7, displayWidth-20, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("210 Hz", 7, (int)(501.02f*vS)-7, displayWidth-20, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("100 Hz", 7, (int)(530.02f*vS)-7, displayWidth-20, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("60 Hz", 7, (int)(550.2f*vS)-7, displayWidth-20, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("45 Hz", 7, (int)(564.9f*vS)-7, displayWidth-20, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("35 Hz", 7, (int)(575.2f*vS)-7, displayWidth-20, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("30 Hz", 7, (int)(582.5f*vS)-7, displayWidth-20, (int)scaleFont, juce::Justification::bottomLeft);
    }//zero cross markings
    
    g.setColour(juce::Colours::grey);
    g.fillRect(dataPosition, 0, 1, (int)(599.0f*vS)); //the moving line
    g.setColour(juce::Colours::lightgrey);
    g.fillRect(0, (int)(201.0f*vS), getWidth(), 2); // border with slew meter
    g.setColour(juce::Colours::black);
    g.fillRect(0, (int)(400.0f*vS), getWidth(), 2);
    g.fillRect(0, (int)(430.0f*vS), getWidth(), 3); // outline score color line
 
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.382f));
    g.fillRect(0, 0, getWidth(), 2);
    g.fillRect(0, 0, 2, getHeight());

    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.618f));
    g.fillRect(2, getHeight()-2, getWidth(), 2);
    g.fillRect(getWidth()-2, 2, 2, getHeight()-2);
}
