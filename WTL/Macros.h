////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Macros.h
//! \brief Collection of helpful macros
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MACROS_HPP
#define WTL_MACROS_HPP

#include "WTL.hpp"

// --------------------------------------------------
// -------------- CONDITIONAL BUILDS ----------------
// --------------------------------------------------

//! \def DEVELOPMENT_MODE - Activate boundary, domain/logic invariant, iterator, and function argument verification
#define DEVELOPMENT_MODE

// --------------------------------------------------
// -------------------- COMPILER --------------------
// --------------------------------------------------

//! \if __clang__ - Clang
#ifdef __clang__
  //! \def CONSTEXPR_CAP - Enabled iff compiler provides full constexpr support
  #define CONSTEXPR_CAP

//! \if _MSC_VER - Visual Studio 
#elif _MSC_VER 
//! \def STRINGIFY_CAP - Enabled iff compiler supports 'stringify' preprocessor operator '#'
  #define STRINGIFY_CAP

  //! \if _MSC_VER >= 1900 - Visual Studio 2015
  #if _MSC_VER >= 1900
    //! \def CONSTEXPR_CAP - Enabled iff compiler provides full constexpr support
    //#define CONSTEXPR_CAP
  #endif
#endif

// --------------------------------------------------
// ----------------- GLOBAL MACROS ------------------
// --------------------------------------------------

//! \def STR - Stringifies a symbol
#define STR2(s)  #s
#define STR(s) STR2(s)

//! \def HERE - Generates a source file error location string: '<function>() on line <line> of <file> : '
#ifdef STRINGIFY_CAP
  #define HERE  __FUNCTION__ "() on line " STR(__LINE__) " of " __FILE__ " : "
#else
  #define HERE  __FUNCTION__
#endif

//! \def STRSAFE_NO_DEPRECATE - Disable the StringSafe library depreciation warnings
#define STRSAFE_NO_DEPRECATE

//! \def _SCL_SECURE_NO_WARNINGS - Disable visual C++ checked iterator warnings, provided by wtl
#define _SCL_SECURE_NO_WARNINGS

//! \def _CRT_SECURE_NO_WARNINGS - Disable C runtime library warnings
#define _CRT_SECURE_NO_WARNINGS

// ---------------------------------------------------------
// -------------------- COPY SEMANTICS  --------------------
// ---------------------------------------------------------

//! \def NO_COPY_CTOR - Prevents a type from being copy-constructed
#define NO_COPY_CTOR(type)          type(const type& r) = delete

//! \def NO_COPY_ASSIGN - Prevents copy-assignment upon a type
#define NO_COPY_ASSIGN(type)        type& operator=(const type& r) = delete

//! \def NO_COPY - Prevents a type from being copy-constructed or copy-assigned
#define NO_COPY(type)               NO_COPY_CTOR(type);  NO_COPY_ASSIGN(type)


//! \def DEFAULT_COPY_CTOR - Enables compiler generated copy constructor
#define DEFAULT_COPY_CTOR(type)     type(const type& r) = default

//! \def DEFAULT_COPY_ASSIGN - Enables compiler generated copy-assign operator
#define DEFAULT_COPY_ASSIGN(type)   type& operator=(const type& r) = default

//! \def DEFAULT_COPY - Enables compiler generated copy constructor
#define DEFAULT_COPY(type)          DEFAULT_COPY_CTOR(type);  DEFAULT_COPY_ASSIGN(type)

// ---------------------------------------------------------
// -------------------- MOVE SEMANTICS  --------------------
// ---------------------------------------------------------

//! \def NO_MOVE_CTOR - Prevents a type from being move-constructed
#define NO_MOVE_CTOR(type)          type(type&& r) = delete

//! \def NO_MOVE_ASSIGN - Prevents move-assignment upon a type
#define NO_MOVE_ASSIGN(type)        type& operator=(type&& r) = delete

//! \def NO_MOVE - Prevents a type from being move-constructed or move-assigned
#define NO_MOVE(type)               NO_MOVE_CTOR(type);  NO_MOVE_ASSIGN(type)


//! \def DEFAULT_MOVE_CTOR - Enables compiler generated move constructor
#define DEFAULT_MOVE_CTOR(type)     type(type&& r) = default

//! \def DEFAULT_MOVE_ASSIGN - Enables compiler generated move-assignment operator
#define DEFAULT_MOVE_ASSIGN(type)   type& operator=(type&& r) = default

//! \def DEFAULT_MOVE - Enables compiler generated move constructor
#define DEFAULT_MOVE(type)          DEFAULT_MOVE_CTOR(type);  DEFAULT_MOVE_ASSIGN(type)

// --------------------------------------------------
// ------------------ DEVELOPMENT -------------------
// --------------------------------------------------

//! if DEVELOPMENT_MODE - Activate  Query whether in release mode  (Can be manually set)
#ifdef DEVELOPMENT_MODE
  //! \def CHECKED_ARGUMENTS - Activate argument checks
  #define CHECKED_ARGUMENTS

  //! \def CHECKED_BOUNDARIES - Activate boundary checks
  #define CHECKED_BOUNDARIES

  //! \def CHECKED_INVARIANTS - Activate invariant checks
  #define CHECKED_INVARIANTS

#else
  //! \def RELEASE_MODE - Enables optimizations and disables debugging checks
  #define RELEASE_MODE
#endif


//! \if CHECKED_ARGUMENTS - Activates function argument verification
#ifdef CHECKED_ARGUMENTS
  ////////////////////////////////////////////////////////////////////////////////
  //! \def REQUIRED_PARAM - Throws an exception when a parameter is missing
  //! \param[in] arg - Parameter name
  ////////////////////////////////////////////////////////////////////////////////
#ifdef STRINGIFY_CAP
  #define REQUIRED_PARAM(arg)           { if ((arg) == nullptr) throw wtl::invalid_argument(HERE, "Missing argument: '" #arg "'"); }
#else
  #define REQUIRED_PARAM(arg)           { if ((arg) == nullptr) throw wtl::invalid_argument(HERE, "Missing argument"); }
#endif

  ////////////////////////////////////////////////////////////////////////////////
  //! \def PARAM_INVARIANT - Throws an exception when a parameter is missing
  //! \param[in] exp - Expression defining a parameter invariant 
  ////////////////////////////////////////////////////////////////////////////////
#ifdef STRINGIFY_CAP
  #define PARAM_INVARIANT(arg,exp)      { if ((exp) == false) throw wtl::invalid_argument(HERE, "Missing argument: '" #arg "'"); }
#else
  #define PARAM_INVARIANT(arg,exp)      { if ((exp) == false) throw wtl::invalid_argument(HERE, "Missing argument"); }
#endif

#else
  //! \def REQUIRED_PARAM - Deactivated
  #define REQUIRED_PARAM(arg)       
#endif


//! \if CHECKED_BOUNDARIES - Activates boundary verification
#ifdef CHECKED_BOUNDARIES
  ////////////////////////////////////////////////////////////////////////////////
  //! \def CHECKED_INDEX - Throws an exception when an index is outside of an implementation defined range
  //! \param[in] idx - Index
  //! \param[in] min - Inclusive lower bounds
  //! \param[in] max - Exclusive upper bounds
  ////////////////////////////////////////////////////////////////////////////////
  #define CHECKED_INDEX(idx, min, max)  { if (((idx) >= (min) && (idx) < (max)) == false) throw wtl::out_of_range(HERE, "Index %d outside of range %d to %d", idx, min, max); }

//#ifdef STRINGIFY_CAP
//  #define CHECKED_INDEX(idx, min, max)  { if (((idx) >= (min) && (idx) < (max)) == false) throw wtl::out_of_range(HERE, "Index " #idx " outside of range " #min " to " #max); }
//#else
//  #define CHECKED_INDEX(idx, min, max)  { if (((idx) >= (min) && (idx) < (max)) == false) throw wtl::out_of_range(HERE, "Index %d outside of range %d to %d", idx, min, max); }
//#endif

  ////////////////////////////////////////////////////////////////////////////////
  //! \def CHECKED_LENGTH - Throws an exception when an implementation defined length is exceeded
  //! \param[in] len - Length
  //! \param[in] max - Inclusive upper bound
  ////////////////////////////////////////////////////////////////////////////////
  #define CHECKED_LENGTH(len, max)      { if (((len) <= (max)) == false) throw wtl::length_error(HERE, "Insufficient space: %d available, %s required", max, len); }

  ////////////////////////////////////////////////////////////////////////////////
  //! \def LENGTH_INVARIANT - Throws an exception when an implementation defined length is exceeded
  //! \param[in] exp - Expression defining a length invariant
  ////////////////////////////////////////////////////////////////////////////////
#ifdef STRINGIFY_CAP
  #define LENGTH_INVARIANT(exp)       { if ((exp) == false) throw wtl::length_error(HERE, "Length invariant violated: '" #exp "'"); }
#else
  #define LENGTH_INVARIANT(exp)       { if ((exp) == false) throw wtl::length_error(HERE, "Insufficient space"); }
#endif

#else
  //! \def CHECKED_INDEX - Deactivated
  #define CHECKED_INDEX(idx, min, max)  

  //! \def CHECKED_LENGTH - Deactivated
  #define CHECKED_LENGTH(len, max) 
#endif


  
//! \if CHECKED_INVARIANTS - Activates invariant verification
#ifdef CHECKED_INVARIANTS
  ////////////////////////////////////////////////////////////////////////////////
  //! \def DOMAIN_INVARIANT - Throws an exception upon violation of a domain invariant 
  //! \param[in] exp - Expression defining a domain invariant
  ////////////////////////////////////////////////////////////////////////////////
#ifdef STRINGIFY_CAP
  #define DOMAIN_INVARIANT(exp)         { if ((exp) == false) throw wtl::domain_error(HERE, "Domain invariant violated: '" #exp "'"); }
#else
  #define DOMAIN_INVARIANT(exp)         { if ((exp) == false) throw wtl::domain_error(HERE, "Domain invariant violated"); }
#endif

  ////////////////////////////////////////////////////////////////////////////////
  //! \def LOGIC_INVARIANT - Throws an exception upon violation of a logic invariant 
  //! \param[in] exp - Expression defining a logic invariant
  ////////////////////////////////////////////////////////////////////////////////
#ifdef STRINGIFY_CAP
  #define LOGIC_INVARIANT(exp)          { if ((exp) == false) throw wtl::logic_error(HERE, "Logic invariant violated: '" #exp "'"); }
#else
  #define LOGIC_INVARIANT(exp)          { if ((exp) == false) throw wtl::logic_error(HERE, "Logic invariant violated"); }
#endif

#else
  //! \def DOMAIN_INVARIANT - Deactivated
  #define DOMAIN_INVARIANT(exp) 

  //! \def LOGIC_INVARIANT - Deactivated
  #define LOGIC_INVARIANT(exp) 
#endif



#endif // WTL_MACROS_HPP


