#ifndef Q3DGS_H
#define Q3DGS_H

// Q3DGS
const uint8_t kSystemId = 1;
const uint8_t kComponentId = 1;

// UAV
const uint8_t kTargetSystemId = 1;
const uint8_t kTargetComponentId = 1;

typedef enum MAVLinkCmd{
  UN_LOCK,
  LOCK,
  LOITER,
  RTL,
  LAND,
} MAVLinkCmd;

typedef struct Params {
  float param1 = 0.0;
  float param2 = 0.0;
  float param3 = 0.0;
  float param4 = 0.0;
  float param5 = 0.0;
  float param6 = 0.0;
  float param7 = 0.0;
} Params;


#endif // Q3DGS_H
