#ifndef _MOTORMESSAGE_H_
#define _MOTORMESSAGE_H_

struct MotorMessage
{
  int type;
  unsigned int rpm;
  int status;
  int error;
  int dcVoltage;
  int acVoltage;
};

auto constexpr tMotorMessage = 0;

#endif // _MOTORMESSAGE_H_
