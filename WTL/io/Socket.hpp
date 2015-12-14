//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\io\Socket.hpp
//! \brief 
//! \date 4 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SOCKET_HPP
#define WTL_SOCKET_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Handle.hpp>                 //!< Handle
#include <wtl/utils/String.hpp>                 //!< String
#include <wtl/traits/SocketTraits.hpp>          //!< handle_alloc<::SOCKET>
#include <wtl/platform/SocketFlags.hpp>         //!< AddressFamily,SocketType,SocketProtocol
#include <wtl/windows/Window.hpp>               //!< Window

//! \namespace wtl - Windows template library
namespace wtl
{

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Socket - Encapsulates a windows socket
  //! 
  //! \tparam FAMILY - [optional] Address family, defaults to IPv4
  //////////////////////////////////////////////////////////////////////////////////////////
  template <AddressFamily FAMILY = AddressFamily::IPv4>
  struct Socket 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
    
    //! \alias type - Define own type
    using type = Socket<FAMILY>;

    //! \ifnot NTDDI_VISTA - Define SOCKADDR_IN6 as sentinel type when compiling on less than Windows Vista
#if (NTDDI_VERSION < NTDDI_VISTA)
    using SOCKADDR_IN6 = void;
#endif
    //! \alias address_t - Define address structure type
	  using address_t = std::conditional_t<FAMILY != AddressFamily::IPv6, SOCKADDR_IN, SOCKADDR_IN6>;

    //! \alias family - Define address family
	  static constexpr AddressFamily family = FAMILY;

    // ----------------------------------- REPRESENTATION -----------------------------------
  private:
	  HSocket  Handle;	      //!< Socket handle
    bool     Async;         //!< Whether socket is asychronous
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // Socket::Socket
    //! Create windows socket
    //! 
    //! \param[in] t - Socket type
    //! \param[in] p - Socket protocol
    //! 
    //! \throw wtl::socket_error - Failed to create socket handle
    //////////////////////////////////////////////////////////////////////////////////////////
	  Socket(SocketType t, SocketProtocol p) : Handle(family,t,p), Async(false)
	  {
	  }

    // ----------------------------------- STATIC METHODS -----------------------------------
  
    // ---------------------------------- ACCESSOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Socket::handle const
    //! Get the shared socket handle 
    //! 
    //! \return const HSocket& - Shared socket handle
    /////////////////////////////////////////////////////////////////////////////////////////
    const HSocket& handle() const
    {
      return Handle;
    }

  private:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Socket::succeeded const
    //! Query whether an operation succeeded
    //! 
    //! \return bool - True iff operation succeeded immediately or was executed asychronously
    /////////////////////////////////////////////////////////////////////////////////////////
    bool succeeded(int result)
    {
      return result == ERROR_SUCCESS || (Async && result == SOCKET_ERROR && ::WSAGetLastError() == WSAEWOULDBLOCK);
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // Socket::async
    //! Request asychronous socket behaviour and message-based notifications
    //!
    //! \param[in] const& wnd - Window 
    //! \param[in] ev - Requested notifications
    //! 
    //! \throw wtl::socket_error - Unable to set asychronous socket behaviour
    //////////////////////////////////////////////////////////////////////////////////////////
    template <Encoding ENC>
    void async(const Window<ENC>& wnd, SocketEvent ev = SocketEvent::Read|SocketEvent::Write|SocketEvent::Connect|SocketEvent::Close|SocketEvent::Accept)
    {
      // Set socket options
      if (::WSAAsyncSelect(Handle, wnd, enum_cast(WindowMessage::Socket), enum_cast(ev)) != ERROR_SUCCESS)
        throw socket_error(HERE, "Unable to set asychronous socket behaviour");

      // Mark socket as asychronous
      Async = true;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Socket::close
    //! Closes the socket
    //////////////////////////////////////////////////////////////////////////////////////////
    void close()
    {
      // Release handle
      Handle.release();
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Socket::connect
    //! Connect to a host
    //!
    //! \param[in] const& host - Host IP Address
    //! \param[in] port - Host port (in host byte order)
    //! 
    //! \throw wtl::socket_error - Failed to connect
    //////////////////////////////////////////////////////////////////////////////////////////
    void connect(const String<Encoding::ANSI>& host, uint16_t port)
    {
      // Encode host address
      address_t addr { enum_cast(family), ::htons(port) }; //, ::inet_addr(addr.c_str()) };
      addr.sin_addr.s_addr = ::inet_addr(host.c_str());

      // Connect to host
      int32_t res = ::connect(Handle, reinterpret_cast<SOCKADDR*>(&addr), sizeof(address_t));
      
      // Check for synchronous/asychronous success states
      if (res == ERROR_SUCCESS || (Async && res == SOCKET_ERROR && ::WSAGetLastError() == WSAEWOULDBLOCK))
        return;

      // Failed to connect
      throw socket_error(HERE, "Unable to connect to host: " + host);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Socket::receive
    //! Receive data from a connected socket
    //!
    //! \param[in] const* buf - Data buffer
    //! \param[in] len - Length of buffer (in elements rather than bytes)
    //! \param[in] flags - [optional] Flags
    //! \return int32_t - Number of bytes sent
    //! 
    //! \throw wtl::socket_error - Failed to connect
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename ELEM>
    int32_t receive(ELEM* buf, uint32_t len, int32_t flags = 0)
    {
      int32_t n;
      
      // Reinterpret as byte buffer and receieve data
      if ((n = ::recv(handle(), reinterpret_cast<char*>(buf), len*sizeof(ELEM), flags)) == SOCKET_ERROR)
        throw socket_error(HERE, "Unable to receieve data");

      return n;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Socket::send
    //! Send data across a connected socket
    //!
    //! \param[in] const* buf - Data buffer
    //! \param[in] len - Length of buffer (in elements rather than bytes)
    //! \param[in] flags - [optional] Flags
    //! \return int32_t - Number of bytes sent
    //! 
    //! \throw wtl::socket_error - Failed to connect
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename ELEM>
    int32_t send(const ELEM* buf, uint32_t len, int32_t flags = 0)
    {
      int32_t n;

      // Reinterpret as byte buffer and send data
      if ((n = ::send(handle(), reinterpret_cast<const char*>(buf), len*sizeof(ELEM), flags)) == SOCKET_ERROR)
        throw socket_error(HERE, "Unable to send data");

      return n;
    }
    
  };

}



#endif // WTL_SOCKET_HPP


