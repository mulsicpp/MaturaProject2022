#pragma once

#include <gear/core/core.h>
#include <stdint.h>
#include <errno.h>

_GEAR_START

/*
Simple file stream that allows to read and write to a file.
*/
class FileStream
{
private:
  FILE *m_File;
  int m_Position;
  size_t m_Size;

  FileStream(const char *filename, const char *mode);

  FileStream(const FileStream &) = delete;
  FileStream &operator=(const FileStream &) = delete;

public:
  /*
  Opens a file stream for the specified file in a certain mode.

  @param filename the path to the file to open
  @param mode the mode of the stream (for example "r" to read and "w" to write)

  @return 0 if no error occured, else -1
  */
  static FileStream *open(const char *filename, const char *mode);

  /*
  Closes the file stream.

  @return 0 if no error occured, else -1
  */
  static int close(FileStream *file_Stream);

  /*
  @return the size of the opened file or 0 if the file stream is not open.
  */
  size_t size(void) const;

  char *gets(char *buffer, int max_Bytes);

  int puts(const char *buffer);

  template <class... Ts>
  int printf(const char *const format, Ts... args)
  {
    return fprintf(m_File, format, args...);
  }

  /*
  Gets one value of a certain type from the file.

  @param val a pointer, where the data is stored
  @return 0 if no error occured, else -1
  */
  template <class T = uint8_t>
  int get(T *val)
  {
    int size_Of_Type = sizeof(T);
    int get_Val = 0;
    for (int i = 0; i < size_Of_Type; i++)
    {
      get_Val = fgetc(m_File);
      if (get_Val != -1){
        ((char *)val)[i] = get_Val;
      }
      else
      {
        return -1;
      }
    }
    return 0;
  }

  /*
  Gets an array value of a certain type from the file.

  @param val a pointer to the array
  @param count the count of values in the array
  @return 0 if no error occured, else -1
  */
  template <class T = uint8_t>
  int get(T *vals, int count)
  {
    int size_Of_Data = count * sizeof(T);
    int get_Val = 0;
    for (int i = 0; i < size_Of_Data; i++)
    {
      get_Val = fgetc(m_File);
      if (get_Val != -1){
        ((char *)vals)[i] = get_Val;
      }
      else
      {
        return -1;
      }
    }
    m_Position += size_Of_Data;
    return 0;
  }

  /*
  Puts the value of a certain type at the end of the file.

  @param val a the value
  @return 0 if no error occured, else -1
  */
  template <class T = uint8_t>
  int put(T val)
  {
    int size_Of_Type = sizeof(T);
    for (int i = 0; i < size_Of_Type; i++)
      fputc(((char *)&val)[i], m_File);
    return 0;
  }

  /*
  Puts an array of values of a certain type at the end of the file.

  @param val a pointer to the array
  @param count the count of values in the array
  @return 0 if no error occured, else -1
  */
  template <class T = uint8_t>
  int put(T *vals, int count)
  {
    int size_Of_Data = sizeof(T) * count;
    for (int i = 0; i < size_Of_Data; i++)
      fputc(((char *)vals)[i], m_File);
    return 0;
  }
};

_GEAR_END