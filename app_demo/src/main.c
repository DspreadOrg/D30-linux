#include "appinc.h"
#include <signal.h>

void sigHandle(int sig)
{
    switch (sig) {
    case SIGINT:
		OsLog(LOG_ERROR,"*** catch signal : SIGINT, value = %d\n", sig);
        break;
    case SIGTERM:
		OsLog(LOG_ERROR,"*** catch signal : SIGTERM, value = %d\n", sig);
        break;
    case SIGBUS:
		OsLog(LOG_ERROR,"*** catch signal : SIGBUS, value = %d\n", sig);
        break;
    case SIGSEGV:
		OsLog(LOG_ERROR,"*** catch signal : SIGSEGV, value = %d\n", sig);
        break;
    case SIGFPE:
		OsLog(LOG_ERROR,"*** catch signal : SIGFPE, value = %d\n", sig);
        break;
    case SIGABRT:
		OsLog(LOG_ERROR,"*** catch signal : SIGABRT, value = %d\n", sig);
        break;
    case SIGSTOP:
		OsLog(LOG_ERROR,"*** catch signal : SIGSTOP, value = %d\n", sig);
        break;
    default:
		OsLog(LOG_ERROR,"*** catch unknown signal, value = %d\n", sig);
        break;
    }
    exit(0);
}

void signalHandle(){
    signal(SIGPIPE, sigHandle);	//忽略信号
    signal(SIGINT, sigHandle);  //Ctrl + C
    signal(SIGTERM, sigHandle); //kill发出的软件终止
    signal(SIGBUS, sigHandle);  //总线错误
    signal(SIGSEGV, sigHandle); //非法内存访问
    signal(SIGFPE, sigHandle);  //数学相关的异常，如被0除，浮点溢出等
    signal(SIGABRT, sigHandle); //由调用abort函数产生，进程非正常退出
    signal(0x108, sigHandle);
    signal(0x107, sigHandle);
    // signal(SIGSTOP, sigHandle); //由调用abort函数产生，进程非正常退出
}

extern int ymframwork_init(int argc, char *argv[]);
extern int ymframwork_finish(int ret);
extern void lvgl_main(void);

void lvgl_disp_pause(int runstate, void *par)
{
    disp_disable_update();
}
void lvgl_disp_resume(int runstate, void *par)
{
    disp_enable_update();
}

void DisplayInit(){
    Disp_vInit(); //UI  init
    Disp_vSetStatusBarStatus(0);
    Disp_vRegisterPauseCallBack(lvgl_disp_pause);
    Disp_vRegisterResumeCallBack(lvgl_disp_resume);
}

int App_nInitialization()
{
    char szAppId[] = "linux_app";
    OsLogSetTag(szAppId);  //set logcat tag
    signalHandle();
    Disp_vSetStatusBarStatus(0);
	DisplayInit();   //UI init
    DB_bInit(szAppId); //database init
    EmvL2_Init();   //EMV init
    //EmvL2_SetDefaultAidAndCapk();

	return 0;
}

int main(int argc, char *argv[])
{
    int ret;
    ymframwork_init(argc, argv);//system api
    App_nInitialization();
    lvgl_main();
    Disp_vSetStatusBarStatus(1);
    ymframwork_finish(ret);//system api
    return 0;
}