/**
 * @file injector.cpp
 * @brief Remote Thread DLL Injector
 *
 * Uses CreateRemoteThread + LoadLibrary method to inject DLL into target process
 */

#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <string>
#include <iostream>
#include <vector>

// Console color output
namespace console {
    inline void info(const std::wstring& msg) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11); // Cyan
        std::wcout << L"[INFO] " << msg << std::endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }

    inline void success(const std::wstring& msg) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); // Green
        std::wcout << L"[SUCCESS] " << msg << std::endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }

    inline void error(const std::wstring& msg) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); // Red
        std::wcerr << L"[ERROR] " << msg << std::endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }

    inline void warning(const std::wstring& msg) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // Yellow
        std::wcout << L"[WARNING] " << msg << std::endl;
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }
}

/**
 * @brief Get process ID by process name
 * @param processName Process name (e.g. "left4dead2.exe")
 * @return Process ID, returns 0 on failure
 */
DWORD GetProcessIdByName(const std::wstring& processName) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(hSnapshot, &pe32)) {
        do {
            if (_wcsicmp(pe32.szExeFile, processName.c_str()) == 0) {
                CloseHandle(hSnapshot);
                return pe32.th32ProcessID;
            }
        } while (Process32NextW(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return 0;
}

/**
 * @brief Get process list (for user selection)
 * @return Process list {process name, PID}
 */
std::vector<std::pair<std::wstring, DWORD>> GetProcessList() {
    std::vector<std::pair<std::wstring, DWORD>> processes;

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return processes;
    }

    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(hSnapshot, &pe32)) {
        do {
            processes.emplace_back(pe32.szExeFile, pe32.th32ProcessID);
        } while (Process32NextW(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return processes;
}

/**
 * @brief Check if process is 32-bit
 * @param hProcess Process handle
 * @return true for 32-bit process
 */
bool IsWow64ProcessTarget(HANDLE hProcess) {
    BOOL isWow64 = FALSE;
    IsWow64Process(hProcess, &isWow64);
    return isWow64 == TRUE;
}

/**
 * @brief Inject DLL using remote thread
 * @param processId Target process ID
 * @param dllPath Full DLL path
 * @return true on success
 */
bool InjectDLL(DWORD processId, const std::wstring& dllPath) {
    // Open target process
    HANDLE hProcess = OpenProcess(
        PROCESS_CREATE_THREAD |
        PROCESS_QUERY_INFORMATION |
        PROCESS_VM_OPERATION |
        PROCESS_VM_WRITE |
        PROCESS_VM_READ,
        FALSE,
        processId
    );

    if (!hProcess) {
        console::error(L"Cannot open target process, error code: " + std::to_wstring(GetLastError()));
        console::error(L"Please run injector as Administrator!");
        return false;
    }

    // Check target process architecture
    BOOL isWow64 = FALSE;
    IsWow64Process(hProcess, &isWow64);

#if defined(_WIN64)
    if (isWow64) {
        console::warning(L"Target process is 32-bit, but injector is 64-bit");
        console::error(L"Please use 32-bit version of injector!");
        CloseHandle(hProcess);
        return false;
    }
#else
    if (!isWow64) {
        // Target process is native architecture
        BOOL selfWow64 = FALSE;
        IsWow64Process(GetCurrentProcess(), &selfWow64);
        if (selfWow64) {
            console::warning(L"Target process may be 64-bit");
        }
    }
#endif

    // Get LoadLibraryW address
    HMODULE hKernel32 = GetModuleHandleW(L"kernel32.dll");
    if (!hKernel32) {
        console::error(L"Cannot get kernel32.dll module handle");
        CloseHandle(hProcess);
        return false;
    }

    FARPROC pLoadLibraryW = GetProcAddress(hKernel32, "LoadLibraryW");
    if (!pLoadLibraryW) {
        console::error(L"Cannot get LoadLibraryW address");
        CloseHandle(hProcess);
        return false;
    }

    // Allocate memory in target process
    size_t pathSize = (dllPath.length() + 1) * sizeof(wchar_t);
    LPVOID pRemotePath = VirtualAllocEx(
        hProcess,
        NULL,
        pathSize,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );

    if (!pRemotePath) {
        console::error(L"Cannot allocate memory in target process, error code: " + std::to_wstring(GetLastError()));
        CloseHandle(hProcess);
        return false;
    }

    // Write DLL path
    if (!WriteProcessMemory(
        hProcess,
        pRemotePath,
        dllPath.c_str(),
        pathSize,
        NULL
    )) {
        console::error(L"Cannot write DLL path to target process");
        VirtualFreeEx(hProcess, pRemotePath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    // Create remote thread to execute LoadLibraryW
    HANDLE hThread = CreateRemoteThread(
        hProcess,
        NULL,
        0,
        reinterpret_cast<LPTHREAD_START_ROUTINE>(pLoadLibraryW),
        pRemotePath,
        0,
        NULL
    );

    if (!hThread) {
        console::error(L"Cannot create remote thread, error code: " + std::to_wstring(GetLastError()));
        VirtualFreeEx(hProcess, pRemotePath, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    // Wait for thread to complete
    console::info(L"Waiting for DLL injection...");
    WaitForSingleObject(hThread, INFINITE);

    // Get thread exit code (module handle returned by LoadLibrary)
    DWORD exitCode = 0;
    GetExitCodeThread(hThread, &exitCode);

    // Cleanup
    VirtualFreeEx(hProcess, pRemotePath, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    if (exitCode == 0) {
        console::error(L"DLL injection failed! LoadLibrary returned NULL");
        console::error(L"Possible causes: Wrong DLL path, missing dependencies, or DLL init failed");
        return false;
    }

    return true;
}

/**
 * @brief Eject DLL from remote process
 * @param processId Target process ID
 * @param dllPath Full DLL path (for module matching)
 * @return true on success
 */
bool EjectDLL(DWORD processId, const std::wstring& dllPath) {
    HANDLE hProcess = OpenProcess(
        PROCESS_CREATE_THREAD |
        PROCESS_QUERY_INFORMATION |
        PROCESS_VM_OPERATION,
        FALSE,
        processId
    );

    if (!hProcess) {
        console::error(L"Cannot open target process");
        return false;
    }

    // Find DLL module
    HMODULE hModules[1024];
    DWORD cbNeeded;

    if (!EnumProcessModules(hProcess, hModules, sizeof(hModules), &cbNeeded)) {
        console::error(L"Cannot enumerate target process modules");
        CloseHandle(hProcess);
        return false;
    }

    HMODULE hTargetModule = nullptr;
    WCHAR modulePath[MAX_PATH];

    for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
        if (GetModuleFileNameExW(hProcess, hModules[i], modulePath, MAX_PATH)) {
            if (_wcsicmp(modulePath, dllPath.c_str()) == 0) {
                hTargetModule = hModules[i];
                break;
            }
        }
    }

    if (!hTargetModule) {
        console::warning(L"DLL module not found in target process");
        CloseHandle(hProcess);
        return false;
    }

    // Get FreeLibrary address
    HMODULE hKernel32 = GetModuleHandleW(L"kernel32.dll");
    FARPROC pFreeLibrary = GetProcAddress(hKernel32, "FreeLibrary");

    // Create remote thread to execute FreeLibrary
    HANDLE hThread = CreateRemoteThread(
        hProcess,
        NULL,
        0,
        reinterpret_cast<LPTHREAD_START_ROUTINE>(pFreeLibrary),
        hTargetModule,
        0,
        NULL
    );

    if (!hThread) {
        console::error(L"Cannot create remote thread");
        CloseHandle(hProcess);
        return false;
    }

    WaitForSingleObject(hThread, INFINITE);

    DWORD exitCode = 0;
    GetExitCodeThread(hThread, &exitCode);

    CloseHandle(hThread);
    CloseHandle(hProcess);

    return exitCode != 0;
}

void PrintBanner() {
    std::wcout << L"\n";
    std::wcout << L"  ===========================================\n";
    std::wcout << L"       DLL Injector - Remote Thread\n";
    std::wcout << L"          CreateRemoteThread API\n";
    std::wcout << L"  ===========================================\n";
    std::wcout << L"\n";
}

void PrintUsage() {
    std::wcout << L"Usage:\n";
    std::wcout << L"  injector.exe inject <process|PID> <DLL_path>   - Inject DLL\n";
    std::wcout << L"  injector.exe eject <process|PID> <DLL_path>    - Eject DLL\n";
    std::wcout << L"  injector.exe list                              - List all processes\n";
    std::wcout << L"\n";
    std::wcout << L"Examples:\n";
    std::wcout << L"  injector.exe inject left4dead2.exe cheat.dll\n";
    std::wcout << L"  injector.exe inject 1234 C:\\path\\to\\cheat.dll\n";
    std::wcout << L"  injector.exe eject left4dead2.exe cheat.dll\n";
}

int wmain(int argc, wchar_t* argv[]) {
    // Set console code page
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    PrintBanner();

    if (argc < 2) {
        PrintUsage();
        return 1;
    }

    std::wstring command = argv[1];

    // List processes
    if (command == L"list") {
        console::info(L"Enumerating process list...\n");

        auto processes = GetProcessList();

        std::wcout << L"+--------------------------------+----------+\n";
        std::wcout << L"| Process Name                   | PID      |\n";
        std::wcout << L"+--------------------------------+----------+\n";

        for (const auto& [name, pid] : processes) {
            std::wcout << L"| " << name;
            for (size_t i = name.length(); i < 28; i++) std::wcout << L" ";
            std::wcout << L"| " << pid;
            for (int i = std::to_wstring(pid).length(); i < 8; i++) std::wcout << L" ";
            std::wcout << L"|\n";
        }

        std::wcout << L"+--------------------------------+----------+\n";
        std::wcout << L"Total: " << processes.size() << L" processes\n";
        return 0;
    }

    // Inject/Eject commands
    if (argc < 4) {
        console::error(L"Not enough arguments!");
        PrintUsage();
        return 1;
    }

    std::wstring target = argv[2];
    std::wstring dllPath = argv[3];

    // Get absolute DLL path
    WCHAR absPath[MAX_PATH];
    if (!GetFullPathNameW(dllPath.c_str(), MAX_PATH, absPath, nullptr)) {
        console::error(L"Invalid DLL path: " + dllPath);
        return 1;
    }
    dllPath = absPath;

    // Check if DLL file exists
    if (GetFileAttributesW(dllPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        console::error(L"DLL file not found: " + dllPath);
        return 1;
    }

    // Get process ID
    DWORD processId = 0;

    // Try to parse as PID
    try {
        processId = std::stoul(target);
    } catch (...) {
        // Not a number, search by process name
        processId = GetProcessIdByName(target);
    }

    if (processId == 0) {
        console::error(L"Target process not found: " + target);
        return 1;
    }

    console::info(L"Target process ID: " + std::to_wstring(processId));
    console::info(L"DLL path: " + dllPath);

    bool success = false;

    if (command == L"inject") {
        console::info(L"Starting DLL injection...");
        success = InjectDLL(processId, dllPath);

        if (success) {
            console::success(L"DLL injection successful!");
        }
    }
    else if (command == L"eject") {
        console::info(L"Starting DLL ejection...");
        success = EjectDLL(processId, dllPath);

        if (success) {
            console::success(L"DLL ejection successful!");
        }
    }
    else {
        console::error(L"Unknown command: " + command);
        PrintUsage();
        return 1;
    }

    return success ? 0 : 1;
}
