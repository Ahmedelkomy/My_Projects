/*
 * MQTT_Interface.h
 *
 *  Created on: ???/???/????
 *      Author: Ahmed
 */

#ifndef HAL_MQTT_INTERFACE_H_
#define HAL_MQTT_INTERFACE_H_

/*--------------------------------------------------------------------------------------------*/
/*   INCLUDES   */
/*--------------------------------------------------------------------------------------------*/
#include "STD.h"
#include "DIO_Interface.h"
#include <util/delay.h>
#include <avr/io.h>
#include <string.h>



/*--------------------------------------------------------------------------------------------*/
/*   DEFINES   */
/*--------------------------------------------------------------------------------------------*/


#define MQTT_CTRL_CONNECT		0x01
#define MQTT_CTRL_CONNECTACK	0x2
#define MQTT_CTRL_PUBLISH		0x3
#define MQTT_CTRL_PUBACK		0x4
#define MQTT_CTRL_PUBREC		0x5
#define MQTT_CTRL_PUBREL		0x6
#define MQTT_CTRL_PUBCOMP		0x7
#define MQTT_CTRL_SUBSCRIBE		0x8
#define MQTT_CTRL_SUBACK		0x9
#define MQTT_CTRL_UNSUBSCRIBE	0xA
#define MQTT_CTRL_UNSUBACK		0xB
#define MQTT_CTRL_PINGREQ		0xC
#define MQTT_CTRL_PINGRESP		0xD
#define MQTT_CTRL_DISCONNECT	0xE

#define MQTT_QOS_0			0x0
#define MQTT_QOS_1			0x1
#define MQTT_QOS_2			0x2


#define MQTT_CONN_USERNAMEFLAG		0x80
#define MQTT_CONN_PASSWORDFLAG		0xc2
#define MQTT_CONN_WILLRETAIN		0x20
#define MQTT_CONN_WILLQOS_1			0x08
#define MQTT_CONN_WILLQOS_2			0x18
#define MQTT_CONN_WILLFLAG			0x04
#define MQTT_CONN_CLEANSESSION		0x02
#define will_to						0x01

#define DEFAULT_BUFFER_SIZE		200
#define DEFAULT_TIMEOUT			10000
#define DEFAULT_CRLF_COUNT		2





/*--------------------------------------------------------------------------------------------*/
/*   PROTOTYPES   */
/*--------------------------------------------------------------------------------------------*/

uint8_t *AddStringToBuf(uint8_t *_buf, const char *_string);
uint16_t MQTT_publishPacket(uint8_t *packet, const char *topic, char *data, uint8_t qos);
uint16_t MQTT_connectpacket(uint8_t* packet);
uint16_t MQTT_subscribePacket(uint8_t *packet, const char *topic, uint8_t qos);
uint16_t MQTT_unsubscribePacket(uint8_t *packet, const char *topic);
uint8_t MQTT_definePacket(char *packet,char *ack_typ);
char MQTT_puplish_read (uint8_t *packet ,char* topic ,char* data);
char MQTT_disconnect_packet(char* packet);


#endif /* HAL_MQTT_INTERFACE_H_ */


