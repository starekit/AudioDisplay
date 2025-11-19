#include "Sensor.h"
using namespace core;
namespace manage{
    class SensorManage{
        private:
            std::unique_ptr<Sensor> sensor=std::make_unique<Sensor>();
        public:
            SensorManage();
            ~SensorManage();


    };
}