#ifndef _VOICE_H
#define _VOICE_H

#define VOICE_1    0x00   //��ѡ�����
#define VOICE_2    0x01   //��ѡ�񸶿ʽ
#define VOICE_3    0x02   //��Ͷ��
#define VOICE_4    0x03   //��ˢ��
#define VOICE_5    0x04   //�뽫IC������ˢ������
#define VOICE_6    0x05   //�������㣬���ֵ
#define VOICE_7    0x06   //��ѡ���Ƿ��ӡСƱ
#define VOICE_8    0x07   //��������ȴ�
#define VOICE_9    0x08   //��ȡ��
#define VOICE_10   0x09   //����ȡ�߳��ͿڵĲͺ�
#define VOICE_11   0x0A   //���ͳ�������ϵ����Ա
#define VOICE_12   0X0B   //��ӭ�´ι���
#define VOICE_13   0X0C   //��ѡ�����


void PlayMusic(char Data);
void InitVoice(void);
#endif
