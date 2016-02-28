//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\MsgResult.hpp
//! \brief 
//! \date 1 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MESSAGE_RESULT_HPP
#define WTL_MESSAGE_RESULT_HPP

#include <wtl/WTL.hpp>

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \enum MsgRoute - Defines how a message was routed
  /////////////////////////////////////////////////////////////////////////////////////////
  enum class MsgRoute
  {
    Handled,      //!< Window handled message
    Reflected,    //!< Window reflected command/notification to child window
    Unhandled,    //!< Window ignored message
  };
  

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct MsgResult - Encapsulates the result and routing of a windows message 
  //! 
  //! \tparam RESULT - Message result type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename RESULT>
  struct MsgResult
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = MsgResult<RESULT>;
  
    //! \alias result_t - Define result type
    using result_t = RESULT;

    // ----------------------------------- REPRESENTATION -----------------------------------
  
    MsgRoute  Route;        //!< Message routing
    result_t  Result;       //!< Message result

    // ------------------------------------ CONSTRUCTION ------------------------------------
	
    /////////////////////////////////////////////////////////////////////////////////////////
    //! MsgResult::MsgResult
    //! Create 'unhandled' result with a default value of -1
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr 
    MsgResult() noexcept : Route(MsgRoute::Unhandled), Result(-1)
    {}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    //! MsgResult::MsgResult
    //! This overload only exists to enable handles to use a clearer syntax to indicate unhandled messages 
    //! 
    //! \param[in] - Ignored
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr 
    MsgResult(MsgRoute) noexcept : Route(MsgRoute::Unhandled), Result(-1)
    {}
    
    /////////////////////////////////////////////////////////////////////////////////////////
    //! MsgResult::MsgResult
    //! Create 'handled' result from a value
    //! 
    //! \param[in] res - Result value
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr 
    MsgResult(result_t res) noexcept : Route(MsgRoute::Handled), Result(res)
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    //! MsgResult::MsgResult
    //! Create result with custom routing and value
    //! 
    //! \param[in] route - Routing
    //! \param[in] res - Result value
    /////////////////////////////////////////////////////////////////////////////////////////
    constexpr 
    MsgResult(MsgRoute route, result_t res) noexcept : Route(route), Result(res)
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    CONSTEXPR_COPY_CTOR(MsgResult);      //!< Can be deep copied at compile-time
    CONSTEXPR_MOVE_CTOR(MsgResult);      //!< Can be moved at compile-time
    ENABLE_COPY_ASSIGN(MsgResult);       //!< Can be assigned
    ENABLE_MOVE_ASSIGN(MsgResult);       //!< Can be move-assigned
    DISABLE_POLY(MsgResult);             //!< Cannot be polymorphic

    /////////////////////////////////////////////////////////////////////////////////////////
    //! MsgResult::operator=
    //! Assign custom result  (Implicitly changes routing to 'Handled')
    //! 
    //! \param[in] res - Result value
    /////////////////////////////////////////////////////////////////////////////////////////
    type& operator=(result_t res) noexcept
    {
      Result = res;
      Route = MsgRoute::Handled;
      return *this;
    }

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    //! MsgResult::operator== const
    //! Compare routing and result against another instance of equal type
    //! 
    //! \param[in] const& r - Another instance
    //! \return bool - True iff equal result and routing
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator == (const type& r) const noexcept
    {
      return Route == r.Route && Result == r.Result;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    //! MsgResult::operator!= const
    //! Compare routing and result against another instance of equal type
    //! 
    //! \param[in] const& r - Another instance
    //! \return bool - True iff unequal result or routing
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator != (const type& r) const noexcept
    {
      return Route != r.Route || Result != r.Result;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    //! MsgResult::operator== const
    //! Compare routing 
    //! 
    //! \param[in] r - Routing to query
    //! \return bool - True iff equal routing
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator == (MsgRoute r) const noexcept
    {
      return Route == r;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    //! MsgResult::operator!= const
    //! Compare routing 
    //! 
    //! \param[in] r - Routing to query
    //! \return bool - True iff equal routing
    /////////////////////////////////////////////////////////////////////////////////////////
    bool operator != (MsgRoute r) const noexcept
    {
      return Route != r;
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
  };

  /////////////////////////////////////////////////////////////////////////////////////////
  //! \alias LResult - Default window message return type
  /////////////////////////////////////////////////////////////////////////////////////////
  using LResult = MsgResult<::LRESULT>;
  


} //namespace wtl
#endif // WTL_MESSAGE_RESULT_HPP

