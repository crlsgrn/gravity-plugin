/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
GravityAudioProcessor::GravityAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    
    ws = easywsclient::WebSocket::from_url("ws://localhost:8126/foo");
    //ws = easywsclient::WebSocket::from_url("ws://192.168.1.133/ws");

  
}

GravityAudioProcessor::~GravityAudioProcessor()
{
    
}

//==============================================================================
const juce::String GravityAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GravityAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GravityAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GravityAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GravityAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GravityAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GravityAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GravityAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String GravityAudioProcessor::getProgramName (int index)
{
    return {};
}

void GravityAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void GravityAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
}

void GravityAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GravityAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void GravityAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

  

        
        
        // ..do something to the data...
    }
}

//==============================================================================
bool GravityAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* GravityAudioProcessor::createEditor()
{
    return new GravityAudioProcessorEditor (*this);
}

//==============================================================================
void GravityAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.


    // Agregar campos al objeto JSON para cada miembro de AudioStateGravity.
    audioStateObject->getProperty("operation_mode");
    // Agrega otros campos aquí para todos los miembros de AudioStateGravity.

    // Asignar el objeto DynamicObject a audioStateJson.
    audioStateJson = juce::var(audioStateObject);

    // Luego, puedes convertir el objeto var en una cadena JSON.
    juce::String json = juce::JSON::toString(audioStateJson);

    juce::String jsonString = juce::JSON::toString(audioStateJson);
    // Guarda la cadena JSON en destData
    destData = juce::MemoryBlock(jsonString.toRawUTF8(), jsonString.getNumBytesAsUTF8());

}

void GravityAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================

/*
        OPERATION MODE
 */

bool GravityAudioProcessor::setOperationMode( int _operationMode, bool _sync)
{
    this->state.operation_mode = _operationMode;
    
    if(_sync)
        this->syncParameters();
    
    return true;
}
/*
        VUMETER MODE
 */
bool GravityAudioProcessor::setVUmeterMode(int _vuMode, bool _sync){
    this->state.vumeter_mode = _vuMode;
    
    if(_sync)
        this->syncParameters();
    
    return true;
}
/*
        FAST PRESET
 */

bool GravityAudioProcessor::setFastPreset(int i, int _channel, int _preset, bool _sync){
    if (_preset <0 || _preset >2) {
        return false;
    }
    this->state.channel[_channel].fast_presets[i] = _preset;
    
    if(_sync)
        this->syncParameters();
    
    return true;
}

/*
        LINK CHANNELS
 */
bool GravityAudioProcessor::setLinkChanels(bool _link, bool _sync){
    
    this->state.link_channels = _link;
    
    
    if(_sync)
        this->syncParameters();
    
    return true;
}

bool GravityAudioProcessor::setLinkAbsolute(bool _absolute, bool _sync){
    
    this->state.link_absolute = _absolute;
    
    
    if(_sync)
        this->syncParameters();
    
    return true;
}




/*
        ======PARÁMETROS=====
 */

/*
        THRESHOLD
 */

bool GravityAudioProcessor::setThreshold(int _channel, float _threshold, bool _sync)
{
    if (_channel < 0 || _channel > 1)
        return false;
    if (_threshold < MIN_THRESHOLD || _threshold > MAX_THRESHOLD)
        return false;
    
    this->state.channel[_channel].threshold_value = _threshold;
    
    // Enviamos la información por el socket
    if(_sync)
        this->syncParameters();
    
    return true;
}

bool GravityAudioProcessor::setThresholdResolution(int _channel, float _res, bool _sync)
{
    if (_channel < 0 || _channel > 1)
        return false;
    if (_res < threshold_resolutions_values[0] || _res > threshold_resolutions_values[MAX_THRESHOLD_RESOLUTIONS-1])
        return false;
    this->state.channel[_channel].threshold_resolution = _res;
    
    // Enviamos la información por el socket
    if(_sync)
        this->syncParameters();
    
    return true;
}

bool GravityAudioProcessor::setThresholdBypass(int _channel, bool _bypass, bool _sync)
{
    if (_channel < 0 || _channel > 1)
        return false;
    
    this->state.channel[_channel].threshold_muted = _bypass;
    
    if (_sync)
        this->syncParameters();
    
    return true;
}


/*
        ATTACK
 */

bool GravityAudioProcessor::setAttack(int _channel, int _attack, bool _sync){
    
    if (_channel < 0 || _channel > 1)
        return false;
    
    if (_attack < 0 || _attack > ATTACK_FAST)
        return false;
    
    this->state.channel[_channel].attack= _attack;

    if (_sync)
        this->syncParameters();
    
    return true;
}

/*
        RELEASE
 */

bool GravityAudioProcessor::setRelease(int _channel, int _release, bool _sync){
    
    if (_channel < 0 || _channel > 1)
        return false;
    
    if (_release < 0 || _release > RELEASE_FAST)
        return false;
    
    this->state.channel[_channel].release= _release;

    if (_sync)
        this->syncParameters();
    
    return true;
}

/*
        RATIO
 */

bool GravityAudioProcessor::setRatio(int _channel, int _ratio, bool _sync){
    
    if (_channel < 0 || _channel > 1)
        return false;
    
    if (_ratio < 0 || _ratio > RATIO_LIMIT)
        return false;
    
    this->state.channel[_channel].ratio= _ratio;

    if (_sync)
        this->syncParameters();
    
    return true;
}

/*
        KNEE
 */

bool GravityAudioProcessor::setKnee(int _channel, int _knee, bool _sync){
    
    if (_channel < 0 || _channel > 1)
        return false;
    
    if (_knee < 0 || _knee > KNEE_HARD)
        return false;
    
    this->state.channel[_channel].knee= _knee;

    if (_sync)
        this->syncParameters();
    
    return true;
}

/*
        MAKEUP
 */

bool GravityAudioProcessor::setMakeUp(int _channel, float _makeUp, bool _sync)
{
    if (_channel < 0 || _channel > 1)
        return false;
    if (_makeUp < MIN_MAKEUP || _makeUp > MAX_MAKEUP)
        return false;
    
    this->state.channel[_channel].makeup = _makeUp;
    
    // Enviamos la información por el socket
    if(_sync)
        this->syncParameters();
    
    return true;
}


bool GravityAudioProcessor::setMakeUpResolution(int _channel, float _res, bool _sync)
{
    if (_channel < 0 || _channel > 1)
        return false;
    
    if (_res < makeup_resolutions_values[0] || _res > makeup_resolutions_values[MAX_MAKEUP_RESOLUTIONS-1])
        return false;
    this->state.channel[_channel].makeup_resolution = _res;
    
    // Enviamos la información por el socket
    if(_sync)
        this->syncParameters();
    
    return true;
}

bool GravityAudioProcessor::setMakeUpBypass(int _channel, bool _bypass, bool _sync)
{
    if (_channel < 0 || _channel > 1)
        return false;
    
    this->state.channel[_channel].makeup_muted = _bypass;
    
    if (_sync)
        this->syncParameters();
    
    return true;
}


/*
        BLEND
 */

bool GravityAudioProcessor::setBlend(int _channel, int _blend, bool _sync)
{
    if (_channel < 0 || _channel > 1)
        return false;
    if (_blend < MIN_BLEND || _blend > MAX_BLEND)
        return false;
    
    this->state.channel[_channel].blend = _blend;
    
    // Enviamos la información por el socket
    if(_sync)
        this->syncParameters();
    
    return true;
}


bool GravityAudioProcessor::setBlendResolution(int _channel, int _res, bool _sync)
{
    if (_channel < 0 || _channel > 1)
        return false;
    
    if (_res < blend_resolutions_list[0] || _res > blend_resolutions_list[MAX_BLEND_RESOLUTIONS-1])
        return false;
    this->state.channel[_channel].blend_resolution = _res;
    
    // Enviamos la información por el socket
    if(_sync)
        this->syncParameters();
    
    return true;
}

bool GravityAudioProcessor::setBlendBypass(int _channel, bool _bypass, bool _sync)
{
    if (_channel < 0 || _channel > 1)
        return false;
    
    this->state.channel[_channel].blend_muted = _bypass;
    
    if (_sync)
        this->syncParameters();
    
    return true;
}


/*
        FILTROS
 */

bool GravityAudioProcessor::setNotch(int _channel, bool _notch, bool _sync)
{
    if (_channel < 0 || _channel > 1)
        return false;
    
    this->state.channel[_channel].notch = _notch;
    
    if (_sync)
        this->syncParameters();
    
    return true;
}

bool GravityAudioProcessor::setNotchFreq(int _channel, float _freq, bool _sync)
{
    if (_channel < 0 || _channel > 1)
        return false;
    
    this->state.channel[_channel].notch_f = _freq;
    
    if (_sync)
        this->syncParameters();
    return true;
}

bool GravityAudioProcessor::setNotchQ(int _channel, float _q, bool _sync)
{
    if (_channel < 0 || _channel > 1)
        return false;
    
    this->state.channel[_channel].notch_q = _q;
    
    if (_sync)
        this->syncParameters();
    return true;
}


bool GravityAudioProcessor::setLPF(int _channel, bool _lpf, bool _sync)
{
    if (_channel < 0 || _channel > 1)
        return false;
    
    this->state.channel[_channel].lpf = _lpf;
    
    if (_sync)
        this->syncParameters();
    
    return true;
}

bool GravityAudioProcessor::setLPFFreq(int _channel, float _freq, bool _sync)
{
    if (_channel < 0 || _channel > 1)
        return false;
    
    this->state.channel[_channel].lpf_f = _freq;
    
    if (_sync)
        this->syncParameters();
    return true;
}

bool GravityAudioProcessor::setHPF(int _channel, bool _hpf, bool _sync)
{
    if (_channel < 0 || _channel > 1)
        return false;
    
    this->state.channel[_channel].hpf = _hpf;
    
    if (_sync)
        this->syncParameters();
    
    return true;
}

bool GravityAudioProcessor::setHPFFreq(int _channel, float _freq, bool _sync)
{
    if (_channel < 0 || _channel > 1)
        return false;
    
    this->state.channel[_channel].hpf_f = _freq;
    
    if (_sync)
        this->syncParameters();
    return true;
}

bool GravityAudioProcessor::setGeneralBypass(int _channel, bool _bypass, bool _sync)
{
    if (_channel < 0 || _channel > 1)
        return false;
    this->state.channel[_channel].bypass = _bypass;
    
    if(_sync)
        this->syncParameters();
    return true;
}

bool GravityAudioProcessor::setGeneralMute(int _channel, bool _mute, bool _sync)
{
    if (_channel < 0 || _channel > 1)
        return false;
    this->state.channel[_channel].mute = _mute;
    
    if(_sync)
        this->syncParameters();
    return true;
}


/*
        Donde se van a sicronizar los parámetros entre Editor y Processor
 */


void GravityAudioProcessor::init(){
    // Crear un comando JSON para "init"
    Command initCommand;
    initCommand.command = "init";
    initCommand.data = juce::var::undefined();
    //Después del init el GMM devuelve el paquete set-state que envia el estado completo para que yo actualice mis variables
}


void GravityAudioProcessor::syncParameters()
{
    // TODO: enviar datos Plugin --> GMM
    //IMPLEMENTAR LOGICA PARA ENVIAR DATOS A TRAVES DEL WEBSOCKET
    //Usar hilos para que no se bloquee la comunicación
    // Crea un comando JSON para enviar el estado actualizado al hardware

    
    
}



AudioStateGravity GravityAudioProcessor::getState()
{
//Se convierte el JSON en variables para actualizar los estados del plugin y que se refelje en el editor
//      GMM --> Plugin
    
    // Crear un comando JSON para "get-state"
    Command getStateCommand;
    getStateCommand.command = "get-state";
    getStateCommand.data = juce::var::undefined();


    // Devuelve un estado temporal, puedes actualizarlo cuando recibas la respuesta
    AudioStateGravity tempState;
    return tempState;
    
}

int GravityAudioProcessor::setState(AudioStateGravity _state){
    
    return 0;
}





//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GravityAudioProcessor();
}


