#ifndef SUPPORT_NETWORKING_ROUTE_MANAGER_BASE_H
#define SUPPORT_NETWORKING_ROUTE_MANAGER_BASE_H

#include <functional>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <utility>

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
class PocBuilder;

template <typename RequestT>
Endpoint GetEndpoint(const RequestT& req);

enum class HttpMethod
{
    UNKNOWN,
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
    RouteManagerBase() = default;
    RouteManagerBase(const RouteManagerBase&) = delete;
    RouteManagerBase(RouteManagerBase&&) = delete;
    RouteManagerBase& operator=(const RouteManagerBase&) = delete;
    RouteManagerBase& operator=(RouteManagerBase&&) = delete;
    friend class PocBuilder<CallbackT>;

    template <typename RequestT, typename ResponseT>
    void HandleRequest(
        HttpMethod method,
        const Endpoint& endpoint,
        const RequestT& request,
        ResponseT& response)
    {
        const auto cb = GetCallback(method, endpoint);
        if (cb)
        {
            std::invoke(*cb, request, response);
        }
    }

  protected:
    virtual CallbackT* GetCallback(HttpMethod method, const Endpoint& endpoint)
    {
        auto method_it = callbacks_.find(method);
        if (method_it != callbacks_.end())
        {
            auto endpointIt = method_it->second.find(endpoint);
            if (endpointIt != method_it->second.end())
            {
                return &endpointIt->second;
            }
        }
        return nullptr;
    }

  private:
    void AddCallback(HttpMethod method, const Endpoint& endpoint, CallbackT&& callback)
    {
        callbacks_[method].emplace(endpoint, std::move(callback));
    }

    std::unordered_map<HttpMethod, std::unordered_map<Endpoint, CallbackT>> callbacks_;
};

template <typename CallbackT>
class PocBuilder
{
  public:
    using RMPtr = std::unique_ptr<RouteManagerBase<CallbackT>>;

    RMPtr Build() { return std::move(rm_ptr); }

    PocBuilder<CallbackT>& Add(
        HttpMethod method, const Endpoint& endpoint, CallbackT&& callback)
    {
        rm_ptr->AddCallback(method, endpoint, std::move(callback));
        return *this;
    }

  private:
    RMPtr rm_ptr = std::make_unique<RouteManagerBase<CallbackT>>();
};

template <typename CallbackT>
PocBuilder<CallbackT> Add(
    HttpMethod method, const Endpoint& endpoint, CallbackT&& callback)
{
    PocBuilder<CallbackT> builder;
    builder.Add(method, endpoint, std::move(callback));
    return builder;
}

/// @}
/// @}

}  // namespace game::support

#endif  // SUPPORT_NETWORKING_ROUTE_MANAGER_BASE_H
