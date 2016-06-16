

#ifndef __scISemaphore_H
#define __scISemaphore_H

#ifdef _WIN32
#	pragma once
#endif

namespace SharedCore
{
	namespace FreeRTOS
	{

		/// <summary>
		/// This class provides an interface to a basic semaphore implementation. Specific implementations will then
		/// be constructed based on the core library in use.
		/// </summary>
		class scSemaphore
		{
		public:

			/// <summary>
			///	The interface version of this implementation doesn't do anything. It is a placeholder
			/// for the derived classes.
			/// </summary>
			scSemaphore( void ) {}

			/// <summary>
			/// Virtual destructor
			/// </summary>
			virtual ~scSemaphore() {}

			/// <summary>
			/// This method will wait indefinitely until the semaphore is released.
			/// The return is true when the semaphore is acquired. False if it is released
			/// due to an error of some sort.
			/// </summary>
			virtual bool Wait( void ) { return true; }

			/// <summary>
			/// Try to acquire the semaphore within the timeout. The value is usually milliseconds
			/// The return is True if acquired, false if not acquired.
			/// </summary>
			virtual bool Wait( uint32_t timeout ) { return true; }

			/// <summary>
			/// This will release the semaphore causing any waiting tasks to be released.
			/// </summary>
			virtual void Release(void) { return; }

			/// <summary>
			/// For FreeRTOS, this is the call needed when inside an ISR routine.
			/// </summary>
			virtual void ReleaseISR(void)  { return; }
		};
	}
} // namespace SharedCore::FreeRTOS

#endif // __scISemaphore_H

