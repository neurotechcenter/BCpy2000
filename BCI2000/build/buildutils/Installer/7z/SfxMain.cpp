// Main.cpp

#include "StdAfx.h"

#include "Common/MyInitGuid.h"

#include "Common/CommandLineParser.h"
#include "Common/StringConvert.h"
#include "Common/TextConfig.h"

#include "Windows/DLL.h"
#include "Windows/FileDir.h"
#include "Windows/FileFind.h"
#include "Windows/FileIO.h"
#include "Windows/NtCheck.h"
#include "Windows/ResourceString.h"

#include "../../UI/Explorer/MyMessages.h"
#include "../../UI/GUI/ExtractDialog.h"

#include "ExtractEngine.h"

#include "resource.h"

using namespace NWindows;

HINSTANCE g_hInstance;

extern UString gFriendlyName;

static LPCTSTR kTempDirPrefix = TEXT("7zS");

#define _SHELL_EXECUTE

static bool ReadDataString(LPCWSTR fileName, LPCSTR startID,
    LPCSTR endID, AString &stringResult)
{
  stringResult.Empty();
  NFile::NIO::CInFile inFile;
  if (!inFile.Open(fileName))
    return false;
  const int kBufferSize = (1 << 12);

  Byte buffer[kBufferSize];
  int signatureStartSize = MyStringLen(startID);
  int signatureEndSize = MyStringLen(endID);
  
  UInt32 numBytesPrev = 0;
  bool writeMode = false;
  UInt64 posTotal = 0;
  for (;;)
  {
    if (posTotal > (1 << 20))
      return (stringResult.IsEmpty());
    UInt32 numReadBytes = kBufferSize - numBytesPrev;
    UInt32 processedSize;
    if (!inFile.Read(buffer + numBytesPrev, numReadBytes, processedSize))
      return false;
    if (processedSize == 0)
      return true;
    UInt32 numBytesInBuffer = numBytesPrev + processedSize;
    UInt32 pos = 0;
    for (;;)
    {
      if (writeMode)
      {
        if (pos > numBytesInBuffer - signatureEndSize)
          break;
        if (memcmp(buffer + pos, endID, signatureEndSize) == 0)
          return true;
        char b = buffer[pos];
        if (b == 0)
          return false;
        stringResult += b;
        pos++;
      }
      else
      {
        if (pos > numBytesInBuffer - signatureStartSize)
          break;
        if (memcmp(buffer + pos, startID, signatureStartSize) == 0)
        {
          writeMode = true;
          pos += signatureStartSize;
        }
        else
          pos++;
      }
    }
    numBytesPrev = numBytesInBuffer - pos;
    posTotal += pos;
    memmove(buffer, buffer + pos, numBytesPrev);
  }
}

static char kStartID[] = ",!@Install@!UTF-8!";
static char kEndID[] = ",!@InstallEnd@!";

class CInstallIDInit
{
public:
  CInstallIDInit()
  {
    kStartID[0] = ';';
    kEndID[0] = ';';
  };
} g_CInstallIDInit;


#ifndef UNDER_CE
class CCurrentDirRestorer
{
  CSysString m_CurrentDirectory;
public:
  CCurrentDirRestorer() { NFile::NDirectory::MyGetCurrentDirectory(m_CurrentDirectory); }
  ~CCurrentDirRestorer() { RestoreDirectory();}
  bool RestoreDirectory() { return BOOLToBool(::SetCurrentDirectory(m_CurrentDirectory)); }
};
#endif

#define NT_CHECK_FAIL_ACTION ShowErrorMessage(L"Unsupported Windows version"); return 1;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /* hPrevInstance */,
    #ifdef UNDER_CE
    LPWSTR
    #else
    LPSTR
    #endif
    /* lpCmdLine */,int /* nCmdShow */)
{
  g_hInstance = (HINSTANCE)hInstance;

  NT_CHECK

  // InitCommonControls();

  UString archiveName, switches;
  #ifdef _SHELL_EXECUTE
  UString executeFile, executeParameters;
  #endif
  NCommandLineParser::SplitCommandLine(GetCommandLineW(), archiveName, switches);

  UString fullPath;
  NDLL::MyGetModuleFileName(g_hInstance, fullPath);

  switches.Trim();
  bool assumeYes = false;
  if (switches.Left(2).CompareNoCase(UString(L"-y")) == 0)
  {
    assumeYes = true;
    switches = switches.Mid(2);
    switches.Trim();
  }

  AString config;
  if (!ReadDataString(fullPath, kStartID, kEndID, config))
  {
    if (!assumeYes)
      ShowErrorMessage(L"Can't load config info");
    return 1;
  }

  UString dirPrefix = L"." WSTRING_PATH_SEPARATOR;
  UString appLaunched,
        dirPrompt,
        donePrompt,
        abortPrompt,
        friendlyName;
  bool showProgress = true;
  bool showExtractDialog = false;
  if (!config.IsEmpty())
  {
    CObjectVector<CTextConfigPair> pairs;
    if (!GetTextConfig(config, pairs))
    {
      if (!assumeYes)
        ShowErrorMessage(L"Config failed");
      return 1;
    }
    friendlyName = GetTextConfigValue(pairs, L"Title");
    gFriendlyName = friendlyName;
    UString installPrompt = GetTextConfigValue(pairs, L"BeginPrompt");
    UString extractDialog = GetTextConfigValue(pairs, L"ShowExtractDialog");
    if(extractDialog.CompareNoCase(L"yes") == 0)
      showExtractDialog = true;
    donePrompt = GetTextConfigValue(pairs, L"DonePrompt");
    abortPrompt = GetTextConfigValue(pairs, L"AbortPrompt");
    UString progress = GetTextConfigValue(pairs, L"Progress");
    if (progress.CompareNoCase(L"no") == 0)
      showProgress = false;
    int index = FindTextConfigItem(pairs, L"Directory");
    if (index >= 0)
      dirPrefix = pairs[index].String;
    if (!installPrompt.IsEmpty() && !assumeYes)
    {
      if (MessageBoxW(0, installPrompt, friendlyName, MB_YESNO |
          MB_ICONQUESTION) != IDYES)
        return 0;
    }
    appLaunched = GetTextConfigValue(pairs, L"RunProgram");
    
    #ifdef _SHELL_EXECUTE
    executeFile = GetTextConfigValue(pairs, L"ExecuteFile");
    executeParameters = GetTextConfigValue(pairs, L"ExecuteParameters") + switches;
    #endif
  }

  CCodecs *codecs = new CCodecs;
  CMyComPtr<IUnknown> compressCodecsInfo = codecs;
  HRESULT result = codecs->Load();
  if (result != S_OK)
  {
    ShowErrorMessage(L"Can not load codecs");
    return 1;
  }

  UString tempDirPath;
  NFile::NDirectory::CTempDirectory tempDir;
  if( !showExtractDialog )
  {
    if (!tempDir.Create(kTempDirPrefix))
    {
      if (!assumeYes)
        ShowErrorMessage(L"Can not create temp folder archive.");
      return 1;
    }
    tempDirPath = GetUnicodeString(tempDir.GetPath());
  }
  else
  {
    #ifndef UNDER_CE
    NFile::NDirectory::MyGetCurrentDirectory(tempDirPath);
    #endif
    CExtractDialog dialog;
    bool dirCreated = false;
    while( !dirCreated )
    {
      dialog.DirectoryPath = tempDirPath;
      NFile::NName::NormalizeDirPathPrefix(dialog.DirectoryPath);
      INT_PTR result = dialog.Create(NULL);
      if( result == -1 )
      {
        if( !assumeYes )
          ShowErrorMessage(L"Could not open file selection dialog (missing resource?)");
        return -1;
      }
      if (result != IDOK)
      {
        if (!abortPrompt.IsEmpty() && !assumeYes)
          ::MessageBoxW(0, abortPrompt, friendlyName, MB_YESNO);
        return 1;
      }
      NFile::NName::NormalizeDirPathPrefix(dialog.DirectoryPath);
      if( !NFile::NDirectory::CreateComplexDirectory(dialog.DirectoryPath) )
      {
        if( !assumeYes )
          ShowErrorMessage(L"Could not create installation directory.");
      }
      else
      {
        dirCreated = true;
      }
    }
    if (!NFile::NDirectory::MyGetFullPathName(dialog.DirectoryPath, tempDirPath))
    {
      if( !assumeYes )
        ShowErrorMessage(L"Incorrect output directory path.");
      return 1;
    }
  }
  {
    bool isCorrupt = false;
    UString errorMessage;
    HRESULT result = ExtractArchive(codecs, fullPath, tempDirPath, showProgress,
      isCorrupt, errorMessage);
    
    if (result != S_OK)
    {
      if (!assumeYes)
      {
        if (result == S_FALSE || isCorrupt)
        {
          errorMessage = NWindows::MyLoadStringW(IDS_EXTRACTION_ERROR_MESSAGE);
          result = E_FAIL;
        }
        if (result != E_ABORT && !errorMessage.IsEmpty())
          ::MessageBoxW(0, errorMessage, NWindows::MyLoadStringW(IDS_EXTRACTION_ERROR_TITLE), MB_ICONERROR);
      }
      return 1;
    }
  }

  #ifndef UNDER_CE
  CCurrentDirRestorer currentDirRestorer;
  if (!SetCurrentDirectory(GetSystemString(tempDirPath)))
    return 1;
  #endif
  
  HANDLE hProcess = 0;
#ifdef _SHELL_EXECUTE
  if (!executeFile.IsEmpty())
  {
    CSysString filePath = GetSystemString(executeFile);
    SHELLEXECUTEINFO execInfo;
    execInfo.cbSize = sizeof(execInfo);
    execInfo.fMask = SEE_MASK_NOCLOSEPROCESS
      #ifndef UNDER_CE
      | SEE_MASK_FLAG_DDEWAIT
      #endif
      ;
    execInfo.hwnd = NULL;
    execInfo.lpVerb = NULL;
    execInfo.lpFile = filePath;

    if (!switches.IsEmpty())
      executeParameters += switches;

    CSysString parametersSys = GetSystemString(executeParameters);
    if (parametersSys.IsEmpty())
      execInfo.lpParameters = NULL;
    else
      execInfo.lpParameters = parametersSys;

    execInfo.lpDirectory = NULL;
    execInfo.nShow = SW_SHOWNORMAL;
    execInfo.hProcess = 0;
    /* BOOL success = */ ::ShellExecuteEx(&execInfo);
    UINT32 result = (UINT32)(UINT_PTR)execInfo.hInstApp;
    if(result <= 32)
    {
      if (!assumeYes)
        ShowErrorMessage(L"Can not open file.");
      return 1;
    }
    hProcess = execInfo.hProcess;
  }
  else
#endif
  {
    if (!appLaunched.IsEmpty())
    {
      {
        UString s2 = tempDirPath;
        NFile::NName::NormalizeDirPathPrefix(s2);
        appLaunched.Replace(L"%%T" WSTRING_PATH_SEPARATOR, s2);
      }
    
      appLaunched.Replace(L"%%T", tempDirPath);

      if (!switches.IsEmpty())
      {
        appLaunched += L' ';
        appLaunched += switches;
      }
      STARTUPINFO startupInfo;
      startupInfo.cb = sizeof(startupInfo);
      startupInfo.lpReserved = 0;
      startupInfo.lpDesktop = 0;
      startupInfo.lpTitle = 0;
      startupInfo.dwFlags = 0;
      startupInfo.cbReserved2 = 0;
      startupInfo.lpReserved2 = 0;
    
      PROCESS_INFORMATION processInformation;
    
      CSysString appLaunchedSys = GetSystemString(dirPrefix + appLaunched);
    
      BOOL createResult = CreateProcess(NULL, (LPTSTR)(LPCTSTR)appLaunchedSys,
        NULL, NULL, FALSE, 0, NULL, NULL /*tempDir.GetPath() */,
        &startupInfo, &processInformation);
      if (createResult == 0)
      {
        if (!assumeYes)
          ShowLastErrorMessage();
        return 1;
      }
      ::CloseHandle(processInformation.hThread);
      hProcess = processInformation.hProcess;
    }
  }
  if (hProcess != 0)
  {
    WaitForSingleObject(hProcess, INFINITE);
    ::CloseHandle(hProcess);
  }
  if (!donePrompt.IsEmpty() && !assumeYes)
  {
    donePrompt.Replace(L"%%T", tempDirPath);
    ::MessageBoxW(0, donePrompt, friendlyName, MB_OK);
  }
  return 0;
}
