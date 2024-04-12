#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    setResizable(true, true);
   // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    idleTimer = std::make_unique<IdleTimer>(this);
    idleTimer->startTimer(1000/30); //space between UI screen updates. Larger is slower updates to screen

    meter.setOpaque(true);
    meter.resetArrays();
    addAndMakeVisible(meter);

    setSize (1200, 600);
    //the aspect ratio stuff leads to cropping the content area off the top
    
    setResizeLimits(600, 300, 1200, 600);
    //place this last and it will call resized() on everything within
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::paint (juce::Graphics& g)
{
}

void PluginEditor::resized()
{
    auto area = getLocalBounds();
    meter.setBounds(area.getProportion(juce::Rectangle{0.0f, 0.0f, 1.0f, 1.0f}));
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
