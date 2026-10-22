/* SPDX-License-Identifier: MIT
 * Read-only trophy discovery probe. This is not an unlock implementation.
 * Writes only a new report inside this application's own USRDIR.
 */
#include <sys/file.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define REPORT_ROOT "/dev_hdd0/game/TROPH2026/USRDIR"
#define MAX_USERS 32
#define MAX_SETS 1024
static int report_fd = -1;
static int report_failed = 0;

static void line(const char *format, ...)
{
    char buffer[768]; u64 written = 0; va_list args;
    va_start(args, format);int n = vsnprintf(buffer, sizeof(buffer), format, args);va_end(args);
    if (n < 0 || (size_t)n >= sizeof(buffer) || sysLv2FsWrite(report_fd, buffer, (u64)n, &written) || written != (u64)n)
        report_failed = 1;
}

static int digits(const char *s, size_t count)
{
    if (strlen(s) != count)return 0;
    for (size_t i=0;i<count;i++)if (s[i]<'0'||s[i]>'9')return 0;
    return 1;
}

static int npwr(const char *s)
{
    if (strlen(s)!=12 || strncmp(s,"NPWR",4) || s[9]!='_')return 0;
    for(int i=4;i<12;i++)if(i!=9&&(s[i]<'0'||s[i]>'9'))return 0;
    return 1;
}

static uint32_t be32(const unsigned char *p)
{
    return ((uint32_t)p[0]<<24)|((uint32_t)p[1]<<16)|((uint32_t)p[2]<<8)|p[3];
}

static void inspect_header(const char *root, const char *file)
{
    char path[512];unsigned char header[48];sysFSStat st;int fd;u64 got=0;
    if(snprintf(path,sizeof(path),"%s/%s",root,file)>=(int)sizeof(path))return;
    int error=sysLv2FsStat(path,&st);
    if(error){line("  %s stat_error=0x%08x\n",file,(unsigned)error);return;}
    error=sysLv2FsOpen(path,SYS_O_RDONLY,&fd,0,NULL,0);
    if(error){line("  %s open_error=0x%08x\n",file,(unsigned)error);return;}
    error=sysLv2FsRead(fd,header,sizeof(header),&got);sysLv2FsClose(fd);
    if(error||got!=sizeof(header)){line("  %s short_header size=%llu error=0x%08x\n",file,(unsigned long long)st.st_size,(unsigned)error);return;}
    int decoded=be32(header)==0x818f54ad && be32(header+4)==0x00010000;
    line("  %s size=%llu decoded_header=%d",file,(unsigned long long)st.st_size,decoded);
    if(decoded)line(" table_count=%u",(unsigned)be32(header+8));
    line("\n");
}

int main(void)
{
    char path[512];int home_fd;sysFSDirent user;u64 read=0;unsigned users=0,sets=0;
    for(unsigned index=1;index<=999;index++){
        snprintf(path,sizeof(path),REPORT_ROOT "/probe-%03u.txt",index);
        int candidate=-1;
        int result=sysLv2FsOpen(path,SYS_O_WRONLY|SYS_O_CREAT|SYS_O_EXCL,&candidate,0600,NULL,0);
        if(!result){report_fd=candidate;break;}
        sysFSStat st;if(sysLv2FsStat(path,&st))return 2;
    }
    if(report_fd<0)return 2;
    line("PS3 Trophy Unlocker 2026: READ-ONLY discovery probe v0.0.1\nNo trophy writes, syscall8, account/console identifiers or PSN calls.\n");
    int error=sysLv2FsOpenDir("/dev_hdd0/home",&home_fd);
    if(error){line("home_open_error=0x%08x\n",(unsigned)error);report_failed=1;goto finish;}
    unsigned home_entries=0;
    while(users<MAX_USERS&&sets<MAX_SETS&&home_entries++<16384){
        error=sysLv2FsReadDir(home_fd,&user,&read);
        if(error){line("home_read_error=0x%08x\n",(unsigned)error);report_failed=1;break;}
        if(!read)break;
        user.d_name[sizeof(user.d_name)-1]=0;
        if(!digits(user.d_name,8))continue;
        users++;
        snprintf(path,sizeof(path),"/dev_hdd0/home/%.8s/trophy",user.d_name);
        int trophy_fd;sysFSDirent set;
        error=sysLv2FsOpenDir(path,&trophy_fd);
        if(error){line("user=%s trophy_open_error=0x%08x\n",user.d_name,(unsigned)error);continue;}
        unsigned set_entries=0;
        while(sets<MAX_SETS&&set_entries++<16384){
            error=sysLv2FsReadDir(trophy_fd,&set,&read);
            if(error){line("trophy_read_error=0x%08x\n",(unsigned)error);report_failed=1;break;}
            if(!read)break;
            set.d_name[sizeof(set.d_name)-1]=0;
            if(!npwr(set.d_name))continue;
            sets++;line("user=%s set=%s\n",user.d_name,set.d_name);
            snprintf(path,sizeof(path),"/dev_hdd0/home/%.8s/trophy/%.12s",user.d_name,set.d_name);
            inspect_header(path,"TROPUSR.DAT");inspect_header(path,"TROPTRNS.DAT");
        }
        if(set_entries>16384)line("user_directory_limit_reached=1\n");
        sysLv2FsCloseDir(trophy_fd);
    }
    sysLv2FsCloseDir(home_fd);
    line("users=%u sets=%u limit_reached=%d\n",users,sets,users==MAX_USERS||sets==MAX_SETS||home_entries>16384);
finish:
    line("report_complete=%d\n",!report_failed);
    if(sysLv2FsFsync(report_fd))report_failed=1;
    sysLv2FsClose(report_fd);
    return report_failed?3:0;
}
