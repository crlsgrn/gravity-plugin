/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "plugin.h"
#include "easywsclient.hpp"


//==============================================================================
/**
*/
class GravityAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    GravityAudioProcessor();
    ~GravityAudioProcessor() override;
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif
    
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
    //==============================================================================
    const juce::String getName() const override;
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    
    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    
    //using APVTS = juce::AudioProcessorValueTreeState;
    //static APVTS::ParameterLayout createParameterLayout();
    
    //APVTS apvts {*this, nullptr, "Parameters", createParameterLayout() };
    
    //juce::AudioProcessorValueTreeState apvts;
    
    void init();
    void syncParameters();
    
    AudioStateGravity getState();
    int setState(AudioStateGravity _state);
    
    
    // Setters y Getters
    bool setOperationMode(int _operationMode, bool _sync);
    bool setVUmeterMode(int _vuMode, bool _sync);
    bool setFastPreset(int i, int _channel, int _preset, bool _sync);
    bool setLinkChanels(bool _link, bool _sync);
    bool setLinkAbsolute(bool _absolute, bool _sync);
    bool setThreshold(int _channel, float _threshold, bool _sync);
    bool setThresholdResolution(int _channel, float _res, bool _sync);
    bool setThresholdBypass(int _channel, bool _bypass, bool _sync);
    bool setAttack(int _channel, int _attack, bool _sync);
    bool setRelease(int _channel, int _release, bool _sync);
    bool setRatio(int _channel, int _ratio, bool _sync);
    bool setKnee(int _channel, int _knee, bool _sync);
    bool setMakeUp(int _channel, float _makeUp, bool _sync);
    bool setMakeUpResolution(int _channel, float _res, bool _sync);
    bool setMakeUpBypass(int _channel, bool _bypass, bool _sync);
    bool setBlend(int _channel, int _blend, bool _sync);
    bool setBlendResolution(int _channel, int _res, bool _sync);
    bool setBlendBypass(int _channel, bool _bypass, bool _sync);
    bool setNotch(int _channel, bool _notch, bool _sync);
    bool setNotchFreq(int _channel, float _freq, bool _sync);
    bool setNotchQ(int _channel, float _q, bool _sync);
    bool setLPF(int _channel, bool _lpf, bool _sync);
    bool setLPFFreq(int _channel, float _freq, bool _sync);
    bool setHPF(int _channel, bool _hpf, bool _sync);
    bool setHPFFreq(int _channel, float _freq, bool _sync);
    bool setGeneralBypass(int _channel, bool _bypass, bool _sync);
    bool setGeneralMute(int _channel, bool _mute, bool _sync);
    
    
    
private:
    struct Command
    {
        juce::String command;
        juce::var data;
    };
    
    AudioChannelGravity channel;
    AudioStateGravity state;
    juce::var audioStateJson;
    // Crear un objeto DynamicObject para representar audioState como JSON.
    juce::DynamicObject* audioStateObject = new juce::DynamicObject();
    easywsclient::WebSocket *ws = NULL;
    
//CREAR UN HILO

    
    juce::AudioParameterChoice* operationMode{ nullptr };
    juce::AudioParameterChoice* vuMode { nullptr };
    juce::AudioParameterChoice* fastPreset { nullptr };
    juce::AudioParameterChoice* channelSelector{ nullptr };

    
    juce::AudioParameterFloat* leftThreshold { nullptr };
    juce::AudioParameterBool* leftThresholdBypass{ nullptr };
    juce::AudioParameterChoice* leftThresholdResolution{ nullptr };
    
    juce::AudioParameterFloat* rightThreshold { nullptr };
    juce::AudioParameterBool* rightThresholdBypass{ nullptr };
    juce::AudioParameterChoice* rightThresholdResolution{ nullptr };
    
    juce::AudioParameterChoice* leftAttack { nullptr };
    juce::AudioParameterChoice* rightAttack { nullptr };
    
    juce::AudioParameterChoice* leftRelease { nullptr };
    juce::AudioParameterChoice* rightRelease { nullptr };
    
    juce::AudioParameterChoice* leftRatio { nullptr };
    juce::AudioParameterChoice* rightRatio { nullptr };
    
    juce::AudioParameterChoice* leftKnee { nullptr };
    juce::AudioParameterChoice* rightKnee { nullptr };
    
    
    juce::AudioParameterFloat* leftMakeUp { nullptr };
    juce::AudioParameterBool* leftMakeUpBypass{ nullptr };
    juce::AudioParameterChoice* leftMakeUpResolution{ nullptr };

    juce::AudioParameterFloat* rightMakeUp { nullptr };
    juce::AudioParameterBool* rightMakeUpBypass{ nullptr };
    juce::AudioParameterChoice* rightMakeUpResolution{ nullptr };
    
    juce::AudioParameterFloat* leftBlend { nullptr };
    juce::AudioParameterBool* leftBlendBypass{ nullptr };
    juce::AudioParameterChoice* leftBlendResolution{ nullptr };
    
    juce::AudioParameterFloat* rightBlend { nullptr };
    juce::AudioParameterBool* rightBlendBypass{ nullptr };
    juce::AudioParameterChoice* rightBlendResolution{ nullptr };
    
    juce::AudioParameterBool* leftBypass{ nullptr };
    juce::AudioParameterBool* leftMute{ nullptr };
    
    juce::AudioParameterBool* rightBypass{ nullptr };
    juce::AudioParameterBool* rightMute{ nullptr };
    
    
    //Filters
    
    //juce::AudioParameterBool* scExt{ nullptr };
    
    juce::AudioParameterBool* leftHpf{ nullptr };
    juce::AudioParameterFloat* leftHpfFreq{ nullptr };

    juce::AudioParameterBool* rightHpf{ nullptr };
    juce::AudioParameterFloat* rightHpfFreq{ nullptr };
    
    juce::AudioParameterBool* leftLpf{ nullptr };
    juce::AudioParameterFloat* leftLpfFreq{ nullptr };
    
    juce::AudioParameterBool* rightLpf{ nullptr };
    juce::AudioParameterFloat* rightLpfFreq{ nullptr };
    
    juce::AudioParameterBool* leftNotch{ nullptr };
    juce::AudioParameterFloat* leftNotchFreq{ nullptr };
    juce::AudioParameterFloat* leftNotchQ{ nullptr };

    juce::AudioParameterBool* rightNotch{ nullptr };
    juce::AudioParameterFloat* rightNotchFreq{ nullptr };
    juce::AudioParameterFloat* rightNotchQ{ nullptr };
    
    
    
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GravityAudioProcessor)
};
