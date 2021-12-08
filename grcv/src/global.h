#pragma once

//colors
#define C_RESET "\033[0m"

#define BLACK "\033[0;30m"
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define BOLD "\033[1m"

template <class... T>
void error(const char *str, T... args)
{
  printf(RED);
  printf(str, args...);
  printf("\n" C_RESET);
  exit(1);
}

template<class... T>
void msg(const char *format, const char *str, T... args)
{
  printf(format);
  printf(str, args...);
  printf("\n" C_RESET);
}