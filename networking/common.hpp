#ifndef NETWORKING_COMMON_H
#define NETWORKING_COMMON_H

#include <string>
#include <unordered_map>

namespace game::networking {

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

}  // namespace game::networking

#endif  // NETWORKING_COMMON_H
