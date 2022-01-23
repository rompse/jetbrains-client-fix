#include <iostream>
#include <fstream>
#include <filesystem>
#include <MinHook.h>

void writeFile(const std::filesystem::path& path) {
	constexpr auto vmoptions =
			"-Xms16384m\n"
			"-Xmx24576m\n"
			"-XX:ReservedCodeCacheSize=512m\n"
			"-XX:+IgnoreUnrecognizedVMOptions\n"
			"-XX:+UseG1GC\n"
			"-XX:SoftRefLRUPolicyMSPerMB=50\n"
			"-XX:CICompilerCount=2\n"
			"-XX:+HeapDumpOnOutOfMemoryError\n"
			"-XX:-OmitStackTraceInFastThrow\n"
			"-ea\n"
			"-Dsun.io.useCanonCaches=false\n"
			"-Djdk.http.auth.tunneling.disabledSchemes=""\n"
			"-Djdk.attach.allowAttachSelf=true\n"
			"-Djdk.module.illegalAccess.silent=true\n"
			"-Dkotlinx.coroutines.debug=off\n";

	std::ofstream f(path.parent_path() / "jetbrains_client64.exe.vmoptions", std::ofstream::trunc);
	f << vmoptions;
}

decltype(&CreateProcessW) CreateProcessWOriginal;
BOOL
WINAPI
createProcessWHook(
		_In_opt_ LPCWSTR lpApplicationName,
		_Inout_opt_ LPWSTR lpCommandLine,
		_In_opt_ LPSECURITY_ATTRIBUTES lpProcessAttributes,
		_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
		_In_ BOOL bInheritHandles,
		_In_ DWORD dwCreationFlags,
		_In_opt_ LPVOID lpEnvironment,
		_In_opt_ LPCWSTR lpCurrentDirectory,
		_In_ LPSTARTUPINFOW lpStartupInfo,
		_Out_ LPPROCESS_INFORMATION lpProcessInformation
) {
	if (lpApplicationName && std::wstring(lpApplicationName).find(L"jetbrains_client64.exe") != std::wstring::npos)
		writeFile(lpApplicationName);

	return CreateProcessWOriginal(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes,
								  bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo,
								  lpProcessInformation);
}

bool __stdcall DllMain(void* instance, std::uint32_t reason, void*) {
	if (reason != DLL_PROCESS_ATTACH)
		return false;

	if (auto thread = CreateThread(nullptr, 0, [](void* data) -> unsigned long {
		MH_Initialize();
		MH_CreateHook(CreateProcessW, createProcessWHook, (void**)&CreateProcessWOriginal);
		MH_EnableHook(CreateProcessW);
		return 0;
	}, instance, 0, nullptr))
		CloseHandle(thread);

	return true;
}
