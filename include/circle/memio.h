//
/// \file memio.h
//
// Circle - A C++ bare metal environment for Raspberry Pi
// Copyright (C) 2014-2025  R. Stange <rsta2@gmx.net>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#ifndef _circle_memio_h
#define _circle_memio_h

#include <circle/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// \brief Read 8-bit value from MMIO address
static inline u8 read8 (uintptr ulAddress)
{
	return *(volatile u8 *) ulAddress;
}

/// \brief Write 8-bit value to MMIO address
static inline void write8 (uintptr ulAddress, u8 uchValue)
{
	*(volatile u8 *) ulAddress = uchValue;
}

/// \brief Read 16-bit value from MMIO address
static inline u16 read16 (uintptr ulAddress)
{
	return *(volatile u16 *) ulAddress;
}

/// \brief Write 16-bit value to MMIO address
static inline void write16 (uintptr ulAddress, u16 usValue)
{
	*(volatile u16 *) ulAddress = usValue;
}

/// \brief Read 32-bit value from MMIO address
static inline u32 read32 (uintptr ulAddress)
{
	return *(volatile u32 *) ulAddress;
}

/// \brief Write 32-bit value to MMIO address
static inline void write32 (uintptr ulAddress, u32 nValue)
{
	*(volatile u32 *) ulAddress = nValue;
}

#if AARCH == 64

/// \brief Read 64-bit value from MMIO address
static inline u64 read64 (uintptr ulAddress)
{
	return *(volatile u64 *) ulAddress;
}

/// \brief Write 64-bit value to MMIO address
static inline void write64 (uintptr ulAddress, u64 nValue)
{
	*(volatile u64 *) ulAddress = nValue;
}

#endif

#ifdef __cplusplus
}
#endif

#endif
