#ifndef SUPPORT_NETWORKING_ROUTE_MANAGER_BASE_H
#define SUPPORT_NETWORKING_ROUTE_MANAGER_BASE_H

#include <functional>
#include <memory>
#include <utility>

#include "common.hpp"

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

template <typename RequestT, typename ResponseT>
class RouteManagerBuilder;

/**
 * \brief Base class for handling HTTP requests.
 *
 * This class is used to map HTTP endpoints and methods to callbacks, which are
 * then called when an HTTP request is handled.
 *
 * The user of this class can configure this class by extending and implementing the
 * virtual functions. The user can then call HandleRequest() to handle an HTTP request.
 *
 * @tparam RequestT The type of the request.
 * @tparam ResponseT The type of the response.
 */
template <typename RequestT, typename ResponseT>
class RouteManagerBase
{
  public:
    using CallbackT = std::function<void(const RequestT&, ResponseT&)>;
    using CallbackMap =
        std::unordered_map<HttpMethod, std::unordered_map<Endpoint, CallbackT>>;
    friend class RouteManagerBuilder<RequestT, ResponseT>;

    RouteManagerBase() = default;
    RouteManagerBase(const RouteManagerBase&) = delete;
    RouteManagerBase(RouteManagerBase&&) = delete;
    RouteManagerBase& operator=(const RouteManagerBase&) = delete;
    RouteManagerBase& operator=(RouteManagerBase&&) = delete;

    /**
     * @brief Handle an HTTP request.
     *
     * This function takes an HTTP request and a response, and calls the appropriate
     * callback function for the given HTTP method and endpoint.
     *
     * @param method The HTTP method to which this request corresponds.
     * @param endpoint The endpoint to which this request corresponds.
     * @param request The request.
     * @param response The response.
     */
    void HandleRequest(
        const HttpMethod method,
        const Endpoint& endpoint,
        const RequestT& request,
        ResponseT& response)
    {
        const auto cbs = GetCallbacks(method, endpoint);
        if (not cbs.empty())
        {
            for (const auto& cb : cbs)
                std::invoke(cb, request, response);
        }
    }

  protected:
    /**
     * @brief Get the callbacks for the given HTTP method and endpoint.
     *
     * This function will return a vector of callbacks for the given HTTP method and
     * endpoint. This function is virtual so that you can implement a more
     * sophisticated search like regex pattern matching.
     *
     * Example:
     * @code
     * class MyRouteManager : public RouteManagerBase<RequestT, ResponseT>
     * {
     *  public:
     *   using RouteManagerBase::RouteManagerBase;
     *   std::vector<std::reference_wrapper<CallbackT>> GetCallback(
     *       const HttpMethod method, const Endpoint& endpoint) override
     *   {
     *     std::vector<std::reference_wrapper<CallbackT>> callbacks;
     *     for (const auto& cb : callbacks_)
     *     {
     *       if (std::regex_search(cb.first, std::regex(endpoint)))
     *       {
     *         callbacks.push_back(cb.second);
     *       }
     *     }
     *     return callbacks;
     *   }
     * };
     * @endcode
     *
     * @param method The HTTP method to which this request corresponds.
     * @param endpoint The endpoint to which this request corresponds.
     * @return A vector of callbacks for the given HTTP method and endpoint.
     */
    virtual std::vector<std::reference_wrapper<CallbackT>> GetCallbacks(
        const HttpMethod method, const Endpoint& endpoint)
    {
        auto method_it = callbacks_.find(method);
        if (method_it != callbacks_.end())
        {
            auto endpointIt = method_it->second.find(endpoint);
            if (endpointIt != method_it->second.end())
            {
                return {endpointIt->second};
            }
        }
        return {};
    }

    /**
     * @brief Add a callback to the callback map.
     *
     * This is a user-overridable function that allows the user to specify how to add a
     * callback to the callback map. The default implementation simply adds the callback
     * to the map.
     *
     * @param callbacks The callback map to which the callback should be added.
     * @param method The HTTP method to which this request corresponds.
     * @param endpoint The endpoint to which this request corresponds.
     * @param callback The callback to add.
     */
    virtual void AddCallback(
        CallbackMap& callbacks,
        const HttpMethod method,
        const Endpoint& endpoint,
        CallbackT&& callback)
    {
        callbacks[method].emplace(endpoint, std::move(callback));
    }

  private:
    virtual void AddCallbackInternal(
        const HttpMethod method, const Endpoint& endpoint, CallbackT&& callback)
    {
        AddCallback(callbacks_, method, endpoint, std::move(callback));
    }
    CallbackMap callbacks_;
};

/**
 * @brief Builder class for RouteManagerBase.
 *
 * This class is used to create a RouteManagerBase instance and add callbacks to
 * it. The instance is created when the user calls Build() and the callbacks are
 * added when the user calls Add().
 *
 * @tparam RequestT The type of the request.
 * @tparam ResponseT The type of the response.
 */
template <typename RequestT, typename ResponseT>
class RouteManagerBuilder
{
  public:
    using CallbackT = std::function<void(const RequestT&, ResponseT&)>;
    using RMPtr = std::unique_ptr<RouteManagerBase<RequestT, ResponseT>>;

    /**
     * @brief Build the RouteManagerBase instance.
     *
     * This function will move the internal RouteManagerBase instance and return
     * it as a unique pointer of type
     * @ref game::support::RouteManagerBase<RequestT,
     * ResponseT>>;
     *
     * @return The created RouteManagerBase instance.
     */

    RMPtr Build() { return std::move(rm_ptr); }

    /**
     * @brief Add a callback to the RouteManagerBase instance.
     *
     * This function will add a callback to the RouteManagerBase instance.
     *
     * @param method The HTTP method to which this callback is added.
     * @param endpoint The endpoint to which this callback is added.
     * @param callback The callback to add.
     *
     * @return The instance of RouteManagerBuilder so that this function can be
     *         chained.
     */
    RouteManagerBuilder<RequestT, ResponseT>& Add(
        const HttpMethod method, const Endpoint& endpoint, CallbackT callback)
    {
        rm_ptr->AddCallbackInternal(method, endpoint, std::move(callback));
        return *this;
    }

  private:
    RMPtr rm_ptr = std::make_unique<RouteManagerBase<RequestT, ResponseT>>();
};

/// @}
/// @}

}  // namespace game::support

#endif  // SUPPORT_NETWORKING_ROUTE_MANAGER_BASE_H
