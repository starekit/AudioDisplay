#ifndef FILE_SYSTEM_HPP_
#define FILE_SYSTEM_HPP_
#include "string"
#include "esp_littlefs.h"
using namespace std;
namespace core{
    class FileSystem{
        public:
        FileSystem();
        ~FileSystem();

        private:
            bool writeHTML();
            bool writeTXT();
            bool writeLOG();

        public:
            string read(string filePath);
            bool write(string *file);
         




    };
    string FileSystem::read(string filePath){
        return "h";

    }
    bool FileSystem::write(string *file){
        //write html
        //write txt
        //write LOG
        return false;

    }



}

#endif