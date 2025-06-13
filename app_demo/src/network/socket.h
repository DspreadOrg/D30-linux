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
	int  custno;								// Current merchant number
	char standby_flag;							// Flag indicating whether to use the backup domain name or IP
    char host[256];
    char ip[256];                               /* IP address or domain name */
    int  port;                                  /* Port number */
    int  ssl_flag;                              /* Flag indicating whether to enable SSL */
	int  union_https_flag;						// Flag indicating direct connection to UnionPay public network
	int  normal_http_flag;						// Ordinary HTTP header processing
    int  connect_time;                          /* Connection timeout in seconds, default is 30s */
	int  connect_time_us;                       /* Connection timeout in microseconds, default is 0s */
    int  recv_time;                             /* Receive timeout in seconds, default is 30s */
	int  recv_time_us;                          /* Receive timeout in microseconds, default is 0s */
    int  is_show_title;                         /* Flag indicating whether to show the countdown // 0: foreground, 1: background, 2: do not show*/
    int  sendlen;
    char *sendbuf;
	int socket_id;								//socket id
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

s32 ssl_server_connect(char * pHost,ssize_t port,int timeout,int mode);
s32 ssl_send_msg(char * pData, ssize_t DataLength, int timeout);
s32 ssl_recv_msg(char * outbuf, ssize_t recvlen, int timeout);
void ssl_server_disconnect();
#endif
