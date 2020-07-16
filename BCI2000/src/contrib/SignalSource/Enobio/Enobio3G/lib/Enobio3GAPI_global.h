#ifndef ENOBIOAPI_GLOBAL_H
#define ENOBIOAPI_GLOBAL_H

#ifndef NULL
#define NULL 0
#endif

#if defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define Q_DECL_EXPORT __declspec(dllexport)
#  define Q_DECL_IMPORT __declspec(dllimport)
#else
#  define Q_DECL_EXPORT
#  define Q_DECL_IMPORT
#endif

#if defined(ENOBIO3GAPI_LIBRARY)
#  define ENOBIO3GAPISHARED_EXPORT Q_DECL_EXPORT
#else
#  define ENOBIO3GAPISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ENOBIOAPI_GLOBAL_H
