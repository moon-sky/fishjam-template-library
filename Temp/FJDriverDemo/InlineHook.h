#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    #define MAX_REPLACE_CODE_SIZE       0x40

    typedef struct _INLINE_HOOK_INFO
    {
        PBYTE   pTarget;
        PBYTE   pDetour;
        PBYTE   pOriginal;

        UCHAR   targetBackup[MAX_REPLACE_CODE_SIZE];
        ULONG   targetBackupSize;

        UCHAR   trampoline[MAX_REPLACE_CODE_SIZE];
        ULONG   trampolineSize;

    }INLINE_HOOK_INFO, *PINLINE_HOOK_INFO;

    //struct _INLINE_HOOK_INFO;
    //typedef _INLINE_HOOK_INFO* PINLINE_HOOK_INFO;

    BOOL CreateInlineHook(PVOID pTarget, PVOID const pDetour, PVOID* ppOriginal, PINLINE_HOOK_INFO* ppOutHookInfo);
    BOOL RestoreInlineHook(PINLINE_HOOK_INFO pHookInfo);
    //NTSTATUS EnableInlineHook(PVOID pTarget, PINLINE_HOOK_INFO pHookInfo);
    //NTSTATUS DisableInlineHook(PVOID pTarget, PINLINE_HOOK_INFO pHookInfo);


#ifdef __cplusplus
}
#endif