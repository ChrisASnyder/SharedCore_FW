

#ifndef __scIMutex_H
#define __scIMutex_H

#ifdef WIN32
#	pragma once
#endif

namespace SharedCore
{
	namespace FreeRTOS
	{
		/// <summary>
		/// This class provides an interface to a basic mutex implementation. Specific implementations will then
		/// be constructed. This is a pure virtual class.
		/// </summary>
		class scMutex
		{
		public:

			/// <summary>
			///	The interface version of this implementation doesn't do anything. It is a placeholder
			/// for the derived classes.
			/// </summary>
			scMutex( void ) {}

			/// <summary>
			/// Virtual destructor
			/// </summary>
			virtual ~scMutex() {}

			/// <summary>
			/// This method will attempt to acquire the mutex using the timeout value. This is primarily
			/// dependent on the specific mutex implementation since some platforms do not support
			/// a timeout wait on mutex acquisition.
			/// </summary>
			virtual int TryAcquire( uint32_t timeout ){ return 0; }

			/// <summary>
			/// This will acquire the mutex and prevent other implementations from using it.
			/// </summary>
			virtual void Acquire( void ) {};

			/// <summary>
			/// This will release the mutex. Other tasks waiting will get released.
			/// </summary>
			virtual void Release(void) {};

			/// <summary>
			/// For the FreeRTOS there is a difference between a non ISR and and ISR acquisition
			/// of a mutex.
			/// </summary>
			virtual void ReleaseISR(void) {};
		};
	}
}
#endif // __scIMutex_H

