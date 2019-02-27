#!/bin/sh
toolpath=$(pwd)
echo "export ACTUATOR_TOOL_PATH=$(pwd)" >> /etc/profile
groupadd pcap 
usermod -a -G pcap $USER
chgrp pcap ActuatorLogTool 
chmod 750 ActuatorLogTool
setcap cap_net_raw,cap_net_admin=eip ActuatorLogTool
