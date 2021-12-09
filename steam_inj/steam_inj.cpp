#include "tool.hpp"

// https://github.com/blxckshark/ (2021)
// 
// we will use this as free undetected injector by valve (at this moment)
// this would be patched in the future actually it works well
// this idea was done through the public education (its up from 2k18 +-)
// but this tool is automatized and doesnt has any issues
// 
// why do I replacing our fake original back to the right original? 
// at this momnent we just want to avoid some issues with injecting
// thats mean if we keep our fake original then every time when u open the game
// the cheat get injected so that's my way how to avoid this issue
// if you have some problems with compile please use (release x86)

// place this application into the steam folder and name your cheat as "cheat.dll" (default)

int main()
{
    static std::string original = "crashhandler.dll";
    static std::string copy = "crashhandler_original.dll";
    static std::string cheat = "cheat.dll";

    auto process = find_proc("csgo.exe");
    auto module = find_module("crashhandler.dll");

    // check if the process running before doing same changes then ask for exit
    while (process) {
        process = find_proc("csgo.exe");
        std::printf("[~] please close csgo.exe..");
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    // check if we created copy in the previous time & delete (avoid crash)
    if (std::filesystem::exists(copy.c_str())) {
        std::printf("[~] copy file exists [%s] \n", copy.c_str());
        std::remove(copy.c_str());
    }

    // create copy of crashhandler.dll
    if (std::rename(original.c_str(), copy.c_str()))
        std::printf("[+] file renamed [%s] \n", copy.c_str());

    // check if our dll exist 
    if (std::filesystem::exists(cheat.c_str()))
        std::printf("[+] our dll found [%s] \n", cheat.c_str());

    // lets rename our dll to original
    std::filesystem::rename(cheat.c_str(), original.c_str());

    // check if the our dll is placed as original
    if (std::filesystem::exists(original.c_str()))
        std::printf("[+] our dll is placed as original [%s] \n", original.c_str());

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
    std::printf("[+] our dll found in target process as [%s] \n", original.c_str());

    // lets rename our fake original to cheat
    std::filesystem::rename(original, cheat);

    // lets rename the copy as the original
    std::filesystem::rename(copy, original);

    // would delete this its just cool to see messages
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    // exit the process if we everything done
    ExitProcess(0);
}