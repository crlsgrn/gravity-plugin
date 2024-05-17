/*
  ==============================================================================

    OtherLookAndFeel.h
    Created: 29 Sep 2023 10:09:23am
    Author:  Carlos Garin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class OtherLookAndFeel : public juce::LookAndFeel_V4
{
public:
    OtherLookAndFeel(){
        
    }
    void drawRotarySlider (juce::Graphics&, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
                           juce::Slider&) override;

};
