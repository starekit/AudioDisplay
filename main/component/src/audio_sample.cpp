// #include <AudioSample.h>
// AudioSample::AudioSample(){
//     audio_init_config();
//     xTaskCreate(
//         audio_sample_task,
//         "AUDIO_SAMPLE_TASK",
//         2048,
//         NULL,
//         5,
//         &audioHandle
//     );
// }
// AudioSample::~AudioSample(){
//     if(data!=nullptr){
//         delete data;
//     }
//     else{
//         #ifdef DEBUGE

//         #endif
//     }

//     vTaskDelete(audioHandle);
// }

// void AudioSample::audio_init_config(){
//     adc1_config_width(ADC_WIDTH);
//     adc1_config_channel_atten(ADC_CHANNEL,ADC_ATTEN);
//     esp_adc_cal_value_t calValue=esp_adc_cal_characterize(
//         ADC_UNIT_1,
//         ADC_ATTEN,
//         ADC_WIDTH,
//         1100,
//         &adc_cal
//     );
    
//     #ifdef DEBUGE
//         Serial.println();
//     #endif

// }
// void AudioSample::audio_sample_task(void *){
//     while(1){
//         uint32_t voltage_mv=esp_adc_cal_raw_to_voltage(adc1_get_raw(ADC_CHANNEL),&adc_cal);
//         #ifdef DEBUGE
//             Serial.println("原始电压，采样电压");
//         #endif
//         // return voltage_mv/1000;

//     }
// }
// AudioData* AudioSample::get_data(){
//     return data;
// }