/*
  ==============================================================================

    OtherLookAndFeel.cpp
    Created: 29 Sep 2023 10:09:23am
    Author:  Carlos Garin

  ==============================================================================
*/

#include "OtherLookAndFeel.h"

//VERSION MODERNA MINIMALISTA
void OtherLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                                       const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    float diameter = juce::jmin(width, height)*0.7f;
    const float radius = diameter / 2;
    const float centreX = x + width  * 0.5f;
    const float centreY =  y + height * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));

    juce::Rectangle<float> dialArea (rx, ry, diameter, diameter);

    g.setColour(juce::Colours::transparentBlack);
    g.fillEllipse(dialArea);

    g.setColour(juce::Colours::black);

    juce::Path dialTick;
    dialTick.addRectangle (0, -radius, 2.0f, radius * 0.5);

    g.fillPath (dialTick, juce::AffineTransform::rotation(angle).translated(centreX, centreY));

    g.setColour(juce::Colours::black);
    g.drawEllipse(rx, ry, diameter, diameter, 10.0f);

}



