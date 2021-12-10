#pragma once
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <tlhelp32.h>
#include <thread>

DWORD find_proc(LPCTSTR name)
{
    PROCESSENTRY32 pe32;
    HANDLE snapshot = NULL;
    DWORD pid = 0;

    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE) {
        pe32.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(snapshot, &pe32)) {
            do {
                std::string sName = pe32.szExeFile;
                std::transform(sName.begin(), sName.end(), sName.begin(), ::tolower);

                if (!lstrcmp(sName.c_str(), name)) {
                    pid = pe32.th32ProcessID;
                    break;
                }
            } while (Process32Next(snapshot, &pe32));
        }

        CloseHandle(snapshot);
    }

    return pid;
}

DWORD find_module(LPCTSTR name)
{
    HANDLE _module = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, find_proc("csgo.exe"));
    MODULEENTRY32 _entry;
    _entry.dwSize = sizeof(_entry);
    do
        if (!strcmp(_entry.szModule, name))
        {
            CloseHandle(_module);
            return (DWORD)_entry.modBaseAddr;
        }
    while (Module32Next(_module, &_entry));
    return 0;
}
