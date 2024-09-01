#ifndef SUPPORT_NETWORKING_COMMON_H
#define SUPPORT_NETWORKING_COMMON_H

#include <string>

namespace game::support
{

using Path = std::string;

enum class HttpMethod
{
    UNKNOWN,
    GET,
    POST,
    PUT,
    DELETE,
    PATCH
};

}  // namespace game::support

#endif