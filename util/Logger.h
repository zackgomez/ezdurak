#pragma once
#include <iostream>
#include <string>

class Logger
{
public:
    static Logger getLogger(const std::string &prefix)
    {
        Logger ret(prefix);
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

