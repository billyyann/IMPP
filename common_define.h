#pragma once
#pragma pack(push)
#pragma pack(1)
#include <cstdint>
#include <string>
#include <sstream>



struct TIMESTAMP
{
	uint16_t _year = 2022;
	uint16_t _mon = 6;
	uint16_t _day = 1;
	uint16_t _hour = 0;
	uint16_t _min = 0;
	uint16_t _sec = 0;
	float _ms = 0;

	double toDouble() {
		double seconds_since_epoch = (_year - 1970) * 31556952 + (_mon - 1) * 2629744 + _day * 86400 + _hour * 3600 + _min * 60 + _sec + _ms / 1000;
		return seconds_since_epoch;
	};

	std::string toString() {
		std::stringstream ss;
		ss << _year << '-' << _mon << '-' << _day << ' ' << _hour << ':' << _min << ':' << _sec;
		return ss.str();
	}
};

//����äԪ����
typedef struct CommBlindProc
{
	uint32_t					SatID;											//���Ǳ�ʶ
	uint32_t					PayloadID;										//�غɱ�ʶ
	uint32_t					WaveBandID;										//���α�ʶ
	TIMESTAMP			DateTimeStamp;									//��������ʱ��
	uint32_t					IntegralTime;									// ������Чʱ������(��λ����)
	char					ChannelGain[64];								// ͨ������
	char					CapacityState;									// ̽�������ֵ���״̬
	uint16_t            width;
	uint16_t            height;
	uint16_t				Reserve[48];
}COMMBLINDPROC;
//���äԪ��0��ʾ��Ԫ��2Ϊ��Ԫ��3Ϊ��Ԫ��1Ϊ��äԪ������������

//�Ǿ���У������(NonUniformCheckPara)- �����Ǿ���У������
typedef struct CommNonUniformCheckPara
{
	int					SatID;														//���Ǳ�ʶ
	int					PayloadID;													//�غɱ�ʶ
	int					WaveBandID;													//���α�ʶ
	int					ChannelID;													//ͨ����ʶ
	TIMESTAMP			DateTimeStamp;												//����ʱ��
	short				DynamicRangeID;												//�غɶ�̬��Χ��ʶ->�Ҷ�ֵֵ��Χ
	int					IntegralTime;												// ����ʱ��(��λ��΢��)
	char				ChannelGain[64];											// ͨ������:��ɨ�������ǰ32�ֽ�Ϊ�в�4ƬASICͨ�����棬��32�ֽ�Ϊ�̲�4ƬASICͨ������,���������������һ�ֽ���Ч��
	char				CapacityState;												// ̽�������ֵ���״̬:���غɱ�ʶΪ�����������Ч
	uint16_t            width;
	uint16_t            height;
	uint16_t				Reserve[48];												// �����ֶ�
}COMMNONUNIFORMCHECKPARA;
//�Ǿ���У������:���һά��������������Ϊ���ζ���ʽ��0�Ρ�1�μ�2����ϵ��



enum SAT_NAME :uint32_t
{
	HY0000 = 0x0000,	//�����ں�
	HY1001 = 0x1001,	//DDYJ����G01��
	HY1002 = 0x1002,	//DDYJ����G02��
	HY1003 = 0x1003,	//DDYJ����G03��
	HY1004 = 0x1004,	//DDYJ����G04��
	HY1101 = 0x1101,	//DDYJ����H01��
	HY1102 = 0x1102,	//DDYJ����H02��
	HY1301 = 0x1301,	//IGSODDYJ����KS01
	HY1401 = 0x1401,	//�е���ʾ��֤����L01�ǣ�A�ǣ�
	HY1402 = 0x1402,	//�е���ʾ��֤����L02�ǣ�B�ǣ�
	HY1403 = 0x1403	//�е���ʾ��֤����L03�ǣ�C�ǣ�
};

enum SENSOR_NAME :uint32_t
{
	//	BHA1=0xA1,		//A���Ǵ��ӳ��������
	//	GFA2=0xA2,      //A���Ǹ߷ָ������
	//	JMA3=0xA3,      //A���Ǿ��ܸ������
	//	HSA4=0xA4,      //A���ǻ�ɨ�������
	//	BHB1=0xB1,      //B���Ǵ��ӳ��������
	//	DPB2=0xB2,      //B���Ǻ�����׶����
	//	DKB3=0xB3,      //B���Ǵ�ھ��������
	//	YDC1=0xC1,      //C���������������
	//	DPC2=0xC2,      //C���Ǻ�����׶����
	//	HCC3=0xC3,      //C���Ǻ��ⳤ�����
	//	JGC4=0xC4,       //C���Ǽ��������
	//	UNKS=0xFF
	YDXJ = 0x01, //B dsc C dsc
	LGXJ = 0x02, // B dpd, C dpd
	DGXJ = 0x03, // B dkj, C dkj
	DJXJ = 0x04,
	JGXJ = 0x05,
	UNKS = 0x00

};

enum BAND_NAME :uint32_t
{
	DBTC = 0x01,		//�̲�̽���׶�
	ZBTC = 0x02,      //�в�̽�Ⲩ��
	ZCTC = 0x03,      //�г�������
	CBTC = 0x04,      //��������
	KJGB = 0x05,      //�ɼ��Ⲩ��
	RMZW = 0x06,      //��ä����
	JZWD = 0x07,      //������
	JGTC = 0x08,      //����
	DPD1 = 0x11,      //������׶�1
	DPD2 = 0x12,      //������׶�2
	DPD3 = 0x13,      //������׶�3
	DPD4 = 0x14,      //������׶�4
	DPD5 = 0x15,      //������׶�5
	DPD6 = 0x16,      //������׶�6
	DPD7 = 0x17,      //������׶�7
	DPD8 = 0x18,       //������׶�8
	UNKB = 0xFF
};


typedef struct _DataMeta
{
	SAT_NAME _SatID;	//���Ǳ�ʶ
	SENSOR_NAME _SensorID;		//�غɱ�ʶ
	BAND_NAME _BandID;		//̽�������α�ʶ
	uint32_t _ChannelID;	//ͨ����ʶ��1ֱ����2����ѹ��

}DataMeta;


typedef struct DATETIME
{
	uint16_t _Year = 2022;
	uint16_t _Month = 6;
	uint16_t _Day = 1;
	uint16_t _Hour = 0;
	uint16_t _Minute = 0;
	uint16_t _Second = 0;

}TIME;


struct TEMPERATURE {
	char _Dummy1[72];
	char temp1[2];
	char temp2[2];
	char temp3[2];
	char _Dummy2[434];
};

struct DBLPOS
{
	double _x;
	double _y;
	double _z;
};
struct DBLVEL
{
	double _Vx;
	double _Vy;
	double _Vz;
};
struct DBLATT
{
	double _Roll;
	double _Yaw;
	double _Pitch;
};

struct ORBIT_POS
{
	DBLPOS _p;
	DBLVEL _v;
};

typedef struct _FrameHeader
{
	TIMESTAMP 	_ImageTime;
	TIMESTAMP   _RecvTime;
	uint32_t	_FrameNum;
	uint32_t	_WorkMode;		//01:����ģʽ,02:�Եس���ģʽ,03:�������ģʽ
	char		_PlatCtlMode;	//B ��̬�ȶ����Ʒ�
	char		_ChangeDir;
	char 		_PlatWorkState;
	char		_RotPlatState;
	char		_CamRotPlatMode;
	char		_RotPlatStableID;
	int32_t		_CompMirrV;
	double		_RotPlatV[2];
	//char		_CalibInstPos;
	char		_Gain;
	//double		_VISGain;
	int32_t		_IT;			//΢��
	int32_t		_FrameSpace;
	TEMPERATURE _temperature;
	double		_RotPlatAZ;
	double		_RotPlatEL;
	double		_CompMirrAng;
	ORBIT_POS	_OrbPos;
	double		_QuaterNion[3];				//�����⣬3��double ������Ԫ����
	DBLATT		_SatAttVel;
	DBLPOS		_SolarPos;
	double		_RotPlatQun[4];				//ת̨��Ԫ����δ����
	char		_Reserve2[620];             // �°����ݸ�ʽ
	uint16_t	_Lines;
	uint16_t	_Cols;
} FrameHeader;


typedef struct _ImageHeader {
	DataMeta dataMeta;
	FrameHeader frameHeader;
}ImageHeader;
#pragma pack(pop)


