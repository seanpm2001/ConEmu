﻿
/*
Copyright (c) 2009-2013 Maximus5
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#pragma once


#undef SHOW_SHUTDOWNSRV_STEPS
#ifdef _DEBUG
	#define SHOW_SHUTDOWNSRV_STEPS
#endif


#ifdef _DEBUG
// Раскомментировать для вывода в консоль информации режима Comspec
#define PRINT_COMSPEC(f,a) //wprintf(f,a)
//#define DEBUGSTR(s) OutputDebugString(s)
#elif defined(__GNUC__)
//  Раскомментировать, чтобы сразу после запуска процесса (conemuc.exe) показать MessageBox, чтобы прицепиться дебаггером
//  #define SHOW_STARTED_MSGBOX
#define PRINT_COMSPEC(f,a) //wprintf(f,a)
//#define DEBUGSTR(s)
#define CRTPRINTF
#else
#define PRINT_COMSPEC(f,a)
#define DEBUGSTR(s)
#endif

#ifdef _DEBUG
#define xf_check() { xf_validate(); xf_dump_chk(); }
#else
#define xf_check()
#endif

#define DEBUGLOG(s) //DEBUGSTR(s)
#define DEBUGLOGSIZE(s) DEBUGSTR(s)
#define DEBUGLOGLANG(s) //DEBUGSTR(s) //; Sleep(2000)

#ifdef _DEBUG
//CRITICAL_ SECTION gcsHeap;
//#define MCHKHEAP { Enter CriticalSection(&gcsHeap); int MDEBUG_CHK=_CrtCheckMemory(); _ASSERTE(MDEBUG_CHK); LeaveCriticalSection(&gcsHeap); }
//#define MCHKHEAP HeapValidate(ghHeap, 0, NULL);
//#define MCHKHEAP { int MDEBUG_CHK=_CrtCheckMemory(); _ASSERTE(MDEBUG_CHK); }
//#define HEAP_LOGGING
#else
//#define MCHKHEAP
#endif




#define CSECTION_NON_RAISE

#include <Windows.h>
#include <WinCon.h>
#ifdef _DEBUG
#include <stdio.h>
#endif
#include <Shlwapi.h>
#include <Tlhelp32.h>

void ShutdownSrvStep(LPCWSTR asInfo, int nParm1 = 0, int nParm2 = 0, int nParm3 = 0, int nParm4 = 0);

enum SetTerminateEventPlace
{
	ste_None = 0,
	ste_ServerDone,
	ste_ConsoleMain,
	ste_ProcessCountChanged,
	ste_CheckProcessCount,
	ste_DebugThread,
	ste_WriteMiniDump,
	ste_CmdDetachCon,
	ste_HandlerRoutine,
	ste_Attach2GuiFailed,
};
extern SetTerminateEventPlace gTerminateEventPlace;
void SetTerminateEvent(SetTerminateEventPlace eFrom);

/*  Global  */
extern DWORD   gnSelfPID;
//HANDLE  ghConIn = NULL, ghConOut = NULL;
extern HWND    ghConWnd;
extern HWND    ghConEmuWnd; // Root! window
extern HWND    ghConEmuWndDC; // ConEmu DC window
extern HWND    ghConEmuWndBack; // ConEmu Back window
void SetConEmuWindows(HWND hDcWnd, HWND hBackWnd);
extern DWORD   gnMainServerPID; // PID сервера (инициализируется на старте, при загрузке Dll)
extern DWORD   gnAltServerPID; // PID сервера (инициализируется на старте, при загрузке Dll)
extern BOOL    gbLogProcess; // (pInfo->nLoggingType == glt_Processes)
extern BOOL    gbWasBufferHeight;
extern BOOL    gbNonGuiMode;
extern HANDLE  ghExitQueryEvent; // выставляется когда в консоли не остается процессов
extern int nExitQueryPlace, nExitPlaceStep, nExitPlaceThread;
extern HANDLE  ghQuitEvent;      // когда мы в процессе закрытия (юзер уже нажал кнопку "Press to close console")
extern bool    gbQuit;           // когда мы в процессе закрытия (юзер уже нажал кнопку "Press to close console")
extern int     gnConfirmExitParm;
extern BOOL    gbAlwaysConfirmExit, gbInShutdown, gbAutoDisableConfirmExit;
extern int     gbRootWasFoundInCon;
enum AttachModeEnum
{
	am_None = 0,
	am_Simple,    // As is
	am_Auto,      // Same as am_Simple, but always return 0 as errorlevel
	am_Admin,     // Special "attach" when ConEmu is run under "User" and console "As admin"
};
extern AttachModeEnum gbAttachMode; // сервер запущен НЕ из conemu.exe (а из плагина, из CmdAutoAttach, или -new_console)
extern BOOL    gbAlienMode;  // сервер НЕ является владельцем консоли (корневым процессом этого консольного окна)
extern BOOL    gbForceHideConWnd;
extern DWORD   gdwMainThreadId;
//int       gnBufferHeight = 0;
extern wchar_t* gpszRunCmd;
extern BOOL    gbRunInBackgroundTab;
extern DWORD   gnImageSubsystem;
//HANDLE  ghCtrlCEvent = NULL, ghCtrlBreakEvent = NULL;
extern HANDLE ghHeap; //HeapCreate(HEAP_GENERATE_EXCEPTIONS, nMinHeapSize, 0);
#ifdef _DEBUG
extern size_t gnHeapUsed, gnHeapMax;
extern HANDLE ghFarInExecuteEvent;
#endif

#include "../common/common.hpp"
#include "../common/ConEmuCheck.h"
#include "../common/WinObjects.h"
#include "../common/ConsoleAnnotation.h"
#include "../common/InQueue.h"

#ifdef _DEBUG
extern wchar_t gszDbgModLabel[6];
#endif

#define START_MAX_PROCESSES 1000
#define CHECK_PROCESSES_TIMEOUT 500
#define CHECK_ANTIVIRUS_TIMEOUT 6*1000
#define CHECK_ROOTSTART_TIMEOUT 10*1000
#ifdef _DEBUG
	#define CHECK_ROOTOK_TIMEOUT (IsDebuggerPresent() ? ((DWORD)-1) : (10*1000)) // под отладчиком - ждать всегда
#else
	#define CHECK_ROOTOK_TIMEOUT 10*1000
#endif
#define MAX_FORCEREFRESH_INTERVAL 500
#define MAX_SYNCSETSIZE_WAIT 1000
#define GUI_PIPE_TIMEOUT 300
#define RELOAD_INFO_TIMEOUT 500
#define EXTCONCOMMIT_TIMEOUT 500
#define REQSIZE_TIMEOUT 5000
#define GUIREADY_TIMEOUT 10000
#define UPDATECONHANDLE_TIMEOUT 1000
#define GUIATTACH_TIMEOUT 10000
#define INPUT_QUEUE_TIMEOUT 100
#define ATTACH2GUI_TIMEOUT 10000
#define GUIATTACHEVENT_TIMEOUT 250
#define REFRESH_FELL_SLEEP_TIMEOUT 3000
#define LOCK_READOUTPUT_TIMEOUT 10000
#define LOCK_REOPENCONOUT_TIMEOUT 250
#define WAIT_SETCONSCRBUF_MAX_TIMEOUT 60000
#define WAIT_SETCONSCRBUF_MIN_TIMEOUT 15000

//#define IMAGE_SUBSYSTEM_DOS_EXECUTABLE  255

#define MAX_INPUT_QUEUE_EMPTY_WAIT 1000


//#ifndef _DEBUG
// Релизный режим
#define FORCE_REDRAW_FIX
#define RELATIVE_TRANSMIT_DISABLE
//#else
//// Отладочный режим
////#define FORCE_REDRAW_FIX
//#endif

#if !defined(CONSOLE_APPLICATION_16BIT)
#define CONSOLE_APPLICATION_16BIT       0x0001
#endif


//#if defined(__GNUC__)
//	//#include "assert.h"
//	#ifndef _ASSERTE
//		#define _ASSERTE(x)
//	#endif
//	#ifndef _ASSERT
//		#define _ASSERT(x)
//	#endif
//#else
//	#include <crtdbg.h>
//#endif

#ifndef EVENT_CONSOLE_CARET
#define EVENT_CONSOLE_CARET             0x4001
#define EVENT_CONSOLE_UPDATE_REGION     0x4002
#define EVENT_CONSOLE_UPDATE_SIMPLE     0x4003
#define EVENT_CONSOLE_UPDATE_SCROLL     0x4004
#define EVENT_CONSOLE_LAYOUT            0x4005
#define EVENT_CONSOLE_START_APPLICATION 0x4006
#define EVENT_CONSOLE_END_APPLICATION   0x4007
#endif

//#undef USE_WINEVENT_SRV

BOOL createProcess(BOOL abSkipWowChange, LPCWSTR lpApplicationName, LPWSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCWSTR lpCurrentDirectory, LPSTARTUPINFOW lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);

BOOL ProcessSrvCommand(CESERVER_REQ& in, CESERVER_REQ** out);
//#ifdef USE_WINEVENT_SRV
//DWORD WINAPI WinEventThread(LPVOID lpvParam);
//void WINAPI WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD anEvent, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);
//#endif
void CheckCursorPos();
BOOL ReloadFullConsoleInfo(BOOL abForceSend);
bool CheckWasFullScreen();
DWORD WINAPI RefreshThread(LPVOID lpvParam); // Нить, перечитывающая содержимое консоли
int ServerInit(int anWorkMode/*0-Server,1-AltServer,2-Reserved*/); // Создать необходимые события и нити
void ServerDone(int aiRc, bool abReportShutdown = false);
BOOL ServerInitConsoleMode();
void SetupCreateDumpOnException();
int ComspecInit();
void ComspecDone(int aiRc);
BOOL SetConsoleSize(USHORT BufferHeight, COORD crNewSize, SMALL_RECT rNewRect, LPCSTR asLabel = NULL);
void CreateLogSizeFile(int nLevel);
void LogSize(COORD* pcrSize, LPCSTR pszLabel);
void LogString(LPCSTR asText);
void PrintExecuteError(LPCWSTR asCmd, DWORD dwErr, LPCWSTR asSpecialInfo=NULL);
BOOL MyReadConsoleOutput(HANDLE hOut, CHAR_INFO *pData, COORD& bufSize, SMALL_RECT& rgn);
BOOL MyWriteConsoleOutput(HANDLE hOut, CHAR_INFO *pData, COORD& bufSize, COORD& crBufPos, SMALL_RECT& rgn);


#if defined(__GNUC__)
extern "C" {
#endif
	BOOL WINAPI HandlerRoutine(DWORD dwCtrlType);
	int WINAPI RequestLocalServer(/*[IN/OUT]*/RequestLocalServerParm* Parm);
#if defined(__GNUC__)
};
#endif

int GetProcessCount(DWORD *rpdwPID, UINT nMaxCount);
SHORT CorrectTopVisible(int nY);
BOOL CorrectVisibleRect(CONSOLE_SCREEN_BUFFER_INFO* pSbi);
WARNING("Вместо GetConsoleScreenBufferInfo нужно использовать MyGetConsoleScreenBufferInfo!");
BOOL MyGetConsoleScreenBufferInfo(HANDLE ahConOut, PCONSOLE_SCREEN_BUFFER_INFO apsc);
void CmdOutputStore(bool abCreateOnly = false);
void CmdOutputRestore(bool abSimpleMode);
void CheckConEmuHwnd();
HWND FindConEmuByPID();
typedef BOOL (__stdcall *FGetConsoleKeyboardLayoutName)(wchar_t*);
extern FGetConsoleKeyboardLayoutName pfnGetConsoleKeyboardLayoutName;
void CheckKeyboardLayout();
bool IsKeyboardLayoutChanged(DWORD* pdwLayout);
int CALLBACK FontEnumProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme, DWORD FontType, LPARAM lParam);
typedef DWORD (WINAPI* FGetConsoleProcessList)(LPDWORD lpdwProcessList, DWORD dwProcessCount);
extern FGetConsoleProcessList pfnGetConsoleProcessList;
//BOOL HookWinEvents(int abEnabled);
BOOL CheckProcessCount(BOOL abForce = FALSE);
BOOL ProcessAdd(DWORD nPID, MSectionLock *pCS = NULL);
void SendStarted();
CESERVER_REQ* SendStopped(CONSOLE_SCREEN_BUFFER_INFO* psbi = NULL);
BOOL SendConsoleEvent(INPUT_RECORD* pr, UINT nCount);
typedef BOOL (WINAPI *FDebugActiveProcessStop)(DWORD dwProcessId);
extern FDebugActiveProcessStop pfnDebugActiveProcessStop;
typedef BOOL (WINAPI *FDebugSetProcessKillOnExit)(BOOL KillOnExit);
extern FDebugSetProcessKillOnExit pfnDebugSetProcessKillOnExit;
void ProcessDebugEvent();
//BOOL IsUserAdmin();
void _wprintf(LPCWSTR asBuffer);
bool IsOutputRedirected();
#ifdef CRTPRINTF
void _printf(LPCSTR asBuffer);
void _printf(LPCSTR asFormat, DWORD dwErr);
void _printf(LPCSTR asFormat, DWORD dwErr, LPCWSTR asAddLine);
void _printf(LPCSTR asFormat, DWORD dw1, DWORD dw2, LPCWSTR asAddLine=NULL);
#else
#define _printf printf
//#define _wprintf(s) wprintf(L"%s",s)
#endif
HWND Attach2Gui(DWORD nTimeout);

int InjectRemote(DWORD nRemotePID, bool abDefTermOnly = false);
int InfiltrateDll(HANDLE hProcess, LPCWSTR dll);

int ParseCommandLine(LPCWSTR asCmdLine /*, wchar_t** psNewCmd, BOOL* pbRunInBackgroundTab*/); // Разбор параметров командной строки
wchar_t* ParseConEmuSubst(LPCWSTR asCmd, bool bUpdateTitle = false);
void UpdateConsoleTitle(LPCWSTR lsCmdLine, BOOL& lbNeedCutStartEndQuot, bool bExpandVars);
BOOL SetTitle(bool bExpandVars, LPCWSTR lsTitle);
void Help();
void DosBoxHelp();
int  ExitWaitForKey(WORD* pvkKeys, LPCWSTR asConfirm, BOOL abNewLine, BOOL abDontShowConsole);
bool IsMainServerPID(DWORD nPID);

bool AltServerWasStarted(DWORD nPID, HANDLE hAltServer, bool ForceThaw = false);

int CreateMapHeader();
void CloseMapHeader();
void CopySrvMapFromGuiMap();
void UpdateConsoleMapHeader();
int Compare(const CESERVER_CONSOLE_MAPPING_HDR* p1, const CESERVER_CONSOLE_MAPPING_HDR* p2);
BOOL ReloadGuiSettings(ConEmuGuiMapping* apFromCmd);

int CreateColorerHeader(bool bForceRecreate = false);

void DisableAutoConfirmExit(BOOL abFromFarPlugin=FALSE);

int MySetWindowRgn(CESERVER_REQ_SETWINDOWRGN* pRgn);

bool IsAutoAttachAllowed();

#ifdef _DEBUG
	#undef WAIT_INPUT_READY
#else
	#define WAIT_INPUT_READY
#endif
////#define USE_INPUT_SEMAPHORE
//#undef USE_INPUT_SEMAPHORE
//#define INSEMTIMEOUT_WRITE 250
//#define INSEMTIMEOUT_READ  500
//#ifdef USE_INPUT_SEMAPHORE
//extern HANDLE ghConInSemaphore;
//#endif
//void InitializeConsoleInputSemaphore();
//void ReleaseConsoleInputSemaphore();

/* Console Handles */
//extern MConHandle ghConIn;
extern MConHandle ghConOut;
void ConOutCloseHandle();


typedef enum tag_RunMode
{
	RM_UNDEFINED = 0,
	RM_SERVER,
	RM_COMSPEC,
	RM_SETHOOK64,
	RM_ALTSERVER,
	RM_APPLICATION,
} RunMode;

extern RunMode gnRunMode;

extern BOOL gbDumpServerInitStatus;
extern BOOL gbNoCreateProcess;
extern int  gnCmdUnicodeMode;
extern BOOL gbUseDosBox;
extern BOOL gbRootIsCmdExe;
extern BOOL gbAttachFromFar;
extern BOOL gbConsoleModeFlags;
extern DWORD gnConsoleModeFlags;
extern WORD  gnDefTextColors, gnDefPopupColors;
extern BOOL  gbVisibleOnStartup;

#ifdef WIN64
#ifndef __GNUC__
#pragma message("ComEmuC compiled in X64 mode")
#endif
#define NTVDMACTIVE FALSE
#else
#ifndef __GNUC__
#pragma message("ComEmuC compiled in X86 mode")
#endif
#define NTVDMACTIVE (gpSrv->bNtvdmActive)
#endif

#include "../common/PipeServer.h"
#include "../common/MArray.h"
#include "../common/MMap.h"

struct AltServerInfo
{
	DWORD  nPID; // Для информации
	HANDLE hPrev;
	DWORD  nPrevPID;
};

#include "Debugger.h"

typedef BOOL (WINAPI* FGetConsoleDisplayMode)(LPDWORD);
extern FGetConsoleDisplayMode pfnGetConsoleDisplayMode;

struct SrvInfo
{
	HANDLE hRootProcess, hRootThread;
	DWORD dwRootProcess, dwRootThread; DWORD dwRootStartTime;
	DWORD dwParentFarPID;

	HANDLE hMainServer; DWORD dwMainServerPID;
	HANDLE hServerStartedEvent;

	HANDLE hAltServer, hCloseAltServer, hAltServerChanged;
	DWORD dwAltServerPID; DWORD dwPrevAltServerPID;
	MMap<DWORD,AltServerInfo> AltServers;

	HANDLE hFreezeRefreshThread;

	// CECMD_SETCONSCRBUF
	HANDLE hWaitForSetConBufThread;    // Remote thread (check it for abnormal termination)
	HANDLE hInWaitForSetConBufThread;  // signal that RefreshThread is ready to wait for hWaitForSetConBufThread
	HANDLE hOutWaitForSetConBufThread; // signal that RefreshThread may continue

	HWND   hRootProcessGui; // Если работаем в Gui-режиме (Notepad, Putty, ...), ((HWND)-1) пока фактичеки окно еще не создано, но exe-шник уже есть
	DebuggerInfo DbgInfo;
	DWORD  dwGuiPID; // GUI PID (ИД процесса графической части ConEmu)
	DWORD  dwGuiAID; // ConEmu internal ID of started CRealConsole
	HWND   hGuiWnd; // передается через аргумент "/GHWND=%08X", чтобы окно не искать
	BOOL   bRequestNewGuiWnd;
	DWORD  nActiveFarPID; // PID последнего активного Far
	BOOL   bWasDetached; // Выставляется в TRUE при получении CECMD_DETACHCON
	BOOL   bWasReattached; // Если TRUE - то при следующем цикле нужно передернуть ReloadFullConsoleInfo(true)
	//
	PipeServer<CESERVER_REQ> CmdServer;
	HANDLE hRefreshThread;  DWORD dwRefreshThread;  BOOL bRefreshTermination;
	PipeServer<MSG64> InputServer;
	PipeServer<CESERVER_REQ> DataServer;
	//
	OSVERSIONINFO osv;
	BOOL bReopenHandleAllowed;
	UINT nMaxFPS;
	//
	MSection *csProc;
	MSection *csAltSrv;
	// Список процессов нам нужен, чтобы определить, когда консоль уже не нужна.
	// Например, запустили FAR, он запустил Update, FAR перезапущен...
	UINT nProcessCount, nMaxProcesses;
	UINT nConhostPID; // Windows 7 and higher: "conhost.exe"
	DWORD *pnProcesses, *pnProcessesGet, *pnProcessesCopy, nProcessStartTick;
	DWORD dwProcessLastCheckTick;
#ifndef WIN64
	BOOL bNtvdmActive; DWORD nNtvdmPID;
#endif
	//BOOL bTelnetActive;
	//
	wchar_t szPipename[MAX_PATH], szInputname[MAX_PATH], szGuiPipeName[MAX_PATH], szQueryname[MAX_PATH];
	wchar_t szGetDataPipe[MAX_PATH], szDataReadyEvent[64];
	HANDLE /*hInputPipe,*/ hQueryPipe/*, hGetDataPipe*/;
	//
	MFileMapping<CESERVER_CONSAVE_MAPHDR> *pStoredOutputHdr;
	MFileMapping<CESERVER_CONSAVE_MAP> *pStoredOutputItem;
	//
	MFileMapping<CESERVER_CONSOLE_MAPPING_HDR> *pConsoleMap;
	DWORD guiSettingsChangeNum;
	DWORD nLastConsoleActiveTick;
	ConEmuGuiMapping guiSettings;
	CESERVER_REQ_CONINFO_FULL *pConsole;
	CHAR_INFO *pConsoleDataCopy; // Local (Alloc)
	CRITICAL_SECTION csReadConsoleInfo;
	FGetConsoleDisplayMode pfnWasFullscreenMode;
	// Input
	HANDLE hInputThread;
	DWORD dwInputThread; BOOL bInputTermination;
	HANDLE hInputEvent;   // Выставляется в InputThread, флаг появления новых событий в очереди
	HANDLE hInputWasRead; // Выставляется в InputThread после чтения InputQueue.ReadInputQueue
	//int nInputQueue, nMaxInputQueue;
	//INPUT_RECORD* pInputQueue;
	//INPUT_RECORD* pInputQueueEnd;
	//INPUT_RECORD* pInputQueueRead;
	//INPUT_RECORD* pInputQueueWrite;
	InQueue InputQueue;
	// TrueColorer buffer
	//HANDLE hColorerMapping;
	CRITICAL_SECTION csColorerMappingCreate;
	MFileMapping<const AnnotationHeader>* pColorerMapping; // поддержка Colorer TrueMod
	AnnotationHeader ColorerHdr; // для сравнения индексов
	//
	HANDLE hConEmuGuiAttached;
	HWINEVENTHOOK /*hWinHook,*/ hWinHookStartEnd; //BOOL bWinHookAllow; int nWinHookMode;
	DWORD dwCiRc; CONSOLE_CURSOR_INFO ci; // GetConsoleCursorInfo
	DWORD dwConsoleCP, dwConsoleOutputCP, dwConsoleMode;
	DWORD dwSbiRc; CONSOLE_SCREEN_BUFFER_INFO sbi; // MyGetConsoleScreenBufferInfo
	DWORD dwDisplayMode;
	BOOL  bAltBufferEnabled;
	//USHORT nUsedHeight; // Высота, используемая в GUI - вместо него используем gcrBufferSize.Y
	SHORT nTopVisibleLine; // Прокрутка в GUI может быть заблокирована. Если -1 - без блокировки, используем текущее значение
	SHORT nVisibleHeight;  // По идее, должен быть равен (gcrBufferSize.Y). Это гарантированное количество строк psChars & pnAttrs
	DWORD nMainTimerElapse;
	HANDLE hRefreshEvent; // ServerMode, перечитать консоль, и если есть изменения - отослать в GUI
	HANDLE hRefreshDoneEvent; // ServerMode, выставляется после hRefreshEvent
	HANDLE hDataReadyEvent; // Флаг, что в сервере есть изменения (GUI должен перечитать данные)
	HANDLE hFarCommitEvent; // ServerMode, перечитать консоль, т.к. Far вызвал Commit в ExtConsole, - отослать в GUI
	HANDLE hCursorChangeEvent; // ServerMode, перечитать консоль (облегченный режим), т.к. был изменен курсор, - отослать в GUI
	BOOL   bFarCommitRegistered; // Загружен (в этом! процессе) ExtendedConsole.dll
	BOOL   bCursorChangeRegistered; // Загружен (в этом! процессе) ExtendedConsole.dll
	#ifdef USE_COMMIT_EVENT
	HANDLE hExtConsoleCommit; // Event для синхронизации (выставляется по Commit);
	DWORD  nExtConsolePID;
	#endif
	BOOL bForceConsoleRead; // Пнуть нить опроса консоли RefreshThread чтобы она без задержек перечитала содержимое
	// Смена размера консоли через RefreshThread
	int nRequestChangeSize;
	BOOL bRequestChangeSizeResult;
	USHORT nReqSizeBufferHeight;
	COORD crReqSizeNewSize;
	SMALL_RECT rReqSizeNewRect;
	LPCSTR sReqSizeLabel;
	HANDLE hReqSizeChanged;
	MSection* pReqSizeSection;
	//
	HANDLE hAllowInputEvent; BOOL bInSyncResize;
	//
	DWORD nLastPacketID; // ИД пакета для отправки в GUI

	// Keyboard layout name
	wchar_t szKeybLayout[KL_NAMELENGTH+1];

	// Optional console font (may be specified in registry)
	wchar_t szConsoleFont[LF_FACESIZE];
	//wchar_t szConsoleFontFile[MAX_PATH]; -- не помогает
	SHORT nConFontWidth, nConFontHeight;

	// Когда была последняя пользовательская активность
	DWORD dwLastUserTick;

	// Если нужно заблокировать нить RefreshThread
	//HANDLE hLockRefreshBegin, hLockRefreshReady;

	// Console Aliases
	wchar_t* pszAliases; DWORD nAliasesSize;

	// ComSpec mode
	BOOL  bK;
	BOOL  bNewConsole;
	wchar_t szComSpecName[32];
	wchar_t szSelfName[32];
	wchar_t *pszPreAliases;
	DWORD nPreAliasSize;

	void InitFields()
	{
		InitializeCriticalSection(&csColorerMappingCreate);
		InitializeCriticalSection(&csReadConsoleInfo);
		AltServers.Init();
	};
	void FinalizeFields()
	{
		DeleteCriticalSection(&csColorerMappingCreate);
		DeleteCriticalSection(&csReadConsoleInfo);
	};
};

extern SrvInfo *gpSrv;
extern CEStartupEnv* gpStartEnv;
extern OSVERSIONINFO gOSVer;
extern WORD gnOsVer;
extern bool gbIsWine;
extern bool gbIsDBCS;
extern BOOL gbRootAliveLess10sec;
extern BOOL	gbTerminateOnCtrlBreak;

extern HMODULE ghOurModule;

#define USER_IDLE_TIMEOUT ((DWORD)1000)
#define CHECK_IDLE_TIMEOUT 250 /* 1000 / 4 */
#define USER_ACTIVITY ((gnBufferHeight == 0) || ((GetTickCount() - gpSrv->dwLastUserTick) <= USER_IDLE_TIMEOUT))

void PrintVersion();

//#pragma pack(push, 1)
//extern CESERVER_CONSAVE* gpStoredOutput;
//#pragma pack(pop)
//extern MSection* gpcsStoredOutput;

//typedef struct tag_CmdInfo
//{
//	DWORD dwFarPID;
//	//DWORD dwSrvPID;
//	BOOL  bK;
//	//BOOL  bNonGuiMode; // Если запущен НЕ в консоли, привязанной к GUI. Может быть из-за того, что работает как COMSPEC
//	CONSOLE_SCREEN_BUFFER_INFO sbi;
//	BOOL  bNewConsole;
//	//DWORD nExitCode;
//	wchar_t szComSpecName[32];
//	wchar_t szSelfName[32];
//	wchar_t *pszPreAliases;
//	DWORD nPreAliasSize;
//	// По завершении ComSpec не отключать буфер
//	//BOOL  bWasBufferHeight;
//} CmdInfo;
//
//extern CmdInfo* gpSrv;

extern COORD gcrVisibleSize;
extern BOOL  gbParmVisibleSize, gbParmBufSize;
extern SHORT gnBufferHeight, gnBufferWidth;
extern wchar_t* gpszPrevConTitle;

//extern HANDLE ghLogSize;
//extern wchar_t* wpszLogSizeFile;
class MFileLog;
extern MFileLog* gpLogSize;


extern BOOL gbInRecreateRoot;



#include "ExitCodes.h"


#ifndef __GNUC__
#include <intrin.h>
#else
#define _InterlockedIncrement InterlockedIncrement
#endif


// Message Logger
// Originally from http://preshing.com/20120522/lightweight-in-memory-logging
namespace InputLogger
{
	static const int BUFFER_INFO_SIZE = RELEASEDEBUGTEST(0x1000,0x1000);   // Must be a power of 2
	struct Event {
		DEBUGTEST(DWORD time;)
		enum Source {
			evt_Empty,
			evt_ReadInputQueue,
			evt_SetEvent,
			evt_ResetEvent,
			evt_SendStart,
			evt_SendEnd,
			evt_ProcessInputMessage,
			evt_WriteInputQueue1,
			evt_WaitIntputReady,
			evt_WriteInputQueue2,
			evt_InputQueueFlush,
			evt_Overflow,
		} what;
		LONG val;
		INPUT_RECORD ir;
	};
	extern Event g_evt[BUFFER_INFO_SIZE];
	extern LONG g_evtidx;
	extern LONG g_overflow;

	inline void Log(Event::Source what, LONG val = 0)
	{
		// Get next message index
		// Wrap to buffer size
		LONG i = (_InterlockedIncrement(&g_evtidx) & (BUFFER_INFO_SIZE - 1));
		// Write a message at this index
		g_evt[i].what = what;
		DEBUGTEST(g_evt[i].time = GetTickCount();)
		g_evt[i].val = val;
	}

	inline void Log(Event::Source what, const INPUT_RECORD& ir, LONG val = 0)
	{
		// Get next message index
		// Wrap to buffer size
		LONG i = (_InterlockedIncrement(&g_evtidx) & (BUFFER_INFO_SIZE - 1));
		// Write a message at this index
		g_evt[i].what = what;
		DEBUGTEST(g_evt[i].time = GetTickCount();)
		// Fill info
		g_evt[i].val = val;
		g_evt[i].ir = ir;		
	}
}
