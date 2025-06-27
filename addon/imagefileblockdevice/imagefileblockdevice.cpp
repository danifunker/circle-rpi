#include "imagefileblockdevice.h"
#include <circle/logger.h>
#include <string.h>

LOGMODULE ("ImageFileBlockDevice");
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
    LOGNOTE("ReadSector: nLBA=%u, nCount=%u, byte_offset=%u", nLBA, nCount, nLBA * 512);
    if (!m_bOK) {
        LOGERR("ReadSector: Device not ready");
        return FALSE;
    }
    FSIZE_t size = f_size(&m_File);
    if ((nLBA + nCount) * 512 > size) {
        LOGERR("ReadSector: Out of bounds! nLBA=%u, nCount=%u, size=%u, calculated_offset=%u", 
               nLBA, nCount, (u32)size, (nLBA + nCount) * 512);
        return FALSE;
    }
    if (f_lseek(&m_File, nLBA * 512) != FR_OK) {
        LOGERR("ReadSector: Seek failed for nLBA=%u", nLBA);
        return FALSE;
    }
    UINT br = 0;
    FRESULT result = f_read(&m_File, pBuffer, nCount * 512, &br);
    if (result != FR_OK || br != nCount * 512) {
        LOGERR("ReadSector: Read failed, result=%d, bytes_read=%u, expected=%u", result, br, nCount * 512);
        return FALSE;
    }
    LOGNOTE("ReadSector: Success, read %u bytes", br);
    return TRUE;
}

boolean CImageFileBlockDevice::WriteSector(const void* pBuffer, u32 nLBA, u32 nCount)
{
    if (!m_bOK) return FALSE;
    if (f_lseek(&m_File, nLBA * 512) != FR_OK) return FALSE;
    UINT bw = 0;
    return f_write(&m_File, pBuffer, nCount * 512, &bw) == FR_OK && bw == nCount * 512;
}

u32 CImageFileBlockDevice::GetBlockSize() const {
    LOGNOTE("GetBlockSize() called");
    return 512;
}

u32 CImageFileBlockDevice::GetSectorCount() const
{
    LOGNOTE("GetSectorCount() called, m_bOK=%d, size=%u", m_bOK, m_bOK ? (u32)f_size(&m_File) : 0);
    if (!m_bOK) return 0;
    FSIZE_t size = f_size(&m_File);
    return (u32)(size / 512);
}

u32 CImageFileBlockDevice::GetFileSize() const {
    return m_bOK ? (u32)f_size(&m_File) : 0;
}

boolean CImageFileBlockDevice::IsReady() const {
    LOGNOTE("IsReady() called, m_bOK=%d", m_bOK);
    return m_bOK;
}

u64 CImageFileBlockDevice::GetSize(void) const
{
    if (!m_bOK) return (u64)-1;
    return (u64)f_size(&m_File);
}

u64 CImageFileBlockDevice::Seek(u64 ullOffset)
{
    if (!m_bOK) return (u64)-1;
    if (f_lseek(&m_File, (FSIZE_t)ullOffset) != FR_OK) return (u64)-1;
    return ullOffset;
}

int CImageFileBlockDevice::Read(void *pBuffer, size_t nCount)
{
    if (!m_bOK) return -1;
    UINT br = 0;
    if (f_read(&m_File, pBuffer, nCount, &br) != FR_OK) return -1;
    return (int)br;
}

int CImageFileBlockDevice::Write(const void *pBuffer, size_t nCount)
{
    if (!m_bOK) return -1;
    UINT bw = 0;
    if (f_write(&m_File, pBuffer, nCount, &bw) != FR_OK) return -1;
    return (int)bw;
}