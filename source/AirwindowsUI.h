// AirwindowsUI by Chris Johnson
// Initial seed code for the meter created by Paul Walker on 8/23/21.

#ifndef AIRWINDOWSUI_H
#define AIRWINDOWSUI_H
#include "PluginProcessor.h"


class AirwindowsLookAndFeel : public juce::LookAndFeel_V4
{
public:

    AirwindowsLookAndFeel()
    {
        setColour(juce::Slider::backgroundColourId, juce::Colours::red);
        setColour(juce::Slider::thumbColourId, juce::Colours::grey);
        setColour(juce::Slider::trackColourId, juce::Colours::grey);
        setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::grey);
        setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black); //track after thumb
        setColour(juce::Slider::textBoxTextColourId, juce::Colours::grey);
        setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::grey);
        setColour(juce::Slider::textBoxHighlightColourId, juce::Colours::grey);
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::grey);
        setColour(juce::TextButton::buttonColourId, juce::Colours::lightgrey);
        setColour(juce::TextButton::buttonOnColourId, juce::Colours::lightgrey);
        setColour(juce::TextButton::textColourOffId, juce::Colours::lightgrey);
        setColour(juce::TextButton::textColourOnId, juce::Colours::lightgrey);
        setColour(juce::ResizableWindow::backgroundColourId, juce::Colours::lightgrey);
        
        juce::String newWidth = juce::String(); juce::String namedWidth = JucePlugin_Name; namedWidth.append("Width",1024);
        juce::String newHeight = juce::String(); juce::String namedHeight = JucePlugin_Name; namedHeight.append("Height",1024);
        juce::String namedFont = JucePlugin_Name; namedFont.append("Font",1024);
        juce::String newColour = juce::String(); juce::String namedColour = JucePlugin_Name; namedColour.append("Colour",1024);
        juce::String newImage = juce::String(); juce::String namedImage = JucePlugin_Name; namedImage.append("Image",1024);
        juce::String newApplyTrackColourAmount = juce::String(); juce::String namedApplyTrackColourAmount = JucePlugin_Name; namedApplyTrackColourAmount.append("TrackColourAmount",1024);
        juce::String newLEDColour = juce::String(); juce::String namedLEDColour = JucePlugin_Name; namedLEDColour.append("LEDColour",1024);
        juce::String newKnobMode = juce::String(); juce::String namedKnobMode = JucePlugin_Name; namedKnobMode.append("KnobMode",1024);
        juce::String newInterpolation = juce::String(); juce::String namedInterpolation = JucePlugin_Name; namedInterpolation.append("Interpolation",1024);
        juce::String newTilt = juce::String(); juce::String namedTilt = JucePlugin_Name; namedInterpolation.append("Tilt",1024);

        juce::File customSettings = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory).getChildFile("Airwindows").getChildFile("AirwindowsGlobals.txt");
        juce::String xmlFile = customSettings.loadFileAsString();
        std::unique_ptr<juce::XmlElement> body (juce::XmlDocument::parse (xmlFile));
        if ((xmlFile != juce::String()) && (xmlFile.length() > 0)) {
            for (auto* e : body->getChildIterator()) {
                if (e->hasTagName ("option")) { // find the "PARAM" sub-element
                    juce::String attributeValueAsString = e->getStringAttribute("id");
                    if (attributeValueAsString.equalsIgnoreCase("userWidth") && newWidth == juce::String()) newWidth = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase(namedWidth)) newWidth = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase("userHeight") && newHeight == juce::String()) newHeight = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase(namedHeight)) newHeight = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase("userFont") && newFont == juce::String()) newFont = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase(namedFont)) newFont = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase("userColour") && newColour == juce::String()) newColour = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase(namedColour)) newColour = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase("userImage") && newImage == juce::String()) newImage = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase(namedImage)) {newImage = e->getStringAttribute("value"); usingNamedImage = true;}
                    if (attributeValueAsString.equalsIgnoreCase("userTrackColourAmount") && newApplyTrackColourAmount == juce::String()) newApplyTrackColourAmount = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase(namedApplyTrackColourAmount)) newApplyTrackColourAmount = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase("userLEDColour") && newLEDColour == juce::String()) newLEDColour = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase(namedLEDColour)) newLEDColour = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase("userKnobMode") && newKnobMode == juce::String()) newKnobMode = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase(namedKnobMode)) newKnobMode = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase("userInterpolation") && newInterpolation == juce::String()) newInterpolation = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase(namedInterpolation)) newInterpolation = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase("userTilt") && newTilt == juce::String()) newTilt = e->getStringAttribute("value");
                    if (attributeValueAsString.equalsIgnoreCase(namedTilt)) newTilt = e->getStringAttribute("value");
                }
            }
            body.release();
        }
        
        juce::File customBackground = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory).getChildFile("Airwindows").getChildFile (newImage);
        if (customBackground.existsAsFile()) {
            backgroundImage = juce::ImageFileFormat::loadFrom(juce::File(customBackground));
            blurImage = backgroundImage.rescaled(3, 3);
         }
        userWidth = newWidth.getIntValue(); if (userWidth < 8 || userWidth > 16386) userWidth = 1000;
        userHeight = newHeight.getIntValue(); if (userHeight < 8 || userHeight > 16386) userHeight = 537;
        //if you've not specified anything or your settings are crazy enough we go with defaults
        defaultColour = juce::Colours::findColourForName(newColour, juce::Colours::lightgrey);
        applyTrackColour = fmax(fmin(newApplyTrackColourAmount.getFloatValue(),1.0f),0.0f);
        LEDColour = juce::Colours::findColourForName(newLEDColour, juce::Colours::red);
        applyTilt = fmax(fmin(newTilt.getFloatValue(),1.0f),0.0f) * 0.5f; //value is 0-1 but in use it's 0-0.5

        knobMode = 0; //defaults to rotary because it allows for really fine adjustments
        if (newKnobMode.equalsIgnoreCase("rotary")) knobMode = 0;
        if (newKnobMode.equalsIgnoreCase("airwindows")) knobMode = 0;
        if (newKnobMode.equalsIgnoreCase("realistic")) knobMode = 0;
        if (newKnobMode.equalsIgnoreCase("vertical")) knobMode = 1;
        if (newKnobMode.equalsIgnoreCase("up and down")) knobMode = 1;
        if (newKnobMode.equalsIgnoreCase("normal")) knobMode = 1; //I'm told vertical is customary ;)
        if (newKnobMode.equalsIgnoreCase("default")) knobMode = 1; // so here ya go ;)
        if (newKnobMode.equalsIgnoreCase("horizontal")) knobMode = 2;
        if (newKnobMode.equalsIgnoreCase("sideways")) knobMode = 2;
        
        alfInterpolation = 2; //defaults to bicubic 'cloud' interpolation
        if (newInterpolation.equalsIgnoreCase("none")) alfInterpolation = 0;
        if (newInterpolation.equalsIgnoreCase("off")) alfInterpolation = 0;
        if (newInterpolation.equalsIgnoreCase("nearestneighbor")) alfInterpolation = 0;
        if (newInterpolation.equalsIgnoreCase("nearest neighbor")) alfInterpolation = 0; //variations on mondrian-meter
        if (newInterpolation.equalsIgnoreCase("bilinear")) alfInterpolation = 1; //option for softer definition
     }
    int userWidth;
    int userHeight;
    juce::Colour defaultColour = juce::Colours::lightgrey;
    juce::Image backgroundImage = juce::Image();
    juce::Image blurImage = juce::Image();
    juce::String newFont = juce::String();
    bool usingNamedImage = false;
    float applyTrackColour = 0.5;
    juce::Colour LEDColour = juce::Colours::red;
    int knobMode;
    int alfInterpolation;
    float applyTilt = 0.0;
};

struct AirwindowsMeter : public juce::Component
{
    void paint(juce::Graphics &g) override;
    
    static constexpr int dataPoints = 2000;
    static constexpr int peakBins = 16;
    int displayWidth = 1280;
    int displayHeight = 720;
    int dataPosition = 0;
    float maxScore = 0.0f;
    float lastScore = 0.0f;
    float evennessNovelty = 0.0f;
    float evennessHype = 0.0f;
    float prevLoud = 0.0f;
    float prevPeak = 0.0f;
    float prevHit = 0.0f;
    float smoothMomentary = 0.0f;
    float lastLOutline = 0.0f;
    float lastROutline = 0.0f;
    float lastLPeak = 0.0f;
    float lastRPeak = 0.0f;
    float lastLSlew = 0.0f;
    float lastRSlew = 0.0f;
    float brightPeaks = 0.0f;
    float midPeaks = 0.0f;
    float darkPeaks = 1.0f;
    float hue = 0.0f;
    float outGreen = 0.0f;
    int highestGrade = 0;
    int highestColor = 0;
    float hitColor = 0.0f;
    juce::String hypeRating = juce::String();
    juce::String textScore = juce::String();
    juce::String rating = juce::String();
    float hype = 0.618033988749894f; //this is now set in stone :)
    
    std::array<float, dataPoints> loudScore;
    std::array<float, dataPoints> varietyScore;
    std::array<float, dataPoints> hypeScore;
    std::array<float, peakBins> peakTrack;
    std::array<float, dataPoints> momentaryScoreL;
    std::array<float, dataPoints> momentaryScoreR;
    std::array<float, dataPoints> brightPeaksDisplay;
    std::array<float, dataPoints> midPeaksDisplay;
    std::array<float, dataPoints> darkPeaksDisplay;

    std::array<float, dataPoints> dataA;
    std::array<float, dataPoints> dataB;
    std::array<float, dataPoints> dataC;
    std::array<float, dataPoints> dataD;
    std::array<float, dataPoints> dataE;
    std::array<float, dataPoints> dataF;
    std::array<float, dataPoints> dataG;
    std::array<float, dataPoints> dataH;
     
    void pushA(float X) {dataA[dataPosition] = X;}
    void pushB(float X) {dataB[dataPosition] = X;}
    void pushC(float X) {dataC[dataPosition] = X;}
    void pushD(float X) {dataD[dataPosition] = X;}
    void pushE(float X) {dataE[dataPosition] = X;}
    void pushF(float X) {dataF[dataPosition] = X;}
    void pushG(float X) {dataG[dataPosition] = X;}
    void pushH(float X) {dataH[dataPosition] = X;}
    void pushIncrement(float limit) {
        dataPosition++;
        if (dataPosition >= (int)displayWidth) dataPosition = 0;
    }

    void resetArrays(){
        dataPosition = 0;
        maxScore = 0.0f;
        lastScore = 0.0f;
        evennessNovelty = 0.0f;
        evennessHype = 0.0f;
        prevLoud = 0.0f;
        prevPeak = 0.0f;
        prevHit = 0.0f;
        smoothMomentary = 0.0f;
        lastLOutline = 0.0f;
        lastROutline = 0.0f;
        lastLPeak = 0.0f;
        lastRPeak = 0.0f;
        lastLSlew = 0.0f;
        lastRSlew = 0.0f;
        brightPeaks = 0.0f;
        midPeaks = 0.0f;
        darkPeaks = 1.0f;
        hue = 0.0f;
        outGreen = 0.0f;
        highestGrade = 0;
        highestColor = 0;
        hitColor = 0.0f;
        hypeRating = juce::String();
        textScore = juce::String();
        rating = juce::String();
        hype = 0.618033988749894f; //0.3819661
        for (int count = 0; count < peakBins; ++count) //count through all the points in the array
        {
            peakTrack[count] = 10.0;
        }
        for (int count = 0; count < dataPoints; ++count) //count through all the points in the array
        {
            loudScore[count] = 0.0f;
            varietyScore[count] = 0.0f;
            hypeScore[count] = 0.0f;
            momentaryScoreL[count] = 0.0f;
            momentaryScoreR[count] = 0.0f;
            brightPeaksDisplay[count] = 0.0f;
            midPeaksDisplay[count] = 0.0f;
            darkPeaksDisplay[count] = 1.0f;
            dataA[count] = 0.0f;
            dataB[count] = 0.0f;
            dataC[count] = 0.0f;
            dataD[count] = 0.0f;
            dataE[count] = 0.0f;
            dataF[count] = 0.0f;
            dataG[count] = 0.0f;
            dataH[count] = 0.0f;
        }
    }
};

extern AirwindowsLookAndFeel airwindowsLookAndFeel;
extern AirwindowsMeter meter;

#endif // AIRWINDOWSUI_H
