
/*
  ==============================================================================

    BroadcasterComponent.h
    Created: 6 Sep 2023 8:26:39am
    Author:  Carlos Garin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class BroadcasterComponent  : public juce::Component, juce::ChangeBroadcaster
{
public:
    BroadcasterComponent(juce::ChangeListener& l);
    ~BroadcasterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent &event) override;
private:
    juce::ChangeListener& listener;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BroadcasterComponent)
};
