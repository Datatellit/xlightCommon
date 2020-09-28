#ifndef __DTIT_SENSOR_MAP_H
#define __DTIT_SENSOR_MAP_H

// DataTellIt Sensor Data Index
typedef enum
{
    senDataType_None = 0,
    senDataType_Temp,       // 1
    senDataType_Humi,       // 2
    senDataType_PM25,       // 3
    senDataType_PM10,       // 4

    senDataType_CO2,        // 5
    senDataType_CH2O,       // 6
    senDataType_TVOC,       // 7
    senDataType_ALS,        // 8

    senDataType_PIR,        // 9
    senDataType_MIC,        // 10
    senDataType_VIB,        // 11
    senDataType_SMOKE,      // 12

    senDataType_DUST,       // 13
    senDataType_GAS,        // 14
    senDataType_LEAK,       // 15
    senDataType_C2H5OH,     // 16

    senDataType_MAGNET,     // 17
    senDataType_EMI,        // 18
    senDataType_COLOR,      // 19
    senDataType_SICK,       // 20
    senDataType_RADAR       // 21
    // More...
} eSensorDataType_t;

// DataTellIt Sensor Bitmap
#define DTIT_SMAP_SEN_TEMP			0x00000001				// 温度传感器
#define DTIT_SMAP_SEN_HUMI			0x00000002				// 湿度传感器
#define DTIT_SMAP_SEN_PM25			0x00000004				// PM2.5传感器
#define DTIT_SMAP_SEN_PM10			0x00000008				// PM10传感器

#define DTIT_SMAP_SEN_CO2			0x00000010				// 二氧化碳传感器
#define DTIT_SMAP_SEN_CH2O			0x00000020				// 甲醛传感器
#define DTIT_SMAP_SEN_TVOC			0x00000040				// TVOC传感器
#define DTIT_SMAP_SEN_ALS			0x00000080				// 光强度传感器

#define DTIT_SMAP_SEN_PIR			0x00000100				// 人体红外传感器
#define DTIT_SMAP_SEN_MIC			0x00000200				// 麦克风
#define DTIT_SMAP_SEN_VIB			0x00000400				// 震动传感器
#define DTIT_SMAP_SEN_SMOKE			0x00000800				// 烟雾传感器

#define DTIT_SMAP_SEN_GAS			0x00001000				// 可燃气体传感器
#define DTIT_SMAP_SEN_DUST			0x00002000				// 粉尘传感器
#define DTIT_SMAP_SEN_LEAK			0x00004000				// 漏水传感器
#define DTIT_SMAP_SEN_C2H5OH		0x00008000				// 酒精传感器

#define DTIT_SMAP_SEN_MAGNET		0x00010000				// 磁场传感器
#define DTIT_SMAP_SEN_EMI			0x00020000				// 电磁场传感器
#define DTIT_SMAP_SEN_COLOR			0x00040000				// 颜色传感器
#define DTIT_SMAP_SEN_SICK			0x00080000				// 咳嗽打喷嚏、噪声

#define DTIT_SMAP_SEN_RADAR			0x00100000				// 超声波雷达（数据为毫米距离）

#define DTIT_SENSOR_EIGHT_IN_ONE	0x000000FF				// 八合一

#endif /* __DTIT_SENSOR_MAP_H */
