#include "FileStream.h"

gear::FileStream::FileStream(void) : m_File(nullptr), m_Position(0), m_Size(0) {}

gear::FileStream::FileStream(const char *filename, const char *mode)
{
  open(filename, mode);
}

int gear::FileStream::open(const char *filename, const char *mode)
{
  m_File = fopen(filename, mode);
  if (m_File != nullptr)
  {
    fseek(m_File, 0L, SEEK_END);
    m_Size = ftell(m_File);
    fseek(m_File, 0L, SEEK_SET);
    return 0;
  }
  else
  {
    m_Position = 0;
    m_Size = 0;
    return -1;
  }
}

int gear::FileStream::close(void)
{
  if (m_File != nullptr)
  {
    int ret = fclose(m_File);
    if (ret == 0)
      m_File = nullptr;
    return ret;
  }
  return 0;
}

bool gear::FileStream::is_Open(void) const
{
  return m_File != nullptr;
}

size_t gear::FileStream::size(void) const
{
  return m_Size;
}