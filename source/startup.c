/* SPDX-License-Identifier: MIT */
#include <malloc.h>
#include <stdio.h>
#include <sys/file.h>
#include <sysutil/msg.h>
#include <sysutil/sysutil.h>
#include "rsxutil.h"

static volatile int finished=0;
static volatile int exit_requested=0;
int trophy_discovery(void);
static void button(msgButton choice,void *data)
{
    (void)choice;(void)data;finished=1;
}
static void event(u64 status,u64 param,void *data)
{
    (void)param;(void)data;
    if(status==SYSUTIL_EXIT_GAME){finished=1;exit_requested=1;}
}
static int report=-1;
static void stage(const char *text)
{
    if(report>=0){
        u64 written=0;size_t length=0;
        while(text[length])length++;
        sysLv2FsWrite(report,text,length,&written);
        sysLv2FsFsync(report);
    }
}
int main(void)
{
    char name[160];
    for(unsigned i=1;i<=99;i++){
        snprintf(name,sizeof(name),"/dev_hdd0/game/TROPH2026/USRDIR/startup-%03u.txt",i);
        if(!sysLv2FsOpen(name,SYS_O_WRONLY|SYS_O_CREAT|SYS_O_EXCL,&report,0600,NULL,0))break;
        report=-1;
    }
    stage("startup_version=0.0.4\nstage=main_entered\n");
    void *memory=memalign(1024*1024,HOST_SIZE);
    if(!memory){stage("host_memory_failed=1\n");if(report>=0)sysLv2FsClose(report);return 2;}
    stage("stage=before_screen_init\n");
    init_screen(memory,HOST_SIZE);
    stage("stage=screen_initialized\n");
    sysUtilRegisterCallback(SYSUTIL_EVENT_SLOT0,event,NULL);
    int result=msgDialogOpen2((msgType)(MSG_DIALOG_NORMAL|MSG_DIALOG_BTN_TYPE_OK),
        "PS3 Trophy Discovery v0.0.4\n\nPress OK to scan installed trophy folders.\nThis only reads trophy headers.\nNo trophies will be modified.",button,NULL,NULL);
    stage(result?"dialog_open_failed=1\n":"stage=dialog_opened\n");
    for(unsigned frames=0;!finished&&!result&&frames<3600;frames++){
        sysUtilCheckCallback();flip();
    }
    msgDialogAbort();
    if(!result&&!exit_requested&&finished){
        stage("stage=discovery_started\n");
        int discovery=trophy_discovery();
        stage(discovery?"discovery_failed=1\n":"stage=discovery_finished\n");
        char message[320];
        snprintf(message,sizeof(message),
            "PS3 Trophy Discovery v0.0.4\n\nDiscovery return code: %d\n%s\nNo trophies were modified.\n\nPress OK to return to XMB.",
            discovery,discovery?"Read-only discovery needs investigation.":"Report saved; ready for FTP inspection.");
        finished=0;
        result=msgDialogOpen2((msgType)(MSG_DIALOG_NORMAL|MSG_DIALOG_BTN_TYPE_OK),message,button,NULL,NULL);
        for(unsigned frames=0;!finished&&!result&&frames<3600;frames++){
            sysUtilCheckCallback();flip();
        }
        msgDialogAbort();
    }
    stage("stage=returning_to_xmb\n");
    if(report>=0)sysLv2FsClose(report);
    sysUtilUnregisterCallback(SYSUTIL_EVENT_SLOT0);
    return result?3:0;
}

