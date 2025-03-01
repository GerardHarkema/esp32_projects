#ifndef NETWORK_CONFIG_H
#define NETWORK_CONFIG_H


typedef struct{
  String ssid;
  String password;
}NETWORK_CONFIG;

bool configureNetwork(bool forceConfigure, NETWORK_CONFIG *networkConfig);

#endif