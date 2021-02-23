/*
 * This file is part of CLionDeleaker (https://github.com/nedrysoft/CLionDeleaker)
 *
 * MIT License
 *
 * Copyright (c) 2021 Adrian Carpenter
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <windows.h>
#include <stdio.h>
#include <iostream>

char *titleMatchString = nullptr;

bool windowFinder(HWND hWnd, LPARAM lParam) {
    char titleString[MAX_PATH];
    char processName[MAX_PATH];
    DWORD processId = 0;

    GetWindowText(hWnd, (LPSTR) titleString, sizeof(titleString));

    GetWindowThreadProcessId(hWnd, &processId);

    if (!processId) {
        return true;
    }

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ , FALSE, processId);

    if (!hProcess) {
        return true;
    }

    DWORD processNameLength = sizeof(processName);

    if (QueryFullProcessImageNameA(hProcess, 0, (LPSTR) processName,  &processNameLength)) {
        if (strstr(processName, "CLion")) {
            if (strstr(titleString, titleMatchString)) {
                SetForegroundWindow(hWnd);
            }
        }
    }

    CloseHandle(hProcess);

    return true;
}

int main(int argc, char *argv[]) {
    STARTUPINFO startupInfo;
    PROCESS_INFORMATION processInfo;
    char commandLine[32765];

    ZeroMemory(&startupInfo, sizeof(startupInfo) );
    ZeroMemory(&processInfo, sizeof(processInfo) );

    startupInfo.cb = sizeof(startupInfo);

    if (argc!=5) {
        std::cout << "CLionDeleaker <path to CLion.exe> <CLion project name> <line number> <filename>" << std::endl;

        return 1;
    }

    titleMatchString = argv[2];

    sprintf(commandLine, "%s --line %s %s", argv[1], argv[3], argv[4]);

    if (!CreateProcess(nullptr, commandLine, nullptr, nullptr, FALSE, 0, nullptr,nullptr, &startupInfo, &processInfo)) {
        std::cout << "CLionDeleaker: Error, unable to launch CLion." << std::endl;

        return 1;
    }

    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);

    EnumWindows((WNDENUMPROC) windowFinder, 0);

    WaitForSingleObject(processInfo.hProcess, 10000);

    return 0;
}
