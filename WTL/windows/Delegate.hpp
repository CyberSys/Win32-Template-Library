////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\Delegate.hpp
//! \brief Provides an event delegate
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_EVENT_DELEGATE_HPP
#define WTL_EVENT_DELEGATE_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  
  ///////////////////////////////////////////////////////////////////////////////
  //! \alias delegate_traits - Defines delegate traits
  //! 
  //! \tparam FN - Function type
  ///////////////////////////////////////////////////////////////////////////////
  template <typename FN> 
  struct delegate_traits;   /* Undefined */

  ///////////////////////////////////////////////////////////////////////////////
  //! \alias delegate_traits - Defines delegate traits
  //! 
  //! \tparam RET - Delegate return type
  //! \tparam ...ARGS - [optional] Delegate argument types
  ///////////////////////////////////////////////////////////////////////////////
  template <typename RET, typename ...ARGS> 
  struct delegate_traits<std::function<RET (ARGS...)>>
  {
    //! \var arguments - Number of arguments
    static constexpr uint32  arguments = sizeof...(ARGS);

    //! \alias result_t - Define return type
    using result_t = RET;

    ///////////////////////////////////////////////////////////////////////////////
    //! \struct argument - Argument type accessor
    //! 
    //! \tparam IDX - Zero-based argument index
    ///////////////////////////////////////////////////////////////////////////////
    template <unsigned IDX>
    struct argument
    {
      static_assert(IDX < arguments, "Argument index out of range");

      //! \alias type - Define argument type
      using type = typename std::tuple_element<IDX, std::tuple<ARGS...>>::type;
    };
  };

  /*template <typename FN>
  struct Delegate;*/

  template <unsigned NUM, typename RET, typename ...ARGS>
  struct Delegate;
  
  
  template <typename RET, typename ...ARGS>
  struct Delegate<0,RET,ARGS...> : std::function<RET (ARGS...)>
  {
    using base = std::function<RET (ARGS...)>;

    template <typename OBJ>
    Delegate(OBJ* obj, RET (*fn)(ARGS...)) : base(std::bind(fn, obj))
    {}
  };

  template <typename RET, typename ...ARGS>
  struct Delegate<1,RET,ARGS...> : std::function<RET (ARGS...)>
  {
    using base = std::function<RET (ARGS...)>;

    template <typename OBJ>
    Delegate(OBJ* obj, RET (*fn)(ARGS...)) : base(std::bind(fn, obj, std::placeholders::_1))
    {}
  };

  template <typename RET, typename ...ARGS>
  struct Delegate<2,RET,ARGS...> : std::function<RET (ARGS...)>
  {
    using base = std::function<RET (ARGS...)>;

    template <typename OBJ>
    Delegate(OBJ* obj, RET (*fn)(ARGS...)) : base(std::bind(fn, obj, std::placeholders::_1, std::placeholders::_2))
    {}
  };
  
  /*template <typename OBJ, typename RET, typename ...ARGS, unsigned COUNT = sizeof...(ARGS)>
  Delegate<RET(ARGS...),COUNT>* make_delegate(OBJ* obj, RET (*fn)(ARGS...))
  {
    return new Delegate<COUNT,RET,ARGS...>(obj, fn);
  }*/
  
  template <typename ...ARGS>
  using message_signature_t = LResult (ARGS...);
  
  template <unsigned NUM, typename OBJ, typename RET, typename ...ARGS>
  std::function<RET(ARGS...)> bind_method(OBJ* obj, RET (*fn)(ARGS...)) //-> decltype(std::bind(fn,obj));
  {
    return Delegate<NUM,RET,ARGS>(obj,fn);
  }
}

#endif // WTL_EVENT_DELEGATE_HPP
