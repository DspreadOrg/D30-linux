/*
********************************************************************************
*
*   File Name:
*       Req_Server.c
*   Author:
*       SW R&D Department
*   Version:
*       V1.0
*   Description:
*
*
********************************************************************************
*/

/*-----------------------------------------------------------------------------
|   Includes
+----------------------------------------------------------------------------*/
#include "app_server.h"

/*-----------------------------------------------------------------------------
|   Variables
+----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
|   Constants
+----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
|   Functions
+----------------------------------------------------------------------------*/
/*--------------------------------------
|   Function Name:
|       RequestPackHttpFrame
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
Rc_t RequestPackHttpFrame(char * strMsg, char * pURL, char * pMethod,char * pFrame,char * pRange, u32 FrameLengthMax )
{
    HttpPack_t HttpPack;

    memset((pu8)(&HttpPack), 0x00, sizeof(HttpPack));
    OsLog(LOG_DEBUG, "\r\nRequest_URL: %s\r\n\r\n",pURL);
    HttpPack.pRequestMessage = strMsg;
    HttpPack.pRequestBody = pFrame;
    HttpPack.Line.pMethod = malloc(32);
    HttpPack.Line.pUrl = malloc(256);
    HttpPack.Line.pVersion = malloc(32);
    HttpPack.Head.pConnection = malloc(32);
    HttpPack.Head.pHost = malloc(128);
    HttpPack.Head.pContentType = malloc(64);
    if(pRange!=NULL)
    {
        HttpPack.Head.pRange = malloc(64);
        memset(HttpPack.Head.pRange, 0x00, 64);
    }

    memset(HttpPack.pRequestBody, 0x00, FrameLengthMax);
    memset(HttpPack.Line.pMethod, 0x00, 32);
    memset(HttpPack.Line.pUrl, 0x00, 256);
    memset(HttpPack.Line.pVersion, 0x00, 32);
    memset(HttpPack.Head.pConnection, 0x00, 32);
    memset(HttpPack.Head.pHost, 0x00, 128);
    memset(HttpPack.Head.pContentType, 0x00, 64);


    strcat(HttpPack.Line.pMethod, pMethod);

    HttpUrlGetPath(pURL, HttpPack.Line.pUrl, 256);

    strcat(HttpPack.Line.pVersion, "HTTP/1.1");//HTTP/1.1

    strcat(HttpPack.Head.pConnection, "Keep-Alive");

    HttpUrlGetHost(pURL, HttpPack.Head.pHost, 128);

    if(pRange!=NULL)
    {
        strcat(HttpPack.Head.pRange, pRange);
    }

    strcat(HttpPack.Head.pContentType, "application/json");//application/json

    HttpFramePack(&HttpPack);

    free(HttpPack.Line.pMethod);
    free(HttpPack.Line.pUrl);
    free(HttpPack.Line.pVersion);
    free(HttpPack.Head.pConnection);
    free(HttpPack.Head.pHost);
    free(HttpPack.Head.pContentType);
    if(HttpPack.Head.pRange!=NULL)
    {
        free(HttpPack.Head.pRange);
    }

    return RC_SUCCESS;
}
/*--------------------------------------
|   Function Name:
|       GetHttpHeadFramLen
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
u32 GetHttpHeadFramLen(const char *strMsg)
{
    int Cnt = 0;
    char* pStr=NULL;
    if(strMsg==NULL)
    {
        return Cnt;
    }
    pStr = strstr(strMsg, "\r\n\r\n");
    if(pStr)
    {
        pStr += 4;
        Cnt=pStr-strMsg;
    }
    else
    {
        Cnt = 0;
    }
    pStr=NULL;
    return Cnt;
}
/*--------------------------------------
|   Function Name:
|       GetHttpContentFram
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static u32 GetHttpContentFram(const char *strMsg,s32 FramLen)
{
    s32 Cnt = 0;
    char* pStr=NULL;

    if(strMsg==NULL || FramLen==0)
    {
        return Cnt;
    }
    pStr = strstr(strMsg, "\r\n\r\n");
    if(pStr)
    {
        pStr += 4;

        Cnt=pStr-strMsg;
    }
    else
    {
        Cnt = 0;
    }
    return FramLen-Cnt;
}
/*--------------------------------------
|   Function Name:
|       RequestHttpMessageCheck
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static bool RequestHttpMessageCheck(char * pHttpBody,u32 ReceiveLen)
{
    HttpUnpack_t HttpUnpack;
    u32 ContentLen = 0;
    u32 HeadLen=0;
    bool res=false;
    u8 ContentLength[32] = {0};
    u8 Connection[32] = {0};
    if(pHttpBody==NULL || ReceiveLen==0)
    {
        return true;
    }

    if(!strstr(pHttpBody,"\r\n\r\n"))
    {
        return false;
    }

    memset((pu8)(&HttpUnpack), 0x00, sizeof(HttpUnpack));
    HttpUnpack.pResponseBody = pHttpBody;

    HttpUnpack.Head.pContentLength = ContentLength;
    HttpUnpack.Head.pConnection = Connection;
    memset(HttpUnpack.Head.pContentLength, 0x00,32);
    memset(HttpUnpack.Head.pConnection, 0x00,32);
    HttpFrameUnpack(&HttpUnpack);

    ContentLen = atoi(HttpUnpack.Head.pContentLength);
    if(ContentLen!=0)
    {
        HeadLen=GetHttpHeadFramLen(HttpUnpack.pResponseBody);
        #if defined(CFG_OTA_DEBUG)
        OsLog(LOG_DEBUG,"\r\n%s--ReceiveLen=%d contentlen=%d  HeadLen=%d\r\n",__FUNCTION__,ReceiveLen,ContentLen,HeadLen);
        #endif
        if(ContentLen+HeadLen==ReceiveLen)
        {
            res= true;
        }
    }
    else if(strcmp(HttpUnpack.Head.pConnection,"close"))
    {
        res=true;
    }
    return res;
}

/*--------------------------------------
|   Function Name:
|       HttpLineStatusCheck
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
Rc_t HttpLineStatusCheck(pu8 pTradingFile, const char * pHttpBody)
{
    Rc_t Rc = RC_FAIL;
    HttpUnpack_t HttpUnpack;
    memset((pu8)(&HttpUnpack), 0x00, sizeof(HttpUnpack));
    HttpUnpack.pResponseBody = (char * )pHttpBody;

    HttpUnpack.Line.pStatus = malloc(64);
    memset(HttpUnpack.Line.pStatus, 0x00, 64);

    HttpFrameUnpack(&HttpUnpack);
    Rc=((strcmp(HttpUnpack.Line.pStatus,"200")|| strcmp(HttpUnpack.Line.pStatus,"206"))?RC_SUCCESS:RC_FAIL);
    free(HttpUnpack.Line.pStatus);
    return Rc;
}

int is_chunked_and_process(const char* response, size_t resp_len, Buffer* new_resp) {
    const char* header_end = "\r\n\r\n";
    const char* body_start = strstr(response, header_end);
    if (!body_start) return 0;

    // 检查Transfer-Encoding头
    const char* te_header = "Transfer-Encoding:";
    const char* hdr = response;
    int is_chunked = 0;
    
    // 遍历头部
    while (hdr < body_start) {
        const char* line_end = strstr(hdr, "\r\n");
        if (!line_end) break;

        // 检查是否为Transfer-Encoding头
        if (strncasecmp(hdr, te_header, strlen(te_header)) == 0) {
            const char* value = hdr + strlen(te_header);
            while (*value && isspace(*value)) value++;
            if (strncasecmp(value, "chunked", 7) == 0) {
                is_chunked = 1;
                break;
            }
        }
        hdr = line_end + 2;
    }
   
    if (!is_chunked) return 0;

    // 处理分块数据
    const char* chunk_ptr = body_start + 4;
    const char* resp_end = response + resp_len;
    Buffer decoded = {0};
 
    #define MAX_HEX_DIGITS 16
    #define MAX_BODY_SIZE (10*1024*1024)
    
    while (chunk_ptr < resp_end) {
        // 1. 获取块长度行
        const char* chunk_end = strstr(chunk_ptr, "\r\n");
        if (!chunk_end || chunk_end >= resp_end) break;
    
        // 2. 安全解析HEX长度
        char hex_len[MAX_HEX_DIGITS + 1] = {0};
        size_t hex_size = chunk_end - chunk_ptr;
        hex_size = (hex_size > MAX_HEX_DIGITS) ? MAX_HEX_DIGITS : hex_size;
        memcpy(hex_len, chunk_ptr, hex_size);
        hex_len[hex_size] = '\0';
    
        char* endptr;
        long chunk_size = strtol(hex_len, &endptr, 16);
        if (endptr == hex_len || *endptr != '\0' || chunk_size <= 0) break;
    
        // 3. 指针安全跳转
        chunk_ptr = chunk_end + 2;
        if (chunk_ptr + chunk_size + 2 > resp_end) break;
    
        // 4. 安全内存分配
        size_t new_size = decoded.length + chunk_size + 1;
        if (new_size > MAX_BODY_SIZE) {
            free(decoded.data);
            return 0;
        }
        void* tmp = realloc(decoded.data, new_size);
        if (!tmp) {
            free(decoded.data);
            return 0;
        }
        decoded.data = tmp;
    
        // 5. 数据拷贝
        memcpy(decoded.data + decoded.length, chunk_ptr, chunk_size);
        decoded.length += chunk_size;
    
        // 6. 跳转到下一个块
        chunk_ptr += chunk_size + 2;
    }

    // 重构响应头
    Buffer headers = {0};
    hdr = response;

    while (hdr < body_start) {
        const char* line_end = strstr(hdr, "\r\n");
        if (!line_end) break;

        // 跳过Transfer-Encoding头
        if (strncasecmp(hdr, te_header, strlen(te_header)) != 0) {
            // 保留其他头
            size_t line_len = line_end - hdr + 2;
            headers.data = realloc(headers.data, headers.length + line_len);
            memcpy(headers.data + headers.length, hdr, line_len);
            headers.length += line_len;
        }
        
        hdr = line_end + 2;
    }

    // 添加Content-Length头
    char cl_header[128];
    int cl_len = snprintf(cl_header, sizeof(cl_header), 
                        "Content-Length: %zu\r\n", decoded.length);
    headers.data = realloc(headers.data, headers.length + cl_len);
    memcpy(headers.data + headers.length, cl_header, cl_len);
    headers.length += cl_len;

    // 组合新响应
    new_resp->length = headers.length + 2 + decoded.length; // +2 for ending CRLF
    new_resp->data = malloc(new_resp->length);
    memcpy(new_resp->data, headers.data, headers.length);
    memcpy(new_resp->data + headers.length, "\r\n", 2);
    memcpy(new_resp->data + headers.length + 2, decoded.data, decoded.length);

    free(headers.data);
    free(decoded.data);
    return 1;
}

/*--------------------------------------
|   Function Name:
|       RequestHttpTransmit
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
s32 RequestHttpTransmit(char * pSend,u32 SendLen, char * pURL,u32 port, char * pReceived, u32 FrameLengthMax )
{
    int ret;
    char * pScheme = NULL;
    char * pHost = NULL;
    s32    Length = 0;
    if(pSend == NULL || pURL == NULL || FrameLengthMax == 0)
    {
        return 0;
    }

    OsLog(LOG_DEBUG, "\r\npSend(%d) : \r\n%s\r\n\r\n",SendLen,pSend);

    pScheme = malloc(128);
    pHost = malloc(64);
    memset(pScheme, 0x00, 128);
    memset(pHost, 0x00,64);
    HttpUrlGetScheme(pURL, pScheme, 128);
    HttpUrlGetHost(pURL, pHost, 64);

    if( ssl_server_connect(pHost,port,ONLINE_REQUEST_TIMEOUT,SSL_VERIFY_NONE) != RC_SUCCESS)
    {
        ret = -99;
        if(pScheme!=NULL)
        {
            memset(pScheme, 0x00, 128);
            free(pScheme);
            pScheme=NULL;
        }
        if(pHost!=NULL)
        {
            memset(pHost, 0x00,64);
            free(pHost);
            pHost=NULL;
        }

        return ret;
    }

    if(ssl_send_msg(pSend,SendLen,ONLINE_REQUEST_TIMEOUT) < 0)
    {
        ret = -99;
        goto RequestHttpTransmit_end;
    }
        
    Length = ssl_recv_msg(pReceived,FrameLengthMax,ONLINE_REQUEST_TIMEOUT);    

     if(Length<0)
    {
        pReceived[0] = '\0';
    }
    else
    {
        Buffer new_response = {0};
        if (is_chunked_and_process(pReceived, Length, &new_response)) {
            // OsLog(LOG_DEBUG,"Refactored response:\n%.*s\n[Body:%zu]\n",(int)new_response.length, new_response.data, new_response.length);
            memset(pReceived,0,FrameLengthMax);
            memcpy(pReceived,new_response.data,new_response.length);
            Length = new_response.length;      
            free(new_response.data);
        } else {
            #ifdef CFG_DBG
            OsLog(LOG_DEBUG,"No need refactored\n");
            #endif
        }

        
    }
RequestHttpTransmit_end:

    ssl_server_disconnect();
    if(pScheme!=NULL)
    {
        memset(pScheme, 0x00, 128);
        free(pScheme);
        pScheme=NULL;
    }
    if(pHost!=NULL)
    {
        memset(pHost, 0x00,64);
        free(pHost);
        pHost=NULL;
    }

    OsLog(LOG_DEBUG, "\r\nRECV(%d) : \r\n",Length);
    return Length;
}
/*--------------------------------------
|   Function Name:
|       checkRepo
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static Rc_t checkRepo(pu8 pValue)
{
    return (strstr(pValue,"path")?RC_SUCCESS:RC_FAIL);
}