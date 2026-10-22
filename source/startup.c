/* SPDX-License-Identifier: MIT */
#include <malloc.h>
#include <stdio.h>
#include <sys/file.h>
#include <sysutil/msg.h>
#include <sysutil/sysutil.h>
#include "rsxutil.h"

static volatile int finished=0;
static volatile int exit_requested=0;
static volatile int dialog_choice=MSG_DIALOG_BTN_NONE;
int trophy_discovery(void);
unsigned trophy_set_count(void);
int trophy_set_label(unsigned index,char *buffer,size_t size);
int trophy_set_details(unsigned index,char *buffer,size_t size);
static void button(msgButton choice,void *data)
{
    (void)data;dialog_choice=choice;finished=1;
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
static int show_dialog(msgType type,const char *message)
{
    finished=0;dialog_choice=MSG_DIALOG_BTN_NONE;
    int result=msgDialogOpen2(type,message,button,NULL,NULL);
    if(result){stage("menu_dialog_failed=1\n");return MSG_DIALOG_BTN_ESCAPE;}
    for(unsigned frames=0;!finished&&!exit_requested&&frames<18000;frames++){
        sysUtilCheckCallback();flip();
    }
    msgDialogAbort();
    /* Let the system process dialog teardown before opening the next view. */
    for(unsigned frame=0;frame<4;frame++){sysUtilCheckCallback();flip();}
    return finished?dialog_choice:MSG_DIALOG_BTN_ESCAPE;
}
static void browse_sets(void)
{
    unsigned count=trophy_set_count(),selected=0;
    if(!count){show_dialog((msgType)(MSG_DIALOG_NORMAL|MSG_DIALOG_BTN_TYPE_OK),"No installed trophy sets were discovered.");return;}
    while(!exit_requested){
        char label[80],message[512];
        if(trophy_set_label(selected,label,sizeof(label)))return;
        snprintf(message,sizeof(message),
            "PS3 Trophy Browser v0.0.5\n\nSet %u of %u\n%s\n\nYES: inspect this set\nNO: next set (wraps to first)\nCircle / Cancel: return to XMB\n\nRead-only; no trophy changes.",selected+1,count,label);
        int choice=show_dialog((msgType)(MSG_DIALOG_NORMAL|MSG_DIALOG_BTN_TYPE_YESNO),message);
        if(exit_requested)return;
        if(choice==MSG_DIALOG_BTN_NO){selected=(selected+1)%count;continue;}
        if(choice!=MSG_DIALOG_BTN_YES)return;
        char details[256];
        int result=trophy_set_details(selected,details,sizeof(details));
        stage(result?"selected_set_read_failed=1\n":"stage=selected_set_header_read\n");
        snprintf(message,sizeof(message),"%s\n\n%s\n\nOK: back to set selection",label,details);
        show_dialog((msgType)(MSG_DIALOG_NORMAL|MSG_DIALOG_BTN_TYPE_OK),message);
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
    stage("startup_version=0.0.5\nstage=main_entered\n");
    void *memory=memalign(1024*1024,HOST_SIZE);
    if(!memory){stage("host_memory_failed=1\n");if(report>=0)sysLv2FsClose(report);return 2;}
    stage("stage=before_screen_init\n");
    init_screen(memory,HOST_SIZE);
    stage("stage=screen_initialized\n");
    sysUtilRegisterCallback(SYSUTIL_EVENT_SLOT0,event,NULL);
    int result=msgDialogOpen2((msgType)(MSG_DIALOG_NORMAL|MSG_DIALOG_BTN_TYPE_OK),
        "PS3 Trophy Browser v0.0.5\n\nPress OK to discover installed trophy sets\nand open the read-only set selector.\nNo trophies will be modified.",button,NULL,NULL);
    stage(result?"dialog_open_failed=1\n":"stage=dialog_opened\n");
    for(unsigned frames=0;!finished&&!result&&frames<3600;frames++){
        sysUtilCheckCallback();flip();
    }
    msgDialogAbort();
    for(unsigned frame=0;frame<4;frame++){sysUtilCheckCallback();flip();}
    if(!result&&!exit_requested&&finished&&dialog_choice==MSG_DIALOG_BTN_OK){
        stage("stage=discovery_started\n");
        int discovery=trophy_discovery();
        stage(discovery?"discovery_failed=1\n":"stage=discovery_finished\n");
        if(!discovery)browse_sets();
        else show_dialog((msgType)(MSG_DIALOG_NORMAL|MSG_DIALOG_BTN_TYPE_OK),"Discovery reported an error. The report can be inspected over FTP.");
    }
    stage("stage=returning_to_xmb\n");
    if(report>=0)sysLv2FsClose(report);
    sysUtilUnregisterCallback(SYSUTIL_EVENT_SLOT0);
    return result?3:0;
}

