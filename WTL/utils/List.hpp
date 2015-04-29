////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\List.hpp
//! \brief Provides a singly or doubly linked list exposing a simple interface
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_LIST_HPP
#define WTL_LIST_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \enum LinkedListType - Define linked list types
  enum class ListType { Single, Double };

  ///////////////////////////////////////////////////////////////////////////////
  //! \struct List - Linked list providing forward only or bi-directional element navigation
  //! 
  //! \tparam T - Element type
  //! \tparam NAV - Navigation type (Default is single)
  ///////////////////////////////////////////////////////////////////////////////
  template <typename T, ListType NAV = ListType::Single>
  struct List : std::conditional_t<NAV == ListType::Single,std::forward_list<T>,std::list<T>>
  {
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias base - Define base type
    using base = std::conditional_t<NAV == ListType::Single,std::forward_list<T>,std::list<T>>;

    //! \alias value_type - Inherit value type
    using value_type = typename base::value_type;

  protected:
    //! \var bidirectional - Whether list supports bidirectional traversal
    static constexpr bool  bidirectional = (NAV == ListType::Double); 

    //! \var type - Defines whether a singley/doubley linked list
    static constexpr ListType  type = NAV; 

    // -------------------- REPRESENTATION ---------------------
    
    // --------------------- CONSTRUCTION ----------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // List::List
    //! Create empty list
    ///////////////////////////////////////////////////////////////////////////////
    List()
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // List::List
    //! Create list and populate from initializer list
    //! 
    //! \param[in] &&list - List of elements
    ///////////////////////////////////////////////////////////////////////////////
    List(std::initializer_list<T>&& list) : base(std::forward(list))
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // List::~List
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~List()
    {}
    
    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			

    ///////////////////////////////////////////////////////////////////////////////
    // List::empty const
    //! Query whether the list is empty
    ///////////////////////////////////////////////////////////////////////////////
    //using base::empty;
    
    // ----------------------- MUTATORS ------------------------

    ///////////////////////////////////////////////////////////////////////////////
    // List::begin/end
    //! Iterate over the list
    ///////////////////////////////////////////////////////////////////////////////
    /*using base::begin;
    using base::end;*/

    ///////////////////////////////////////////////////////////////////////////////
    // List::clear
    //! Clear the list
    ///////////////////////////////////////////////////////////////////////////////
    //using base::empty;
    
    ///////////////////////////////////////////////////////////////////////////////
    // List::operator += 
    //! Append an item to the list
    //! 
    //! \param[in] const& v - Item
    ///////////////////////////////////////////////////////////////////////////////
    List& operator += (const value_type& v)
    {
      base::push_back(v);
    } 
  };


}



#endif // WTL_LIST_HPP


