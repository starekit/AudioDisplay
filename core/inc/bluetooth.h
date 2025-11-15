#pragma once
// #include "?"
namespace core{

    class Bluetooth{



        public:
            Bluetooth();
            ~Bluetooth();

        public:
            void begin();
            void scan();
            void link();

    };


}