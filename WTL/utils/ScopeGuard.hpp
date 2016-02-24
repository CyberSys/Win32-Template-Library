//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\ScopeGuard.hpp
//! \brief Provides basic version of Andrei Alexandrescu's ScopeGuard idiom
//! \date 24 February 2016
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SCOPE_GUARD_HPP
#define WTL_SCOPE_GUARD_HPP

#include <wtl/WTL.hpp>
#include <functional>

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct BasicScopeGuard - Encapsulates a piece of logic executed upon normal cleanup or stack unwinding
  /////////////////////////////////////////////////////////////////////////////////////////
  struct BasicScopeGuard
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = BasicScopeGuard;
	
  private:
    //! \alias signature_t - Define signature required by all functors
    using signature_t = void ();

    // ----------------------------------- REPRESENTATION -----------------------------------
  private:
    std::function<signature_t> OnExit;    //!< Logic executed upon normal/erroneous exit

    // ---------------------------------- CREATE & DESTROY ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // BasicScopeGuard::BasicScopeGuard
    //! Construct from on-exit functor
    //!
    //! \param[in] fn - Function object providing on-exit logic
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename Func>
    BasicScopeGuard(Func fn) : OnExit(fn) 
    {}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // BasicScopeGuard::~BasicScopeGuard
    //! Executes on-exit logic
    /////////////////////////////////////////////////////////////////////////////////////////
    ~BasicScopeGuard()
    {
      OnExit();
    }

    // ------------------------------------ COPY & MOVE -------------------------------------
  public:
    ENABLE_COPY(type);        //!< Enable deep copy
    ENABLE_MOVE(type);        //!< Enable move
		
    // ----------------------------------- STATIC METHODS -----------------------------------
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };
}



#endif // WTL_SCOPE_GUARD_HPP


