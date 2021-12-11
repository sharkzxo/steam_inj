/* github.com/sharkzxo/ */

#include "tool.hpp"

int main()
{
    std::printf("[steam_inj] started.. \n");

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
    static const std::string& cheat("cheat.dll");

    auto process = find_proc("csgo.exe");
    auto module = find_module("crashhandler.dll");

    // check if the process running before doing same changes then ask for exit
    while (process) {
        process = find_proc("csgo.exe");
        std::printf("[~] please close csgo.exe..");
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    // check if we created copy in the previous time & delete (avoid crash)
    if (std::filesystem::exists((steam_path + copy))) {
        std::printf("[~] copy file exists [%s] \n", copy.data());
        std::remove(copy.data());
    }

    // create copy of crashhandler.dll
    std::filesystem::rename((steam_path + original), (steam_path + copy));

    std::printf("[+] file renamed [%s] \n", copy.data());

    // check if our dll exist 
    if (std::filesystem::exists(cheat.data()))
        std::printf("[+] our dll found [%s] \n", cheat.data());

    // lets copy our dll to csgo file and rename
    std::filesystem::copy_file(cheat, (steam_path + original));

    // check if the our dll is placed as original
    if (std::filesystem::exists((steam_path + original)))
        std::printf("[+] our dll is placed as original [%s] \n", original.data());

    // loop for process
    while (!process) {
        process = find_proc("csgo.exe");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // let us know if the csgo was found
    std::printf("[+] the target process was found \n");

    // loop for module
    while (!module) {
        module = find_module("crashhandler.dll");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // let us know if the dll was found in the target process
    std::printf("[+] our dll found in target process as [%s] \n", original.data());

    // let us know if the dll is active in the target process
    std::printf("\n[!] software is active in the [%lu] process \n", process);

    // lets rename the fake original to cheat
    std::filesystem::rename((steam_path + original), (steam_path + cheat));

    // lets rename the copy as the original
    std::filesystem::rename((steam_path + copy), (steam_path + original));

    // let us know if the dll was found in the target process
    std::printf("[+] our changes was removed now backup the original [%s] \n", original.data());

    std::printf("[~] github.com/sharkzxo/\n");

    // would delete this its just cool to see messages
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    // exit the process if we everything done
    ExitProcess(0);
}
