#pragma once

//#define SYNCIFY

#ifdef SYNCIFY

	void	*operator new(size_t reportedSize);
	void	*operator new[](size_t reportedSize);
	void	operator delete(void *reportedAddress);
	void	operator delete[](void *reportedAddress);

	namespace Syncify {
		void InitSyncify();
		void EndSyncify();
		void EnterSyncedMode();
		void EnterUnsyncedMode();
		void EnterMixedMode();
		void PushCodeMode();
		void PopCodeMode();
		void SetCodeModeToMem(void* mem);
		void AssertSyncedMode(char* file,int line);
		void AssertUnsyncedMode(char* file,int line);
		void AssertMixedMode(char* file,int line);
	}

	#define INIT_SYNCIFY Syncify::InitSyncify();
	#define END_SYNCIFY Syncify::EndSyncify();
	#define ENTER_SYNCED	Syncify::EnterSyncedMode();
	#define ENTER_UNSYNCED	Syncify::EnterUnsyncedMode();
	#define ENTER_MIXED	Syncify::EnterMixedMode();
	#define PUSH_CODE_MODE	Syncify::PushCodeMode();
	#define POP_CODE_MODE Syncify::PopCodeMode();
	#define SET_CODE_MODE_TO_MEM(pointer)	Syncify::SetCodeModeToMem(pointer);
	#define ASSERT_SYNCED_MODE Syncify::AssertSyncedMode(__FILE__,__LINE__);
	#define ASSERT_UNSYNCED_MODE Syncify::AssertUnsyncedMode(__FILE__,__LINE__);
	#define ASSERT_MIXED_MODE Syncify::AssertMixedMode(__FILE__,__LINE__);

#else

	#define INIT_SYNCIFY
	#define END_SYNCIFY
	#define ENTER_SYNCED
	#define ENTER_UNSYNCED
	#define ENTER_MIXED
	#define POP_CODE_MODE
	#define PUSH_CODE_MODE
	#define SET_CODE_MODE_TO_MEM(pointer)
	#define ASSERT_SYNCED_MODE
	#define ASSERT_UNSYNCED_MODE
	#define ASSERT_MIXED_MODE
#endif