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
  //! \namespace concepts - Defines concepts used by 'Delegate'
  /////////////////////////////////////////////////////////////////////////////////////////
  namespace concepts
  {
    /////////////////////////////////////////////////////////////////////////////////////////
    //! \struct MatchingSignature - Defines a concept requiring a matching function signature
    //! 
    //! \tparam SIG - Signature (function type) of handler function
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename SIG>
    struct MatchingSignature
    {
      template <typename U, enable_if_same_t<U,SIG,void*> = nullptr> 
      static void* test( void* );
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

    //! \alias signature_t - Define signature
    using signature_t = RET (ARGS...);

    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    /////////////////////////////////////////////////////////////////////////////////////////
    // Delegate::Delegate
    //! Create from instance method pointer
    //! 
    //! \tparam OBJ - Class containing method
    //! \tparam R - Delegate return type
    //! \tparam ...A - [optional] Delegate argument types
    //!
    //! \param[in] obj - Object instance
    //! \param[in] fn - Method pointer
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename OBJ, typename R, typename... A>
    Delegate(OBJ* obj, R (OBJ::*fn)(A...)) : base(bind(obj,fn))   
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY  -------------------------------

    // ----------------------------------- STATIC METHODS -----------------------------------
  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Delegate::bind() 
    //! Binds the handler to a function object. Also ensures it possesses the correct signature.
    //! 
    //! \tparam OBJ - Class containing method
    //! \tparam R - Delegate return type
    //! \tparam ...A - Delegate argument types
    //!
    //! \param[in] obj - Object instance
    //! \param[in] fn - Method pointer
    //! \return std::function<signature_t> - Function object encapsulating the handler
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename OBJ, typename R, typename... A>
    static std::function<signature_t>  bind(OBJ* obj, R (OBJ::*fn)(A...))
    {
      concept_check(R(A...),MatchingSignature<signature_t>);

      //! Encapsulate invoking the delegate within a lambda  (Fix for std::bind(..) bug in TDM-GCC)
      return [=](ARGS&&... args)->RET { return (obj->*fn)(args...); };
    }
                                                          
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  public:
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
