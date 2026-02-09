/*******************************************************************************
 * Copyright (c) 2014, 2017 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *   Allan Stockdill-Mander/Ian Craggs - initial API and implementation and/or initial documentation
 *   Ian Craggs - fix for #96 - check rem_len in readPacket
 *   Ian Craggs - add ability to set message handler separately #6
 *******************************************************************************/
#include "MQTTClient.h"
#include "main.h"
#include "interface.h"
#include "TelaidianProto.h"


void TimerInit(Timer* t)
{
	t->enable =1;
	t->start = clock_time();
}
unsigned char TimerIsExpired(Timer* t)
{
	unsigned int u32temp;
	unsigned int u32ClockTime;

	u32ClockTime = clock_time();

	if(u32ClockTime < t->start)
	{
		u32temp = (0xffffffff - t->start + u32ClockTime);
	}
	else
	{
		u32temp = (u32ClockTime - t->start);
	}

	if(u32temp >= t->interval)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
void TimerCountdownMS(Timer* t, clock_time_t timeoutMS)
{
	t->enable = 1;
	t->start = clock_time();
	t->interval = timeoutMS;
}
void TimerCountdown(Timer* t, clock_time_t timeout)
{
	t->enable = 1;
	t->start = clock_time();
	t->interval = timeout;
}
clock_time_t TimerLeftMS(Timer* t)
{
	clock_time_t lefttime;
	unsigned int u32temp;
	unsigned int u32ClockTime;

	u32ClockTime = clock_time();

	if(u32ClockTime < t->start)
	{
		u32temp = (0xffffffff - t->start + u32ClockTime);
	}
	else
	{
		u32temp = (u32ClockTime - t->start);
	}

	if(u32temp >= t->interval)
	{
		lefttime = 0;
	}
	else
	{
		lefttime = t->interval - u32temp;
	}

	return lefttime;
}

static void NewMessageData(MessageData* md, MQTTString* aTopicName, MQTTMessage* aMessage) {
    md->topicName = aTopicName;
    md->message = aMessage;
}


static int getNextPacketId(MQTTClient *c) {
    return c->next_packetid = (c->next_packetid == MAX_PACKET_ID) ? 1 : c->next_packetid + 1;
}


static int sendPacket(MQTTClient* c, int length, Timer* timer)
{
      int rc;
      
	UINT8 u8Result = 0;

	/*gNET_TCP_Info.u8SendBuf.u16Lengh = length;
	u8Result = NetSendData();*/

	u8Result = NET_Send(c->buf, length, 1000);

	if (TRUE == u8Result)
	{
		TimerCountdown(&c->last_sent, c->keepAliveInterval); // record the fact that we have successfully sent the packet
		rc = SUCCESS_MQTT;
	}
	else
	{
		 rc = FAILURE_MQTT;	
	}
	return rc;
}

//
//void MQTTClientInit(MQTTClient* c, Network* network, unsigned int command_timeout_ms,
//		unsigned char* sendbuf, size_t sendbuf_size, unsigned char* readbuf, size_t readbuf_size)
//{
//    int i;
//    c->ipstack = network;
//
//    for (i = 0; i < MAX_MESSAGE_HANDLERS; ++i)
//        c->messageHandlers[i].topicFilter = 0;
//    c->command_timeout_ms = command_timeout_ms;
//    c->buf = sendbuf;
//    c->buf_size = sendbuf_size;
//    c->readbuf = readbuf;
//    c->readbuf_size = readbuf_size;
//    c->isconnected = 0;
//    c->cleansession = 0;
//    c->ping_outstanding = 0;
//    c->defaultMessageHandler = NULL;
//	  c->next_packetid = 1;
//    TimerInit(&c->last_sent);
//    TimerInit(&c->last_received);
//#if defined(MQTT_TASK)
//	  MutexInit(&c->mutex);
//#endif
//}

//
//static int decodePacket(MQTTClient* c, int* value, int timeout)
//{
//    unsigned char i;
//    int multiplier = 1;
//    int len = 0;
//    const int MAX_NO_OF_REMAINING_LENGTH_BYTES = 4;
//
//    *value = 0;
//    do
//    {
//        int rc = MQTTPACKET_READ_ERROR;
//
//        if (++len > MAX_NO_OF_REMAINING_LENGTH_BYTES)
//        {
//            rc = MQTTPACKET_READ_ERROR; /* bad data */
//            goto exit;
//        }
//        rc = c->ipstack->mqttread(c->ipstack, &i, 1, timeout);
//        if (rc != 1)
//            goto exit;
//        *value += (i & 127) * multiplier;
//        multiplier *= 128;
//    } while ((i & 128) != 0);
//exit:
//    return len;
//}


static int readPacket(MQTTClient* c, Timer* timer, unsigned char isBlockMode)
{
    MQTTHeader header = {0};
    int len, lenchk;
	int multiplier = 1;

	UINT16 u16TotalLength = 0;
	unsigned int u32RemainLenth = 0;

	int rc;
	unsigned char uctemp, i;

	const int MAX_NO_OF_REMAINING_LENGTH_BYTES = 4;

	rc = MQTTPACKET_READ_ERROR; /* bad data */

	memset(c->readbuf, 0, NET_MAX_BUF/2);

	if(isBlockMode)
	{
		while(1)
		{
			if(TimerIsExpired(timer))
			{
				break;
			}
			u16TotalLength = NET_Read(c->readbuf);
			if(u16TotalLength > 0)
			{
				break;
			}
			gSelfTest.fSetTaskID(Tasko_ID_ETHERNET_App);
			OSTimeDly(10);
		}
	}
	else
	{
		u16TotalLength = NET_Read(c->readbuf);		
	}

	
	

	if(u16TotalLength == 0)
	{
		 goto exit;
	}

	gNET_TCP_Info.u32BreakCount =0;

	//计算剩余数据长度
	i = 1;
	len = 0;
	lenchk = 1;
	do
	{		
		if (++len > MAX_NO_OF_REMAINING_LENGTH_BYTES)
		{
			//rc = MQTTPACKET_READ_ERROR; /* bad data */
			goto exit;
		}
		uctemp = c->readbuf[i++];
		u32RemainLenth += (uctemp & 0x7F) * multiplier;
		multiplier *= 128;

		lenchk++;
		if(lenchk > u16TotalLength)
		{
			//rc = MQTTPACKET_READ_ERROR; /* bad data */
			goto exit;
		}		

	} while ((uctemp & 0x80) != 0);

	//检查数据长度是否正确 
	if((u32RemainLenth+lenchk) != u16TotalLength)
	{
		//rc = MQTTPACKET_READ_ERROR; /* bad data */
		goto exit;
	}

	//读取消息类型码
	header.byte = c->readbuf[0];
	rc = header.bits.type;
	if (c->keepAliveInterval > 0)
		TimerCountdown(&c->last_received, c->keepAliveInterval); // record the fact that we have successfully received a packet
exit:
	return rc;	
}


// assume topic filter and name is in correct format
// # can only be at end
// + and # can only be next to separator
static char isTopicMatched(char* topicFilter, MQTTString* topicName)
{
    char* curf = topicFilter;
    char* curn = topicName->lenstring.data;
    char* curn_end = curn + topicName->lenstring.len;

    while (*curf && curn < curn_end)
    {
        if (*curn == '/' && *curf != '/')
            break;
        if (*curf != '+' && *curf != '#' && *curf != *curn)
            break;
        if (*curf == '+')
        {   // skip until we meet the next separator, or end of string
            char* nextpos = curn + 1;
            while (nextpos < curn_end && *nextpos != '/')
                nextpos = ++curn + 1;
        }
        else if (*curf == '#')
            curn = curn_end - 1;    // skip until end of string
        curf++;
        curn++;
    };

    return (curn == curn_end) && (*curf == '\0');
}


int deliverMessage(MQTTClient* c, MQTTString* topicName, MQTTMessage* message)
{
    int i;
    int rc = FAILURE_MQTT;

    // we have to find the right message handler - indexed by topic
    for (i = 0; i < MAX_MESSAGE_HANDLERS; ++i)
    {
        if (c->messageHandlers[i].topicFilter != 0 && (MQTTPacket_equals(topicName, (char*)c->messageHandlers[i].topicFilter) ||
                isTopicMatched((char*)c->messageHandlers[i].topicFilter, topicName)))
        {
            if (c->messageHandlers[i].fp != NULL)
            {
                MessageData md;
                NewMessageData(&md, topicName, message);
                c->messageHandlers[i].fp(&md);
                rc = SUCCESS_MQTT;
            }
        }
    }

    if (rc == FAILURE_MQTT && c->defaultMessageHandler != NULL)
    {
        MessageData md;
        NewMessageData(&md, topicName, message);
        c->defaultMessageHandler(&md);
        rc = SUCCESS_MQTT;
    }

    return rc;
}


int keepalive(MQTTClient* c)
{
    int rc = SUCCESS_MQTT;

    if (c->keepAliveInterval == 0)
        goto exit;

    if (TimerIsExpired(&c->last_sent) || TimerIsExpired(&c->last_received))
    {
        if (c->ping_outstanding)
            rc = FAILURE_MQTT; /* PINGRESP not received in keepalive interval */
        else
        {
            Timer timer;
            TimerInit(&timer);
            TimerCountdownMS(&timer, 500);
            int len = MQTTSerialize_pingreq(c->buf, c->buf_size);
            if (len > 0 && (rc = sendPacket(c, len, &timer)) == SUCCESS_MQTT) // send the ping packet
                c->ping_outstanding = 1;
        }
    }

exit:
    return rc;
}


void MQTTCleanSession(MQTTClient* c)
{
    int i = 0;

    for (i = 0; i < MAX_MESSAGE_HANDLERS; ++i)
        c->messageHandlers[i].topicFilter = NULL;
}


void MQTTCloseSession(MQTTClient* c)
{
    c->ping_outstanding = 0;
    c->isconnected = 0;

	//2017070926 lkf 已定阅标志	清0
	memset(g_ucSubTopicxxIsSubed, 0, sizeof(g_ucSubTopicxxIsSubed));  

    if (c->cleansession)
        MQTTCleanSession(c);
}


int cycle(MQTTClient* c, Timer* timer)
{
    int len = 0,
        rc = SUCCESS_MQTT;

    int packet_type = readPacket(c, timer, TRUE);     /* read the socket, see what work is due */

    switch (packet_type)
    {
        default:
            /* no more data to read, unrecoverable. Or read packet fails due to unexpected network error */
            rc = packet_type;
            goto exit;
        case 0: /* timed out reading packet */
            break;
        case CONNACK:
        case PUBACK:
        case SUBACK:
            break;
        case PUBLISH:
        {
            MQTTString topicName;
            MQTTMessage msg;
            int intQoS;
            msg.payloadlen = 0; /* this is a size_t, but deserialize publish sets this as int */
            if (MQTTDeserialize_publish(&msg.dup, &intQoS, &msg.retained, &msg.id, &topicName,
               (unsigned char**)&msg.payload, (int*)&msg.payloadlen, c->readbuf, c->readbuf_size) != 1)
                goto exit;
            msg.qos = (enum QoS)intQoS;
            deliverMessage(c, &topicName, &msg);
            if (msg.qos != QOS0)
            {
                if (msg.qos == QOS1)
                    len = MQTTSerialize_ack(c->buf, c->buf_size, PUBACK, 0, msg.id);
                else if (msg.qos == QOS2)
                    len = MQTTSerialize_ack(c->buf, c->buf_size, PUBREC, 0, msg.id);
                if (len <= 0)
                    rc = FAILURE_MQTT;
                else
                    rc = sendPacket(c, len, timer);
                if (rc == FAILURE_MQTT)
                    goto exit; // there was a problem
            }
            break;
        }
        case PUBREC:
        case PUBREL:
        {
            unsigned short mypacketid;
            unsigned char dup, type;
            if (MQTTDeserialize_ack(&type, &dup, &mypacketid, c->readbuf, c->readbuf_size) != 1)
                rc = FAILURE_MQTT;
            else if ((len = MQTTSerialize_ack(c->buf, c->buf_size,
                (packet_type == PUBREC) ? PUBREL : PUBCOMP, 0, mypacketid)) <= 0)
                rc = FAILURE_MQTT;
            else if ((rc = sendPacket(c, len, timer)) != SUCCESS_MQTT) // send the PUBREL packet
                rc = FAILURE_MQTT; // there was a problem
            if (rc == FAILURE_MQTT)
                goto exit; // there was a problem
            break;
        }

        case PUBCOMP:
            break;
        case PINGRESP:
            c->ping_outstanding = 0;
            break;
    }

    //if (keepalive(c) != SUCCESS_MQTT) {
    //    //check only keepalive FAILURE_MQTT status so that previous FAILURE_MQTT status can be considered as FAULT
    //    rc = FAILURE_MQTT;
    //}

exit:
    if (rc == SUCCESS_MQTT)
        rc = packet_type;
    else if (c->isconnected)
        MQTTCloseSession(c);
    return rc;
}

void DealServerCmdData(MQTTClient* c)
{
	Timer timer;	

	int len = 0,
		rc = SUCCESS_MQTT;

	TimerInit(&timer);
	TimerCountdown(&timer, c->command_timeout_ms);

	int packet_type = readPacket(c, &timer, 0);     /* read the socket, see what work is due */

	switch (packet_type)
	{
	default:
		/* no more data to read, unrecoverable. Or read packet fails due to unexpected network error */
		rc = packet_type;
		goto exit;
	case 0: /* timed out reading packet */
		break;
	case CONNACK:
	case PUBACK:
	case SUBACK:
		break;
	case PUBLISH:
		{
			MQTTString topicName;
			MQTTMessage msg;
			int intQoS;
			msg.payloadlen = 0; /* this is a size_t, but deserialize publish sets this as int */
			if (MQTTDeserialize_publish(&msg.dup, &intQoS, &msg.retained, &msg.id, &topicName,
				(unsigned char**)&msg.payload, (int*)&msg.payloadlen, c->readbuf, c->readbuf_size) != 1)
				goto exit;
			msg.qos = (enum QoS)intQoS;
			deliverMessage(c, &topicName, &msg);
			if (msg.qos != QOS0)
			{
				if (msg.qos == QOS1)
					len = MQTTSerialize_ack(c->buf, c->buf_size, PUBACK, 0, msg.id);
				else if (msg.qos == QOS2)
					len = MQTTSerialize_ack(c->buf, c->buf_size, PUBREC, 0, msg.id);
				if (len <= 0)
					rc = FAILURE_MQTT;
				else
					rc = sendPacket(c, len, &timer);
				if (rc == FAILURE_MQTT)
					goto exit; // there was a problem
			}
			break;
		}
	case PUBREC:
	case PUBREL:
		{
			unsigned short mypacketid;
			unsigned char dup, type;
			if (MQTTDeserialize_ack(&type, &dup, &mypacketid, c->readbuf, c->readbuf_size) != 1)
				rc = FAILURE_MQTT;
			else if ((len = MQTTSerialize_ack(c->buf, c->buf_size,
				(packet_type == PUBREC) ? PUBREL : PUBCOMP, 0, mypacketid)) <= 0)
				rc = FAILURE_MQTT;
			else if ((rc = sendPacket(c, len, &timer)) != SUCCESS_MQTT) // send the PUBREL packet
				rc = FAILURE_MQTT; // there was a problem
			if (rc == FAILURE_MQTT)
				goto exit; // there was a problem
			break;
		}

	case PUBCOMP:
		break;
	case PINGRESP:
		c->ping_outstanding = 0;
		break;
	}

	//if (keepalive(c) != SUCCESS_MQTT) {
	//    //check only keepalive FAILURE_MQTT status so that previous FAILURE_MQTT status can be considered as FAULT
	//    rc = FAILURE_MQTT;
	//}

exit:
	
}


int MQTTYield(MQTTClient* c, int timeout_ms)
{
    int rc = SUCCESS_MQTT;
    Timer timer;

    TimerInit(&timer);
    TimerCountdownMS(&timer, timeout_ms);

	  do
    {
        if (cycle(c, &timer) < 0)
        {
            rc = FAILURE_MQTT;
            break;
        }
  	} while (!TimerIsExpired(&timer));

    return rc;
}


void MQTTRun(void* parm)
{
	Timer timer;
	MQTTClient* c = (MQTTClient*)parm;

	TimerInit(&timer);

	while (1)
	{
#if defined(MQTT_TASK)
		MutexLock(&c->mutex);
#endif
		TimerCountdownMS(&timer, 250); /* Don't wait too long if no traffic is incoming */
		cycle(c, &timer);
#if defined(MQTT_TASK)
		MutexUnlock(&c->mutex);
#endif
	}
}


#if defined(MQTT_TASK)
int MQTTStartTask(MQTTClient* client)
{
	return ThreadStart(&client->thread, &MQTTRun, client);
}
#endif


int waitfor(MQTTClient* c, int packet_type, Timer* timer)
{
    int rc = FAILURE_MQTT;

    do
    {
        if (TimerIsExpired(timer))
            break; // we timed out
        rc = cycle(c, timer);
    }
    while (rc != packet_type && rc >= 0);

    return rc;
}



int MQTTConnectWithResults(MQTTClient* c, MQTTPacket_connectData* options, MQTTConnackData* data)
{
    Timer connect_timer;
    int rc = FAILURE_MQTT;
    MQTTPacket_connectData default_options = MQTTPacket_connectData_initializer;
    int len = 0;

#if defined(MQTT_TASK)
	  MutexLock(&c->mutex);
#endif
	  if (c->isconnected) /* don't send connect packet again if we are already connected */
		  goto exit;

    TimerInit(&connect_timer);
    TimerCountdownMS(&connect_timer, c->command_timeout_ms);

    if (options == 0)
        options = &default_options; /* set default options if none were supplied */

    c->keepAliveInterval = options->keepAliveInterval*500;
    c->cleansession = options->cleansession;
    TimerCountdown(&c->last_received, c->keepAliveInterval);

    if ((len = MQTTSerialize_connect(c->buf, c->buf_size, options)) <= 0)
        goto exit;

    if ((rc = sendPacket(c, len, &connect_timer)) != SUCCESS_MQTT)  // send the connect packet      
		goto exit; // there was a problem

    // this will be a blocking call, wait for the connack
	//TimerCountdownMS(&connect_timer, 60000);  //20170927 lkf 增加等待回复延时
    if (waitfor(c, CONNACK, &connect_timer) == CONNACK)
    {

        data->rc = 0;
        data->sessionPresent = 0;
        if (MQTTDeserialize_connack(&data->sessionPresent, &data->rc, c->readbuf, c->readbuf_size) == 1)
            rc = data->rc;
        else
            rc = FAILURE_MQTT;
    }
    else
        rc = FAILURE_MQTT;

exit:
    if (rc == SUCCESS_MQTT)
    {
        c->isconnected = 1;
        c->ping_outstanding = 0;

		TimerCountdown(&c->last_received, c->keepAliveInterval);
		TimerCountdown(&c->last_sent, c->keepAliveInterval);
    }

#if defined(MQTT_TASK)
	  MutexUnlock(&c->mutex);
#endif

    return rc;
}


int MQTTConnect(MQTTClient* c, MQTTPacket_connectData* options)
{
    MQTTConnackData data;
    return MQTTConnectWithResults(c, options, &data);
}


int MQTTSetMessageHandler(MQTTClient* c, const char* topicFilter, messageHandler messageHandler)
{
    int rc = FAILURE_MQTT;
    int i = -1;

    /* first check for an existing matching slot */
    for (i = 0; i < MAX_MESSAGE_HANDLERS; ++i)
    {
        if (c->messageHandlers[i].topicFilter != NULL && strcmp(c->messageHandlers[i].topicFilter, topicFilter) == 0)
        {
            if (messageHandler == NULL) /* remove existing */
            {
                c->messageHandlers[i].topicFilter = NULL;
                c->messageHandlers[i].fp = NULL;
            }
            rc = SUCCESS_MQTT; /* return i when adding new subscription */
            break;
        }
    }
    /* if no existing, look for empty slot (unless we are removing) */
    if (messageHandler != NULL) {
        if (rc == FAILURE_MQTT)
        {
            for (i = 0; i < MAX_MESSAGE_HANDLERS; ++i)
            {
                if (c->messageHandlers[i].topicFilter == NULL)
                {
                    rc = SUCCESS_MQTT;
                    break;
                }
            }
        }
        if (i < MAX_MESSAGE_HANDLERS)
        {
            c->messageHandlers[i].topicFilter = topicFilter;
            c->messageHandlers[i].fp = messageHandler;
        }
    }
    return rc;
}


int MQTTSubscribeWithResults(MQTTClient* c, const char* topicFilter, enum QoS qos,
       messageHandler messageHandler, MQTTSubackData* data)
{
    int rc = FAILURE_MQTT;
    Timer timer;
    int len = 0;
    MQTTString topic = MQTTString_initializer;
    topic.cstring = (char *)topicFilter;

#if defined(MQTT_TASK)
	  MutexLock(&c->mutex);
#endif
	  if (!c->isconnected)
		    goto exit;

    TimerInit(&timer);
    TimerCountdownMS(&timer, c->command_timeout_ms);

    len = MQTTSerialize_subscribe(c->buf, c->buf_size, 0, getNextPacketId(c), 1, &topic, (int*)&qos);
    if (len <= 0)
        goto exit;
    if ((rc = sendPacket(c, len, &timer)) != SUCCESS_MQTT) // send the subscribe packet
        goto exit;             // there was a problem

    if (waitfor(c, SUBACK, &timer) == SUBACK)      // wait for suback
    {
        int count = 0;
        unsigned short mypacketid;
        data->grantedQoS = QOS0;
        if (MQTTDeserialize_suback(&mypacketid, 1, &count, (int*)&data->grantedQoS, c->readbuf, c->readbuf_size) == 1)
        {
            if (data->grantedQoS != 0x80)
                rc = MQTTSetMessageHandler(c, topicFilter, messageHandler);
        }
    }
    else
        rc = FAILURE_MQTT;

exit:
    if (rc == FAILURE_MQTT)
        MQTTCloseSession(c);
#if defined(MQTT_TASK)
	  MutexUnlock(&c->mutex);
#endif
    return rc;
}


int MQTTSubscribe(MQTTClient* c, const char* topicFilter, enum QoS qos,
       messageHandler messageHandler)
{
    MQTTSubackData data;
    return MQTTSubscribeWithResults(c, topicFilter, qos, messageHandler, &data);
}


int MQTTUnsubscribe(MQTTClient* c, const char* topicFilter)
{
    int rc = FAILURE_MQTT;
    Timer timer;
    MQTTString topic = MQTTString_initializer;
    topic.cstring = (char *)topicFilter;
    int len = 0;

#if defined(MQTT_TASK)
	  MutexLock(&c->mutex);
#endif
	  if (!c->isconnected)
		  goto exit;

    TimerInit(&timer);
    TimerCountdownMS(&timer, c->command_timeout_ms);

    if ((len = MQTTSerialize_unsubscribe(c->buf, c->buf_size, 0, getNextPacketId(c), 1, &topic)) <= 0)
        goto exit;
    if ((rc = sendPacket(c, len, &timer)) != SUCCESS_MQTT) // send the subscribe packet
        goto exit; // there was a problem

    if (waitfor(c, UNSUBACK, &timer) == UNSUBACK)
    {
        unsigned short mypacketid;  // should be the same as the packetid above
        if (MQTTDeserialize_unsuback(&mypacketid, c->readbuf, c->readbuf_size) == 1)
        {
            /* remove the subscription message handler associated with this topic, if there is one */
            MQTTSetMessageHandler(c, topicFilter, NULL);
        }
    }
    else
        rc = FAILURE_MQTT;

exit:
    if (rc == FAILURE_MQTT)
        MQTTCloseSession(c);
#if defined(MQTT_TASK)
	  MutexUnlock(&c->mutex);
#endif
    return rc;
}


int MQTTPublish(MQTTClient* c, const char* topicName, MQTTMessage* message)
{
    int rc = FAILURE_MQTT;
    Timer timer;
    MQTTString topic = MQTTString_initializer;
    topic.cstring = (char *)topicName;
    int len = 0;

#if defined(MQTT_TASK)
	  MutexLock(&c->mutex);
#endif
	  if (!c->isconnected)
		    goto exit;

    TimerInit(&timer);
    TimerCountdownMS(&timer, c->command_timeout_ms);

    if (message->qos == QOS1 || message->qos == QOS2)
        message->id = getNextPacketId(c);

    len = MQTTSerialize_publish(c->buf, c->buf_size, 0, message->qos, message->retained, message->id,
              topic, (unsigned char*)message->payload, message->payloadlen);
    if (len <= 0)
        goto exit;
    if ((rc = sendPacket(c, len, &timer)) != SUCCESS_MQTT) // send the subscribe packet
        goto exit; // there was a problem

    if (message->qos == QOS1)
    {
        if (waitfor(c, PUBACK, &timer) == PUBACK)
        {
            unsigned short mypacketid;
            unsigned char dup, type;
            if (MQTTDeserialize_ack(&type, &dup, &mypacketid, c->readbuf, c->readbuf_size) != 1)
                rc = FAILURE_MQTT;
        }
        else
            rc = FAILURE_MQTT;
    }
    else if (message->qos == QOS2)
    {
        if (waitfor(c, PUBCOMP, &timer) == PUBCOMP)
        {
            unsigned short mypacketid;
            unsigned char dup, type;
            if (MQTTDeserialize_ack(&type, &dup, &mypacketid, c->readbuf, c->readbuf_size) != 1)
                rc = FAILURE_MQTT;
        }
        else
            rc = FAILURE_MQTT;
    }

exit:
    if (rc == FAILURE_MQTT)
        MQTTCloseSession(c);
#if defined(MQTT_TASK)
	  MutexUnlock(&c->mutex);
#endif
    return rc;
}


int MQTTDisconnect(MQTTClient* c)
{
    int rc = FAILURE_MQTT;
    Timer timer;     // we might wait for incomplete incoming publishes to complete
    int len = 0;

#if defined(MQTT_TASK)
	MutexLock(&c->mutex);
#endif
    TimerInit(&timer);
    TimerCountdownMS(&timer, c->command_timeout_ms);

	  len = MQTTSerialize_disconnect(c->buf, c->buf_size);
    if (len > 0)
        rc = sendPacket(c, len, &timer);            // send the disconnect packet
    MQTTCloseSession(c);

#if defined(MQTT_TASK)
	  MutexUnlock(&c->mutex);
#endif
    return rc;
}

int MQTTIsConnected(MQTTClient* client)
{
	return client->isconnected;
}