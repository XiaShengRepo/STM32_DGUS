/**
  ******************************************************************************
  * �ļ�: gui_demo.c
  * ����: LeiTek (leitek.taobao.com)
  * �汾: V1.0.0
  * ����: gui demo
  ******************************************************************************
  *
  *                  ��Ȩ���� (C), LeiTek (leitek.taobao.com)
  *                                www.leitek.com
  *
  ******************************************************************************
  */


#include <stddef.h>
#include "GUI.h"
#if GUI_WINSUPPORT
#include "PROGBAR.h"
#include "LISTBOX.h"
#include "FRAMEWIN.h"
#include "BUTTON.h"
#endif
#include "gui_demo.h"
#include "stm32f10x.h"
#include "ltk_systick.h"
#include "ltk_touchscreen.h"
#include "ltk_lcd.h"
#include "ltk_debug.h"
#include "ltk_freertos.h"
#include <assert.h>
#include <stdio.h>
#include "ff.h"

#define    MAINPAGE_ID_TIMER        0
#define    MAINPAGE_ID_SETTINGS     1
#define    MAINPAGE_ID_BOOK         2
#define    MAINPAGE_ID_PHOTO        3
#define    MAINPAGE_ID_FOLDER       4
#define    MAINPAGE_ID_CALC         5
#define    MAINPAGE_ID_TASK         6
#define    MAINPAGE_ID_CONTACT      7
#define    MAINPAGE_ID_TODO         8
#define    MAINPAGE_ID_MUSIC        9
#define    MAINPAGE_ID_VIDEO        10
#define    MAINPAGE_ID_WHEATHER     11
#define    MAINPAGE_ID_TXT          12
#define    MAINPAGE_ID_PAINT        13
#define    MAINPAGE_ID_GAME         14
#define    MAINPAGE_ID_ABOUT        15

#define    GUI_ID_CLEAR             (GUI_ID_USER + 1)

WM_HWIN hAboutWM, hPaintWM, hPhotoPage;

/**
  * ����: ͨ���������ϵ����꣬ת��Ϊ��Ļ��ͼ���λ�ã�����λΪͼ��� x��
  *       ����λΪͼ��� y���磬ʱ�� ���� ����Ϊ 0x12��
  * ����: x, �������� x ����
  *       y, �������� y ����
  * ����: ͼ��λ��
  */
static uint16_t ltk_get_icon_range(uint16_t x, uint16_t y)
{
    uint8_t i, j;
    
    /* ����������Чͼ���� */
    if(x <= 8 || y <= 24)
        return 0xff;
    
    i = (x - 8) / 58;
    /* ����������Чͼ���� */
    if(x >= 58 * (i + 1))
        return 0xff;
    
    j = (y - 24) / 74;
    /* ����������Чͼ���� */
    if(y >= 74 * (j + 1))
        return 0xff;
    /* ����ͼ��λ�� */
    return (i | (j << 8));
}

/**
  * ����: �� SD ���ڵõ�����
  * ����: *p, SD ������Ҫ��ȡ���ļ����ļ���
  *       **sd_data, ���ش� SD ����ȡ������
  *       byte_num, һ�ζ�ȡ�����ݵĴ�С
  *       file_seek, �Ƿ�Ϊ��һ�ζ�ȡ
  * ����: ��
  */
int ltk_get_data(void *p, const U8 **sd_data, int byte_num, U32 file_seek)
{
    static unsigned char buffer[GUI_DRAW_ALLOC_MEM];
    uint32_t re;
    FIL *ph_file;
    
    ph_file = (FIL *)p;
    /* �����ҵ����ļ� */
    if(file_seek == 1)
        f_lseek(ph_file, 0);
    
    /* ���� fatfs ��������ȡ���ļ��ڵ����� */
    f_read(ph_file, buffer, byte_num, &re);

    /* �����淵�� */
    *sd_data = buffer;
    
    /* ���ػ��ж����ֽ���Ҫ��ȡ */
    return re;
}

/**
  * ����: ����ҳ��Ļص�����
  * ����: *pMsg�����ݸ��ص���������Ϣ
  * ����: ��
  */
static void ltk_cb_about(WM_MESSAGE* pMsg)
{
    BUTTON_Handle hButton = WM_HWIN_NULL;
    switch (pMsg->MsgId)
    {
        /* ���´�����Ϣ */
        case WM_PAINT:
            /* ���ñ���Ϊ��ɫ */
            GUI_SetBkColor(GUI_BLUE);
            GUI_Clear();
            
            /* ����Ļ�����һ���ַ�������ɫΪ��ɫ���м���룬λ��Ϊ (120, 130) */
            GUI_SetTextAlign(GUI_TA_CENTER);
            GUI_SetColor(GUI_BLACK);
            GUI_DispStringAt("leitek all in one demo", 120, 160 - 30);
            
            /* ����Ļ�����һ���ַ�������ɫΪ��ɫ���м���룬λ��Ϊ (120, 160) */
            GUI_SetTextAlign(GUI_TA_CENTER);
            GUI_DispStringAt("www.leitek.com", 120, 160);
            
            /* ����Ļ�����һ���ַ�������ɫΪ��ɫ���м���룬λ��Ϊ (120, 190) */
            GUI_SetColor(GUI_RED);
            GUI_SetTextAlign(GUI_TA_CENTER);
            GUI_DispStringAt("leitek.taobao.com", 120, 160 + 30);
            
            /* ���� OK ��ť */
            hButton = BUTTON_CreateEx(180, 280, 40, 20, hAboutWM, WM_CF_SHOW, 0, GUI_ID_OK);
            
            /* ��ť�ϵ��ַ���Ϊ OK����ɫΪ��ɫ */
            BUTTON_SetText (hButton, "OK");
            BUTTON_SetBkColor(hButton, 0, GUI_RED);
            break;
            
        /* ֪ͨ��������Ϣ */
        case WM_NOTIFY_PARENT:
            {
                /* �õ�֪ͨ��Ϣ���� */
                int n_code = pMsg->Data.v;             /* Get notification code */
                switch (n_code)
                {
                    /* �����ť�ɿ� */
                    case WM_NOTIFICATION_RELEASED:      /* React only if released */
                        /* ɾ�������ؼ� */
                        BUTTON_Delete(hButton);
                        /* ɾ�� about ���� */
                        WM_DeleteWindow(hAboutWM);
                        /* ���ر���ҳ�� */
                        ltk_ucgui_background();
                        break;
                }
            }
            break;
            
        default:
            WM_DefaultProc(pMsg);
            break;
    }
}

/**
  * ����: ����ҳ�棬�ص�����Ϊ ltk_cb_about
  * ����: ��
  * ����: ��
  */
static void ltk_about_page(void)
{
    /* ��������ҳ�� */
    hAboutWM = WM_CreateWindowAsChild(0, 0, 240, 320, NULL, WM_CF_SHOW,
                                      &ltk_cb_about, 0);
}

/**
  * ����: ��ͼҳ��Ļص�����
  * ����: *pMsg�����ݸ��ص���������Ϣ
  * ����: ��
  */
static void ltk_cb_paint(WM_MESSAGE* pMsg)
{
    GUI_PID_STATE *touch_state;
    BUTTON_Handle button_back = WM_HWIN_NULL;
    BUTTON_Handle button_clear = WM_HWIN_NULL;
    switch (pMsg->MsgId)
    {
        /* ���´�����Ϣ */
        case WM_PAINT:
            /* ���ñ���Ϊ��ɫ */
            GUI_SetBkColor(GUI_BLUE);
            GUI_Clear();
            
            /* ���� ���� ��ť */
            button_back = BUTTON_CreateEx(190, 290, 40, 20, hPaintWM,
                                          WM_CF_SHOW, 0, GUI_ID_OK);
            /* ���øð�ť�ϵ�����Ϊ back */
            BUTTON_SetText (button_back, "back");
            /* ���øð�ť�ı���Ϊ��ɫ */
            BUTTON_SetBkColor(button_back, 0, GUI_GREEN);
            
            /* ���� ���� ��ť */
            button_clear = BUTTON_CreateEx(140, 290, 40, 20, hPaintWM,
                                           WM_CF_SHOW, 0, GUI_ID_CLEAR);
            /* ���øð�ť�ϵ�����Ϊ clear */
            BUTTON_SetText (button_clear, "clear");
            /* ���øð�ť�ı���Ϊ��ɫ */
            BUTTON_SetBkColor(button_clear, 0, GUI_GREEN);
            break;
            
        /* ֪ͨ��������Ϣ */
        case WM_NOTIFY_PARENT:
            {
                /* ������µ��� ���� ���������Ұ����ͷ� */
                if((WM_GetId(pMsg->hWinSrc) == GUI_ID_OK) &&
                   (pMsg->Data.v == WM_NOTIFICATION_RELEASED))
                {
                    /* ɾ���ô����ڵ����а������ô��� */
                    BUTTON_Delete(button_back);
                    BUTTON_Delete(button_clear);
                    WM_DeleteWindow(hPaintWM);
                    /* ���ص�����ҳ�� */
                    ltk_ucgui_background();
                }
                /* ������µ��� ���� ���������Ұ����ͷ� */
                if((WM_GetId(pMsg->hWinSrc) == GUI_ID_CLEAR) &&
                   (pMsg->Data.v == WM_NOTIFICATION_RELEASED ))
                {
                    /* ��Ǹ�������Ҫ���¸��£���֪ͨ������ */
                    WM_InvalidateWindow(hPaintWM);
                    WM_NotifyParent(hPaintWM, WM_PAINT);
                }
            }
            break;

        /* ������Ϣ */
        case WM_TOUCH:
            /* �õ�������״̬ */
            touch_state = (GUI_PID_STATE *) pMsg->Data.p;
            /* ���ñ���ɫΪ��ɫ�����ڴ���λ�û��� */
            GUI_SetColor(GUI_RED);
            GL_FillCircle(touch_state->x, touch_state->y, 2);
            break;
            
        default:
            WM_DefaultProc(pMsg);
            break;
    }
}

/**
  * ����: ��ͼҳ�棬�ص�����Ϊ ltk_cb_paint
  * ����: ��
  * ����: ��
  */
static void ltk_paint_page(void)
{
    /* ������ͼҳ�� */
    hPaintWM = WM_CreateWindowAsChild(0, 0, 240, 320, NULL, WM_CF_SHOW,
                                      &ltk_cb_paint, 0);
}

/**
  * ����: ͼƬҳ��Ļص�����
  * ����: *pMsg�����ݸ��ص���������Ϣ
  * ����: ��
  */
static void ltk_cb_photo(WM_MESSAGE* pMsg)
{
    FATFS fs;
    
    FIL file;
    FRESULT res;
    switch (pMsg->MsgId)
    {
        /* ���´�����Ϣ */
        case WM_PAINT:
            /* ͼƬ�Ǵ� SPI FLASH �ڶ�ȡ�ģ�����֮ǰ������ spi ���ߣ���ֹ
               spi �����ϵ������������ʣ��� touchscreen */
            ltk_spi_lock();
            
            /* ��ʼ�� SD �������򿪿��ڵ� 2.bmp */
            res = f_mount(&fs, "", 0);
            res = f_open(&file, "/ltk/picture/2.bmp", FA_OPEN_EXISTING | FA_READ);
            if (res == FR_OK)
            {
                
                /* �� LCD ��Ļ����ʾͼƬ */
                GUI_BMP_DrawEx(ltk_get_data, &file, 0, 0);
                /* ʹ�����ر��ļ� file */
                f_close(&file); 
            }
            /* SPI SD ��ʹ����ϣ�ж�� SD �� */
            res = f_mount(NULL, "", 0);
            /* ���� SPI ���ߣ��Ա������������� */
            ltk_spi_unlock();
            break;
            
        /* ������Ϣ */
        case WM_TOUCH:
            /* ɾ����ǰ���� */
            WM_DeleteWindow(hPhotoPage);

            /* ���ص�����ҳ�� */
            ltk_ucgui_background();
            break;
            
        default:
            WM_DefaultProc(pMsg);
            break;
    }
}

/**
  * ����: ͼƬҳ�棬�ص�����Ϊ ltk_cb_photo
  * ����: ��
  * ����: ��
  */
static void ltk_photo_page(void)
{
    /* ����ͼƬҳ�� */
    hPhotoPage = WM_CreateWindowAsChild(0, 0, 240, 320, NULL, WM_CF_SHOW,
                                        &ltk_cb_photo, 0);
}

/**
  * ����: ����ҳ��Ļص�����
  * ����: *pMsg�����ݸ��ص���������Ϣ
  * ����: ��
  */
static void ltk_cb_bk_window(WM_MESSAGE *pMsg) 
{
    
    /* ���屳��ҳ��ͼ��������� */
    static char *icon_text[] = {"ʱ��",   "����",   "ͼ��",     "ͼƬ",
                                "Ŀ¼",   "������", "�������", "ͨѶ¼",
                                "����¼", "����",   "��Ƶ",     "����",
                                "���±�", "��ͼ",    "��Ϸ",    "����"
                               };
    GUI_PID_STATE *touch_state;
    FATFS fs;
    FIL file;
    FRESULT res;
    uint8_t background_icon, i, j;
    uint16_t area;

    switch (pMsg->MsgId) 
    {
        /* ���´�����Ϣ */
        case WM_PAINT:
            /* ����ͼƬ�Ǵ� SPI FLASH �ڶ�ȡ�ģ�����֮ǰ������ spi ���ߣ���ֹ
               spi �����ϵ������������ʣ��� touchscreen */
            ltk_spi_lock();
            
            /* ��ʼ�� SD �������򿪿��ڵ� background.bmp */
            res = f_mount(&fs, "", 0);
            res = f_open(&file, "/ltk/theme/mainpage/background.bmp", 
                         FA_OPEN_EXISTING | FA_READ);
            if (res == FR_OK)
            {
                /* �� LCD ��Ļ�ϻ�ͼ */
                GUI_BMP_DrawEx(ltk_get_data, &file, 0, 0);
                /* ʹ�����ر��ļ� file */
                f_close(&file); 
            }
            
            /* �������壬������������� */
            GUI_SetFont(&GUI_FontArialNarrow12);
            /* ����������ʾ��ģʽ */
            GUI_SetTextMode(GUI_TM_TRANS);
            for(j = 0; j < 4; j++)
            {
                for( i = 0; i < 4; i++)
                {
                    /* �����м俿�� */
                    GUI_SetTextAlign(GUI_TA_CENTER);
                    /* �ڲ�ͬ��λ����ʾ icon_text[] �ڵ��� */
                    GUI_DispStringAt(icon_text[i + 4 * j], 33 + 58 * i, 76 + 74 * j);
                }
            }
            /* SPI SD ��ʹ����ϣ�ж�� SD �� */
            res = f_mount(NULL, "", 0);
            /* ���� SPI ���ߣ��Ա������������� */
            ltk_spi_unlock();
            break;
            
        /* ������Ϣ */
        case WM_TOUCH:
            /* �õ�������״̬ */
            touch_state = (GUI_PID_STATE *) pMsg->Data.p;
            /* ������������д��� */
            if ((touch_state) && (!touch_state->Pressed))
            {
                /* ����������ת��Ϊҳ���ϵ�ͼ��λ�ã�����λΪ x������λΪ y */
                area = ltk_get_icon_range(touch_state->x, touch_state->y);
                if(area != 0xff)
                {
                    /* ��ͼ��λ��ת��Ϊͼ�� ID��ÿ���� 4 ��ͼ�� */
                    background_icon = (area & 0xff) + ((area >> 8) & 0xff) * 4;
                    switch (background_icon)
                    {
                        /* ������µ��� ���� ͼ�� */
                        case MAINPAGE_ID_ABOUT:
                            ltk_about_page();
                            break;
                            
                        /* ������µ��� ��ͼ ͼ�� */
                        case MAINPAGE_ID_PAINT:
                            ltk_paint_page();
                            break;

                        /* ������µ��� ͼƬ ͼ�� */
                        case MAINPAGE_ID_PHOTO:
                            ltk_photo_page();
                            break;
                    }
                }
            }
            break;
            
        default:
            WM_DefaultProc(pMsg);
            break;
    }
}

/**
  * ����: ����ҳ�棬�ص�����Ϊ ltk_cb_bk_window
  * ����: ��
  * ����: ��
  */
void ltk_ucgui_background(void) 
{
    /* ������Ϊ��ɫ */
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    /* ���ñ���ҳ��Ļص����� */
    WM_SetCallback(WM_HBKWIN, ltk_cb_bk_window);
}

/****************************** leitek.taobao.com *****************************/
