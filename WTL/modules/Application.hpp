//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\modules\Application.hpp
//! \brief Encapsulates a program
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_APPLICATION_HPP
#define WTL_APPLICATION_HPP

#include "wtl/WTL.hpp"
//#include "wtl/traits/ApplicationTraits.hpp"
#include "wtl/modules/Module.hpp"                   //!< Module
#include "wtl/threads/MessagePump.hpp"              //!< MessagePump

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Application - Encapsulates a program
  //! 
  //! \tparam ENC - Program character encoding 
  //! \tparam WINDOW - Main window type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename WINDOW>
  struct Application : Module,
                       MessagePump<ENC,WINDOW>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias module_base - Define module base type
    using module_base = Module;
    
    //! \alias msgpump_base - Define message pump base type
    using msgpump_base = MessagePump<ENC,WINDOW>;

    //! \alias char_t - Inherit program character type
    using char_t = typename msgpump_base::char_t;

    //! \alias resource_t - Inherit program resource id type
    using resource_t = typename msgpump_base::resource_t;
    
    //! \var encoding - Inherit program character encoding
    static constexpr Encoding encoding = msgpump_base::encoding;

    // ------------------------------------ CONSTRUCTION ------------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Application::Application
    //! Create application from handle supplied by WinMain(..)
    //!
    //! \param[in] app - Application handle
    /////////////////////////////////////////////////////////////////////////////////////////
    Application(::HMODULE app) : module_base(app), 
                                 msgpump_base(app)
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    DISABLE_COPY(Module);       //!< Cannot be copied
    ENABLE_MOVE(Module);        //!< Can be moved
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
  };
}

#endif // WTL_APPLICATION_HPP
