/****************************************************************************************

File		WakeOnLAN.c
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

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#include <Winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#pragma comment (lib, "Ws2_32.lib")

#include "./WakeOnLAN.h"

#ifdef THIS_IS_ONOFFMATE
	#include "./WinRuntimeReplacements.h"

	#define memcpy(d, s, l)		memcpyU (d, s, l)
	#define memset(d, c, l)		memsetU (d, c, l)
#else
	#include <stdio.h>
#endif

/*
	Excerpt from https://en.wikipedia.org/wiki/Wake-on-LAN#Magic_packet:

	"The magic packet is a frame that is most often sent as a broadcast and that contains
	anywhere within its payload 6 bytes of all 255 (FF FF FF FF FF FF in hexadecimal),
	followed by sixteen repetitions of the target computer's 48-bit MAC address, for a total
	of 102 bytes."
*/
void initWOLmagicPacket (char *szMagicPacket, const unsigned char cucMAC [6])
{
	memset (szMagicPacket, 0xFF, 6);

	size_t offs = 6;
	while (offs < U_WAKEONLAN_MAGIC_PACKET_LEN)
	{
		memcpy (szMagicPacket + offs, cucMAC, 6);
		offs += 6;
	}
}

static bool bWSAStartupComplete;

/*
	See
	https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup .
*/
bool callWSAStartup (void)
{
	WSADATA		wsa;
	int			iws;

	if (!bWSAStartupComplete)
	{
		iws = WSAStartup (MAKEWORD (2, 2), &wsa);

		/*
		switch (iws)
		{
			case WSASYSNOTREADY:		break;
			case WSAVERNOTSUPPORTED:	break;
			case WSAEINPROGRESS:		break;
			case WSAEPROCLIM:			break;
			case WSAEFAULT:				break;
			case 0:
				bWSAStartupComplete = true;
				return true;
		}
		*/

		bWSAStartupComplete = 0 == iws;
	}
	return bWSAStartupComplete;
}

void CallWSACleanup (void)
{
	if (bWSAStartupComplete)
		WSACleanup ();
}

bool isGoodIPv4string (const char *strip)
{
	struct in_addr addr;
	
	return 1 == inet_pton (AF_INET, strip, &addr);
}

bool isGoodIPv4stringW (const wchar_t *wstrip)
{
	char	strip [U_WAKEONLAN_IPV4_SIZ];
	int		iReq = reqUTF8size (wstrip);
	if (iReq <= U_WAKEONLAN_IPV4_SIZ)
	{
		UTF8_from_WinU16 (strip, U_WAKEONLAN_IPV4_SIZ, wstrip);
		
		struct in_addr addr;
		return 1 == inet_pton (AF_INET, strip, &addr);
	}
	return false;
}

bool isGoodIPv6string (const char *strip)
{
	struct in6_addr addr;
	
	return 1 == inet_pton (AF_INET6, strip, &addr);
}

bool isGoodIPv6stringW (const wchar_t *wstrip)
{
	char	strip [U_WAKEONLAN_IPV6_SIZ];
	int		iReq = reqUTF8size (wstrip);
	if (iReq <= U_WAKEONLAN_IPV6_SIZ)
	{
		UTF8_from_WinU16 (strip, U_WAKEONLAN_IPV6_SIZ, wstrip);

		struct in6_addr addr;
		return 1 == inet_pton (AF_INET6, strip, &addr);
	}
	return false;
}

/*
	Note that ccPeerIP should be a broadcast IP, i.e. if the peer's IP is
	192.168.0.201 and the subnet mask is 255.255.255.0, use 192.168.0.255.

	If bForceIPv4 is true, we send an IPv4 packet. Otherwise an IPv4 address
	is converted to IPv6 first.
*/
bool sendWOLmagicPacket (const char *ccPeerIP, const char *szMagicPacket, bool bForceIPv6)
{
	SOCKET	sUDP;
	int		iso;
	bool	bRet		= false;
	int		iErr		= 0;
	int		ipo;

	if (!bForceIPv6 && isGoodIPv4string (ccPeerIP))
	{
		/*
			See
			https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket .
		*/
		sUDP = socket (AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (INVALID_SOCKET == sUDP)
			return bRet;

		struct sockaddr_in si_peer;
		si_peer.sin_family			= AF_INET;
		si_peer.sin_addr.s_addr		= inet_addr (ccPeerIP);
		si_peer.sin_port			= htons (U_WAKEONLAN_MAGIC_PACKET_PORT);

		/*
			See
			https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-setsockopt .
		*/
		BOOL so_broadcast	= true;
		iso = setsockopt (sUDP, SOL_SOCKET, SO_BROADCAST, (char *) &so_broadcast, sizeof (BOOL));
		if (0 != iso)
			return bRet;

		/*
			Must be sendto () instead of send (). See
			https://stackoverflow.com/questions/66554581/difference-between-send-and-sendto-in-c-for-a-udp-network-implementation .
		*/
		iErr = sendto (sUDP, szMagicPacket, U_WAKEONLAN_MAGIC_PACKET_LEN, 0, (struct sockaddr*) &si_peer, sizeof (si_peer));
	} else
	if (isGoodIPv4string (ccPeerIP) || isGoodIPv6string (ccPeerIP))
	{
		sUDP = socket (AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
		if (INVALID_SOCKET == sUDP)
			return bRet;

		/*
			See
			https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-setsockopt .
		*/
		BOOL so_broadcast	= true;
		iso = setsockopt (sUDP, SOL_SOCKET, SO_BROADCAST, (char *) &so_broadcast, sizeof (BOOL));
		if (0 != iso)
			return bRet;
		DWORD so_v6only = 0;
		iso = setsockopt (sUDP, IPPROTO_IPV6, IPV6_V6ONLY, (char *) &so_v6only, sizeof (DWORD));
		if (0 != iso)
			return bRet;

		char		szIP4asIP6 [U_WAKEONLAN_IPV6_SIZ]	= U_WAKEONLAN_IPV6V4_PFX;
		const char	*szIP								= szIP4asIP6;
		if (isGoodIPv4string (ccPeerIP))
			memcpy (szIP4asIP6 + U_WAKEONLAN_IPV6V4_PFX_LEN, ccPeerIP, strlenU (ccPeerIP) + 1);
		else
			szIP = ccPeerIP;

		struct sockaddr_in6 si_peer;
		memset (&si_peer, 0, sizeof (si_peer));
		si_peer.sin6_family			= AF_INET6;
		si_peer.sin6_port			= htons (U_WAKEONLAN_MAGIC_PACKET_PORT);
		ipo = inet_pton (AF_INET6, szIP, &si_peer.sin6_addr);

		/*
			Must be sendto () instead of send (). See
			https://stackoverflow.com/questions/66554581/difference-between-send-and-sendto-in-c-for-a-udp-network-implementation .
		*/
		iErr = sendto (sUDP, szMagicPacket, U_WAKEONLAN_MAGIC_PACKET_LEN, 0, (struct sockaddr*) &si_peer, sizeof (si_peer));
	} else
		return false;

	bRet = U_WAKEONLAN_MAGIC_PACKET_LEN == iErr;
	closesocket (sUDP);
	return bRet;
}

enum eWOLret wakeOnLAN_W (const wchar_t *wzHost, const wchar_t *wzMAC, bool bForceIPv6, char **szErr)
{
	char			szMACu8	[U_WAKEONLAN_MAC_SIZ];
	char			szHstu8	[U_WAKEONLAN_DEF_U8_SIZE];
	unsigned char	ucMAC	[6];

	int iRequ = reqUTF8size (wzHost);
	if (iRequ < U_WAKEONLAN_MIN_IP_LEN)
		return wolretSyntaxHst;
	if (iRequ < U_WAKEONLAN_DEF_U8_SIZE)
	{
		UTF8_from_WinU16 (szHstu8, U_WAKEONLAN_DEF_U8_SIZE, wzHost);

		// A MAC address has exactly 17 characters (18 when NUL is included).
		size_t lenMAC = strlenW (wzMAC);
		if (U_WAKEONLAN_MAC_LEN != lenMAC)
			return wolretSyntaxMAC;
		UTF8_from_WinU16l (szMACu8, U_WAKEONLAN_MAC_SIZ, wzMAC, U_WAKEONLAN_MAC_SIZ);
		int s = 0;
		int n;
		for (n = 0; n < 6; ++ n)
		{
			size_t l = ubf_octet_from_hex (ucMAC + n, szMACu8 + s);
			if (2 != l)
			{
				*szErr = szMACu8 + s;
				return wolretSyntaxMAC;
			}
			s += 3;
		}

		/*
			Magic packet. See https://en.wikipedia.org/wiki/Wake-on-LAN#Magic_packet .

			  6 =  1 * 6 for FF FF FF FF FF FF
			 96 = 16 * 6 for MAC address
			---
			102
		*/
		unsigned char ucWOLmagicPacket [U_WAKEONLAN_MAGIC_PACKET_LEN];
		initWOLmagicPacket (ucWOLmagicPacket, ucMAC);
		bool bSent = sendWOLmagicPacket (szHstu8, ucWOLmagicPacket, bForceIPv6);
		if (bSent)
			return wolretOk;
		return wolretErrSend;
	}
	return wolretSyntaxHst;
}

bool makeUnifiedMACaddress (wchar_t *wzOut, const wchar_t *wzMAC)
{
	size_t lenMAC = strlenW (wzMAC);
	if (U_WAKEONLAN_MAC_LEN != lenMAC)
		return false;
	memcpy (wzOut, wzMAC, 2 * U_WAKEONLAN_MAC_SIZ);
	int n = 2;
	while (n < U_WAKEONLAN_MAC_LEN)
	{
		wzOut [n] = L'-';
		n += 3;
	}
	return true;
}
