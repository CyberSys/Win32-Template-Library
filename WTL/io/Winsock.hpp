//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\io\Winsock.hpp
//! \brief Handles the init/cleanup of the winsock library
//! \date 4 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#include <wtl/WTL.hpp>
#include <wtl/casts/EnumCast.hpp>               //!< enum_cast
#include <wtl/platform/SocketFlags.hpp>         //!< WinsockVersion
#include <wtl/utils/Exception.hpp>              //!< socket_exception
#include <WinSock2.h>                           //!< WSAStartup,WSACleanup

//! \namespace wtl - Windows template library
namespace wtl
{
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct WinsockLibrary - Initializes and frees the winsock library upon program startup/exit
  //////////////////////////////////////////////////////////////////////////////////////////
  struct WinsockLibrary
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
   
    // ----------------------------------- REPRESENTATION -----------------------------------
  private:
    ::WSADATA  Properties = {};

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // WinsockLibrary::WinsockLibrary
    //! Initializes the winsock library
    //! 
    //! \param[in] ver - Desired version
    //!
    //! \throw wtl::socket_error - Failed to initialize library
    //////////////////////////////////////////////////////////////////////////////////////////
    WinsockLibrary(WinsockVersion ver = WinsockVersion::v2_2)
    {
      // Initialize the library
      if (::WSAStartup(enum_cast(ver), &Properties) != ERROR_SUCCESS)      
        throw socket_error(HERE, "Unable to initialize Winsock library");

      // Verify version
      if (enum_cast<WinsockVersion>(Properties.wVersion) != ver)      
        throw socket_error(HERE, "Unable to initialize desired version of Winsock library");
    }
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // WinsockLibrary::~WinsockLibrary
    //! Cleanup the winsock library
    //////////////////////////////////////////////////////////////////////////////////////////
    ~WinsockLibrary()
    {
      ::WSACleanup();
    }

		DISABLE_COPY(WinsockLibrary);			//!< Singleton type
		DISABLE_MOVE(WinsockLibrary);			//!< Singleton type
		
    // ----------------------------------- STATIC METHODS -----------------------------------
    
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------

  };


  //////////////////////////////////////////////////////////////////////////////////////////
  //! \var wtl::winsock - Initializes and frees the winsock library upon program startup/exit
  //////////////////////////////////////////////////////////////////////////////////////////
  //WinsockLibrary winsock;

} //namespace wtl

