#ifndef SUPPORT_NETWORKING_ROUTE_MANAGER_BASE_H
#define SUPPORT_NETWORKING_ROUTE_MANAGER_BASE_H

#include <functional>
#include <memory>
#include <type_traits>
#include <unordered_map>

namespace game::support
{

/**
 * \addtogroup support Support
 * \brief Supporting classes and functions
 * @{
 */

/**
 * \addtogroup networking Networking Utilities
 * \brief Utility classes and functions for network operations
 * \ingroup support
 * @{
 */

using Endpoint = std::string;

template <typename CallbackT>
struct PocBuilder;

template <typename RequestT>
Endpoint GetEndpoint(const RequestT& req);

enum class HttpMethod
{
    GET,
    POST,
    PUT,
    DELETE,
    PATCH
};

template <typename CallbackT>
class RouteManagerBase
{
  public:
    RouteManagerBase(const RouteManagerBase&) = delete;
    RouteManagerBase(RouteManagerBase&&) = delete;
    RouteManagerBase& operator=(const RouteManagerBase&) = delete;
    RouteManagerBase& operator=(RouteManagerBase&&) = delete;

    template <typename RequestT, typename ResponseT>
    void HandleRequest(
        HttpMethod method,
        const Endpoint& endpoint,
        const RequestT& request,
        ResponseT& response)
    {
        // const auto endpoint = GetEndpoint(req);
        auto method_it = callbacks_.find(method);
        if (method_it != callbacks_.end())
        {
            auto endpointIt = method_it->second.find(request);
            if (endpointIt != method_it->second.end())
            {
                auto& cb = endpointIt->second;

                std::invoke(endpointIt->second, request, response);
            }
        }
    }

  private:
    RouteManagerBase() = default;
    void AddCallback(HttpMethod method, const Endpoint& endpoint, CallbackT callback)
    {
        callbacks_[method][endpoint] = std::move(callback);
    }

    friend PocBuilder<CallbackT>;
    std::unordered_map<HttpMethod, std::unordered_map<Endpoint, CallbackT>> callbacks_;
};

template <typename CallbackT>
class PocBuilder
{
    using RMPtr = std::unique_ptr<RouteManagerBase<CallbackT>>;

    PocBuilder<CallbackT>& Add(
        HttpMethod method, const Endpoint& endpoint, CallbackT callback)
    {
        rm_ptr->AddCallback(method, endpoint, std::move(callback));
        return *this;
    }

    RMPtr Build() { return std::move(rm_ptr); }

    RMPtr rm_ptr = std::make_unique<RouteManagerBase<CallbackT>>();

    friend std::unique_ptr<RouteManagerBase<CallbackT>> Add();
};

template <typename CallbackT>
PocBuilder<CallbackT> Add(HttpMethod method, const Endpoint& endpoint, CallbackT callback)
{
    PocBuilder<CallbackT> builder;
    builder.Add(method, endpoint, std::move(callback));
    return builder;
}

/// @}
/// @}

}  // namespace game::support

#endif  // SUPPORT_NETWORKING_ROUTE_MANAGER_BASE_H
