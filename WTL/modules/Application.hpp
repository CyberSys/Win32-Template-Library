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
#include "wtl/platform/DateTime.hpp"                //!< DateTime
#include "wtl/platform/SystemVersion.hpp"           //!< SystemVersion

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
    DISABLE_COPY(Application);       //!< Cannot be copied
    ENABLE_MOVE(Application);        //!< Can be moved
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Application::name const 
    //! Get the application name
    //!
    //! \return const char_t* - Full application name
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual const char_t* name() const = 0;
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Application::version const 
    //! Get the application version
    //!
    //! \return const char_t* - Version string
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual const char_t* version() const = 0;

    // ----------------------------------- MUTATOR METHODS ----------------------------------

    /////////////////////////////////////////////////////////////////////////////////////////
    // Application::run
    //! Executes the application
    //! 
    //! \param[in] mode - Initial display mode
    //! \return int32 - Return code
    /////////////////////////////////////////////////////////////////////////////////////////
    int32  run(ShowWindowFlags mode = ShowWindowFlags::ShowNormal) override
    {
      // Feedback: AppName + Operating System
      cdebug << Cons::Cyan << "--------------------------------------------------------------------------" << endl
             << (Cons::Cyan|Cons::Bold) << "\t\t\t" << this->name() << " " << this->version() << Cons::Yellow << LongDateString<ENC>() << endl
             << Cons::Cyan << "--------------------------------------------------------------------------" << endl 
             << endl
         
             // Operating System
             << Cons::Bold << "Operating System: " << Cons::Yellow << SystemVersion<ENC>().fullname() << endl
             << Cons::Bold << "Module Path: "      << Cons::Yellow << this->path<ENC>()               << endl;

      // Execute
      return this->run(mode);
    }

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
  };
}

#endif // WTL_APPLICATION_HPP
