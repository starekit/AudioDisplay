#pragma once
#include "core.hpp"                                                                                        
#include "driver/temperature_sensor.h"


namespace core{
	typedef struct {
		uint32_t lastIdle0;
		uint32_t lastIdle1;
		uint32_t lastTotal;
		float core0Used;
		float core1Used;
	} cpuStats;

    class SystemMonitor{

        private:
			cpuStats stats;

            temperature_sensor_handle_t temp_sensor = nullptr;
			
            static constexpr const char* TAG = "SystemMonitor"; 
            void initSensor();
			void updateCpuStats();
        public:
            SystemMonitor(){
				stats={};
                initSensor();
            }
            ~SystemMonitor(){
				//停止传感器
                temperature_sensor_disable(temp_sensor);
            }
            float getCpuTemperature();
            float getCpuUsed();
            uint32_t getMemoryUsed();




    };

}