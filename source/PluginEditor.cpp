#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    setResizable(true, true);
    setLookAndFeel(&airwindowsLookAndFeel);
    juce::Desktop::getInstance().setDefaultLookAndFeel(&airwindowsLookAndFeel);
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
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    //the aspect ratio stuff leads to cropping the content area off the top
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::paint (juce::Graphics& g)
{
    if (hostTrackColour != juce::Colour()) {
        airwindowsLookAndFeel.setColour(juce::ResizableWindow::backgroundColourId, hostTrackColour);
        airwindowsLookAndFeel.setColour(juce::Slider::thumbColourId, hostTrackColour);
    } else {
        airwindowsLookAndFeel.setColour(juce::ResizableWindow::backgroundColourId, airwindowsLookAndFeel.defaultColour);
        airwindowsLookAndFeel.setColour(juce::Slider::thumbColourId, airwindowsLookAndFeel.defaultColour);
    }
    if (hostTrackName == juce::String()) {
        hostTrackName = juce::String("Airwindows Meter");
    }
    auto global = getLocalBounds();
    auto linewidth = global.getWidth();
    if (global.getHeight() > linewidth) linewidth = global.getHeight();
    linewidth = (int)cbrt(linewidth/2)/2;
    
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.382f));
    g.fillRect(0, 0, global.getWidth(), linewidth);
    g.fillRect(0, 0, linewidth, global.getHeight());
    
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.618f));
    g.fillRect(linewidth, global.getHeight()-linewidth, global.getWidth(), linewidth);
    g.fillRect(global.getWidth()-linewidth, linewidth, linewidth, global.getHeight()-linewidth);
    
    float radius = global.getWidth();
    if (radius > (global.getHeight())*0.0618f) radius = (global.getHeight())*0.0618f;
    auto labelScale = sqrt(radius*0.618f)*1.618f;
    auto embossScale = sqrt(labelScale)*0.618f;
    g.setFont ((radius*12.0f) / (float)g.getCurrentFont().getHeight());
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::white, 0.75f)); //highlight
    g.drawFittedText(hostTrackName, juce::Rectangle<int>((int)(global.getWidth()+embossScale),(int)((global.getHeight()*0.0618f)+embossScale)), juce::Justification::centredBottom, 1);
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.75f)); //shadow
    g.drawFittedText(hostTrackName, juce::Rectangle<int>((int)(global.getWidth()-embossScale),(int)((global.getHeight()*0.0618f)-embossScale)), juce::Justification::centredBottom, 1);
    g.setColour (findColour(juce::ResizableWindow::backgroundColourId).interpolatedWith (juce::Colours::black, 0.25f)); //text inside emboss
    g.drawFittedText(hostTrackName, juce::Rectangle<int>((int)global.getWidth(),(int)(global.getHeight()*0.0618f)), juce::Justification::centredBottom, 1);
    
    g.setColour (juce::Colours::black);
    g.drawRect(0, 0, global.getWidth(), global.getHeight());
}

void PluginEditor::resized()
{
    auto area = getLocalBounds();
    auto linewidth = area.getWidth();
    if (area.getHeight() > linewidth) linewidth = area.getHeight();
    linewidth = (int)cbrt(linewidth/2)/2;
    meter.displayWidth = (1.0f-(((float)linewidth*4.0f)/area.getWidth()))*(float)area.getWidth();
    meter.displayHeight = (0.95f-(((float)linewidth*2.0f)/area.getHeight()))*(float)area.getHeight();
    area.reduce(linewidth, linewidth);
    //getProportion sets first start X and Y placement, then size X and Y placement
    
    meter.setBounds(area.getProportion(juce::Rectangle{((float)linewidth*2.0f)/area.getWidth(), 0.05f, 1.0f-(((float)linewidth*4.0f)/area.getWidth()), 0.95f-(((float)linewidth*2.0f)/area.getHeight())}));
}

void PluginEditor::sliderValueChanged(juce::Slider *s)
{
}

void PluginEditor::sliderDragStarted(juce::Slider *s)
{
    sliderDragInternal(s, true);
}

void PluginEditor::sliderDragEnded(juce::Slider *s)
{
    sliderDragInternal(s, false);
}

void PluginEditor::sliderDragInternal(juce::Slider *s, bool bv)
{
}

void PluginEditor::updateTrackProperties()
{
    hostTrackColour = processorRef.trackProperties.colour;
    hostTrackName = processorRef.trackProperties.name;
    repaint();
}

void PluginEditor::idle()
{
    PluginProcessor::AudioToUIMessage msg;
    bool repaintTS{false};
    while (processorRef.audioToUI.pop(msg))
    {
        switch (msg.what) {
        case PluginProcessor::AudioToUIMessage::NEW_VALUE:
                break;
        case PluginProcessor::AudioToUIMessage::RMS_LEFT:
                meter.pushA(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::RMS_RIGHT:
                meter.pushB(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::PEAK_LEFT:
                meter.pushC(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::PEAK_RIGHT:
                meter.pushD(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::SLEW_LEFT:
                meter.pushE(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::SLEW_RIGHT:
                meter.pushF(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::ZERO_LEFT:
                meter.pushG(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::ZERO_RIGHT:
                meter.pushH(msg.newValue); break;
        case PluginProcessor::AudioToUIMessage::INCREMENT:
                meter.pushIncrement(msg.newValue); repaintTS = true; break;
        default:
                std::cout << "Unhandled message type " << msg.what << std::endl; break;
        }
    } //this appears to only fire when JUCE is idle for a moment.
    if (repaintTS)
        meter.repaint();
}
