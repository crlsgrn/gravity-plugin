
/*
  ==============================================================================

    BroadcasterComponent.cpp
    Created: 6 Sep 2023 8:26:39am
    Author:  Carlos Garin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "BroadcasterComponent.h"

//==============================================================================
BroadcasterComponent::BroadcasterComponent(juce::ChangeListener& l): listener (l)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addChangeListener(&listener);
}

BroadcasterComponent::~BroadcasterComponent()
{
    removeChangeListener(&listener);
}

void BroadcasterComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(30.0f);
    g.drawFittedText("Notch", getLocalBounds(), juce::Justification::centred, 2);
 
}

void BroadcasterComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    setBounds(30, 150, 100, 50);


}

void BroadcasterComponent::mouseDown(const juce::MouseEvent &event){
    
    sendChangeMessage();
    std::cout << "I´ve been clicked" << std::endl;
    
}



