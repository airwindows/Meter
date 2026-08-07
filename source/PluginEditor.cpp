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
    updatePluginSize();
    
    idleTimer = std::make_unique<IdleTimer>(this);
    idleTimer->startTimer(1000/30); //space between UI screen updates. Larger is slower updates to screen
    
    meter.pluginColour = airwindowsLookAndFeel.defaultColour;
    meter.setOpaque(true);
    meter.resetArrays();
    meter.addMouseListener(this, false);
    addAndMakeVisible(meter);
    meter.outputR = 10.0;
    meter.outputG = 10.0;
    meter.outputB = 10.0;
    setSize (airwindowsLookAndFeel.userWidth, airwindowsLookAndFeel.userHeight);
    // Make sure that before the constructor has finished, you've set the editor's size to whatever you need it to be.
}

PluginEditor::~PluginEditor(){
    setLookAndFeel(nullptr); //we do need to set the look and feel to null pointer
}

void PluginEditor::paint (juce::Graphics& g)
{
    if (airwindowsLookAndFeel.alfInterpolation == 0) g.setImageResamplingQuality(g.lowResamplingQuality);
    if (airwindowsLookAndFeel.alfInterpolation == 1) g.setImageResamplingQuality(g.mediumResamplingQuality);
    if (airwindowsLookAndFeel.alfInterpolation == 2) g.setImageResamplingQuality(g.highResamplingQuality); //copy this section to other stretched-bitmap meters
    if (airwindowsLookAndFeel.blurImage == juce::Image()) { // == means image is BLANK so we do a color
        g.fillAll (airwindowsLookAndFeel.defaultColour);
        if (hostTrackColour != juce::Colour()) {
            g.setFillType(juce::FillType(hostTrackColour)); g.setOpacity(airwindowsLookAndFeel.applyTrackColour);
        }
        airwindowsLookAndFeel.setColour(juce::ResizableWindow::backgroundColourId, airwindowsLookAndFeel.defaultColour.interpolatedWith (hostTrackColour, airwindowsLookAndFeel.applyTrackColour));
        airwindowsLookAndFeel.setColour(juce::Slider::thumbColourId, airwindowsLookAndFeel.defaultColour.interpolatedWith (hostTrackColour, airwindowsLookAndFeel.applyTrackColour));
        airwindowsLookAndFeel.setColour(juce::TextButton::buttonColourId, airwindowsLookAndFeel.defaultColour.interpolatedWith (hostTrackColour, airwindowsLookAndFeel.applyTrackColour));
        airwindowsLookAndFeel.setColour(juce::TextButton::buttonOnColourId, airwindowsLookAndFeel.defaultColour.interpolatedWith (hostTrackColour, airwindowsLookAndFeel.applyTrackColour));
        meter.pluginColour = airwindowsLookAndFeel.defaultColour.interpolatedWith (hostTrackColour, airwindowsLookAndFeel.applyTrackColour);
        if (airwindowsLookAndFeel.useToneColor) {
            g.fillAll (meter.backdropColour);
            airwindowsLookAndFeel.setColour(juce::ResizableWindow::backgroundColourId, meter.backdropColour);
            airwindowsLookAndFeel.setColour(juce::Slider::thumbColourId, meter.backdropColour);
            airwindowsLookAndFeel.setColour(juce::TextButton::buttonColourId, meter.backdropColour.interpolatedWith (hostTrackColour, airwindowsLookAndFeel.applyTrackColour));
            airwindowsLookAndFeel.setColour(juce::TextButton::buttonOnColourId, meter.backdropColour.interpolatedWith (hostTrackColour, airwindowsLookAndFeel.applyTrackColour));
        } else {
            g.fillAll (airwindowsLookAndFeel.defaultColour);
        }
    } else { //we have an image that might be named and actual size, or might be the overall tiled texture you can use
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
        airwindowsLookAndFeel.setColour(juce::TextButton::buttonColourId, airwindowsLookAndFeel.defaultColour);
        airwindowsLookAndFeel.setColour(juce::TextButton::buttonOnColourId, airwindowsLookAndFeel.defaultColour);
        meter.pluginColour = airwindowsLookAndFeel.defaultColour;
    } //find the color of the background tile or image, if there is one. Please use low-contrast stuff, but I'm not your mom :)
    
    
    if (airwindowsLookAndFeel.newFont == juce::String()) airwindowsLookAndFeel.newFont = "Jost";
    float whiteLevel = (findColour(juce::ResizableWindow::backgroundColourId).getLightness()+0.37f)*0.618f; //apply about half of this
    float blackLevel = 1.0f-whiteLevel;
    whiteLevel *= whiteLevel;
    blackLevel *= blackLevel;
    
    auto area = getLocalBounds(); // this is a huge huge routine, but not all of it runs at all times!
    auto linewidth = (int)fmin(area.getHeight(),area.getWidth());
    linewidth = (int)cbrt(linewidth/2)-1;
    area.reduce((int)(linewidth*1.618f), (int)(linewidth*1.618f));
    g.setColour(juce::Colours::white); g.setOpacity(whiteLevel);
    g.fillRect(0, 0, getLocalBounds().getWidth()-linewidth, linewidth);
    g.fillRect(0, linewidth, linewidth, getLocalBounds().getHeight()-(linewidth*2));
    g.setColour(juce::Colours::black); g.setOpacity(blackLevel);
    g.fillRect(linewidth, getLocalBounds().getHeight()-linewidth, getLocalBounds().getWidth(), linewidth);
    g.fillRect(getLocalBounds().getWidth()-linewidth, linewidth, linewidth, getLocalBounds().getHeight()-linewidth);
    
    g.setColour (juce::Colours::black);
    g.setOpacity(0.384f); g.drawRect(2, 2, getLocalBounds().getWidth()-4, getLocalBounds().getHeight()-4);
    g.setOpacity(0.618f); g.drawRect(1, 1, getLocalBounds().getWidth()-2, getLocalBounds().getHeight()-2);
    g.setOpacity(1.000f); g.drawRect(0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight());
    //draw global bevel effect, either from the color or from the color of the blurred texture, and a black border
}

void PluginEditor::resized()
{
    auto area = getLocalBounds();
    processorRef.pluginWidth = airwindowsLookAndFeel.userWidth = area.getWidth();
    processorRef.pluginHeight = airwindowsLookAndFeel.userHeight = area.getHeight();
    auto linewidth = area.getWidth();
    if (area.getHeight() > linewidth) linewidth = area.getHeight();
    linewidth = (int)cbrt(linewidth/2)-1;
    area.reduce((int)(linewidth*1.618f), (int)(linewidth*1.618f));
    meter.displayWidth = (int)area.getWidth();
    meter.displayHeight = (int)area.getHeight(); //meter-only display
    meter.setBounds(area);
}

void PluginEditor::sliderValueChanged(juce::Slider *s) {}                           //there are no sliders
void PluginEditor::sliderDragStarted(juce::Slider *s) {sliderDragInternal(s, true);}      //on this plugin
void PluginEditor::sliderDragEnded(juce::Slider *s) {sliderDragInternal(s, false);} //so this section does
void PluginEditor::sliderDragInternal(juce::Slider *s, bool bv) {if (bv) sliderValueChanged(s);} //nothing

void PluginEditor::updateTrackProperties() {hostTrackColour=processorRef.trackProperties.colour; hostTrackName=processorRef.trackProperties.name; repaint();}


void PluginEditor::updatePluginSize() {
    airwindowsLookAndFeel.userWidth = processorRef.pluginWidth;
    airwindowsLookAndFeel.userHeight = processorRef.pluginHeight;
    if (airwindowsLookAndFeel.userWidth < 8 || airwindowsLookAndFeel.userWidth > 16386) airwindowsLookAndFeel.userWidth = 1280;
    if (airwindowsLookAndFeel.userHeight < 8 || airwindowsLookAndFeel.userHeight > 16386) airwindowsLookAndFeel.userHeight = 720;
    repaint();
}

void PluginEditor::idle()
{
    PluginProcessor::AudioToUIMessage msg;
    bool repaintTS{false}; //we don't redraw interface just for getting data into the GUI section
    while (processorRef.audioToUI.pop(msg)) {
        switch (msg.what) {
            case PluginProcessor::AudioToUIMessage::NEW_VALUE: break; //no knobs on this plugin
            case PluginProcessor::AudioToUIMessage::PEAK_LEFT: meter.pushPeakL(msg.newValue); break;
            case PluginProcessor::AudioToUIMessage::PEAK_RIGHT: meter.pushPeakR(msg.newValue); break;
            case PluginProcessor::AudioToUIMessage::SLEW_LEFT: meter.pushSlewL(msg.newValue); break;
            case PluginProcessor::AudioToUIMessage::SLEW_RIGHT: meter.pushSlewR(msg.newValue); break;
            case PluginProcessor::AudioToUIMessage::ZERO_LEFT: meter.pushZeroL(msg.newValue); break;
            case PluginProcessor::AudioToUIMessage::ZERO_RIGHT: meter.pushZeroR(msg.newValue); break;
            case PluginProcessor::AudioToUIMessage::INCREMENT: //Increment is running at 24 FPS and giving the above calculations
                meter.pushIncrement(); repaintTS = true;
                
                meter.outputMax = fmax(fmax(meter.outputR+0.00000001f,meter.outputG),meter.outputB);
                meter.outputMin = fmin(fmin(meter.outputR,meter.outputG),meter.outputB) / meter.outputMax;
                meter.gradientMin = meter.outputMin*meter.outputMin*meter.outputMin;
                //smallest number means brightest color, relative to highest number will be 1.0 meaning white
                meter.outputMin = 1.0f-((1.0f-meter.outputMin)*0.125f*meter.outputVol*meter.outputVol);
                //quiet parts converge on 1.0 also, to slow color activity in fades
                meter.outputMin = fmax(fmin(meter.outputMin,1.0f),0.0f);
                meter.storeR = pow(meter.outputR/meter.outputMax, 1.618033988749894f);;
                meter.storeG = pow(meter.outputG/meter.outputMax, 1.618033988749894f);
                meter.storeB = pow(meter.outputB/meter.outputMax, 1.618033988749894f);
                //if all these are nearly white, they're near 1.0, but cyan is too much like white to be visible.
                //this causes scores to look wrong, because cyan tints 'should' score higher but it's an optical illusion.
                //Cyan is R0 G1 B1, so we will make up a color-tinter that reduces G and B by the amount that they match,
                meter.cyanFix = pow((1.0f-meter.storeR)*(1.0f-meter.storeR)*(1.0f-((meter.storeG-meter.storeB)*(meter.storeG-meter.storeB))),1.618033988749894f);
                meter.storeG -= meter.cyanFix; meter.storeB -= meter.cyanFix; //darken cyan tints for visual reference to white balance
                meter.backdropColour = juce::Colour::fromFloatRGBA (meter.storeR, meter.storeG, meter.storeB, 1.0f);
                meter.outputR *= meter.outputMin;
                meter.outputG *= meter.outputMin;
                meter.outputB *= meter.outputMin;
                break;
            default: std::cout << "Unhandled message type " << msg.what << std::endl; break;
        } //end of switch statement for msg.what
    }
    if (repaintTS) {
        if (airwindowsLookAndFeel.useToneColor && meter.backdropColour.operator!=(meter.cachedColour)) {
            repaint();
            meter.cachedColour = meter.backdropColour;
            //we're going to maybe NOT constantly repaint everything, even in tonecolor mode
        } else {
            meter.repaint();
        }
        //normally it's the meter we have to update without touching it with the mouse,
        //but if the whole plugin has to be tone color, it all must repaint.
    }
}
