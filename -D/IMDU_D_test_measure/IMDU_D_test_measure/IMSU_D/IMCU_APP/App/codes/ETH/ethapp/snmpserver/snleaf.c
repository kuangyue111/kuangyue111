/*
 *****************************************************************************
 *                                                                           *
 *  Filename:   snleaf.c                                                   *
 *  Date:       April 1, 2008                                                  *
 *  Description:net comm                                                     *
 *  Version:    V1.00                                                        *
 *  Hardware:                                                                *
 *  Remarks:    For NGC application                                          *
 *                                                                           *
 *****************************************************************************
 */
#include "kernel.h"
//#include "network.h"
//#include mem.h>

//#ifdef	SNMP

#include "snmp.h"
#include "mib.h"
#include "asn1.h"
#include "main.h"
//caili
//extern OS_MEM  *p_MEM;
/*
extern char  temp_malloc_char[10][50];
extern int temp_i_malloc_char;
*/
/*------------------------------------------------------------------------
 * snleaf - perform the requested operation on the leaf SNMP variable
 *------------------------------------------------------------------------
 */
const char s1[MAX_SEND]="InfyPower";


int
snleaf(struct snbentry *bindl,  struct mib_info *mip, int op)
{
//	int 		len;
//	char		*strp;
//	struct oid	*oip;
//		
//	long lpData;
//	char s_get[MAX_SEND];
//
//	if (op == SOP_GETN) {
//		if (mip->mi_next)
//			return((*mip->mi_next->mi_func)
//				(bindl, mip->mi_next, SOP_GETF));
//		return SERR_NO_SUCH;
//	}
//	
//	if (op == SOP_SET) {
//		if (! mip->mi_writable)
//			return SERR_NO_SUCH;
//		switch(mip->mi_vartype) {
//		case ASN1_INT:
//			if (SVTYPE(bindl) != ASN1_INT)
//				return SERR_BAD_VALUE;
//			if (mip->mi_param == 0)
//				return SERR_NO_SUCH;
//			*((int *) mip->mi_param) = SVINT(bindl);
//			break;
//		case ASN1_OCTSTR:
//			if (SVTYPE(bindl) != ASN1_OCTSTR)
//				return SERR_BAD_VALUE;
//                        if(SVSTRLEN(bindl)>32)//31) wzp08.11.12
//                        {
//                            return  SNMP_OK;
//                        }
//                        else
//                        {
//				UCHAR U_temp;
//				int i=0;
//                                char strp1[33];
//                                 memcpy(strp1, SVSTR(bindl), SVSTRLEN(bindl));
//			        *(strp1 + SVSTRLEN(bindl)) = '\0';
//				int k=SVSTRLEN(bindl);
//                                for(i=0;i< k;i++)
//			        {
//			            U_temp=(UCHAR)strp1[i];
// 			            SetCharSetData(SETDATA_SiteName_NUM+i, &U_temp);
// 				}
//		        	U_temp='\0';
//			        SetCharSetData(SETDATA_SiteName_NUM+i, &U_temp);
//                        }
//			/*
//			strp = (char *) mip->mi_param;
//                           char strp1[20];
//                                  memcpy(strp1, SVSTR(bindl), SVSTRLEN(bindl));
//			*(strp1 + SVSTRLEN(bindl)) = '\0';*/
//			break;
//		case ASN1_OBJID:
//			if (SVTYPE(bindl) != ASN1_OBJID)
//				return SERR_BAD_VALUE;
//			oip = (struct oid *) mip->mi_param;
//			oip->len = SVSTRLEN(bindl);
//			memcpy(oip->id, SVSTR(bindl), oip->len * 2);
//			break;
//		}
//		return SNMP_OK;
//	}
//
//  
//
//	
//	if (op == SOP_GETF) {
//		
//		bindl->sb_oid.len = mip->mi_objid.len;
//		memcpy(bindl->sb_oid.id, mip->mi_objid.id,
//			mip->mi_objid.len * 2);
//	}
//	
//
//	
//
//	GetDataByNodeID(mip->i_Node, &lpData, s_get);
//
//	SVTYPE(bindl) = mip->mi_vartype;
//
//	switch(mip->mi_vartype) {
//	case ASN1_INT:
//	case ASN1_TIMETICKS:
//	case ASN1_GAUGE:
//	case ASN1_COUNTER:
//
//		SVINT(bindl) = (int)lpData;
//		break;
//	case ASN1_OCTSTR:
//		strp = s_get;
//		if (strp == NULL) {
//			SVSTRLEN(bindl) = 0;
//			SVSTR(bindl) = NULL;
//			break;
//		}
//		len = SVSTRLEN(bindl) = strlen(s_get);
//                //caili
//	//	SVSTR(bindl) = (char *) OSMemGet(p_MEM,&err);
//                //SVSTR(bindl) = (char *) malloc(100);
//
//memset(& gs_trap.temp_malloc_char[gs_trap.temp_i_malloc_char][0],0,sizeof(gs_trap.temp_malloc_char[gs_trap.temp_i_malloc_char][0]));
//               	SVSTR(bindl) = (char *)(& gs_trap.temp_malloc_char[gs_trap.temp_i_malloc_char][0]);
//									gs_trap.temp_i_malloc_char++;
//		memcpy(SVSTR(bindl), s_get, len);
//		break;
//	case ASN1_OBJID:
//		oip = (struct oid *) mip->mi_param;
//		SVOIDLEN(bindl) = oip->len;
//		memcpy(SVOID(bindl), oip->id, oip->len * 2);
//		break;
//	}
	return SNMP_OK;
}
//#endif	/* SNMP */




 int GetDataByNodeID(int nNodeID, long* lpData, char* string)
 {
 //	*lpData = 0;
 //	*string = 0;
 //       float ftemp=0;
	//float	fsum=0;
	//unsigned char U_temp=0;
	//int i = 0;
	//int j = 0;
 //     UCHAR  site_name[34];
 //     UCHAR hw[10];
	//  //Modified by hulw 090628 For 1phase system, phaseA Volt = phase B = phase C
	//  UCHAR ucacintype = m3PHASE;
 //	switch(nNodeID)
	//
 //       
 //       {       
 //       case IDENTMANUFACTURER:          

	//	memcpy(string,s1,MAX_SEND);
 //         
	//	break;

	//
	//	case IDENTMODEL:
	//		memset(site_name,0,sizeof(site_name));
	//		for(i = 0, j = 0; i<= 15; i++)
	//		{
	//			GetCharSetData(SETDATA_OFF_LINE_PSWD+12+i, &U_temp);

	//			if(U_temp != 0x20 && U_temp != 0 && U_temp != 0xFF)
	//			{
	//				site_name[j] = U_temp;
	//				j++;
	//			}
	//		}
	//		site_name[j]='\0';

	//		memcpy(string, site_name, j + 1);

	//		break;

	//
 //       case IDENTCONTROLLERFIRMWAREVERSION:
	//       GetFloatSetData(SETDATA_SoftwareVersion_NUM , &ftemp);
 //              memset(hw,0,sizeof(hw));
 //              sprintf((char *)hw,"%3.2f",ftemp);
 //              hw[4] = '\0';
 //       	memcpy(string,hw,5);
	//        
	//	break;


	//	case IDENTNAME:
	//		memset(site_name,0,sizeof(site_name));
	//		for(i=0;i<33;i++)
	//		{
	//			GetCharSetData(SETDATA_SiteName_NUM+i, &U_temp);
	//			site_name[i]=U_temp;
	//		}
	//		site_name[33]='\0';
	//		memcpy(string,site_name,34);

	//		break;
	//	case SYSSTATUE:
	//		U_temp = 0;//gs_AlarmMng.fGetCriticalAlarmNum();			

	//		if(U_temp > 0)
	//		{
	//			*lpData = 6;		
	//		}
	//		else
	//		{
	//			U_temp = 0;//gs_AlarmMng.fGetMajorAlarmNum();

	//			if(U_temp > 0)
	//			{
	//				*lpData = 5;
	//			}
	//			else
	//			{
	//				U_temp = 0;//gs_AlarmMng.fGetObservationAlarmNum();

	//				if(U_temp > 0)
	//				{
	//					*lpData = 3;
	//				}
	//				else
	//				{
	//					*lpData = 2;
	//				}
	//			}
	//		}

	//		break;

	//case SYSVOLTAGE:
 //  		 //gMeasureInfo.fGetMeasureAnalogData(DCV_NUM,&ftemp);
	//	*lpData=(long)(ftemp*1000);
	//	break;
	//case SYSCURRENT:
	//	gMeasureInfo.fGetMeasureAnalogData(LOADCURR_NUM,&ftemp);
	//	*lpData=(long)(ftemp*1000);
	//	break;
	//case SYSUSEDCAPACITY:
	//	//ftemp = gs_CANComm.RectPower;
	//	//ftemp = gs_CANComm.fGetRectAnalog(RECT_POWER_VALUE, 0);
	//	*lpData=(long)ftemp;
	//	break;
	//case PSBATTERYVOLTAGE:
	//		//gMeasureInfo.fGetMeasureAnalogData(BATTV_NUM,&ftemp);
	//	*lpData=(long)(ftemp*1000);
	//	break;
	//case PSBATTERYCURRENT:
	//	//gMeasureInfo.fGetMeasureAnalogData(BATT1CURR_NUM,&ftemp);
	//	fsum=ftemp;
	//	//gMeasureInfo.fGetMeasureAnalogData(BATT2CURR_NUM,&ftemp);
	//	fsum += ftemp;
	//	*lpData=(long)(fsum*1000);
	//	break;
	//case PSINPUTLINEAVOLTAGE:

 //           gMeasureInfo.fGetMeasureAnalogData(AC1VA_NUM,&ftemp);
	//	*lpData=(long)(ftemp*1000);
	//	break;
	//case PSINPUTLINEBVOLTAGE:
	//	//Modified by hulw 090628 For 1phase system, phaseA Volt = phase B = phase C
	//	GetCharSetData(SETDATA_ACinType_NUM, &ucacintype);
	//	if (ucacintype == m3PHASE)		/*三相*/
	//	{			
	//		gMeasureInfo.fGetMeasureAnalogData(AC1VB_NUM,&ftemp);
	//	}
	//	else
	//	{
	//		gMeasureInfo.fGetMeasureAnalogData(AC1VA_NUM,&ftemp);
	//	}

	//	*lpData=(long)(ftemp*1000);
	//	break;
	//
	//case PSINPUTLINECVOLTAGE:
	//	//Modified by hulw 090628 For 1phase system, phaseA Volt = phase B = phase C
	//	GetCharSetData(SETDATA_ACinType_NUM, &ucacintype);
	//	if (ucacintype == m3PHASE)		/*三相*/
	//	{			
	//		gMeasureInfo.fGetMeasureAnalogData(AC1VC_NUM,&ftemp);
	//	}
	//	else
	//	{
	//		gMeasureInfo.fGetMeasureAnalogData(AC1VA_NUM,&ftemp);
	//	}
	//	*lpData=(long)(ftemp*1000);
	//	break;
	//	
	//case PSTEMPERATURE1:
	//		gMeasureInfo.fGetMeasureAnalogData(TEMP1_NUM,&ftemp);
	//	*lpData=(long)(ftemp*1000);
	//	break;
	//case PSTEMPERATURE2:
	//		gMeasureInfo.fGetMeasureAnalogData(TEMP2_NUM,&ftemp);
	//	*lpData=(long)(ftemp*1000);
	//	break;
	//case PSSTATUSCOMMUNICATION:
	//	*lpData=2;//normal
	//	for(int i=0;i<30;i++)
	//	{
	//	    //gs_AlarmMng.fGetAlarmTableAlarmStatus(ALARMTABLE_RECTCOMMBREAK+i, &U_temp);
 //           U_temp = 0;
	//	 	if(U_temp)
	//	 	{
 //             *lpData=3;//interrupt
 //               break;
 //           }
	//	}		
	//    break;
	//case PSSTATUSBATTERYMODE:
	//	 U_temp = 0;//gs_BattMng.BattMngProc.fGetECFlag();
	//	 if(U_temp <= 2)//float/float(temp)/need BC
	//	 {
	//	 	*lpData=2;      //floatcharge
	//	 }
 //        else if(U_temp==3) //BC
	//	 {
	//	 	GetCharSetData(SETDATA_CtlMode_NUM, &U_temp);
	//		if(U_temp==mMANUAL)
	//		{
	//			*lpData=9;//ManualBC
	//		}
	//		else
	//		{
	//			*lpData=10;//AutoBC
	//		}
	//	 }
 //        else if(U_temp==4)//cylibc
	//	 {
	//	 	*lpData=11;
	//	 }
 //        else if(U_temp==5)
	//	 {
	//	     GetCharSetData(SETDATA_CtlMode_NUM, &U_temp);
	//	     if(U_temp==mMANUAL)
	//		 {
	//		       *lpData=5;
	//			   break;
	//		  }
	//		  else
	//		  {
 //                  *lpData=6;
 //                  break;
	//		  }
	//	 }
 //        else if(U_temp == 6)//Concurr BT
 //        {
 //           GetCharSetData(SETDATA_CtlMode_NUM, &U_temp);
	//		if(U_temp==mMANUAL)
	//		{
	//			*lpData=5;//Manual BT
	//		}
	//		else
	//		{
	//			*lpData=6;//Plan BT
	//		}
 //        }
 //        else if(U_temp==7)//shorttest
	//	 {
	//	 	*lpData=3;	
	//	 }
 //        else
 //        {
	//	 	*lpData=2;      //floatcharge
	//	 }
 //        break;
 //        
 //        /*caili*/
	///*	 if(U_temp==7)//shorttest
	//	 {
	//	 	*lpData=3;
	//		break;
	//		
	//	 }
	//	  if(U_temp==4)//cylibc
	//	 {
	//	 	*lpData=11;
	//		break;
	//	 }
	//	 if(U_temp==3)
	//	 {
	//	 	GetCharSetData(SETDATA_CtlMode_NUM, &U_temp);
	//		if(U_temp==mMANUAL)
	//		{
	//			*lpData=9;//ManualBC
	//			break;
	//		}
	//		else
	//		{
	//			*lpData=10;//AutoBC
	//			break;
	//		}
	//	  }
	//	  if(U_temp==5)
	//	  {
	//	        GetCharSetData(SETDATA_CtlMode_NUM, &U_temp);
	//	        if(U_temp==mMANUAL)
	//		    {
	//			    *lpData=5;
	//			    break;
	//		    }
	//		    else
	//			{
 //                   *lpData=6;
 //                   break;
	//			}
	//	  }*/
	//case PSSMACNUMBER:
	//	*lpData=0;
	//	break;
	//case PSSMBATNUMBER:
	//	*lpData=0;
	//        break;
	//case PSSMIONUMBER:
	//	*lpData=0;
	//	break;
	//case ALARMLASTTRAPNO:
	//	{
	//		//Modified by hulw 090429 to support Trap alarm table
	//		extern unsigned long  Trap_TrapNum;
	//		*lpData= Trap_TrapNum;
	//	}
	//	break;
	//}
    return ERR_SNP_OK;
}
int snalarmleaf(struct snbentry *bindl,  struct mib_info *mip, int op)
{

#ifdef SUPPORT_TRAP_TABLE

	//int 		len;

	//char s_get[MAX_SEND];

	static int nAlarmSN = 0;
	int nAlarmNum = 0;
	//UINT16 ui16AlarmIndex;
	//int nStrLen;
        nAlarmSN = nAlarmSN;
	nAlarmNum = 0;//gs_AlarmMng.fGetActiveAlarmNum();//get the active alarm num

	if(nAlarmNum == 0)
	{
		return SERR_NO_SUCH;
	}
	

	if (op == SOP_GETN) {
		//Modified by hulw 090516 to support SNMP TABLE by Get cmd
		if (bindl->sb_oid.len == mip->mi_objid.len - 1)
		{
			op = SOP_GETF;

		}
		else if (bindl->sb_oid.len == mip->mi_objid.len)
		{
			if (bindl->sb_oid.id[bindl->sb_oid.len - 1] < nAlarmNum)
			{
				bindl->sb_oid.id[bindl->sb_oid.len - 1]++;
				op = SOP_GET;
			}
			else
			{
				if (mip->mi_next)
				return((*mip->mi_next->mi_func)
				(bindl, mip->mi_next, SOP_GETF));

				return SERR_NO_SUCH;
			}

		}
		else
		{
			return SERR_NO_SUCH;
		}
	}

	if (op == SOP_GETF) {

		nAlarmSN = 0;
          
		bindl->sb_oid.len = mip->mi_objid.len;
		memcpy(bindl->sb_oid.id, mip->mi_objid.id,
			mip->mi_objid.len * 2);
	}

	if (op == SOP_GET) {

		nAlarmSN = bindl->sb_oid.id[bindl->sb_oid.len - 1] - 1;

	}

	SVTYPE(bindl) = mip->mi_vartype;

	/*if (gs_AlarmMng.fGetAlarmTableIndex(nAlarmSN, &ui16AlarmIndex) == TRUE)
	{
		AlarmToSNMPTrap_TypeDef stTrap;
		if(gs_AlarmMng.fGetSNMPTrap(ui16AlarmIndex, &stTrap, TRUE) == TRUE)
		{
			switch(mip->i_Node) {
                case ALARMTRAPNO:
		{
			SVINT(bindl) = 0;//Don't support, only support when trap send

			break;

		}
	case ALARMSTATUSCHANGE:
		{
			SVINT(bindl) = (int)stTrap.TrapStatus;

			break;

		}
	case ALARMSEVERITY:
		{
			SVINT(bindl) = (int)stTrap.TrapType;

			break;

		}
	case ALARMTYPE:
		{
			SVINT(bindl) = (int)stTrap.AlarmNum;

			break;

		}
	default:
          {
          if(mip->i_Node == ALARMTIME)
          {
            memcpy(s_get,stTrap.TrapTime,sizeof(stTrap.TrapTime));
			nStrLen = sizeof(stTrap.TrapTime);
          }
          else if(mip->i_Node == ALARMDESCRIPTION)
          {
            memcpy(s_get,stTrap.TrapName,sizeof(stTrap.TrapName));
			nStrLen = strlen((char const*)(stTrap.TrapName));
          }
          else
          {
            return SERR_NO_SUCH;
          }
          len = SVSTRLEN(bindl) = nStrLen;
		//caili
		//	SVSTR(bindl) = (char *) OSMemGet(p_MEM,&err);
		//SVSTR(bindl) = (char *) malloc(100);

		memset(& gs_trap.temp_malloc_char[gs_trap.temp_i_malloc_char][0],0,sizeof(gs_trap.temp_malloc_char[gs_trap.temp_i_malloc_char][0]));
		SVSTR(bindl) = (char *)(& gs_trap.temp_malloc_char[gs_trap.temp_i_malloc_char][0]);
		gs_trap.temp_i_malloc_char++;

		memcpy(SVSTR(bindl), s_get, len);
                
                break;
          }
                
		

			}

			return SNMP_OK;
		}
	}*/

#endif//SUPPORT_TRAP_TABLE
        
	return SERR_NO_SUCH;
}
//End Modified 
