#include "GameResources.h"

FaceGenTriDatabase * FaceGenTriDatabase::GetSingleton(void)
{
	// 515F6D3421B9B5634E8CFA419159BB297C282A3B+3E
	static RelocPtr<FaceGenTriDatabase*> g_FaceGenTriDatabase(0x02F017E8);
	return *g_FaceGenTriDatabase;
}

BSFaceGenDB::TRI::DBTraits::Data * FaceGenTriDatabase::GetInvalidEntry(void)
{
	// 836BA46E0B9EB46A38D8BAD8A4A7D14584D61BB0+8E
	static RelocPtr<BSFaceGenDB::TRI::DBTraits::Data> g_FaceGenTriDatabase(0x01DEA01C);
	return g_FaceGenTriDatabase.GetPtr();
}
