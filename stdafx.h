/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : The MIT License
/*---------------------------------------------------------------------------*/

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0501
#endif

#ifndef _WIN32_WINNT              
#define _WIN32_WINNT 0x0501
#endif						

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0410
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0600
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard component
#include <afxext.h>         // Extended MFC

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC IE4 Common Control support
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC Windows Common Control support
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "DebugPrint.h"

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// Version Information
#define PRODUCT_NAME			L"CrystalDiskMark 8 -Cecile-"
#define PRODUCT_VERSION			L"Alpha0"
#define PRODUCT_ROMING_NAME		L"Cecile"

#ifdef UWP
#ifdef SUISHO_SHIZUKU_SUPPORT
#define PRODUCT_SHORT_NAME		L""
#ifdef _M_ARM
#define PRODUCT_EDITION			L"Shizuku Edition ARM32 (UWP)"
#elif _M_ARM64
#define PRODUCT_EDITION			L"Shizuku Edition ARM64 (UWP)"
#elif _M_X64
#define PRODUCT_EDITION			L"Shizuku Edition x64 (UWP)"
#else
#define PRODUCT_EDITION			"Shizuku Edition (UWP)")
#endif
#else
#define PRODUCT_SHORT_NAME		L""
#ifdef _M_ARM
#define PRODUCT_EDITION			L"ARM32 (UWP)"
#elif _M_ARM64
#define PRODUCT_EDITION			L"ARM64 (UWP)"
#elif _M_X64
#define PRODUCT_EDITION			L"x64 (UWP)"
#else
#define PRODUCT_EDITION			L"(UWP)"
#endif
#endif

#else

#ifdef SUISHO_SHIZUKU_SUPPORT
#define PRODUCT_SHORT_NAME		L""
#ifdef _M_ARM
#define PRODUCT_EDITION			L"Shizuku Edition ARM32"
#elif _M_ARM64
#define PRODUCT_EDITION			L"Shizuku Edition ARM64"
#elif _M_X64
#define PRODUCT_EDITION			L"Shizuku Edition x64"
#else
#define PRODUCT_EDITION			L"Shizuku Edition"
#endif
#else
#define PRODUCT_SHORT_NAME		L""
#ifdef _M_ARM
#define PRODUCT_EDITION			L"ARM32"
#elif _M_ARM64
#define PRODUCT_EDITION			L"ARM64"
#elif _M_X64
#define PRODUCT_EDITION			L"x64"
#else
#define PRODUCT_EDITION			L""
#endif
#endif
#endif

/// Beta
// #define PRODUCT_EDITION			L"-Cecile-")

#define PRODUCT_RELEASE				L"2020/04/18"
#define PRODUCT_COPY_YEAR			L"2007-2020"
#define PRODUCT_COPYRIGHT			L"© hiyohiyo 2007-2020"
#define PRODUCT_LICENSE				L"The MIT License"

#define URL_CRYSTAL_DEW_WORLD_JA	L"https://crystalmark.info/ja/"
#define URL_CRYSTAL_DEW_WORLD_EN 	L"https://crystalmark.info/en/"

#define	URL_VERSION_JA				L"https://crystalmark.info/ja/software/crystaldiskmark/crystaldiskmark-history/"
#define	URL_VERSION_EN				L"https://crystalmark.info/en/software/crystaldiskmark/crystaldiskmark-history/"
#define	URL_LICENSE_JA				L"https://crystalmark.info/ja/software/crystaldiskmark/crystaldiskmark-license/"
#define	URL_LICENSE_EN				L"https://crystalmark.info/en/software/crystaldiskmark/crystaldiskmark-license/"


#define	URL_PROJECT_SHIZUKU_JA		L"https://suishoshizuku.com/ja/"
#define	URL_PROJECT_SHIZUKU_EN		L"https://suishoshizuku.com/en/"

#define URL_DISKSPD					L"https://github.com/microsoft/diskspd"

#define URL_HTML_HELP_JA			L"https://crystalmark.info/ja/software/crystaldiskmark/"
#define URL_HTML_HELP_EN 			L"https://crystalmark.info/en/software/crystaldiskmark/"

#ifdef SUISHO_SHIZUKU_SUPPORT

#define	URL_PROJECT_SITE_1		    L"http://kirinokasumu.com/"
#define URL_PROJECT_SITE_2		    L"http://linux-ha.sourceforge.jp/wp/"
#define URL_PROJECT_SITE_3	        L"http://ch.nicovideo.jp/oss"
#define URL_PROJECT_SITE_4			L"http://www.bellche.com/"
#define URL_PROJECT_SITE_5			L"https://suishoshizuku.com/"

#endif

#define MAX_THREADS 64
#define MAX_QUEUES 512

static const int RE_EXEC = 5963;

#pragma warning(disable : 4996)

