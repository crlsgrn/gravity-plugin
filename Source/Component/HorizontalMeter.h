/*
  ==============================================================================

    HorizontalMeter.h
    Created: 8 Sep 2023 3:01:39pm
    Author:  Carlos Garin

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace Gui {
class HorizontalMeter : public juce::Component{
public:
    void paint(juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();

        g.setColour(juce::Colours::white.withBrightness(0.4f));
        g.fillRoundedRectangle(bounds, 5.f);

        g.setColour(juce::Colours::white);
        const auto scaledX = juce::jmap(level, -60.f, 6.f, 0.f, static_cast<float>(getWidth()));
        g.fillRoundedRectangle(bounds.removeFromLeft(scaledX), 5.f);
    }
    void setLevel(const float value) { level = value; }
private:
    float level = -60.f;
};
}
