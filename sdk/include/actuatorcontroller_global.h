#ifndef ACTUATORCONTROLLER_GLOBAL_H
#define ACTUATORCONTROLLER_GLOBAL_H

#  ifdef _WIN64
#    define Q_DECL_EXPORT     __declspec(dllexport)
#    define Q_DECL_IMPORT     __declspec(dllimport)
#  else
#    define Q_DECL_EXPORT     __attribute__((visibility("default")))
#    define Q_DECL_IMPORT     __attribute__((visibility("default")))
#  endif

#if defined(ACTUATORCONTROLLER_LIBRARY)
#  define ACTUATORCONTROLLERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ACTUATORCONTROLLERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ACTUATORCONTROLLER_GLOBAL_H
