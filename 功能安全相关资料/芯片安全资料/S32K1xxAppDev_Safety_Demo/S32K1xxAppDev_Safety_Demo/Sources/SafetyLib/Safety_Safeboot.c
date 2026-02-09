/*
 * Safety_Safeboot.c
 *
 *  Created on: Jun 26, 2019
 *      Author: B33124
 */


#include "SafetyLib.h"

/*********************************************************************************************
 * max allowed define 4 safe blocks: start address + block size
 * Block start address should be 0x20 aligned (32bytes aligned)
 * Block size should be 0x20 aligned (32bytes aligned)
 *********************************************************************************************/
extern uint32_t __CUSTOM_END;	//this is the symbol defined in linker file (.ld) to indicate the end address of used Flash.

uint32_t safeBootStartAddr[NUM_OF_SAFE_BLOCKS] = {
	SAFE_BLOCK0_START_ADDR,
	SAFE_BLOCK1_START_ADDR,
	SAFE_BLOCK2_START_ADDR,
	SAFE_BLOCK3_START_ADDR
};

uint32_t safeBootSize[NUM_OF_SAFE_BLOCKS] = {
	SAFE_BLOCK0_SIZE,
	SAFE_BLOCK1_SIZE,
	SAFE_BLOCK2_SIZE,
	SAFE_BLOCK3_SIZE
};

/******************************************************************************************
 * CRC reference value is saved in fixed address 0x410
 ******************************************************************************************/
const uint32_t safeCrcRefValue __attribute__((section (".sectionChkSum"))) = 0xFFFFFFFF;
const uint32_t safeDummyWord1  __attribute__((section (".sectionChkSum"))) = 0xFFFFFFFF;
const uint32_t safeDummyWord2  __attribute__((section (".sectionChkSum"))) = 0xFFFFFFFF;
const uint32_t safeDummyWord3  __attribute__((section (".sectionChkSum"))) = 0xFFFFFFFF;

/*********************************************************************************************
 * It holds the CRC calculation result of safe boot.
 *********************************************************************************************/
uint32_t safeBootCheckSum[2] = {0, 0};

/*********************************************************************************************
 * It's 0 after reset.
 * And after safeboot flash CRC calculation, software checks the CRC base value in Flash.
 * It's set to 1 if the CRC base value is 0xFFFFFFFF, meaning it's a new MCU which is not checked for Flash CRC before.
 * Software can do other operations (such as clearing all Faults in EEPROM) within this reset cycle.
 * After any reset, it's back to 0. Until any function erase the Flash that holds CRC base value.
**********************************************************************************************/
uint32_t safeBootNewMcu = 0;

/*********************************************************************************************
 * It holds DMA TCD settings for the second flash block.
 *********************************************************************************************/
extern edma_software_tcd_t myTcd2;

/*************************************************************************************
 * This demo shows how to config two DMA channel with link feature to generate two blocks CRC value
 * DMA channel 14 for flash block 0 transfer (into CRC register)
 * DMA channel 15 for flash block 1 transfer, this is the main block, may be up to 512K
 ************************************************************************************/
#define DMA_MINOR_LOOP_SIZE		0x10
edma_loop_transfer_config_t dmaCrcLoopConfig1 = {
	SAFE_BLOCK0_SIZE/DMA_MINOR_LOOP_SIZE,   //majorLoopIterationCount
    true,                   				//srcOffsetEnable
    false,                   				//dstOffsetEnable
    4,                						//minorLoopOffset
    false,         							//minorLoopChnLinkEnable
    0,         								//minorLoopChnLinkNumber
    false,            						//majorLoopChnLinkEnable
    0,         								//majorLoopChnLinkNumber
};

edma_loop_transfer_config_t dmaCrcLoopConfig2 = {
	SAFE_BLOCK1_SIZE/DMA_MINOR_LOOP_SIZE,   //majorLoopIterationCount
    true,                   				//srcOffsetEnable
    false,                   				//dstOffsetEnable
    4,                						//minorLoopOffset
    false,         							//minorLoopChnLinkEnable
    0,         								//minorLoopChnLinkNumber
    false,            						//majorLoopChnLinkEnable
    0,         								//majorLoopChnLinkNumber
};

edma_transfer_config_t dmaCrcTcd1 = {
	(uint32_t)SAFE_BLOCK0_START_ADDR,           //srcAddr
	(uint32_t)((uint8_t *)&(CRC->DATAu.DATA)),	//destAddr
	2,             							//srcTransferSize	2=4bytes
	2,            							//destTransferSize	2=4bytes
	4,                                		//srcOffset
	0,                               		//destOffset
	0,                      				//srcLastAddrAdjust
	0,                       				//destLastAddrAdjust
	0,                          			//srcModulo
	0,                         				//destModulo
	DMA_MINOR_LOOP_SIZE,                  	//minorByteTransferCount
	true,                      				//scatterGatherEnable
	(uint32_t)&myTcd2,               		//scatterGatherNextDescAddr
	false,                             		//interruptEnable
	&dmaCrcLoopConfig1,  					//loopTransferConfig
};

edma_transfer_config_t dmaCrcTcd2 = {
	(uint32_t)SAFE_BLOCK1_START_ADDR,          	//srcAddr
	(uint32_t)((uint8_t *)&(CRC->DATAu.DATA)),	//destAddr
	2,             							//srcTransferSize	2=4bytes
	2,            							//destTransferSize	2=4bytes
	4,                                		//srcOffset
	0,                               		//destOffset
	0,                      				//srcLastAddrAdjust
	0,                       				//destLastAddrAdjust
	0,                          			//srcModulo
	0,                         				//destModulo
	DMA_MINOR_LOOP_SIZE,                  	//minorByteTransferCount
	false,                         			//scatterGatherEnable
	0,               						//scatterGatherNextDescAddr
	true,                             		//interruptEnable
	&dmaCrcLoopConfig2,  					//loopTransferConfig
};


/*
edma_software_tcd_t myTcd1 = {
	SAFE_BLOCK0_START_ADDR,	    //uint32_t SADDR;
	4,							//int16_t SOFF;
	(4<<8) | 2,					//uint16_t ATTR; SSIZE=4=16bytes, DSIZE=2=4bytes
	(1<<31) | (4<<10) | 16,		//uint32_t NBYTES; transfer 16bytes in each minor loop, minor loop offset 4bytes, minor loop offset enable
	0,							//int32_t SLAST;
	(uint32_t)((uint8_t *)&(CRC->DATAu.DATA)),	//uint32_t DADDR;
	0,							//int16_t DOFF;
	SAFE_BLOCK0_SIZE/DMA_MINOR_LOOP_SIZE,	//uint16_t CITER; CITER, channel linking disabled
	(uint32_t)((uint8_t *)&(myTcd2)),	//int32_t DLAST_SGA;
	(1<<4) | (1<<3) | (0<<1),	//uint16_t CSR; ESG=1, DREQ=1, INTMAJOR=0
	SAFE_BLOCK0_SIZE/DMA_MINOR_LOOP_SIZE	//uint16_t BITER;
};
*/

/*************************************************************************
 * This struct is used for DMA scatter/gather feature.
 * This feature can collect multiple blocks of data;
 **************************************************************************/
edma_software_tcd_t __attribute__ ((aligned(32))) myTcd2;
/*
edma_software_tcd_t __attribute__ ((aligned(32))) myTcd2 = {
	SAFE_BLOCK1_START_ADDR,	    //uint32_t SADDR;
	4,							//int16_t SOFF;
	(2<<8) | 2,					//uint16_t ATTR; SSIZE=2=4bytes, DSIZE=2=4bytes
	16,							//uint32_t NBYTES; transfer 16bytes in each minor loop
	0,							//int32_t SLAST;
	(uint32_t)((uint8_t *)&(CRC->DATAu.DATA)),	//uint32_t DADDR;
	0,							//int16_t DOFF;
	SAFE_BLOCK1_SIZE/DMA_MINOR_LOOP_SIZE,	//uint16_t CITER; CITER, channel linking disabled
	0,							//int32_t DLAST_SGA;
	(0<<4) | (1<<3) | (1<<1),	//uint16_t CSR; ESG=0, DREQ=1, INTMAJOR=1
	SAFE_BLOCK1_SIZE/DMA_MINOR_LOOP_SIZE	//uint16_t BITER;
};
*/

/**********************************************************************************************************
 * This function goes through all the used code flash and generate a CRC value.
 * Must take into consideration that its execution time is long and might cause WDOG reset.
 **********************************************************************************************************/
void Safeboot_Init(void)
{
	safeBootCheckSum[0] = Safeboot_CrcCalc();
	safeBootCheckSum[1] = 0xFFFFFFFF;

	//check whether it's the first time run the code: Y - initialize the crc value; N - compare the crc value;
	if( safeCrcRefValue == 0xFFFFFFFF )		//if CRC value is not initialized yet
	{
		//program the u32CheckSum value to address CRC32_SAVE_ADDR;
		FLASH_DRV_Program( &mySSDConfig, (uint32_t)(&safeCrcRefValue), 8, (uint8_t *)(&safeBootCheckSum[0]) );
	}
	else		//CRC value is ready for compare
	{
		if( safeCrcRefValue != safeBootCheckSum[0] )	//if checksum value invalid
		{
			Safety_Safeboot_Callback();
		}
	}
}

uint32_t Safeboot_CrcCalc(void)
{
	uint32_t i;
	if(bCrcInUse)		//check whether CRC is in use, to avoid conflicts.
		return 0;
	else
		bCrcInUse = 1;

	safeBootSize[1] = ((uint32_t)(&__CUSTOM_END)) - SAFE_BLOCK1_START_ADDR;
	if( (safeBootSize[1]&0xF) > 0)
		safeBootSize[1] = (safeBootSize[1] & 0xFFFFFFF0) + 0x10;

	//calculate the CRC32 value of specified memory blocks
	CRC_DRV_Init(INST_CRC1, &crc1_InitConfig2);		//configure CRC for 32bit checksum calculation
	for(i = 0; i < 4; i++)							//calculate CRC for all 4 blocks
	{
		if(safeBootSize[i] > 0)
			CRC_DRV_WriteData(INST_CRC1, (uint8_t *)safeBootStartAddr[i], safeBootSize[i]);
	}
	i = CRC_DRV_GetCrcResult(INST_CRC1);
	bCrcInUse = 0;
	return i;
}

/**********************************************************************
 * Start code flash CRC calculation by start DMA channel 14
 * When data transfer completed, interrupt invoked and ch14 callback function will be called
 *********************************************************************/
uint32_t Safeboot_StartCrcDma(void)
{
	safeBootCheckSum[0] = 0;

	if(bCrcInUse)		//check whether CRC is in use, to avoid conflicts.
		return 0;
	else
		bCrcInUse = 1;	//lock CRC for safe boot function

	//the block size should be 16bytes aligned
	safeBootSize[1] = ((uint32_t)(&__CUSTOM_END)) - SAFE_BLOCK1_START_ADDR;
	if( (safeBootSize[1]&0xF) > 0)
		safeBootSize[1] = (safeBootSize[1] & 0xFFFFFFF0) + 0x10;

	CRC_DRV_Init(INST_CRC1, &crc1_InitConfig2);		//CRC 32bit, byte order reverse (CRC_CTRL_TOT=3);

	//config myTcd2 which includes the information of the second memory block to transfer to CRC;
	dmaCrcTcd2.loopTransferConfig->majorLoopIterationCount = safeBootSize[1]/DMA_MINOR_LOOP_SIZE;
	EDMA_DRV_PushConfigToSTCD(&dmaCrcTcd2, &myTcd2);			//write DMA channel settings to a TCD which is a duplicate of DMA channel TCD registers
	myTcd2.CSR |= (DMA_TCD_CSR_DREQ_MASK | DMA_TCD_CSR_BWC(2));	//disable this DMA channel after major loop, and limit DMA bandwidth to have CPU best performance

	//initialize DMA channel 14 for move code memory block 0 into CRC register
	EDMA_DRV_ChannelInit(&dmaController1Chn2_State, &dmaController1Chn2_Config);
	EDMA_DRV_PushConfigToReg(EDMA_CHN2_NUMBER, &dmaCrcTcd1);
	//EDMA_DRV_DisableRequestsOnTransferComplete(EDMA_CHN2_NUMBER, true);	//should not disable DMA channel after first block transfer completed
	EDMA_DRV_StartChannel(EDMA_CHN2_NUMBER);
	//use a timer to check DMA status in case timeout occurred, or DMA fault.
	//LPTMR_DRV_SetCompareValueByCount(INST_LPTMR1, 65500);
	LPTMR_DRV_SetCompareValueByCount(INST_LPTMR1, (safeBootSize[1] + safeBootSize[0])/10);	//result shows that 187200bytes => 12350cnt, SO size/10 is enough count to timeout the DMA transfer
	LPTMR_DRV_SetInterrupt(INST_LPTMR1, true);
	LPTMR_DRV_StartCounter(INST_LPTMR1);
	INT_SYS_EnableIRQ(LPTMR0_IRQn);
	return 1;
}

/****************************************************************************
 * This function is called when DMA ch14 transfer major loop completed.
 ****************************************************************************/
void DMA_Ch14Callback(void *parameter, edma_chn_status_t status)
{
	safeBootCheckSum[0] = CRC_DRV_GetCrcResult(INST_CRC1);
	//safeBootCheckSum[1] = 0xFFFFFFFF;
	safeBootCheckSum[1] = LPTMR_DRV_GetCounterValueByCount(INST_LPTMR1);
	bCrcInUse = 0;				//release CRC to other functions

	//check whether it's the first time run the code: Y - initialize the crc value; N - compare the crc value;
	if(safeCrcRefValue == 0xFFFFFFFF)		//if CRC value is not initialized yet
	{
		safeBootNewMcu = 1;			//indicate it's a new MCU (the first time to run safeboot)
		//program the u32CheckSum value to address CRC32_SAVE_ADDR;
		FLASH_DRV_Program( &mySSDConfig, (uint32_t)(&safeCrcRefValue), 8, (uint8_t *)(&safeBootCheckSum[0]) );
	}
	else
	{
		//if(*(uint32_t *)CRC32_SAVE_ADDR != safeBootCheckSum[0])	//if checksum value invalid
		if(safeCrcRefValue != safeBootCheckSum[0])	//if checksum value invalid

		{
			faultBuffer.faultType = faultSafetyBoot;
			faultBuffer.faultPcAddr = safeCrcRefValue;
			faultBuffer.faultDataAddr = safeBootCheckSum[0];
			Safety_FaultManage_SaveFault(&faultBuffer);	//save fault if invalid checksum detected
			Safety_Safeboot_Callback();
		}
	}
	//DMAMUX->CHCFG[14] = 0;		//turn off DMA Ch14 always enable to avoid spurious DMA transfer;
	//PINS_DRV_SetPins(PTE, (1<<11));
}

/*****************************************************************************
 * LPTMR0 is used in safety initial check to avoid DMA timeout.
 * DMA is used to generate CRC for all used code flash.
 * We set LPTMR0 timeout ~16ms to generate interrupt and check DMA status.
 * If DMA is still not finished, stop DMA to avoid possible fault.
 *****************************************************************************/
void LPTMR0_IRQHandler(void)
{
	LPTMR_DRV_ClearCompareFlag(INST_LPTMR1);
	LPTMR_DRV_StopCounter(INST_LPTMR1);
	if(safeBootCheckSum[0] == 0)	//means DMA CH14 ISR callback not invoked, so DMA not finished
	{
		EDMA_DRV_StopChannel(EDMA_CHN2_NUMBER);
		safeBootCheckSum[0] = 0xFFFFFFFF;
		Safety_Safeboot_Callback();
	}
	bCrcInUse = 0;				//release CRC to other functions
}
