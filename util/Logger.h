#pragma once
#include <iostream>
#include <string>

class Logger;
typedef std::auto_ptr<Logger> LoggerPtr;

class Logger
{
public:
    static LoggerPtr getLogger(const std::string &prefix)
    {
        LoggerPtr ret(new Logger(prefix));
        return ret;
    }

    std::ostream& debug()
    {
        std::clog << "DEBUG - " << prefix_ << ": ";
        return std::clog;
    }
    std::ostream& info()
    {
        std::cout << "INFO - " << prefix_ << ": ";
        return std::cout;
    }
    std::ostream& warning()
    {
        std::cerr << "WARNING - " << prefix_ << ": ";
        return std::cerr;
    }
    std::ostream& error()
    {
        std::cerr << "WARNING - " << prefix_ << ": ";
        return std::cerr;
    }

private:
    Logger(const std::string &prefix) :
        prefix_(prefix)
    { /* Empty */ }

    std::string prefix_;
};

