//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\List.hpp
//! \brief Provides a linked list exposing a simple interface
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_LIST_HPP
#define WTL_LIST_HPP

#include "wtl/WTL.hpp"
#include <utility>                          //!< std::forward
#include <initializer_list>                 //!< std::initializer_list
#include <list>                             //!< std::list
#include <algorithm>                        //!< std::find_if
#include <functional>                       //!< std::function

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct List - Bi-directional indexed Linked list 
  //! 
  //! \tparam T - Element type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct List : std::list<T>
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias base - Define base type
    using base = std::list<T>;

    //! \alias type - Define own type
    using type = List<T>;

    //! \alias iterator - Inherit iterator type
    using iterator = typename base::iterator;

    //! \alias const_iterator - Inherit immutable iterator type
    using const_iterator = typename base::const_iterator;

    //! \alias value_type - Inherit value type
    using value_type = typename base::value_type;

    // ----------------------------------- REPRESENTATION -----------------------------------
    
    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // List::List
    //! Create empty list
    /////////////////////////////////////////////////////////////////////////////////////////
    List()
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // List::List
    //! Create list and populate from initializer list
    //! 
    //! \param[in] &&list - List of elements
    /////////////////////////////////////////////////////////////////////////////////////////
    List(std::initializer_list<T>&& list) : base(std::forward(list))
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // List::~List
    //! Can be polymorphic
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual ~List()
    {}
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // List::at const
    //! Finds an element by index
    //! 
    //! \param[in] idx - Zero-based element index
    //! \return const_iterator - Immutable element position
    //! 
    //! \throw wtl::out_of_range - [Debug only] Invalid index
    /////////////////////////////////////////////////////////////////////////////////////////
    const_iterator  at(uint32 idx) const 
    {
      CHECKED_INDEX(idx,0,size()+1);    //!< Permit query of position beyond last element

      // Return 'end' if querying beyond last element
      if (idx == size())
        return this->cend();

      // Forward linear search
      auto pos = this->cbegin();
      std::advance(pos, idx);
      return pos;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // List::find_if const
    //! Search entire containiner for an element matching predicate
    //! 
    //! \param[in] pred - Element predicate
    //! \return const_iterator - Immutable element position if found, otherwise 'end'
    /////////////////////////////////////////////////////////////////////////////////////////
    const_iterator  find_if(std::function<bool (const value_type&)> pred) const 
    {
      return std::find_if(this->cbegin(), this->cend(), pred);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // List::operator [] const
    //! Access element by index
    //! 
    //! \param[in] idx - Zero-based element index
    //! \return const value_type& - Immutable element reference 
    //! 
    //! \throw wtl::out_of_range - [Debug only] Invalid index
    /////////////////////////////////////////////////////////////////////////////////////////
    const value_type&  operator [] (uint32 idx) const 
    {
      CHECKED_INDEX(idx,0,size());

      // Lookup element
      return *at(idx);
    }
    
    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // List::at 
    //! Finds an element by index
    //! 
    //! \param[in] idx - Zero-based element index
    //! \return iterator - Mutable element position
    //! 
    //! \throw wtl::out_of_range - [Debug only] Invalid index
    /////////////////////////////////////////////////////////////////////////////////////////
    iterator  at(uint32 idx) 
    {
      CHECKED_INDEX(idx,0,size()+1);    //!< Permit query of position beyond last element

      // Return 'end' if querying beyond last element
      if (idx == size())
        return this->end();

      // Forward linear search
      auto pos = this->begin();
      std::advance(pos, idx);
      return pos;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // List::find_if 
    //! Search entire containiner for an element matching predicate
    //! 
    //! \param[in] pred - Element predicate
    //! \return const_iterator - Mutable element position if found, otherwise 'end'
    /////////////////////////////////////////////////////////////////////////////////////////
    iterator  find_if(std::function<bool (const value_type&)> pred) 
    {
      return std::find_if(this->begin(), this->end(), pred);
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // List::operator [] 
    //! Access element by index
    //! 
    //! \param[in] idx - Zero-based element index
    //! \return value_type& - Mutable element reference 
    //! 
    //! \throw wtl::out_of_range - [Debug only] Invalid index
    /////////////////////////////////////////////////////////////////////////////////////////
    value_type&  operator [] (uint32 idx)
    {
      CHECKED_INDEX(idx,0,size());

      // Lookup element
      return *at(idx);
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // List::operator += 
    //! Append an element to the list
    //! 
    //! \param[in] const& value - Item
    /////////////////////////////////////////////////////////////////////////////////////////
    List& operator += (const value_type& value)
    {
      // Append
      base::push_back(value);
      return *this;
    } 
  };


}



#endif // WTL_LIST_HPP


