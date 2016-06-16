//==============================================================================
//          UNCLASSIFIED
//==============================================================================
//          © Copyright Common Ground Software Solutions 2014
//          All Rights Reserved
//
// File Name:		sctEventIODriver.h
// Created By:		Christopher Snyder
// Creation Date:	25-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCTEVENTIODRIVER_H__INCLUDED_)
#define __SCTEVENTIODRIVER_H__INCLUDED_

#include "scDeviceGeneric.h"
#include "scEvent.h"

#define IDLE_TIMEOUT_DEFAULT	(0xFFFF)

namespace SharedCore
{
	namespace HAL
	{
		typedef enum
		{
			scError,
			scStartup,
			scIdle,
			scReceiveData,
			scTransmitComplete,
			scOverflow,
			scUnderflow,
			scRejected,
		} scDataEvents_t;


		/// <summary>
		/// Provides the interface for the event activity.
		/// </summary>
		template<class Base_T>
		class sctEventIODriver : public Base_T
		{
		public:

			/// <summary>
			/// Event handler function pointer definition
			/// source 		- Subject - the object which fired the event.
			/// eventArg 	- The event argument
			/// context		- Context information, which a subscriber needs to get with an event notification
			/// 					Usually, this can be a pointer to the subscriber object itself.
			/// <summary>
			typedef void (*EventHandler)(sctEventIODriver *source, scDataEvents_t eventArg, void* context);


			virtual ~sctEventIODriver();

			/// <summary>
			/// Perform one time initialization activity. This is called by the Device Manager
			/// during the application startup and this method is only ever called once. The
			/// Configure/Unconfigure methods are used to change the resource usage.
			/// </summary>
			/// <param name="pDm"></param>
			virtual void Initialize(scDeviceManager* pDm)
			{
				Base_T::Initialize( pDm );
			}

			/// <summary>
			/// Register the callback method with the event handler
			/// </summary>
			void Subscribe( EventHandler handler, void* context = 0);

			/// <summary>
			/// Register the callback method with the event handler
			/// </summary>
			void Unsubscribe(EventHandler handler, void* context = 0);

			/// <summary>
			/// Changes the delay between idle notification events.
			/// </summary>
			/// <param name="nTimeout">Timeout in milliseconds</param>
			uint16_t SetIdleNotifyTimeout(uint16_t nTimeout)
			{ 
				uint16_t nResult = _nIdleTimeout; 
				_nIdleTimeout = nTimeout;
				return nResult;
			}

			/// <summary>
			/// Changes the delay between idle notification events.
			/// </summary>
			uint16_t GetIdleNotifyTimeout(void) const
			{
				return _nIdleTimeout;
			}

		protected:
			/// <summary>
			/// Construct the class using the Guid
			/// </summary>
			/// <param name="descriptor">Describes the descriptor for this device.</param>
			sctEventIODriver(const scDeviceDescriptor& descriptor);

			/// <summary>
			/// Construct the class using the Guid and multiple dependants
			/// </summary>
			/// <param name="descriptor">Describes the descriptor for this device.</param>
			/// <param name="nCount">Number of dependants listed.</param>
			sctEventIODriver(const scDeviceDescriptor& descriptor, int nCount, ... );

			/// <summary>
			/// Construct the class using the Guid and one dependant
			/// </summary>
			/// <param name="descriptor">Describes the descriptor for this device.</param>
			/// <param name="dependant">the sole dependant of the device.</param>
			sctEventIODriver(const scDeviceDescriptor& descriptor, const scDeviceDescriptor& dependant);

		protected:
			/// <summary>
			/// Subject-observer system for the callback registration. This is public to make event
			/// registration easier. I could create a pass-through call, but I don't want to. The
			/// Pointer is used instead of a aggregated class because of the need for the this
			/// pointer in the constructor.
			/// </summary>
			scEvent<sctEventIODriver, scDataEvents_t>		_event;

			uint16_t										_nIdleTimeout;
		};


//////////////////////////////////////////////////////////////////////////////////////
/// Public Methods
//////////////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Destructor. This will use the internal allocator to release all the memory used.
		/// </summary>
		template<class Base_T>
		sctEventIODriver<Base_T>::~sctEventIODriver()
		{
		}


//////////////////////////////////////////////////////////////////////////////////////
/// Protected Methods
//////////////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// Construct the class using the Guid
		/// </summary>
		/// <param name="descriptor">Describes the descriptor for this device.</param>
		template<class Base_T>
		sctEventIODriver<Base_T>::sctEventIODriver(const scDeviceDescriptor& descriptor)
			: Base_T( descriptor )
			, _event(NULL)
			, _nIdleTimeout(IDLE_TIMEOUT_DEFAULT)
		{
			_event.SetSource( this );
		}

		/// <summary>
		/// Construct the class using the Guid and multiple dependants
		/// </summary>
		/// <param name="descriptor">Describes the descriptor for this device.</param>
		/// <param name="nCount">Number of dependants listed.</param>
		/// <param name="prm1"></param>
		template<class Base_T>
		sctEventIODriver<Base_T>::sctEventIODriver(const scDeviceDescriptor& descriptor, int nCount, ... )
			: Base_T( descriptor )
			, _event(NULL)
			, _nIdleTimeout(IDLE_TIMEOUT_DEFAULT)
		{
			va_list vl;
			va_start( vl, nCount );
			Base_T::AddDependants( vl, nCount );
			va_end(vl);

			_event.SetSource( this );
		}

		/// <summary>
		/// Construct the class using the Guid and one dependant
		/// </summary>
		/// <param name="descriptor">Describes the descriptor for this device.</param>
		/// <param name="dependant">the sole dependant of the device.</param>
		template<class Base_T>
		sctEventIODriver<Base_T>::sctEventIODriver(const scDeviceDescriptor& descriptor, const scDeviceDescriptor& dependant)
			: Base_T( descriptor, dependant )
			, _event(NULL)
			, _nIdleTimeout(IDLE_TIMEOUT_DEFAULT)
		{
			_event.SetSource( this );
		}

		
		/// <summary>
		/// Register the callback method with the event handler
		/// </summary>
		template<class Base_T>
		void sctEventIODriver<Base_T>::Subscribe( EventHandler handler, void* context)
		{
			_event.Subscribe( handler, context );
		}

		/// <summary>
		/// Register the callback method with the event handler
		/// </summary>
		template<class Base_T>
		void sctEventIODriver<Base_T>::Unsubscribe(EventHandler handler, void* context)
		{
			_event.Unsubscribe( handler, context );
		}
	}

}
#endif // !defined(__SCTEVENTIODRIVER_H__INCLUDED_)
