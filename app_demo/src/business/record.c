#include "appinc.h"

ST_WATER_BRIEF stWaterList[100] = {0};
TransactionData stTransactionRecord = {0};

static int limit_record_time = 0;
static int limit_record_type = 0xFF;

time_t getSecondCurDay()
{
	time_t ret = 0;
	ST_TIME stCurDay = {0};
	struct tm tmCurDay = {0};
	OsGetTime(&stCurDay);
	tmCurDay.tm_year = stCurDay.Year-1900;
	tmCurDay.tm_mon = stCurDay.Month-1;
	tmCurDay.tm_mday = stCurDay.Day;
	tmCurDay.tm_hour = 23;
	tmCurDay.tm_min = 59;
	tmCurDay.tm_sec = 59;
	ret = mktime(&tmCurDay);
	OsLog(LOG_DEBUG,"%d-%d=%d:getSecondDay ! ", stCurDay.Year,stCurDay.Month,stCurDay.Day);
	OsLog(LOG_DEBUG,"%ld:getSecondCurDay ! ", ret);
	return ret;
}

time_t getSecondDay(int year,int month,int day)
{
	time_t ret = 0;
	struct tm tmCurDay = {0};
	tmCurDay.tm_year = year-1900;
	tmCurDay.tm_mon = month-1;
	tmCurDay.tm_mday = day;
	tmCurDay.tm_hour = 23;
	tmCurDay.tm_min = 59;
	tmCurDay.tm_sec = 59;
	ret = mktime(&tmCurDay);
	OsLog(LOG_DEBUG,"%d-%d=%d:getSecondDay ! ", year,month,day);
	OsLog(LOG_DEBUG,"%ld:getSecondDay ! ", ret);
	return ret;
}

static char TransactionGetPaymenOrg(char* szpan)
{
    // find org from card bin list, this is just a simple example
    if (0 == strncmp(szpan,"4",1))
    {
        return 0;
    }
    else if (0 == strncmp(szpan,"5",1))
    {
        return 1;
    }
    else if (0 == strncmp(szpan,"62",2))
    {
        return 2;
    }
    else if (0 == strncmp(szpan,"35",2))
    {
        return 3;
    }
    else if (0 == strncmp(szpan,"34",2) || 0 == strncmp(szpan,"37",2))
    {
        return 4;
    }
    else if (0 == strncmp(szpan,"30",2) || 0 == strncmp(szpan,"36",2))
    {
        return 5;
    }
    else
    {
        return 0;
    }
    return 0;
}

static void TransactionDataToWaterBrief(TransactionData* txn_data, ST_WATER_BRIEF* water_brief)
{
    int int_amt = 0;
    int int_time[6] = {0};  //[0]-yy,[1]-mm,....
    char time_buf[12] = {0};

    int_amt = atoi(txn_data->sAmount);
	OsLog(LOG_ERROR,"txn_data->sTransTime = %s\n", txn_data->sTransTime);
    for (size_t i = 0; i < 6; i++)
    {
        int_time[i] = (txn_data->sTransTime[2+i*2]-'0')*10 +  txn_data->sTransTime[3+i*2]-'0';
    }
    //ordernum no. //use orderbum for key
    strcpy(water_brief->sOrderNo,txn_data->sOrderNo);
    //year
    sprintf(water_brief->sYear,"%02d/%02d/%02d",int_time[1],int_time[2],int_time[0]);
    //amount
    sprintf(water_brief->sAmt,"$%d.%02d",int_amt/100,int_amt%100);
    //describe
    if (strlen(txn_data->sCardNo)>4)        //card transaction
    {
        strcat(water_brief->szTransDesc,txn_data->sCardNo+strlen(txn_data->sCardNo)-4);
        strcat(water_brief->szTransDesc,"-");
    }
    
    strcat(water_brief->szTransDesc,water_brief->sOrderNo);
    sprintf(time_buf,"-%02d:%02d",int_time[3],int_time[4]);
    strcat(water_brief->szTransDesc,time_buf);
    //trans name
    water_brief->szTransName[0] = txn_data->nTransType;
    //payment org
    if (txn_data->nPosEntryMode == INPUT_QRCODE || txn_data->nPosEntryMode == INPUT_SCANCODE)
    {
        water_brief->szPaymentOrg = 6;
    }
    else
    {
        water_brief->szPaymentOrg = TransactionGetPaymenOrg(txn_data->sCardNo);
    }
    
    return;
}

void TranRecord_set_time_limit(int second)
{
    limit_record_time = second;
    return;
}

void TranRecord_set_type_limit(int type)
{
    limit_record_type = type;
    return;
}

void TranRecord_ReadTradeTotal(void)
{
    int record_total_num = 0;
    int record_effect_num = 0;
    TransactionData data_buf = {0};

    record_total_num = TranRecord_GetTradeCount();
    memset(stWaterList,0x00,sizeof(stWaterList));
    for (int i = record_total_num-1; i >= 0; i--)    // Load in reverse order
    {
        memset(&data_buf,0x00,sizeof(data_buf));
        memset(&stWaterList[record_effect_num],0x00,sizeof(ST_WATER_BRIEF));
        TranRecord_ReadTradeByIndex(i,&data_buf);
        if (strlen(data_buf.sTrace) <= 0)
        {
            continue;
        }
        else
        {
            if (limit_record_time != 0) //timer limit
            {
                char strYear[5] = {0};
                char strMonth[3] = {0};
                char strDay[3] = {0};
	            OsLog(LOG_DEBUG,"%ld :limit_record_time ! ", limit_record_time);
                memcpy(strYear,data_buf.sTransTime,4);
                memcpy(strMonth,data_buf.sTransTime+4,2);
                memcpy(strDay,data_buf.sTransTime+6,2);
                if(limit_record_time<=(getSecondCurDay()-getSecondDay(atoi(strYear),atoi(strMonth),atoi(strDay))))
                    continue;
            }
            TransactionDataToWaterBrief(&data_buf,&stWaterList[record_effect_num]);
            record_effect_num++;
        }
    }
    return;
}
