#pragma once

#include <circle/device.h>
#include <fatfs/ff.h>

// Inherit from CDevice or CBlockDevice as required by your MSD gadget
class CImageFileBlockDevice : public CDevice
{
public:
    CImageFileBlockDevice(const char* filename, FATFS* pFileSystem);
    ~CImageFileBlockDevice();

    boolean IsReady() const { return m_bOK; }
    boolean Read(u32 nStartSector, void* pBuffer, u32 nCount);
    boolean Write(u32 nStartSector, const void* pBuffer, u32 nCount);
    boolean ReadSector(void* pBuffer, u32 nLBA, u32 nCount);

    boolean WriteSector(const void* pBuffer, u32 nLBA, u32 nCount);
    u32 GetSectorCount() const;
    
private:
    FIL m_File;
    bool m_bOK;
};