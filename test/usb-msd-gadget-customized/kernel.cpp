//
// kernel.cpp
//
// Test for USB Mass Storage Gadget by Mike Messinides
//
// Circle - A C++ bare metal environment for Raspberry Pi
// Copyright (C) 2023-2024  R. Stange <rsta2@o2online.de>
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
#include "kernel.h"
#include <filelogdaemon/filelogdaemon.h>
#include <circle/sched/scheduler.h>
#include <imagefileblockdevice/imagefileblockdevice.h>


#ifndef USB_GADGET_VENDOR_ID
#error "USB_GADGET_VENDOR_ID is not defined!"
#endif
#define STR(x) #x
#define XSTR(x) STR(x)
#pragma message ("USB_GADGET_VENDOR_ID = " XSTR(USB_GADGET_VENDOR_ID))
#define DRIVE "SD:"
#define CONFIG_FILE DRIVE "/config.txt"
#define LOG_FILE DRIVE "/logfile.txt"

LOGMODULE ("kernel");

CKernel::CKernel (void)
:	m_Screen (m_Options.GetWidth (), m_Options.GetHeight ()),
	m_Timer (&m_Interrupt),
	m_Logger (m_Options.GetLogLevel (), &m_Timer),
	m_EMMC (&m_Interrupt, &m_Timer, &m_ActLED),
	m_MSDGadget (&m_Interrupt),
	m_ImageFileBlockDevice (nullptr)
{
	//m_ActLED.Blink (5);	// show we are alive
}

CKernel::~CKernel (void)
{
	    if (m_ImageFileBlockDevice)
        delete m_ImageFileBlockDevice;
}

boolean CKernel::Initialize (void)
{
	boolean bOK = TRUE;

	if (bOK)
	{
		bOK = m_Screen.Initialize ();
		if (bOK)
		{
			LOGNOTE("Build: " __DATE__ " " __TIME__);
		}
	}

	if (bOK)
	{
		bOK = m_Serial.Initialize (115200);
		LOGNOTE("Build: " __DATE__ " " __TIME__);

	}

	if (bOK)
	{
		CDevice *pTarget = m_DeviceNameService.GetDevice (m_Options.GetLogDevice (), FALSE);
		if (pTarget == 0)
		{
			pTarget = &m_Screen;
		}

		bOK = m_Logger.Initialize (pTarget);
	}

	if (bOK)
	{
		bOK = m_Interrupt.Initialize ();
	}


	if (bOK)
	{
		bOK = m_Timer.Initialize ();
	}

	if (bOK)
	{
		bOK = m_EMMC.Initialize ();
	}


	    if (bOK) {
        if (f_mount(&m_FileSystem, DRIVE, 1) != FR_OK) {
            LOGERR("Cannot mount drive: %s", DRIVE);

            bOK = FALSE;
        }
        LOGNOTE("Initialized filesystem");
		LOGNOTE("Build: " __DATE__ " " __TIME__);

    }

	if(bOK)
	{
		bOK = m_MSDGadget.Initialize ();
	}

	return bOK;
}

TShutdownMode CKernel::Run (void)
{
    // Load our config file loader
    CPropertiesFatFsFile Properties(CONFIG_FILE, &m_FileSystem);
    if (!Properties.Load()) {
        LOGERR("Error loading properties from %s (line %u)",
               CONFIG_FILE, Properties.GetErrorLine());
        return ShutdownHalt;
    }

	Properties.SelectSection("msd");
	// Start the file logging daemon
    const char *logfile = Properties.GetString("logfile", nullptr);
    if (logfile) {
        new CFileLogDaemon(logfile);
        LOGNOTE("Started log file daemon");
    }

	LOGNOTE("=====================================");
    LOGNOTE("Welcome to USBMSD");
    LOGNOTE("Compile time: " __DATE__ " " __TIME__);
    LOGNOTE("=====================================");

	const char *imgfile = Properties.GetString("imagefile", nullptr);
    if (imgfile) {
        LOGNOTE("Attempting to open image file: %s", imgfile);
        m_ImageFileBlockDevice = new CImageFileBlockDevice(imgfile, &m_FileSystem);
        if (m_ImageFileBlockDevice->IsReady()) {
            m_MSDGadget.SetDevice(m_ImageFileBlockDevice);
            LOGNOTE("Exposing image file as MSD gadget");
            LOGNOTE("After SetDevice: GetSectorCount=%u, GetBlockSize=%u",
                m_ImageFileBlockDevice->GetSectorCount(),
                m_ImageFileBlockDevice->GetBlockSize());
        } else {
            LOGERR("Failed to open image file for MSD gadget, falling back to SDCard");
            delete m_ImageFileBlockDevice;
            m_ImageFileBlockDevice = nullptr;
            m_MSDGadget.SetDevice(&m_EMMC);
            LOGNOTE("Exposing SDCard as MSD gadget");
        }
    } else {
        LOGERR("No image file specified in config, exposing SDCard as MSD gadget");
        m_MSDGadget.SetDevice(&m_EMMC);
        LOGNOTE("Exposing SDCard as MSD gadget");
    }

	if (m_ImageFileBlockDevice && m_ImageFileBlockDevice->IsReady()) {
    LOGNOTE("Image file size: %u bytes, sectors: %u",
        m_ImageFileBlockDevice->GetFileSize(),
        m_ImageFileBlockDevice->GetSectorCount());
	}

    uint32_t lastYield = m_Timer.GetTicks();

	for (unsigned nCount = 0; 1; nCount++)
	{
		m_MSDGadget.UpdatePlugAndPlay ();

		// must be called from TASK_LEVEL to allow I/O operations
		m_MSDGadget.Update ();

		m_Screen.Rotor (0, nCount);
        uint32_t now = m_Timer.GetTicks();
        if ((now - lastYield) >= 100) // 100ms elapsed
        {
            CScheduler::Get()->Yield();
            lastYield = now;
        }
	}

	return ShutdownHalt;
}
