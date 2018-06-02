#ifndef MAVLINK_PROTOCOL_H
#define MAVLINK_PROTOCOL_H


class MAVLinkProtocol
{
public:
  MAVLinkProtocol();
  ~MAVLinkProtocol();

public:
  void HandleSerialMessage();

};

#endif // MAVLINK_PROTOCOL_H
