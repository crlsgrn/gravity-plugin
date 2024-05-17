
/*
  ==============================================================================

    ListenerComponent.h
    Created: 6 Sep 2023 8:25:19am
    Author:  Carlos Garin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ListenerComponent  : public juce::Component, public juce::ChangeListener
{
public:
    ListenerComponent();
    ~ListenerComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void changeListenerCallback (juce::ChangeBroadcaster *source) override;

private:
    juce::ToggleButton receiverButton {"Receiver"};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ListenerComponent)
};
