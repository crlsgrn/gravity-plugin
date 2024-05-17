/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ListenerComponent.h"
#include "BroadcasterComponent.h"
#include "Component/HorizontalMeter.h"
#include "OtherLookAndFeel.h"




struct CustomRotarySlider2 : juce::Slider
{
    CustomRotarySlider2() : juce::Slider(juce::Slider::SliderStyle::RotaryVerticalDrag,
                                        juce::Slider::TextEntryBoxPosition::TextBoxAbove)
    {

    }
};

struct PowerButton : juce::ToggleButton { };




/**
*/
//linea 236 del repositorio
class GravityAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Button::Listener
{
public:
    GravityAudioProcessorEditor (GravityAudioProcessor&);
    ~GravityAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked (juce::Button *button) override;
    

private:
    bool manualChange = false;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    /*ListenerComponent listenerComponent;
    BroadcasterComponent broadcasterComponent;*/
    GravityAudioProcessor& audioProcessor;
    juce::TextButton text;

    CustomRotarySlider2 leftAttackSlider, rightAttackSlider, leftThresholdSlider, rightThresholdSlider, leftRatioSlider, rightRatioSlider, leftKneeSlider, rightKneeSlider, leftReleaseSlider, rightReleaseSlider, leftMakeUpSlider, rightMakeUpSlider, leftBlendSlider, rightBlendSlider, leftHpfFreqSlider, rightHpfFreqSlider, leftLpfFreqSlider, rightLpfFreqSlider, leftNotchFreqSlider, leftNotchQSlider, rightNotchFreqSlider, rightNotchQSlider;
    
    juce::ComboBox leftThresholdRes, rightThresholdRes, leftMakeUpRes, rightMakeUpRes, leftBlendRes, rightBlendRes, operationMode, vuMode, leftFastPreset, rightFastPreset;
    
    using APVTS = juce::AudioProcessorValueTreeState;
    using AttachmentSlider = APVTS::SliderAttachment;
    
    std::vector<juce::Component*> getComps();
    std::vector<juce::Component*> getCompsLF();
    std::vector<juce::Component*> getCompsOtherLF();
    
        
    PowerButton leftThresholdBypass, rightThresholdBypass, leftMakeUpBypass, rightMakeUpBypass, leftBlendBypass, rightBlendBypass, leftBypass, rightBypass, leftMute, rightMute, scExtButton, leftHpfscButton, leftLpfscButton, leftNotchButton,  rightHpfscButton, rightLpfscButton, rightNotchButton, fastPresetButton2, sendButton, linkChannels, linkAbsolute;
    //BroadcasterComponent notchButton;
    //ListenerComponent lpfscButton;
    Gui::HorizontalMeter horizontalMeterL, horizontalMeterR;
    std::unique_ptr<gin::WebSocket> ws;
    gin::GinLookAndFeel ginLookAndFeel;
    OtherLookAndFeel otherLookAndFeel;
    juce::Image backgroundImage;
    juce::ImageComponent imageComponent;
    
    


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GravityAudioProcessorEditor)
};

