#include "GameResources.h"

FaceGenTriDatabase * FaceGenTriDatabase::GetSingleton(void)
{
	// 9FA4657D28964C48501C6FF2E9E8E52D86D2BDEF+3E
	static RelocPtr<FaceGenTriDatabase*> g_FaceGenTriDatabase(0x02F9BF00);
	return *g_FaceGenTriDatabase;
}

BSFaceGenDB::TRI::DBTraits::Data * FaceGenTriDatabase::GetInvalidEntry(void)
{
	// CD3B953DD742EFE4CEFC4ECA602FC9F442434777+A2
	static RelocPtr<BSFaceGenDB::TRI::DBTraits::Data> g_FaceGenTriDatabase_InvalidEntry(0x01E81C58);
	return g_FaceGenTriDatabase_InvalidEntry.GetPtr();
}
