//==============================================================================
//          UNCLASSIFIED
//==============================================================================
//          © Copyright Common Ground Software Solutions 2014
//          All Rights Reserved
//
// File Name:		scBlockMemoryIF.h
// Created By:		Christopher Snyder
// Creation Date:	23-Oct-2014 
// $Id: $
//
//==============================================================================
//          UNCLASSIFIED
//==============================================================================


#if !defined(__scBlockMemoryIF_H__INCLUDED_)
#define __scBlockMemoryIF_H__INCLUDED_

#include "../scDeviceGeneric.h"

namespace SharedCore
{
	namespace HAL
	{
		/// <summary>
		/// Many devices in an embedded system exchange data using blocks of memory, not
		/// single byte transactions. For example, the SDIO device must always read and
		/// write data in blocks of 512 bytes. Theses devices will internally know their
		/// block size and the data transaction will always have an address and block count.
		/// This class provides a common interface for those types of devices.
		/// </summary>
		class scBlockMemoryIF : public SharedCore::scDeviceGeneric
		{

		public:
			virtual ~scBlockMemoryIF();

			/// <summary>
			/// Perform a block read operation to transfer the data from the device to the
			/// pointer provided using the specified number of blocks. The return code will be
			/// unique to every driver, in most cases the ERROR_SUCCESS flag will be returned
			/// on a successful copy.
			/// </summary>
			/// <param name="pData">Pointer to receive the requested block of data.</param>
			/// <param name="nDataLength">The number of bytes available in the memory pointer
			/// provided. This value is not blocks but bytes. </param>
			/// <param name="nStartAddress">The address relative to the device for the memory
			/// to copy.</param>
			/// <param name="nBlockCount">The number of blocks to transfer</param>
			virtual uint32_t Read(uint8_t* pData, uint32_t nDataLength, uint32_t nStartAddress, uint32_t nBlockCount) = 0;

			/// <summary>
			/// Perform a block write operation to transfer the data to the device from pointer
			/// provided using the specified number of blocks. The return code will be unique
			/// to every driver, in most cases the ERROR_SUCCESS flag will be returned on a
			/// successful copy.
			/// </summary>
			/// <param name="pData">Pointer to receive the requested block of data.</param>
			/// <param name="nStartAddress">The address relative to the device for the memory
			/// to copy.</param>
			/// <param name="nBlockCount">The number of blocks to transfer</param>
			virtual uint32_t Write(const uint8_t* pData, uint32_t nStartAddress, uint32_t nBlockCount) = 0;

			/// <summary>
			/// Returns the block size in bytes the drive will use by default.
			/// </summary>
			virtual uint32_t DefaultBlockSize(void) const = 0;

			/// <summary>
			/// Obtains the total amount of space available in the device in BYTES. Note some
			/// devices may not be capable or reporting the 64 bit value.
			/// </summary>
			virtual const uint64_t TotalSizeBytes(void)  const = 0;

			/// <summary>
			/// Reports the total size of the storage using the Units of blocks.
			/// </summary>
			virtual uint32_t TotalSizeBlocks(void) const = 0;

			/// <summary>
			/// If the device permits, bulk erase operations are managed through this method.
			/// In many cases the bulk erase is very fast compared to byte operations or may be
			/// required before data can be written.
			/// </summary>
			virtual uint32_t BulkErase( uint32_t nStartSector, uint32_t nCount, bool bWait ) = 0;

			/// <summary>
			/// This operation will select internal components if available. Some drivers can
			/// support multiple interfaces and a single driver can switch between them, such
			/// as a disk drive. Returns ERROR_SC_DEVICE_FEATURE_UNSUPPORTED if Select feature
			/// not supported ERROR_SUCCESS if it is supported.
			/// </summary>
			virtual uint32_t Select( uint32_t nItem );

			/// <summary>
			/// Obtain the index of the currently selected item.
			/// </summary>
			virtual uint32_t Selected(void) const;

		protected:
			/// <summary>
			/// Construct the class using the Guid
			/// </summary>
			/// <param name="descriptor">Describes the descriptor for this device.</param>
			scBlockMemoryIF( const scDeviceDescriptor& descriptor );

			/// <summary>
			/// Construct the class using the Guid and one dependent
			/// </summary>
			/// <param name="descriptor">Describes the descriptor for this device.</param>
			/// <param name="dependant">the sole dependent of the device.</param>
			scBlockMemoryIF( const scDeviceDescriptor& descriptor, const scDeviceDescriptor& dependant );

			/// <summary>
			/// Construct the class using the Guid and multiple dependents
			/// </summary>
			/// <param name="descriptor">Describes the descriptor for this device.</param>
			/// <param name="nCount">Number of dependents listed.</param>
			scBlockMemoryIF( const scDeviceDescriptor& descriptor, int nCount, ... );

		};

	}

}
#endif // !defined(__scBlockMemoryIF_H__INCLUDED_)
