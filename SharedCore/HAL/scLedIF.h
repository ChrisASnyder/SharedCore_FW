//==============================================================================
//          UNCLASSIFIED
//==============================================================================
//          © Copyright Common Ground Software Solutions 2014
//          All Rights Reserved
//
// File Name:		scLedIF.h
// Created By:		Christopher Snyder
// Creation Date:	25-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__SCLEDIF_H__INCLUDED_)
#define __SCLEDIF_H__INCLUDED_

#include <vector>
#include "../scDeviceGeneric.h"

namespace SharedCore
{
	namespace HAL
	{
		/// <summary>
		/// This interface is a container for 1 or more LED (Light Emitting Diode) that is
		/// attached to a single GPIO pin. Each LED then as a numerical identifier to allow
		/// it to be turned on and off through the illuminate method. These simple have an
		/// on/off state and no intensity level associated with it. The main idea of this
		/// class is to provide a simple and common interface to controlling LED functions
		/// independent of the actual underlying setup.
		/// </summary>
		class scLedIF : public SharedCore::scDeviceGeneric
		{
		public:
			typedef enum
			{

				/// <summary>
				/// The LED is not displaying any light
				/// </summary>
				scOff,

				/// <summary>
				/// The LED is displaying light.
				/// </summary>
				scOn
			} LedState_t;

			/// <summary>
			/// Default constructor.
			/// </summary>
			virtual ~scLedIF();

			/// <summary>
			/// Perform the initialization required for the driver to control the LED pin and
			/// port.
			/// </summary>
			/// <param name="pDm">Pointer to the device manager in case it's needed.</param>
			virtual void Initialize( scDeviceManager* pDm ) = 0;

			/// <summary>
			/// Turn the Led on or off.
			/// </summary>
			/// <param name="nLedID">The common name identifier for the LED being controlled.
			/// </param>
			/// <param name="nState">The desired state of the LED</param>
			virtual void Illuminate(uint32_t nLedID, LedState_t nState);

			/// <summary>
			/// Returns the number of  LED's known to the driver.
			/// </summary>
			uint32_t Count(void) const;

			/// <summary>
			/// Gets the LED descriptor assigned to the index.
			/// </summary>
			/// <param name="nIndex">The array index.</param>
			uint32_t GetId( uint32_t nIndex ) const;

		protected:

			typedef struct
			{
				/// <summary>
				/// A number used to identify the LED.
				/// </summary>
				uint32_t			_nLedId;

				/// <summary>
				/// The microcontroller port
				/// </summary>
				void*				_pPort;

				/// <summary>
				/// The microcontroller pin on the above port
				/// </summary>
				uint16_t			_nPin;

				/// <summary>
				/// Does the logic need to be inverted for proper illumination state.
				/// </summary>
				bool				_bInvert;
			} LedDescriptor_t;


			/// <summary>
			/// Constructor as protected since this class must be sub classed.
			/// </summary>
			/// <param name="dd">The descriptor for the driver.</param>
			scLedIF( const scDeviceDescriptor& dd );

			/// <summary>
			/// Adds a new LED definition to the list of known LED's. Sub classed drivers
			/// should call this method during the initialize call to add all the known ports
			/// to the driver.
			/// </summary>
			/// <param name="nLed">The identifier for the led being added.</param>
			/// <param name="pPort">generic pointer for the port identifier</param>
			/// <param name="nPin">the pin being used for the led on the specified port.</param>
			uint32_t Add(uint32_t nLed, void* pPort, uint16_t nPin);

			/// <summary>
			/// Adds a new LED definition to the list of known LED's. Sub classed drivers
			/// should call this method during the initialize call to add all the known ports
			/// to the driver.
			/// </summary>
			/// <param name="nLed">The identifier for the led being added.</param>
			/// <param name="pPort">generic pointer for the port identifier</param>
			/// <param name="nPin">the pin being used for the led on the specified port.
			/// </param>
			/// <param name="bInverted">Depending on the hardware configuration, the logic for
			/// an illuminated pin may be inverted.</param>
			uint32_t Add( uint32_t nLed, void* pPort, uint16_t nPin, bool bInverted );

			/// <summary>
			/// Sub classed implementations must define how the LED is turned on and off.
			/// </summary>
			/// <param name="pLed">The LED definition to control.</param>
			/// <param name="nState">The state the LED should be placed in.</param>
			virtual void ChangeState(const LedDescriptor_t* pLed, LedState_t nState) =0;

			/// <summary>
			/// This is a vector of all the LED's defined in the system.
			/// </summary>
			uint32_t Add( const LedDescriptor_t* pItem );

		private:
			/// <summary>
			/// This is a vector of all the LED's defined in the system.
			/// </summary>
			vector<const LedDescriptor_t*>					_Leds;

		};

	}

}
#endif // !defined(__SCLEDIF_H__INCLUDED_)
