#pragma once
#include "core.hpp"
namespace core{

    class Sensor{
        private:
        
        
        
        public:
            Sensor();
            ~Sensor();
        private:
            void initSensor();
        public:
            void getData();
    };



}