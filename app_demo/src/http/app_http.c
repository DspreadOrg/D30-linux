/*
********************************************************************************
*
*   File Name:
*       Req_Http.c
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
#include "app_http.h"

/*-----------------------------------------------------------------------------
|   Macros
+----------------------------------------------------------------------------*/
#define IS_SPACE(c, x)  \
    ((c) == ' ' || (c) == '\t' || (c) == '\r' || (c) == '\n' || (c) == '\v' || (c) == '\f' \
        || (x != '\0' && (c) == x))
/*-----------------------------------------------------------------------------
|   Functions
+----------------------------------------------------------------------------*/
char *trim(char *s, char cExt)
{
    char * p = s;
    int l = strlen(p);

    while(IS_SPACE(p[l - 1], cExt)) p[--l] = 0;
    while(* p && IS_SPACE(*p, cExt)) ++p, --l;
    memcpy(s, p, l + 1);
    return s;
}

T_URL_INFO* parse_url(T_URL_INFO* info, char* strUrl)
{
	T_U8_VIEW uvTmp = {(u8 *)strUrl, 0};
	char *pSuHead = strUrl;
    if (!info || !strUrl) return NULL;
	char *pCur = strstr(strUrl, "://");
	if(!pCur)
		info->eProtocol = TCP;
	else
	{
		uvTmp.len = pCur - strUrl;
		if(uvcmpstr(&uvTmp, "http") == 0)
			info->eProtocol = HTTP;
		else if(uvcmpstr(&uvTmp, "https") == 0)
			info->eProtocol = HTTPS;
		else
			info->eProtocol = UNKNOWN_PROTO;
		pSuHead = pCur + 3;
	}
	pCur = strstr(pSuHead, ":");
	if(pCur)
	{
		info->uvSite.head = (u8 *)pSuHead;
		info->uvSite.len = pCur - pSuHead;
		pSuHead = pCur + 1;
	}
	pCur = strstr(pSuHead, "/");
	if(pCur)
	{
		if(info->uvSite.head)
		{
			uvTmp.head = (u8 *)pSuHead;

			uvTmp.len = pCur - pSuHead;
			info->wPort = su2num(&uvTmp);
		}
		else
		{
			info->uvSite.head = (u8 *)pSuHead;
			info->uvSite.len = pCur - pSuHead;
			if(info->eProtocol == HTTP)
				info->wPort = 80;
			else if(info->eProtocol == HTTPS)
				info->wPort = 443;
		}
		pSuHead = pCur + 1;
		info->uvPath.head = (u8 *)pSuHead;
		info->uvPath.len = strlen(pSuHead);
	}
	else if(info->eProtocol == TCP)
	{
		uvTmp.head = (u8 *)pSuHead;
		uvTmp.len = strlen(pSuHead);
		info->wPort = su2num(&uvTmp);
	}
	return info;
}

/*--------------------------------------
|   Function Name:
|       HttpUnpackGetVersion
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static int HttpUnpackGetLineVersion(const char * strMsg, char * pValue, unsigned int ValueLenMax)
{
    int Cnt = 0;
    if(strMsg==NULL || ValueLenMax==0)
    {
        return Cnt;
    }
    while(*strMsg != ' ')
    {
        if(Cnt > (int)ValueLenMax)
        {
            return -1;
        }
        else
        {

        }
        pValue[Cnt++] = *strMsg;
        strMsg++;
    }
    return Cnt;
}
/*--------------------------------------
|   Function Name:
|       HttpUnpackGetLineStatus
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int HttpUnpackGetLineStatus(const char * strMsg, char * pValue, unsigned int ValueLenMax)
{
    int Cnt = 0;
    if(strMsg==NULL || ValueLenMax==0)
    {
        return Cnt;
    }
    while(*strMsg != ' ')
    {
        strMsg++;
    }
    strMsg++;
    while(*strMsg != ' ')
    {
        if(Cnt > (int)ValueLenMax)
        {
            return -1;
        }
        else
        {

        }
        pValue[Cnt++] = *strMsg;
        strMsg++;
    }
    return Cnt;
}
/*--------------------------------------
|   Function Name:
|       HttpUnpackGetLineStatusDescription
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static int HttpUnpackGetLineStatusDescription(const char * strMsg, char * pValue, unsigned int ValueLenMax)
{
    int Cnt = 0;
    if(strMsg==NULL || ValueLenMax==0)
    {
        return Cnt;
    }
    while(*strMsg != ' ')
    {
        strMsg++;
    }
    strMsg++;
    while(*strMsg != ' ')
    {
        strMsg++;
    }
    strMsg++;
    while(*strMsg != '\n')
    {
        if(Cnt > (int)ValueLenMax)
        {
            return -1;
        }
        else
        {

        }
        pValue[Cnt++] = *strMsg;
        strMsg++;
    }

    return Cnt;
}
/*--------------------------------------
|   Function Name:
|       HttpUnpackGetHeadParam
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int HttpUnpackGetHeadParam(const char * strMsg, const char * strKey, char * pValue, unsigned int ValueLenMax)
{
    int Cnt = 0;
    char * pStr=NULL;

    while(*strMsg != ' ') strMsg++;
    strMsg++;
    while(*strMsg != ' ') strMsg++;
    strMsg++;
    while(*strMsg != '\n')
        strMsg++;
    strMsg++;
    pStr = strstr(strMsg, strKey);
    if(pStr)
    {
        pStr += strlen(strKey);
        pStr += 2;
        while(*pStr != '\n')
        {
            pValue[Cnt++] = *pStr;
            pStr++;
        }
    }
	trim(pValue, 0);
    return Cnt;
}

/*--------------------------------------
|   Function Name:
|       HttpUnpackGetResponseParam
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static int HttpUnpackGetResponseParam(const char * strMsg, char * pValue, unsigned int ValueLenMax)
{
    int Cnt = 0;
    char* pStr=NULL;
    if(strMsg==NULL || ValueLenMax==0)
    {
        return Cnt;
    }
    pStr = strstr(strMsg, "\r\n\r\n");
    if(pStr)
    {
        pStr += 4;
        if(strlen(pStr) > ValueLenMax)
        {
            return -1;
        }
        else
        {

        }
        Cnt = strlen(pStr);
        memcpy(pValue, pStr, Cnt);
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
|       HttpUrlGetHost
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int HttpUrlGetHost(char * strMsg, char * pValue, unsigned int ValueLenMax)
{
    int Cnt = 0;
    char* pStr=NULL;
    if(strMsg==NULL || ValueLenMax==0)
    {
        return Cnt;
    }
    pStr = strstr(strMsg, "//");
    if(pStr)
    {
        pStr += 2;
        while(*pStr != '\0')
        {
            if(*pStr==':' || *pStr=='/')
            {
                break;
            }
            pValue[Cnt++] = *pStr;
            pStr++;
        }
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
|       HttpUrlGetScheme
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int HttpUrlGetScheme(char * strMsg, char * pValue, unsigned int ValueLenMax)
{
    int Cnt = 0;
    char* pStr=NULL;
    if(strMsg==NULL || ValueLenMax==0)
    {
        return Cnt;
    }
    pStr = strMsg;
    while(*pStr != ':' && *pStr != '\0')
    {
        pValue[Cnt++] = *pStr;
        pStr++;
    }
    pStr=NULL;
    return Cnt;
}
/*--------------------------------------
|   Function Name:
|       HttpUrlGetPath
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int HttpUrlGetPath(char * strMsg, char * pValue, unsigned int ValueLenMax)
{
    int Cnt = 0;
    char* pStr=NULL;
    if(strMsg==NULL || ValueLenMax==0)
    {
        return Cnt;
    }
    pStr = strstr(strMsg, "//");
    if(pStr)
    {
        pStr += 2;
        while(*pStr != '/' && *pStr != '\0')
        {
            pStr++;
        }
        if(*pStr != '\0')
        {
            while(*pStr != '\0')
            {
                pValue[Cnt++] = *pStr;
                pStr++;
            }

        }
        else
        {
            Cnt = 0;
        }
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
|       HttpGetStrKey
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int HttpGetStrKey(char * strMsg, char * strKey, char * pValue, unsigned int ValueLenMax)
{
    int Cnt = 0;
    char * pStr;

    pStr = strstr(strMsg, strKey);
    if(pStr)
    {
        pStr += strlen(strKey);
        pStr += 2;
        while(*pStr!= '\"')
        {

            if(*pStr == '\"')
            {
                break;
            }
            pStr++;
        }
        if(*pStr == '\"')
        {
            pStr++;
            while(*pStr != '\"')
            {
                if(*pStr != '\\')
                {
                    pValue[Cnt++] = *pStr;
                }
                else
                {
                    pStr++;
                    pValue[Cnt++] = *pStr;
                }
                pStr++;
            }
        }
        else
        {
            while(*pStr != ',' && *pStr != '}')
            {
                if(*pStr != '\\')
                {
                    pValue[Cnt++] = *pStr;
                }
                else
                {
                    pStr++;
                    pValue[Cnt++] = *pStr;
                }
                pStr++;
            }
        }
    }
    else
    {
        Cnt = 0;
    }
    #if defined(CFG_DBG)
    OsLog(LOG_DEBUG,"key:%s  value:%s\r\n",strKey,pValue);
    #endif
    return Cnt;
}

/*--------------------------------------
|   Function Name:
|       HttpPack
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int HttpFramePack(pHttpPack_t pHttpPack)
{
    if(!pHttpPack->pRequestBody)
    {
        return 0;
    }
    else
    {

    }

    if(pHttpPack->Line.pMethod)
    {
        strcat(pHttpPack->pRequestBody, pHttpPack->Line.pMethod);
    }
    else
    {
        return 0;
    }

    strcat(pHttpPack->pRequestBody, " ");

    if(pHttpPack->Line.pUrl)
    {
        strcat(pHttpPack->pRequestBody, pHttpPack->Line.pUrl);
    }
    else
    {
        return 0;
    }

    strcat(pHttpPack->pRequestBody, " ");

    if(pHttpPack->Line.pVersion)
    {
        strcat(pHttpPack->pRequestBody, pHttpPack->Line.pVersion);
    }
    else
    {
        return 0;
    }

    strcat(pHttpPack->pRequestBody, "\r\n");

    //head
    if(pHttpPack->Head.pAccept)
    {
        strcat(pHttpPack->pRequestBody, "Accept: ");
        strcat(pHttpPack->pRequestBody, pHttpPack->Head.pAccept);
        strcat(pHttpPack->pRequestBody, "\r\n");
    }
    else
    {

    }

    if(pHttpPack->Head.pAcceptCharset)
    {
        strcat(pHttpPack->pRequestBody, "Accept-Charset: ");
        strcat(pHttpPack->pRequestBody, pHttpPack->Head.pAcceptCharset);
        strcat(pHttpPack->pRequestBody, "\r\n");
    }
    else
    {

    }

    if(pHttpPack->Head.pAcceptEncoding)
    {
        strcat(pHttpPack->pRequestBody, "Accept-Encoding: ");
        strcat(pHttpPack->pRequestBody, pHttpPack->Head.pAcceptEncoding);
        strcat(pHttpPack->pRequestBody, "\r\n");
    }
    else
    {

    }

    if(pHttpPack->Head.pAcceptLanguage)
    {
        strcat(pHttpPack->pRequestBody, "Accept-Language: ");
        strcat(pHttpPack->pRequestBody, pHttpPack->Head.pAcceptLanguage);
        strcat(pHttpPack->pRequestBody, "\r\n");
    }
    else
    {

    }

    if(pHttpPack->Head.pAuthorization)
    {
        strcat(pHttpPack->pRequestBody, "Authorization: ");
        strcat(pHttpPack->pRequestBody, pHttpPack->Head.pAuthorization);
        strcat(pHttpPack->pRequestBody, "\r\n");
    }
    else
    {

    }

    if(pHttpPack->Head.pConnection)
    {
        strcat(pHttpPack->pRequestBody, "Connection: ");
        strcat(pHttpPack->pRequestBody, pHttpPack->Head.pConnection);
        strcat(pHttpPack->pRequestBody, "\r\n");
    }
    else
    {

    }

    if(pHttpPack->pRequestMessage)
    {
        char* pTmp = NULL;

        pTmp = malloc(128);
        memset(pTmp, 0x00, 128);
        sprintf(pTmp, "Content-Length: %d\r\n", strlen(pHttpPack->pRequestMessage));
        strcat(pHttpPack->pRequestBody, pTmp);
        free(pTmp);
        pTmp=NULL;
    }
    else
    {

    }

    if(pHttpPack->Head.pCookie)
    {
        strcat(pHttpPack->pRequestBody, "Cookie: ");
        strcat(pHttpPack->pRequestBody, pHttpPack->Head.pCookie);
        strcat(pHttpPack->pRequestBody, "\r\n");
    }
    else
    {

    }

    if(pHttpPack->Head.pFrom)
    {
        strcat(pHttpPack->pRequestBody, "From: ");
        strcat(pHttpPack->pRequestBody, pHttpPack->Head.pFrom);
        strcat(pHttpPack->pRequestBody, "\r\n");
    }
    else
    {

    }

    if(pHttpPack->Head.pHost)
    {
        strcat(pHttpPack->pRequestBody, "Host: ");
        strcat(pHttpPack->pRequestBody, pHttpPack->Head.pHost);
        strcat(pHttpPack->pRequestBody, "\r\n");
    }
    else
    {
        return 0;
    }

    if(pHttpPack->Head.pIfModifiedSince)
    {
        strcat(pHttpPack->pRequestBody, "If-Modified-Since: ");
        strcat(pHttpPack->pRequestBody, pHttpPack->Head.pIfModifiedSince);
        strcat(pHttpPack->pRequestBody, "\r\n");
    }
    else
    {

    }

    if(pHttpPack->Head.pPragma)
    {
        strcat(pHttpPack->pRequestBody, "Pragma: ");
        strcat(pHttpPack->pRequestBody, pHttpPack->Head.pPragma);
        strcat(pHttpPack->pRequestBody, "\r\n");
    }
    else
    {

    }

    if(pHttpPack->Head.pRange)
    {
        strcat(pHttpPack->pRequestBody, "Range: ");
        strcat(pHttpPack->pRequestBody, pHttpPack->Head.pRange);
        strcat(pHttpPack->pRequestBody, "\r\n");
    }
    else
    {

    }

    if(pHttpPack->Head.pReferer)
    {
        strcat(pHttpPack->pRequestBody, "Referer: ");
        strcat(pHttpPack->pRequestBody, pHttpPack->Head.pReferer);
        strcat(pHttpPack->pRequestBody, "\r\n");
    }
    else
    {

    }

    if(pHttpPack->Head.pUA_Color)
    {
        strcat(pHttpPack->pRequestBody, "UA-Color: ");
        strcat(pHttpPack->pRequestBody, pHttpPack->Head.pUA_Color);
        strcat(pHttpPack->pRequestBody, "\r\n");
    }
    else
    {

    }

    if(pHttpPack->Head.pUA_CPU)
    {
        strcat(pHttpPack->pRequestBody, "UA-CPU: ");
        strcat(pHttpPack->pRequestBody, pHttpPack->Head.pUA_CPU);
        strcat(pHttpPack->pRequestBody, "\r\n");
    }
    else
    {

    }

    if(pHttpPack->Head.pUA_OS)
    {
        strcat(pHttpPack->pRequestBody, "UA-OS: ");
        strcat(pHttpPack->pRequestBody, pHttpPack->Head.pUA_OS);
        strcat(pHttpPack->pRequestBody, "\r\n");
    }
    else
    {

    }

    if(pHttpPack->Head.pUA_Pixels)
    {
        strcat(pHttpPack->pRequestBody, "UA-Pixels: ");
        strcat(pHttpPack->pRequestBody, pHttpPack->Head.pUA_Pixels);
        strcat(pHttpPack->pRequestBody, "\r\n");
    }
    else
    {

    }

    if(pHttpPack->Head.pUserAgent)
    {
        strcat(pHttpPack->pRequestBody, "User-Agent: ");
        strcat(pHttpPack->pRequestBody, pHttpPack->Head.pUserAgent);
        strcat(pHttpPack->pRequestBody, "\r\n");
    }
    else
    {

    }

    if(pHttpPack->Head.pContentType)
    {
        strcat(pHttpPack->pRequestBody, "Content-Type: ");
        strcat(pHttpPack->pRequestBody, pHttpPack->Head.pContentType);
        strcat(pHttpPack->pRequestBody, "\r\n");
    }
    else
    {

    }

    strcat(pHttpPack->pRequestBody, "\r\n");

    if(pHttpPack->pRequestMessage)
    {
        strcat(pHttpPack->pRequestBody, pHttpPack->pRequestMessage);
    }
    else
    {

    }

    return strlen(pHttpPack->pRequestBody);

}
/*--------------------------------------
|   Function Name:
|       HttpUnpack
|   Description:
|       http����
|   Parameters:
|       pHttpUnpack: defined by the type of pHttpUnpack_t
|   Returns:
|       00:success 01 fail
+-------------------------------------*/
int HttpFrameUnpack(pHttpUnpack_t pHttpUnpack)
{
    if(pHttpUnpack->Line.pVersion)
    {
        HttpUnpackGetLineVersion(pHttpUnpack->pResponseBody, pHttpUnpack->Line.pVersion, 64);
    }
    else
    {

    }

    if(pHttpUnpack->Line.pStatus)
    {
        HttpUnpackGetLineStatus(pHttpUnpack->pResponseBody, pHttpUnpack->Line.pStatus, 64);
    }
    else
    {

    }

    if(pHttpUnpack->Line.pStatusDescription)
    {
        HttpUnpackGetLineStatusDescription(pHttpUnpack->pResponseBody, pHttpUnpack->Line.pStatusDescription, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pAccept)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "Accept", pHttpUnpack->Head.pAccept, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pAcceptCharset)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "Accept-Charset", pHttpUnpack->Head.pAcceptCharset, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pAcceptEncoding)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "Accept-Encoding", pHttpUnpack->Head.pAcceptEncoding, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pAcceptLanguage)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "Accept-Language", pHttpUnpack->Head.pAcceptLanguage, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pAuthorization)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "Authorization", pHttpUnpack->Head.pAuthorization, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pContentType)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "Content-Type", pHttpUnpack->Head.pContentType, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pContentEncoding)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "Content-Encoding", pHttpUnpack->Head.pContentEncoding, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pContentLength)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "Content-Length", pHttpUnpack->Head.pContentLength, 128);
    }
    else
    {

    }
    if(pHttpUnpack->Head.pTotalLength)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "Total-Length", pHttpUnpack->Head.pTotalLength, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pHash)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "HASH", pHttpUnpack->Head.pHash,64);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pHost)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "Host", pHttpUnpack->Head.pHost, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pIfModifiedSince)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "If-Modified-Since", pHttpUnpack->Head.pIfModifiedSince, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pReferer)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "Referer", pHttpUnpack->Head.pReferer, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pUserAgent)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "User-Agent", pHttpUnpack->Head.pUserAgent, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pCookie)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "Cookie", pHttpUnpack->Head.pCookie, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pPragma)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "Pragma", pHttpUnpack->Head.pPragma, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pFrom)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "From", pHttpUnpack->Head.pFrom, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pConnection)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "Connection", pHttpUnpack->Head.pConnection, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pRange)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "Range", pHttpUnpack->Head.pRange, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pUA_Pixels)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "UA-Pixels", pHttpUnpack->Head.pUA_Pixels, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pUA_Color)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "UA-Color", pHttpUnpack->Head.pUA_Color, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pUA_OS)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "UA-OS", pHttpUnpack->Head.pUA_OS, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pUA_CPU)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "UA-CPU", pHttpUnpack->Head.pUA_CPU, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pDate)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "Date", pHttpUnpack->Head.pDate, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pServer)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "Server", pHttpUnpack->Head.pServer, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pVary)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "Vary", pHttpUnpack->Head.pVary, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.pKeepAlive)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "Keep-Alive", pHttpUnpack->Head.pKeepAlive, 128);
    }
    else
    {

    }

    if(pHttpUnpack->Head.PDisposition)
    {
        HttpUnpackGetHeadParam(pHttpUnpack->pResponseBody, "Content-Disposition", pHttpUnpack->Head.PDisposition, 128);
    }
    else
    {

    }

    if(pHttpUnpack->pResponseParam)
    {
        HttpUnpackGetResponseParam(pHttpUnpack->pResponseBody, pHttpUnpack->pResponseParam, HTTP_RESPONSE_FRAME_LENGTH_MAX);
    }
    else
    {

    }

    return 0;
}


