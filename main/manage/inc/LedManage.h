#include "Led.h"
using namespace core;
namespace manage{
    class LedManage{
        private:
        
            std::unique_ptr<Led> led=std::unique_ptr<Led>();

        public:
            LedManage();
            ~LedManage();
        
    };

}