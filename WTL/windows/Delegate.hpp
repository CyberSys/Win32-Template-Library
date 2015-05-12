//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\Delegate.hpp
//! \brief Provides an event delegate
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EVENT_DELEGATE_HPP
#define WTL_EVENT_DELEGATE_HPP

#include "wtl/WTL.hpp"
#include <functional>             //!< std::function, std::bind, std::placeholders

//! \namespace wtl - Windows template library
namespace wtl
{
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias delegate_traits - Defines delegate traits
  //! 
  //! \tparam FN - Function type
  /////////////////////////////////////////////////////////////////////////////////////////
  //template <typename FN> 
  //struct delegate_traits;   /* Undefined */

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias delegate_traits - Defines delegate traits
  //! 
  //! \tparam RET - Delegate return type
  //! \tparam ...ARGS - [optional] Delegate argument types
  /////////////////////////////////////////////////////////////////////////////////////////
  //template <typename RET, typename ...ARGS> 
  //struct delegate_traits<std::function<RET (ARGS...)>>
  //{
  //  //! \var arguments - Number of arguments
  //  static constexpr uint32  arguments = sizeof...(ARGS);

  //  //! \alias result_t - Define return type
  //  using result_t = RET;

  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  //! \struct argument - Argument type accessor
  //  //! 
  //  //! \tparam IDX - Zero-based argument index
  //  /////////////////////////////////////////////////////////////////////////////////////////
  //  template <unsigned IDX>
  //  struct argument
  //  {
  //    static_assert(IDX < arguments, "Argument index out of range");

  //    //! \alias type - Define argument type
  //    using type = typename std::tuple_element<IDX, std::tuple<ARGS...>>::type;
  //  };
  //};

  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Delegate - Encapsulates an instance method functor
  //! 
  //! \tparam NUM - Number of arguments
  //! \tparam RET - Delegate return type
  //! \tparam ...ARGS - [optional] Delegate argument types
  /////////////////////////////////////////////////////////////////////////////////////////
  template <unsigned NUM, typename RET, typename ...ARGS>
  struct Delegate;
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Delegate<0> - Zero-argument instance method functor
  //! 
  //! \tparam RET - Delegate return type
  //! \tparam ...ARGS - [optional] Delegate argument types
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename RET, typename ...ARGS>
  struct Delegate<0,RET,ARGS...> : std::function<RET (ARGS...)>
  {
    //! \alias base - Define base type
    using base = std::function<RET (ARGS...)>;

    /////////////////////////////////////////////////////////////////////////////////////////
    // Delegate::Delegate
    //! Create from instance method pointer
    //! 
    //! \param[in] obj - Instance
    //! \param[in] fn - Method pointer
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename OBJ>
    Delegate(OBJ* obj, RET (OBJ::*fn)(ARGS...)) : base(std::bind(fn, obj))
    {}
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Delegate<1> - Single-argument instance method functor
  //! 
  //! \tparam RET - Delegate return type
  //! \tparam ...ARGS - [optional] Delegate argument types
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename RET, typename ...ARGS>
  struct Delegate<1,RET,ARGS...> : std::function<RET (ARGS...)>
  {
    //! \alias base - Define base type
    using base = std::function<RET (ARGS...)>;

    /////////////////////////////////////////////////////////////////////////////////////////
    // Delegate::Delegate
    //! Create from instance method pointer
    //! 
    //! \param[in] obj - Instance
    //! \param[in] fn - Method pointer
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename OBJ>
    Delegate(OBJ* obj, RET (OBJ::*fn)(ARGS...)) : base(std::bind(fn, obj, std::placeholders::_1))
    {}
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Delegate<2> - Double-argument instance method functor
  //! 
  //! \tparam RET - Delegate return type
  //! \tparam ...ARGS - [optional] Delegate argument types
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename RET, typename ...ARGS>
  struct Delegate<2,RET,ARGS...> : std::function<RET (ARGS...)>
  {
    //! \alias base - Define base type
    using base = std::function<RET (ARGS...)>;

    /////////////////////////////////////////////////////////////////////////////////////////
    // Delegate::Delegate
    //! Create from instance method pointer
    //! 
    //! \param[in] obj - Instance
    //! \param[in] fn - Method pointer
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename OBJ>
    Delegate(OBJ* obj, RET (OBJ::*fn)(ARGS...)) : base(std::bind(fn, obj, std::placeholders::_1, std::placeholders::_2))
    {}
  };

  
  /////////////////////////////////////////////////////////////////////////////////////////
  // wtl::bind_method - Creates an instance method delegate
  //! 
  //! \tparam OBJ - Instance type
  //! \tparam RET - Delegate return type
  //! \tparam ...ARGS - [optional] Delegate argument types
  //! 
  //! \param[in] obj - Instance 
  //! \param[in] fn - Method pointer
  //! \return std::function<RET(ARGS...)>* - New instance method delegate. Caller is responsible for destruction.
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename OBJ, typename RET, typename ...ARGS>
  std::function<RET(ARGS...)>*  bind_method(OBJ* obj, RET (OBJ::*fn)(ARGS...)) 
  {
    return new Delegate<sizeof...(ARGS),RET,ARGS...>(obj,fn);
  }
}

#endif // WTL_EVENT_DELEGATE_HPP
