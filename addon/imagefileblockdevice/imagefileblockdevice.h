#pragma once

#include <circle/device.h>
#include <fatfs/ff.h>
#include <circle/device.h>

// Inherit from CDevice or CBlockDevice as required by your MSD gadget
class CImageFileBlockDevice : public CDevice
{
public:
    CImageFileBlockDevice(const char* filename, FATFS* pFileSystem);
    ~CImageFileBlockDevice();

    boolean IsReady() const;
    int Read(void *pBuffer, size_t nCount) override;
    int Write(const void *pBuffer, size_t nCount) override;
    boolean ReadSector(void* pBuffer, u32 nLBA, u32 nCount);
    u64 Seek(u64 ullOffset) override;
    boolean WriteSector(const void* pBuffer, u32 nLBA, u32 nCount);
    u32 GetSectorCount() const;
    u32 GetFileSize() const;
    u32 GetBlockSize() const;
    u64 GetBlocks() const { return (u64)GetSectorCount(); }
    u64 GetSize(void) const override;
private:
    FIL m_File;
    bool m_bOK;
};