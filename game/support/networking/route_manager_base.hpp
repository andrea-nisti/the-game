#ifndef SUPPORT_NETWORKING_ROUTE_MANAGER_BASE_H
#define SUPPORT_NETWORKING_ROUTE_MANAGER_BASE_H

#include <functional>
#include <memory>
#include <utility>

#include "support/networking/http/common.hpp"
#include "support/networking/websocket/ws_handler.hpp"

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
 * This class is used to map HTTP paths and methods to callbacks, which are
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
        std::unordered_map<HttpMethod, std::unordered_map<Path, CallbackT>>;
    using WSHandlerMap = std::unordered_map<Path, WSHandler>;
    friend class RouteManagerBuilder<RequestT, ResponseT>;

    RouteManagerBase() = default;
    RouteManagerBase(const RouteManagerBase&) = delete;
    RouteManagerBase(RouteManagerBase&&) = delete;
    RouteManagerBase& operator=(const RouteManagerBase&) = delete;
    RouteManagerBase& operator=(RouteManagerBase&&) = delete;

    std::optional<std::reference_wrapper<WSHandler>> GetWSHandler(
        const HttpMethod method, const Path& path)
    {
        if (method == HttpMethod::GET)
        {
            auto it = ws_handlers_.find(path);
            if (it != ws_handlers_.end())
            {
                return it->second;
            }
        }
        return {};
    }

    virtual std::optional<std::reference_wrapper<CallbackT>> GetCallback(
        const HttpMethod method, const Path& path)
    {
        auto method_it = callbacks_.find(method);
        if (method_it != callbacks_.end())
        {
            auto pathIt = method_it->second.find(path);
            if (pathIt != method_it->second.end())
            {
                return pathIt->second;
            }
        }
        return {};
    }

  protected:
    /**
     * @brief Add a callback to the callback map.
     *
     * This is a user-overridable function that allows the user to specify how to add a
     * callback to the callback map. The default implementation simply adds the callback
     * to the map.
     *
     * @param callbacks The callback map to which the callback should be added.
     * @param method The HTTP method to which this request corresponds.
     * @param path The path to which this request corresponds.
     * @param callback The callback to add.
     */
    virtual void AddCallback(
        CallbackMap& callbacks,
        const HttpMethod method,
        const Path& path,
        CallbackT&& callback)
    {
        callbacks[method].emplace(path, std::move(callback));
    }

  private:
    void AddCallbackInternal(
        const HttpMethod method, const Path& path, CallbackT&& callback)
    {
        AddCallback(callbacks_, method, path, std::move(callback));
    }

    void AddWSCallbackInternal(const Path& path, WSHandler&& handler)
    {
        ws_handlers_.emplace(path, std::move(handler));
    }

    CallbackMap callbacks_;
    WSHandlerMap ws_handlers_;
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
     * @param path The path to which this callback is added.
     * @param callback The callback to add.
     *
     * @return The instance of RouteManagerBuilder so that this function can be
     *         chained.
     */
    RouteManagerBuilder<RequestT, ResponseT>& Add(
        const HttpMethod method, const Path& path, CallbackT callback)
    {
        rm_ptr->AddCallbackInternal(method, path, std::move(callback));
        return *this;
    }

    RouteManagerBuilder<RequestT, ResponseT>& AddWS(const Path& path, WSHandler handler)
    {
        rm_ptr->AddWSCallbackInternal(path, std::move(handler));
        return *this;
    }

  private:
    RMPtr rm_ptr = std::make_unique<RouteManagerBase<RequestT, ResponseT>>();
};

/// @}
/// @}

}  // namespace game::support

#endif  // SUPPORT_NETWORKING_ROUTE_MANAGER_BASE_H
