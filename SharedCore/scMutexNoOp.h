

#ifndef __scMutexNoOp_H
#define __scMutexNoOp_H

#include "scIMutex.h"

#ifdef WIN32
#	pragma once
#endif

namespace SharedCore
{
	/// <summary>
	/// This implements a stub for the mutex that does nothing. This allows classes that have an optional
	/// locking feature to use this class in cases where it isn't need and by default.
	/// </summary>
	class scMutexNoOp : public scIMutex
	{
	public:

		/// <summary>
		///	The interface version of this implementation doesn't do anything. It is a placeholder
		/// for the derived classes.
		/// </summary>
		scMutexNoOp( void ) 
			: scIMutex()
		{}

		/// <summary>
		/// Virtual destructor
		/// </summary>
		virtual ~scMutexNoOp() 
		{}

		/// <summary>
		/// This method will attempt to acquire the mutex using the timeout value. This is primarily
		/// dependent on the specific mutex implementation since some platforms do not support
		/// a timeout wait on mutex acquisition.
		/// </summary>
		virtual int TryAcquire( uint32_t timeout ) 
		{ return 1; }

		/// <summary>
		/// This will acquire the mutex and prevent other implementations from using it.
		/// </summary>
		virtual void Acquire( void ) 
		{}

		/// <summary>
		/// This will release the mutex. Other tasks waiting will get released.
		/// </summary>
		virtual void Release(void) 
		{}

		/// <summary>
		/// For the FreeRTOS there is a difference between a non ISR and and ISR acquisition
		/// of a mutex.
		/// </summary>
		virtual void ReleaseISR(void)
		{}
	};

}	// Namespace SharedCore

#endif // __scMutexNoOp_H

