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
        setColour(juce::Slider::thumbColourId, juce::Colours::lightgrey);
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
        
        juce::String namedFont = JucePlugin_Name; namedFont.append("Font",1024);
        
        juce::String newColour = juce::String(); juce::String namedColour = JucePlugin_Name; namedColour.append("Colour",1024);
        juce::String newImage = juce::String(); juce::String namedImage = JucePlugin_Name; namedImage.append("Image",1024);
        juce::String newApplyTrackColourAmount = juce::String(); juce::String namedApplyTrackColourAmount = JucePlugin_Name; namedApplyTrackColourAmount.append("TrackColourAmount",1024);
        juce::String newLEDColour = juce::String(); juce::String namedLEDColour = JucePlugin_Name; namedLEDColour.append("LEDColour",1024);

        juce::File customSettings = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory).getChildFile("Airwindows").getChildFile("AirwindowsGlobals.txt");
        juce::String xmlFile = customSettings.loadFileAsString();
        std::unique_ptr<juce::XmlElement> body (juce::XmlDocument::parse (xmlFile));
        if ((xmlFile != juce::String()) && (xmlFile.length() > 0)) {
            for (auto* e : body->getChildIterator()) {
                if (e->hasTagName ("option")) { // find the "PARAM" sub-element
                    juce::String attributeValueAsString = e->getStringAttribute("id");
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
                }
            }
            body.release();
        }
        
        juce::File customBackground = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory).getChildFile("Airwindows").getChildFile (newImage);
        if (customBackground.existsAsFile()) {
            backgroundImage = juce::ImageFileFormat::loadFrom(juce::File(customBackground));
            blurImage = backgroundImage.rescaled(3, 3);
         }

        defaultColour = juce::Colours::findColourForName(newColour, juce::Colours::lightgrey);
        applyTrackColour = fmax(fmin(newApplyTrackColourAmount.getFloatValue(),1.0f),0.0f);
        LEDColour = juce::Colours::findColourForName(newLEDColour, juce::Colours::red);
     }
    juce::Colour defaultColour = juce::Colours::lightgrey;
    juce::Image backgroundImage = juce::Image();
    juce::Image blurImage = juce::Image();
    juce::String newFont = juce::String();
    bool usingNamedImage = false;
    float applyTrackColour = 0.5;
    juce::Colour LEDColour = juce::Colours::red;
};


struct AirwindowsMeter : public juce::Component
{
    void paint(juce::Graphics &g) override;
    
    static constexpr int dataPoints = 2000;
    int displayWidth = 1200;
    int displayHeight = 675;
    int dataPosition = 0;
    float maxScore = 0.0f;
    float lastScore = 0.0f;
    float lingerScore = 0.0f;
    float smoothMomentary = 0.0f;
    std::array<float, dataPoints> hitScore;
    std::array<float, dataPoints> momentaryScoreL;
    std::array<float, dataPoints> momentaryScoreR;

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
        for (int count = 0; count < dataPoints; ++count) //count through all the points in the array
        {
            hitScore[count] = 0.0f;
            momentaryScoreL[count] = 0.0f;
            momentaryScoreR[count] = 0.0f;
            dataA[count] = 0.0f;
            dataB[count] = 0.0f;
            dataC[count] = 0.0f;
            dataD[count] = 0.0f;
            dataE[count] = 0.0f;
            dataF[count] = 0.0f;
            dataG[count] = 0.0f;
            dataH[count] = 0.0f;
            dataPosition = 0;
            maxScore = 0.0f;
            lastScore = 0.0f;
            lingerScore = 0.0f;
            smoothMomentary = 0.0f;
            lastLOutline = 0.0f;
            lastROutline = 0.0f;
            lastLPeak = 0.0f;
            lastRPeak = 0.0f;
            lastLSlew = 0.0f;
            lastRSlew = 0.0f;
            highestScore = 0;
            textScore = juce::String();
            hype = 0.5f;
        }
    }
    float lastLOutline = 0.0f;
    float lastROutline = 0.0f;
    float lastLPeak = 0.0f;
    float lastRPeak = 0.0f;
    float lastLSlew = 0.0f;
    float lastRSlew = 0.0f;
    int highestScore = 0;
    juce::String textScore = juce::String();
    float hype = 0.618033988749894f; //this is now set in stone :)
};

extern AirwindowsLookAndFeel airwindowsLookAndFeel;
extern AirwindowsMeter meter;

#endif // AIRWINDOWSUI_H
