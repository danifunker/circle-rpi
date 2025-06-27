#include "imagefileblockdevice.h"
#include <string.h>

CImageFileBlockDevice::CImageFileBlockDevice(const char* filename, FATFS* /*pFileSystem*/)
: m_bOK(false)
{
    memset(&m_File, 0, sizeof(m_File));
    if (f_open(&m_File, filename, FA_READ | FA_WRITE) == FR_OK)
        m_bOK = true;
}

CImageFileBlockDevice::~CImageFileBlockDevice()
{
    if (m_bOK)
        f_close(&m_File);
}

boolean CImageFileBlockDevice::ReadSector(void* pBuffer, u32 nLBA, u32 nCount)
{
    if (!m_bOK) return FALSE;
    if (f_lseek(&m_File, nLBA * 512) != FR_OK) return FALSE;
    UINT br = 0;
    return f_read(&m_File, pBuffer, nCount * 512, &br) == FR_OK && br == nCount * 512;
}

boolean CImageFileBlockDevice::WriteSector(const void* pBuffer, u32 nLBA, u32 nCount)
{
    if (!m_bOK) return FALSE;
    if (f_lseek(&m_File, nLBA * 512) != FR_OK) return FALSE;
    UINT bw = 0;
    return f_write(&m_File, pBuffer, nCount * 512, &bw) == FR_OK && bw == nCount * 512;
}

u32 CImageFileBlockDevice::GetSectorCount() const
{
    if (!m_bOK) return 0;
    FSIZE_t size = f_size(&m_File);
    return (u32)(size / 512);
}

boolean CImageFileBlockDevice::Read(u32 nStartSector, void* pBuffer, u32 nCount)
{
    return ReadSector(pBuffer, nStartSector, nCount);
}

boolean CImageFileBlockDevice::Write(u32 nStartSector, const void* pBuffer, u32 nCount)
{
    return WriteSector(pBuffer, nStartSector, nCount);
}