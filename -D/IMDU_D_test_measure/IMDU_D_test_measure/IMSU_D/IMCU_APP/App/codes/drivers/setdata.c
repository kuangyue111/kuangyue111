#include "main.h"

/*获取CFG数据*/
static UCHAR get_setdata(UINT16 nID, void *pDataBuf)
{
	if(nID > CHAR_TYPE + FLOAT_TYPE - 1)        //error
	{
		return  FALSE;
	}
	MutexLock(gSetDataInfo.hMutexReadWriteSetData);
	if(nID > CHAR_TYPE - 1)                //float
	{
		*((float *)pDataBuf) = gSetDataInfo.SetData.ArrayData.CfgFloatData[nID - CHAR_TYPE];
	}
	else                                        //char
	{
		*((unsigned char *)pDataBuf) = gSetDataInfo.SetData.ArrayData.CfgCharData[nID];
	}
	MutexUnlock(gSetDataInfo.hMutexReadWriteSetData);
	return  TRUE;
}

/*设置CFG数据*/
static UCHAR set_setdata(UINT16 nID, void *pDataBuf)
{
	if(nID > CHAR_TYPE + FLOAT_TYPE - 1)        //error
	{
		return  FALSE;
	}
	MutexLock(gSetDataInfo.hMutexReadWriteSetData);
	if(nID > CHAR_TYPE - 1)                //float
	{
		gSetDataInfo.SetData.ArrayData.CfgFloatData[nID - CHAR_TYPE] = *((float *)pDataBuf);

		gSelfTest.fLastSetDataValue = (UCHAR)(*((float *)pDataBuf));
	}
	else                                        //char
	{
		gSetDataInfo.SetData.ArrayData.CfgCharData[nID] = *((unsigned char *)pDataBuf);

		gSelfTest.fLastSetDataValue = (*((unsigned char *)pDataBuf));
	}
	//save to eeprom	
	gSelfTest.LastSetDataID = nID;
	gs_E2PROM.fSetSetDataUpdate();
	MutexUnlock(gSetDataInfo.hMutexReadWriteSetData);
	return  TRUE;
}

/*设置CFG数据*/
static UCHAR set_setdata1(UINT16 nID, void *pDataBuf)
{
	if(nID > CHAR_TYPE + FLOAT_TYPE - 1)        //error
	{
		return  FALSE;
	}
	MutexLock(gSetDataInfo.hMutexReadWriteSetData);
	if(nID > CHAR_TYPE - 1)                //float
	{
		gSetDataInfo.SetData.ArrayData.CfgFloatData[nID - CHAR_TYPE] = *((float *)pDataBuf);
	}
	else                                        //char
	{
		gSetDataInfo.SetData.ArrayData.CfgCharData[nID] = *((unsigned char *)pDataBuf);
	}
	//not save to eeprom
	//gs_E2PROM.fSetSetDataUpdate();
	MutexUnlock(gSetDataInfo.hMutexReadWriteSetData);
	return  TRUE;
}

static UCHAR get_switchname(UINT16 nID, void *pDataBuf)
{

  return  TRUE;
}

static UCHAR init_mutex(void)
{
  gSetDataInfo.hMutexReadWriteSetData = MutexCreate(UPPER_PRIO_READ_WRITE_SETDATA);
  if(gSetDataInfo.hMutexReadWriteSetData == NULL)
  {
    return FALSE;
  }
	return  TRUE;
}

//取配置数据
static UCHAR get_defaultsetdata(UINT16 nID, void *pDataBuf)
{
	if(nID > CHAR_TYPE + FLOAT_TYPE - 1)        //error
	{
		return  FALSE;
	}

	if(nID > CHAR_TYPE - 1)                     //float
	{
		*((float *)pDataBuf) = dv_E2PROM.fReadE2PROMFloat(nID - CHAR_TYPE);
	}
	else //char
	{
		*((UCHAR *)pDataBuf) = dv_E2PROM.fReadE2PROM(E2PROMDEFAULTCharData + nID);
	}

	return  TRUE;
}

//设置配置数据
static UCHAR set_defaultsetdata(UINT16 nID, void *pDataBuf)
{
	if(nID > CHAR_TYPE + FLOAT_TYPE - 1)        //error
	{
		return  FALSE;
	}

	if(nID > CHAR_TYPE - 1)                     //float
	{
		dv_E2PROM.fWriteE2PROMFloat(nID - CHAR_TYPE, *((float *)pDataBuf));
	}
	else                                        //char
	{
		dv_E2PROM.fWriteE2PROM(E2PROMDEFAULTCharData + nID, *((UCHAR *)pDataBuf));
	}

	return  TRUE;
}

/*下载配置数据*/
static UCHAR dl_setdata(UINT16 nID, void *pDataBuf)
{
	UINT16  i;
	unsigned char *pChar;
	float * pFloat;
	float ftemp;
	UCHAR ucdata[4];
	UCHAR ucdata1[4];

	pChar = (unsigned char *)pDataBuf;
	pFloat = (float *)((unsigned char *)pDataBuf + CHAR_TYPE);

	MutexLock(gSetDataInfo.hMutexReadWriteSetData);

	for (i = 0; i < CHAR_TYPE; i++)//char
	{
		gSetDataInfo.SetData.ArrayData.CfgCharData[i] = *pChar;
		pChar++;
	}

	for (i = 0; i < FLOAT_TYPE; i++)//float
	{
		gSetDataInfo.SetData.ArrayData.CfgFloatData[i] = *pFloat;
		pFloat++;
	}

	/*pChar = (float *)((unsigned char *)pDataBuf + SETDATA_ucSetDataVersion_H_NUM);
	ftemp = *pFloat;
	
	//写配置数据文件版本号
	floattoascii(ftemp, ucdata);
	ucdata1[3] = ucdata[0];
	ucdata1[2] = ucdata[1];
	ucdata1[1] = ucdata[2];
	ucdata1[0] = ucdata[3];
	dv_E2PROM.fWriteE2PROMPage(ucdata1, E2PROMCharData + 4096 + SETDATA_SetDataVersion_NUM * 4, 4);
	dv_E2PROM.fWriteE2PROMPage(ucdata1, E2PROMCharData1 + 4096 + SETDATA_SetDataVersion_NUM * 4, 4);
	dv_E2PROM.fWriteE2PROMPage(ucdata1, E2PROMCharData2 + 4096 + SETDATA_SetDataVersion_NUM * 4, 4);
	
	if(ftemp < 1.008)
	{
		;		
	}
	//save to eeprom*/
	gs_E2PROM.fSetSetDataUpdate();
	MutexUnlock(gSetDataInfo.hMutexReadWriteSetData);
	return  TRUE;
}

SetDataExchangeInterface_TypeDef gSetDataInfo;

//初始化
void init_gs_SetData(void)
{
	gSetDataInfo.hMutexReadWriteSetData = NULL;

	gSetDataInfo.fInitMutex = init_mutex;
	gSetDataInfo.fGetSwitchName = get_switchname;
	gSetDataInfo.fGetSetData = get_setdata;
	gSetDataInfo.fSetSetData = set_setdata;
	gSetDataInfo.fSetSetData1 = set_setdata1;
	gSetDataInfo.fGetDefaultSetData = get_defaultsetdata;
	gSetDataInfo.fSetDefaultSetData = set_defaultsetdata;
	gSetDataInfo.fDownLoadSetData = dl_setdata;
}
