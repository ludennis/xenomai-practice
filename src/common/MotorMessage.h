#ifndef _MOTORMESSAGE_H_
#define _MOTORMESSAGE_H_

struct MotorMessage
{
  unsigned int rpm;
  int status;
  int error;
  int dcVoltage;
  int acVoltage;
};

#endif // _MOTORMESSAGE_H_
