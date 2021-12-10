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
                std::string_view sName(pe32.szExeFile);

                if (!lstrcmpiA(sName.data(), name)) {
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
    HANDLE _entry = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, find_proc("csgo.exe"));
    MODULEENTRY32 _module;
    _module.dwSize = sizeof(MODULEENTRY32);
    Module32First(_entry, &_module);
    if (_entry == INVALID_HANDLE_VALUE)
        return 0;
    do
    {
        if (!strcmp(_module.szModule, name))
            break;
    } while (Module32Next(_entry, &_module));
    CloseHandle(_entry);

    return (DWORD)_module.modBaseAddr;
}
