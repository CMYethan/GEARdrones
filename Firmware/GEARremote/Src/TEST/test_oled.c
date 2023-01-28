//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//����Ӳ������Ƭ��STM32F103ZET6,����ԭ��ELITE STM32������,��Ƶ72MHZ������12MHZ
//QDtech-OLEDҺ������ for STM32
//xiao��@ShenZhen QDtech co.,LTD
//��˾��վ:www.qdtft.com
//�Ա���վ��http://qdtech.taobao.com
//wiki������վ��http://www.lcdwiki.com
//��˾�ṩ����֧�֣��κμ������⻶ӭ��ʱ����ѧϰ
//�̻�(����) :+86 0755-23594567 
//�ֻ�:15989313508���빤�� 
//����:lcdwiki01@gmail.com    support@lcdwiki.com    goodtft@163.com
//����֧��QQ:3002773612  3002778157
//��������QQȺ:324828016
//��������:2018/9/13
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������ȫ�����Ӽ������޹�˾ 2018-2028
//All rights reserved
/****************************************************************************************************
//=========================================��Դ����================================================//
// OLEDģ��               STM32��Ƭ��
//   VCC         ��       DC 5V/3.3V      //OLED����Դ��
//   GND         ��          GND          //OLED����Դ��
//=======================================Һ���������߽���==========================================//
//��ģ��Ĭ��������������ΪIIC
// OLEDģ��               STM32��Ƭ��
//   SDA         ��          PB15         //OLED��IIC�����ź�
//=======================================Һ���������߽���==========================================//
// OLEDģ��               STM32��Ƭ��
//   SCL         ��          PB13         //OLED��IICʱ���ź�
//=========================================����������=========================================//
//��ģ�鲻���������ܣ����Բ���Ҫ����������
****************************************************************************************************/	
/***************************************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
****************************************************************************************************/		

#include "test_oled.h"


/*****************************************************************************
 * @name       :void TEST_MainPage(void)
 * @date       :2018-08-27 
 * @function   :Drawing the main Interface of the Comprehensive Test Program
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TEST_MainPage(void)
{	
	GUI_ShowString(37,0,"OLED TEST",8,1);
	GUI_ShowString(25,8,"0.91\" SSD1306",8,1);
	GUI_ShowString(46,16,"32X128",8,1);
	GUI_ShowString(19,24,"www.lcdwiki.com",8,1);
	Delay_MS(1500);		
	Delay_MS(1500);
}

/*****************************************************************************
 * @name       :void Test_Color(void)
 * @date       :2018-08-27 
 * @function   :Color fill test(white,black)
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Color(void)
{
	 GUI_Fill(0,0,WIDTH-1,HEIGHT-1,0);
	 GUI_ShowString(10,10,"BLACK",16,1);
	 Delay_MS(1000);	
	 GUI_Fill(0,0,WIDTH-1,HEIGHT-1,1);
	 GUI_ShowString(10,10,"WHITE",16,0);
	 Delay_MS(1500);
}

/*****************************************************************************
 * @name       :void Test_Rectangular(void))
 * @date       :2018-08-27
 * @function   :Rectangular display and fill test
								Display black,white rectangular boxes in turn,1000 
								milliseconds later,Fill the rectangle in black,white in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Rectangular(void)
{
	GUI_Fill(0,0,WIDTH/2-1,HEIGHT-1,0);
	GUI_Fill(WIDTH/2,0,WIDTH-1,HEIGHT-1,1);
	GUI_DrawRectangle(5, 5, WIDTH/2-1-5, HEIGHT-1-5,1);
	GUI_DrawRectangle(WIDTH/2-1+5, 5, WIDTH-1-5, HEIGHT-1-5,0);
	Delay_MS(1000);
	GUI_FillRectangle(5, 5, WIDTH/2-1-5, HEIGHT-1-5,1);
	GUI_FillRectangle(WIDTH/2-1+5, 5, WIDTH-1-5, HEIGHT-1-5,0);
	Delay_MS(1500);
}


/*****************************************************************************
 * @name       :void Test_Circle(void)
 * @date       :2018-08-27 
 * @function   :circular display and fill test
								Display black,white circular boxes in turn,1000 
								milliseconds later,Fill the circular in black,white in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Circle(void)
{
	GUI_Fill(0,0,WIDTH/2-1,HEIGHT-1,0);
	GUI_Fill(WIDTH/2,0,WIDTH-1,HEIGHT-1,1);
	GUI_DrawCircle(WIDTH/2/2-1, HEIGHT/2-1, 1, HEIGHT/2-3);
	GUI_DrawCircle(WIDTH/2+WIDTH/2/2-1, HEIGHT/2-1, 0, HEIGHT/2-3);
	Delay_MS(1000);
	GUI_FillCircle(WIDTH/2/2-1, HEIGHT/2-1, 1, HEIGHT/2-3);
	GUI_FillCircle(WIDTH/2+WIDTH/2/2-1, HEIGHT/2-1, 0, HEIGHT/2-3);
	Delay_MS(1500);
}

/*****************************************************************************
 * @name       :void Test_Triangle(void)
 * @date       :2018-08-27 
 * @function   :triangle display and fill test
								Display black,white triangle boxes in turn,1000 
								milliseconds later,Fill the triangle in black,white in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Triangle(void)
{
	GUI_Fill(0,0,WIDTH/2-1,HEIGHT-1,0);
	GUI_Fill(WIDTH/2,0,WIDTH-1,HEIGHT-1,1);
	GUI_DrawTriangel(5,HEIGHT-1-5,WIDTH/2/2-1,5,WIDTH/2-1-5,HEIGHT-1-5,1);
	GUI_DrawTriangel(WIDTH/2-1+5,HEIGHT-1-5,WIDTH/2+WIDTH/2/2-1,5,WIDTH-1-5,HEIGHT-1-5,0);
	Delay_MS(1000);
	GUI_FillTriangel(5,HEIGHT-1-5,WIDTH/2/2-1,5,WIDTH/2-1-5,HEIGHT-1-5,1);
	GUI_FillTriangel(WIDTH/2-1+5,HEIGHT-1-5,WIDTH/2+WIDTH/2/2-1,5,WIDTH-1-5,HEIGHT-1-5,0);
	Delay_MS(1500);
}


/*****************************************************************************
 * @name       :void TEST_English(void)
 * @date       :2018-08-27 
 * @function   :English display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TEST_English(void)
{
	GUI_ShowString(0,8,"6x8:abcdefghijklmnopqrstuvwxyz",8,1);
	Delay_MS(1000);
	GUI_ShowString(0,0,"8x16:abcdefghijklmnopqrstuvwxyz",16,1);
	Delay_MS(1000);
	OLED_Clear(0); 
	GUI_ShowString(0,8,"6x8:ABCDEFGHIJKLMNOPQRSTUVWXYZ",8,1);
	Delay_MS(1000);
	GUI_ShowString(0,0,"8x16:ABCDEFGHIJKLMNOPQRSTUVWXYZ",16,1);
	Delay_MS(1500);
}

/*****************************************************************************
 * @name       :void TEST_Number_Character(void) 
 * @date       :2018-08-27 
 * @function   :number and character display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TEST_Number_Character(void) 
{
	GUI_Fill(0,0,WIDTH-1,HEIGHT/2-1,0);
	GUI_ShowString(0,0,"6x8:!\"#$%&'()*+,-./:;<=>?@[]\\^_`~{}|",8,1);
	GUI_ShowNum(68,8,1234567890,10,8,1);
	GUI_Fill(0,HEIGHT/2,WIDTH-1,HEIGHT-1,1);
	GUI_ShowString(0,HEIGHT/2+1,"6x8:!\"#$%&'()*+,-./:;<=>?@[]\\^_`~{}|",8,0);
	GUI_ShowNum(68,HEIGHT/2+9,1234567890,10,8,0);
	Delay_MS(1000);
	OLED_Clear(0); 
  GUI_ShowString(0,0,"8x16:!\"#$%&'()*+,-./:;<=>?@[]\\^_`~{}|",16,1);
	GUI_ShowNum(48,16,1234567890,10,16,1);
	Delay_MS(1500);
	OLED_Clear(0);
}

/*****************************************************************************
 * @name       :void TEST_Chinese(void)
 * @date       :2018-08-27
 * @function   :chinese display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TEST_Chinese(void)
{	
	GUI_ShowString(45,2,"16x16",8,1);
	GUI_ShowCHinese(16,14,16,"ȫ�����Ӽ���",1);
	Delay_MS(1000);
	GUI_Fill(0,0,WIDTH-1,HEIGHT-1,1);
	GUI_ShowString(45,2,"16x16",8,0);
	GUI_ShowCHinese(16,14,16,"ȫ�����Ӽ���",0);
	Delay_MS(1000);
	OLED_Clear(0);
	GUI_ShowString(45,0,"24x24",8,1);
	GUI_ShowCHinese(16,8,24,"ȫ������",1);
	Delay_MS(1000);
	OLED_Clear(0);
	GUI_ShowCHinese(0,0,32,"ȫ������",1);	
  Delay_MS(1000);
	OLED_Clear(0);
}

/*****************************************************************************
 * @name       :void TEST_BMP(void)
 * @date       :2018-08-27 
 * @function   :BMP monochromatic picture display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TEST_BMP(void)
{
	GUI_DrawBMP(0,0,128,32, BMP2, 1);
	Delay_MS(1000);
	GUI_DrawBMP(0,0,128,32, BMP2, 0);
	Delay_MS(1000);
	GUI_DrawBMP(0,0,128,32, BMP3, 1);
	Delay_MS(1000);
	GUI_DrawBMP(0,0,128,32, BMP4, 1);
	Delay_MS(1000);
}

/*****************************************************************************
 * @name       :void TEST_Menu(void)
 * @date       :2018-08-27 
 * @function   :English weather interface display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void TEST_Menu(void)
{
	uint8_t i;
	srand(123456789);
	GUI_DrawBMP(0,0,32,32, BMP5, 1);
	GUI_ShowString(38,0,"Steps:",16,1);
	GUI_ShowString(38,16,"Cal:",16,1);
	GUI_ShowString(86,0,"8888",16,1);
	GUI_ShowString(70,16,"888",16,1);
	for(i=0;i<15;i++)
	{
		GUI_ShowNum(86,0,rand()%10+1,1,16,1);
		GUI_ShowNum(94,0,rand()%10,1,16,1);
		GUI_ShowNum(102,0,rand()%10,1,16,1);
		GUI_ShowNum(110,0,rand()%10,1,16,1);
		GUI_ShowNum(70,16,rand()%10+1,1,16,1);
		GUI_ShowNum(78,16,rand()%10,1,16,1);
		GUI_ShowNum(86,16,rand()%10,1,16,1);
    Delay_MS(500);	
	}
}



