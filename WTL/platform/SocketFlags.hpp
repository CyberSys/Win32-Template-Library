//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\platform\SocketFlags.hpp
//! \brief Defines flags for socket API functions
//! \date 4 December 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_SOCKET_FLAGS_HPP
#define WTL_SOCKET_FLAGS_HPP

#include <wtl/WTL.hpp>
#include <wtl/traits/EnumTraits.hpp>          //!< is_attribute, is_contiguous
#include <wtl/utils/Default.hpp>              //!< default_t
#include <WinSock2.h>                          //!< 

//! \namespace wtl - Windows template library
namespace wtl
{
  
  // --------------------------------------------------------------------------------------------------------------
  // -----------------------------------------------    SOCKETS     -----------------------------------------------
  // --------------------------------------------------------------------------------------------------------------
  
  //! \enum AddressFamily - Socket address family
  enum class AddressFamily : uint8_t
  {
    Unspecified = AF_UNSPEC,	  //!< [Windows 4.00] The address family is unspecified.
    Unix = AF_UNIX,             //!< [Windows ?.??] local to host (pipes, portals)
    IPv4 = AF_INET,			        //!< [Windows 4.00] The Internet Protocol version 4 (IPv4) address family.
    ImpLink = AF_IMPLINK,			  //!< [Windows ?.??] arpanet imp addresses
    Pup = AF_PUP,			          //!< [Windows ?.??] pup protocols: e.g. BSP
    Chaos = AF_CHAOS,			      //!< [Windows ?.??] mit CHAOS protocols
    XeroxNs = AF_NS,			      //!< [Windows ?.??] XEROX NS protocols
    Ips = AF_IPX,			          //!< [Windows 4.00] The IPX/SPX address family. This address family is only supported if the NWLink IPX/SPX NetBIOS Compatible Transport protocol is installed.
                                //!<                This address family is not supported on Windows Vista and later.
    Iso = AF_ISO,			          //!< [Windows ?.??] ISO protocols
    Osi = AF_ISO,			          //!< [Windows ?.??] OSI is ISO
    Ecma = AF_ECMA,			        //!< [Windows ?.??] european computer manufacturers
    Datakit = AF_DATAKIT,			  //!< [Windows ?.??] datakit protocols
    CCITT = AF_CCITT,			      //!< [Windows ?.??] CCITT protocols, X.25 etc
    SNA = AF_SNA,			          //!< [Windows ?.??] IBM SNA
    DECnet = AF_DECnet,			    //!< [Windows ?.??] DECnet
    DataLink = AF_DLI,			    //!< [Windows ?.??] Direct data link interface
    LAT = AF_LAT,			          //!< [Windows ?.??] LAT
    HyperChannel = AF_HYLINK,	  //!< [Windows ?.??] NSC Hyperchannel
    AppleTalk = AF_APPLETALK,	  //!< [Windows 4.00] The AppleTalk address family. This address family is only supported if the AppleTalk protocol is installed.
                                //!<                This address family is not supported on Windows Vista and later.
    NetBIOS = AF_NETBIOS,			  //!< [Windows 4.00] The NetBIOS address family. This address family is only supported if the Windows Sockets provider for NetBIOS is installed.
                                //!<                The Windows Sockets provider for NetBIOS is supported on 32-bit versions of Windows. This provider is installed by default on 32-bit versions of Windows.
                                //!<                The Windows Sockets provider for NetBIOS is not supported on 64-bit versions of windows including Windows 7, Windows Server 2008, Windows Vista, Windows Server 2003, or Windows XP.
                                //!<                The Windows Sockets provider for NetBIOS only supports sockets where the type parameter is set to SOCK_DGRAM.
                                //!<                The Windows Sockets provider for NetBIOS is not directly related to the NetBIOS programming interface. The NetBIOS programming interface is not supported on Windows Vista, Windows Server 2008, and later.
    Voiceview = AF_VOICEVIEW,	  //!< [Windows ?.??] VoiceView
    Firefox = AF_FIREFOX,			  //!< [Windows ?.??] Protocols from Firefox
    Unknown1 = AF_UNKNOWN1,		  //!< [Windows ?.??] Somebody is using this!
    Banyan = AF_BAN,			      //!< [Windows ?.??] Banyan
    Atm = AF_ATM,			          //!< [Windows ?.??] Native ATM Services
    IPv6 = AF_INET6,			      //!< [Windows 4.00] The Internet Protocol version 6 (IPv6) address family.
    Cluster = AF_CLUSTER,			  //!< [Windows ?.??] Microsoft Wolfpack
    Ieee12844 = AF_12844,			  //!< [Windows ?.??] IEEE 1284.4 WG AF
    Infrared = AF_IRDA,			    //!< [Windows 4.00] The Infrared Data Association (IrDA) address family.
                                //!<                This address family is only supported if the computer has an infrared port and driver installed.
    NETDES = AF_NETDES,			    //!< [Windows ?.??] Network Designers OSI & gateway
#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
    Bluetooth = AF_BTH,			    //!< [Windows 6.00] The Bluetooth address family.
                                //!<                This address family is supported on Windows XP with SP2 or later if the computer has a Bluetooth adapter and driver installed.
#endif
  };
  
  //! Define traits: Contiguous enumeration
  template <> struct is_attribute<AddressFamily>  : std::false_type  {};
  template <> struct is_contiguous<AddressFamily> : std::true_type   {};
  template <> struct default_t<AddressFamily>     : std::integral_constant<AddressFamily,AddressFamily::Unspecified>   {};

  // --------------------------------------------------------------------------------------------------------------

  //! \enum SocketType - Defines socket types
  enum class SocketType : uint8_t
  {
    Stream = SOCK_STREAM,			      //!< [Windows 4.00] A socket type that provides sequenced, reliable, two-way, connection-based byte streams with an OOB data transmission mechanism. This socket type uses the Transmission Control Protocol (TCP) for the Internet address family (AF_INET or AF_INET6).
    Datagram = SOCK_DGRAM,			    //!< [Windows 4.00] A socket type that supports datagrams, which are connectionless, unreliable buffers of a fixed (typically small) maximum length. This socket type uses the User Datagram Protocol (UDP) for the Internet address family (AF_INET or AF_INET6).
    Raw = SOCK_RAW,			            //!< [Windows 4.00] A socket type that provides a raw socket that allows an application to manipulate the next upper-layer protocol header. To manipulate the IPv4 header, the IP_HDRINCL socket option must be set on the socket. To manipulate the IPv6 header, the IPV6_HDRINCL socket option must be set on the socket.
    Reliable = SOCK_RDM,			      //!< [Windows 4.00] A socket type that provides a reliable message datagram. An example of this type is the Pragmatic General Multicast (PGM) multicast protocol implementation in Windows, often referred to as reliable multicast programming.
                                    //!<                This type value is only supported if the Reliable Multicast Protocol is installed.
    Sequenced = SOCK_SEQPACKET,			//!< [Windows 4.00] A socket type that provides a pseudo-stream packet based on datagrams.
  };

  //! Define traits: Contiguous enumeration
  template <> struct is_attribute<SocketType>  : std::false_type  {};
  template <> struct is_contiguous<SocketType> : std::true_type   {};
  template <> struct default_t<SocketType>     : std::integral_constant<SocketType,SocketType::Stream>   {};
  
  // --------------------------------------------------------------------------------------------------------------

  //! \enum SocketEvent - Defines network events
  enum class SocketEvent : uint32_t
  {
    Read = FD_READ,                                 //!< [Windows 4.00] Set to receive notification of readiness for reading.
    Write = FD_WRITE,                               //!< [Windows 4.00] Wants to receive notification of readiness for writing.
    OOB = FD_OOB,                                   //!< [Windows 4.00] Wants to receive notification of the arrival of OOB data.
    Accept = FD_ACCEPT,                             //!< [Windows 4.00] Wants to receive notification of incoming connections.
    Connect = FD_CONNECT,                           //!< [Windows 4.00] Wants to receive notification of completed connection or multipoint join operation.
    Close = FD_CLOSE,                               //!< [Windows 4.00] Wants to receive notification of socket closure.
    QOS = FD_QOS,                                   //!< [Windows 4.00] Wants to receive notification of socket Quality of Service (QoS) changes.
    GroupQOS = FD_GROUP_QOS,                        //!< [Windows 4.00] Wants to receive notification of socket group Quality of Service (QoS) changes (reserved for future use with socket groups). Reserved
    RoutingChange = FD_ROUTING_INTERFACE_CHANGE,    //!< [Windows 4.00] Wants to receive notification of routing interface changes for the specified destination(s).
    AddressChange = FD_ADDRESS_LIST_CHANGE,         //!< [Windows 4.00] Wants to receive notification of local address list changes for the socket protocol family.
    All = FD_ALL_EVENTS,                            //!< [Windows 4.00] All events
  };

  //! Define traits: Non-Contiguous attribute
  template <> struct is_attribute<SocketEvent>  : std::true_type    {};
  template <> struct is_contiguous<SocketEvent> : std::false_type   {};
  template <> struct default_t<SocketEvent>     : std::integral_constant<SocketEvent,SocketEvent::Read>   {};

  // --------------------------------------------------------------------------------------------------------------

  //! \enum SocketProtocol - Defines socket protocols
  enum class SocketProtocol : uint16_t
  {
    Icmp = IPPROTO_ICMP,			          //!< [Windows 5.01] The Internet Control Message Protocol (ICMP). This is a possible value when the af parameter is AF_UNSPEC, AF_INET, or AF_INET6 and the type parameter is SOCK_RAW or unspecified.
    Igmp = IPPROTO_IGMP,			          //!< [Windows 5.01] The Internet Group Management Protocol (IGMP). This is a possible value when the af parameter is AF_UNSPEC, AF_INET, or AF_INET6 and the type parameter is SOCK_RAW or unspecified.
    Bluetooth = IPPROTO_GGP,	          //!< [Windows 5.01] The Bluetooth Radio Frequency Communications (Bluetooth RFCOMM) protocol. This is a possible value when the af parameter is AF_BTH and the type parameter is SOCK_STREAM.
    Tcp = IPPROTO_TCP,			            //!< [Windows 4.00] The Transmission Control Protocol (TCP). This is a possible value when the af parameter is AF_INET or AF_INET6 and the type parameter is SOCK_STREAM.
    Udp = IPPROTO_UDP,			            //!< [Windows 4.00] The User Datagram Protocol (UDP). This is a possible value when the af parameter is AF_INET or AF_INET6 and the type parameter is SOCK_DGRAM.

#if _WIN32_WINNT >= _WIN32_WINNT_WINXP
    IPv6_HopOptions = IPPROTO_HOPOPTS,  //!< [Windows 5.01] IPv6 Hop-by-Hop options   
    IPv4 = IPPROTO_IPV4,	              //!< [Windows 5.01] 
    IPv6 = IPPROTO_IPV6,			          //!< [Windows 5.01] IPv6 header
    IPv6_Routing = IPPROTO_ROUTING,			//!< [Windows 5.01] IPv6 Routing header
    IPv6_Fragment = IPPROTO_FRAGMENT,		//!< [Windows 5.01] IPv6 fragmentation header
    SecurityPayload = IPPROTO_ESP,			//!< [Windows 5.01] encapsulating security payload
    AuthHeader = IPPROTO_AH,			      //!< [Windows 5.01] authentication header
    ICMPv6 = IPPROTO_ICMPV6,			      //!< [Windows 5.01] The Internet Control Message Protocol Version 6 (ICMPv6). This is a possible value when the af parameter is AF_UNSPEC, AF_INET, or AF_INET6 and the type parameter is SOCK_RAW or unspecified.
    IPv6_None = IPPROTO_NONE,			      //!< [Windows 5.01] IPv6 no next header
    IPv6_DestOptions = IPPROTO_DSTOPTS,	//!< [Windows 5.01] IPv6 Destination options
#endif

#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
    Pgm = IPPROTO_RM,			              //!< [Windows 6.00] The PGM protocol for reliable multicast. This is a possible value when the af parameter is AF_INET and the type parameter is SOCK_RDM. On the Windows SDK released for Windows Vista and later, this protocol is also called IPPROTO_PGM.
                                        //!< This protocol value is only supported if the Reliable Multicast Protocol is installed.
#endif
  };

  //! Define traits: Non-contiguous enumeration
  template <> struct is_attribute<SocketProtocol>  : std::false_type  {};
  template <> struct is_contiguous<SocketProtocol> : std::false_type  {};
  template <> struct default_t<SocketProtocol>     : std::integral_constant<SocketProtocol,SocketProtocol::IPv6_HopOptions>   {};
  
  // --------------------------------------------------------------------------------------------------------------

  //! \enum WinsockVersion - Defines winsock library versions
  enum class WinsockVersion : uint16_t
  {
    v1_0 = MAKEWORD(0,1),		//!< [Windows 4.00] Version 1.0
    v1_1 = MAKEWORD(1,1),		//!< [Windows 4.00] Version 1.1
    v2_0 = MAKEWORD(0,2),		//!< [Windows 5.00] Version 2.0
    v2_1 = MAKEWORD(1,2),		//!< [Windows 5.00] Version 2.1
    v2_2 = MAKEWORD(2,2),		//!< [Windows 5.00] Version 2.2
  };

  //! Define traits: Non-Contiguous enumeration
  template <> struct is_attribute<WinsockVersion>  : std::false_type  {};
  template <> struct is_contiguous<WinsockVersion> : std::false_type  {};
  template <> struct default_t<WinsockVersion>     : std::integral_constant<WinsockVersion,WinsockVersion::v1_0>   {};
  
  // 

} // WTL namespace



#endif // WTL_SOCKET_FLAGS_HPP


