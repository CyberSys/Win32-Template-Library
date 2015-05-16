//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Macros.h
//! \brief Collection of helpful macros
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MACROS_HPP
#define WTL_MACROS_HPP

#include "WTL.hpp"

// --------------------------------------------------------------
// -------------------- CONDITIONAL BUILDS ----------------------
// --------------------------------------------------------------

//! \def DEVELOPMENT_MODE - Activate boundary, domain/logic invariant, iterator, and function argument verification
#define DEVELOPMENT_MODE

// --------------------------------------------------------------
// -------------------------- COMPILER --------------------------
// --------------------------------------------------------------

//! \if __clang__ - Clang
#ifdef __clang__
  //! \def CONSTEXPR_SDMI_CAP - Enabled iff compiler supports constexpr static data member initialization 
  #define CONSTEXPR_SDMI_CAP

//! \if _MSC_VER - Visual Studio 
#elif _MSC_VER 
//! \def STRINGIFY_CAP - Enabled iff compiler supports 'stringify' preprocessor operator '#'
  #define STRINGIFY_CAP

  //! \if _MSC_VER >= 1900 - Visual Studio 2015
  #if _MSC_VER >= 1900
    
  #endif
#endif
// --------------------------------------------------------------------------------------------------------
// ------------------------------------------ GLOBAL MACROS -----------------------------------------------
// --------------------------------------------------------------------------------------------------------
//! \def STR - Stringifies a symbol
#define STR2(s)  #s
#define STR(s) STR2(s)

//! \def HERE - Generates a source file error location string: '<function>() on line <line> of <file> : '
#ifdef STRINGIFY_CAP
  #define HERE  __FUNCTION__ "() on line " STR(__LINE__) " of " __FILE__ " : "
#else
  #define HERE  __FUNCTION__
#endif
// --------------------------------------------------------------------------------------------------------
// --------------------------------------- CONSTRUCTION SEMANTICS -----------------------------------------
// --------------------------------------------------------------------------------------------------------

//! \def DEFAULT_CTOR - Enables trivial default-contructor
#define DEFAULT_CTOR(type)          type() = default

//! \def DISABLE_CTOR - Disables trivial default-contructor
#define DISABLE_CTOR(type)          type() = delete

// --------------------------------------------------------------------------------------------------------
// ---------------------------------------- CONSTEXPR SEMANTICS -------------------------------------------
// --------------------------------------------------------------------------------------------------------

//! \def CONSTEXPR_CTOR - Enables constexpr default-construction
#define CONSTEXPR_CTOR(type)        constexpr type() noexcept {}

//! \def CONSTEXPR_COPY_CTOR - Enables constexpr copy-construction
#define CONSTEXPR_COPY_CTOR(type)   constexpr type(const type& r) noexcept = default

//! \def CONSTEXPR_MOVE_CTOR - Enables constexpr move-construction
#define CONSTEXPR_MOVE_CTOR(type)   constexpr type(type&& r) noexcept = default

//! \def DEFAULT_CONSTEXPR - Enables constexpr default-construction
#define DEFAULT_CONSTEXPR(type)     CONSTEXPR_CTOR(type);  CONSTEXPR_COPY_CTOR(type);  CONSTEXPR_MOVE_CTOR(type)

// --------------------------------------------------------------------------------------------------------
// ------------------------------------------- COPY SEMANTICS ---------------------------------------------
// --------------------------------------------------------------------------------------------------------
//! \def DISABLE_COPY_CTOR - Prevents copy-construction
#define DISABLE_COPY_CTOR(type)     type(const type& r) = delete

//! \def DISABLE_COPY_ASSIGN - Prevents copy-assignment 
#define DISABLE_COPY_ASSIGN(type)   type& operator=(const type& r) = delete

//! \def DISABLE_COPY - Prevents copy-construction and copy-assignment
#define DISABLE_COPY(type)          DISABLE_COPY_CTOR(type);  DISABLE_COPY_ASSIGN(type)


//! \def ENABLE_COPY_CTOR - Enables copy-construction
#define ENABLE_COPY_CTOR(type)     type(const type& r) = default

//! \def ENABLE_COPY_ASSIGN - Enables copy-assignment
#define ENABLE_COPY_ASSIGN(type)   type& operator=(const type& r) = default

//! \def ENABLE_COPY - Enables copy-construction and copy-assignment
#define ENABLE_COPY(type)          ENABLE_COPY_CTOR(type);  ENABLE_COPY_ASSIGN(type)

// --------------------------------------------------------------------------------------------------------
// ------------------------------------------ MOVE SEMANTICS ----------------------------------------------
// --------------------------------------------------------------------------------------------------------

//! \def DISABLE_MOVE_CTOR - Prevents move-construction
#define DISABLE_MOVE_CTOR(type)     type(type&& r) = delete

//! \def DISABLE_MOVE_ASSIGN - Prevents move-assignment 
#define DISABLE_MOVE_ASSIGN(type)   type& operator=(type&& r) = delete

//! \def DISABLE_MOVE - Prevents move-construction and move-assignment
#define DISABLE_MOVE(type)          DISABLE_MOVE_CTOR(type);  DISABLE_MOVE_ASSIGN(type)


//! \def ENABLE_MOVE_CTOR - Enables move-construction
#define ENABLE_MOVE_CTOR(type)      type(type&& r) = default

//! \def ENABLE_MOVE_ASSIGN - Enables move-assignment 
#define ENABLE_MOVE_ASSIGN(type)    type& operator=(type&& r) = default

//! \def ENABLE_MOVE - Enables move-construction and move-assignment
#define ENABLE_MOVE(type)           ENABLE_MOVE_CTOR(type);  ENABLE_MOVE_ASSIGN(type)

// --------------------------------------------------------------------------------------------------------
// ---------------------------------------- DESTROY SEMANTICS ---------------------------------------------
// --------------------------------------------------------------------------------------------------------

//! \def DISABLE_DTOR - Disables destruction entirely
#define DISABLE_DTOR(type)          ~type() = delete

//! \def DISABLE_POLY - Disables polymorphism by enabling the non-virtual destructor
#define DISABLE_POLY(type)          ~type() = default

//! \def ENABLE_POLY - Enables polymorphism by enabling the virtual destructor
#define ENABLE_POLY(type)           virtual ~type() = default

// --------------------------------------------------------------------------------------------------------
// -------------------------------------------- DEVELOPMENT -----------------------------------------------
// --------------------------------------------------------------------------------------------------------

//! if DEVELOPMENT_MODE - Activate debugging checks 
#ifdef DEVELOPMENT_MODE
  //! \def CHECKED_ARGUMENTS - Activate argument checks
  #define CHECKED_ARGUMENTS

  //! \def CHECKED_BOUNDARIES - Activate boundary checks
  #define CHECKED_BOUNDARIES

  //! \def CHECKED_INVARIANTS - Activate invariant checks
  #define CHECKED_INVARIANTS

//! \ifnot DEVELOPMENT_MODE - Deactivates debugging checks 
#else
  //! \def RELEASE_MODE - Enables optimizations and disables debugging checks
  #define RELEASE_MODE
#endif


// --------------------------------------------------------------------------------------------------------
// ------------------------------------------ CHECKED ARGUMENTS -------------------------------------------
// --------------------------------------------------------------------------------------------------------

//! \if CHECKED_ARGUMENTS - Activates argument verification
#ifdef CHECKED_ARGUMENTS
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \def REQUIRED_PARAM - Throws an exception when a parameter is missing
  //! \param[in] arg - Parameter name
  //////////////////////////////////////////////////////////////////////////////////////////
#ifdef STRINGIFY_CAP
  #define REQUIRED_PARAM(arg)           { if ((arg) == nullptr) throw wtl::invalid_argument(HERE, "Missing argument: '" #arg "'"); }
#else
  #define REQUIRED_PARAM(arg)           { if ((arg) == nullptr) throw wtl::invalid_argument(HERE, "Missing argument"); }
#endif

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \def PARAM_INVARIANT - Throws an exception when a parameter is missing
  //! \param[in] exp - Expression defining a parameter invariant 
  //////////////////////////////////////////////////////////////////////////////////////////
#ifdef STRINGIFY_CAP
  #define PARAM_INVARIANT(arg,exp)      { if ((exp) == false) throw wtl::invalid_argument(HERE, "Missing argument: '" #arg "'"); }
#else
  #define PARAM_INVARIANT(arg,exp)      { if ((exp) == false) throw wtl::invalid_argument(HERE, "Missing argument"); }
#endif

//! \ifnot CHECKED_ARGUMENTS - Deactivates argument verification
#else
  //! \def REQUIRED_PARAM - Deactivated
  #define REQUIRED_PARAM(arg)       
#endif


// --------------------------------------------------------------------------------------------------------
// ----------------------------------------- CHECKED BOUNDARIES -------------------------------------------
// --------------------------------------------------------------------------------------------------------

//! \if CHECKED_BOUNDARIES - Activates boundary verification
#ifdef CHECKED_BOUNDARIES
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \def CHECKED_INDEX - Throws an exception when an index is outside of an implementation defined range
  //! \param[in] idx - Index
  //! \param[in] min - Inclusive lower bounds
  //! \param[in] max - Exclusive upper bounds
  //////////////////////////////////////////////////////////////////////////////////////////
  #define CHECKED_INDEX(idx, min, max)  { if (((idx) >= (min) && (idx) < (max)) == false) throw wtl::out_of_range(HERE, "Index %d outside of range %d to %d", idx, min, max); }

//#ifdef STRINGIFY_CAP
//  #define CHECKED_INDEX(idx, min, max)  { if (((idx) >= (min) && (idx) < (max)) == false) throw wtl::out_of_range(HERE, "Index " #idx " outside of range " #min " to " #max); }
//#else
//  #define CHECKED_INDEX(idx, min, max)  { if (((idx) >= (min) && (idx) < (max)) == false) throw wtl::out_of_range(HERE, "Index %d outside of range %d to %d", idx, min, max); }
//#endif

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \def CHECKED_LENGTH - Throws an exception when an implementation defined length is exceeded
  //! \param[in] len - Length
  //! \param[in] max - Inclusive upper bound
  //////////////////////////////////////////////////////////////////////////////////////////
  #define CHECKED_LENGTH(len, max)      { if (((len) <= (max)) == false) throw wtl::length_error(HERE, "Insufficient space: %d available, %s required", max, len); }

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \def LENGTH_INVARIANT - Throws an exception when an implementation defined length is exceeded
  //! \param[in] exp - Expression defining a length invariant
  //////////////////////////////////////////////////////////////////////////////////////////
#ifdef STRINGIFY_CAP
  #define LENGTH_INVARIANT(exp)       { if ((exp) == false) throw wtl::length_error(HERE, "Length invariant violated: '" #exp "'"); }
#else
  #define LENGTH_INVARIANT(exp)       { if ((exp) == false) throw wtl::length_error(HERE, "Insufficient space"); }
#endif

//! \ifnot CHECKED_BOUNDARIES - Deactivates boundary verification
#else
  //! \def CHECKED_INDEX - Deactivated
  #define CHECKED_INDEX(idx, min, max)  

  //! \def CHECKED_LENGTH - Deactivated
  #define CHECKED_LENGTH(len, max) 
#endif


// --------------------------------------------------------------------------------------------------------
// ---------------------------------------- CHECKED INVARIANTS --------------------------------------------
// --------------------------------------------------------------------------------------------------------

  
//! \if CHECKED_INVARIANTS - Activates invariant verification
#ifdef CHECKED_INVARIANTS
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \def DOMAIN_INVARIANT - Throws an exception upon violation of a domain invariant 
  //! \param[in] exp - Expression defining a domain invariant
  //////////////////////////////////////////////////////////////////////////////////////////
#ifdef STRINGIFY_CAP
  #define DOMAIN_INVARIANT(exp)         { if ((exp) == false) throw wtl::domain_error(HERE, "Domain invariant violated: '" #exp "'"); }
#else
  #define DOMAIN_INVARIANT(exp)         { if ((exp) == false) throw wtl::domain_error(HERE, "Domain invariant violated"); }
#endif

  //////////////////////////////////////////////////////////////////////////////////////////
  //! \def LOGIC_INVARIANT - Throws an exception upon violation of a logic invariant 
  //! \param[in] exp - Expression defining a logic invariant
  //////////////////////////////////////////////////////////////////////////////////////////
#ifdef STRINGIFY_CAP
  #define LOGIC_INVARIANT(exp)          { if ((exp) == false) throw wtl::logic_error(HERE, "Logic invariant violated: '" #exp "'"); }
#else
  #define LOGIC_INVARIANT(exp)          { if ((exp) == false) throw wtl::logic_error(HERE, "Logic invariant violated"); }
#endif

//! \ifnot CHECKED_INVARIANTS - Deactivates invariant verification
#else
  //! \def DOMAIN_INVARIANT - Deactivated
  #define DOMAIN_INVARIANT(exp) 

  //! \def LOGIC_INVARIANT - Deactivated
  #define LOGIC_INVARIANT(exp) 
#endif



#endif // WTL_MACROS_HPP


