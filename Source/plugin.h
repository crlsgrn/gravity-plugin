/*
  ==============================================================================

    plugin.h
    Created: 5 Sep 2023 9:53:55am
    Author:  Carlos Garin

  ==============================================================================
*/

#pragma once

#define MAX_CHANNELS                    2

#define STEREO_OPERATION                0
#define DUAL_OPERATION                  1
#define MS_OPERATION                    2
#define SERIAL_MONO_OPERATION           3
#define PARALLEL_MONO_OPERATION         4

#define STEREO_CHANNEL                  0
#define LEFT_CHANNEL                    0
#define RIGHT_CHANNEL                   1
#define MID_CHANNEL                     0
#define SIDE_CHANNEL                    1
#define FIRST_STAGE                     0
#define SECOND_STAGE                    1

enum {
    PRESET_A = 0,
    PRESET_B = 1,
    PRESET_C = 2
};

enum {
    ATTACK_SLOW = 0,
    ATTACK_2 = 1,
    ATTACK_3 = 2,
    ATTACK_4 = 3,
    ATTACK_5 = 4,
    ATTACK_6 = 5,
    ATTACK_7 = 6,
    ATTACK_FAST = 7
};

enum {
    RELEASE_AUTO = 0,
    RELEASE_SLOW = 1,
    RELEASE_3 = 2,
    RELEASE_4 = 3,
    RELEASE_5 = 4,
    RELEASE_6 = 5,
    RELEASE_7 = 6,
    RELEASE_FAST = 7
};

enum {
    RATIO_1_5_1 = 0,
    RATIO_2_1 = 1,
    RATIO_3_1 = 2,
    RATIO_4_1 = 3,
    RATIO_8_1 = 4,
    RATIO_10_1 = 5,
    RATIO_15_1 = 6,
    RATIO_LIMIT = 7
};

enum {
    KNEE_SOFT = 0,
    KNEE_MEDIUM = 1,
    KNEE_HARD = 2
};

enum {
    VUMETER_IN = 0,
    VUMETER_OUT = 1,
    VUMETER_GR = 2,
    VUMETER_SC_EXT = 3,
    VUMETER_FANTASY = 4
};

enum {
    VUMETER_MODE_CLEAN = 0,             // Sin ningún tipo de memoria
    VUMETER_MODE_ONLY_PEAK = 1,         // Solo reteniendo el posible PEAK (rojo) N segundos
    VUMETER_MODE_FIXED_PEAK = 2,        // Reteniendo el PEAK (rojo) hasta que se borre
    VUMETER_MODE_MEMORY = 3,            // Reteniendo los valores máximos N segundos (independientemente del PEAK)
    VUMETER_MODE_MEMORY_AND_PEAK = 4    // Reteniendo los valores máximos N segundos (además de dejar fijo el PEAK)
};

const uint8_t TOTAL_VUMETERS    = 5;

const uint8_t DEFAULT_HPF_F     = 95;
const uint8_t DEFAULT_LPF_F     = 251;
const uint8_t DEFAULT_NOTCH_F   = 237;
const uint8_t DEFAULT_NOTCH_Q   = 0;

const float MIN_BLEND = 0.0f;
const float MAX_BLEND = 100.0f;
const int MAX_BLEND_RESOLUTIONS = 5;
const int MAX_BLEND_STEPS = 100;
const float blend_resolutions_list[MAX_BLEND_RESOLUTIONS]      = {   1,  2,  5, 10, 20 };
const float blend_resolutions_max_steps[MAX_BLEND_RESOLUTIONS] = { 100, 50, 20, 10,  5 };

const float MIN_THRESHOLD = -20.0f;
const float MAX_THRESHOLD = 20.0f;
const int MAX_THRESHOLD_RESOLUTIONS = 7;
const int MAX_THRESHOLD_STEPS = 200;
const float threshold_resolutions_values[MAX_THRESHOLD_RESOLUTIONS]      = { 0.2, 0.25, 0.5, 1.0, 2.0, 4.0, 5.0 };
const float threshold_resolutions_max_steps[MAX_THRESHOLD_RESOLUTIONS]   = { 200,  160,  80,  40,  20,  10,   8 };

const float MIN_MAKEUP = 0.0f;
const float MAX_MAKEUP = 20.0f;
const int MAX_MAKEUP_RESOLUTIONS = 7;
const int MAX_MAKEUP_STEPS = 100;
const float makeup_resolutions_values[MAX_MAKEUP_RESOLUTIONS]      = { 0.2, 0.25, 0.5, 1.0, 2.0, 4.0, 5.0 };
const float makeup_resolutions_max_steps[MAX_MAKEUP_RESOLUTIONS]   = { 100,   80,  40,  20,  10,   5,   4 };

const uint16_t blend2dry[101] = { 0,7,7,8,9,10,11,13,14,15,17,19,21,22,24,27,29,31,34,37,40,43,46,50,53,57,61,66,70,75,80,85,91,97,103,109,116,123,130,137,145,153,162,171,180,190,200,210,221,232,244,256,268,281,295,309,323,338,353,369,386,403,420,438,457,476,496,517,538,560,582,605,629,653,678,704,731,758,786,815,845,876,907,939,972,1006,1041,1076,1113,1150,1189,1228,1268,1310,1352,1395,1440,1485,1531,1579,4095 };
const uint16_t blend2wet[101] = { 4095,1579,1531,1485,1440,1395,1352,1310,1268,1228,1189,1150,1113,1076,1041,1006,972,939,907,876,845,815,786,758,731,704,678,653,629,605,582,560,538,517,496,476,457,438,420,403,386,369,353,338,323,309,295,281,268,256,244,232,221,210,200,190,180,171,162,153,145,137,130,123,116,109,103,97,91,85,80,75,70,66,61,57,53,50,46,43,40,37,34,31,29,27,24,22,21,19,17,15,14,13,11,10,9,8,7,7,0 };

struct AudioChannelGravity {
    uint8_t id = LEFT_CHANNEL;                                  // Id del canal. 0 = STEREO/LEFT/MID/S1, 1 = RIGHT/SIDE/S2
    char role[6] = "Left";                                      // Nombre del Id del canal (el rol que juega)

    char name[100] = "Channel 1";                               // Nombre del canal que se ha establecido desde el DAW
    uint32_t color = 0x000000;                                  // Color del canal que se ha establecido desde el DAW

    uint8_t threshold = uint8_t(MAX_THRESHOLD_STEPS / 2);       // Número de paso en el que está el Umbral
    float threshold_value = 0.0f;                               // Valor en dB del Umbral, calculado a partir del número de paso
    bool threshold_muted = false;                               // Si se ha silenciado el parámetro (es decir, por defecto a 0 dB = Step 100)
    uint8_t last_threshold = uint8_t(MAX_THRESHOLD_STEPS / 2);  // Valor en pasos que tenía el parámetro cuando se activó su mute
    float last_threshold_value = 0.0f;                          // Valor en dB que tenía el parámetro cuando se activó su mute
    uint8_t threshold_resolution = 3;                           // Resolución elegida (índice de los arrays threshold_resolutions_values y threshold_resolutions_max_steps)
    bool available_threshold_resolutions[MAX_THRESHOLD_RESOLUTIONS] = {true, true, true, true, true, true, true}; // Qué resoluciones tengo habilitadas o deshabilitadas
    uint8_t attack = ATTACK_SLOW;                               // Posición del parámetro Attack
    uint8_t ratio = RATIO_4_1;                                  // Posición del Ratio en modo Compressor
    bool expander = false;                                      // Si está activo el Expander (Expansor)
    uint8_t expander_ratio = RATIO_1_5_1;                       // Posición del Ratio en modo Expander
    uint8_t release = RELEASE_6;                                // Posición del parámetro Release
    uint8_t knee = KNEE_SOFT;                                   // Posición de la rodilla
    uint8_t makeup = MIN_MAKEUP;                                // Posición en pasos del parámetro MakeUp
    float makeup_value = 0.0;                                   // Valor en dB del parámetro MakeUp
    bool makeup_muted = false;                                  // Si está muteado/bypaseado el MakeUp (es decir, 0 dB)
    uint8_t last_makeup = MIN_MAKEUP;                           // Valor en pasos del MakeUp cuando se silenció el parámetro
    float last_makeup_value = MIN_MAKEUP;                       // Valor en dB del MakeUp cuando se silenció el parámetro
    uint8_t makeup_resolution = 3;                              // Resolución elegida para el MakeUp
    bool available_makeup_resolutions[MAX_MAKEUP_RESOLUTIONS] = {true, true, true, true, true, true, true}; // Qué resoluciones tengo habilitadas o deshabilitadas
    uint8_t blend = MAX_BLEND_STEPS;                            // Valor en pasos del Blend
    bool blend_muted = false;                                   // Si se ha slenciado el parámetro (sin mezcla, DRY)
    uint8_t last_blend = MAX_BLEND_STEPS;                       // Valor en pasos del Blend cuando se silenció el parámetro
    uint8_t blend_resolution = 3;                               // Valor en dB del Blend cuando se silenció el parámetro
    bool available_blend_resolutions[MAX_BLEND_RESOLUTIONS] = {true, true, true, true, true}; // Qué resoluciones tengo habilitadas o deshabilitadas
    
    bool hpf = false;                                           // HPF SC: On/Off
    bool lpf = false;                                           // LPF SC: On/Off
    bool notch = false;                                         // Notch SC: On/Off
    uint8_t hpf_f = DEFAULT_HPF_F;                              // Frecuencia del HPF: por defecto posicion 95 = unos 100 Hz
    uint8_t lpf_f = DEFAULT_LPF_F;                              // Frecuencia del LPF: por defecto posición 251 = unos 17.5 kHz
    uint8_t notch_f = DEFAULT_NOTCH_F;                          // Frecuencia del Notch: por defecto posición 237 = unos 10.0 kHz
    uint8_t notch_q = DEFAULT_NOTCH_Q;                          // Q (amplitud) del Notch
    bool filters_muted = false;                                 // Silenciamiento de los filtros (HPF + LPF + Notch)
    bool sc_ext = false;                                        // Si está activo el SC Externo, o el interno (Input) que es el de por defecto
    bool bypass = false;                                        // Bypass completo de compresión
    bool mute = false;                                          // Mute completo del equipo (se corta la salida en los Output)
    
    uint16_t fast_presets[3] = {0, 0, 0};                       // Id de los FastPresets: 0 = A, 1 = B, 2 = C

    float gain_reduction = 0.0;                                 // Valor en dB de GR que se están midiendo en tiempo real
    bool auto_gain_reduction = false;                           // Activar el sistema de ajuste del MakeUp automáticamente según la GR medida

    uint8_t vumeter_mapping = VUMETER_GR;                       // Señal que está midiendo el vúmetro: 0 = IN, 1 = OUT, 2 = GR, 3 = EXT. SC, 4 = FANTASY
    uint8_t vumeter_mode = VUMETER_MODE_CLEAN;                  // Comportamiento del vúmetro
    uint8_t vumeter_mode_seconds = 2;                           // Número de segundos del vúmetro
    uint16_t vumeter_raw[4] = {0, 0, 0, 0};                     // Valor medido en el vúmetro por el ADC
    uint8_t vumeter_value[4] = {0, 0, 0, 0};                    // Valor traducido del vúmertro de 0 a 100
    bool vumeter_peak[4] = {false, false, false, false};        // indicador para saber si está picando el vúmetro o no
};

struct AudioStateGravity {
    uint8_t operation_mode = STEREO_OPERATION;                  // 0 = STEREO, 1 = DUAL MONO, 2 = MID/SIDE, 3 = SERIAL MONO, 4 = PARALLEL MONO
    bool link_channels = false;                                 // Si los canales están enlazados entre si
    bool link_absolute = false;                                 // Enlace entre canales: Absoluto o Relativo (por defecto)

    // Lo seleccionado en el frontal
    uint8_t real_channel = LEFT_CHANNEL;                        // Qué Id de canal está seleccionado en el frontal

    // Donde escribimos y guardamos lo que estamos usando en tiempo real
    uint8_t current_channel = LEFT_CHANNEL;                     // Id del canal en el que estoy trabajando
    AudioChannelGravity channel[MAX_CHANNELS];                  // Configuración del canal en el que estoy trabajando

    // Donde guardo como quedó ese canal antes de cmabiar de modo
    AudioChannelGravity stereo[1];                              // Configuración en tiempo real del modo Stereo
    AudioChannelGravity dual[MAX_CHANNELS];                     // Configuración en tiempo real del modo Dual Mono
    AudioChannelGravity ms[MAX_CHANNELS];                       // Configuración en tiempo real del modo Mid/Side
    AudioChannelGravity serial[MAX_CHANNELS];                   // Configuración en tiempo real del modo Serial Mono
    AudioChannelGravity parallel[MAX_CHANNELS];                 // Configuración en tiempo real del modo Parallel Mono

    uint16_t fast_presets[3] = {0, 0, 0};                       // Id de los Fast Presets (A, B y C)

    uint8_t vumeter_mapping = VUMETER_GR;                       // Señal que está midiendo el vúmetro: 0 = IN, 1 = OUT, 2 = GR, 3 = EXT. SC, 4 = FANTASY
    uint8_t vumeter_mode = VUMETER_MODE_CLEAN;                  // Comportamiento del vúmetro
    uint8_t vumeter_mode_seconds = 2;                           // Número de segundos del vúmetro
    uint16_t vumeter_raw[TOTAL_VUMETERS][MAX_CHANNELS] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};    // Valor medido para cada vúmetro en cada canal y tipo de mapeo
    uint8_t vumeter_value[TOTAL_VUMETERS][MAX_CHANNELS] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}};   // Valor traducido para cada vúmetro en cada canal y tipo de mapeo
    bool vumeter_peak[TOTAL_VUMETERS][MAX_CHANNELS] = {{false, false}, {false, false}, {false, false}, {false, false}, {false, false}}; // Indicador de Pico para cada vúmetro por canal y tipo de mapeo
    
};



/*
float bit2freq(uint8_t _bit);
char* freq2text(float _freq);

float bit2freq(uint8_t _bit)
{
    float mV = (360.0 / 255.0) * float(_bit);
    float bit2mv = pow(20000.0, (mV / 360.0));
    float compensation = pow(20.0, (359.0 + mV) / 360.0);
    
    return bit2mv + compensation;
}

char* freq2text(float _freq)
{
    char* data = (char*) calloc(10, sizeof(char));

    if (_freq < 1000.0)
        sprintf(data, "%d Hz", int(_freq));
    else if (_freq >= 1000.0 && _freq < 10000.0)
        sprintf(data, "%.2f kHz", (_freq / 1000));
    else if (_freq >= 10000.0)
        sprintf(data, "%.1f kHz", (_freq / 1000));
    else
        sprintf(data, "0 Hz");

    return data;
}
*/
