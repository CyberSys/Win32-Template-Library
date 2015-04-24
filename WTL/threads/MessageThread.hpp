////////////////////////////////////////////////////////////////////////////////
//! \file wtl\threads\MessageThread.hpp
//! \brief Provides a thread with a message pump
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MESSAGE_THREAD_HPP
#define WTL_MESSAGE_THREAD_HPP

#include "wtl/WTL.hpp"

//! \namespace wtl - Windows template library
namespace wtl
{
  ////////////////////////////////////////////////////////////////////////////////
  //! \struct MessageThread - Thread with a message pump
  //! 
  //! \tparam ENC - Character encoding type 
  ////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename WINDOW>
  struct MessageThread //: std::thread
  {
    static_assert(ENC == WINDOW::encoding, "Message thread and window must have the same character encoding");

    // ------------------- TYPES & CONSTANTS -------------------
  
    //! \alias base - Define base type
    //using base = std::thread;

    //! \alias window_t - Define window type
    using window_t = WINDOW;

    //! \alias DialogCollection - Define dialog windows collection type
    using DialogCollection = typename window_t::WindowCollection;

    //! \enum PumpState - Define message pump states
    enum class PumpState
    {
      Idle,           //!< Pump idle
      Running,        //!< Pumping messages normally
      ModalLoop,      //!< Pumping within modal menu/dialog/msgbox loop
      //MenuModal,      //!< Pumping within modal menu loop
      //DialogModal,    //!< Pumping within modal dialog loop
      //MsgBoxModal,    //!< Pumping within modal msgbox loop
    };
    
    // --------------------- CONSTRUCTION ----------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // MessageThread::MessageThread
    //! Creates a message pump thread
    //! 
    //! \param[in] instance - Instance handle
    ///////////////////////////////////////////////////////////////////////////////
    MessageThread(HINSTANCE instance) : Instance(instance),
                                        Window(instance)
    {}
    
    // ------------------------ STATIC -------------------------

    // ---------------------- ACCESSORS ------------------------			

    // ----------------------- MUTATORS ------------------------
  public:
    ///////////////////////////////////////////////////////////////////////////////
    // MessageThread::addDialog
    //! Informs the thread a dialog has been created
    //! 
    //! \return bool - True iff unique
    ///////////////////////////////////////////////////////////////////////////////
    bool  addDialog(window_t* dlg)
    {
      Dialogs.push_back(dlg);
      return true;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // MessageThread::removeDialog
    //! Informs the thread a dialog has been closed
    //! 
    //! \return bool - True iff found and removed
    ///////////////////////////////////////////////////////////////////////////////
    bool  removeDialog(window_t* dlg)
    {
      Dialogs.erase( std::find(Dialogs.begin(), Dialogs.end(), dlg) );
      return true;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    // MessageThread::operator()
    //! Executes the thread logic (a message pump)
    //! 
    //! \return int32 - Thread return code
    ///////////////////////////////////////////////////////////////////////////////
    int32 operator()()
    {
      try
      {
        // [EVENT] Raise 'onStart'
        onThreadStart();

        // Ensure window exists
        if (!Window.exists())
          throw platform_error(HERE, "Failed to initialise application");

        // Run message pump
        int32 ret = messagePump();

        // [EVENT] Raise 'onExit'
        onThreadExit();
        return ret;
      }
      catch (wtl::exception& e)
      {
        cdebug.log(HERE, e);
        errorBox(Window, c_arr("Program Error"), e);
        return -1;
      }
      catch (std::exception& e)
      {
        cdebug.log(HERE, e);
        errorBox(Window, c_arr("Program Error"), e);
        return -1;
      }
      catch (...)
      {
        cdebug << wtl::Cons::Error << "Unknown exception" << wtl::endl;
        errorBox(Window, c_arr("Program Error"), c_arr("An unrecognised terminal error has occurred, the program will now exit."));
        return -2;
      }
    }

  protected:
    ///////////////////////////////////////////////////////////////////////////////
    // MessageThread::onThreadStart
    //! Called once before message pump starts
    ///////////////////////////////////////////////////////////////////////////////
    virtual void onThreadStart()
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // MessageThread::onThreadExit
    //! Called once after message pump finishes
    ///////////////////////////////////////////////////////////////////////////////
    virtual void onThreadExit()
    {}

    ///////////////////////////////////////////////////////////////////////////////
    // MessageThread::messagePump
    //! Thread message pump
    //! 
    //! \return int32 - Return code
    ///////////////////////////////////////////////////////////////////////////////
    int32 messagePump()
    {
      //! \var dispatchMessage - Functor for 'DispatchMessage'
      static const auto dispatchMessage = getFunc<ENC>(::DispatchMessageA,::DispatchMessageW);

      //! \var getMessage - Functor for 'GetMessage'
      static const auto getMessage = getFunc<ENC>(::GetMessageA,::GetMessageW);

      //! \var isDialogMessage - Functor for 'isDialogMessage'
      static const auto isDialogMessage = getFunc<ENC>(::IsDialogMessageA,::IsDialogMessageW);

      //! \var translateAccelerator - Functor for 'TranslateAccelerator'
      static const auto translateAccelerator = getFunc<ENC>(::TranslateAcceleratorA,::TranslateAcceleratorW);

      MSG msg;

      // Show window
      Window.show(ShowWindowFlags::Show);
      Window.update();

      // Retrieve next message for any window
      while (getMessage(&msg, nullptr, 0ul, 0ul))
      {
        // [EXISTS] 
        //if (Window && Window->exists())
        //{
        //  // [ACCELERATOR] Pass all accelerators to main window
        //  if (translateAccelerator(*Window, activeAccelerators, &msg))
        //    continue;

        //  // [DIALOG] Translate accelerators or dispatch to dialog
        //  if (Dialogs.contains(msg.hwnd))
        //    if (translateAccelerator(msg.hwnd, activeAccelerators, &msg)
        //     || isDialogMessage(msg.hwnd, &msg))
        //     continue;

        //  // TODO: Property sheets
        //}

        // Translate and dispatch to target
        ::TranslateMessage(&msg);
        dispatchMessage(&msg);
      }

      return msg.wParam;
    }
    
    // -------------------- REPRESENTATION ---------------------
  protected:
    HINSTANCE         Instance;   //!< Module instance
    DialogCollection  Dialogs;    //!< Currently active modeless dialogs
    window_t          Window;     //!< Main thread window
  };

}

#endif  // WTL_MESSAGE_THREAD_HPP
