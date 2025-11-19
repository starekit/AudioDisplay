#pragma once
#include "core.hpp"                                                                                        
#include "driver/temp_sensor.h"


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
			
            static constexpr const char* TAG = "SystemMonitor"; 
            void initSensor();
			void updateCpuStats();
        public:
            SystemMonitor(){
				stats={0};
                initSensor();
            }
            ~SystemMonitor(){
				//停止传感器
				temp_sensor_stop();

            }
            float getCpuTemperature();
            float getCpuUsed();
            uint32_t getMemoryUsed();




    };

}