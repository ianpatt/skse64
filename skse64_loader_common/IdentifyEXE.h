#pragma once

enum
{
	kProcType_Steam,
	kProcType_Normal,
	kProcType_WinStore,
	kProcType_GOG,
	kProcType_Epic,

	kProcType_Packed,

	kProcType_Unknown
};

struct ProcHookInfo
{
	UInt64	version;
	UInt32	procType;
	UInt32	packedVersion;
};

bool IdentifyEXE(const char * procName, bool isEditor, std::string * dllSuffix, ProcHookInfo * hookInfo);
bool GetFileVersion(const char * path, VS_FIXEDFILEINFO * info, std::string * outProductName, std::string * outProductVersion);
void DumpVersionInfo(const VS_FIXEDFILEINFO & info);
