#ifndef _SOCKET_H__
#define _SOCKET_H__  

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define	INVALID_SOCKET			-1
#define closesocket(sock)		close(sock)
#define SOCKET_CONNECT_TIMEOUT           60
#define SOCKET_RECV_TIMEOUT              60
#define   SOCK_ERR_SUCCESS                   0
#define  READBUFF_SIZE                      (4*1024) 

typedef enum __SOCKET_ERR_STEP_ENUM {
    SOCK_ERR_STEP_PARA = 1,           
    SOCK_ERR_STEP_GETHOSTBYNAME,
    SOCK_ERR_STEP_CREATESOCKET,
    SOCK_ERR_STEP_CONNECT,  
    SOCK_ERR_STEP_SEND,
    SOCK_ERR_STEP_RECV,
	SOCK_ERR_STEP_PARSE,

    SOCK_ERR_STEP_SSL_INIT = 50,  
    SOCK_ERR_STEP_SSL_CONNECT,  
    SOCK_ERR_STEP_SSL_SEND ,
    SOCK_ERR_STEP_SSL_RECV,
    SOCK_ERR_STEP_SELECT,  

    SOCK_ERR_CURL_PARA = 100,
    SOCK_ERR_CURL_INIT,
    SOCK_ERR_CURL_PERFORM,
    SOCK_ERR_MAX = 999,
}SOCKET_ERR_STEP_ENUM;				/* ----------  end of enum SOCKET_ERR_STEP_ENUM  ---------- */

typedef struct __socket_parameters_t {
	int  custno;								//当前商户编号
	char standby_flag;							//是否使用备用域名ip标识
    char host[256];
    char ip[256];                               /* IP、域名 */
    int  port;                                  /* 端口 */
    int  ssl_flag;                              /* 是否启动SSL */
	int  union_https_flag;						//直连银联公网标识
	int  normal_http_flag;						//普通http报文头处理
    int  connect_time;                          /* 连接超时时间，单位秒,默认30s */
	int  connect_time_us;                       /* 连接超时时间，单位微秒,默认0s */
    int  recv_time;                             /* 接收超时时间，单位秒,默认30s */
	int  recv_time_us;                          /* 接收超时时间，单位微秒,默认0s */
    int  is_show_title;                         /* 是否显示倒计时 //0前台、1后台、2不显示标识*/
    int  sendlen;
    char *sendbuf;
	int socket_id;								//返回的预拨号的socket id
	SSL_CTX *ctx;								
	SSL *ssl;
}socket_parameters_t;				/* ----------  end of struct socket_parameters_t  ---------- */

int socket_set_noblock (int s,int noblock);

int socket_connect (int socket, struct sockaddr_in addr,struct timeval tv);
int socket_recv(int nCustNo, int socket, char *buf, int len,struct timeval tv);

int protocol_socket_tcp(socket_parameters_t *parameters, int recvlen,char *outbuf);
int protocol_socket_udp (socket_parameters_t *parameters, int recvlen, char *outbuf);

int protocol_socket_tcp_pre_connect (socket_parameters_t *parameters);
int protocol_socket_tcp_follow_connect (socket_parameters_t *parameters, char *pre_connect_flag);
int protocol_socket_tcp_write (socket_parameters_t *parameters);
int protocol_socket_tcp_read (socket_parameters_t *parameters, int recvlen,char *outbuf);
int protocol_socket_tcp_dialoff (socket_parameters_t *parameters);
int socket_gethostbyname (int nCustNo, char * indata, char * outdata);
int socket_tcp_alive (socket_parameters_t *parameters, int recvlen,char *outbuf);
int socket_tcp_dialoff (socket_parameters_t *parameters);

int socket_tcp_ssl (socket_parameters_t *parameters, int recvlen,char *outbuf);
int socket_conversion_parameters (socket_parameters_t *parameters, struct sockaddr_in *addr);

s32 ssl_server_connect(char * pHost,ssize_t port,int timeout);
s32 ssl_send_msg(char * pData, ssize_t DataLength, int timeout);
s32 ssl_recv_msg(char * outbuf, ssize_t recvlen, int timeout);
void ssl_server_disconnect();
#endif
