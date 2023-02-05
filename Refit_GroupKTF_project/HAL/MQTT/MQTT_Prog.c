
/*
 * MQTT_prog.c
 *
 *  Created on: ???/???/????
 *      Author: Ahmed
 */



#include "MQTT_interface.h"


u16 packet_id_counter = 0x1;
char clientID[] ="7";
char will_topic[] = "";
char will_payload[] ="";
u8 will_qos = 0;
u8 will_retain = 0;


u8* AddStringToBuf(u8 *_buf, const char *_string)
{
	u16 _length = strlen(_string);	/* Add length and string in buffer */
	_buf[0] = _length >> 8;
	_buf[1] = _length & 0xFF;
	_buf+=2;
	strncpy((char *)_buf, _string, _length);
	return _buf + _length;
}
u16 MQTT_publishPacket(u8 *packet, const char *topic, char *data, u8 qos)
{
	u8 *_packet = packet;
	u16 _length = 0;
	u16 Datalen=strlen(data);

	_length += 2;
	_length += strlen(topic);
	if(qos > 0) {
		_length += 2;
	}
	_length += Datalen;
	_packet[0] = MQTT_CTRL_PUBLISH << 4 | qos << 1;
	_packet++;
	do {
		u8 encodedByte = _length % 128;
		_length /= 128;
		if ( _length > 0 ) {
			encodedByte |= 0x80;
		}
		_packet[0] = encodedByte;
		_packet++;
	} while ( _length > 0 );
	_packet = AddStringToBuf(_packet, topic);
	if(qos > 0) {
		_packet[0] = (packet_id_counter >> 8) & 0xFF;
		_packet[1] = packet_id_counter & 0xFF;
		_packet+=2;
		packet_id_counter++;
	}
	memmove(_packet, data, Datalen);
	_packet+= Datalen;
	_length = _packet - packet;

	return _length;
}



u16 MQTT_connectpacket(u8* packet)
{
	u8*_packet = packet;
	u16 _length;

	_packet[0] = 0x10;
	_packet+=2;
	_packet = AddStringToBuf(_packet, "MQIsdp"); /*MQIsdp cloud*/


	_packet[0] = 0x3;
	_packet[1] = 0xc2; /*c2 with password and user name */
	_packet+=2;

	_packet[0] = MQTT_CONN_KEEPALIVE >> 8;
	_packet++;
	_packet[0] = MQTT_CONN_KEEPALIVE & 0xFF;
	_packet++;
	if (strlen(clientID) != 0x0) {
		_packet = AddStringToBuf(_packet, clientID);
		} else {
		_packet[0] = 0x0;
		_packet++;
		_packet[0] = 0x0;
		_packet++;
	}
	if ( strlen(will_topic) != 0) {
		_packet = AddStringToBuf(_packet, will_topic);
		_packet = AddStringToBuf(_packet, will_payload);
	}

	if (strlen(AIO_USERNAME) != 0) {
		_packet = AddStringToBuf(_packet, AIO_USERNAME);
	}
	if (strlen(AIO_KEY) != 0) {
		_packet = AddStringToBuf(_packet, AIO_KEY);
	}
	_length = _packet - packet;
	packet[1] = _length-2;

	return _length;
}
u16 MQTT_subscribePacket(u8 *packet, const char *topic, u8 qos)
{
	u8 *_packet = packet;
	u16 _length;

	_packet[0] = MQTT_CTRL_SUBSCRIBE << 4 | MQTT_QOS_0 << 1;
	_packet+=2;

	_packet[0] = (packet_id_counter >> 8) & 0xFF;
	_packet[1] = packet_id_counter & 0xFF;
	_packet+=2;
	packet_id_counter++;

	_packet = AddStringToBuf(_packet, topic);

	_packet[0] = qos;
	_packet++;

	_length = _packet - packet;
	packet[1] = _length-2;

	return _length;
}
u16 MQTT_unsubscribePacket(u8 *packet, const char *topic)
{
	u8 *_packet = packet;
	u16 _length;

	_packet[0] = MQTT_CTRL_UNSUBSCRIBE << 4 | MQTT_QOS_0 << 1;
	_packet+=2;

	_packet[0] = (packet_id_counter >> 8) & 0xFF;
	_packet[1] = packet_id_counter & 0xFF;
	_packet+=2;
	packet_id_counter++;

	_packet = AddStringToBuf(_packet, topic);

	_length = _packet - packet;
	packet[1] = _length-2;

	return _length;
}

u8 MQTT_definePacket(char *packet,char *ack_typ)
{

	if((packet[0]>>4)== MQTT_CTRL_CONNECTACK)
	{
			if(packet[3]==0x00)
			{
				return 0X01;
				strncpy(ack_typ,"COnnectACK",10);
			}
			else{
				strncpy(ack_typ,"False_connection",16);
				return 0x02;
			}
	}

	if((packet[0]>>4)== MQTT_CTRL_PUBLISH)
	{
		strncpy(ack_typ,"Publish_packet",15);
		return 'p';
	}

	return 0;
}


char MQTT_puplish_read (u8 *packet ,char* topic ,char* data)
{
	u8 i=0,  multiplier = 1;
	u16 RL = 0,topic_len=0 ,data_len=0;
	packet ++;

	while ((packet[0]&128) != 0)
	{

		RL += (packet[i] & 127) * multiplier;
		multiplier *= 128;
		if (multiplier > 128*128){return 'e';}
		packet++;
	}

	RL +=packet[0]*multiplier;

	packet++;

	if(packet[0]==0)
	{
		topic_len=packet[1];
	}
	else
	{
		topic_len +=(packet[0]<<8)+packet[1];
	}
	packet +=2;


	memset((char*)topic,'\0',10);
	strncpy((char*)topic,(char*)packet,topic_len);
	packet+=topic_len;



	memset((char*)data,'\0',10);
	data_len=RL-(topic_len+2);
	strncpy((char*)data,(char*)packet,data_len);
return 0;
}
char MQTT_disconnect_packet(char* packet)
{
	packet[0]=0x14;
	packet[1]=0x00;

	return 2;
}
