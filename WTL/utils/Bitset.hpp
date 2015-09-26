//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Bitset.hpp
//! \brief Simple implementation of a bitset
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_BITSET_HPP
#define WTL_BITSET_HPP

#include "wtl/WTL.hpp"
#include "wtl/utils/Default.hpp"            //!< Default
#include "wtl/utils/DynamicArray.hpp"       //!< Array
#include "wtl/utils/Range.hpp"              //!< delimited_range
#include "wtl/utils/DebugInfo.hpp"          //!< DebugInfo

//! \namespace wtl - Windows template library
namespace wtl
{
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Bitset - Simple implementation of a bitset
  //! 
  //! \tparam MASK - Underlying mask type
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename MASK>
  struct Bitset 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  public:
    //! \alias type - Define own type
    using type = Bitset<MASK>;
  
    //! \typedef mask_t - Mask type
    using mask_t = MASK;

    //! \var bytes - Defines the number of bits in the set
    static constexpr uint32_t bytes = sizeof(mask_t);
      
    //! \var bits - Defines the number of bytes in the set
    static constexpr uint32_t bits = 8 * bytes;
    
    //! \alias BitArray - Array large enough to hold all bit indicies
    using BitArray = DynamicArray<uint32_t,bits>;
  
    //////////////////////////////////////////////////////////////////////////////////////////
    //! \struct loop - Unfurls the high bits of the mask into a variable length array
    //!
    //! \tparam IDX - Zero-based iteration index
    //////////////////////////////////////////////////////////////////////////////////////////
    template <uint32_t IDX>
    struct loop
    {
      static_assert((IDX >= 0) && (IDX < bits), "Invalid loop index");

      //////////////////////////////////////////////////////////////////////////////////////////
      // Bitset::flatten
      //! Populates an array with the zero-based indicies of all high bits 
      //!
      //! \param[in] const &mask - Bitset mask to be flattened
      //! \param[in,out] &out - Array to populate. For efficiency this is not cleared before use.
      //////////////////////////////////////////////////////////////////////////////////////////
      static void flatten(const mask_t& mask, BitArray& out)
      {
        // Query mask for relevant bit: 2^(n-1)
        if (mask & static_cast<mask_t>(power_of<2,IDX>::value))
          out.emplace_back(IDX);

        // Recursive step
        loop<IDX+1>::flatten(mask,out);
      }
    };

    //////////////////////////////////////////////////////////////////////////////////////////
    //! \struct loop<bits> - Base case 
    //////////////////////////////////////////////////////////////////////////////////////////
    template <>
    struct loop<bits>
    {
      static void flatten(const mask_t& mask, BitArray& out)
      { /*no-op*/ }
    };
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    mask_t  Mask;       //!< Underlying representation

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::Bitset constexpr 
    //! Initialize an empty/clear bitset 
    //////////////////////////////////////////////////////////////////////////////////////////
    constexpr Bitset() : Mask(defvalue<mask_t>())
    {}

    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::Bitset constexpr 
    //! Initialize a bitset with an initial value
    //! 
    //! \param[in] m - Initial value for mask
    //////////////////////////////////////////////////////////////////////////////////////////
    explicit constexpr Bitset(mask_t m) : Mask(m)
    {}

    // -------------------------------- COPYING & DESTRUCTION -------------------------------
  public:
    CONSTEXPR_COPY_CTOR(Bitset);      //!< Can be deep copied at compile-time
    CONSTEXPR_MOVE_CTOR(Bitset);      //!< Can be moved at compile-time
    ENABLE_COPY_ASSIGN(Bitset);       //!< Can be assigned
    ENABLE_MOVE_ASSIGN(Bitset);       //!< Can be move-assigned
    ENABLE_POLY(Bitset);              //!< Can be polymorphic

    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::operator =
    //! Overwrite the bitset mask 
    //!
    //! \param[in] m - Mask
    //! \return Bitset& - Reference to self containing new mask
    //////////////////////////////////////////////////////////////////////////////////////////
    type& operator= (mask_t m) 
    {
      Mask = m;
      return *this;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::operator =
    //! Copies the mask from another Bitset of different type
    //!
    //! \tparam M - Foreign bitset mask type
    //! 
    //! \param[in] const& r - Another bitset (of different type)
    //! \return Bitset& - Reference to self containing new mask
    //////////////////////////////////////////////////////////////////////////////////////////
    template <typename M>
    type& operator= (const Bitset<M>& r) 
    {
      // Perform implicit conversion upon foreign mask
      if (static_cast<void*>(this) != static_cast<const void*>(&r))
        Mask = static_cast<mask_t>(r.Mask);

      return *this;
    }


    // ----------------------------------- STATIC METHODS -----------------------------------
  public:

    // ---------------------------------- ACCESSOR METHODS ----------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::get
    //! Query the state of a bit
    //!
    //! \param[in] index - Zero-based index of bit to query
    //! \return bool - State of desired bit
    //! 
    //! \throw wtl::out_of_range - [Debug only] Index out of range
    //////////////////////////////////////////////////////////////////////////////////////////
    bool get(uint32_t index) const
    {
      CHECKED_INDEX(index, 0, bits);

      // Query state of bit
      return (Mask & static_cast<mask_t>(1 << index)) != defvalue<mask_t>();
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::empty
    //! Query whether mask is empty
    //!
    //! \return bool - True if empty, otherwise False
    //////////////////////////////////////////////////////////////////////////////////////////
    bool empty() const
    {
      return Mask == defvalue<mask_t>();
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::flatten const
    //! Generates an array containing the zero-based indicies of any high bits 
    //!
    //! \return BitArray : Array containing indicies of high bits
    //////////////////////////////////////////////////////////////////////////////////////////
    BitArray flatten() const
    {
      BitArray bits;    //!< 
      
      // Populate array with indicies of high bits
      loop<0>::flatten(Mask, bits);
      return bits;
    };


    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::operator mask_t()
    //! Implicitly cast to desired underlying type
    //////////////////////////////////////////////////////////////////////////////////////////
    operator mask_t() const
    {
      return Mask;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::operator []
    //! Query the state of a bit
    //!
    //! \param[in] index - zero-based index of bit to query
    //! \return bool - State of desired bit
    //////////////////////////////////////////////////////////////////////////////////////////
    bool operator[](uint32_t index) const
    {
      return get(index);
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::operator &
    //! Bitwise AND operator upon a value
    //!
    //! \param[in] v - Operand
    //! \return mask_t - Returns result of 'mask bitwise-and value'
    //////////////////////////////////////////////////////////////////////////////////////////
    mask_t operator& (const mask_t& v) const
    {
      return Mask & v;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::operator &
    //! Bitwise AND operator upon another bitset
    //!
    //! \param[in] r - Another bitset
    //! \return mask_t - Returns result of 'mask bitwise-and value'
    //////////////////////////////////////////////////////////////////////////////////////////
    mask_t operator& (const type& r) const
    {
      return Mask & r.Mask;
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::operator |
    //! Bitwise OR operator upon a value
    //!
    //! \param[in] v - Operand
    //! \return mask_t - Returns result of 'mask bitwise-or value'
    //////////////////////////////////////////////////////////////////////////////////////////
    mask_t operator| (const mask_t& v) const
    {
      return Mask | v;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::operator |
    //! Bitwise OR operator upon another bitset
    //!
    //! \param[in] r - Another bitset
    //! \return mask_t - Returns result of 'mask bitwise-or value'
    //////////////////////////////////////////////////////////////////////////////////////////
    mask_t operator| (const type& r) const
    {
      return Mask | r.Mask;
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
  public:
    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::clear
    //! Clears the mask
    //////////////////////////////////////////////////////////////////////////////////////////
    void  clear()
    {
      Mask = defvalue<mask_t>();
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::clear
    //! Clears the state of a bit
    //!
    //! \param[in] index - Zero-based bit index 
    //! 
    //! \throw wtl::out_of_range - [Debug only] Index out of range
    //////////////////////////////////////////////////////////////////////////////////////////
    void  clear(uint32_t index)
    {
      CHECKED_INDEX(index, 0, bits);

      // Clear bit
      Mask &= ~static_cast<mask_t>(1 << index);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    // Bitset::set
    //! Sets the state of a bit
    //!
    //! \param[in] index - Zero-based bit index 
    //! 
    //! \throw wtl::out_of_range - [Debug only] Index out of range
    //////////////////////////////////////////////////////////////////////////////////////////
    void  set(uint32_t index)
    {
      CHECKED_INDEX(index, 0, bits);
      
      // Set bit
      Mask |= static_cast<mask_t>(1 << index);
    }
  };
  
  
  
  //////////////////////////////////////////////////////////////////////////////////////////
  // wtl::operator <<
  //! Prints the valid bits of a BitSet to the debug console
  //!
  //! \tparam DATA - Bitset data type
  //! 
  //! \param[in,out] &c - Debugging console
  //! \param[in] const& b - Bitset 
  //! \return Console& - Reference to input console
  //////////////////////////////////////////////////////////////////////////////////////////
  template <typename DATA>
  Console& operator << (Console& c, const Bitset<DATA>& b)
  {
    // Print comma separated zero-based indicies of high-bits
    return c << Cons::Grey  << '{' 
             << Cons::White << delimited_range(b.flatten(), ',')
             << Cons::Grey  << '}';
  };


} //namespace wtl


#endif // WTL_BITSET_HPP


