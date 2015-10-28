//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\Event.hpp
//! \brief Provides an observeable event
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EVENT_HPP
#define WTL_EVENT_HPP

#include "wtl/WTL.hpp"
#include "wtl/casts/OpaqueCast.hpp"           //!< OpaqueCast
#include "wtl/windows/Delegate.hpp"           //!< Delegate
#include <tuple>                              //!< std::tuple
#include <utility>                            //!< std::tuple_element
#include <memory>                             //!< std::shared_ptr
#include <list>                               //!< std::list

//! \namespace wtl - Windows template library
namespace wtl 
{
  namespace
  {
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct call_proxy - Encapsulates the notification call for different return types
    //! 
    //! \tparam RET - Return type
    //! \tparam ARGS... - [optional] Argument types
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename RET, typename... ARGS>
    struct call_proxy
    {
      /////////////////////////////////////////////////////////////////////////////////////////
      // call_proxy::invoke()
      //! Raises the event, notifying each subscriber in the order in which they subscribed
      //! 
      //! \param[in] subscribers - Subscriber collection
      //! \param[in] &&... args - [optional] Event arguments
      //! \return result_t - Result of call to final subscriber. If no subscribers then a default constructed 'result_t'
      /////////////////////////////////////////////////////////////////////////////////////////
      template <typename COLLECTION>
      static RET invoke(COLLECTION& subscribers, ARGS&&... args) 
      {
        RET r(defvalue<RET>());

        // Forward (Copy construct) arguments to each subscriber
        for (auto& fn : subscribers)
          r = (*fn)(std::forward<ARGS>(args)...);

        // Return result
        return r;
      }
    };
    
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct call_proxy<void> - Encapsulates notification calls with no return
    //! 
    //! \tparam ARGS... - [optional] Argument types
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    struct call_proxy<void, ARGS...>
    {
      /////////////////////////////////////////////////////////////////////////////////////////
      // call_proxy::invoke()
      //! Raises the event, notifying each subscriber in the order in which they subscribed
      //! 
      //! \param[in] subscribers - Subscriber collection
      //! \param[in] &&... args - [optional] Event arguments
      /////////////////////////////////////////////////////////////////////////////////////////
      template <typename COLLECTION>
      static void invoke(COLLECTION& subscribers, ARGS&&... args) 
      {
        // Forward (Copy construct) arguments to each subscriber
        for (auto& fn : subscribers)
          (*fn)(std::forward<ARGS>(args)...);
      }
    };
  }

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Event - Provides an observeable event pattern with multiple subscribers
  //! 
  //! \tparam RET - [optional] Handler function return type (If unspecified, no return)
  //! \tparam SIG - [optional] Handler function signature (If unspecified, no arguments)
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename RET = void, typename... ARGS>
  struct Event 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \alias argument_t - Delegate argument type accessor
    //!
    //! \tparam IDX - Zero-based argument index
    /////////////////////////////////////////////////////////////////////////////////////////
    template <unsigned IDX>
    using argument_t = typename std::tuple_element<IDX, std::tuple<ARGS...>>::type;
  
    //! \alias delegate_t - Define delegate type
    using delegate_t = Delegate<RET,ARGS...>;
    
    //! \alias result_t - Define delegate return type
    using result_t = RET;

    //! \alias signature_t - Define delegate signature
    using signature_t = RET (ARGS...);
  
    //! \var arguments - Number of arguments
    static constexpr uint32_t  arguments = sizeof...(ARGS);

  protected:
    //! \alias storage_t - Define delegate storage type
    using storage_t = std::shared_ptr<delegate_t>;

    //! \alias collection_t - Define delegate collection type
    using collection_t = std::list<storage_t>;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    collection_t  Subscribers;          //!< Subscribers collection (delegates to handler functions)

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Event::Event
    //! Create event with no subscribers
    /////////////////////////////////////////////////////////////////////////////////////////
    Event() 
    {}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Event::~Event
    //! Can be polymorphic
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~Event() 
    {}
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Event::empty() const
    //! Query whether event has any subscribers
    //!
    //! \return bool - True iff event has no subscribers
    //////////////////////////////////////////////////////////////////////////////////////////
    bool empty() const
    {
      return Subscribers.empty();
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Event::clear
    //! Removes all subscribers 
    /////////////////////////////////////////////////////////////////////////////////////////
    void clear()
    {
      Subscribers.clear();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Event::raise()
    //! Raises the event, notifying each subscriber in the order in which they subscribed
    //! 
    //! \param[in] &&... args - [optional] Event arguments
    //! \return result_t - [Returns value] Result of call to final subscriber. If no subscribers then a default constructed 'result_t'
    //!                    [Returns void] Nothing
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... FN_ARGS>
    result_t raise(FN_ARGS&&... args) 
    {
      using call_proxy_t = call_proxy<result_t,FN_ARGS...>;

      // Forward (copy) arguments to each subscriber, and capture return value (iff function signature has a return type)
      return call_proxy_t::invoke(Subscribers, std::forward<FN_ARGS>(args)...);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Event::operator +=
    //! Adds a subscriber to the collection. Also ensures it possesses the correct signature.
    //! 
    //! \param[in] *ptr - Pointer to subscriber (Transfers ownership to the event)
    //! \return LPARAM - Unique subscriber identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename R, typename... A>
    LPARAM operator += (Delegate<R,A...>* ptr) 
    {
      static_assert(std::is_same<signature_t,R(A...)>::value, "Unable to add subscriber to event - Incorrect delegate signature");

      Subscribers.emplace_back(ptr);
      return opaque_cast(*ptr);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Event::operator -=
    //! Removes a subscriber from the collection
    //! 
    //! \param[in] cookie - Unique subscriber identifier
    /////////////////////////////////////////////////////////////////////////////////////////
    void operator -= (LPARAM cookie) 
    {
      auto findByAddress = [cookie] (const storage_t& ptr) { return ptr.get() == opaque_cast<delegate_t>(cookie); };

      // Remove by delegate address
      Subscribers.remove_if( findByAddress );
    }
  };


  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias handler_t - Defines the handler type for any event
  //! 
  //! \tparam EVENT - Any event type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename EVENT>
  using handler_t = typename EVENT::delegate_t;
        
} // namespace wtl

#endif // WTL_EVENT_HPP
