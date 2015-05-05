//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\threads\WorkerThread.hpp
//! \brief Background thread
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_WORKER_THREAD_HPP
#define WTL_WORKER_THREAD_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  //! \alias WorkerThread - Uses std::thread
  using WorkerThread = std::thread;

}

#endif  // WTL_WORKER_THREAD_HPP
