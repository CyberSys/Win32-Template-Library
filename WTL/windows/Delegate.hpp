//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\windows\Delegate.hpp
//! \brief Provides an event delegate
//! \date 25 October 2015
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
  //! \namespace <anon> - Utility
  /////////////////////////////////////////////////////////////////////////////////////////
  namespace
  {
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct method_binder - Binds methods of different arities to polymorphic function wrappers
    //! 
    //! \tparam N - Number of arguments (Excluding the implicit object)
    /////////////////////////////////////////////////////////////////////////////////////////
    template <uint32_t N>
    struct binder;
    
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct method_binder<0> - Binds parameterless methods to polymorphic function wrappers
    /////////////////////////////////////////////////////////////////////////////////////////
    template <>
    struct binder<0>
    {
      /////////////////////////////////////////////////////////////////////////////////////////
      // method_binder<0>::bind 
      //! Binds a parameterless method
      //! 
      //! \param[in] *obj - Implied object
      //! \param[in] *fn - Method pointer
      //! \return std::function<RET ()> - Function wrapper
      /////////////////////////////////////////////////////////////////////////////////////////
      template <typename OBJ, typename RET>
      static std::function<RET ()>  bind(OBJ* obj, RET (OBJ::*fn)())
      {
        return std::bind(fn, obj);
      }
    };

    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct method_binder<1> - Binds single parameter methods to polymorphic function wrappers
    /////////////////////////////////////////////////////////////////////////////////////////
    template <>
    struct binder<1>
    {
      /////////////////////////////////////////////////////////////////////////////////////////
      // method_binder<1>::bind 
      //! Binds a single parameter method
      //! 
      //! \param[in] *obj - Implied object
      //! \param[in] *fn - Method pointer
      //! \return std::function<RET (ARGS...)> - Function wrapper
      /////////////////////////////////////////////////////////////////////////////////////////
      template <typename OBJ, typename RET, typename... ARGS>
      static std::function<RET (ARGS...)>  bind(OBJ* obj, RET (OBJ::*fn)(ARGS...))
      {
        return std::bind(fn, obj, std::placeholders::_1);
      }
    };

    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct method_binder<2> - Binds dual parameter methods to polymorphic function wrappers
    /////////////////////////////////////////////////////////////////////////////////////////
    template <>
    struct binder<2>
    {
      /////////////////////////////////////////////////////////////////////////////////////////
      // method_binder<2>::bind 
      //! Binds a dual parameter method
      //! 
      //! \param[in] *obj - Implied object
      //! \param[in] *fn - Method pointer
      //! \return std::function<RET (ARGS...)> - Function wrapper
      /////////////////////////////////////////////////////////////////////////////////////////
      template <typename OBJ, typename RET, typename... ARGS>
      static std::function<RET (ARGS...)>  bind(OBJ* obj, RET (OBJ::*fn)(ARGS...))
      {
        return std::bind(fn, obj, std::placeholders::_1, std::placeholders::_2);
      }
    };
  }
  
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Delegate - Provides delegate types for class methods with differing numbers of arguments
  //! 
  //! \tparam RET - Delegate return type
  //! \tparam ...ARGS - [optional] Delegate argument types
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename RET, typename... ARGS>
  struct Delegate : std::function<RET (ARGS...)>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = Delegate<RET,ARGS...>;
  
    //! \alias base - Define base type
    using base = std::function<RET (ARGS...)>;

    // ----------------------------------- REPRESENTATION -----------------------------------
  
    // ------------------------------------ CONSTRUCTION --------------------------------------
	
    /////////////////////////////////////////////////////////////////////////////////////////
    // Delegate::Delegate
    //! Create from instance method pointer
    //! 
    //! \param[in] obj - Instance
    //! \param[in] fn - Method pointer
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename OBJ>
    Delegate(OBJ* obj, RET (OBJ::*fn)(ARGS...)) : base( binder<sizeof...(ARGS)>::bind(obj, fn) )
    {}
    
    // -------------------------------- COPYING & DESTRUCTION -------------------------------

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Delegate::operator() const
    //! Execute delegate
    //! 
    //! \param[in,out] &&arg - Argument
    //! \return RET - Return value
    /////////////////////////////////////////////////////////////////////////////////////////
    //template <typename... ARGS2>
    RET operator()(ARGS&&... args) const
    {
      return base::operator()(std::forward<ARGS>(args)...);
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };


} // namespace wtl

#endif // WTL_EVENT_DELEGATE_HPP
