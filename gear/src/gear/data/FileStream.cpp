#include "FileStream.h"

gear::FileStream::FileStream(const char *filename, const char *mode)
{
  m_File = fopen(filename, mode);
  if (m_File != nullptr)
  {
    fseek(m_File, 0L, SEEK_END);
    m_Size = ftell(m_File);
    fseek(m_File, 0L, SEEK_SET);
  }
}

gear::FileStream *gear::FileStream::open(const char *filename, const char *mode)
{
  FileStream *file_Stream = new FileStream(filename, mode);
  if(file_Stream->m_File != nullptr)
    return file_Stream;
  else
  {
    delete file_Stream;
    return nullptr;
  }
}

int gear::FileStream::close(FileStream *file_Stream)
{
  int ret = fclose(file_Stream->m_File);
  if(ret == 0){
    delete file_Stream;
    return 0;
  }
  else
  {
    return -1;
  }
}

size_t gear::FileStream::size(void) const
{
  return m_Size;
}

char *gear::FileStream::gets(char *buffer, int max_Bytes)
{
  return fgets(buffer, max_Bytes, m_File);
}

int gear::FileStream::puts(const char *buffer)
{
  return fputs(buffer, m_File);
}