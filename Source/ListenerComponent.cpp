
/*
  ==============================================================================

    ListenerComponent.cpp
    Created: 6 Sep 2023 8:25:19am
    Author:  Carlos Garin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ListenerComponent.h"

//==============================================================================
ListenerComponent::ListenerComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    receiverButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::darkred);

    
}

ListenerComponent::~ListenerComponent()
{
}

void ListenerComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll(juce::Colours::slategrey);
/*    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawFittedText("Listener", getLocalBounds(), juce::Justification::topLeft, 1);
    */
}

void ListenerComponent::resized()
{
    receiverButton.setBounds(getLocalBounds().reduced(30));

}

void ListenerComponent::changeListenerCallback (juce::ChangeBroadcaster *source) {
    receiverButton.setToggleState(false, juce::dontSendNotification);
}
