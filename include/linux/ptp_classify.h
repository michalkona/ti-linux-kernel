/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * PTP 1588 support
 *
 * This file implements a BPF that recognizes PTP event messages.
 *
 * Copyright (C) 2010 OMICRON electronics GmbH
 */

#ifndef _PTP_CLASSIFY_H_
#define _PTP_CLASSIFY_H_

#include <linux/ip.h>
#include <linux/skbuff.h>

#define PTP_CLASS_NONE  0x00 /* not a PTP event message */
#define PTP_CLASS_V1    0x01 /* protocol version 1 */
#define PTP_CLASS_V2    0x02 /* protocol version 2 */
#define PTP_CLASS_VMASK 0x0f /* max protocol version is 15 */
#define PTP_CLASS_IPV4  0x10 /* event in an IPV4 UDP packet */
#define PTP_CLASS_IPV6  0x20 /* event in an IPV6 UDP packet */
#define PTP_CLASS_L2    0x40 /* event in a L2 packet */
#define PTP_CLASS_PMASK	0x70 /* mask for the packet type field */
#define PTP_CLASS_VLAN	0x80 /* event in a VLAN tagged packet */

#define PTP_CLASS_V1_IPV4 (PTP_CLASS_V1 | PTP_CLASS_IPV4)
#define PTP_CLASS_V1_IPV6 (PTP_CLASS_V1 | PTP_CLASS_IPV6) /* probably DNE */
#define PTP_CLASS_V2_IPV4 (PTP_CLASS_V2 | PTP_CLASS_IPV4)
#define PTP_CLASS_V2_IPV6 (PTP_CLASS_V2 | PTP_CLASS_IPV6)
#define PTP_CLASS_V2_L2   (PTP_CLASS_V2 | PTP_CLASS_L2)
#define PTP_CLASS_V2_VLAN (PTP_CLASS_V2 | PTP_CLASS_VLAN)
#define PTP_CLASS_L4      (PTP_CLASS_IPV4 | PTP_CLASS_IPV6)

#define PTP_EV_PORT 319
#define PTP_GEN_BIT 0x08 /* indicates general message, if set in message type */

/* PTP message types */
#define PTP_SYNC_MSG_ID			0x0
#define PTP_DLY_REQ_MSG_ID		0x1
#define PTP_PDLY_REQ_MSG_ID		0x2
#define PTP_PDLY_RSP_MSG_ID		0x3
#define PTP_FOLLOW_UP_MSG_ID		0x8
#define PTP_DLY_RESP_MSG_ID		0x9
#define PTP_PDLY_RESP_FLW_UP_MSG_ID	0xa
#define PTP_ANNOUNCE_MSG_ID		0xb
#define PTP_SIGNAL_MSG_ID		0xc
#define PTP_MGMT_MSG_ID			0xd

#define OFF_PTP_SOURCE_UUID	22 /* PTPv1 only */
#define OFF_PTP_SEQUENCE_ID	30
#define OFF_PTP_CONTROL		32 /* PTPv1 only */

/* Below defines should actually be removed at some point in time. */
#define IP6_HLEN	40
#define UDP_HLEN	8
#define OFF_IHL		14
#define IPV4_HLEN(data) (((struct iphdr *)(data + OFF_IHL))->ihl << 2)

#if defined(CONFIG_NET_PTP_CLASSIFY)
/**
 * ptp_classify_raw - classify a PTP packet
 * @skb: buffer
 *
 * Runs a minimal BPF dissector to classify a network packet to
 * determine the PTP class. In case the skb does not contain any
 * PTP protocol data, PTP_CLASS_NONE will be returned, otherwise
 * PTP_CLASS_V1_IPV{4,6}, PTP_CLASS_V2_IPV{4,6} or
 * PTP_CLASS_V2_{L2,VLAN}, depending on the packet content.
 */
unsigned int ptp_classify_raw(const struct sk_buff *skb);

void __init ptp_classifier_init(void);
#else
static inline void ptp_classifier_init(void)
{
}
static inline unsigned int ptp_classify_raw(struct sk_buff *skb)
{
	return PTP_CLASS_NONE;
}
#endif
#endif /* _PTP_CLASSIFY_H_ */
