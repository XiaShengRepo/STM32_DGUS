#include "bsp.h"
/***************************************
硬件spi接口
***************************************/
//
//void  SD_GPIO_Configuration(void)
//{
// 
//	GPIO_InitTypeDef GPIO_InitStructure;
//  	SPI_InitTypeDef  SPI_InitStructure;
////	DMA_InitTypeDef DMA_InitStructure; 
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	 
//
////	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 						 
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
// 
//	GPIO_PinAFConfig( GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);
//	GPIO_PinAFConfig( GPIOB,GPIO_PinSource14,GPIO_AF_SPI2);
//	GPIO_PinAFConfig( GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);	
//
//    /* Configure SPI2 pins: SCK, MISO and MOSI */
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
////	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//
// //	GPIO_SetBits(GPIOB,GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);  //PB13/14/15上拉
//
//   GPIO_SetBits(GPIOA,GPIO_Pin_4);	      //后面添加的
// //   GPIO_ResetBits(GPIOA,GPIO_Pin_4);	
//  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
//  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
//  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
//  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
//  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
//  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//  SPI_InitStructure.SPI_CRCPolynomial = 7;
//  SPI_Init(SPI2, &SPI_InitStructure);
//  SPI_Cmd(SPI2, ENABLE);
//
//}
//
//uint8_t SD_SPI_ReadWriteByte(uint8_t TxData)
//{	
//	   
//	uint8_t retry=0;				 	
//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
//		{
//		retry++;
//		if(retry>200)return 0;
//		}			  
//	SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
//	retry=0;
//
//	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
//		{
//		retry++;
//		if(retry>200)return ;
//		}	  						    
//	return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据					    
//}

/*************************************************************************
一下是模拟SPI	 start
**************************************************************************/
void SD_GPIO_Configuration(void)
{
 GPIO_InitTypeDef GPIO_InitStructure;
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
 	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;	   //CS
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 GPIO_Init(GPIOA, &GPIO_InitStructure);

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;	   //MISO
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//GPIO_Mode_OUT;
// GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 GPIO_Init(GPIOA, &GPIO_InitStructure);

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 	;   //SCLK
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 GPIO_Init(GPIOA, &GPIO_InitStructure);

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;	   //MOSI
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//GPIO_Mode_IN;
 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
}

 /******************************************************************
 - 功能描述：延时函数
 - 隶属模块：公开函数模块
 - 函数属性：外部，用户可调用
 - 参数说明：time:time值决定了延时的时间长短           
 - 返回说明：无
 - 注：.....
 ******************************************************************/
unsigned char is_init;  //在初始化的时候设置此变量为1,同步数据传输（SPI）会放慢

#define DELAY_TIME 1000 //SD卡的复位与初始化时SPI的延时参数，根据实际速率修改其值，否则会造成SD卡复位或初始化失败

void delay(unsigned int time) 
{
 while(time--);
}

void SD_spi_write(unsigned char x) 
{
 unsigned char i;
 for(i=0;i<8;i++)
 { 
	 ResetSCLK ;
	 if(x&0x80)
	 {
     GPIOB->BSRRL = GPIO_Pin_5;
	 }
	 else
	 {
     GPIOB->BSRRH = GPIO_Pin_5;
	 }
	 if(is_init) delay(DELAY_TIME);
	 SetSCLK 	;
   x <<= 1 ;
 }
}

/******************************************************************
 - 功能描述：IO模拟SPI，读取一个字节
 - 隶属模块：SD/SDHC模块
 - 函数属性：内部
 - 参数说明：无
 - 返回说明：返回读到的字节
 ******************************************************************/

unsigned char SD_spi_read() //SPI读一个字节
{  
  unsigned char i,temp=0;

  for(i=0;i<8;i++)
  {
    SetSCLK ;
    if(is_init) delay(DELAY_TIME);
    ResetSCLK ;
    if(is_init) delay(DELAY_TIME);
    if(GPIOA->IDR & GPIO_Pin_6)
    {
      temp|=(0x80>>i);
    }
  }
  return (temp);
}

/*************************************************************************
以上是模拟SPI	 end
**************************************************************************/
//判断是否闰年
u8 isRunNian(u16 y)
{
  return (y % 4 == 0 && y % 100 != 0 || y % 400 == 0) ? 1 : 0;
}
//根据月与年得到月的天数
u8 getDays(u16 y, u8 m)
{
  u8 days = 0;
  switch(m)
  {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			days = 31; break;
		case 4:
		case 6:
		case 9:
		case 11:
			days = 30; break;
		case 2:
			days = isRunNian(y) ? 29 : 28; break;
		default:;
	}
	return days;
}


  /*******************************************************************************
 * 函数名称:itoa                                                                   
 * 描    述:把int转换成字符型                                                              
 *                                                                               
 * 输    入:无                                                                     
 * 输    出:无                                                                     
 * 返    回:无                                                                  
 * 修改日期:2013年4月20日                                                                    
 *******************************************************************************/
char 	f_name[12];
void itoa(char *f_name, Struct_TD  a)
{
	 uint8_t i,j=0;
	 f_name[j++] = 2               ;
	 f_name[j++] = 0			         ;
	 f_name[j++] = a.Year     /	10 ;
	 f_name[j++] = a.Year     %	10 ;
	 f_name[j++] = a.Month    / 10;
	 f_name[j++] = a.Month    % 10;
	 f_name[j++] = a.Date     / 10;
	 f_name[j++] = a.Date     % 10;
//	 f_name[j++] = a.Hours    / 10;
//	 f_name[j++] = a.Hours    % 10;
//	 f_name[j++] = a.Minutes  / 10;
//	 f_name[j++] = a.Minutes  % 10;
	
	 for(i=0;i<8;i++)
	 f_name[i] += '0'; //日期数据转换为字符
	 f_name[j++]  = '.';
	 f_name[j++]  = 't';
	 f_name[j++]	= 'x';
	 f_name[j++]	= 't';
}
  /*******************************************************************************
 * 函数名称:itoa   把int转换成字符型                                                                 
 * 描    述:无                                                                   
 *                                                                               
 * 输    入:无                                                                     
 * 输    出:无                                                                     
 * 返    回:无                                                                  
 * 修改日期:2013年4月20日                                                                    
 *******************************************************************************/
FATFS fs;
FRESULT res;
FILINFO fileInfo;
UINT br,bw;
UINT r=1;
DIR dir;
FIL fsrc;
 
void Fwriter(unsigned char *p)
{
		uint8_t sd_time;   
	  uint32_t rd; 
	  res = f_mount(0, &fs); 
		if(res!=FR_OK)
		{
		  return ;
		}
		else
		{
		}
		for(sd_time=0;sd_time<20;sd_time++)
		{
    res = f_open(&fsrc,f_name,FA_OPEN_ALWAYS|FA_WRITE );
	//	res = f_open(&fsrc,"0:2013011209408.txt", FA_OPEN_ALWAYS|FA_WRITE);  //文件不存在则创建新文件
		if(res==FR_OK) break;
	   }
		if(res!=FR_OK)
		{

		return ;
		}
		else
		{ 
		

		res = f_lseek(&fsrc,fsrc.fsize); 
   	res = f_write(&fsrc,p,2, &rd);
	//	res = f_write(&fsrc,"小王童鞋你好！！！\r\n",512, &rd);
		f_sync(&fsrc)	;	
	  if(res!=FR_OK)
	{
	  f_close(&fsrc);
	  f_mount(0, NULL);
	  return;
	}
	else
	{
	   f_close(&fsrc);
	   f_mount(0, NULL);
	}  

}
}


  /*******************************************************************************
 * 函数名称: Fread                                                                
 * 描    述: 从卡中读取数据                                                                
 *                                                                               
 * 输    入:无                                                                     
 * 输    出:无                                                                     
 * 返    回:无                                                                  
 * 修改日期:2013年4月20日                                                                    
 *******************************************************************************/
static uint32_t  Index = 0; 
unsigned char    Buffer[1024] = {0};
unsigned char    Fread(unsigned char *p)
{
		uint8_t   sd_time;   
    uint32_t  rd; 
	  uint16_t  Lenght = 0 ;	  
	  uint32_t  indexflag = 0; 
	
	  res = f_mount(0, &fs); 
		if(res!=FR_OK)
		{
			return 0;
		}
		for(sd_time=0;sd_time<20;sd_time++) //重复打开20次
		{
      res = f_open(&fsrc,f_name,FA_OPEN_EXISTING|FA_READ |FA_WRITE);  //默认打开一个文件
		//res = f_open(&fsrc,"0:123456789.txt", FA_OPEN_EXISTING|FA_READ );  //文件不存在则创建新文件
		  if(res==FR_OK) 	
		     break;
	  }
		if(res!=FR_OK) 
		{
		  return  1; //返回错误
		}
		else
		{ 	
		  // res = f_lseek(&fsrc,fsrc.fsize); 
		  res = f_lseek(&fsrc,Index); //指针指向第Index个字节
			//	printf("Index = %d \r\n",res);
		  res = f_read(&fsrc,Buffer,512, &rd);//读取512字节的数据到Buffer,rd 存储的是读到的字节数
			//	printf("Index = %d \r\n",res);
		  res = f_lseek(&fsrc,Index+512); //指针偏移Index+512个字节
			//	printf("Index = %d \r\n",res);
			res = f_read(&fsrc,&Buffer[512],512, &rd);//传递的是Buffer[512]的地址，读取了512字节的数据
//				printf("Index = %d \r\n",res);
//			  printf("Buffer = %c \r\n",Buffer[1021]);
//			  printf("Buffer = %c \r\n",Buffer[1022]);
//				printf("Buffer = %c \r\n",Buffer[1023]);
			if(Buffer[1022]== 'N' && Buffer[1023]== 'O') //表示没有发送成功
			{
//				  printf("Buffer = %c \r\n",Buffer[1021]);
//				  printf("Buffer = %c \r\n",Buffer[1022]);
//				  printf("Buffer = %c \r\n",Buffer[1023]);
				Lenght = Buffer[1019] << 8 | Buffer[1020];
				if(1)
				//if(Resend(Buffer,Lenght) == 0x00)
				{   		
					indexflag = Index+1021 ;
					res = f_lseek(&fsrc,indexflag);//偏移Index+1021  	  
          res = f_write(&fsrc,"Yes",3, &rd); //写"Yes"
				  f_close(&fsrc);
        } 
				else 
				{
					*p =  0x01 ; 
					res = f_lseek(&fsrc,1000);
					res = f_write(&fsrc,"Uncomplete",10, &rd);
					f_close(&fsrc);		      						
        }
      }	 
		  Index += 1024 ; //偏移1024
		  // printf("Index = %d \r\n",Index);
	    if(res!=FR_OK ||fsrc.fsize <= Index ) //fsrc.fsize是文件大小，这里判断文件大小
	    {
	      f_close(&fsrc);
//      f_mount(0, NULL);
	      return 1;
	    }
	    else
	    {
	      f_close(&fsrc);
//	    f_mount(0, NULL);
		    return 0 ;
	    }  
   }
}
  /*******************************************************************************
 * 函数名称:WriteDatatoSD                                                                  
 * 描    述:把信息写进sd卡                                                                 
 *                                                                               
 * 输    入:无                                                                     
 * 输    出:无                                                                     
 * 返    回:无                                                                  
 * 修改日期:2013年4月20日                                                                    
 *******************************************************************************/
void WriteDatatoSD(unsigned char *data)
{
	RTC_TimeShow();
	itoa(f_name,TimeDate);	  //把时间转换成字符
	Fwriter(data);					      //假如出错了。	
}


  /*******************************************************************************
 * 函数名称:ReadDatatoBuffer                                                                  
 * 描    述:从SD卡中读取数据                                                               
 *                                                                               
 * 输    入:无                                                                     
 * 输    出:无                                                                     
 * 返    回:无                                                                  
 * 修改日期:2013年4月20日                                                                    
 *******************************************************************************/
void ReadDatatoBuffer(void)
{
  unsigned char DelteFlag = 0 ;//用来标记是否全部上传了数据的函数
	unsigned char Times  = 0  ;
	Index = 0 ;
	for(Times = 0 ;Times <10 ;Times--)
	{
	  do
	  { 
	     if(Fread(&DelteFlag)== 0x01)					      //假如出错了�???
		   break ;
    }while(1);
  }
}


  /*******************************************************************************
 * 函数名称:SendtoServce                                                                  
 * 描    述:上电时刻检查数据     可以脱机交易7天。。。                                                             
 *                                                                               
 * 输    入:无                                                                     
 * 输    出:无                                                                     
 * 返    回:无                                                                  
 * 修改日期:2013年4月20日                                                                    
 *******************************************************************************/
void SendtoServce(void)
{
//20140211.txt	 
	unsigned char i    = 0;
	RTC_TimeShow();
	for(i=0;i<7;i++)
	{
		itoa(f_name,TimeDate);	  //文件名为TimeDate
//	printf("%s\r\n",f_name);
	 	if(TimeDate.Date == 1)
		{
			TimeDate.Date  = getDays(2000+TimeDate.Year,TimeDate.Month);
			if(TimeDate.Month == 1)
		  TimeDate.Month   = 12;
    }
		 //发送数据 ???
		ReadDatatoBuffer();
		TimeDate.Date -- ;	
  }	 
}

  /*******************************************************************************
 * 函数名称:File_delete                                                                  
 * 描    述:上电时刻删除前2个月的记录                                                                
 *                                                                               
 * 输    入:无                                                                     
 * 输    出:无                                                                     
 * 返    回:无                                                                  
 * 修改日期:2013年4月20日                                                                    
 *******************************************************************************/
void File_delete(void)
{
	 unsigned char i = 0 ,j=0,days = 0;
	 RTC_TimeShow();
 //  TimeDate.Month  = TimeDate.Month % 10 - 2 ;  //删除前二个月的记录
	 itoa(f_name,TimeDate);	  //把时间转换成字符 
//	 printf("%s\r\n",f_name);
	 if(TimeDate.Month == 0x01)
	 {
		 TimeDate.Month = 11;
		 TimeDate.Year -=1 ; 
	 }
	 if(TimeDate.Month == 0x02)
	 {
		 TimeDate.Month = 12;
	   TimeDate.Year -=1 ; 
	 }
// TimeDate.Month =TimeDate.Month>2 ? TimeDate.Month-2: 12-TimeDate.Month;        
	 for(i=0;i<2;i++)
	 { 
			res = f_mount(0, &fs); 
			if(res!=FR_OK)
			{
			return ;
			}
//	  TimeDate.Month= GetageMonth(TimeDate.Month);
	    itoa(f_name,TimeDate);	  //把时间转换成字符 	  
		  TimeDate.Date  = getDays(2000+TimeDate.Year,TimeDate.Month);
		  days = TimeDate.Date ;
		  for(j=0;j<days;j++)
		  {
				itoa(f_name,TimeDate);	  //把时间转换成字符 
//				printf("%s\r\n",f_name);
				f_unlink(f_name);  //
				TimeDate.Date -- ;
      }
			TimeDate.Month-- ;
   }
}


