#include "bsp_eeprom.h"

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus; 

I2C_HandleTypeDef  heval_I2c;
I2C_HandleTypeDef  heval_I2c3;
static void     I2Cx_Init(void);
static HAL_StatusTypeDef I2Cx_WriteMultiple(uint8_t Addr, uint16_t Reg, uint16_t MemAddress, uint8_t *Buffer, uint16_t Length);
static HAL_StatusTypeDef I2Cx_ReadMultiple(uint8_t Addr, uint16_t Reg, uint16_t MemAddress, uint8_t *Buffer, uint16_t Length);
static HAL_StatusTypeDef  I2Cx_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);
static void     I2Cx_Error(uint8_t Addr);
static void I2C3_Error(uint8_t Addr);

uint32_t BSP_EEPROM_WritePage(uint8_t *pBuffer, uint16_t WriteAddr, uint8_t *NumByteToWrite);
void                EEPROM_IO_Init(void);
HAL_StatusTypeDef   EEPROM_IO_WriteData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize);
HAL_StatusTypeDef   EEPROM_IO_ReadData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize);
HAL_StatusTypeDef   EEPROM_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);
void     BSP_EEPROM_TIMEOUT_UserCallback(void);
HAL_StatusTypeDef RTC_IO_ReadData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize);
HAL_StatusTypeDef RTC_IO_WriteData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize);
HAL_StatusTypeDef I2C3_IO_WriteData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize);
HAL_StatusTypeDef I2C3_IO_ReadData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize);
uint32_t BSP_WaitIICStandbyState(void); 
static HAL_StatusTypeDef I2C3_IsDeviceReady(uint16_t DevAddress, uint32_t Trials);

#define GPIO_PIN_2                 ((uint16_t)0x0004)  /* Pin 2 selected    */

int BSP_EepromInit (void)
{ 
  //BSP_Eeprom_WP_Ctrl(EEPROM_WP_DISENABLE);
  EEPROM_IO_Init();
  BSP_Eeprom_WP_Ctrl(EEPROM_WP_ENABLE);
  return BSP_EEPROM_WaitEepromStandbyState(); 
}
//由于需要1000V和750V通过读取EEPROM进行兼容，所以这里两个版本用于写保护的IO都要初始化
//另外该初始话只在启动时运行一次。如果启动后还需要控制写保护，该函数需要根据版本分别控制
void BSP_Eeprom_WP_Ctrl (GPIO_PinState u8CtrlStatus)
{
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,u8CtrlStatus);
  HAL_GPIO_WritePin(GPIOF,GPIO_PIN_7,u8CtrlStatus);
}


uint32_t BSP_EEPROM_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead)
{  
    if(EEPROM_IO_ReadData(0xA0, ReadAddr, pBuffer, NumByteToRead) != HAL_OK)
    {
        BSP_EEPROM_TIMEOUT_UserCallback();
        return EEPROM_FAIL;
    }
    return EEPROM_OK;
}

uint32_t BSP_EEPROM_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t  numofpage = 0, numofsingle = 0, count = 0;
  uint16_t addr = 0;
  uint8_t  dataindex = 0;
  uint32_t status = EEPROM_OK;

  addr = WriteAddr % EEPROM_PAGESIZE;
  count = EEPROM_PAGESIZE - addr;
  numofpage =  NumByteToWrite / EEPROM_PAGESIZE;
  numofsingle = NumByteToWrite % EEPROM_PAGESIZE;
  
  if(addr == 0) 
  {
    if(numofpage == 0) 
    {
      /* Store the number of data to be written */
      dataindex = numofsingle;
      /* Start writing data */
      status = BSP_EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&dataindex));
      if(status != EEPROM_OK)
      {
        return status;
      }
    }
    else  
    {
      while(numofpage--)
      {
        /* Store the number of data to be written */
        dataindex = EEPROM_PAGESIZE;        
        status = BSP_EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&dataindex));
        if(status != EEPROM_OK)
        {
          return status;
        }
        
        WriteAddr +=  EEPROM_PAGESIZE;
        pBuffer += EEPROM_PAGESIZE;
      }
      
      if(numofsingle!=0)
      {
        /* Store the number of data to be written */
        dataindex = numofsingle;          
        status = BSP_EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&dataindex));
        if(status != EEPROM_OK)
        {
          return status;
        }
      }
    }
  }
  else 
  {
    /* If NumByteToWrite < EEPROM_PAGESIZE */
    if(numofpage == 0) 
    {
      /* If the number of data to be written is more than the remaining space 
      in the current page: */
      if(NumByteToWrite > count)
      {
        /* Store the number of data to be written */
        dataindex = count;        
        /* Write the data contained in same page */
        status = BSP_EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&dataindex));
        if(status != EEPROM_OK)
        {
          return status;
        }
        
        /* Store the number of data to be written */
        dataindex = (NumByteToWrite - count);          
        /* Write the remaining data in the following page */
        status = BSP_EEPROM_WritePage((uint8_t*)(pBuffer + count), (WriteAddr + count), (uint8_t*)(&dataindex));
        if(status != EEPROM_OK)
        {
          return status;
        }
      }      
      else      
      {
        /* Store the number of data to be written */
        dataindex = numofsingle;         
        status = BSP_EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&dataindex));
        if(status != EEPROM_OK)
        {
          return status;
        }
      }     
    }
    /* If NumByteToWrite > EEPROM_PAGESIZE */
    else
    {
      NumByteToWrite -= count;
      numofpage =  NumByteToWrite / EEPROM_PAGESIZE;
      numofsingle = NumByteToWrite % EEPROM_PAGESIZE;
      
      if(count != 0)
      {  
        /* Store the number of data to be written */
        dataindex = count;         
        status = BSP_EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&dataindex));
        if(status != EEPROM_OK)
        {
          return status;
        }
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(numofpage--)
      {
        /* Store the number of data to be written */
        dataindex = EEPROM_PAGESIZE;          
        status = BSP_EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&dataindex));
        if(status != EEPROM_OK)
        {
          return status;
        }
        WriteAddr +=  EEPROM_PAGESIZE;
        pBuffer += EEPROM_PAGESIZE;  
      }
      if(numofsingle != 0)
      {
        /* Store the number of data to be written */
        dataindex = numofsingle; 
        status = BSP_EEPROM_WritePage(pBuffer, WriteAddr, (uint8_t*)(&dataindex));
        if(status != EEPROM_OK)
        {
          return status;
        }
      }
    }
  }  
                                   
  /* If all operations OK, return EEPROM_OK (0) */
  return EEPROM_OK;
}

uint32_t BSP_EEPROM_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite)
{ 
  uint32_t buffersize = *NumByteToWrite;
  uint32_t status = EEPROM_OK;
  
  /* Set the pointer to the Number of data to be written */
  if(EEPROM_IO_WriteData(0xA0, WriteAddr, pBuffer, buffersize) != HAL_OK)  
  {
    BSP_EEPROM_TIMEOUT_UserCallback();
    status = EEPROM_FAIL;
  }
  
  while(BSP_EEPROM_WaitEepromStandbyState() != EEPROM_OK)
  {
    return EEPROM_FAIL;
  }

  /* If all operations OK, return EEPROM_OK (0) */
  return status;
}


uint32_t BSP_EEPROM_WaitEepromStandbyState(void)      
{
  /* Check if the maximum allowed number of trials has bee reached */
  if(EEPROM_IO_IsDeviceReady(0xA0, EEPROM_MAX_TRIALS) != HAL_OK)
  {
    /* If the maximum number of trials has been reached, exit the function */
    BSP_EEPROM_TIMEOUT_UserCallback();
    return EEPROM_TIMEOUT;
  }
  return EEPROM_OK;
}
uint32_t BSP_WaitIICStandbyState(void)      
{
	return (I2C3_IsDeviceReady(0xA0,EEPROM_MAX_TRIALS));
}
__weak void BSP_EEPROM_TIMEOUT_UserCallback(void)
{
}

void EEPROM_IO_Init(void)
{
  I2Cx_Init();
}


HAL_StatusTypeDef EEPROM_IO_WriteData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize)
{
  return (I2Cx_WriteMultiple(DevAddress, MemAddress, 0x10, pBuffer, BufferSize));
}


HAL_StatusTypeDef EEPROM_IO_ReadData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize)
{
   
  return (I2Cx_ReadMultiple(DevAddress, MemAddress, 0x10, pBuffer, BufferSize));
}

HAL_StatusTypeDef EEPROM_IO_IsDeviceReady(uint16_t DevAddress, uint32_t Trials)
{ 
  return (I2Cx_IsDeviceReady(DevAddress, Trials));
}


static void I2Cx_Init(void)
{
  if(HAL_I2C_GetState(&heval_I2c) == HAL_I2C_STATE_RESET)
  {
    heval_I2c.Instance = I2C1;
    heval_I2c.Init.ClockSpeed      = 100000;
    heval_I2c.Init.DutyCycle       = 0x00;
    heval_I2c.Init.OwnAddress1     = 0;
    heval_I2c.Init.AddressingMode  = 0x4000;
    heval_I2c.Init.DualAddressMode = 0x00;
    heval_I2c.Init.OwnAddress2     = 0;
    heval_I2c.Init.GeneralCallMode = 0x00;
    heval_I2c.Init.NoStretchMode   = 0x00;  

    /* Init the I2C */
    HAL_I2C_Init(&heval_I2c);
  }
}

static HAL_StatusTypeDef I2Cx_ReadMultiple(uint8_t Addr, uint16_t Reg, uint16_t MemAddress, uint8_t *Buffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Read(&heval_I2c, Addr, (uint16_t)Reg, MemAddress, Buffer, Length, I2C_TIMEOUT);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* I2C error occured */
    I2Cx_Error(Addr);
  }
  return status;    
}
static HAL_StatusTypeDef I2C3_ReadMultiple(uint8_t Addr, uint16_t Reg, uint16_t MemAddress, uint8_t *Buffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Read(&heval_I2c3, Addr, (uint16_t)Reg, MemAddress, Buffer, Length, I2C_TIMEOUT);

  if(status != HAL_OK)
  {
    I2C3_Error(Addr);
  }
  return status;    
}

static HAL_StatusTypeDef I2Cx_WriteMultiple(uint8_t Addr, uint16_t Reg, uint16_t MemAddress, uint8_t *Buffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Write(&heval_I2c, Addr, (uint16_t)Reg, MemAddress, Buffer, Length, I2C_TIMEOUT);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Re-Initiaize the I2C Bus */
    I2Cx_Error(Addr);
  }
  return status;
}

static HAL_StatusTypeDef I2C3_WriteMultiple(uint8_t Addr, uint16_t Reg, uint16_t MemAddress, uint8_t *Buffer, uint16_t Length)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_I2C_Mem_Write(&heval_I2c3, Addr, (uint16_t)Reg, MemAddress, Buffer, Length, I2C_TIMEOUT);

  if(status != HAL_OK)
  {
    I2C3_Error(Addr);
  }
  BSP_WaitIICStandbyState();
  return status;
}

static HAL_StatusTypeDef I2Cx_IsDeviceReady(uint16_t DevAddress, uint32_t Trials)
{ 
  return (HAL_I2C_IsDeviceReady(&heval_I2c, DevAddress, Trials, I2C_TIMEOUT));
}
static HAL_StatusTypeDef I2C3_IsDeviceReady(uint16_t DevAddress, uint32_t Trials)
{ 
  return (HAL_I2C_IsDeviceReady(&heval_I2c3, DevAddress, Trials, I2C_TIMEOUT));
}

static void I2Cx_Error(uint8_t Addr)
{
  /* De-initialize the IOE comunication BUS */
  HAL_I2C_DeInit(&heval_I2c);
  
  /* Re-Initiaize the IOE comunication BUS */
  I2Cx_Init();  
}
static void I2C3_Error(uint8_t Addr)
{
  HAL_I2C_DeInit(&heval_I2c3);
  
  //I2C3_Init();  
}
HAL_StatusTypeDef RTC_IO_WriteData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize)
{
  return (I2Cx_WriteMultiple(DevAddress, MemAddress, 0x01, pBuffer, BufferSize));
}
HAL_StatusTypeDef RTC_IO_ReadData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize)
{
   return (I2Cx_ReadMultiple(DevAddress, MemAddress, 0x01, pBuffer, BufferSize));
}

uint8_t BSP_WriteRtcReg(uint8_t Buffer, uint16_t WriteAddr)
{ 
  if(RTC_IO_WriteData(0x64, WriteAddr, &Buffer, 1) != HAL_OK)  
  {
    return EEPROM_FAIL;
  }
  return EEPROM_OK;
}
uint8_t BSP_ReadRtcReg(uint8_t* pBuffer, uint16_t ReadAddr)
{  
    if(RTC_IO_ReadData(0x64, ReadAddr, pBuffer, 1) != HAL_OK)
    {
        return EEPROM_FAIL;
    }
    return EEPROM_OK;
}
void I2C3_Init(void)
{
	//if(HAL_I2C_GetState(&heval_I2c3) == HAL_I2C_STATE_RESET)
	//{
	//	heval_I2c3.Instance = I2C3;
	//	heval_I2c3.Init.ClockSpeed      = 100000;
	//	heval_I2c3.Init.DutyCycle       = 0x00;
	//	heval_I2c3.Init.OwnAddress1     = 0;
	//	heval_I2c3.Init.AddressingMode  = 0x4000;
	//	heval_I2c3.Init.DualAddressMode = 0x00;
	//	heval_I2c3.Init.OwnAddress2     = 0;
	//	heval_I2c3.Init.GeneralCallMode = 0x00;
	//	heval_I2c3.Init.NoStretchMode   = 0x00;
	//	HAL_I2C_Init(&heval_I2c3);
	//}
}

HAL_StatusTypeDef I2C3_IO_WriteData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize)
{
  return (I2C3_WriteMultiple(DevAddress, MemAddress, 0x10, pBuffer, BufferSize));
}
HAL_StatusTypeDef I2C3_IO_ReadData(uint16_t DevAddress, uint16_t MemAddress, uint8_t* pBuffer, uint32_t BufferSize)
{
   return (I2C3_ReadMultiple(DevAddress, MemAddress, 0x10, pBuffer, BufferSize));
}
uint8_t BSP_WriteI2C3Reg(uint16_t DevAddress,uint8_t* pBuffer, uint16_t WriteAddr)
{ 
  if(I2C3_IO_WriteData(DevAddress, WriteAddr, pBuffer, 1) != HAL_OK)  
  {
    return EEPROM_FAIL;
  }
  return EEPROM_OK;
}
uint8_t BSP_ReadI2C3Reg(uint16_t DevAddress,uint8_t* pBuffer, uint16_t ReadAddr)
{  
  if(I2C3_IO_ReadData(DevAddress, ReadAddr, pBuffer, 1) != HAL_OK)
  {
    return EEPROM_FAIL;
  }
  return EEPROM_OK;
}