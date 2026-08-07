// AirwindowsUI by Chris Johnson
// Initial seed code for the meter created by Paul Walker on 8/23/21.
#include "AirwindowsUI.h"
#ifndef M_PI
#  define M_PI (3.14159265358979323846)
#endif
#ifndef M_PI_2
#  define M_PI_2 (1.57079632679489661923)
#endif

void AirwindowsMeter::mouseDown(const juce::MouseEvent &event)
{
    if (event.getNumberOfClicks() > 1) resetArrays();
}

void AirwindowsMeter::paint(juce::Graphics &g)
{
    float vS = displayHeight/600.0f; // short for vScale: everything * this
    if ((sqrt(vS*61.8f)*1.618f) > 10.0f) {
        g.fillAll(juce::Colours::white); //blank screen before doing anything, unless our draw covers the whole display anyway
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
    } else {
        juce::ColourGradient backdropGradient = juce::ColourGradient::vertical(juce::Colours::white, 0.0f, backdropColour, 600.0f*vS); //600 tall version
        backdropGradient.addColour(gradientMin, backdropColour.interpolatedWith(juce::Colours::white, gradientMin));
        //the mid point of the gradient goes toward the bottom and goes whiter when everything's in balance
        g.setFillType(backdropGradient);
        g.fillAll();
    } //blank screen before doing anything, unless our draw covers the whole display anyway
    
    for (unsigned long count = 0; count < fmin(displayWidth,5150); ++count) //count through all the points in the array
    {
        g.setColour(juce::Colours::black);
        float psDotSizeL = 0.0f;
        float psDotSizeR = 0.0f;
        float slewDotSizeL = 0.0f;
        float slewDotSizeR = 0.0f;
        float bassDotSizeL = 0.0f;
        float bassDotSizeR = 0.0f;
        float peakL = dataPL[count] * 200.0f;
        float peakR = dataPR[count] * 200.0f;
        float slewL = sqrt(dataSL[count])*300.0f;
        float slewR = sqrt(dataSR[count])*300.0f;
        float meterZeroL = (sqrt(dataZL[count])*6.0f)-6.0f;
        if (meterZeroL > 192.0f) meterZeroL = 192.0f;
        float bassL = fmin((sqrt(meterZeroL)*16.2f)-20.0f,199.0f);
        float meterZeroR = (sqrt(dataZR[count])*6.0f)-6.0f;
        if (meterZeroR > 192.0f) meterZeroR = 192.0f;
        float bassR = fmin((sqrt(meterZeroR)*16.2f)-20.0f,199.0f);
        //begin draw dots on meters L
        if (peakL > 197.0f) {
            g.setColour(juce::Colour(255, 0, 0));
            g.fillRect((float)count, sustainedClip*vS, 1.9f, (181.9f-sustainedClip)*vS);
            sustainedClip *= 0.99f; //peak is clipping!
            if (peakR > 197.0f) sustainedClip *= 0.975f; //if both are clipping, escalate
        } else sustainedClip = 180.0f;
        if (peakL > 1.0f) { //peak isn't clipping, but is not literally zero so there's something here to work with
            psDotSizeL = (dataPL[count]*64.0f) / (fabs((peakL*0.945433426957143f)-slewL)+6.18033988749894f);
            slewDotSizeL = (sin(0.1618f/psDotSizeL)*6.18f)+(sqrt(slewL)*0.1618f);
            bassDotSizeL = sqrt(meterZeroL*0.1f*dataPL[count]);
            if (psDotSizeL > 1.0f) g.setColour(juce::Colour::fromFloatRGBA(fmax((slewL-(peakL*0.945433426957143f)),0.0f)*0.029f,
                                                                           fmax(((peakL*0.945433426957143f)-slewL),0.0f)*0.012f, 1.0f, 1.0f));
            else if (slewL > peakL) g.setColour(juce::Colour::fromFloatRGBA(fmin((64.0f+slewL)/128.0f,1.0f), 0.0f, 0.0f, 1.0f));
            else                    g.setColour(juce::Colour::fromFloatRGBA(0.0f, fmax((160.0f-peakL)/160.0f,0.0f), 0.0f, 1.0f)); //set COLOR
            g.fillRect((float)count, (float)((200.0f - peakL)*vS), psDotSizeL+0.618f, (psDotSizeL+0.618f)*vS);
            if (slewL > 194.0f) g.fillRect((float)count, (float)((400.0f-(sqrt(slewL-194.0f)*1.618f))*vS), 1.618f, (float)(sqrt(slewL-194.0f)*1.618f)*vS);
            else g.fillRect((float)count, (float)((400.0f-slewL)*vS), slewDotSizeL+0.618f, slewDotSizeL*vS); //draw slew
            g.fillRect((float)count, ((400.0f+bassL)*vS), bassDotSizeL+fmax(pow(peakL/140.0f,4.0f),0.0f), (bassDotSizeL+fmax(pow(peakL/140.0f,4.0f),0.0f))*vS); //zero cross subs
        } //end draw dots on meters L
        //begin draw dots on meters R
        if (peakR > 197.0f) {
            g.setColour(juce::Colour(255, 0, 0));
            g.fillRect((float)count, sustainedClip*vS, 1.9f, (181.9f-sustainedClip)*vS);
            sustainedClip *= 0.99f; //peak is clipping!
            if (peakL > 197.0f) sustainedClip *= 0.975f; //if both are clipping, escalate
        } else sustainedClip = 180.0f;
        if (peakR > 1.0f) { //peak isn't clipping, but is not literally zero so there's something here to work with
            psDotSizeR = (dataPR[count]*64.0f) / (fabs((peakR*0.945433426957143f)-slewR)+6.18033988749894f);
            slewDotSizeR = (sin(0.1618f/psDotSizeR)*6.18f)+(sqrt(slewR)*0.1618f);
            bassDotSizeR = sqrt(meterZeroR*0.1f*dataPR[count]);
            if (count < dataPosition && count > dataPosition-2) {
                backR[count] = storeR;
                backG[count] = storeG;
                backB[count] = storeB; //RGB backdrop for text
            }
            if (psDotSizeR > 1.0f) g.setColour(juce::Colour::fromFloatRGBA(fmax((slewR-(peakR*0.945433426957143f)),0.0f)*0.029f,
                                                                           fmax(((peakR*0.945433426957143f)-slewR),0.0f)*0.012f, 1.0f, 1.0f));
            else if (slewR > peakR) g.setColour(juce::Colour::fromFloatRGBA(fmin((64.0f+slewR)/128.0f,1.0f), 0.0f, 0.0f, 1.0f));
            else                    g.setColour(juce::Colour::fromFloatRGBA(0.0f, fmax((160.0f-peakR)/160.0f,0.0f), 0.0f, 1.0f)); //set COLOR
            g.fillRect((float)count, (float)((200.0f - peakR)*vS), psDotSizeR+0.618f, (psDotSizeR+0.618f)*vS);
            if (slewR > 194.0f) g.fillRect((float)count, (float)((400.0f-(sqrt(slewR-194.0f)*1.618f))*vS), 1.618f, (float)(sqrt(slewR-194.0f)*1.618f)*vS);
            else g.fillRect((float)count, (float)((400.0f-slewR)*vS), slewDotSizeR+0.618f, slewDotSizeR*vS); //draw slew
            g.fillRect((float)count, ((400.0f+bassR)*vS), bassDotSizeR+fmax(pow(peakR/140.0f,4.0f),0.0f), (bassDotSizeR+fmax(pow(peakL/140.0f,4.0f),0.0f))*vS); //zero cross subs
        } //end draw dots on meters R
        
        g.setColour(juce::Colour::fromFloatRGBA(backR[count], backG[count], backB[count], 1.0f)); //set backdrop colour
        g.fillRect((float)(count)-0.25f, 182.5f*vS, 1.5f, 19.5f*vS);
        g.fillRect((float)(count)-0.25f, 401.0f*vS, 1.5f, 19.5f*vS); //draw tonecolor bars
        
        unsigned long bintracker;
        bintracker = (unsigned long)((peakL-((180.0f-sustainedClip)*1.618033988749894f)) * (0.005f*(float)totalBins));
        //converts 0-200 to 0-bin number for textscore bins, and the peak measurement will also try to incorporate varying clip lengths
        //on the grounds that FOR loudenated stuff, variety in the length of total clip registers for our purposes.
        bintracker = (unsigned long)(totalBins - (fabs((int)bintracker-(int)totalBins))); //mirror around max value so it reflects
        if (bintracker > 0 && bintracker <= totalBins) peakTrack[bintracker] += psDotSizeL;
        bintracker = (unsigned long)((peakR-((180.0f-sustainedClip)*1.618033988749894f)) * (0.005f*(float)totalBins));
        bintracker = (unsigned long)(totalBins - (fabs((int)bintracker-(int)totalBins)));
        if (bintracker > 0 && bintracker <= totalBins) peakTrack[bintracker] += psDotSizeR;
        //peak textscore bins
        bintracker = (unsigned long)(slewL * (0.005f*(float)totalBins));//converts 0-200 to 0-bin number for textscore bins
        bintracker = (unsigned long)(totalBins - (fabs((int)bintracker-(int)totalBins))); //mirror around max value so it reflects
        if (bintracker > 0 && bintracker <= totalBins) slewTrack[bintracker] += slewDotSizeL;
        bintracker = (unsigned long)(slewR * (0.005f*(float)totalBins));
        bintracker = (unsigned long)(totalBins - (fabs((int)bintracker-(int)totalBins)));
        if (bintracker > 0 && bintracker <= totalBins) slewTrack[bintracker] += slewDotSizeR;
        //slew textscore bins
        bintracker = (unsigned long)(bassL * (0.005f*(float)totalBins));//converts 0-200 to 0-bin number for textscore bins
        bintracker = (unsigned long)(totalBins - (fabs((int)bintracker-(int)totalBins))); //mirror around max value so it reflects
        if (bintracker > 0 && bintracker <= totalBins) bassTrack[bintracker] += bassDotSizeL;
        bintracker = (unsigned long)(bassR * (0.005f*(float)totalBins));
        bintracker = (unsigned long)(totalBins - (fabs((int)bintracker-(int)totalBins)));
        if (bintracker > 0 && bintracker <= totalBins) bassTrack[bintracker] += bassDotSizeR;
        //bass textscore bins
        
        //bins are for reinforcing score of a dispersed cloud of dot positions rather than maxing out a position
        float peakScore = 0.0;
        float slewScore = 0.0;
        float bassScore = 0.0;
        float sinScale = 0.0;
        for (unsigned long binscale = 0; binscale < totalBins; ++binscale) {
            peakTrack[binscale] = fmax(fmin(peakTrack[binscale]-0.1618033988749894f, 1.618033988749894f),0.0f); //decrement speed
            sinScale = fmax(sin(((float)binscale/(float)totalBins)*(float)M_PI_2),0.0f);
            //sin(1.57) weighting means big red dots in good places, ignore green slews at bottom
            peakScore += (float)sin(peakTrack[binscale]) * sinScale;
            
            slewTrack[binscale] = fmax(fmin(slewTrack[binscale]-0.1618033988749894f, 1.618033988749894f),0.0f); //decrement speed
            slewScore += (float)sin(slewTrack[binscale]) * sinScale;
            
            bassTrack[binscale] = fmax(fmin(bassTrack[binscale]-0.1618033988749894f, 1.618033988749894f),0.0f); //decrement speed
            sinScale = fmax(sin(((float)binscale/(float)totalBins)*(float)M_PI),0.0f);
            //sin(3.1415) does a sine weighting on zero cross meter, but resists overweighting midrange sections
            bassScore += (float)sin(bassTrack[binscale]) * sinScale;
        }
        if (count < dataPosition && count > dataPosition-2) {
            outputB += peakScore;
            outputR += slewScore;
            outputG += bassScore;
            outputVol = sqrt(fmax(fmax(dataPL[count],dataPR[count]),fmax(dataSL[count],dataSR[count])));
            //our volume measurement gets kicked up towards 0 by peak or slew
        }
        if (dataPosition == count) {
            float applyCurve = fmin(fmin(outputR,outputG),outputB) / fmax(fmax(outputR,outputG),outputB+0.0000001f);
            applyCurve = 1.0f-pow(1.0f-applyCurve,1.618033988749894f);
            cumulative += applyCurve * outputVol;
            duration += outputVol;
            //when loudness rises it will be paying proportionally more attention to the balance.
            //that means on varying tracks it'll care more about the main part than intros/outros.
        }
        if (count == (unsigned long)dataPosition-1) { //only update text score display more infrequently
            if (pow(peakScore, 1.618033988749894f) > peakGrade) peakGrade = pow(peakScore, 1.618033988749894f);
            if (pow(slewScore, 1.618033988749894f) > slewGrade) slewGrade = pow(slewScore, 1.618033988749894f);
            if (pow(bassScore, 1.618033988749894f) > bassGrade) bassGrade = pow(bassScore, 1.618033988749894f);
            if (peakGrade < 0) peakGrade = 0;
            if (peakGrade > 28) peakGrade = 28;
            switch ((int)peakGrade) {
                case 0:
                    power = juce::String("s"); break;
                case 1:
                    power = juce::String("r"); break;
                case 2:
                    power = juce::String("q"); break;
                case 3:
                    power = juce::String("p"); break;
                case 4:
                    power = juce::String("o"); break;
                case 5:
                    power = juce::String("n"); break;
                case 6:
                    power = juce::String("m"); break;
                case 7:
                    power = juce::String("l"); break;
                case 8:
                    power = juce::String("k"); break;
                case 9:
                    power = juce::String("j"); break;
                case 10:
                    power = juce::String("i"); break;
                case 11:
                    power = juce::String("h"); break;
                case 12:
                    power = juce::String("g"); break;
                case 13:
                    power = juce::String("f"); break;
                case 14:
                    power = juce::String("e"); break;
                case 15:
                    power = juce::String("d"); break;
                case 16:
                    power = juce::String("c"); break;
                case 17:
                    power = juce::String("b"); break;
                case 18:
                    power = juce::String("a"); break;
                case 19:
                    power = juce::String("A"); break;
                case 20:
                    power = juce::String("B"); break;
                case 21:
                    power = juce::String("C"); break;
                case 22:
                    power = juce::String("D"); break;
                case 23:
                    power = juce::String("E"); break;
                case 24:
                    power = juce::String("F"); break;
                case 25:
                    power = juce::String("G"); break;
                case 26:
                    power = juce::String("H"); break;
                case 27:
                    power = juce::String("I"); break;
                case 28:
                    power = juce::String("J"); break;
            } //this is our letter score, incorporating all the measurements
            
            if (slewGrade < 0) slewGrade = 0;
            if (slewGrade > 32) slewGrade = 32;
            switch ((int)slewGrade) {
                case 0:
                    detail = juce::String("w"); break;
                case 1:
                    detail = juce::String("v"); break;
                case 2:
                    detail = juce::String("u"); break;
                case 3:
                    detail = juce::String("t"); break;
                case 4:
                    detail = juce::String("s"); break;
                case 5:
                    detail = juce::String("r"); break;
                case 6:
                    detail = juce::String("q"); break;
                case 7:
                    detail = juce::String("p"); break;
                case 8:
                    detail = juce::String("o"); break;
                case 9:
                    detail = juce::String("n"); break;
                case 10:
                    detail = juce::String("m"); break;
                case 11:
                    detail = juce::String("l"); break;
                case 12:
                    detail = juce::String("k"); break;
                case 13:
                    detail = juce::String("j"); break;
                case 14:
                    detail = juce::String("i"); break;
                case 15:
                    detail = juce::String("h"); break;
                case 16:
                    detail = juce::String("g"); break;
                case 17:
                    detail = juce::String("f"); break;
                case 18:
                    detail = juce::String("e"); break;
                case 19:
                    detail = juce::String("d"); break;
                case 20:
                    detail = juce::String("c"); break;
                case 21:
                    detail = juce::String("b"); break;
                case 22:
                    detail = juce::String("a"); break;
                case 23:
                    detail = juce::String("A"); break;
                case 24:
                    detail = juce::String("B"); break;
                case 25:
                    detail = juce::String("C"); break;
                case 26:
                    detail = juce::String("D"); break;
                case 27:
                    detail = juce::String("E"); break;
                case 28:
                    detail = juce::String("F"); break;
                case 29:
                    detail = juce::String("G"); break;
                case 30:
                    detail = juce::String("H"); break;
                case 31:
                    detail = juce::String("I"); break;
                case 32:
                    detail = juce::String("J"); break;
            } //this is our two letter score, incorporating all the measurements
            
            if (bassGrade < 0) bassGrade = 0;
            if (bassGrade > 28) bassGrade = 28;
            switch ((int)bassGrade) {
                case 0:
                    authority = juce::String("s"); break;
                case 1:
                    authority = juce::String("r"); break;
                case 2:
                    authority = juce::String("q"); break;
                case 3:
                    authority = juce::String("p"); break;
                case 4:
                    authority = juce::String("o"); break;
                case 5:
                    authority = juce::String("n"); break;
                case 6:
                    authority = juce::String("m"); break;
                case 7:
                    authority = juce::String("l"); break;
                case 8:
                    authority = juce::String("k"); break;
                case 9:
                    authority = juce::String("j"); break;
                case 10:
                    authority = juce::String("i"); break;
                case 11:
                    authority = juce::String("h"); break;
                case 12:
                    authority = juce::String("g"); break;
                case 13:
                    authority = juce::String("f"); break;
                case 14:
                    authority = juce::String("e"); break;
                case 15:
                    authority = juce::String("d"); break;
                case 16:
                    authority = juce::String("c"); break;
                case 17:
                    authority = juce::String("b"); break;
                case 18:
                    authority = juce::String("a"); break;
                case 19:
                    authority = juce::String("A"); break;
                case 20:
                    authority = juce::String("B"); break;
                case 21:
                    authority = juce::String("C"); break;
                case 22:
                    authority = juce::String("D"); break;
                case 23:
                    authority = juce::String("E"); break;
                case 24:
                    authority = juce::String("F"); break;
                case 25:
                    authority = juce::String("G"); break;
                case 26:
                    authority = juce::String("H"); break;
                case 27:
                    authority = juce::String("I"); break;
                case 28:
                    authority = juce::String("J"); break;
            } //this is our letter score, incorporating all the measurements
            
            totalPackage = juce::String(" ");
            double allMatch = (1.0-pow(1.0-(cumulative/duration),1.618033988749894))*27.618033988749894;
            if (allMatch < 0.0) allMatch = 0.0;
            if (allMatch > 27.0) allMatch = 27.0;
            switch ((int)allMatch) {
                case 0:
                    totalPackage = juce::String(" "); break;
                case 1:
                    totalPackage = juce::String(" "); break;
                case 2:
                    totalPackage = juce::String("Z"); break;
                case 3:
                    totalPackage = juce::String("Y"); break;
                case 4:
                    totalPackage = juce::String("X"); break;
                case 5:
                    totalPackage = juce::String("W"); break;
                case 6:
                    totalPackage = juce::String("V"); break;
                case 7:
                    totalPackage = juce::String("U"); break;
                case 8:
                    totalPackage = juce::String("T"); break;
                case 9:
                    totalPackage = juce::String("S"); break;
                case 10:
                    totalPackage = juce::String("R"); break;
                case 11:
                    totalPackage = juce::String("Q"); break;
                case 12:
                    totalPackage = juce::String("P"); break;
                case 13:
                    totalPackage = juce::String("O"); break;
                case 14:
                    totalPackage = juce::String("N"); break;
                case 15:
                    totalPackage = juce::String("M"); break;
                case 16:
                    totalPackage = juce::String("L"); break;
                case 17:
                    totalPackage = juce::String("K"); break;
                case 18:
                    totalPackage = juce::String("J"); break;
                case 19:
                    totalPackage = juce::String("I"); break;
                case 20:
                    totalPackage = juce::String("H"); break;
                case 21:
                    totalPackage = juce::String("G"); break;
                case 22:
                    totalPackage = juce::String("F"); break;
                case 23:
                    totalPackage = juce::String("E"); break;
                case 24:
                    totalPackage = juce::String("D"); break;
                case 25:
                    totalPackage = juce::String("C"); break;
                case 26:
                    totalPackage = juce::String("B"); break;
                case 27:
                    totalPackage = juce::String("A"); break;
            } //this is our letter score, incorporating all the measurements
            directions = juce::String(" ");
            directionsOpacity = fmax(0.925f-backdropColour.getLightness(),0.0f)*1.618033988749894f;
            float directionsHue = backdropColour.getHue()*30.0f;
            switch ((int)directionsHue) {
                case 0:
                    directions = juce::String("less bright"); break;
                case 1:
                case 2:
                case 3:
                case 4:
                    directions = juce::String("too much air"); break;
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                    directions = juce::String("allow fullness"); break;
                case 10:
                case 11:
                case 12:
                case 13:
                    directions = juce::String("allow detail"); break;
                case 14:
                case 15:
                case 16:
                case 17:
                case 18:
                    directions = juce::String("allow activity"); break;
                case 19:
                case 20:
                case 21:
                case 22:
                    directions = juce::String("less boring"); break;
                case 23:
                case 24:
                case 25:
                    directions = juce::String("less harrowing"); break;
                case 26:
                case 27:
                case 28:
                    directions = juce::String("more calm"); break;
                case 29:
                case 30:
                    directions = juce::String("less bright"); break;
            } //this is tone color interpreted as instructions. Not an AI, it's just what you'd do!
            //the opacity will be a factor of balance, so if you don't want to see it, mix right
        }
    }
    
    float scaleFont = (sqrt(vS*64.0f)*1.618f);
    if (scaleFont > 10.0f) {
        g.setFont(scaleFont*1.618f);
        g.setColour(juce::Colours::black);
        g.drawText("power "+power, (int)scaleFont/2, (int)(3*vS), displayWidth/3, 32, juce::Justification::topLeft);
        g.drawText("peaks", (displayWidth/2)-(int)(scaleFont*0.618f), (int)(3*vS), displayWidth/2, 32, juce::Justification::topRight);
        //power is the intensity of varying peak energy, between maximum and minimum.
        //it can't be always maximum because that is just constant loudness and can't startle or vary.
        //Power is the derivative, the unexpected. it's the ability of sound to be producing peak 'aura' beyond what our ears think is the median loud.
        //we do not leave crest factor to be quiet with it, we leave crest factor so we can constantly have the ear tickled by stuff happening,
        //that alerts the brain to peaks far beyond what the loudness seems to be. If they aren't present, the sound is boring.
        //The key is DERIVATIVE: peaks matter to the extent that they extend beyond the RMS (root-mean-square, body/density of the sound),
        //and power is the extent to which peaks are both happening and varying from each other constantly. That's a second derivative.
        //This cloud expands as the capacity for loudness surprise rises: if you have continuously playing sound,
        //whether it's a drone or saturation or simply a reverberation in your mix, it'll restrict the ability of the peak cloud
        //to drop lower. Limiting the volume or bass of such sustaining elements works to widen the peak cloud's range,
        //bearing in mind that the ideal situation is constant peak activity at every dB level at once at all times.
        //This is of course impossible and a contradiction, which is what makes it interesting :)
        //We hear 'activity at very low dB during loud sounds' as openness of the sound, drama/excitement, and mix ease of listening.
        
        g.drawText("detail "+detail, (int)scaleFont/2, (int)(203*vS), displayWidth/2, 32, juce::Justification::topLeft);
        g.drawText("slews", (displayWidth/2)-(int)(scaleFont*0.618f), (int)(203*vS), displayWidth/2, 32, juce::Justification::topRight);
        //detail is the intensity of varying slew energy, between maximum and minimum.
        //it can't be always maximum because that is just hardness and glare and sounds bad to people.
        //it also can't be minimum or it comes off as dull and uneventful in the treble range, so there's a balance to be struck.
        //Recording mediums used to set hard limits to how much treble you could have in the very high frequencies, an artificial limit,
        //but it accidentally served to restrict detail to where it would balance with power (maximized peak energy) because it would
        //end up quieter, balanced with the more hearable RMS loudness, allowing the power to be experienced as a sound event.
        //Modern technology has no such limitations, so detail is commonly made to balance with RMS loudness in a context where
        //ONLY RMS remains and the peak energy is not there anymore. (also, detail can contribute to loudness)
        //That means you can get a 'correct' RMS/detail balance, but it only highlights the lack of power from missing peak energy.
        //When you balance detail with a loudness balance that retains power and uses it, it still sounds 'correct' but the aura
        //of the thing is very different, because the active peak energy is also sensed and it too can be in balance with other parts of the sound.
        //this is the same phenomenon as the balance of taste stimuli in Heinz ketchup. It's like a cheat code.
        
        g.drawText("authority "+authority, (int)scaleFont/2, (int)(423*vS), displayWidth/2, 32, juce::Justification::topLeft);
        g.drawText("zero crosses", (displayWidth/2)-(int)(scaleFont*0.618f), (int)(423*vS), displayWidth/2, 32, juce::Justification::topRight);
        //authority is the amount of varying zero cross energy, between maximum and minimum.
        //it can't be always maximum because that is just one frequency and reads as boring, plus it doesn't translate across bassbins as well.
        //The zero cross meter measures how long the audio can go before crossing the middle of the waveform again.
        //As such, loudness of bass makes it go higher and loudness of treble tends to disrupt this and take away low zero cross data.
        //Thing is, high and mid frequencies do also show up here, and it is just as subject to the need for an 'evenly spaced dot cloud'
        //as everything else, but we hear evenness of this cloud as a sense of scale, especially when it reaches low frequencies.
        //Simple resonant lows down here produce only one point of zero cross, which is why something as simple as an electronic kick
        //that drops in frequency 'translates' better and is more interesting to feel and hear. Sweeping across bassbin resonances works better.
        //But the way higher frequency content interferes with the zero cross is mix-dependent too: for instance, if you lack low mids that
        //should be there, the zero cross will have gaps of no data and then the bass all by itself all the way at the bottom.
        //Bringing in audio at other frequency ranges directly brings in zero cross ranges as they interfere with the zero crossings,
        //and this can be tracked as a dot cloud like the other meters, but maximizing this area (especially with the includion of DEEP bass)
        //is what produces a sense of imposing, majestic authority to the track.
        //This is pretty easily screwed up by having RMS too high, peaks too clipped, and detail boosted to match what RMS is doing.
        //The funny thing is that, since this can be made to balance with peak energy and not simply RMS, that means if you have a
        //high power mix with lots of energy beyond simple RMS, maximizing authority means hitting on all frequencies with perfect low mids.
        //That is a recipe for bass authority, not simply bass loudness, so authority is 'power through peaks, but for the lows'.
        //considering what a black art that sort of thing is, it's useful to view it in this light.
        
        g.setOpacity(0.618f);
        g.setColour(juce::Colours::white);
        g.drawText(totalPackage+power+detail+authority, 1-(int)(scaleFont*1.618f), (int)(194.0f*vS)-(int)(scaleFont-1.0f), displayWidth, 32, juce::Justification::centredTop);
        g.setOpacity(fmin(directionsOpacity,0.618f));
        g.drawText(directions, 1-(int)(scaleFont*1.618f), (int)(413.0f*vS)-(int)(scaleFont-1.0f), displayWidth, 32, juce::Justification::centredTop);
        //underdrawing in white for areas prone to get covered up with dots
        g.setOpacity(1.0f);
        g.setColour(juce::Colours::black);
        g.drawText(totalPackage+power+detail+authority, 0-(int)(scaleFont*1.618f), (int)(194.0f*vS)-(int)(scaleFont), displayWidth, 32, juce::Justification::centredTop);
        g.setOpacity(fmin(directionsOpacity,1.0f));
        g.drawText(directions, 0-(int)(scaleFont*1.618f), (int)(413.0f*vS)-(int)(scaleFont), displayWidth, 32, juce::Justification::centredTop);
        //the Hit Record Letter Grade is 'totalPackage' followed by Power, Detail and Scale in order. If you have different priorities,
        //it's fair to want those in a different order for your quality metric: bass music might go scale first, pop might want detail.
        //I'm using power first simply because it is the most spectacular contrast between modern production and lasting hit status,
        //and so it most highlights what is lacking in production done wrong, and music that won't last or be heard beyond the moment.
        //The importance of totalPackage is that it's a running tally of how well balanced each of these existing categories are with each other.
        //This is best understood with the ketchup analogy again: you can have corn syrup sweet all day long, but it gets cloying.
        //But if you balance it with a lot of salt, and some bitterness, and sourness, no one sensation predominates and the composite
        //ends up with a compelling, addictive profile and takes over entire supermarket shelves :)
        //The sound of music can do that as well, and as much as any given characteristic can have its own superfans
        //(rock for power, pop for detail, orchestration or bassmusic for scale) the ability to cross over and go huge
        //is conditional on making ALL the categories max out, but max out in a way that is perfectly balanced.
        //As such, you can get a high hit record score at totally different music intensities, because there's no ONE ultimate density
        //for musical event: it seems like there's a correlation with natural phenomena similar to how we can be impressed both by
        //a gentle rain's moderate alerting quality, and by the force of a giant scary thunderstorm. These all produce balanced
        //point-clouds of audio data, with dangerous wind increasing the bass that gives you scale, and rain producing power and detail.
        //So the final score letter ends up being a derivative of derivatives: where each of the categories is about how well a data point
        //sits alongside other data points and how evenly it's distributed to produce a sensation that's a part of how music is heard,
        //the final score letter is about how well all the parts come together into a whole, even though it's not actually
        //made up of the other letters: the other letters are maximum scores, an indicator of where you might direct attention
        //in fixing up a mix, but the final score is a running tally of the experience of the whole music as it unfolds.
        //the weight of any given moment is governed by how intense the moment is, but not simply by RMS loudness, the weight of a moment
        //over time is set by the highest peak or slew or RMS (handles scale) at that moment. So, if something dramatic happens,
        //Meter builds more of it into the score, but if there's a quiet intro or a fade, Meter pays less attention to that in favor of
        //caring about what happens when the music is active. Not no attention, just scaled to how intense the music is actively trying to be.
        
        g.setOpacity(1.0f);
        g.setFont(scaleFont);
        g.drawText("-6 dB", (int)scaleFont, (int)(60.0f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("-12 dB", (int)scaleFont, (int)(101.02f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("-18 dB", (int)scaleFont, (int)(130.02f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("-24 dB", (int)scaleFont, (int)(150.2f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("-30 dB", (int)scaleFont, (int)(164.9f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("-36 dB", (int)scaleFont, (int)(175.2f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("900 Hz", (int)scaleFont, (int)(460.0f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("210 Hz", (int)scaleFont, (int)(501.02f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("100 Hz", (int)scaleFont, (int)(530.02f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("60 Hz", (int)scaleFont, (int)(550.2f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("45 Hz", (int)scaleFont, (int)(564.9f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("35 Hz", (int)scaleFont, (int)(575.2f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.drawText("30 Hz", (int)scaleFont, (int)(582.5f*vS)-7, displayWidth/2, (int)scaleFont, juce::Justification::bottomLeft);
        g.setColour(juce::Colours::black);
        g.setOpacity(0.618f);
        g.fillRect(0, (int)(182.5*vS), getWidth(), 2);
        g.fillRect(0, (int)(400.0f*vS), getWidth(), 2);
        g.setOpacity(1.0f);
        g.fillRect(0, (int)(201.0f*vS), getWidth(), 2);
        g.fillRect(0, (int)(420.0f*vS), getWidth(), 2); // outline backdrop color line
    } else {
        g.setColour(juce::Colours::black);
        g.setOpacity(0.618f);
        g.fillRect(0, (int)(182.5*vS), getWidth(), 1);
        g.fillRect(0, (int)(400.0f*vS), getWidth(), 1);
        g.setOpacity(1.0f);
        g.fillRect(0, (int)(201.0f*vS), getWidth(), 1);
        g.fillRect(0, (int)(420.0f*vS), getWidth(), 1); // outline backdrop color line
               
        g.setFont(scaleFont*11.0f); //larger font for the tiny window, and the style seen in ConsoleX3
        g.setOpacity(0.21f);
        g.setColour(juce::Colours::white);
        g.drawText(totalPackage+power+detail+authority, 0, 1, displayWidth, displayHeight+1, juce::Justification::centred, false);
        g.drawText(totalPackage+power+detail+authority, 1, 0, displayWidth+1, displayHeight, juce::Justification::centred, false);
        g.setOpacity(0.51f);
        g.setColour(juce::Colours::white);
        g.drawText(totalPackage+power+detail+authority, 1, 1, displayWidth+1, displayHeight+1, juce::Justification::centred, false);
        g.setOpacity(1.0f);
        g.setColour(juce::Colours::black);
        g.drawText(totalPackage+power+detail+authority, 0, 0, displayWidth, displayHeight, juce::Justification::centred, false);
    }
    
    g.setColour(juce::Colours::grey);
    g.fillRect((int)dataPosition, 0, 1, (int)(599.0f*vS)); //the moving line
        
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.25f));
    g.fillRect(0, 0, getWidth(), 2);
    g.fillRect(0, 0, 2, getHeight());
    
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.25f));
    g.fillRect(2, getHeight()-2, getWidth(), 2);
    g.fillRect(getWidth()-2, 2, 2, getHeight()-2);
}
