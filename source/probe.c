/* SPDX-License-Identifier: MIT
 * Read-only trophy discovery probe. This is not an unlock implementation.
 * Writes only new diagnostics in app-owned locations or an attached USB root.
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
static struct { char user[9];char set[13]; } catalog[MAX_SETS];
static unsigned catalog_count=0;

unsigned trophy_set_count(void){return catalog_count;}

int trophy_set_label(unsigned index,char *buffer,size_t size)
{
    if(index>=catalog_count)return -1;
    int length=snprintf(buffer,size,"User %s / %s",catalog[index].user,catalog[index].set);
    return length<0||(size_t)length>=size?-1:0;
}

int trophy_set_details(unsigned index,char *buffer,size_t size)
{
    if(index>=catalog_count)return -1;
    char path[256];unsigned char header[48];u64 got=0;int fd;
    snprintf(path,sizeof(path),"/dev_hdd0/home/%s/trophy/%s/TROPUSR.DAT",catalog[index].user,catalog[index].set);
    int result=sysLv2FsOpen(path,SYS_O_RDONLY,&fd,0,NULL,0);
    if(result){snprintf(buffer,size,"TROPUSR open error: 0x%08x",(unsigned)result);return -1;}
    result=sysLv2FsRead(fd,header,sizeof(header),&got);sysLv2FsClose(fd);
    if(result||got!=sizeof(header)){
        snprintf(buffer,size,"TROPUSR read error: 0x%08x / %llu bytes",(unsigned)result,(unsigned long long)got);return -1;
    }
    const unsigned char magic[8]={0x81,0x8f,0x54,0xad,0,1,0,0};
    if(memcmp(header,magic,8)){
        snprintf(buffer,size,"TROPUSR header is unsupported.");return -1;
    }
    unsigned tables=((unsigned)header[8]<<24)|((unsigned)header[9]<<16)|((unsigned)header[10]<<8)|header[11];
    snprintf(buffer,size,"TROPUSR header recognized\nTable count: %u\n\nHeader inspection only. Trophy states and\nprotection metadata are not yet validated.",tables);
    return 0;
}

static int begin_report(void)
{
    const char *roots[] = { REPORT_ROOT, "/dev_hdd0/tmp/troph2026", "/dev_usb000", "/dev_usb001" };
    int errors[4] = {0,0,0,0};
    char path[512];
    /* The temporary fallback is app-specific. Never alter trophy folders. */
    sysLv2FsMkdir("/dev_hdd0/tmp/troph2026",0700);
    for(unsigned root=0;root<4;root++){
        for(unsigned index=1;index<=999;index++){
            snprintf(path,sizeof(path),"%s/trophy-probe-diag-%03u.txt",roots[root],index);
            int candidate=-1;
            int result=sysLv2FsOpen(path,SYS_O_WRONLY|SYS_O_CREAT|SYS_O_EXCL,&candidate,0600,NULL,0);
            if(!result){
                static const char marker[]="stage=main_entered\nprobe_version=0.0.15-state-check\n";
                u64 written=0;
                report_fd=candidate;
                if(sysLv2FsWrite(report_fd,marker,sizeof(marker)-1,&written)||written!=sizeof(marker)-1||sysLv2FsFsync(report_fd))
                    report_failed=1;
                for(unsigned prior=0;prior<root;prior++){
                    char message[96];
                    int length=snprintf(message,sizeof(message),"report_root_%u_open_error=0x%08x\n",prior,(unsigned)errors[prior]);
                    if(length<0||(size_t)length>=sizeof(message)||sysLv2FsWrite(report_fd,message,(u64)length,&written)||written!=(u64)length)
                        report_failed=1;
                }
                return 0;
            }
            errors[root]=result;
            sysFSStat info;
            if(sysLv2FsStat(path,&info))break;
        }
    }
    return -1;
}

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

/* Read-only fingerprint for validating unchanged trophy data across runs.
 * This does NOT decode an individual trophy's unlock state.
 */
static void target_state_integrity(const char *root)
{
    char path[512];
    int fd=-1, err;
    u64 got=0,total=0;
    uint64_t hash=UINT64_C(14695981039346656037);
    unsigned char buffer[4096];
    sysFSStat st;
    if(snprintf(path,sizeof(path),"%s/TROPUSR.DAT",root)>=(int)sizeof(path))return;
    err=sysLv2FsStat(path,&st);
    if(err){line("  state_integrity_stat_error=0x%08x\n",(unsigned)err);return;}
    /* Bound the read even if the file is unexpectedly large. */
    if(st.st_size > 4*1024*1024ULL){
        line("  state_integrity=SKIPPED oversized=1\n");return;
    }
    err=sysLv2FsOpen(path,SYS_O_RDONLY,&fd,0,NULL,0);
    if(err){line("  state_integrity_open_error=0x%08x\n",(unsigned)err);return;}
    while(total<(u64)st.st_size){
        u64 remaining=(u64)st.st_size-total;
        u64 want=remaining<sizeof(buffer)?remaining:sizeof(buffer);
        got=0;
        err=sysLv2FsRead(fd,buffer,want,&got);
        if(err||got==0||got>want)break;
        for(u64 i=0;i<got;i++){hash^=buffer[i];hash*=UINT64_C(1099511628211);}
        total+=got;
    }
    sysLv2FsClose(fd);
    if(err||total!=(u64)st.st_size){
        line("  state_integrity=INCOMPLETE bytes=%llu error=0x%08x\n",
             (unsigned long long)total,(unsigned)err);
    }else{
        line("  state_integrity=READ_ONLY bytes=%llu fnv64=%08x%08x\n",
             (unsigned long long)total,(unsigned)(hash>>32),(unsigned)hash);
        line("  state_decoder=UNVERIFIED trophy_11_state=UNKNOWN\n");
    }
}

/* Validated read-only table lookup. Treat all layout failures as UNKNOWN. */
static void target_trophy_state(const char *root)
{
    char path[512];
    int fd=-1, err=0;
    unsigned char header[48], tables[32*32], entry[112];
    u64 got=0;
    sysFSStat st;
    unsigned n=0, found_grade=0,found_state=0,grade=0,state=0;
    uint64_t offsets[2]={0,0};
    unsigned strides[2]={0,0}, counts[2]={0,0};
    int valid=1;
    if(snprintf(path,sizeof(path),"%s/TROPUSR.DAT",root)>=(int)sizeof(path))return;
    err=sysLv2FsStat(path,&st);
    if(err||st.st_size<80||st.st_size>4*1024*1024ULL){line("  state_parse=UNKNOWN reason=size_or_stat\n");return;}
    err=sysLv2FsOpen(path,SYS_O_RDONLY,&fd,0,NULL,0);
    if(err){line("  state_parse=UNKNOWN reason=open\n");return;}
    err=sysLv2FsRead(fd,header,sizeof(header),&got);
    if(err||got!=sizeof(header)||be32(header)!=0x818f54ad||be32(header+4)!=0x00010000){valid=0;goto done;}
    n=be32(header+8);
    if(!n||n>32||48ULL+n*32ULL>(u64)st.st_size){valid=0;goto done;}
    got=0;
    err=sysLv2FsRead(fd,tables,n*32,&got);
    if(err||got!=n*32){valid=0;goto done;}
    for(unsigned i=0;i<n;i++){
        const unsigned char *p=tables+i*32;
        unsigned kind=be32(p),size=be32(p+4),count=be32(p+12);
        uint64_t off=((uint64_t)be32(p+16)<<32)|be32(p+20);
        if(kind!=4&&kind!=6)continue;
        unsigned k=kind==4?0:1, expected=kind==4?80:96;
        if((k==0&&found_grade)||(k==1&&found_state)||size!=expected||count>4096||
           off>(u64)st.st_size||((uint64_t)(size+16)*count)>(u64)st.st_size-off){
            valid=0;goto done;
        }
        offsets[k]=off;strides[k]=size+16;counts[k]=count;
        if(k==0)found_grade=1;else found_state=1;
    }
    if(!found_grade||!found_state){valid=0;goto done;}
    unsigned matches[2]={0,0};
    for(unsigned k=0;k<2;k++){
        for(unsigned i=0;i<counts[k];i++){
            u64 pos=offsets[k]+(u64)i*strides[k];
            if(sysLv2FsLSeek64(fd,pos,SEEK_SET,NULL)){valid=0;goto done;}
            got=0;
            err=sysLv2FsRead(fd,entry,strides[k],&got);
            if(err||got!=strides[k]||be32(entry)!=(k==0?4U:6U)||
               be32(entry+4)!=strides[k]-16){valid=0;goto done;}
            if(be32(entry+16)==11){
                if(++matches[k]!=1){valid=0;goto done;}
                unsigned value=be32(entry+20);
                if(k==0){if(value<1||value>4){valid=0;goto done;}grade=value;}
                else{if(value>1){valid=0;goto done;}state=value;}
            }
        }
    }
    if(matches[0]!=1||matches[1]!=1){valid=0;goto done;}
done:
    sysLv2FsClose(fd);
    if(valid)line("  trophy_11_state=%s grade=%u source=decoded_tables_unverified_on_retail\n",
                  state?"UNLOCKED":"LOCKED",grade);
    else line("  trophy_11_state=UNKNOWN state_parse=UNSUPPORTED_OR_INVALID error=0x%08x\n",(unsigned)err);
}

/* Inspect a bounded configuration snapshot; no trophy data is changed.
 * Matching markers is preflight evidence, NOT native context registration.
 */
static void target_config_preflight(const char *root)
{
    char path[512];
    int fd=-1, error;
    u64 n=0, total=0;
    static unsigned char data[131072 + 1];
    if(snprintf(path,sizeof(path),"%s/TROPCONF.SFM",root)>=(int)sizeof(path))return;
    error=sysLv2FsOpen(path,SYS_O_RDONLY,&fd,0,NULL,0);
    if(error){line("  context_preflight_config_open=0x%08x\n",(unsigned)error);return;}
    error=sysLv2FsRead(fd,data,sizeof(data)-1,&n);
    sysLv2FsClose(fd);
    if(error){line("  context_preflight_config_read=0x%08x\n",(unsigned)error);return;}
    total=n;
    data[total]=0;
    /* An SFM may contain XML; search bounded bytes, not untrusted pointers. */
    const char *title="Merciless Executioner";
    const char *count="trophy";
    unsigned found_title=0, found_trophy_tag=0;
    const size_t tlen=strlen(title),clen=strlen(count);
    for(size_t i=0;i+tlen<=total;i++)
        if(!memcmp(data+i,title,tlen)){found_title=1;break;}
    for(size_t i=0;i+clen<=total;i++)
        if(!memcmp(data+i,count,clen)){found_trophy_tag=1;break;}
    line("  context_preflight=read_only bytes=%llu expected_trophy_name=%u trophy_marker=%u\n",
         (unsigned long long)total,found_title,found_trophy_tag);
    /* Target trophy metadata may be inspected, but unlock state remains unknown.
     * Never derive "locked/unlocked" from the configuration file.
     */
    unsigned found_id_011=0,found_id_11=0;
    const char *id011="id=\"011\"";
    const char *id11="id=\"11\"";
    for(size_t i=0;i+strlen(id011)<=total;i++)
        if(!memcmp(data+i,id011,strlen(id011))){found_id_011=1;break;}
    for(size_t i=0;i+strlen(id11)<=total;i++)
        if(!memcmp(data+i,id11,strlen(id11))){found_id_11=1;break;}
    line("  target_trophy_id=11 name=Merciless_Executioner id_marker=%u name_marker=%u\n",
         found_id_011||found_id_11,found_title);
    line("  target_trophy_unlock_state=UNKNOWN requires_validated_state_parser_or_native_api\n");
    line("  native_context_registered=UNTESTED\n");
}

int trophy_discovery(void)
{
    catalog_count=0;report_fd=-1;report_failed=0;
    char path[512];int home_fd;static sysFSDirent user;u64 read=0;unsigned users=0,sets=0;
    if(begin_report())return 2;
    line("PS3 Trophy Unlocker 2026: READ-ONLY discovery probe v0.0.15-state-check\nLocal numeric user folders are logged; no trophy writes or PSN calls.\nstage=opening_home\n");
    if(sysLv2FsFsync(report_fd))report_failed=1;
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
        int trophy_fd;static sysFSDirent set;
        error=sysLv2FsOpenDir(path,&trophy_fd);
        if(error){line("user=%s trophy_open_error=0x%08x\n",user.d_name,(unsigned)error);continue;}
        unsigned set_entries=0;
        while(sets<MAX_SETS&&set_entries++<16384){
            error=sysLv2FsReadDir(trophy_fd,&set,&read);
            if(error){line("trophy_read_error=0x%08x\n",(unsigned)error);report_failed=1;break;}
            if(!read)break;
            set.d_name[sizeof(set.d_name)-1]=0;
            if(!npwr(set.d_name))continue;
            memcpy(catalog[catalog_count].user,user.d_name,8);catalog[catalog_count].user[8]=0;
            memcpy(catalog[catalog_count].set,set.d_name,12);catalog[catalog_count].set[12]=0;
            catalog_count++;
            sets++;line("user=%s set=%s\n",user.d_name,set.d_name);
            snprintf(path,sizeof(path),"/dev_hdd0/home/%.8s/trophy/%.12s",user.d_name,set.d_name);
            inspect_header(path,"TROPUSR.DAT");inspect_header(path,"TROPTRNS.DAT");
            if(!strcmp(set.d_name,"NPWR11866_00")){target_config_preflight(path);target_state_integrity(path);target_trophy_state(path);}
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

