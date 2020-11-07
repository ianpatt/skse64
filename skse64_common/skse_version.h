#ifndef __SKSE_VERSION_H__
#define __SKSE_VERSION_H__

// these have to be macros so they can be used in the .rc
#define SKSE_VERSION_INTEGER		2
#define SKSE_VERSION_INTEGER_MINOR	0
#define SKSE_VERSION_INTEGER_BETA	19
#define SKSE_VERSION_VERSTRING		"0, 2, 0, 19"
#define SKSE_VERSION_PADDEDSTRING	"0001"
#define SKSE_VERSION_RELEASEIDX		65

#define MAKE_EXE_VERSION_EX(major, minor, build, sub)	((((major) & 0xFF) << 24) | (((minor) & 0xFF) << 16) | (((build) & 0xFFF) << 4) | ((sub) & 0xF))
#define MAKE_EXE_VERSION(major, minor, build)			MAKE_EXE_VERSION_EX(major, minor, build, 0)

#define GET_EXE_VERSION_MAJOR(a)	(((a) & 0xFF000000) >> 24)
#define GET_EXE_VERSION_MINOR(a)	(((a) & 0x00FF0000) >> 16)
#define GET_EXE_VERSION_BUILD(a)	(((a) & 0x0000FFF0) >> 4)
#define GET_EXE_VERSION_SUB(a)		(((a) & 0x0000000F) >> 0)

#define RUNTIME_VERSION_1_1_47	MAKE_EXE_VERSION(1, 1, 47)	// 0x010102F0	initial version released on steam, has edit-and-continue enabled
#define RUNTIME_VERSION_1_1_51	MAKE_EXE_VERSION(1, 1, 51)	// 0x01010330	initial version released on steam, has edit-and-continue enabled
#define RUNTIME_VERSION_1_2_36	MAKE_EXE_VERSION(1, 2, 36)	// 0x01020240	edit-and-continue disabled
#define RUNTIME_VERSION_1_2_39	MAKE_EXE_VERSION(1, 2, 39)	// 0x01020270	edit-and-continue disabled
#define RUNTIME_VERSION_1_3_5	MAKE_EXE_VERSION(1, 3, 5)	// 0x01030050	
#define RUNTIME_VERSION_1_3_9	MAKE_EXE_VERSION(1, 3, 9)	// 0x01030090	
#define RUNTIME_VERSION_1_4_2	MAKE_EXE_VERSION(1, 4, 2)	// 0x01040020	
#define RUNTIME_VERSION_1_5_3	MAKE_EXE_VERSION(1, 5, 3)	// 0x01050030	creation club
#define RUNTIME_VERSION_1_5_16	MAKE_EXE_VERSION(1, 5, 16)	// 0x01050100	creation club cleanup (thanks)
#define RUNTIME_VERSION_1_5_23	MAKE_EXE_VERSION(1, 5, 23)	// 0x01050170	creation club
#define RUNTIME_VERSION_1_5_39	MAKE_EXE_VERSION(1, 5, 39)	// 0x01050270	creation club
#define RUNTIME_VERSION_1_5_50	MAKE_EXE_VERSION(1, 5, 50)	// 0x01050320	creation club
#define RUNTIME_VERSION_1_5_53	MAKE_EXE_VERSION(1, 5, 53)	// 0x01050350	creation club
#define RUNTIME_VERSION_1_5_62	MAKE_EXE_VERSION(1, 5, 62)	// 0x010503E0	creation club
#define RUNTIME_VERSION_1_5_73	MAKE_EXE_VERSION(1, 5, 73)	// 0x01050490	creation club
#define RUNTIME_VERSION_1_5_80	MAKE_EXE_VERSION(1, 5, 80)	// 0x01050500	creation club - no code or data differences
#define RUNTIME_VERSION_1_5_97	MAKE_EXE_VERSION(1, 5, 97)	// 0x01050610	creation club

#define PACKED_SKSE_VERSION		MAKE_EXE_VERSION(SKSE_VERSION_INTEGER, SKSE_VERSION_INTEGER_MINOR, SKSE_VERSION_INTEGER_BETA)

// information about the state of the game at the time of release
#define SKSE_TARGETING_BETA_VERSION	0
#define CURRENT_RELEASE_RUNTIME		RUNTIME_VERSION_1_5_97
#define CURRENT_RELEASE_SKSE_STR	"2.0.19"

#endif /* __SKSE_VERSION_H__ */
