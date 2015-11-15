//////////////////////////////////////////////////////////////////////////////////////////
//! \file wtl\threads\MessageThread.hpp
//! \brief Provides a thread with a message pump
//! \date 6 March 2015
//! \author Nick Crowley
//! \copyright Nick Crowley. All rights reserved.
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef WTL_MESSAGE_THREAD_HPP
#define WTL_MESSAGE_THREAD_HPP

#include <wtl/WTL.hpp>
#include <wtl/traits/EncodingTraits.hpp>        //!< Encoding
#include <wtl/platform/WindowFlags.hpp>         //!< ShowWindowFlags
#include <wtl/utils/Exception.hpp>              //!< exception
#include <wtl/io/Console.hpp>                   //!< Console
#include <wtl/windows/MessageBox.hpp>           //!< MessageBox

//! \namespace wtl - Windows template library
namespace wtl
{
  //////////////////////////////////////////////////////////////////////////////////////////
  //! \struct MessageThread - Thread with a message pump
  //! 
  //! \tparam ENC - Character encoding type 
  //////////////////////////////////////////////////////////////////////////////////////////
  template <Encoding ENC, typename WINDOW>
  struct MessageThread //: std::thread
  {
    static_assert(ENC == WINDOW::encoding, "Message thread and window must have the same character encoding");

    // ---------------------------------- TYPES & CONSTANTS ---------------------------------
  
    //! \alias base - Define base type
    //using base = std::thread;
    
    //! \var encoding - Define window character encoding
    static constexpr Encoding encoding = ENC;

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
    
    // ----------------------------------- REPRESENTATION -----------------------------------
  protected:
    ::HINSTANCE       Instance;   //!< Module instance
    DialogCollection  Dialogs;    //!< Currently active modeless dialogs
    window_t          Window;     //!< Main thread window

    // ------------------------------------ CONSTRUCTION ------------------------------------
	public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // MessageThread::MessageThread
    //! Creates a message pump thread
    //! 
    //! \param[in] instance - Instance handle
    /////////////////////////////////////////////////////////////////////////////////////////
    MessageThread(::HINSTANCE instance) : Instance(instance),
                                        Window(instance)
    {}
    
    // -------------------------------- COPY, MOVE & DESTROY --------------------------------
  public:
    DISABLE_COPY(MessageThread);      //!< Cannot be copied
    ENABLE_MOVE(MessageThread);       //!< Can be moved
    ENABLE_POLY(MessageThread);      //!< Can be polymorphic

    // ----------------------------------- STATIC METHODS -----------------------------------

    // ---------------------------------- ACCESSOR METHODS ----------------------------------			

    // ----------------------------------- MUTATOR METHODS ----------------------------------
  public:
    /////////////////////////////////////////////////////////////////////////////////////////
    // MessageThread::addDialog
    //! Informs the thread a dialog has been created
    //! 
    //! \return bool - True iff unique
    /////////////////////////////////////////////////////////////////////////////////////////
    bool  addDialog(window_t* dlg)
    {
      Dialogs.push_back(dlg);
      return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // MessageThread::removeDialog
    //! Informs the thread a dialog has been closed
    //! 
    //! \return bool - True iff found and removed
    /////////////////////////////////////////////////////////////////////////////////////////
    bool  removeDialog(window_t* dlg)
    {
      Dialogs.erase( std::find(Dialogs.begin(), Dialogs.end(), dlg) );
      return true;
    }
    
    /////////////////////////////////////////////////////////////////////////////////////////
    // MessageThread::operator()
    //! Executes the thread logic (a message pump)
    //! 
    //! \return int32_t - Thread return code
    /////////////////////////////////////////////////////////////////////////////////////////
    int32_t operator()()
    {
      try
      {
        // [EVENT] Raise 'onStart'
        onThreadStart();

        // Ensure window exists
        if (!Window.exists())
          throw platform_error(HERE, "Failed to initialise application");

        // Run message pump
        int32_t ret = messagePump();

        // [EVENT] Raise 'onExit'
        onThreadExit();
        return ret;
      }
      catch (std::exception& e)
      {
        errorBox<encoding>(Window, caught_exception("Unable to dispatch message", HERE, e));
        return -1;
      }
      catch (...)
      {
        errorBox<encoding>(Window, caught_exception("An unrecognised terminal error has occurred, the program will now exit.", HERE));
        return -2;
      }
    }

  protected:
    /////////////////////////////////////////////////////////////////////////////////////////
    // MessageThread::onThreadStart
    //! Called once before message pump starts
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual void onThreadStart()
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // MessageThread::onThreadExit
    //! Called once after message pump finishes
    /////////////////////////////////////////////////////////////////////////////////////////
    virtual void onThreadExit()
    {}

    /////////////////////////////////////////////////////////////////////////////////////////
    // MessageThread::messagePump
    //! Thread message pump
    //! 
    //! \return int32_t - Return code
    /////////////////////////////////////////////////////////////////////////////////////////
    int32_t messagePump()
    {
      //! \var dispatchMessage - Functor for 'DispatchMessage'
      static const auto dispatchMessage = WinAPI<ENC>::DispatchMessage;

      //! \var getMessage - Functor for 'GetMessage'
      static const auto getMessage = WinAPI<ENC>::GetMessage;

      //! \var isDialogMessage - Functor for 'isDialogMessage'
      static const auto isDialogMessage = WinAPI<ENC>::IsDialogMessage;

      //! \var translateAccelerator - Functor for 'TranslateAccelerator'
      static const auto translateAccelerator = WinAPI<ENC>::TranslateAccelerator;

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
  };

}

#endif  // WTL_MESSAGE_THREAD_HPP
