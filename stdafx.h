/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : MIT License
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

#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard component
#include <afxext.h>         // Extended MFC
#include <afxdtctl.h>		// MFC IE4 Common Control support
#include <afxcmn.h>			// MFC Windows Common Control support

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifdef UWP
#ifdef SUISHO_SHIZUKU_SUPPORT
#ifdef _M_ARM
#define PRODUCT_EDITION			L"Shizuku Edition ARM32"
#elif _M_ARM64
#define PRODUCT_EDITION			L"Shizuku Edition ARM64"
#elif _M_X64
#define PRODUCT_EDITION			L"Shizuku Edition x64"
#else
#define PRODUCT_EDITION			L"Shizuku Edition x86"
#endif
#else
#ifdef _M_ARM
#define PRODUCT_EDITION			L"ARM32"
#elif _M_ARM64
#define PRODUCT_EDITION			L"ARM64"
#elif _M_X64
#define PRODUCT_EDITION			L"x64"
#else
#define PRODUCT_EDITION			L"x86"
#endif
#endif

#else

#ifdef SUISHO_AOI_SUPPORT
#ifdef _M_ARM
#define PRODUCT_EDITION			L"Aoi Edition ARM32"
#elif _M_ARM64
#define PRODUCT_EDITION			L"Aoi Edition ARM64"
#elif _M_X64
#define PRODUCT_EDITION			L"Aoi Edition x64"
#else
#define PRODUCT_EDITION			L"Aoi Edition x86"
#endif

#elif MSI_MEI_SUPPORT
#ifdef _M_ARM
#define PRODUCT_EDITION			L"MSI Mei Mihoshi Edition ARM32"
#elif _M_ARM64
#define PRODUCT_EDITION			L"MSI Mei Mihoshi Edition ARM64"
#elif _M_X64
#define PRODUCT_EDITION			L"MSI Mei Mihoshi Edition x64"
#else
#define PRODUCT_EDITION			L"MSI Mei Mihoshi Edition x86"
#endif

#elif SUISHO_SHIZUKU_SUPPORT
#ifdef _M_ARM
#define PRODUCT_EDITION			L"Shizuku Edition ARM32"
#elif _M_ARM64
#define PRODUCT_EDITION			L"Shizuku Edition ARM64"
#elif _M_X64
#define PRODUCT_EDITION			L"Shizuku Edition x64"
#else
#define PRODUCT_EDITION			L"Shizuku Edition x86"
#endif

#else
#ifdef _M_ARM
#define PRODUCT_EDITION			L"ARM32"
#elif _M_ARM64
#define PRODUCT_EDITION			L"ARM64"
#elif _M_X64
#define PRODUCT_EDITION			L"x64"
#else
#define PRODUCT_EDITION			L"x86"
#endif
#endif
#endif

// Version Information
#define PRODUCT_NAME				L"CrystalDiskMark"
#define PRODUCT_FILENAME			L"CrystalDiskMark"
#define PRODUCT_VERSION				L"8.0.5"
#define PRODUCT_SHORT_NAME		    L"CDM"

#define PRODUCT_RELEASE				L"2024/04/30"
#define PRODUCT_COPY_YEAR			L"2007-2024"
#define PRODUCT_LICENSE				L"MIT License"

#ifdef SUISHO_AOI_SUPPORT
#define PRODUCT_COPYRIGHT_1         L"© 2007-2024 hiyohiyo"
#define PRODUCT_COPYRIGHT_2         L"© 2023-2024 nijihashi sola"

#elif MSI_MEI_SUPPORT
#define PRODUCT_COPYRIGHT_1         L"© 2007-2024 hiyohiyo"
#define PRODUCT_COPYRIGHT_2         L"© 2024 Micro-Star INT'L CO., LTD."

#elif SUISHO_SHIZUKU_SUPPORT
#define PRODUCT_COPYRIGHT_1         L"© 2007-2021 hiyohiyo"
#define PRODUCT_COPYRIGHT_2         L"© 2012-2021 kirino kasumu"

#else
#define PRODUCT_COPYRIGHT_1			L"© 2007-2021 hiyohiyo"
#define PRODUCT_COPYRIGHT_2			L""
#endif

#ifdef MSI_MEI_SUPPORT
#define URL_MAIN_JA					L"https://jp.msi.com/"
#define URL_MAIN_EN 				L"https://www.msi.com/"
#else
#define URL_MAIN_JA					L"https://crystalmark.info/ja/"
#define URL_MAIN_EN 				L"https://crystalmark.info/en/"
#endif

#define URL_CRYSTAL_DEW_WORLD_JA	L"https://crystalmark.info/ja/"
#define URL_CRYSTAL_DEW_WORLD_EN 	L"https://crystalmark.info/en/"

#define	URL_VERSION_JA				L"https://crystalmark.info/ja/software/crystaldiskmark/crystaldiskmark-history/"
#define	URL_VERSION_EN				L"https://crystalmark.info/en/software/crystaldiskmark/crystaldiskmark-history/"
#define	URL_LICENSE_JA				L"https://crystalmark.info/ja/software/crystaldiskmark/crystaldiskmark-license/"
#define	URL_LICENSE_EN				L"https://crystalmark.info/en/software/crystaldiskmark/crystaldiskmark-license/"
#define URL_HELP_JA					L"https://crystalmark.info/ja/software/crystaldiskmark/"
#define URL_HELP_EN			 		L"https://crystalmark.info/en/software/crystaldiskmark/"

#define URL_DISKSPD					L"https://github.com/microsoft/diskspd"

#ifdef SUISHO_AOI_SUPPORT
#define	URL_PROJECT_SITE_1		    L"https://twitter.com/sola_no_crayon"
#define URL_PROJECT_SITE_2		    L"https://twitter.com/harakeiko0718"
#define URL_PROJECT_SITE_3	        L"https://instagram.com/kotomi_wicke?igshid=OGQ5ZDc2ODk2ZA=="
#define URL_PROJECT_SITE_4			L"https://twitter.com/bellche"
#define URL_PROJECT_SITE_5			L""

#elif MSI_MEI_SUPPORT
#define	URL_PROJECT_SITE_1		    L"https://jp.msi.com/Landing/mihoshimei/nb"
#define URL_PROJECT_SITE_2		    L"https://twitter.com/hoshi_u3"
#define URL_PROJECT_SITE_3	        L"https://twitter.com/mokowata"
#define URL_PROJECT_SITE_4			L"https://jp.msi.com/"
#define URL_PROJECT_SITE_5			L"https://jp.msi.com/"

#elif SUISHO_SHIZUKU_SUPPORT
#define	URL_PROJECT_SITE_1		    L"https://twitter.com/kirinokasumu"
#define URL_PROJECT_SITE_2		    L"https://linux-ha.osdn.jp/wp/"
#define URL_PROJECT_SITE_3	        L"https://ch.nicovideo.jp/oss"
#define URL_PROJECT_SITE_4			L"https://twitter.com/bellche"
#define URL_PROJECT_SITE_5			L"https://suishoshizuku.com/"
#endif

#define MAX_THREADS 64
#define MAX_QUEUES 512

static const int RE_EXEC = 5963;

#pragma warning(disable : 4996)

//------------------------------------------------
// Option Flags
//------------------------------------------------

// For Task Tray Icon Feature
// #define OPTION_TASK_TRAY

//------------------------------------------------
// Global Sttings
//------------------------------------------------

#define DEFAULT_FONT_FACE_1			L"Segoe UI"
#define DEFAULT_FONT_FACE_2			L"Tahoma"

#define THEME_DIR					L"CdmResource\\themes\\"
#define LANGUAGE_DIR				L"CdmResource\\language\\"
#define VOICE_DIR					L"CdmResource\\voice\\"


#define MENU_THEME_INDEX			3
#define MENU_LANG_INDEX				5

#define DEFAULT_THEME				L"Default"
#define DEFAULT_LANGUAGE			L"English"

#define TIMER_UPDATE_DIALOG			500

#define WM_UPDATE_SCORE				(WM_APP+0x1001)
#define WM_UPDATE_MESSAGE			(WM_APP+0x1002)
#define WM_EXIT_BENCHMARK			(WM_APP+0x1003)