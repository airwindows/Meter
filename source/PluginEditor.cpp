#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    setResizable(true, true);
    setLookAndFeel(&airwindowsLookAndFeel);
    if (hostTrackColour != juce::Colour()) {
        airwindowsLookAndFeel.setColour(juce::ResizableWindow::backgroundColourId, hostTrackColour);
        airwindowsLookAndFeel.setColour(juce::Slider::thumbColourId, hostTrackColour);
    } else {
        airwindowsLookAndFeel.setColour(juce::ResizableWindow::backgroundColourId, airwindowsLookAndFeel.defaultColour);
        airwindowsLookAndFeel.setColour(juce::Slider::thumbColourId, airwindowsLookAndFeel.defaultColour);
    }
    updateTrackProperties();

    idleTimer = std::make_unique<IdleTimer>(this);
    idleTimer->startTimer(1000/30); //space between UI screen updates. Larger is slower updates to screen

    meter.setOpaque(true);
    meter.resetArrays();
    addAndMakeVisible(meter);
    
    setSize (1200, 675);
    // Make sure that before the constructor has finished, you've set the editor's size to whatever you need it to be.
    if (airwindowsLookAndFeel.usingNamedImage) {
        getConstrainer()->setFixedAspectRatio(1200.0/675.0); //the aspect ratio stuff leads to cropping the content area off the top
        setResizeLimits(32, 32, 4196, 4196); //this will not honor resize limits correctly in all the DAWs
    }

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    //the aspect ratio stuff leads to cropping the content area off the top
}

PluginEditor::~PluginEditor(){
    setLookAndFeel(nullptr); //we do need to set the look and feel to null pointer
}

void PluginEditor::paint (juce::Graphics& g)
{
    if (airwindowsLookAndFeel.blurImage == juce::Image()) {
        g.fillAll (airwindowsLookAndFeel.defaultColour);
        if (hostTrackColour != juce::Colour()) {
            g.setFillType(juce::FillType(hostTrackColour)); g.setOpacity(airwindowsLookAndFeel.applyTrackColour); g.fillAll();
        }
        airwindowsLookAndFeel.setColour(juce::ResizableWindow::backgroundColourId, airwindowsLookAndFeel.defaultColour.interpolatedWith (hostTrackColour, airwindowsLookAndFeel.applyTrackColour));
        airwindowsLookAndFeel.setColour(juce::Slider::thumbColourId, airwindowsLookAndFeel.defaultColour.interpolatedWith (hostTrackColour, airwindowsLookAndFeel.applyTrackColour));
    } else {
        if (airwindowsLookAndFeel.usingNamedImage) {
            g.drawImageWithin(airwindowsLookAndFeel.backgroundImage, 0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight(), 0);
        } else {
            g.setTiledImageFill(airwindowsLookAndFeel.backgroundImage, 0, 0, 1.0f); g.fillAll();
        }
                
        if (hostTrackColour != juce::Colour()) {
            g.setFillType(juce::FillType(hostTrackColour)); g.setOpacity(airwindowsLookAndFeel.applyTrackColour); g.fillAll();
        }
        airwindowsLookAndFeel.defaultColour = juce::Colour::fromRGBA(airwindowsLookAndFeel.blurImage.getPixelAt(1,1).getRed(),airwindowsLookAndFeel.blurImage.getPixelAt(1,1).getGreen(),airwindowsLookAndFeel.blurImage.getPixelAt(1,1).getBlue(),1.0);
        airwindowsLookAndFeel.setColour(juce::ResizableWindow::backgroundColourId, airwindowsLookAndFeel.defaultColour);
        airwindowsLookAndFeel.setColour(juce::Slider::thumbColourId, airwindowsLookAndFeel.defaultColour);
    } //find the color of the background tile or image, if there is one. Please use low-contrast stuff, but I'm not your mom :)
    if (airwindowsLookAndFeel.newFont == juce::String()) airwindowsLookAndFeel.newFont = "Jost";
    g.setFont(juce::Font(airwindowsLookAndFeel.newFont, g.getCurrentFont().getHeight(), 0));
    auto linewidth = getLocalBounds().getWidth(); if (getLocalBounds().getHeight() > linewidth) linewidth = getLocalBounds().getHeight();  linewidth = (int)cbrt(linewidth/2)/2;
    if ((hostTrackName == juce::String()) || (hostTrackName.length() < 1.0f)) hostTrackName = juce::String("Hit Record Meter"); //if not track name, then name of pluginu
    float radius = getLocalBounds().getWidth(); if (radius > (getLocalBounds().getHeight())*0.0618f) radius = (getLocalBounds().getHeight())*0.0618f;
    auto embossScale = sqrt(sqrt(radius*0.618f)*1.618f)*0.618f; //this is customized to the needs of the plugin title text area
    g.setFont ((radius*12.0f) / (float)g.getCurrentFont().getHeight());
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.75f)); //highlight
    g.drawFittedText(hostTrackName, juce::Rectangle<int>((int)(getLocalBounds().getWidth()+embossScale),(int)((getLocalBounds().getHeight()*0.0618f)+embossScale)), juce::Justification::centredBottom, 1);
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.75f)); //shadow
    g.drawFittedText(hostTrackName, juce::Rectangle<int>((int)(getLocalBounds().getWidth()-embossScale),(int)((getLocalBounds().getHeight()*0.0618f)-embossScale)), juce::Justification::centredBottom, 1);
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.25f)); //text inside emboss
    g.drawFittedText(hostTrackName, juce::Rectangle<int>((int)getLocalBounds().getWidth(),(int)(getLocalBounds().getHeight()*0.0618f)), juce::Justification::centredBottom, 1);
    //draw the track name or plugin name embossed. This is because there's an unlimited range of colors and textures that could be in play.
    
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.618f));
    g.fillRect(0, 0, getLocalBounds().getWidth(), linewidth); g.fillRect(0, 0, linewidth, getLocalBounds().getHeight());
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.382f));
    g.fillRect(linewidth, getLocalBounds().getHeight()-linewidth, getLocalBounds().getWidth(), linewidth); g.fillRect(getLocalBounds().getWidth()-linewidth, linewidth, linewidth, getLocalBounds().getHeight()-linewidth);
    g.setColour (juce::Colours::black); g.drawRect(0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight());
    //draw global bevel effect, either from the color or from the color of the blurred texture, and a black border
}

void PluginEditor::resized()
{
    auto area = getLocalBounds();
    auto linewidth = area.getWidth();
    if (area.getHeight() > linewidth) linewidth = area.getHeight();
    linewidth = (int)cbrt(linewidth/2)/2;
    meter.displayWidth = (int)((1.0f-(((float)linewidth*4.0f)/area.getWidth()))*(float)area.getWidth());
    meter.displayHeight = (int)((0.95f-(((float)linewidth*2.0f)/area.getHeight()))*(float)area.getHeight());
    area.reduce(linewidth, linewidth);
    //getProportion sets first start X and Y placement, then size X and Y placement
    
    meter.setBounds(area.getProportion(juce::Rectangle{((float)linewidth*2.0f)/area.getWidth(), 0.05f, 1.0f-(((float)linewidth*4.0f)/area.getWidth()), 0.95f-(((float)linewidth*2.0f)/area.getHeight())}));
}

void PluginEditor::sliderValueChanged(juce::Slider *s) {sliderDragInternal(s, false);} //no knobs
void PluginEditor::sliderDragStarted(juce::Slider *s) {sliderDragInternal(s, true);} //on this plugin
void PluginEditor::sliderDragEnded(juce::Slider *s) {sliderDragInternal(s, false);} //so this section does
void PluginEditor::sliderDragInternal(juce::Slider *s, bool bv) {if (bv) sliderValueChanged(s);} //nothing

void PluginEditor::updateTrackProperties() {hostTrackColour=processorRef.trackProperties.colour; hostTrackName=processorRef.trackProperties.name; repaint();}

void PluginEditor::idle()
{
    PluginProcessor::AudioToUIMessage msg;
    bool repaintTS{false}; //we don't redraw interface just for getting data into the GUI section
    while (processorRef.audioToUI.pop(msg)) {
        switch (msg.what) {
        case PluginProcessor::AudioToUIMessage::NEW_VALUE: break; //no knobs on this plugin
                
        case PluginProcessor::AudioToUIMessage::RMS_LEFT: meter.pushA(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::RMS_RIGHT: meter.pushB(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::PEAK_LEFT: meter.pushC(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::PEAK_RIGHT: meter.pushD(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::SLEW_LEFT: meter.pushE(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::SLEW_RIGHT: meter.pushF(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::ZERO_LEFT: meter.pushG(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::ZERO_RIGHT: meter.pushH(msg.newValue); break;
                
        case PluginProcessor::AudioToUIMessage::INCREMENT: //Increment is running at 24 FPS and giving the above calculations
                meter.pushIncrement(msg.newValue); repaintTS = true; break;
        
        default: std::cout << "Unhandled message type " << msg.what << std::endl; break;
        } //end of switch statement for msg.what
    }
    if (repaintTS) meter.repaint();
}
