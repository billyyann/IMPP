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

//公共盲元参数
typedef struct CommBlindProc
{
	uint32_t					SatID;											//卫星标识
	uint32_t					PayloadID;										//载荷标识
	uint32_t					WaveBandID;										//波段标识
	TIMESTAMP			DateTimeStamp;									//参数生成时间
	uint32_t					IntegralTime;									// 参数有效时间上限(单位：秒)
	char					ChannelGain[64];								// 通道增益
	char					CapacityState;									// 探测器积分电容状态
	uint16_t            width;
	uint16_t            height;
	uint16_t				Reserve[48];
}COMMBLINDPROC;
//相机盲元：0表示暗元，2为亮元，3为闪元，1为非盲元，其他保留。

//非均匀校正参数(NonUniformCheckPara)- 公共非均匀校正参数
typedef struct CommNonUniformCheckPara
{
	int					SatID;														//卫星标识
	int					PayloadID;													//载荷标识
	int					WaveBandID;													//波段标识
	int					ChannelID;													//通道标识
	TIMESTAMP			DateTimeStamp;												//数据时标
	short				DynamicRangeID;												//载荷动态范围标识->灰度值值域范围
	int					IntegralTime;												// 积分时间(单位：微秒)
	char				ChannelGain[64];											// 通道增益:对扫描相机，前32字节为中波4片ASIC通道增益，后32字节为短波4片ASIC通道增益,对凝视相机，仅第一字节有效；
	char				CapacityState;												// 探测器积分电容状态:若载荷标识为凝视相机，无效
	uint16_t            width;
	uint16_t            height;
	uint16_t				Reserve[48];												// 保留字段
}COMMNONUNIFORMCHECKPARA;
//非均匀校正参数:最后一维数据组数据依次为二次多项式的0次、1次及2次项系数



enum SAT_NAME :uint32_t
{
	HY0000 = 0x0000,	//多星融合
	HY1001 = 0x1001,	//DDYJ卫星G01星
	HY1002 = 0x1002,	//DDYJ卫星G02星
	HY1003 = 0x1003,	//DDYJ卫星G03星
	HY1004 = 0x1004,	//DDYJ卫星G04星
	HY1101 = 0x1101,	//DDYJ卫星H01星
	HY1102 = 0x1102,	//DDYJ卫星H02星
	HY1301 = 0x1301,	//IGSODDYJ卫星KS01
	HY1401 = 0x1401,	//中低演示验证卫星L01星（A星）
	HY1402 = 0x1402,	//中低演示验证卫星L02星（B星）
	HY1403 = 0x1403	//中低演示验证卫星L03星（C星）
};

enum SENSOR_NAME :uint32_t
{
	//	BHA1=0xA1,		//A卫星大视场捕获相机
	//	GFA2=0xA2,      //A卫星高分跟踪相机
	//	JMA3=0xA3,      //A卫星精密跟踪相机
	//	HSA4=0xA4,      //A卫星环扫监视相机
	//	BHB1=0xB1,      //B卫星大视场捕获相机
	//	DPB2=0xB2,      //B卫星红外多谱段相机
	//	DKB3=0xB3,      //B卫星大口径长波相机
	//	YDC1=0xC1,      //C卫星引导交接相机
	//	DPC2=0xC2,      //C卫星红外多谱段相机
	//	HCC3=0xC3,      //C卫星红外长波相机
	//	JGC4=0xC4,       //C卫星激光测距相机
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
	DBTC = 0x01,		//短波探测谱段
	ZBTC = 0x02,      //中波探测波段
	ZCTC = 0x03,      //中长波波段
	CBTC = 0x04,      //长波波段
	KJGB = 0x05,      //可见光波段
	RMZW = 0x06,      //日盲紫外
	JZWD = 0x07,      //近紫外
	JGTC = 0x08,      //激光
	DPD1 = 0x11,      //红外多谱段1
	DPD2 = 0x12,      //红外多谱段2
	DPD3 = 0x13,      //红外多谱段3
	DPD4 = 0x14,      //红外多谱段4
	DPD5 = 0x15,      //红外多谱段5
	DPD6 = 0x16,      //红外多谱段6
	DPD7 = 0x17,      //红外多谱段7
	DPD8 = 0x18,       //红外多谱段8
	UNKB = 0xFF
};


typedef struct _DataMeta
{
	SAT_NAME _SatID;	//卫星标识
	SENSOR_NAME _SensorID;		//载荷标识
	BAND_NAME _BandID;		//探测器波段标识
	uint32_t _ChannelID;	//通道标识：1直传，2无损压缩

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
	uint32_t	_WorkMode;		//01:观星模式,02:对地成像模式,03:黑体成像模式
	char		_PlatCtlMode;	//B 姿态稳定控制符
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
	int32_t		_IT;			//微秒
	int32_t		_FrameSpace;
	TEMPERATURE _temperature;
	double		_RotPlatAZ;
	double		_RotPlatEL;
	double		_CompMirrAng;
	ORBIT_POS	_OrbPos;
	double		_QuaterNion[3];				//有问题，3个double 定义四元数？
	DBLATT		_SatAttVel;
	DBLPOS		_SolarPos;
	double		_RotPlatQun[4];				//转台四元数，未定义
	char		_Reserve2[620];             // 新版数据格式
	uint16_t	_Lines;
	uint16_t	_Cols;
} FrameHeader;


typedef struct _ImageHeader {
	DataMeta dataMeta;
	FrameHeader frameHeader;
}ImageHeader;
#pragma pack(pop)


