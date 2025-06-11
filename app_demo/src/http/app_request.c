
#include "app_request.h"
#include "app_server.h"
#include "app_http.h"
#include "cJSON.h"

ONLINE_STATUS onlineStatus;

static const int TAG_8A_LENGTH = 4;
static char* RC_ONLINE_APPROVE = "\x8A\x02\x30\x30";
static char* RC_ONLINE_DECLINE = "\x8A\x02\x30\x35";

int setSaleJsonBody(pu8 pbody)
{
    u32 length = 0,i = 0,tmpLen = 0;
    int ret = 0;
    pu8 tempBuffer = NULL;
    pu8 tempBuffer2 = NULL;
    pu8 tempBuffer3 = NULL;
    cJSON *jsonbody = NULL;
    tempBuffer = malloc(512);
    memset(tempBuffer,0,512);
    tempBuffer2 = malloc(512);
    memset(tempBuffer2,0,512);
    tempBuffer3 = malloc(512);
    memset(tempBuffer3,0,512);
    pu8 pNode = NULL;

    do
    {
        jsonbody = cJSON_CreateObject();
        if(!jsonbody)
        {
           length = 0;
           break;
        }
        if(get_transaction_data()->icc_type != INPUT_STRIPE)
        {
            if(Emv_GetPanByTag5A(tempBuffer) == 0)
            {
                memcpy(get_transaction_data()->sCardNo,tempBuffer,strlen(tempBuffer));
                cJSON_AddItemToObject(jsonbody, "pan", cJSON_CreateString(tempBuffer));
            }

            memset(tempBuffer,0,512);
            length = 0;
            
            Pack_EmvData(tempBuffer,&length);
            cJSON_AddItemToObject(jsonbody, "emvdata", cJSON_CreateString(tempBuffer));
        }
        else
        {
            cJSON_AddItemToObject(jsonbody, "pan", cJSON_CreateString(get_transaction_data()->sCardNo));

            //encrypt track2    
            memset(tempBuffer,0,512);
            memset(tempBuffer2,0,512);
            memset(tempBuffer3,0,512);
            ret = DataEncrypt(PED_TRK_IPEK_INDEX,get_transaction_data()->sTracker2,get_transaction_data()->nTracker2Len,tempBuffer,tempBuffer2);
            if(ret)
            {
                tmpLen = nBcd2Asc(tempBuffer,ret*2,tempBuffer3,0);
                cJSON_AddItemToObject(jsonbody, "enTrack2", cJSON_CreateString(tempBuffer3));
                memset(tempBuffer3,0,512);
                tmpLen = nBcd2Asc(tempBuffer2,20,tempBuffer3,0);
                cJSON_AddItemToObject(jsonbody, "trkKsn", cJSON_CreateString(tempBuffer3));
            }
            
        }

        memset(tempBuffer,0,512);
        if(get_transaction_data()->emv_emter_online_pin_result == 1)
        {
            tmpLen = nBcd2Asc(get_transaction_data()->sPin,16, tempBuffer,0);
            cJSON_AddItemToObject(jsonbody, "pinblock", cJSON_CreateString(tempBuffer));
            memset(tempBuffer,0,512);
            tmpLen = nBcd2Asc(get_transaction_data()->sPinKsn,20, tempBuffer,0);
            cJSON_AddItemToObject(jsonbody, "pinksn", cJSON_CreateString(tempBuffer));
        }        

        memcpy(pbody,cJSON_Print(jsonbody),strlen(cJSON_Print(jsonbody)));
        length = strlen(pbody);
    
        /* code */
    } while (0);
    
    if(jsonbody)
    {
        cJSON_Delete(jsonbody);
        jsonbody = NULL;
    }

   if(tempBuffer)
   {
    free(tempBuffer);
    tempBuffer = NULL;
   }
   free(tempBuffer2);
   free(tempBuffer3);
    return length;
}

int sale_online_request(EmvOnlineData_t* pOnlineData)
{
    int ret = PR_FAILD;
    pu8 body=NULL;
    pu8 pSend=NULL;
    pu8 pRecv=NULL;
    s32    RecvLen=0;
    u32    HeadLen=0;
    u32    bodyLen = 0;
    u8 url[128] = {0};
    u32 urlLen = 0;

    body=(pu8)malloc(SERVER_REQUEST_BODY_LENGTH);
    pSend=(pu8)malloc(SERVER_REQUEST_LENGTH);
    memset(body,0,SERVER_REQUEST_BODY_LENGTH);
    memset(pSend,0,SERVER_REQUEST_LENGTH);

    bodyLen = setSaleJsonBody(body);
    OsLog(LOG_DEBUG,"body[%d] \n %s",bodyLen,body);

    RequestPackHttpFrame(body,URL_TEST,"POST",pSend,NULL,SERVER_REQUEST_LENGTH);
    

    pRecv=(pu8)malloc(SERVER_RECV_LENGTH);
    memset(pRecv,0,SERVER_RECV_LENGTH);
    RecvLen=RequestHttpTransmit(pSend,strlen(pSend),URL_TEST,URL_SERVER_PORT,pRecv,SERVER_RECV_LENGTH);
    if(RecvLen>0)
    {
        #ifdef CFG_DBG
        OsLog(LOG_DEBUG,"request success\r\n");
        #endif
        HeadLen=GetHttpHeadFramLen((char*)pRecv);
        T_U8_VIEW Http_head={pRecv,HeadLen};
        T_U8_VIEW Http_Response={pRecv+HeadLen,RecvLen-HeadLen};

        #if 1              // just a demo
            memcpy(pOnlineData->iccResponse,"00",2); //responcess
            //Analyze the EMV data returned by the server, which should include 91tag or 71 or 72tag
            //Convert data to bcd format
            //example 91081111111111111111111111 
            // nAsc2Bcd("91081111111111111111",10,pOnlineData->ackdata,0);
            // pOnlineData->ackdatalen = strlen("91081111111111111111")/2;
    
            ret = RET_OK;
            goto exit;
        #endif
    }
    else
    {
        // setEmvResponse(pTradingFile,RC_ONLINE_DECLINE);
        ret = RecvLen;
    }
    
exit:    
    free(pSend);
    free(pRecv);
    free(body);
    onlineStatus.Rc = ret;
    return ret;
}