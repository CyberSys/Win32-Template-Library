////////////////////////////////////////////////////////////////////////////////
//! \file wtl\utils\Queue.hpp
//! \brief Provides a FIFO queue that exposes a simple interface
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_QUEUE_HPP
#define WTL_QUEUE_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  ///////////////////////////////////////////////////////////////////////////////
  //! \struct Queue - Simple FIFO queue
  //! 
  //! \tparam T - Element type
  ///////////////////////////////////////////////////////////////////////////////
  template <typename T>
  struct Queue : protected std::deque<T>
  {
    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias base - Define base type
    using base = std::deque<T>;
  
    // --------------------- CONSTRUCTION ----------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // Queue::Queue
    //! Create empty queue
    ///////////////////////////////////////////////////////////////////////////////
    Queue()
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // Queue::Queue
    //! Create queue and populate from initializer list
    //! 
    //! \param[in] &&list - List of elements
    ///////////////////////////////////////////////////////////////////////////////
    Queue(std::initializer_list<T>&& list) : Elements(std::forward(list))
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // Queue::~Queue
    //! Virtual d-tor
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~Queue()
    {}
    
    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			

    ///////////////////////////////////////////////////////////////////////////////
    // Queue::empty const
    //! Query whether the queue is empty
    ///////////////////////////////////////////////////////////////////////////////
    bool empty()
    {
      return base::empty();
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Queue::peek const
    //! Peek the first element in the queue
    //! 
    //! \return const T& - Immutable reference to first element
    //! 
    //! \throw wtl::logic_error - Queue is empty
    ///////////////////////////////////////////////////////////////////////////////
    const T&  peek() const
    {
      // Ensure not empty
      if (empty())
        throw logic_error(HERE, "Queue is empty");

      // Return front
      return base::front();
    }

    // ----------------------- MUTATORS ------------------------

    ///////////////////////////////////////////////////////////////////////////////
    // Queue::clear
    //! Clear the queue
    ///////////////////////////////////////////////////////////////////////////////
    void clear()
    {
      base::clear();
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // Queue::peek
    //! Peek the first element in the queue
    //! 
    //! \return T& - Mutable reference to first element
    //! 
    //! \throw wtl::logic_error - Queue is empty
    ///////////////////////////////////////////////////////////////////////////////
    T&  peek()
    {
      // Ensure not empty
      if (empty())
        throw logic_error(HERE, "Queue is empty");

      // Return front
      return base::front();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Queue::push
    //! Push element to the base of the queue
    //! 
    //! \param[in] &&... args - Element c-tor arguments
    ///////////////////////////////////////////////////////////////////////////////
    template <typename... ARGS>
    void push(ARGS&&... args)
    {
      base::emplace_front(std::forward<ARGS>(args)...)
    }
    
    
    ///////////////////////////////////////////////////////////////////////////////
    // Queue::pop
    //! Pop element from the front of the queue
    //! 
    //! \return T - First element in the queue
    //! 
    //! \throw wtl::logic_error - Queue is empty
    ///////////////////////////////////////////////////////////////////////////////
    T pop()
    {
      // Ensure not empty
      if (empty())
        throw logic_error(HERE, "Queue is empty");

      // Copy, pop, and return
      T tmp(base::back());
      base::pop_back();
      return tmp;
    }
    
    // -------------------- REPRESENTATION ---------------------
  
  };


}



#endif // WTL_QUEUE_HPP


