#ifndef NETWORKING_HTTP_BEAST_UTILS
#define NETWORKING_HTTP_BEAST_UTILS

#include <boost/beast/http.hpp>

#include "common.hpp"

using namespace boost::beast;

namespace game::networking {

/**
 * @brief Convert between boost::beast::http::verb and HttpMethod.
 *
 * This function takes a value of type V and returns the corresponding value of
 * type U, where U is the other type of either http::verb or HttpMethod.
 *
 * The conversion is done by looking up the value in a static array of pairs,
 * where each pair contains a value of type V and the corresponding value of type
 * U.
 *
 * If the value is not found in the array, the function returns the value of type
 * U that corresponds to http::verb::unknown or HttpMethod::UNKNOWN,
 * depending on the type of the input value.
 *
 * @tparam V The type of the value to convert. Must be either http::verb or
 * HttpMethod.
 *
 * @param value The value to convert.
 *
 * @return The converted value.
 */
template <typename V>
constexpr auto ConvertVerbBeast(const V& value)
{
    static_assert(
        std::is_same_v<V, http::verb> or std::is_same_v<V, HttpMethod>,
        "Must be http::verb or HttpMethod");

    constexpr std::array map {
        std::make_pair(http::verb::get, HttpMethod::GET),
        std::make_pair(http::verb::post, HttpMethod::POST),
        std::make_pair(http::verb::put, HttpMethod::PUT),
        std::make_pair(http::verb::delete_, HttpMethod::DELETE),
        std::make_pair(http::verb::patch, HttpMethod::PATCH),
        std::make_pair(http::verb::unknown, HttpMethod::UNKNOWN)};

    constexpr auto find = [](const V& value, const auto& map)
    {
        if constexpr (std::is_same_v<V, http::verb>)
        {
            for (const auto& pair : map)
            {
                if (std::get<0>(pair) == value)
                {
                    return std::get<1>(pair);
                }
            }
            return HttpMethod::UNKNOWN;
        } else
        {
            for (const auto& pair : map)
            {
                if (std::get<1>(pair) == value)
                {
                    return std::get<0>(pair);
                }
            }
            return http::verb::unknown;
        }
    };

    return find(value, map);
}

}  // namespace game::networking

#endif  // NETWORKING_HTTP_BEAST_UTILS
