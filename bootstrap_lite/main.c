/*
 * Copyright (C) 2021 skgleba
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/devctl.h>
#include <psp2/ctrl.h>
#include <psp2/shellutil.h>
#include <psp2/net/http.h>
#include <psp2/net/net.h>
#include <psp2/sysmodule.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/net/netctl.h>
#include <psp2/io/stat.h>
#include <psp2/io/dirent.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "graphics.h"
#include "Archives.h"
#include "crc32.c"

#define printf psvDebugScreenPrintf

//#define DEBUG

#ifdef DEBUG
#define cprintf sceClibPrintf
#else
#define cprintf(...)
#endif

#define COLORPRINTF(color, ...)                \
	do {                                       \
		psvDebugScreenSetFgColor(color);       \
		printf(__VA_ARGS__);     \
		psvDebugScreenSetFgColor(COLOR_WHITE); \
	} while (0)

#define CHUNK_SIZE 64 * 1024
#define hasEndSlash(path) (path[strlen(path) - 1] == '/')

#include "ops.c" // misc, too clogged otherwise

#define HEN_REPO_URL "http://917hu8k0n73n7.psp2.dev/hen/"
#define VDEP_VPK_FNAME "vdep.vpk"
#define TAIHEN_K_FNAME "taihen.skprx"
#define TAIHEN_C_FNAME "config.txt" // default config.txt
#define HENKAKU_K_FNAME "henkaku.skprx"
#define HENKAKU_U_FNAME "henkaku.suprx"
#define GAMESD_FNAME "gamesd.skprx"
#define TEMP_UX0_PATH "ux0:temp/"
#define TEMP_UR0_PATH "ur0:bgdl/"

#define BOOTSTRAP_VERSION_STR "henlo-bootstrap v1.0.3 by skgleba"

#define OPTION_COUNT 6
enum E_MENU_OPTIONS {
    MENU_EXIT = 0,
    MENU_INSTALL_HENKEK,
    MENU_INSTALL_VDEP,
    MENU_REPLACE_NEAR,
    MENU_RESET_TAICFG,
    MENU_EXIT_W_SD2VITA
};
const char* menu_items[OPTION_COUNT] = { " -> Exit", " -> Install henkaku", " -> Install VitaDeploy", " -> Replace NEAR with VitaDeploy", " -> Reset taihen config.txt", " -> Exit and mount sd2vita to ux0" };

int __attribute__((naked, noinline)) call_syscall(int a1, int a2, int a3, int num) {
    __asm__(
        "mov r12, %0 \n"
        "svc 0 \n"
        "bx lr \n"
        : : "r" (num)
    );
}

int unzip(const char* src, const char* dst) {
    Zip* handle = ZipOpen(src);
    int ret = ZipExtract(handle, NULL, dst);
    ZipClose(handle);
    return ret;
}

int install_vitadeploy_default() {
    sceIoMkdir(TEMP_UX0_PATH, 0777);
    COLORPRINTF(COLOR_CYAN, "Downloading vitadeploy\n");
    net(1);
    int res = download_file(HEN_REPO_URL VDEP_VPK_FNAME, TEMP_UX0_PATH VDEP_VPK_FNAME, TEMP_UX0_PATH VDEP_VPK_FNAME "_tmp", 0);
    if (res < 0) {
        if ((uint32_t)res == 0x80010013)
            printf("Could not open file for write, is ux0 present?\n");
        return res;
    }
    net(0);

    COLORPRINTF(COLOR_CYAN, "Extracting vpk\n");
    removeDir(TEMP_UX0_PATH "app");
    sceIoMkdir(TEMP_UX0_PATH "app", 0777);
    res = unzip(TEMP_UX0_PATH VDEP_VPK_FNAME, TEMP_UX0_PATH "app");
    if (res < 0)
        return res;

    COLORPRINTF(COLOR_CYAN, "Promoting app\n");
    res = promoteApp(TEMP_UX0_PATH "app");
    if (res < 0)
        return res;

    COLORPRINTF(COLOR_GREEN, "All done\n");
    sceKernelDelayThread(2 * 1000 * 1000);

    return 0;
}

int vitadeploy_x_near(int syscall_id) {
    COLORPRINTF(COLOR_WHITE, "This replaces the discontinued \"near\" app with VitaDeploy.\nRequires reboot to take effect.\n\n");
    COLORPRINTF(COLOR_YELLOW, "WARNING: this will cause your bubble layout to be reset\n\n");
    COLORPRINTF(COLOR_CYAN, "SQUARE: Replace NEAR with VitaDeploy\nTRIANGLE: Restore NEAR\nCIRCLE: Go back\n\n");
    sceKernelDelayThread(0.5 * 1000 * 1000);
    SceCtrlData pad;
    while (1) {
        sceCtrlPeekBufferPositive(0, &pad, 1);
        if (pad.buttons == SCE_CTRL_SQUARE)
            break;
        else if (pad.buttons == SCE_CTRL_CIRCLE)
            return 0;
        else if (pad.buttons == SCE_CTRL_TRIANGLE) {
            printf("Preparing to restore NEAR..\n");
            sceIoMkdir(TEMP_UR0_PATH, 0777);
            removeDir(TEMP_UR0_PATH "app");
            sceIoMkdir(TEMP_UR0_PATH "app", 0777);
            int ret = copyDir("vs0:app/NPXS10000/near_backup", TEMP_UR0_PATH "app");
            if (ret < 0) {
                COLORPRINTF(COLOR_RED, "Failed 0x%08X, rebooting in 5s\n", ret);
                goto VXN_REBOOT;
            }
            printf("Remounting vs0 to grw0..\n");
            ret = call_syscall(0, 0, 0, syscall_id + 4);
            if (ret < 0) {
                COLORPRINTF(COLOR_RED, "Failed 0x%08X, rebooting in 5s\n", ret);
                goto VXN_REBOOT;
            }
            printf("Removing VitaDeploy..\n");
            ret = removeDir("grw0:app/NPXS10000");
            if (ret < 0) {
                COLORPRINTF(COLOR_RED, "Failed 0x%08X, rebooting in 5s\n", ret);
                goto VXN_REBOOT;
            }
            printf("Restoring NEAR..\n");
            ret = copyDir(TEMP_UR0_PATH "app", "grw0:app/NPXS10000");
            if (ret < 0) {
                COLORPRINTF(COLOR_RED, "Failed 0x%08X, rebooting in 5s\n", ret);
                goto VXN_REBOOT;
            }
            printf("Removing the application database\n");
            sceIoRemove("ur0:shell/db/app.db");
            printf("Cleaning up..\n");
            removeDir(TEMP_UR0_PATH "app");
            printf("All done, rebooting in 5s\n");
            goto VXN_REBOOT;
        }
    }
    sceIoMkdir(TEMP_UR0_PATH, 0777);
    printf("Downloading vitadeploy\n");
    net(1);
    int res = download_file(HEN_REPO_URL VDEP_VPK_FNAME, TEMP_UR0_PATH VDEP_VPK_FNAME, TEMP_UR0_PATH VDEP_VPK_FNAME "_tmp", 0);
    if (res < 0) {
        COLORPRINTF(COLOR_RED, "Failed 0x%08X, rebooting in 5s\n", res);
        goto VXN_REBOOT;
    }
    net(0);

    printf("Extracting vpk\n");
    removeDir(TEMP_UR0_PATH "app");
    sceIoMkdir(TEMP_UR0_PATH "app", 0777);
    res = unzip(TEMP_UR0_PATH VDEP_VPK_FNAME, TEMP_UR0_PATH "app");
    if (res < 0) {
        COLORPRINTF(COLOR_RED, "Failed 0x%08X, rebooting in 5s\n", res);
        goto VXN_REBOOT;
    }

    printf("Backing up NEAR\n");
    res = copyDir("vs0:app/NPXS10000", TEMP_UR0_PATH "app/near_backup");
    if (res < 0) {
        COLORPRINTF(COLOR_RED, "Failed 0x%08X, rebooting in 5s\n", res);
        goto VXN_REBOOT;
    }

    printf("Preparing to replace near..\n");
    sceIoRemove(TEMP_UR0_PATH "app/sce_sys/param.sfo");
    sceIoRename(TEMP_UR0_PATH "app/sce_sys/vs.sfo", TEMP_UR0_PATH "app/sce_sys/param.sfo");

    printf("Remounting vs0 to grw0..\n");
    res = call_syscall(0, 0, 0, syscall_id + 4);
    if (res < 0) {
        COLORPRINTF(COLOR_RED, "Failed 0x%08X, rebooting in 5s\n", res);
        goto VXN_REBOOT;
    }

    printf("Replacing NEAR\n");
    res = removeDir("grw0:app/NPXS10000");
    cprintf("remove near : 0x%08X\n", res);

    res = copyDir(TEMP_UR0_PATH "app", "grw0:app/NPXS10000");
    if (res < 0) {
        COLORPRINTF(COLOR_RED, "Failed 0x%08X, rebooting in 5s\n", res);
        goto VXN_REBOOT;
    }

    printf("Removing the application database\n");
    sceIoRemove("ur0:shell/db/app.db");

    printf("Cleaning up..\n");
    removeDir(TEMP_UR0_PATH "app");

    printf("All done, rebooting in 5s\n");
VXN_REBOOT:
    sceKernelDelayThread(5 * 1000 * 1000);
    scePowerRequestColdReset();
    sceKernelDelayThread(2 * 1000 * 1000);
    printf("Process is dead\n");
    while (1) {};
}

int install_henkaku(void) {
    COLORPRINTF(COLOR_CYAN, "Preparing ur0:tai/\n");
    sceIoMkdir("ur0:tai", 0777);
    int fd = sceIoOpen("ux0:tai/config.txt", SCE_O_RDONLY, 0);
    if (fd >= 0) {
        sceIoClose(fd);
        fcp("ux0:tai/config.txt", "ux0:tai/config.txt_old");
        sceIoRemove("ux0:tai/config.txt");
    }
    int already_taid = 0;
    fd = sceIoOpen("ur0:tai/config.txt", SCE_O_RDONLY, 0);
    if (fd >= 0) {
        sceIoClose(fd);
        already_taid = 1;
    }
    COLORPRINTF(COLOR_CYAN, "Downloading the latest taihen build\n");
    int ret = download_file(HEN_REPO_URL TAIHEN_K_FNAME, "ur0:tai/" TAIHEN_K_FNAME, "ur0:tai/" TAIHEN_K_FNAME "_tmp", 0);
    if (ret < 0)
        return ret;
    COLORPRINTF(COLOR_CYAN, "Downloading the latest henkaku builds\n");
    ret = download_file(HEN_REPO_URL HENKAKU_K_FNAME, "ur0:tai/" HENKAKU_K_FNAME, "ur0:tai/" HENKAKU_K_FNAME "_tmp", 0);
    if (ret < 0)
        return ret;
    ret = download_file(HEN_REPO_URL HENKAKU_U_FNAME, "ur0:tai/" HENKAKU_U_FNAME, "ur0:tai/" HENKAKU_U_FNAME "_tmp", 0);
    if (ret < 0)
        return ret;
    if (!already_taid) {
        COLORPRINTF(COLOR_CYAN, "Downloading the default taihen config\n");
        ret = download_file(HEN_REPO_URL TAIHEN_C_FNAME, "ur0:tai/" TAIHEN_C_FNAME, "ur0:tai/" TAIHEN_C_FNAME "_tmp", 0);
        if (ret < 0)
            return ret;
    }
    COLORPRINTF(COLOR_GREEN, "All done\n\n");
    COLORPRINTF(COLOR_PURPLE, "Using the \"Exit\" will launch henkaku\n");
    sceKernelDelayThread(2 * 1000 * 1000);
    return 0;
}

void main_menu(int sel) {
    psvDebugScreenClear(COLOR_BLACK);
    COLORPRINTF(COLOR_YELLOW, BOOTSTRAP_VERSION_STR "\n");
    COLORPRINTF(COLOR_WHITE, "\n---------------------------------------------\n\n");
    for (int i = 0; i < OPTION_COUNT; i++) {
        if (sel == i)
            psvDebugScreenSetFgColor(COLOR_CYAN);
        printf("%s\n", menu_items[i]);
        psvDebugScreenSetFgColor(COLOR_WHITE);
    }
    psvDebugScreenSetFgColor(COLOR_WHITE);
    COLORPRINTF(COLOR_WHITE, "\n---------------------------------------------\n\n");
}

int _start(SceSize args, void* argp) {
    int syscall_id = *(uint16_t*)argp;

    cprintf("preinit\n");
    sceAppMgrDestroyOtherApp();
    sceShellUtilInitEvents(0);
    sceShellUtilLock(1);

    cprintf("loading PAF\n");
    int res = load_sce_paf();
    if (res < 0)
        goto EXIT;

    psvDebugScreenInit();
    COLORPRINTF(COLOR_CYAN, BOOTSTRAP_VERSION_STR "\n");

    int sel = 0, launch_gamesd = 0;
    SceCtrlData pad;
    main_menu(sel);
    while (1) {
        sceCtrlPeekBufferPositive(0, &pad, 1);
        if (pad.buttons == SCE_CTRL_CROSS) {
            res = -1;
            if (sel == MENU_EXIT) {
                res = 0;
                goto EXIT;
            } else if (sel == MENU_INSTALL_VDEP) {
                res = install_vitadeploy_default();
                if (res < 0) {
                    COLORPRINTF(COLOR_RED, "\nFAILED: 0x%08X\n", res);
                    sceKernelDelayThread(3 * 1000 * 1000);
                }
                sel = 0;
                main_menu(sel);
                sceKernelDelayThread(0.3 * 1000 * 1000);
            } else if (sel == MENU_REPLACE_NEAR) {
                res = vitadeploy_x_near(syscall_id);
                if (res < 0) {
                    COLORPRINTF(COLOR_RED, "\nFAILED: 0x%08X\n", res);
                    sceKernelDelayThread(3 * 1000 * 1000);
                }
                sel = 0;
                main_menu(sel);
                sceKernelDelayThread(0.3 * 1000 * 1000);
            } else if (sel == MENU_INSTALL_HENKEK) {
                net(1);
                res = install_henkaku();
                net(0);
                if (res < 0) {
                    COLORPRINTF(COLOR_RED, "\nFAILED: 0x%08X\n", res);
                    sceKernelDelayThread(3 * 1000 * 1000);
                }
                sel = 0;
                main_menu(sel);
                sceKernelDelayThread(0.3 * 1000 * 1000);
            } else if (sel == MENU_RESET_TAICFG) {
                COLORPRINTF(COLOR_CYAN, "Downloading the default taihen config\n");
                sceIoMkdir("ur0:tai", 0777);
                net(1);
                res = download_file(HEN_REPO_URL TAIHEN_C_FNAME, "ur0:tai/" TAIHEN_C_FNAME, "ur0:tai/" TAIHEN_C_FNAME "_tmp", 0);
                net(0);
                if (res < 0) {
                    COLORPRINTF(COLOR_RED, "\nFAILED: 0x%08X\n", res);
                    sceKernelDelayThread(3 * 1000 * 1000);
                } else {
                    COLORPRINTF(COLOR_GREEN, "All done\n");
                    sceKernelDelayThread(2 * 1000 * 1000);
                }
                sel = 0;
                main_menu(sel);
                sceKernelDelayThread(0.3 * 1000 * 1000);
            } else if (sel == MENU_EXIT_W_SD2VITA) {
                COLORPRINTF(COLOR_CYAN, "Downloading the gamesd plugin\n");
                sceIoMkdir("ur0:tai", 0777);
                net(1);
                res = download_file(HEN_REPO_URL GAMESD_FNAME, "ur0:tai/" GAMESD_FNAME, "ur0:tai/" GAMESD_FNAME "_tmp", 0);
                net(0);
                if (res < 0) {
                    COLORPRINTF(COLOR_RED, "\nFAILED: 0x%08X\n", res);
                    sceKernelDelayThread(3 * 1000 * 1000);
                } else {
                    COLORPRINTF(COLOR_GREEN, "Done, will launch at exit\n");
                    launch_gamesd = 1;
                    res = 0;
                    goto EXIT;
                }
                sel = 0;
                main_menu(sel);
                sceKernelDelayThread(0.3 * 1000 * 1000);
            }
        } else if (pad.buttons == SCE_CTRL_UP) {
            if (sel != 0)
                sel--;
            main_menu(sel);
            sceKernelDelayThread(0.3 * 1000 * 1000);
        } else if (pad.buttons == SCE_CTRL_DOWN) {
            if (sel + 1 < OPTION_COUNT)
                sel++;
            main_menu(sel);
            sceKernelDelayThread(0.3 * 1000 * 1000);
        }
    }

    printf("All done!\n");

EXIT:
    cprintf("EXIT with res 0x%08X\n", res);
    printf("Exiting in 3\n");
    sceKernelDelayThread(3 * 1000 * 1000);
    
    // Remove pkg patches
    cprintf("Remove pkg patches.. \n");
    res = call_syscall(0, 0, 0, syscall_id + 1);
    if (res >= 0) {
        // Start HENkaku
        cprintf("Henkkek.. \n");
        printf("\nStarting the taihen framework...\n\n\nIf you are stuck on this screen:\n\n - Force reboot by holding the power button\n\n - Launch the exploit again\n\n - Hold Left Trigger [LT] while exiting\n\n\n\nIf the issue persists reset the taihen config.txt using the bootstrap menu\n");
        res = call_syscall(0, 0, 0, syscall_id + 0);
        psvDebugScreenClear(COLOR_BLACK);
    } else {
        // Remove sig patches
        cprintf("Remove sig patches\n");
        call_syscall(0, 0, 0, syscall_id + 2);
    }

    if (res < 0 && res != 0x8002D013 && res != 0x8002D017) {
        COLORPRINTF(COLOR_YELLOW, BOOTSTRAP_VERSION_STR "\n");
        COLORPRINTF(COLOR_WHITE, "\n---------------------------------------------\n\n");
        printf(" > Failed to start taihen! 0x%08X\n", res);
        printf(" > Please relaunch the exploit and select 'Install HENkaku'.\n");
    }

    if (launch_gamesd) {
        COLORPRINTF(COLOR_YELLOW, BOOTSTRAP_VERSION_STR "\n");
        COLORPRINTF(COLOR_WHITE, "\n---------------------------------------------\n\n");
        printf("Launching gamesd... ");
        res = call_syscall(0, 0, 0, syscall_id + 5);
        if (res < 0) {
            COLORPRINTF(COLOR_RED, "FAILED: 0x%08X\n", res);
            sceKernelDelayThread(3 * 1000 * 1000);
        } else
            COLORPRINTF(COLOR_GREEN, "OK\n");
        sceKernelDelayThread(3 * 1000 * 1000);
        printf("Exiting\n");
    }

    // Clean up
    cprintf("Cleanup.. \n");
    call_syscall(0, 0, 0, syscall_id + 3);

    cprintf("all done, exit\n");

    cprintf("unloading PAF\n");
    unload_sce_paf();

    sceShellUtilUnlock(1);

    sceKernelExitProcess(0);
    return 0;
}