// #pragma once

// #include <Arduino.h>
// #include <esp_adc_cal.h>

// #define RELEASE

// //ADC setting
// #define ADC_CHANNEL ADC1_CHANNEL_0
// #define ADC_ATTEN ADC_ATTEN_11db
// #define ADC_WIDTH ADC_WIDTH_12Bit

// #define SAMPLE_TIME 100
// #define SAMPLE_LENGTH_MAX 1000
// struct AudioData{
//     int data[SAMPLE_LENGTH_MAX];
// };

// class AudioSample{
//     private:
//         AudioData *data;
//         TaskHandle_t audioHandle;

//     private:
//         static esp_adc_cal_characteristics_t adc_cal;

//     public:
//         AudioSample();
//         ~AudioSample();

    
//     private:
//         void audio_init_config();
//         static void audio_sample_task(void *);
//     public:
//         AudioData* get_data();

// };