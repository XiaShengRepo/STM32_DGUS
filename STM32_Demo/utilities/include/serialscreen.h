#ifndef __serialscreen___
#define  __serialscreen___
#include "stdint.h"
/*֡��һЩ�ֽڶ���*/
#define FH0 0x5A 
#define FH1 0xA5
#define RD_LEN 0x00
#define ADR 0x00

#define Logo_interface                 0x00  /*logo����*/
#define Err_interface                  0x01  /*������ʾ����*/
#define DoorOpened_interface           0x03  /*�Ŵ򿪽���*/
#define OnlymachieInit_interface       0x04  /*��е�ֳ�ʼ������*/
#define SignInFunction_interface       0x05  /*����ǩ������*/
#define Szt_GpbocAutoCheckIn_interface 0x06  /*������ǩ������*/
#define SellMeal_TimeSet_interface     0x07  /*�۲�ʱ�����ý���*/
#define SellMeal_TimeWait_interface    0x09  /*�۲͵ȴ�����*/
#define Coinset_interface              0x0B  /*Ӳ������*/
#define MealInput_interface            0x0D  /*�Ų�����ѡ��*/
#define Acount_interface               0x0F  /*�������*/
#define TicketPrint_interface          0x12  /*СƱ��ӡ����*/
#define Mealout_interface              0x14  /*���ͽ�����ʾ*/
#define Password_interface             0x15  /*�����������*/
#define TemperatureSet_interface       0x17  /*�¶����ý���*/
#define Data_synchronization           0x19  /*����ͬ��*/
#define Cardbalence_interface          0x1D  /*ˢ������*/
#define UserAbonamalRecord_interface   0x1F  /*�쳣�û���¼����*/

#define Menu1st_interface              0x21  /*�˵�һ��Ʒ��������*/
#define MealSet1st_interface           0x23  /*�˵�һ��Ʒ���ý���*/
#define Menu2nd_interface              0x25  /*�˵�����Ʒ��������*/
#define MealSet2nd_interface           0x27  /*�˵�����Ʒ���ý���*/
#define Menu3rd_interface              0x29  /*�˵�����Ʒ��������*/
#define MealSet3rd_interface           0x2b  /*�˵�����Ʒ���ý���*/
#define Menu4th_interface              0x2d  /*�˵��Ĳ�Ʒ��������*/
#define MealSet4th_interface           0x2f  /*�˵��Ĳ�Ʒ���ý���*/
#define Menu5th_interface              0x31  /*�˵����Ʒ��������*/
#define MealSet5th_interface           0x33  /*�˵����Ʒ���ý���*/

#define Meal1st_interface              0x35  /*��Ʒһ������*/
#define Meal2nd_interface              0x37  /*��Ʒ��������*/
#define Meal3rd_interface              0x39  /*��Ʒ��������*/
#define Meal4th_interface              0x3B  /*��Ʒ��������*/
#define Meal5th_interface              0x3D  /*��Ʒ��������*/
#define Meal6th_interface              0x3F  /*��Ʒ��������*/
#define Meal7th_interface              0x41  /*��Ʒ��������*/
#define Meal8th_interface              0x43  /*��Ʒ��������*/



/*�Ĵ����ռ䶨��*/
#define Version 0x00  /*DGUS�汾��*/
#define LED_NOW 0X01  /*LED���ȿ��ƼĴ�����0x00~0x40*/
#define BZ_IME  0X02  /*���������п��ƼĴ�������λ10ms*/
#define PIC_ID  0x03  /*2�ֽ� ������ǰ��ʾҳ��ID д���л���ָ��ҳ��(ҳ�����)*/
#define TP_Flag 0x05  /*0x5a=�����������и��£���������������δ����*/
#define TP_Status 0x06  /*0x01=��һ�ΰ��� 0x03һֱ��ѹ�� 0x02=̧�� ����=��Ч*/
#define TP_Position 0x07 /*4�ֽ� ��������ѹ����λ�ã�X_H:L Y_H:L*/
#define TPC_Enable 0x0B /*0x00=���ز����� ����=����*/

#define ScreenEnable  0x01 
#define ScreenDisable 0x00

/*ʣ���Ʒ������ַ*/
#define meal1st_left     0x0020 /*��һ�ݲ�Ʒʣ���λ��*/
#define meal2nd_left	   0x0021 /*�ڶ��ݲ�Ʒʣ���λ��*/
#define meal3rd_left     0x0022 /*�����ݲ�Ʒʣ���λ��*/
#define meal4th_left     0x0023 /*���ķݲ�Ʒʣ���λ��*/
#define meal5th_left     0x0024 /*����ݲ�Ʒʣ���λ��*/
#define meal6th_left     0x0025 /*�����ݲ�Ʒʣ���λ��*/
#define meal7th_left     0x0026 /*���߷ݲ�Ʒʣ���λ��*/
#define meal8th_left     0x0027 /*�ڰ˷ݲ�Ʒʣ���λ��*/
//
/*װ��ѡ��,1:����,2:���,3:ȷ��,4:ȡ��,5:����*/
#define mealinput_choose  0x001A

/*������������ַ*/
#define err_num           0x0007

/*ϵͳ����,1:�¶�����;2:��Ʒ����*/
#define admin_set         0x0006

/*���볤���ı���ʾ*/
#define password_show     0x0005 

/*�������������ַ*/
#define password          0x0004 

/*Ʊ�ݴ�ӡ������ַ,1:��;2:��*/
#define bill_print        0x0003 

/*֧����ʽ������ַ,1:�ֽ�֧��;2:���п�֧��;3:����֧ͨ��*/
#define payment_method    0x0002

/*ѡ�ͽ���ѡ�������ַ:��������ѡ��*/
#define mealcnt_choose    0x0001

/*������ѡ�ͽ���*/
#define meal_choose       0x0000

/*��������ʾ*/
#define  payment_coin     0x0500 //Ӳ��֧�����
#define  payment_bill     0x0501 //ֽ��֧�����
#define  payment_card     0x0502 //ˢ��֧�����
#define  wait_payfor      0x0503 //����ȴ�ʱ��

/*���ͽ������*/
#define  mealout_totle    0x0504 //��Ʒ��������
#define  mealout_already  0x0505 //��Ʒ�ѳ�������

/*������ʾ*/
#define erro_num          0x050A //�۲�ֹͣ����

/*���ﳵ��ʾ*/
#define column1st_name  0x0100  //��һ����Ʒ��
#define column1st_count 0x0110  //��һ����Ʒ����
#define column1st_cost  0x0120  //��һ����Ʒ�ܼ�
#define column2nd_name  0x0200  //�ڶ�����Ʒ��
#define column2nd_count 0x0210  //�ڶ�����Ʒ����
#define column2nd_cost  0x0220  //�ڶ�����Ʒ�ܼ�
#define column3rd_name  0x0300  //��������Ʒ��
#define column3rd_count 0x0310  //��������Ʒ����
#define column3rd_cost  0x0320  //��������Ʒ�ܼ�
#define column4th_name  0x0400  //��������Ʒ��
#define column4th_count 0x0410  //��������Ʒ����
#define column4th_cost  0x0420  //��������Ʒ�ܼ�

#define  mealtotoal_cost     0x0506  //�ܼ�
#define  current_temprature  0x0507  //ʵʱ�¶���ʾ����
#define  temprature_set      0x0508  //�¶����ñ���
#define  count_dowm          0x0509  //ѡ�͵���ʱ

/*��ҳ��Ʒ������ʾ*/
//��һ�ݲ�Ʒѡ�����
#define meal1st_cnt     0x0030  
#define meal1st_cost    0x0031   
//�ڶ��ݲ�Ʒѡ�����
#define meal2nd_cnt     0x0032
#define meal2nd_cost    0x0033
//�����ݲ�Ʒѡ�����
#define meal3rd_cnt     0x0034
#define meal3rd_cost    0x0035
//���ķݲ�Ʒѡ�����
#define meal4th_cnt     0x0036
#define meal4th_cost    0X0037
//����ݲ�Ʒѡ�����
#define meal5th_cnt     0x0038
#define meal5h_cost     0X0039
//�����ݲ�Ʒѡ�����
#define meal6th_cnt     0x003A
#define meal6th_cost    0X003B
//���߷ݲ�Ʒѡ�����
#define meal7th_cnt     0x003C
#define meal7th_cost    0X003D
//�ڰ˷ݲ�Ʒѡ�����
#define meal8th_cnt     0x003E
#define meal8th_cost    0X003F

/*��Ʒװ�����*/
#define row_1st   0x0011 
#define row_2nd   0x0012
#define row_3rd   0x0013
#define floor_num 0x0014
#define meal_num  0x0015

//�˱ұ���
#define coins_in     0x0050 //�˱һ��ڵ�Ӳ������   
#define coins_input  0x0051 //Ӳ�ҷ�������
#define coins_retain 0x0052 //����Ӳ������
#define coins_back	 0x0053 //��ǰ�˱�����
#define coins_key    0x0054 //�˱ҵİ���

#define caedbalence_cancel    0x0055  //ˢ��ȡ������
#define record_clear          0x0056  //�����¼����

//ˢ��������
#define cardbalence_before    0x0060  //ˢ��Ǯ���
#define amountof_consumption  0x0065  //�������ѽ��
#define cardbalence_after     0x006A  //ˢ�������

//ͬ������
#define sync_column1st_name   0x4500
#define sync_column2nd_name   0x4600
#define sync_column3rd_name   0x4700
#define sync_column4th_name   0x4800
#define sync_column1st_number 0x4900
#define sync_column2nd_number 0x4A00
#define sync_column3rd_number 0x4B00
#define sync_column4th_number 0X4C00

//�۲�ʱ�����ý���
#define set_sellmeal_hour     0x4E00    //�۲�ʱ�����õ�Сʱ����
#define set_sellmeal_minute   0x4E01    //�۲�ʱ�����õķ��ӱ���
#define set_sellmeal_key      0x4E02    //�۲�ʱ�����õİ�������

//�ȴ��۲͵�ʱ����ʾ����
#define wait_sellmeal_hour    0x4E03    //�ȴ��۲���ʾ��Сʱ����
#define wait_sellmeal_minute  0x4E04    //�ȴ��۲���ʾ�ķ��ӱ���
#define wait_sellmeal_second  0x4E05    //�ȴ��۲���ʾ�����ӱ���

//�û������¼����
#define record_column1st_name   0x5000 //��һ������
#define record_column2nd_name   0x5100 //�ڶ�������
#define record_column3rd_name   0x5200 //����������
#define record_column4th_name   0x5300 //����������

#define record_column1st_cnt_t  0x5400 //��һ����Ʒ����
#define record_column2nd_cnt_t  0x5500 //�ڶ�����Ʒ������
#define record_column3rd_cnt_t  0x5600 //��������Ʒ������
#define record_column4th_cnt_t  0x5700 //��������Ʒ������

#define record_column1st_cnt    0x5800 //��һ����Ʒ�ѳ�������
#define record_column2nd_cnt    0x5900 //�ڶ�����Ʒ�ѳ�����
#define record_column3rd_cnt    0x5A00 //��������Ʒ�ѳ�����
#define record_column4th_cnt    0x5B00 //��������Ʒ�ѳ�����

#define record_UserActPayAlready     0x5C00  //�û��Ѹ���
#define record_UserActPayBack        0x5D00  //�û�Ӧ�˱�
#define record_UserActPayBackAlready 0x5E00  //�����û�Ӳ����

#define record_UserDataTime          0x5F00  //����ʱ����ʾ


extern uint8_t sell_type[4];  //�洢�۲�ID������
extern uint8_t sell_type_1st[4]; 
extern uint8_t sell_type_2nd[4]; 
extern uint8_t sell_type_3rd[4]; 
extern uint8_t sell_type_4th[4]; 
extern uint8_t sell_type_5th[4];
extern uint8_t Menu_interface;//��ǰ��ʾ���۲ͽ���
extern uint8_t Menuset_interface; //��ǰ��ʾ����ͽ���
extern const char price_1st;
extern const char price_2nd;
extern const char price_3rd;
extern const char price_4th;
extern const char price_5th;
extern const char price_6th;
extern const char price_7th;
extern const char price_8th;
extern bool cardbalence_cancel_flag;
extern bool sellmeal_flag; 
extern char record_time[20];
extern int16_t CoinTotoal_t;
extern int8_t	selltime_hour,selltime_hour_t,selltime_hour_r;
extern int8_t	selltime_minute, selltime_minute_t,selltime_minute_r;
extern int8_t  selltime_second_r;
extern uint32_t sellsecond_remain;
void ClearUserBuffer(void);
void PageChange(char page);
void ReadPage(void);
void ScreenControl(char cmd);
void DispLeftMeal(void);
void DealSeriAceptData(void);
void VariableChage(uint16_t Variable,uint16_t Value);	
void DisplayAbnormal(char *abnomal_code);
void VariableChagelong (uint16_t Variable,uint32_t Value);
void DisplayRecordTime(void);
void DisplayTimeCutDown(void);
void SetScreenRtc(void);
void MenuChange(uint8_t MenuNO);
unsigned char GetMealPrice(char meal_type,char count);

#endif
