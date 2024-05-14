// AirwindowsUI by Chris Johnson
// Initial seed code for the meter created by Paul Walker on 8/23/21.

#ifndef AIRWINDOWSUI_H
#define AIRWINDOWSUI_H
#include "PluginProcessor.h"

class AirwindowsLookAndFeel : public juce::LookAndFeel_V4
{
public:
   // void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override;
    
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
        
        juce::String newName = juce::String();
        juce::String newColour = juce::String();
        juce::String newLED = juce::String();
        juce::String newImage = juce::String();
        juce::File customSettings = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory).getChildFile ("AirwindowsGlobals.txt");
        juce::String xmlFile = customSettings.loadFileAsString();
        std::unique_ptr<juce::XmlElement> body (juce::XmlDocument::parse (xmlFile));
        for (auto* e : body->getChildIterator()) {
            if (e->hasTagName ("PARAM")) { // find the "PARAM" sub-element
                juce::String attributeValueAsString = e->getStringAttribute("id");
                if (attributeValueAsString.equalsIgnoreCase("userFont")) {
                    newName = e->getStringAttribute("value");
                }
                if (attributeValueAsString.equalsIgnoreCase("userColour")) {
                    newColour = e->getStringAttribute("value");
                }
                if (attributeValueAsString.equalsIgnoreCase("LEDColour")) {
                    newLED = e->getStringAttribute("value");
                }
                if (attributeValueAsString.equalsIgnoreCase("userImage")) {
                    newImage = e->getStringAttribute("value");
                }
            }
        }
        body.release();
        
        juce::File customBackground = juce::File::getSpecialLocation(juce::File::SpecialLocationType::userDocumentsDirectory).getChildFile (newImage);
        if (customBackground.existsAsFile()) {
            backgroundImage = juce::ImageFileFormat::loadFrom(juce::File(customBackground));
            blurImage = backgroundImage.rescaled(2, 2);
         }

        if (newName == juce::String()) newName = "Jost";
        setDefaultSansSerifTypefaceName(newName);
        
        defaultColour = juce::Colours::findColourForName(newColour, juce::Colours::lightgrey);
        LEDColour = juce::Colours::findColourForName(newLED, juce::Colours::red);
     }
    juce::Colour defaultColour = juce::Colours::lightgrey;
    juce::Colour LEDColour = juce::Colours::red;
    juce::Image backgroundImage = juce::Image();
    juce::Image blurImage = juce::Image();
};


struct AirwindowsMeter : public juce::Component
{
    void paint(juce::Graphics &g) override;
    
    static constexpr int dataPoints = 2000;
    int displayWidth = 1200;
    int displayHeight = 675;
    u_long dataPosition = 0;
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
        if (dataPosition >= (u_long)displayWidth) dataPosition = 0;
    }
    void resetArrays(){
        for (u_long count = 0; count < dataPoints; ++count) //count through all the points in the array
        {
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
    float lastLOutline = 0.0;
    float lastROutline = 0.0;
    float lastLPeak = 0.0;
    float lastRPeak = 0.0;
    float lastLSlew = 0.0;
    float lastRSlew = 0.0;
};

extern AirwindowsLookAndFeel airwindowsLookAndFeel;
extern AirwindowsMeter meter;

#endif // AIRWINDOWSUI_H
