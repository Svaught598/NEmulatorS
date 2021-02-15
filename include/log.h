#ifndef NES_LOG
#define NES_LOG

#include <fstream>
#include <iomanip>
#include <iostream>


class Logger
{
public:

    // Log types
    enum class logType {
        LOG_ERROR, LOG_WARNING, LOG_OPCODE, LOG_INFO, LOG_ENDLINE
    };

    // Constructor
    explicit Logger (const char *fname = "nes_log.txt")
    {
        numWarnings = 0;
        numErrors = 0;

        myFile.open(fname);
        if (myFile.is_open()) {
            myFile << "NES emulator - Steven Vaught" << std::endl;
            myFile << "Log file created" << std::endl << std::endl;
        }
    }

    // Destructor
    ~Logger(){
        if (myFile.is_open()) {
            myFile << std::endl << std::endl;

            // Report number of errors and warnings
            myFile << numWarnings << " warnings" << std::endl;
            myFile << numErrors << " errors" << std::endl;
            myFile.close();
        } 
    }


    // Overload << operator using log type
    friend Logger &operator << (Logger &logger, const logType l_type){
        switch (l_type) {
            case Logger::logType::LOG_ERROR:
                logger.myFile << "[ERROR]: ";
                ++logger.numErrors;
                break;
            case Logger::logType::LOG_WARNING:
                logger.myFile << "[WARNING]: ";
                ++logger.numWarnings;
                break;
            case Logger::logType::LOG_OPCODE:
                logger.myFile << "[OPCODE]: ";
                break;
            case Logger::logType::LOG_INFO:
                logger.myFile << "[INFO]: ";
                break;
            case Logger::logType::LOG_ENDLINE:
                logger.myFile << std::endl;
                break;
            default:
                break;
        }
        return logger;
    }


    // Overload << operator using char* strings
    friend Logger &operator << (Logger &logger, const char *text) {
        logger.myFile << text;
        return logger;
    }

    friend Logger &operator << (Logger &logger, const std::string &text){
        logger.myFile << text;
        return logger;
    }

    friend Logger &operator << (Logger &logger, const int &val){
        if (val < 0x100){
            logger.myFile << std::hex << std::setfill('0') << std::setw(2) << val;
        } else {
            logger.myFile << std::hex << std::setfill('0') << std::setw(4) << val;
        }
        return logger;
    }

private:

    std::ofstream myFile;
    unsigned int numWarnings;
    unsigned int numErrors;

};

#endif