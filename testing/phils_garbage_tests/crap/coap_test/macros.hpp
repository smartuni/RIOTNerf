#ifndef APP_MACROS_HPP
#define APP_MACROS_HPP

#define APP_UNUSED(arg)          (static_cast<void>((arg)))

#define APP_DETAIL_GLUE(a, b)    a##b

#define APP_GLUE(a, b)           APP_DETAIL_GLUE(a, b)

#define APP_DETAIL_STRINGIFY(x)  #x

#define APP_STRINGIFY(x)         APP_DETAIL_STRINGIFY(x)

#define APP_MACRO_BEGIN          do {

#define APP_MACRO_END           } while (false)

#endif /* APP_MACROS_HPP */
