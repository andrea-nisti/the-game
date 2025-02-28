#ifndef SUPPORT_NETWORKING_HTTP_COMMON_H
#define SUPPORT_NETWORKING_HTTP_COMMON_H

#include <string>
#include <unordered_map>

namespace game::support {

using Path = std::string;
using ParamKey = std::string;
using ParamValue = std::string;

using Params = std::unordered_map<ParamKey, ParamValue>;

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

#endif  // SUPPORT_NETWORKING_HTTP_COMMON_H
