//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Queue.hpp
//! \brief Provides a FIFO queue that exposes a simple interface
//! \date 25 October 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_QUEUE_HPP
#define WTL_QUEUE_HPP

#include <wtl/WTL.hpp>
#include <wtl/utils/Exception.hpp>          //!< Exceptions
#include <utility>                          //!< std::forward
#include <initializer_list>                 //!< std::initializer_list
#include <deque>                            //!< std::deque

//! \namespace wtl - Windows template library
namespace wtl
{
  /////////////////////////////////////////////////////////////////////////////////////////
  //! \struct Queue - Simple FIFO queue
  //! 
  //! \tparam T - Element type
  /////////////////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct Queue 
  {
    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias type - Define own type
    using type = Queue<T>;
  
    //! \alias iterator - Inherit iterator type
    using iterator = typename std::deque<T>::iterator;

    //! \alias const_iterator - Inherit immutable iterator type
    using const_iterator = typename std::deque<T>::const_iterator;

    //! \alias value_type - Inherit value type
    using value_type = typename std::deque<T>::value_type;

    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    std::deque<T>  Items;      //!< Item storage

    // ------------------------------------ CONSTRUCTION ------------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // Queue::Queue
    //! Create empty queue
    /////////////////////////////////////////////////////////////////////////////////////////
    Queue() = default;

    /////////////////////////////////////////////////////////////////////////////////////////
    // Queue::Queue
    //! Create queue and populate from initializer list
    //! 
    //! \param[in] list - List of elements
    /////////////////////////////////////////////////////////////////////////////////////////
    Queue(std::initializer_list<T> list) : Items(std::forward(list))
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    ENABLE_COPY(Queue);        //!< Can be copied
    ENABLE_MOVE(Queue);        //!< Can be moved
    ENABLE_POLY(Queue);        //!< Can be moved
    
    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Queue::begin/end const
    //! Retrieve immutable start/end iterators
    //!
    //! \return const_iterator - Position of first/last elements
    /////////////////////////////////////////////////////////////////////////////////////////
    const_iterator begin() const  { return Items.cbegin(); }
    const_iterator end() const    { return Items.cend();   }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Queue::empty const
    //! Query whether the queue is empty
    /////////////////////////////////////////////////////////////////////////////////////////
    bool empty() const
    {
      return Items.empty();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Queue::peek const
    //! Peek the first element in the queue
    //! 
    //! \return const value_type& - Immutable reference to first element
    //! 
    //! \throw wtl::logic_error - Queue is empty
    /////////////////////////////////////////////////////////////////////////////////////////
    const value_type&  peek() const
    {
      // Ensure not empty
      if (empty())
        throw logic_error(HERE, "Cannot peek an empty queue");

      // Return front
      return Items.front();
    }

    // ----------------------------------- MUTATOR METHODS ----------------------------------
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Queue::begin/end 
    //! Retrieve mutable start/end iterators
    //!
    //! \return iterator - Position of first/last elements
    /////////////////////////////////////////////////////////////////////////////////////////
    iterator begin()  { return std::begin(Items); }
    iterator end()    { return std::end(Items);   }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Queue::clear
    //! Clear the queue
    /////////////////////////////////////////////////////////////////////////////////////////
    void clear()
    {
      Items.clear();
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Queue::peek
    //! Peek the first element in the queue
    //! 
    //! \return value_type& - Mutable reference to first element
    //! 
    //! \throw wtl::logic_error - Queue is empty
    /////////////////////////////////////////////////////////////////////////////////////////
    value_type&  peek()
    {
      // Ensure not empty
      if (empty())
        throw logic_error(HERE, "Cannot peek an empty queue");

      // Return front
      return Items.front();
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // Queue::push
    //! Construct element at the back of the queue
    //! 
    //! \param[in] &&... args - Element constructor arguments
    /////////////////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    void push(ARGS&&... args)
    {
      // Add to back
      Items.emplace_back(std::forward<ARGS>(args)...);
    }
    
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // Queue::pop
    //! Pop element from the front of the queue
    //! 
    //! \throw wtl::logic_error - Queue is empty
    /////////////////////////////////////////////////////////////////////////////////////////
    void pop()
    {
      // Ensure not empty
      if (empty())
        throw logic_error(HERE, "Cannot pop empty queue");

      // Pop from front
      Items.pop_front();
    }
    
  };


}



#endif // WTL_QUEUE_HPP


