/* github.com/sharkzxo/ */

#include "tool.hpp"

int main()
{
    // first message appear in the console
    std::printf("[steam_inj] app started.. \n\n");

    // get steam path (ex. C:\\Program Files (x86)\\Steam\\)
    HKEY rnd;
    char steam_path[MAX_PATH];
    RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Valve\\Steam", 0, KEY_READ, &rnd);
    DWORD pathBytes = sizeof(steam_path);
    RegQueryValueEx(rnd, "SteamPath", nullptr, nullptr, (BYTE*)steam_path, &pathBytes);
    RegCloseKey(rnd);

    DWORD pathChars = pathBytes / sizeof(wchar_t);
    for (DWORD i = 0; i < pathChars; ++i) {
        if (steam_path[i] == L'/') {
            steam_path[i] = L'\\';
        }
    }

    static const std::string& original("\\crashhandler.dll");
    static const std::string& copy("\\crashhandler_original.dll");
    static const std::string& cheat("\\cheat.dll");

    auto process = find_proc("csgo.exe");
    auto module_crashhandler = find_module("crashhandler.dll");

    // check if the process running before doing same changes then ask for exit
    while (process) {
        process = find_proc("csgo.exe");
        std::printf("[~] please close csgo.exe..");
        Sleep(100);
    }

    // check if we created copy in the previous time & delete (avoid crash)
    if (std::filesystem::exists((steam_path + copy))) {
        std::printf("[~] copy file exists [%s] \n", copy.data());
        std::remove(copy.data());
    }

    // rename original crashhandler.dll to copy
    std::filesystem::rename((steam_path + original), (steam_path + copy));

    // let us know the file was renamed
    std::printf("[+] file renamed [%s] \n", copy.data());

    // check if dll exist 
    if (std::filesystem::exists(cheat.data()))
        std::printf("[+] dll found [%s] \n", cheat.data());

    // lets copy dll to csgo file and rename
    std::filesystem::copy_file(cheat, (steam_path + original));

    // check if the dll is placed as original
    if (std::filesystem::exists((steam_path + original)))
        std::printf("[+] dll is placed as original [%s] \n", original.data());

    // loop for process
    while (!process) {
        process = find_proc("csgo.exe");
        Sleep(100);
    }

    // let us know if the csgo was found
    std::printf("[+] the target process was found \n");

    // loop for module
    while (!module_crashhandler) {
        module_crashhandler = find_module("crashhandler.dll");
        Sleep(100);
    }

    // let us know if the dll was found in the target process
    std::printf("[+] dll found in target process as [%s] [%lu] \n", original.data(), module_crashhandler);

    // pause warning message
    std::printf("\n[+] app is paused for 10 seconds \n\n");

    // pause
    Sleep(10000);

    // let us know if the dll is active in the target process
    std::printf("\n[!] software is active in the [%lu] process [%lu] module \n", process, module_crashhandler);

    // lets rename the fake original to cheat
    std::filesystem::rename((steam_path + original), (steam_path + cheat));

    // lets rename the copy as the original
    std::filesystem::rename((steam_path + copy), (steam_path + original));

    // just message if everything was backup'd
    std::printf("[+] changes was removed now backup the original [%s] \n", original.data());

    std::printf("[~] github.com/sharkzxo/\n");

    // would delete this its just cool to see messages
    Sleep(2500);

    // exit the process if we everything done
    ExitProcess(0);
}
