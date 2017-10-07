/** @file MRKVulkanTools.h
*
*  @brief 
*  Contains macros to catch vulkan exceptions
*  and to throw personal exception much easier
*
*  @author Juan Ramos 
*/
#pragma once

#include "Precompiled.h"

constexpr bool ifProjectIsBuiltInDebugMode()
{
    bool enable = false;

#ifdef _DEBUG
    enable = true;
#endif

    return enable;
}

namespace
{
    void handleException(std::exception const &error)
    {
        std::cerr << "============= MRK CAUGHT VULKAN EXCEPTION =============" << std::endl;
        std::cerr << "LINE NUMBER: " << __LINE__ << std::endl << "FILE: " << __FILE__ << std::endl;
        std::cerr << "ERROR MESSAGE: " << error.what();
        std::cerr << "=======================================================" << std::endl;
    }
}

#define MRK_CATCH(expr)                 \
    try {                               \
        (expr);                         \
    }                                   \
    catch (std::exception const & ex) { \
        handleException(ex);            \
    }                                   \

namespace mrk
{

class exception : public std::runtime_error
{
private:
    std::string msg;
public:
    exception(const std::string &arg, const char *file, int line) :
        std::runtime_error(arg)
    {
        std::ostringstream o;

        o << "FILE: " << file << "\nLINE: " << line << "\nERROR MESSAGE: " << arg;

        msg = o.str();
    }

    ~exception() {}

    const char *what() const override {
        return msg.c_str();
    }

    void displayError() const
    {
        using namespace std::this_thread;     // sleep_for, sleep_until
        using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
        using std::chrono::system_clock;

        std::cerr << msg.c_str() << std::endl;

        sleep_for(3ns);
        sleep_until(system_clock::now() + 1s);
    }
};

}

#define throw_line(arg) throw mrk::exception(arg, __FILE__, __LINE__);
