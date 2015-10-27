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
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    /////////////////////////////////////////////////////////////////////////////////////////
    // Delegate::Delegate
    //! Create from instance method pointer
    //! 
    //! \param[in] obj - Instance
    //! \param[in] fn - Method pointer
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename OBJ>
    Delegate(OBJ* obj, RET (OBJ::*fn)(ARGS...)) : base( [=](ARGS&&... args)->RET {          
                                                          return (obj->*fn)(args...); } )   //!< Invoke via lambda
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY  -------------------------------

    // ----------------------------------- STATIC METHODS -----------------------------------
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Delegate::operator() const
    //! Execute delegate
    //! 
    //! \param[in,out] &&arg - Argument
    //! \return RET - Return value
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... VARGS>
    RET operator()(VARGS&&... args) const
    {
      return base::operator()(std::forward<VARGS>(args)...);
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  };


} // namespace wtl

#endif // WTL_EVENT_DELEGATE_HPP
