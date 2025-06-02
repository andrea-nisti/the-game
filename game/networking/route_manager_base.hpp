#ifndef NETWORKING_ROUTE_MANAGER_BASE
#define NETWORKING_ROUTE_MANAGER_BASE

#include <functional>
#include <memory>
#include <optional>
#include <utility>

#include "networking/http/common.hpp"
#include "networking/websocket/ws_handler.hpp"

namespace game::support {

/**
 * \addtogroup networking Networking Utilities
 * \brief Utility classes and functions for network operations
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
    using CallbackT = std::function<void(
        const RequestT&, const std::optional<Params> params, ResponseT&)>;
    using CallbackMap =
        std::unordered_map<HttpMethod, std::unordered_map<Path, CallbackT>>;
    using WSHandlerMap = std::unordered_map<Path, WSHandler>;
    friend class RouteManagerBuilder<RequestT, ResponseT>;

    RouteManagerBase(CallbackMap callbacks, WSHandlerMap ws_handlers)
        : callbacks_(std::move(callbacks)), ws_handlers_(std::move(ws_handlers))
    {}
    RouteManagerBase() = default;
    RouteManagerBase(const RouteManagerBase&) = delete;
    RouteManagerBase(RouteManagerBase&&) = delete;
    RouteManagerBase& operator=(const RouteManagerBase&) = delete;
    RouteManagerBase& operator=(RouteManagerBase&&) = delete;
    virtual ~RouteManagerBase() = default;

    auto GetWSHandler(const HttpMethod method, const Path& path)
        -> std::optional<std::reference_wrapper<const WSHandler>>
    {
        if (method == HttpMethod::GET)
        {
            auto it = ws_handlers_.find(path);
            if (it != ws_handlers_.end())
            {
                return std::cref(it->second);
            }
        }
        return {};
    }

    virtual auto GetCallback(const HttpMethod method, const Path& path)
        -> std::optional<std::reference_wrapper<CallbackT>>
    {
        const auto method_it = callbacks_.find(method);
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
    using CallbackT = typename RouteManagerBase<RequestT, ResponseT>::CallbackT;
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
    auto Build() -> RMPtr { return std::move(rm_ptr_); }

    /**
     * @brief Creates a new RouteManagerBuilder with a RouteManagerBase instance built
     * using the BuilderType.
     *
     * @param args Parameters to be forwarded to the BuilderType constructor.
     * @return A new RouteManagerBuilder with the RouteManagerBase instance built
     * using the BuilderType.
     */
    template <typename BuilderType, typename... Args>
    auto WithRouteManagerType(Args&&... args) -> RouteManagerBuilder<RequestT, ResponseT>&
    {
        rm_ptr_ = std::make_unique<BuilderType>(std::forward<Args>(args)...);
        return *this;
    }

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
    auto Add(const HttpMethod method, const Path& path, CallbackT callback)
        -> RouteManagerBuilder<RequestT, ResponseT>&
    {
        rm_ptr_->AddCallbackInternal(method, path, std::move(callback));
        return *this;
    }

    auto AddWebsocket(const Path& path, WSHandler&& handler)
        -> RouteManagerBuilder<RequestT, ResponseT>&
    {
        rm_ptr_->AddWSCallbackInternal(path, std::move(handler));
        return *this;
    }

  private:
    RMPtr rm_ptr_ = std::make_unique<RouteManagerBase<RequestT, ResponseT>>();
};

/// @}

}  // namespace game::support

#endif  // NETWORKING_ROUTE_MANAGER_BASE
