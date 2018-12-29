/**	Definition for network types

*	Prefix: NET_

*	@author Ramón Mollá
*	@version 2013-01
*/

#ifndef NET_NETWORK
#define NET_NETWORK

/**   
	* It defines the types of network connection that exist in the game
    * @param enum NET_BITRATE_TYPE which lists the connections
*/
typedef enum {
	///DSL speed. Typically 96Kbps up
	NET_BR_DSL=0,
	///Old ISDN. Typically 56Kbps
	NET_BR_ISDN,
	///Old plain analogic telephones. Typically 24Kbps
	NET_BR_PLAIN_TELEPHONE,
	//ADSL, cable modem,... several Mbps
	NET_BR_CUSTOM,
	NET_BR_MAX_BITRATE
} NET_BITRATE_TYPE;

#endif