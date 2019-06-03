#ifndef TCONSOLE
#define TCONSOLE

#ifdef _WIN32
#pragma once

#include <stdio.h>
#include <io.h>    // _open_osfhandle()
#include <fcntl.h> // _O_TEXT
#include <conio.h>
#include <wincon.h>
#include <iostream>

namespace tlib
{
      class TConsole
      {
            inline static bool IsConsole = false;
            inline static bool IsConsoleAttach = false;

        public:
            TConsole()
            {
                  if (!IsConsole)
                  {
                        if ((IsConsoleAttach = AttachConsole(GetCurrentProcessId())) == false)
                        {
                              if (GetLastError() == ERROR_ACCESS_DENIED)
                              { // Если вызывающий процесс уже подключен к консоли, возвращается код ошибки: ERROR_ACCESS_DENIED
                              }
                              else
                              {
                                    if ((IsConsoleAttach = AllocConsole()) == false)
                                          return;
                              }
                        }
                        else
                        {
                              FILE* pFile = nullptr;
                              errno_t err = 0;
                              err = freopen_s(&pFile, "CON", "w", stdout);
                              err = freopen_s(&pFile, "CON", "r", stdin);
                              err = freopen_s(&pFile, "CON", "w", stderr);
                        }

                        IsConsole = true;

                        // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
                        // point to console as well
                        std::ios::sync_with_stdio();

                        DWORD dwMode = 0;
                        GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &dwMode);
                        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                        SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), dwMode);
                  }
            }

            TConsole(DWORD dwProcessId)
            {
                  if (IsConsole)
                  {
                        if (IsConsoleAttach)
                              FreeConsole();
                  }                        
                  else
                  {
                        if ((IsConsole = AttachConsole(dwProcessId)) == false)
                              return;

                        FILE* pFile = nullptr;
                        errno_t err = 0;
                        err = freopen_s(&pFile, "CON", "w", stdout);
                        err = freopen_s(&pFile, "CON", "r", stdin);
                        err = freopen_s(&pFile, "CON", "w", stderr);

                        // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
                        // point to console as well
                        std::ios::sync_with_stdio();
                  }
            }

            void SetScreenBufferSize(SHORT width, SHORT height)
            {
                  if (IsConsole)
                  {
                        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                        CONSOLE_SCREEN_BUFFER_INFO scrBuffInfo = { 0 };
                        GetConsoleScreenBufferInfo(hConsole, &scrBuffInfo);

                        scrBuffInfo.dwSize.X = width + 1;
                        scrBuffInfo.dwSize.Y = height;
                        SetConsoleScreenBufferSize(hConsole, scrBuffInfo.dwSize);
                  }
            }

            void SetWindowSize(SHORT width, SHORT height)
            {
                  if (IsConsole)
                  {
                        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                        CONSOLE_SCREEN_BUFFER_INFO scrBuffInfo = { 0 };
                        GetConsoleScreenBufferInfo(hConsole, &scrBuffInfo);

                        scrBuffInfo.srWindow.Right = width;
                        scrBuffInfo.srWindow.Bottom = height;
                        SetConsoleWindowInfo(hConsole, true, &scrBuffInfo.srWindow);
                  }
            }

            ~TConsole()
            {
                  if (IsConsole)
                  {
                        if (IsConsoleAttach)
                                 FreeConsole();
                        IsConsole = false;
                        IsConsoleAttach = false;
                  }
            }
      };

}

#endif // _WIN32

#endif // TREGEX
