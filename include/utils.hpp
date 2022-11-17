#ifndef TF_UTILS_HPP
#define TF_UTILS_HPP

// debug log macros
#ifndef NDEBUG
  #include <source_location>
  #define TERM_WHITE "\033[38;2;255;255;255m"
  #define TERM_GREY "\033[38;2;99;104;109m"
  #define TERM_CYAN "\033[38;2;61;174;233m"
  #define TERM_YELLOW "\033[38;2;253;220;75m"

  #define DBG(var) std::cerr << TERM_GREY << '[' << std::source_location::current().file_name() << ':' << std::source_location::current().line() << "] " << TERM_CYAN << #var << TERM_WHITE << " = " << TERM_YELLOW << var << TERM_WHITE << std::endl
  #define DBGCOND(cond, var) if(cond) DBG(var)
#else
  #define DBG(var)
  #define DBGCOND(cond, var)
  #define DBGCOND(var)
#endif // NDEBUG

#define uint unsigned int

#define masksMatch(mask1, mask2) static_cast<uint>(mask1) & static_cast<uint>(mask2)

#endif // TF_UTILS_HPP
