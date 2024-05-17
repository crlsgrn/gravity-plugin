/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
GravityAudioProcessorEditor::GravityAudioProcessorEditor (GravityAudioProcessor& p)
: AudioProcessorEditor (&p), audioProcessor (p)
{

    using namespace juce;
      

    /*
    
        1. Creo los componentes visuales con los valores por defecto y los ubico en su sitio
     
    */
    // se hacen visibles los parámetros del vector getComps
    for( auto* comp : getComps() )
    {
        addAndMakeVisible(comp);
    }
    

    for( auto* comp : getCompsLF() )
    {
        comp->setLookAndFeel(&ginLookAndFeel);
    }
    
    for( auto* comp : getCompsOtherLF() )
    {
        comp->setLookAndFeel(&otherLookAndFeel);
    }
    
    getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::white);
    getLookAndFeel().setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::transparentBlack);
    getLookAndFeel().setColour(juce::ToggleButton::tickColourId, juce::Colours::black);
    
    // OPERATION MODE
    operationMode.addItem("Stereo", 1);
    operationMode.addItem("Dual Mono", 2);
    operationMode.addItem("Mid/Side", 3);
    operationMode.addItem("Serial Mono", 4);
    operationMode.addItem("Parallel Mono", 5);
    operationMode.setSelectedId(1);
    
    // VUMETER
    vuMode.addItem("Clean", 1);
    vuMode.addItem("Only Peak", 2);
    vuMode.addItem("Fixed Peak", 3);
    vuMode.addItem("Memory", 4);
    vuMode.addItem("Memory and Peak", 5);
    vuMode.setSelectedId(1);
    
    // FAST PRESET
    leftFastPreset.addItem("A", 1);
    leftFastPreset.addItem("B", 2);
    leftFastPreset.addItem("C", 3);
    leftFastPreset.setTextWhenNothingSelected("Preset");
    rightFastPreset.addItem("A", 1);
    rightFastPreset.addItem("B", 2);
    rightFastPreset.addItem("C", 3);
    rightFastPreset.setTextWhenNothingSelected("Preset");
    
    
    addAndMakeVisible(leftLpfFreqSlider);
    addAndMakeVisible(rightLpfFreqSlider);
    addAndMakeVisible(leftHpfFreqSlider);
    addAndMakeVisible(rightHpfFreqSlider);
    addAndMakeVisible(horizontalMeterL);
    addAndMakeVisible(horizontalMeterR);
    

    
    
    // 2. Cargo los valores desde el Plugin Processor de cuando el plugin se apagó
    AudioStateGravity myState = audioProcessor.getState();
    
    // 3. Asigno los valores que me han llegado desde el AudioProcessor
    /*
        BOTONES GENERALES
     */
    operationMode.setSelectedId(myState.operation_mode + 1);        // Operation Mode
    vuMode.setSelectedId(myState.vumeter_mode +1);                   // Channel Selector
    linkChannels.setToggleState(myState.link_channels, juce::dontSendNotification);
    linkAbsolute.setToggleState(myState.link_absolute, juce::dontSendNotification);
    
    /*
        PARAMETROS DEL COMPRESOR
     */
    
    leftThresholdSlider.setValue(myState.channel[LEFT_CHANNEL].threshold_value);
    leftThresholdRes.setSelectedId(myState.channel[LEFT_CHANNEL].threshold_resolution +1);
    leftThresholdBypass.setToggleState(myState.channel[LEFT_CHANNEL].threshold_muted, juce::dontSendNotification);
    rightThresholdSlider.setValue(myState.channel[RIGHT_CHANNEL].threshold_value);
    rightThresholdRes.setSelectedId(myState.channel[RIGHT_CHANNEL].threshold_resolution +1);
    rightThresholdBypass.setToggleState(myState.channel[RIGHT_CHANNEL].threshold_muted, juce::dontSendNotification);
    
    leftAttackSlider.setValue(myState.channel[LEFT_CHANNEL].attack);
    rightAttackSlider.setValue(myState.channel[RIGHT_CHANNEL].attack);
    
    leftReleaseSlider.setValue(myState.channel[LEFT_CHANNEL].release);
    rightReleaseSlider.setValue(myState.channel[RIGHT_CHANNEL].release);
    
    leftRatioSlider.setValue(myState.channel[LEFT_CHANNEL].ratio);
    rightRatioSlider.setValue(myState.channel[RIGHT_CHANNEL].ratio);
    
    leftKneeSlider.setValue(myState.channel[LEFT_CHANNEL].knee);
    rightKneeSlider.setValue(myState.channel[RIGHT_CHANNEL].knee);
    
    leftMakeUpSlider.setValue(myState.channel[LEFT_CHANNEL].makeup_value);
    leftMakeUpRes.setSelectedId(myState.channel[LEFT_CHANNEL].makeup_resolution +1);
    leftMakeUpBypass.setToggleState(myState.channel[LEFT_CHANNEL].makeup_muted, juce::dontSendNotification);
    rightMakeUpSlider.setValue(myState.channel[RIGHT_CHANNEL].makeup_value);
    rightMakeUpRes.setSelectedId(myState.channel[RIGHT_CHANNEL].makeup_resolution +1);
    rightMakeUpBypass.setToggleState(myState.channel[RIGHT_CHANNEL].makeup_muted, juce::dontSendNotification);
    
    leftBlendSlider.setValue(myState.channel[LEFT_CHANNEL].blend);
    leftBlendRes.setSelectedId(myState.channel[LEFT_CHANNEL].blend_resolution +1);
    leftBlendBypass.setToggleState(myState.channel[LEFT_CHANNEL].blend_muted, juce::dontSendNotification);
    rightBlendSlider.setValue(myState.channel[RIGHT_CHANNEL].blend);
    rightBlendRes.setSelectedId(myState.channel[RIGHT_CHANNEL].blend_resolution +1);
    rightBlendBypass.setToggleState(myState.channel[RIGHT_CHANNEL].blend_muted, juce::dontSendNotification);
    
    /*
        FILTROS
     */
    
    //BOTONES ON/OFF
    leftLpfscButton.setToggleState(myState.channel[LEFT_CHANNEL].lpf, juce::dontSendNotification);
    rightLpfscButton.setToggleState(myState.channel[RIGHT_CHANNEL].lpf, juce::dontSendNotification);
    leftHpfscButton.setToggleState(myState.channel[LEFT_CHANNEL].hpf, juce::dontSendNotification);
    rightHpfscButton.setToggleState(myState.channel[RIGHT_CHANNEL].hpf, juce::dontSendNotification);
    leftNotchButton.setToggleState(myState.channel[LEFT_CHANNEL].notch, juce::dontSendNotification);
    rightNotchButton.setToggleState(myState.channel[RIGHT_CHANNEL].notch, juce::dontSendNotification);
    
    //SLIDERS FRECUENCIA
    leftLpfFreqSlider.setValue(myState.channel[LEFT_CHANNEL].lpf_f);
    rightLpfFreqSlider.setValue(myState.channel[RIGHT_CHANNEL].lpf_f);
    leftHpfFreqSlider.setValue(myState.channel[LEFT_CHANNEL].hpf_f);
    rightHpfFreqSlider.setValue(myState.channel[RIGHT_CHANNEL].hpf_f);
    leftNotchFreqSlider.setValue(myState.channel[LEFT_CHANNEL].notch_f);
    rightNotchFreqSlider.setValue(myState.channel[RIGHT_CHANNEL].notch_f);
    leftNotchQSlider.setValue(myState.channel[LEFT_CHANNEL].notch_q);
    rightNotchQSlider.setValue(myState.channel[RIGHT_CHANNEL].notch_q);
    
    
    
    
    /*
        4. Métodos de la interfaz (clicks y changes)
    */
    /*
            GENERALES
     */
    //OPERATION MODE
    operationMode.onChange=[this]{
        if(operationMode.getSelectedId()==2){
            setSize(1000, 800);
        }else if(operationMode.getSelectedId()!=2){
            setSize(1000, 400);
        }
        audioProcessor.setOperationMode(operationMode.getSelectedItemIndex(), true);
    };
    
    //VUMETER MODE
    vuMode.onChange=[this]{
        audioProcessor.setVUmeterMode(vuMode.getSelectedItemIndex(), true);
    };
    
    //LINK CHANNELS
    linkChannels.onClick = [this]{
        audioProcessor.setLinkChanels(linkChannels.getToggleState(), true);
    };
    
    
    //ABSOLUTE OR RELATIVE
    
    linkAbsolute.onClick = [this]
    {
        if(!linkChannels.getToggleState()){
                linkAbsolute.setToggleState(!linkAbsolute.getToggleState(), juce::dontSendNotification);
            audioProcessor.setLinkAbsolute(linkAbsolute.getToggleState(), true);
            linkAbsolute.setToggleState(false, juce::dontSendNotification);


        }
    };



    
    // LEFT FAST PRESETS
    leftFastPreset.onChange = [this]{
        audioProcessor.setFastPreset(0, LEFT_CHANNEL, leftFastPreset.getSelectedItemIndex(), true);
    };

    // RIGHT FAST PRESETS
    rightFastPreset.onChange = [this]{
        audioProcessor.setFastPreset(0, RIGHT_CHANNEL, rightFastPreset.getSelectedItemIndex(), true);
    };
    
    
    //GENERAL BYPASS LEFT
    leftBypass.onClick = [this]{
        audioProcessor.setGeneralBypass(LEFT_CHANNEL, leftBypass.getToggleState(), true);
    };

    //GENERAL BYPASS RIGHT
    rightBypass.onClick = [this]{
        audioProcessor.setGeneralBypass(RIGHT_CHANNEL, rightBypass.getToggleState(), true);
    };
    
    //MUTE LEFT
    leftMute.onClick = [this]{
        audioProcessor.setGeneralMute(LEFT_CHANNEL, leftMute.getToggleState(), true);
    };
    
    //MUTE RIGHT
    rightMute.onClick = [this]{
        audioProcessor.setGeneralMute(RIGHT_CHANNEL, rightMute.getToggleState(), true);
    };
    
    /*
                PARÁMETROS
     */
    
    //========================================//
    /*
        LEFT THRESHOLD
     */
    //Cuando cambia el valor del slider
    leftThresholdSlider.onValueChange = [this]{
        if (manualChange)
            return;
        audioProcessor.setKnee(LEFT_CHANNEL, leftThresholdSlider.getValue(), true);
    };


    for (int i = 0; i < MAX_THRESHOLD_RESOLUTIONS; ++i)
    {
        leftThresholdRes.addItem(String(threshold_resolutions_values[i]), i + 1);
    }
    //Cuando cambia el valor de la resolución
    leftThresholdRes.onChange = [this]{
        leftThresholdSlider.setRange(juce::Range<double>(MIN_THRESHOLD, MAX_THRESHOLD), threshold_resolutions_values[leftThresholdRes.getSelectedItemIndex()]);
        this->repaint();
        audioProcessor.setThresholdResolution(LEFT_CHANNEL, leftThresholdRes.getSelectedItemIndex(), true);
    };
    
    //SE EJECUTA CUANDO CAMBIA EL VALOR DEL BYPASS (SE BLOQUEA)
    leftThresholdBypass.onStateChange = [this]{
        audioProcessor.setThresholdBypass(LEFT_CHANNEL, leftThresholdBypass.getToggleState(), true);

        if(leftThresholdBypass.getToggleState())
            leftThresholdSlider.setEnabled(false); // Esto bloquea el slider
        else{
            leftThresholdSlider.setEnabled(true); // Esto bloquea el slider
            leftThresholdSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::black);
        }
    };

    
    /*
        RIGHT THRESHOLD
     */
    
    rightThresholdSlider.onValueChange = [this]{
        if (manualChange)
            return;
        audioProcessor.setKnee(RIGHT_CHANNEL, rightThresholdSlider.getValue(), true);
    };

    for (int i = 0; i < MAX_THRESHOLD_RESOLUTIONS; ++i)
    {
        rightThresholdRes.addItem(String(threshold_resolutions_values[i]), i + 1);
    }
    
    rightThresholdRes.onChange = [this]{
        rightThresholdSlider.setRange(juce::Range<double>(MIN_THRESHOLD, MAX_THRESHOLD), threshold_resolutions_values[rightThresholdRes.getSelectedItemIndex()]);
        this->repaint();
        audioProcessor.setThresholdResolution(RIGHT_CHANNEL, rightThresholdRes.getSelectedItemIndex(), true);

    };
    
    //Cuando cambia de valor el bypass
    rightThresholdBypass.onStateChange = [this]{
        audioProcessor.setThresholdBypass(RIGHT_CHANNEL, rightThresholdBypass.getToggleState(), true);
        
        if(rightThresholdBypass.getToggleState()){
            rightThresholdSlider.setEnabled(false); // Esto bloquea el slider
        }
        else{
            rightThresholdSlider.setEnabled(true); // Esto bloquea el slider
            rightThresholdSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::black);
        }
    };

    
    /*
        LEFT ATTACK
     */
    
    leftAttackSlider.onValueChange = [this]{
        if (manualChange)
            return;
        audioProcessor.setAttack(LEFT_CHANNEL, leftAttackSlider.getValue(), true);
    };

    /*
        RIGHT ATTACK
     */
    
    rightAttackSlider.onValueChange = [this]{
        if (manualChange)
            return;
        audioProcessor.setAttack(RIGHT_CHANNEL, rightAttackSlider.getValue(), true);
    };
    
    /*
        LEFT RELEASE
     */
    
    leftReleaseSlider.onValueChange = [this]{
        if (manualChange)
            return;
        audioProcessor.setRelease(LEFT_CHANNEL, leftReleaseSlider.getValue(), true);
    };

    /*
        RIGHT RELEASE
     */
    
    rightReleaseSlider.onValueChange = [this]{
        if (manualChange)
            return;
        audioProcessor.setRelease(RIGHT_CHANNEL, rightReleaseSlider.getValue(), true);
    };

    /*
        LEFT KNEE
     */
    
    leftKneeSlider.onValueChange = [this]{
        if (manualChange)
            return;
        audioProcessor.setKnee(LEFT_CHANNEL, leftKneeSlider.getValue(), true);
    };

    /*
        RIGHT KNEE
     */
    
    rightKneeSlider.onValueChange = [this]{
        if (manualChange)
            return;
        audioProcessor.setKnee(RIGHT_CHANNEL, rightKneeSlider.getValue(), true);
    };
    
    /*
        LEFT MAKE UP
    */
    
    
    for (int i = 0; i < MAX_MAKEUP_RESOLUTIONS; ++i)
    {
        leftMakeUpRes.addItem(String(makeup_resolutions_values[i]), i + 1); // El segundo argumento es el ID de la opción
    }
    //Cuando cambia el valor de la resolucion
    leftMakeUpRes.onChange = [this]{
        leftMakeUpSlider.setRange(juce::Range<double>(MIN_MAKEUP, MAX_MAKEUP), makeup_resolutions_values[leftMakeUpRes.getSelectedItemIndex()]);
        this->repaint();
        
        audioProcessor.setMakeUpResolution(LEFT_CHANNEL, leftMakeUpRes.getSelectedItemIndex(), true);
    };
    
    leftMakeUpBypass.onStateChange = [this]{
        audioProcessor.setMakeUpBypass(LEFT_CHANNEL, leftMakeUpBypass.getToggleState(), true);
        
        if(leftMakeUpBypass.getToggleState()){
            leftMakeUpSlider.setEnabled(false); // Esto bloquea el slider
        }
        else{
            leftMakeUpSlider.setEnabled(true); // Esto bloquea el slider
            leftMakeUpSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::black);
        }
    };

    
    /*
        RIGHT MAKE UP
     */
    
    //Cuando cambia el valor del slider MAKE UP
    rightMakeUpSlider.onValueChange = [this]{
        if (manualChange)
            return;
        audioProcessor.setKnee(RIGHT_CHANNEL, rightMakeUpSlider.getValue(), true);
    };
    
    for (int i = 0; i < MAX_MAKEUP_RESOLUTIONS; ++i)
    {
        rightMakeUpRes.addItem(String(makeup_resolutions_values[i]), i + 1); // El segundo argumento es el ID de la opción
    }
    rightMakeUpRes.onChange = [this]{
        rightMakeUpSlider.setRange(juce::Range<double>(MIN_MAKEUP, MAX_MAKEUP), makeup_resolutions_values[rightMakeUpRes.getSelectedItemIndex()]);
        this->repaint();
        audioProcessor.setMakeUpResolution(RIGHT_CHANNEL, rightMakeUpRes.getSelectedItemIndex(), true);
    };
    //Cuando cambia de valor el bypass
    rightMakeUpBypass.onClick = [this]{
        audioProcessor.setMakeUpBypass(RIGHT_CHANNEL, rightMakeUpBypass.getToggleState(), true);
        
        if(rightMakeUpBypass.getToggleState()){
            rightMakeUpSlider.setEnabled(false); // Esto bloquea el slider
        }
        else{
            rightMakeUpSlider.setEnabled(true); // Esto bloquea el slider
            rightMakeUpSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::black);
        }
    };
    

    
    /*
        LEFT BLEND
     */

    leftBlendRes.setTextWhenNothingSelected("Resolution");
    for (int i = 0; i < MAX_BLEND_RESOLUTIONS; ++i)
    {
        leftBlendRes.addItem(String(blend_resolutions_list[i]), i + 1); // El segundo argumento es el ID de la opción
    }
    leftBlendRes.onChange = [this]{
        leftBlendSlider.setRange(juce::Range<double>(MIN_BLEND, MAX_BLEND), blend_resolutions_list[leftBlendRes.getSelectedItemIndex()]);
        this->repaint();
        audioProcessor.setBlendResolution(LEFT_CHANNEL, leftBlendRes.getSelectedItemIndex(), true);

    };
    
    leftBlendBypass.onStateChange = [this]{
        audioProcessor.setBlendBypass(LEFT_CHANNEL, leftBlendBypass.getToggleState(), true);
        
        if(leftBlendBypass.getToggleState()){
            leftBlendSlider.setEnabled(false); // Esto bloquea el slider
        }
        else{
            leftBlendSlider.setEnabled(true); // Esto bloquea el slider
            leftBlendSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::black);
        }
    };
    

    
    /*
        RIGHT BLEND
     */

    rightBlendRes.setTextWhenNothingSelected("Resolution");
    for (int i = 0; i < MAX_BLEND_RESOLUTIONS; ++i)
    {
        rightBlendRes.addItem(String(blend_resolutions_list[i]), i + 1); // El segundo argumento es el ID de la opción
    }
    rightBlendRes.onChange = [this]{
        rightBlendSlider.setRange(juce::Range<double>(MIN_BLEND, MAX_BLEND), blend_resolutions_list[rightBlendRes.getSelectedItemIndex()]);
        this->repaint();
        audioProcessor.setBlendResolution(RIGHT_CHANNEL, rightBlendRes.getSelectedItemIndex(), true);

        
    };
    
    rightBlendBypass.onStateChange = [this]{
        audioProcessor.setBlendBypass(RIGHT_CHANNEL, rightBlendBypass.getToggleState(), true);
        
        if(rightBlendBypass.getToggleState()){
            rightBlendSlider.setEnabled(false); // Esto bloquea el slider
        }
        else{
            rightBlendSlider.setEnabled(true); // Esto bloquea el slider
            rightBlendSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::black);
        }
    };
    

    
    //===========================================//

    /*
            FILTROS
     */
    
    // CONMUTACION DE BOTONES NOTCH LPF HPF LEFT
    /*
            LEFT
     */
    
    leftNotchButton.onClick = [&, this]
    {
        if (manualChange)
            return;
        
        // Comprobar que los estados de cada botón son correctos
        if (leftNotchButton.getToggleState())
        {
            manualChange = true;
            leftLpfscButton.setToggleState(false, juce::dontSendNotification);
            leftHpfscButton.setToggleState(false, juce::dontSendNotification);
            manualChange = false;
        }
    
        // Comunico los valores correctos
        audioProcessor.setNotch(LEFT_CHANNEL, leftNotchButton.getToggleState(), false);
        audioProcessor.setLPF(LEFT_CHANNEL, leftLpfscButton.getToggleState(), false);
        audioProcessor.setHPF(LEFT_CHANNEL, leftHpfscButton.getToggleState(), false);
        
        audioProcessor.syncParameters();                    //se envían los parámetros al processor.
    };

    
    leftLpfscButton.onClick = [&, this]
    {
        if (manualChange)
            return;
        
        // Comprobar que los estados de cada botón son correctos
        if (leftLpfscButton.getToggleState())
        {
            manualChange = true;
            leftNotchButton.setToggleState(false, juce::dontSendNotification);
            manualChange = false;
        }
    
        // Comunico los valores correctos
        audioProcessor.setNotch(LEFT_CHANNEL, leftNotchButton.getToggleState(), false);
        audioProcessor.setLPF(LEFT_CHANNEL, leftLpfscButton.getToggleState(), false);
        audioProcessor.setHPF(LEFT_CHANNEL, leftHpfscButton.getToggleState(), false);
        
        audioProcessor.syncParameters();                    //se envían los parámetros al processor.
    };
    
    leftHpfscButton.onClick = [&, this]
    {
        if (manualChange)
            return;
        
        // Comprobar que los estados de cada botón LPF/HPF son OK
        if (leftHpfscButton.getToggleState())
        {
            manualChange = true;
            leftNotchButton.setToggleState(false, juce::dontSendNotification);
            manualChange = false;
        }
    
        // Comunico los valores correctos
        audioProcessor.setNotch(LEFT_CHANNEL, leftNotchButton.getToggleState(), false);
        audioProcessor.setLPF(LEFT_CHANNEL, leftLpfscButton.getToggleState(), false);
        audioProcessor.setHPF(LEFT_CHANNEL, leftHpfscButton.getToggleState(), false);
        
        audioProcessor.syncParameters();                    //se envían los parámetros al processor.
    };
    
    //CONMUTACION DE BOTONES NOTCH LPF HPF RIGHT//
    
    /*
            RIGHT
     */

    rightNotchButton.onClick = [&, this]
    {
        if (manualChange)
            return;
        
        // Comprobar que los estados de cada botón son correctos
        if (rightNotchButton.getToggleState())
        {
            manualChange = true;
            rightLpfscButton.setToggleState(false, juce::dontSendNotification);
            rightHpfscButton.setToggleState(false, juce::dontSendNotification);
            manualChange = false;
        }
    
        // Comunico los valores correctos
        audioProcessor.setNotch(RIGHT_CHANNEL, rightNotchButton.getToggleState(), false);
        audioProcessor.setLPF(RIGHT_CHANNEL, rightLpfscButton.getToggleState(), false);
        audioProcessor.setHPF(RIGHT_CHANNEL, rightHpfscButton.getToggleState(), false);
        
        audioProcessor.syncParameters();                    //se envían los parámetros al processor.
    };

    
    rightLpfscButton.onClick = [&, this]
    {
        if (manualChange)
            return;
        
        // Comprobar que los estados de cada botón son correctos
        if (rightLpfscButton.getToggleState())
        {
            manualChange = true;
            rightNotchButton.setToggleState(false, juce::dontSendNotification);
            manualChange = false;
        }
    
        // Comunico los valores correctos
        audioProcessor.setNotch(RIGHT_CHANNEL, rightNotchButton.getToggleState(), false);
        audioProcessor.setLPF(RIGHT_CHANNEL, rightLpfscButton.getToggleState(), false);
        audioProcessor.setHPF(RIGHT_CHANNEL, rightHpfscButton.getToggleState(), false);
        
        audioProcessor.syncParameters();                    //se envían los parámetros al processor.
    };
    
    rightHpfscButton.onClick = [&, this]
    {
        if (manualChange)
            return;
        
        // Comprobar que los estados de cada botón LPF/HPF son OK
        if (rightHpfscButton.getToggleState())
        {
            manualChange = true;
            rightNotchButton.setToggleState(false, juce::dontSendNotification);
            manualChange = false;
        }
    
        // Comunico los valores correctos
        audioProcessor.setNotch(RIGHT_CHANNEL, rightNotchButton.getToggleState(), false);
        audioProcessor.setLPF(RIGHT_CHANNEL, rightLpfscButton.getToggleState(), false);
        audioProcessor.setHPF(RIGHT_CHANNEL, rightHpfscButton.getToggleState(), false);
        
        audioProcessor.syncParameters();                    //se envían los parámetros al processor.
    };
    
    
    //============================================//
    /*
        RANGOS DE SLIDERS
     */

    leftAttackSlider.setRange(juce::Range<double>(0,7), 1);
    rightAttackSlider.setRange(juce::Range<double>(0,7), 1);
    leftReleaseSlider.setRange(juce::Range<double>(0,7), 1);
    rightReleaseSlider.setRange(juce::Range<double>(0,7), 1);
    leftRatioSlider.setRange(juce::Range<double>(0,7), 1);
    rightRatioSlider.setRange(juce::Range<double>(0,7), 1);
    leftKneeSlider.setRange(juce::Range<double>(0,2), 1);
    rightKneeSlider.setRange(juce::Range<double>(0,2), 1);
    //======================================================//
    
    /*
        ETIQUETAS Y NOMBRES DE LOS PARÁMETROS
     */
    leftLpfscButton.setButtonText("LPF");
    leftHpfscButton.setButtonText("HPF");
    leftNotchButton.setButtonText("NOTCH");
    rightLpfscButton.setButtonText("LPF");
    rightHpfscButton.setButtonText("HPF");
    rightNotchButton.setButtonText("NOTCH");
    leftHpfFreqSlider.setRange(1000, 20000, 0.1);
    leftHpfFreqSlider.setNumDecimalPlacesToDisplay(1);
    leftLpfFreqSlider.setRange(20, 1000, 0.1);
    leftLpfFreqSlider.setNumDecimalPlacesToDisplay(1);
    leftNotchFreqSlider.setRange(20, 20000, 0.1);
    leftNotchFreqSlider.setNumDecimalPlacesToDisplay(1);
    leftNotchQSlider.setRange(0, 20, 0.1);
    rightHpfFreqSlider.setRange(1000, 20000, 0.1);
    rightHpfFreqSlider.setNumDecimalPlacesToDisplay(1);
    rightLpfFreqSlider.setRange(20, 1000, 0.1);
    rightLpfFreqSlider.setNumDecimalPlacesToDisplay(1);
    rightNotchFreqSlider.setRange(20, 20000, 0.1);
    rightNotchFreqSlider.setNumDecimalPlacesToDisplay(1);
    rightNotchQSlider.setRange(0, 20, 0.1);
    
    leftThresholdRes.setSelectedId(1);
    leftThresholdBypass.setButtonText("BYPASS");
    
    rightThresholdRes.setSelectedId(1);
    rightThresholdBypass.setButtonText("BYPASS");
    
    leftMakeUpBypass.setButtonText("BYPASS");
    leftMakeUpRes.setSelectedId(1);
    
    rightMakeUpBypass.setButtonText("BYPASS");
    rightMakeUpRes.setSelectedId(1);
    
    leftBlendBypass.setButtonText("BYPASS");
    leftBlendRes.setSelectedId(1);
    
    rightBlendBypass.setButtonText("BYPASS");
    rightBlendRes.setSelectedId(1);

    //TAMAÑO DE LA VENTANA POR DEFECTO
    setSize (1000, 400);
    

}
    

    GravityAudioProcessorEditor::~GravityAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
        leftThresholdSlider.setLookAndFeel(nullptr);
    for( auto* comp : getCompsLF() )
    {
        comp->setLookAndFeel(nullptr);
    }
    
    for( auto* comp : getCompsOtherLF() )
    {
        comp->setLookAndFeel(nullptr);
    }
}

//==============================================================================
void GravityAudioProcessorEditor::paint (juce::Graphics& g)
{
    
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (juce::Colours::black);
    
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
//    g.fillAll(juce::Colours::black);
    
    
    
                //COLORES DE FONDO//
    juce::Rectangle<float> topSection(0, 0, getWidth(), 150);
    juce::Rectangle<float> middleTopSection(0, 150, getWidth(), 150);
    juce::Rectangle<float> middleSection(0, 300, getWidth(), 150); // Nueva sección
    juce::Rectangle<float> middleBottomSection(0, 450, getWidth(), 150);
    juce::Rectangle<float> bottomSection(0, 600, getWidth(), getHeight() - 600);

        // Define los colores para los gradientes
    juce::Colour topColor1 = juce::Colours::black.withAlpha(0.7f);
    juce::Colour topColor2 = juce::Colours::salmon.withAlpha(0.7f);

    juce::Colour middleTopColor1 = juce::Colours::salmon.withAlpha(0.7f);
    juce::Colour middleTopColor2 = juce::Colours::orange.withAlpha(0.7f);
    
    juce::Colour middleColor1 = juce::Colours::orange.withAlpha(0.7f);
    juce::Colour middleColor2 = juce::Colours::salmon.withAlpha(0.7f);

    juce::Colour middleBottomColor1 = juce::Colours::salmon.withAlpha(0.7f);
    juce::Colour middleBottomColor2 = juce::Colours::orange.withAlpha(0.7f);

    juce::Colour bottomColor1 = juce::Colours::orange.withAlpha(0.7f);
    juce::Colour bottomColor2 = juce::Colours::salmon.withAlpha(0.7f);

        // Crea y pinta los gradientes
    g.setGradientFill(juce::ColourGradient(topColor1, 0, 0, topColor2, 0, topSection.getHeight(), false));
    g.fillRect(topSection);

    g.setGradientFill(juce::ColourGradient(middleTopColor1, 0, topSection.getBottom(), middleTopColor2, 0, middleTopSection.getBottom(), false));
    g.fillRect(middleTopSection);
    
    g.setGradientFill(juce::ColourGradient(middleColor1, 0, middleTopSection.getBottom(), middleColor2, 0, middleSection.getBottom(), false));
    g.fillRect(middleSection);

    g.setGradientFill(juce::ColourGradient(middleBottomColor1, 0, middleSection.getBottom(), middleBottomColor2, 0, middleBottomSection.getBottom(), false));
    g.fillRect(middleBottomSection);

    g.setGradientFill(juce::ColourGradient(bottomColor1, 0, middleBottomSection.getBottom(), bottomColor2, 0, bottomSection.getBottom(), false));
    g.fillRect(bottomSection);

    //=================================//
    g.setColour(juce::Colours::white); // Establece el color de la línea
    g.drawLine(0, 401, 1000, 401, 2.0f);
    //=================================//Titulos de botones
    g.setColour (juce::Colours::yellow);
    g.setFont (30.0f);
    g.drawFittedText ("GRAVITY", getLocalBounds(), juce::Justification::topLeft, 1);
    g.setColour (juce::Colours::whitesmoke);
    g.setFont (20.0f);
    g.drawFittedText ("VUMETER", getLocalBounds(), juce::Justification::centredTop, 1);
    
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    
    g.drawText("LINK CHANNELS", 710, 6, 120, 80, juce::Justification::centred);
    g.drawText("ABSOLUTE", 830, 6, 120, 80, juce::Justification::centred);
    
    g.drawText("GENERAL BYPASS", 777, 43, 120, 80, juce::Justification::centred);
    g.drawText("MUTE", 774, 85, 120, 80, juce::Justification::centred);
    g.drawText("GENERAL BYPASS", 777, 680, 120, 80, juce::Justification::centred);
    g.drawText("MUTE", 774, 720, 120, 80, juce::Justification::centred);
    //==============================//
    juce::String texto = "THRESHOLD";
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(16.0f));
    int x = leftThresholdSlider.getX() + leftThresholdSlider.getWidth() / 2;
    int y = leftThresholdSlider.getY() - 20;
    g.drawText(texto, x - 55, y, leftThresholdSlider.getWidth(), 20, juce::Justification::centred, true);
    //g.drawText("BYPASS", x-33, y + 155, thresholdBypass.getWidth(), 20, juce::Justification::centred, true);
    //==============================//
    juce::String texto2 = "ATTACK";
    int x2 = leftAttackSlider.getX() + leftAttackSlider.getWidth() / 2;
    int y2 = leftAttackSlider.getY() - 20;
    g.drawText(texto2, x2-55, y2, leftAttackSlider.getWidth(), 20, juce::Justification::centred, true);
    //==============================//
    juce::String texto3 = "RELEASE";
    int x3 = leftReleaseSlider.getX() + leftReleaseSlider.getWidth() / 2;
    int y3 = leftReleaseSlider.getY() - 20;
    g.drawText(texto3, x3-55, y3, leftReleaseSlider.getWidth(), 20, juce::Justification::centred, true);
    //==============================//
    juce::String texto4 = "RATIO";
    g.drawText(texto4, (leftRatioSlider.getX() + leftRatioSlider.getWidth() / 2) -55, leftRatioSlider.getY() - 20, leftRatioSlider.getWidth(), 20, juce::Justification::centred, true);
    //==============================//
    g.drawText("KNEE", (leftKneeSlider.getX() + leftKneeSlider.getWidth() / 2) -55, leftKneeSlider.getY() - 20, leftKneeSlider.getWidth(), 20, juce::Justification::centred, true);
    //==============================//
    g.drawText("MAKE UP", (leftMakeUpSlider.getX() + leftMakeUpSlider.getWidth() / 2) -55, leftMakeUpSlider.getY() - 20, leftMakeUpSlider.getWidth(), 20, juce::Justification::centred, true);
    //==============================//
    g.drawText("BLEND", (leftBlendSlider.getX() + leftBlendSlider.getWidth() / 2) -55, leftBlendSlider.getY() - 20, leftBlendSlider.getWidth(), 20, juce::Justification::centred, true);
   // g.drawText("BYPASS", x+644, y + 155, thresholdBypass.getWidth(), 20, juce::Justification::centred, true);
    //==============================//
    g.drawText("THRESHOLD", (rightThresholdSlider.getX() + rightThresholdSlider.getWidth() / 2) -55, rightThresholdSlider.getY() - 20, rightThresholdSlider.getWidth(), 20, juce::Justification::centred, true);
    g.drawText("ATTACK", (rightAttackSlider.getX() + rightAttackSlider.getWidth() / 2) -55, rightAttackSlider.getY() - 20, rightAttackSlider.getWidth(), 20, juce::Justification::centred, true);
    g.drawText("RELEASE", (rightReleaseSlider.getX() + rightReleaseSlider.getWidth() / 2) -55, rightReleaseSlider.getY() - 20, rightReleaseSlider.getWidth(), 20, juce::Justification::centred, true);
    g.drawText("RATIO", (rightRatioSlider.getX() + rightRatioSlider.getWidth() / 2) -55, rightRatioSlider.getY() - 20, rightRatioSlider.getWidth(), 20, juce::Justification::centred, true);
    g.drawText("KNEE", (rightKneeSlider.getX() + rightKneeSlider.getWidth() / 2) -55, rightKneeSlider.getY() - 20, rightKneeSlider.getWidth(), 20, juce::Justification::centred, true);
    g.drawText("MAKE UP", (rightMakeUpSlider.getX() + rightMakeUpSlider.getWidth() / 2) -55, rightMakeUpSlider.getY() - 20, rightMakeUpSlider.getWidth(), 20, juce::Justification::centred, true);
    g.drawText("BLEND", (rightBlendSlider.getX() + rightBlendSlider.getWidth() / 2) -55, rightBlendSlider.getY() - 20, rightBlendSlider.getWidth(), 20, juce::Justification::centred, true);
    
    g.setColour (juce::Colours::yellow);
    g.setFont (30.0f);
    
    if(operationMode.getSelectedId()==2){//Se amplia la ventana en caso que se elija el modo Dual Mono
        g.drawText("L", 100, 40, 120, 80, juce::Justification::centred);
        g.drawText("R", 100, 380, 120, 80, juce::Justification::centred);

    }
    


}

void GravityAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    auto bounds = getLocalBounds();
    imageComponent.setBounds(bounds);
    if(operationMode.getSelectedId()==2){
        

        //std::cout << "state: " << leftHpfscButton.getToggleState() << std::endl;

        horizontalMeterL.setBounds(460, 30, 200, 20);
        horizontalMeterR.setBounds(460, 60, 200, 20);

        auto responseArea = bounds.removeFromTop(bounds.getHeight() * 0.225);
        auto normalBounds = bounds.removeFromTop(bounds.getHeight()*0.5);
        auto normalBounds2 = bounds.removeFromTop(bounds.getHeight()*0.5);
        

        auto totalWidth = normalBounds.getWidth();
        auto areaWidth = totalWidth / 9.0;
        auto totalHeight = normalBounds.getHeight();
        auto areaHeight = totalHeight / 2.0;
        

        
        auto thresArea = normalBounds.removeFromLeft(areaWidth);
        auto thresArea2 = normalBounds2.removeFromLeft(areaWidth);
        auto attackArea = normalBounds.removeFromLeft(areaWidth);
        auto attackArea2 = normalBounds2.removeFromLeft(areaWidth);
        auto releaseArea = normalBounds.removeFromLeft(areaWidth);
        auto releaseArea2 = normalBounds2.removeFromLeft(areaWidth);
        auto ratioArea = normalBounds.removeFromLeft(areaWidth);
        auto ratioArea2 = normalBounds2.removeFromLeft(areaWidth);
        auto kneeArea = normalBounds.removeFromLeft(areaWidth);
        auto kneeArea2 = normalBounds2.removeFromLeft(areaWidth);
        auto makeUpArea = normalBounds.removeFromLeft(areaWidth);
        auto makeUpArea2 = normalBounds2.removeFromLeft(areaWidth);
        auto blendArea = normalBounds.removeFromLeft(areaWidth);
        auto blendArea2 = normalBounds2.removeFromLeft(areaWidth);
        auto filtersArea = normalBounds.removeFromLeft(areaWidth);
        auto filtersArea2 = normalBounds2.removeFromLeft(areaWidth);


        leftThresholdSlider.setBounds(thresArea.removeFromTop(thresArea.getHeight() * 0.5));
        rightThresholdSlider.setBounds(thresArea2.removeFromTop(thresArea2.getHeight()));
        leftAttackSlider.setBounds(attackArea.removeFromTop(attackArea.getHeight() * 0.5));
        rightAttackSlider.setBounds(attackArea2.removeFromTop(attackArea2.getHeight()));
        leftReleaseSlider.setBounds(releaseArea.removeFromTop(releaseArea.getHeight() * 0.5));
        rightReleaseSlider.setBounds(releaseArea2.removeFromTop(releaseArea2.getHeight()));
        leftRatioSlider.setBounds(ratioArea.removeFromTop(ratioArea.getHeight() * 0.5));
        rightRatioSlider.setBounds(ratioArea2.removeFromTop(ratioArea2.getHeight()));
        leftKneeSlider.setBounds(kneeArea.removeFromTop(kneeArea.getHeight() * 0.5));
        rightKneeSlider.setBounds(kneeArea2.removeFromTop(kneeArea2.getHeight()));
        leftMakeUpSlider.setBounds(makeUpArea.removeFromTop(makeUpArea.getHeight() * 0.5));
        rightMakeUpSlider.setBounds(makeUpArea2.removeFromTop(makeUpArea2.getHeight()));
        leftBlendSlider.setBounds(blendArea.removeFromTop(blendArea.getHeight() * 0.5));
        rightBlendSlider.setBounds(blendArea2.removeFromTop(blendArea2.getHeight()));
        
        //BOTONES//
        
        int buttonHeight = filtersArea.getHeight() / 3;
        linkChannels.setBounds(822, 20, 50, 50);
        linkAbsolute.setBounds(922, 20, 50, 50);
        leftBypass.setBounds(822, 80, 50, 50);
        leftMute.setBounds(822, 120, 50, 50);
        rightBypass.setBounds(822, 715, 50, 50);
        rightMute.setBounds(822, 755, 50, 50);
        leftLpfscButton.setBounds(filtersArea.removeFromTop(80));
        leftHpfscButton.setBounds(filtersArea.removeFromTop(80));
        leftNotchButton.setBounds(filtersArea.removeFromTop(80));
        rightLpfscButton.setBounds(filtersArea2.removeFromTop(80));
        rightHpfscButton.setBounds(filtersArea2.removeFromTop(80));
        rightNotchButton.setBounds(rightHpfscButton.getX(), rightHpfscButton.getY()+75, leftNotchButton.getWidth(), leftNotchButton.getHeight());
        
        //COMBO BOX//
        
        leftThresholdRes.setBounds(thresArea.removeFromTop(thresArea.getHeight() * 0.2).withWidth(100));
        rightThresholdRes.setBounds(0, 630, 100, 30);
        leftMakeUpRes.setBounds(makeUpArea.removeFromTop(makeUpArea.getHeight() * 0.2).withWidth(100));
        rightMakeUpRes.setBounds(555, 630, 100, 30);
        leftBlendRes.setBounds(blendArea.removeFromTop(blendArea.getHeight() * 0.2).withWidth(100));
        rightBlendRes.setBounds(666, 630, 100, 30);
        operationMode.setBounds(0, 70, 130, 20);
        vuMode.setBounds(300, 70, 130, 20);
        leftFastPreset.setBounds(0, 110, 130, 20);
        rightFastPreset.setBounds(0, 420, 130, 20);

        
        leftThresholdBypass.setBounds(thresArea.removeFromTop(thresArea.getHeight() * 0.2).withWidth(100));
        rightThresholdBypass.setBounds(0, 657, 100, 30);
        leftMakeUpBypass.setBounds(makeUpArea.removeFromTop(makeUpArea.getHeight() * 0.2).withWidth(100));
        rightMakeUpBypass.setBounds(555, 657, 100, 30);
        leftBlendBypass.setBounds(blendArea.removeFromTop(blendArea.getHeight() * 0.2).withWidth(100));
        rightBlendBypass.setBounds(666, 657, 100, 30);
        
        leftLpfFreqSlider.setBounds(leftLpfscButton.getX() + 55, leftLpfscButton.getY() - 20, 80, 80);
        leftHpfFreqSlider.setBounds(leftHpfscButton.getX() +55, leftHpfscButton.getY() -20, 80, 80);
        leftNotchFreqSlider.setBounds(leftNotchButton.getX() +55, leftNotchButton.getY() -20, 80, 80);
        leftNotchQSlider.setBounds(leftNotchFreqSlider.getX() +80, leftNotchFreqSlider.getY() , 80, 80);
        rightLpfFreqSlider.setBounds(rightLpfscButton.getX() + 55, rightLpfscButton.getY() -20, 80, 80);
        rightHpfFreqSlider.setBounds(rightHpfscButton.getX() +55, rightHpfscButton.getY() - 20, 80, 80);
        rightNotchFreqSlider.setBounds(rightNotchButton.getX() +55, rightNotchButton.getY() -20, 80, 80);
        rightNotchQSlider.setBounds(rightNotchFreqSlider.getX() +80, rightNotchFreqSlider.getY() , 80, 80);
        

    }
    
    if(operationMode.getSelectedId()!=2){
        horizontalMeterL.setBounds(460, 30, 200, 20);
        horizontalMeterR.setBounds(460, 60, 200, 20);
        auto responseArea = bounds.removeFromTop(bounds.getHeight() * 0.45);
        auto totalWidth = bounds.getWidth();
        auto areaWidth = totalWidth / 9.0;
        //verticalDiscreteMeterL.setBounds(460, 20, 40, 100);
        //verticalDiscreteMeterR.setBounds(500, 20, 40, 100);

        
        
        auto thresArea = bounds.removeFromLeft(areaWidth);
        auto attackArea = bounds.removeFromLeft(areaWidth);
        auto releaseArea = bounds.removeFromLeft(areaWidth);
        auto ratioArea = bounds.removeFromLeft(areaWidth);
        auto kneeArea = bounds.removeFromLeft(areaWidth);
        auto makeUpArea = bounds.removeFromLeft(areaWidth);
        auto blendArea = bounds.removeFromLeft(areaWidth);
        auto filtersArea = bounds.removeFromLeft(areaWidth);
        auto filtersArea2 = bounds.removeFromLeft(areaWidth);
        
        
        leftThresholdSlider.setBounds(thresArea.removeFromTop(thresArea.getHeight() * 0.5));
        leftAttackSlider.setBounds(attackArea.removeFromTop(attackArea.getHeight() * 0.5));
        leftReleaseSlider.setBounds(releaseArea.removeFromTop(releaseArea.getHeight() * 0.5));
        leftRatioSlider.setBounds(ratioArea.removeFromTop(ratioArea.getHeight() * 0.5));
        leftKneeSlider.setBounds(kneeArea.removeFromTop(kneeArea.getHeight() * 0.5));
        leftMakeUpSlider.setBounds(makeUpArea.removeFromTop(makeUpArea.getHeight() * 0.5));
        leftBlendSlider.setBounds(blendArea.removeFromTop(blendArea.getHeight() * 0.5));
        
        int buttonHeight = filtersArea.getHeight() / 3;
        
        linkChannels.setBounds(822, 20, 50, 50);
        linkAbsolute.setBounds(922, 20, 50, 50);
        leftBypass.setBounds(822, 80, 50, 50);
        leftMute.setBounds(822, 120, 50, 50);
        leftLpfscButton.setBounds(filtersArea.removeFromTop(80));
        leftHpfscButton.setBounds(filtersArea.removeFromTop(80));
        leftNotchButton.setBounds(filtersArea.removeFromTop(80));
        
        
        leftThresholdRes.setBounds(thresArea.removeFromTop(thresArea.getHeight() * 0.2).withWidth(100) );
        
        leftMakeUpRes.setBounds(makeUpArea.removeFromTop(makeUpArea.getHeight() * 0.2).withWidth(100));
        leftBlendRes.setBounds(blendArea.removeFromTop(blendArea.getHeight() * 0.2).withWidth(100));
        operationMode.setBounds(0, 70, 130, 20);
        vuMode.setBounds(300, 70, 130, 20);
        leftFastPreset.setBounds(0, 110, 130, 20);
        rightFastPreset.setBounds(0, 420, 130, 20);
        
        leftThresholdBypass.setBounds(thresArea.removeFromTop(thresArea.getHeight() * 0.2).withWidth(100));
        leftMakeUpBypass.setBounds(makeUpArea.removeFromTop(makeUpArea.getHeight() * 0.2).withWidth(100));
        leftBlendBypass.setBounds(blendArea.removeFromTop(blendArea.getHeight() * 0.2).withWidth(100));
        leftHpfFreqSlider.setBounds(leftHpfscButton.getX() + 55, leftHpfscButton.getY() - 20, 80, 80);
        leftLpfFreqSlider.setBounds(leftLpfscButton.getX() +55, leftLpfscButton.getY() -20, 80, 80);
        leftNotchFreqSlider.setBounds(leftNotchButton.getX() +55, leftNotchButton.getY() -20, 80, 80);
        leftNotchQSlider.setBounds(leftNotchFreqSlider.getX() +80, leftNotchFreqSlider.getY() , 80, 80);


        
    }
}


void GravityAudioProcessorEditor::buttonClicked(juce::Button *button)
{
    if (button == &sendButton)
    {
        if (ws && ws->getReadyState() == gin::WebSocket::OPEN)
                {
                    juce::String messageToSend = "Hello, WebSocket!";

                    ws->send(messageToSend);
                }

    }
}




std::vector<juce::Component*> GravityAudioProcessorEditor::getComps()
{
    return
    {
        &leftThresholdSlider,
        &rightThresholdSlider,
        &leftAttackSlider,
        &rightAttackSlider,
        &leftReleaseSlider,
        &rightReleaseSlider,
        &leftMakeUpSlider,
        &rightMakeUpSlider,
        &leftRatioSlider,
        &rightRatioSlider,
        &leftKneeSlider,
        &rightKneeSlider,
        &leftBlendSlider,
        &rightBlendSlider,
        &leftHpfscButton,
        &rightHpfscButton,
        &leftLpfscButton,
        &rightLpfscButton,
        &leftFastPreset,
        &rightFastPreset,
        &leftNotchButton,
        &leftNotchFreqSlider,
        &leftNotchQSlider,
        &rightNotchButton,
        &rightNotchFreqSlider,
        &rightNotchQSlider,
        &scExtButton,
        &leftThresholdRes,
        &rightThresholdRes,
        &leftMakeUpRes,
        &rightMakeUpRes,
        &leftBlendRes,
        &rightBlendRes,
        &operationMode,
        &vuMode,
        &leftThresholdBypass,
        &rightThresholdBypass,
        &leftMakeUpBypass,
        &rightMakeUpBypass,
        &leftBlendBypass,
        &rightBlendBypass,
        &leftBypass,
        &rightBypass,
        &leftMute,
        &rightMute,
        &sendButton,
        &linkChannels,
        &linkAbsolute
    };
}
//componentes que tienen aspecto personalizado lookAndFeel
std::vector<juce::Component*> GravityAudioProcessorEditor::getCompsLF()
{
    return
    {
        &leftAttackSlider,
        &rightAttackSlider,
        &leftReleaseSlider,
        &rightReleaseSlider,
        &leftKneeSlider,
        &rightKneeSlider,
        &leftRatioSlider,
        &rightRatioSlider

    };
}

std::vector<juce::Component*> GravityAudioProcessorEditor::getCompsOtherLF()
{
    return
    {
        &leftThresholdSlider,
        &rightThresholdSlider,
        &leftMakeUpSlider,
        &rightMakeUpSlider,
        &leftBlendSlider,
        &rightBlendSlider

    };
}


