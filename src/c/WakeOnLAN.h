/****************************************************************************************

File		WakeOnLAN.h
Why:		Implements wake on LAN functionality.
OS:			Windows
Created:	2025-05-17

History
-------

When		Who				What
-----------------------------------------------------------------------------------------
2025-05-17	Thomas			Created.

****************************************************************************************/

/*
	This file is maintained as part of OnOffMate. See https://github.com/ThomasPGH/OnOffMate .
*/

/*
	This code is covered by the MIT License. See https://opensource.org/license/mit .

	Copyright (c) 2024, 2025 Thomas

	Permission is hereby granted, free of charge, to any person obtaining a copy of this
	software and associated documentation files (the "Software"), to deal in the Software
	without restriction, including without limitation the rights to use, copy, modify,
	merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
	permit persons to whom the Software is furnished to do so, subject to the following
	conditions:

	The above copyright notice and this permission notice shall be included in all copies
	or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
	PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
	CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
	OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef U_WAKEONLAN_H
#define U_WAKEONLAN_H

#include <stdbool.h>
#include <inttypes.h>
#include "./externC.h"

/*
	Length of magic packet. See https://en.wikipedia.org/wiki/Wake-on-LAN#Magic_packet .

	  6 =  1 * 6 for FF FF FF FF FF FF
	 96 = 16 * 6 for MAC address
	---
	102
*/
#define U_WAKEONLAN_MAGIC_PACKET_LEN	(102)

/*
	Min length of an IP address.

	"1.0.0.1"
*/
#define U_WAKEONLAN_MIN_IP_LEN			(7)

/*
	Space for a MAC address in ASCII/UTF-8.

	"00-00-00-00-00-00" + NUL, which is 17 characters excluding NUL and 18 with NUL.
*/
#define U_WAKEONLAN_MAC_LEN				(17)
#define U_WAKEONLAN_MAC_SIZ				(17 + 1)

/*
	Max length and size of an IPv4 IP address.
	"255.255.255.255" + NUL.
*/
#define U_WAKEONLAN_IPV4_LEN			(3 + 1 + 3 + 1 + 3 + 1 + 3)
#define U_WAKEONLAN_IPV4_SIZ			(U_WAKEONLAN_IPV4_LEN + 1)

/*
	Max length and size of an IPv6 IP address.
	"2001:0db8:85a3:0000:0000:8a2e:0370:7334" + NUL.
	This example address is from https://en.wikipedia.org/wiki/IPv6_address .
*/
#define U_WAKEONLAN_IPV6_LEN			(8 * 4 + 7)
#define U_WAKEONLAN_IPV6_SIZ			(U_WAKEONLAN_IPV6_LEN + 1)

/*
*/
#define U_WAKEONLAN_IPV6V4_PFX			"::FFFF:"
#define U_WAKEONLAN_IPV6V4_PFX_LEN		(sizeof (U_WAKEONLAN_IPV6V4_PFX) - 1)
#define U_WAKEONLAN_IPV6V4_PFX_SIZ		(sizeof (U_WAKEONLAN_IPV6V4_PFX))
#define U_WAKEONLAN_IPV6V4_PFXW			L"::FFFF:"
#define U_WAKEONLAN_IPV6V4_PFXW_LEN		(sizeof (U_WAKEONLAN_IPV6V4_PFXW) - sizeof (wchar_t))
#define U_WAKEONLAN_IPV6V4_PFXW_SIZ		(sizeof (U_WAKEONLAN_IPV6V4_PFXW))

/*
	See https://en.wikipedia.org/wiki/Wake-on-LAN#Magic_packet .

	Excerpt:

	"Since the magic packet is only scanned for the string above, and not actually parsed by
	a full protocol stack, it could be sent as payload of any network- and transport-layer
	protocol, although it is typically sent as a UDP datagram to port 0 (reserved port number),
	7 (Echo Protocol) or 9 (Discard Protocol), or directly over Ethernet using EtherType 0x0842.
	A connection-oriented transport-layer protocol like TCP is less suited for this task as it
	requires establishing an active connection before sending user data."
*/
#ifndef U_WAKEONLAN_MAGIC_PACKET_PORT
#define U_WAKEONLAN_MAGIC_PACKET_PORT	(9)
#endif

#ifndef U_WAKEONLAN_DEF_U8_SIZE
#define	U_WAKEONLAN_DEF_U8_SIZE			(4096)
#endif

EXTERN_C_BEGIN

/*
	See
	https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup .
*/
bool callWSAStartup (void)
;

/*
	CallWSACleanup

*/
void CallWSACleanup (void)
;

/*
	These functions check if the given IP address is valid.
*/
bool isGoodIPv4string (const char *strip);
bool isGoodIPv4stringW (const wchar_t *wstrip);
bool isGoodIPv6string (const char *strip);
bool isGoodIPv6stringW (const wchar_t *wstrip);

enum eWOLret
{
	wolretOk,
	wolretSyntaxMAC,
	wolretSyntaxHst,
	wolretErrSend,
	wolretMissing
};

/*
	wakeOnLAN_W

*/
enum eWOLret wakeOnLAN_W (const wchar_t *wzHost, const wchar_t *wzMAC, bool bForceIPv6, char **szErr)
;

/*
	makeUnifiedMACaddress

*/
bool makeUnifiedMACaddress (wchar_t *wzOut, const wchar_t *wzMAC)
;

EXTERN_C_END

#endif														// Of #ifndef U_WAKEONLAN_H.
