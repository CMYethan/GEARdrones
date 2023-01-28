#include "i2c.h"
#include "main.h"
#include "cmsis_os.h"
void delay_us(uint32_t us)
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us);
	 while (delay--)
	{
		;
	}
}

void TOF_I2C_Init(void)
{
	TOF_SDA_0();
	TOF_SCL_0();
	delay_us(50);
	TOF_SDA_1();
	TOF_SCL_1();
	
//	HAL_GPIO_WritePin(XSHUT_GPIO_Port,XSHUT_Pin,GPIO_PIN_RESET);
//	HAL_Delay(30);
//	HAL_GPIO_WritePin(XSHUT_GPIO_Port,XSHUT_Pin,GPIO_PIN_SET);
}

void TOF_I2C_Start(void)
{
	TOF_SDA_1();
	TOF_SCL_1();
	TOF_SDA_0();
	TOF_SCL_0();
}
void TOF_I2C_Stop(void)
{
	TOF_SDA_0();
	TOF_SCL_0();
	TOF_SCL_1();
	TOF_SDA_1();
}

void TOF_I2C_Ack(void)
{
	TOF_SCL_0();
	TOF_SDA_0();
	TOF_SCL_1();
	TOF_SCL_0();
}
void TOF_I2C_NAck(void)
{
	TOF_SCL_0();
	TOF_SDA_1();
	TOF_SCL_1();
	TOF_SCL_0();
}

//ack: 		0
//not ack:1
uint8_t TOF_I2C_WaitAck(void)
{
	uint8_t ucErrTime=0;

	TOF_SDA_1();   
	TOF_SCL_1(); 
	while(TOF_READSDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			TOF_I2C_Stop();
			return 1;
		}
	}
	TOF_SCL_0();//ʱ�����0 	   
	return 0;  
}

void TOF_I2C_WriteByte(uint8_t txMsg)
{
	uint8_t i;

	TOF_SCL_0();//ʱ�����0 	
	for (i = 0; i < 8; i++)
	{		
		if ((txMsg&0x80)>>7){TOF_SDA_1();}
		else{TOF_SDA_0();}
		txMsg <<= 1;	/* ����һ��bit */
		TOF_SCL_1();
		TOF_SCL_0();
	}
}


//ack: 		 1
//not ack: 0
uint8_t TOF_I2C_ReadByte(unsigned char ack)
{
	uint8_t i;
	uint8_t rxMsg;

	/* ������1��bitΪ���ݵ�bit7 */
	rxMsg = 0;
	TOF_SDA_1();
	delay_us(4);
	for (i = 0; i < 8; i++)
	{
		rxMsg <<= 1;
		TOF_SCL_0();
		TOF_SCL_1();
		if (TOF_READSDA){rxMsg |= 0x01;}
	}
	if (!ack)
		TOF_I2C_NAck();//����nACK
	else
		TOF_I2C_Ack(); //����ACK   
	TOF_SCL_0();
	return rxMsg;
}

#define TRUE 1
#define FALSE 0

static uint8_t IIC_Start(void);				
static void IIC_Stop(void);	  		
static void IIC_Send_Byte(uint8_t txd);	
static uint8_t IIC_Read_Byte(void);
static uint8_t IIC_Wait_Ack(void); 	
static void IIC_Ack(void);				
static void IIC_NAck(void);		


//��VL53L0������VL53L1�������ƺ���ѹ�ƻ��������沿�ֵ����iic����ʱ��û�о����ϲ��������������iic���ȷ�������

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Start(void)
*��������:		����IIC��ʼ�ź�
*******************************************************************************/
static uint8_t IIC_Start(void)
{
	SDA_H;
	SCL_H;
	if(!SDA_read)
		return FALSE;	
	SDA_L;
	if(SDA_read)
		return FALSE;	
	SDA_L;
	return TRUE;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Stop(void)
*��������:	    //����IICֹͣ�ź�
*******************************************************************************/
static void IIC_Stop(void)
{
	SCL_L;
	SDA_L;
	SCL_H;
	SDA_H;

}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8_t IIC_Wait_Ack(void)
*��������:	    �ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
*******************************************************************************/
static uint8_t IIC_Wait_Ack(void) 	
{
	SCL_L;
	SDA_H;
	SCL_H;
	if(SDA_read)
	{
    SCL_L;
      return FALSE;
	}
	SCL_L;
	return TRUE;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Ack(void)
*��������:	    ����ACKӦ��
*******************************************************************************/
static void IIC_Ack(void)
{
	SCL_L;
	SDA_L;
	SCL_H;
	SCL_L;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_NAck(void)
*��������:	    ����NACKӦ��
*******************************************************************************/
static void IIC_NAck(void)
{
	SCL_L;
	SDA_H;
	SCL_H;
	SCL_L;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		void IIC_Send_Byte(uint8_t txd)
*��������:	    IIC����һ���ֽ�
*******************************************************************************/
static void IIC_Send_Byte(uint8_t SendByte) 
{
    uint8_t i=8;
    while(i--)
    {
			SCL_L;
			if(SendByte&0x80) SDA_H;
			else SDA_L;
			SendByte<<=1;
			SCL_H;
    }
    SCL_L;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8_t IIC_Read_Byte(unsigned char ack)
*��������:	    //��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
*******************************************************************************/
static unsigned char IIC_Read_Byte(void)  
{
    uint8_t i=8;
    uint8_t ReceiveByte=0;

    SDA_H;
    while(i--)
    {
			ReceiveByte<<=1;
			SCL_L;
			SCL_H;
			if(SDA_read)
			{
				ReceiveByte|=0x01;
			}
    }
    SCL_L;
    return ReceiveByte;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char IIC_ReadOneByte(unsigned char I2C_Addr,unsigned char addr)
*��������:	    ��ȡָ���豸 ָ���Ĵ�����һ��ֵ
����	I2C_Addr  Ŀ���豸��ַ
		addr	   �Ĵ�����ַ
����   ��������ֵ
*******************************************************************************/
uint8_t IIC_ReadOneByte(uint8_t SlaveAddress,uint16_t REG_Address,uint8_t* data)
{
	if(!IIC_Start())
			return FALSE;
    IIC_Send_Byte(SlaveAddress); 
    if(!IIC_Wait_Ack())
		{
			IIC_Stop();
			return FALSE;
		}
    IIC_Send_Byte((uint8_t) REG_Address>>8);   
    IIC_Wait_Ack();
    IIC_Send_Byte((uint8_t) REG_Address & 0x00ff);   
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(SlaveAddress+1);
    IIC_Wait_Ack();

		*data= IIC_Read_Byte();
    IIC_NAck();
    IIC_Stop();
    return TRUE;
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data)
*��������:	    ��ȡָ���豸 ָ���Ĵ����� length��ֵ
����	dev  Ŀ���豸��ַ
		reg	  �Ĵ�����ַ
		length Ҫ�����ֽ���
		*data  ���������ݽ�Ҫ��ŵ�ָ��
����   ���������ֽ�����
*******************************************************************************/
uint8_t IICreadBytes(uint8_t SlaveAddress,uint16_t REG_Address,uint8_t len,uint8_t *data)
{
		uint8_t i = 0;
		if(!IIC_Start())
			return FALSE;
    IIC_Send_Byte(SlaveAddress); 
    if(!IIC_Wait_Ack())
		{
			IIC_Stop();
			return FALSE;
		}
    IIC_Send_Byte((uint8_t) REG_Address>>8);   
    IIC_Wait_Ack();
    IIC_Send_Byte(REG_Address&0x00ff);   
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(SlaveAddress+1);
    IIC_Wait_Ack();

		for(i = 0;i<len;i++)
		{
			if(i != (len -1))
			{
				data[i]= IIC_Read_Byte();
				IIC_Ack();
			}
			else
			{
				data[i]= IIC_Read_Byte();
				IIC_NAck();
			}
		}
		IIC_Stop();
		return len;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8_t IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data)
*��������:	    ������ֽ�д��ָ���豸 ָ���Ĵ���
����	dev  Ŀ���豸��ַ
		reg	  �Ĵ�����ַ
		length Ҫд���ֽ���
		*data  ��Ҫд�����ݵ��׵�ַ
����   ����д�볤��
*******************************************************************************/
uint8_t IICwriteBytes(uint8_t dev, uint16_t reg, uint16_t length, uint8_t* data)
{

 	uint8_t count = 0;
	IIC_Start();
	IIC_Send_Byte(dev);	   
	IIC_Wait_Ack();
	IIC_Send_Byte(reg>>8);   
    IIC_Wait_Ack();
	IIC_Send_Byte(reg & 0x00ff);   
    IIC_Wait_Ack();
	for(count=0;count<length;count++)
	{
		IIC_Send_Byte(data[count]);
		IIC_Wait_Ack();
	 }
	IIC_Stop();

    return length; //status == 0;
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		unsigned char IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
*��������:	    д��ָ���豸 ָ���Ĵ���һ���ֽ�
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		data  ��Ҫд����ֽ�
����   1
*******************************************************************************/
uint8_t IICwriteByte(uint8_t dev, uint16_t reg, uint8_t data)
{
    return IICwriteBytes(dev, reg, 1, &data);
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:		uint8_t IICwriteBit(uint8_t dev, uint8_t reg, uint8_t bitNum, uint8_t data)
*��������:	    �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �е�1��λ
����	dev  Ŀ���豸��ַ
		reg	   �Ĵ�����ַ
		bitNum  Ҫ�޸�Ŀ���ֽڵ�bitNumλ
		data  Ϊ0 ʱ��Ŀ��λ������0 ���򽫱���λ
����   �ɹ� Ϊ1
 		ʧ��Ϊ0
*******************************************************************************/
uint8_t IICwriteBit(uint8_t dev, uint16_t reg, uint8_t bitNum, uint8_t data)
{
    uint8_t b;
    IIC_ReadOneByte(dev, reg, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return IICwriteByte(dev, reg, b);
}

//------------------End of File----------------------------
