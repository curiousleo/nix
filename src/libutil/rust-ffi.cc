#include <string.h>

#include "logging.hh"
#include "rust-ffi.hh"

extern "C" std::exception_ptr * make_error(rust::StringSlice s)
{
    return new std::exception_ptr(std::make_exception_ptr(nix::Error(std::string(s.ptr, s.size))));
}

extern "C" void destroy_error(std::exception_ptr * ex)
{
    free(ex);
}

extern "C" size_t message_size(std::exception_ptr * ex)
{
    try {
        std::rethrow_exception(*ex);
    } catch (std::exception &e) {
        return strlen(e.what());
    } catch (...) {
        return 0;
    }
}

extern "C" void copy_error_message(std::exception_ptr * ex, char * dest, size_t dest_sz)
{
    try {
        std::rethrow_exception(*ex);
    } catch (std::exception &e) {
        strncpy(dest, e.what(), dest_sz);
    } catch (...) {
        // Do nothing
    }
}

namespace rust {

std::ostream & operator << (std::ostream & str, const String & s)
{
    str << (std::string_view) s;
    return str;
}

}
