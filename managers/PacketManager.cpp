/****************************************************************************
**
** PacketManager.cpp
**
** Copyright (C) September 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//----------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------
TPacketManager PacketManager;
//----------------------------------------------------------------------------
//����� ������� �� ��� �������
#define UMSG(size) { "?", size, DIR_BOTH, 0 }
// A message type sent to the server
#define SMSG(name, size) { name, size, DIR_SEND, 0 }
// A message type received from the server
#define RMSG(name, size) { name, size, DIR_RECV, 0 }
// A message type transmitted in both directions
#define BMSG(name, size) { name, size, DIR_BOTH, 0 }
// Message types that have handler methods
#define RMSGH(name, size, rmethod) \
	{ name, size, DIR_RECV, &TPacketManager::Handle ##rmethod }
#define BMSGH(name, size, rmethod) \
	{ name, size, DIR_BOTH, &TPacketManager::Handle ##rmethod }
//---------------------------------------------------------------------------
TMessageType TPacketManager::m_MessageTypes[0x100] =
{
	/*0x00*/ SMSG("Create Character", 0x68),
	/*0x01*/ SMSG("Disconnect", 0x05),
	/*0x02*/ SMSG("Walk Request", 0x07),
	/*0x03*/ SMSG("Client Talk", SIZE_VARIABLE),
	/*0x04*/ SMSG("Request God mode (God client)", 0x02),
	/*0x05*/ SMSG("Attack", 0x05),
	/*0x06*/ SMSG("Double Click", 0x05),
	/*0x07*/ SMSG("Pick Up Item", 0x07),
	/*0x08*/ SMSG("Drop Item", 0x0e),
	/*0x09*/ SMSG("Single Click", 0x05),
	/*0x0A*/ SMSG("Edit (God client)", 0x0b),
	/*0x0B*/ RMSG("Damage Visualization",0x07),
	/*0x0C*/ BMSG("Edit tiledata (God client)", SIZE_VARIABLE),
	/*0x0D*/ UMSG(0x03),
	/*0x0E*/ UMSG(0x01),
	/*0x0F*/ UMSG(0x3d),
	/*0x10*/ UMSG(0xd7),
	/*0x11*/ RMSGH("Character Status", SIZE_VARIABLE, CharacterStatus),
	/*0x12*/ SMSG("Perform Action", SIZE_VARIABLE),
	/*0x13*/ SMSG("Client Equip Item", 0x0a),
	/*0x14*/ SMSG("Send elevation (God client)", 0x06),
	/*0x15*/ BMSG("Follow", 0x09),
	/*0x16*/ UMSG(0x01),
	/*0x17*/ RMSG("Health status bar update (KR)", SIZE_VARIABLE),
	/*0x18*/ UMSG(SIZE_VARIABLE),
	/*0x19*/ UMSG(SIZE_VARIABLE),
	/*0x1A*/ RMSGH("Update Item", SIZE_VARIABLE, UpdateItem),
	/*0x1B*/ RMSGH("Enter World", 0x25, EnterWorld),
	/*0x1C*/ RMSGH("Server Talk", SIZE_VARIABLE, Talk),
	/*0x1D*/ RMSGH("Delete Object", 0x05, DeleteObject),
	/*0x1E*/ UMSG(0x04),
	/*0x1F*/ UMSG(0x08),
	/*0x20*/ RMSGH("Update Player", 0x13, UpdatePlayer),
	/*0x21*/ RMSGH("Deny Walk", 0x08, DenyWalk),
	/*0x22*/ BMSGH("Confirm Walk", 0x03, ConfirmWalk),
	/*0x23*/ RMSGH("Drag Animation", 0x1a, DragAnimation),
	/*0x24*/ RMSGH("Open Container", 0x07, OpenContainer),
	/*0x25*/ RMSGH("Update Contained Item", 0x14, UpdateContainedItem),
	/*0x26*/ UMSG(0x05),
	/*0x27*/ RMSGH("Deny Move Item", 0x02, DenyMoveItem),
	/*0x28*/ UMSG(0x05),
	/*0x29*/ RMSG("Drop Item Approved", 0x01),
	/*0x2A*/ UMSG(0x05),
	/*0x2B*/ UMSG(0x02),
	/*0x2C*/ BMSGH("Death Screen", 0x02, DeathScreen),
	/*0x2D*/ RMSG("Mob Attributes", 0x11),
	/*0x2E*/ RMSGH("Server Equip Item", 0x0f, EquipItem),
	/*0x2F*/ RMSG("Combat Notification", 0x0a),
	/*0x30*/ RMSG("Attack ok", 0x05),
	/*0x31*/ RMSG("Attack end", 0x01),
	/*0x32*/ UMSG(0x02),
	/*0x33*/ RMSGH("Pause Control", 0x02, PauseControl),
	/*0x34*/ SMSG("Status Request", 0x0a),
	/*0x35*/ UMSG(0x28d),
	/*0x36*/ UMSG(SIZE_VARIABLE),
	/*0x37*/ UMSG(0x08),
	/*0x38*/ BMSG("Pathfinding in Client", 0x07),
	/*0x39*/ RMSG("Remove (Group)", 0x09),
	/*0x3A*/ BMSGH("Update Skills", SIZE_VARIABLE, UpdateSkills),
	/*0x3B*/ BMSGH("Vendor Buy Reply", SIZE_VARIABLE, BuyReply),
	/*0x3C*/ RMSGH("Update Contained Items", SIZE_VARIABLE, UpdateContainedItems),
	/*0x3D*/ UMSG(0x02),
	/*0x3E*/ UMSG(0x25),
	/*0x3F*/ RMSG("Update Statics (God Client)", SIZE_VARIABLE),
	/*0x40*/ UMSG(0xc9),
	/*0x41*/ UMSG(SIZE_VARIABLE),
	/*0x42*/ UMSG(SIZE_VARIABLE),
	/*0x43*/ UMSG(0x229),
	/*0x44*/ UMSG(0x2c9),
	/*0x45*/ BMSG("Version OK", 0x05),
	/*0x46*/ UMSG(SIZE_VARIABLE),
	/*0x47*/ UMSG(0x0b),
	/*0x48*/ UMSG(0x49),
	/*0x49*/ UMSG(0x5d),
	/*0x4A*/ UMSG(0x05),
	/*0x4B*/ UMSG(0x09),
	/*0x4C*/ UMSG(SIZE_VARIABLE),
	/*0x4D*/ UMSG(SIZE_VARIABLE),
	/*0x4E*/ RMSGH("Personal Light Level", 0x06, PersonalLightLevel),
	/*0x4F*/ RMSGH("Global Light Level", 0x02, LightLevel),
	/*0x50*/ UMSG(SIZE_VARIABLE),
	/*0x51*/ UMSG(SIZE_VARIABLE),
	/*0x52*/ UMSG(SIZE_VARIABLE),
	/*0x53*/ RMSGH("Error Code", 0x02, ErrorCode),
	/*0x54*/ RMSGH("Sound Effect", 0x0c, PlaySoundEffect),
	/*0x55*/ RMSGH("Login Complete", 0x01, LoginComplete),
	/*0x56*/ BMSGH("Map Data", 0x0b, MapData),
	/*0x57*/ BMSG("Update Regions", 0x6e),
	/*0x58*/ UMSG(0x6a),
	/*0x59*/ UMSG(SIZE_VARIABLE),
	/*0x5A*/ UMSG(SIZE_VARIABLE),
	/*0x5B*/ RMSGH("Set Time", 0x04, SetTime),
	/*0x5C*/ BMSG("Restart Version", 0x02),
	/*0x5D*/ SMSG("Select Character", 0x49),
	/*0x5E*/ UMSG(SIZE_VARIABLE),
	/*0x5F*/ UMSG(0x31),
	/*0x60*/ UMSG(0x05),
	/*0x61*/ UMSG(0x09),
	/*0x62*/ UMSG(0x0f),
	/*0x63*/ UMSG(0x0d),
	/*0x64*/ UMSG(0x01),
	/*0x65*/ RMSGH("Set Weather", 0x04, SetWeather),
	/*0x66*/ BMSGH("Book Page Data", SIZE_VARIABLE, BookData),
	/*0x67*/ UMSG(0x15),
	/*0x68*/ UMSG(SIZE_VARIABLE),
	/*0x69*/ UMSG(SIZE_VARIABLE),
	/*0x6A*/ UMSG(0x03),
	/*0x6B*/ UMSG(0x09),
	/*0x6C*/ BMSGH("Target Data", 0x13, Target),
	/*0x6D*/ RMSGH("Play Music", 0x03, PlayMusic),
	/*0x6E*/ RMSGH("Character Animation", 0x0e, CharacterAnimation),
	/*0x6F*/ BMSGH("Secure Trading", SIZE_VARIABLE, SecureTrading),
	/*0x70*/ RMSGH("Graphic Effect", 0x1c, GraphicEffect),
	/*0x71*/ BMSGH("Bulletin Board Data", SIZE_VARIABLE, BulletinBoardData),
	/*0x72*/ BMSGH("War Mode", 0x05, Warmode),
	/*0x73*/ BMSGH("Ping", 0x02, Ping),
	/*0x74*/ RMSGH("Vendor Buy List", SIZE_VARIABLE, BuyList),
	/*0x75*/ SMSG("Rename Character", 0x23),
	/*0x76*/ RMSG("New Subserver", 0x10),
	/*0x77*/ RMSGH("Update Character", 0x11, UpdateCharacter),
	/*0x78*/ RMSGH("Update Object", SIZE_VARIABLE, UpdateObject),
	/*0x79*/ UMSG(0x09),
	/*0x7A*/ UMSG(SIZE_VARIABLE),
	/*0x7B*/ UMSG(0x02),
	/*0x7C*/ RMSGH("Open Menu Gump", SIZE_VARIABLE, OpenMenuGump),
	/*0x7D*/ SMSG("Menu Choice", 0x0d),
	/*0x7E*/ UMSG(0x02),
	/*0x7F*/ UMSG(SIZE_VARIABLE),
	/*0x80*/ SMSG("First Login", 0x3e),
	/*0x81*/ UMSG(SIZE_VARIABLE),
	/*0x82*/ RMSGH("Login Error", 0x02, LoginError),
	/*0x83*/ SMSG("Delete Character", 0x27),
	/*0x84*/ UMSG(0x45),
	/*0x85*/ RMSGH("Character List Notification", 0x02, CharacterListNotification),
	/*0x86*/ RMSGH("Resend Character List", SIZE_VARIABLE, ResendCharacterList),
	/*0x87*/ UMSG(SIZE_VARIABLE),
	/*0x88*/ RMSGH("Open Paperdoll", 0x42, OpenPaperdoll),
	/*0x89*/ RMSGH("Corpse Equipment", SIZE_VARIABLE, CorpseEquipment),
	/*0x8A*/ UMSG(SIZE_VARIABLE),
	/*0x8B*/ UMSG(SIZE_VARIABLE),
	/*0x8C*/ RMSGH("Relay Server", 0x0b, RelayServer),
	/*0x8D*/ UMSG(SIZE_VARIABLE),
	/*0x8E*/ UMSG(SIZE_VARIABLE),
	/*0x8F*/ UMSG(SIZE_VARIABLE),
	/*0x90*/ RMSGH("Display Map", 0x13, DisplayMap),
	/*0x91*/ SMSG("Second Login", 0x41),
	/*0x92*/ UMSG(SIZE_VARIABLE),
	/*0x93*/ RMSGH("Open Book", 0x63, OpenBook),
	/*0x94*/ UMSG(SIZE_VARIABLE),
	/*0x95*/ BMSGH("Dye Data", 0x09, DyeData),
	/*0x96*/ UMSG(SIZE_VARIABLE),
	/*0x97*/ RMSG("Move Player", 0x02),
	/*0x98*/ BMSG("All Names (3D Client Only)", SIZE_VARIABLE),
	/*0x99*/ BMSGH("Multi Placement", 0x1a, MultiPlacement),
	/*0x9A*/ BMSGH("ASCII Prompt", SIZE_VARIABLE, ASCIIPrompt),
	/*0x9B*/ SMSG("Help Request", 0x102),
	/*0x9C*/ UMSG(0x135),
	/*0x9D*/ UMSG(0x33),
	/*0x9E*/ RMSGH("Vendor Sell List", SIZE_VARIABLE, SellList),
	/*0x9F*/ SMSG("Vendor Sell Reply", SIZE_VARIABLE),
	/*0xA0*/ SMSG("Select Server", 0x03),
	/*0xA1*/ RMSGH("Update Hitpoints", 0x09, UpdateHitpoints),
	/*0xA2*/ RMSGH("Update Mana", 0x09, UpdateMana),
	/*0xA3*/ RMSGH("Update Stamina", 0x09, UpdateStamina),
	/*0xA4*/ SMSG("System Information", 0x95),
	/*0xA5*/ RMSGH("Open URL", SIZE_VARIABLE, OpenUrl),
	/*0xA6*/ RMSGH("Tip Window", SIZE_VARIABLE, TipWindow),
	/*0xA7*/ SMSG("Request Tip", 0x04),
	/*0xA8*/ RMSGH("Server List", SIZE_VARIABLE, ServerList),
	/*0xA9*/ RMSGH("Character List", SIZE_VARIABLE, CharacterList),
	/*0xAA*/ RMSGH("Attack Reply", 0x05, AttackCharacter),
	/*0xAB*/ RMSGH("Text Entry Dialog", SIZE_VARIABLE, TextEntryDialog),
	/*0xAC*/ SMSG("Text Entry Dialog Reply", SIZE_VARIABLE),
	/*0xAD*/ SMSG("Unicode Client Talk", SIZE_VARIABLE),
	/*0xAE*/ RMSGH("Unicode Server Talk", SIZE_VARIABLE, UnicodeTalk),
	/*0xAF*/ RMSGH("Display Death", 0x0d, DisplayDeath),
	/*0xB0*/ RMSGH("Open Dialog Gump", SIZE_VARIABLE, OpenGump),
	/*0xB1*/ SMSG("Dialog Choice", SIZE_VARIABLE),
	/*0xB2*/ BMSG("Chat Data", SIZE_VARIABLE),
	/*0xB3*/ RMSG("Chat Text ?", SIZE_VARIABLE),
	/*0xB4*/ UMSG(SIZE_VARIABLE),
	/*0xB5*/ BMSG("Open Chat Window", 0x40),
	/*0xB6*/ SMSG("Popup Help Request", 0x09),
	/*0xB7*/ RMSG("Popup Help Data", SIZE_VARIABLE),
	/*0xB8*/ BMSGH("Character Profile", SIZE_VARIABLE, CharacterProfile),
	/*0xB9*/ RMSGH("Enable locked client features", 0x03, EnableLockedFeatures),
	/*0xBA*/ RMSGH("Display Quest Arrow", 0x06, DisplayQuestArrow),
	/*0xBB*/ SMSG("Account ID ?", 0x09),
	/*0xBC*/ RMSGH("Season", 0x03, Season),
	/*0xBD*/ BMSGH("Client Version", SIZE_VARIABLE, ClientVersion),
	/*0xBE*/ BMSGH("Assist Version", SIZE_VARIABLE, AssistVersion),
	/*0xBF*/ BMSGH("Extended Command", SIZE_VARIABLE, ExtendedCommand),
	/*0xC0*/ RMSGH("Graphical Effect", 0x24, GraphicEffect),
	/*0xC1*/ RMSG("Display cliloc String", SIZE_VARIABLE),
	/*0xC2*/ BMSGH("Unicode prompt", SIZE_VARIABLE, UnicodePrompt),
	/*0xC3*/ UMSG(SIZE_VARIABLE),
	/*0xC4*/ UMSG(0x06),
	/*0xC5*/ UMSG(0xcb),
	/*0xC6*/ UMSG(0x01),
	/*0xC7*/ UMSG(0x31),
	/*0xC8*/ BMSGH("Client View Range", 0x02, ClientViewRange),
	/*0xC9*/ UMSG(0x06),
	/*0xCA*/ UMSG(0x06),
	/*0xCB*/ UMSG(0x07),
	/*0xCC*/ RMSG("Localized Text Plus String", SIZE_VARIABLE),
	/*0xCD*/ UMSG(0x01),
	/*0xCE*/ UMSG(SIZE_VARIABLE),
	/*0xCF*/ UMSG(0x4e),
	/*0xD0*/ UMSG(SIZE_VARIABLE),
	/*0xD1*/ UMSG(0x02),
	/*0xD2*/ RMSGH("Update Player (New)", 0x19, UpdatePlayer),
	/*0xD3*/ RMSGH("Update Object (New)", SIZE_VARIABLE, UpdateObject),
	/*0xD4*/ BMSGH("Open Book (New)", SIZE_VARIABLE, OpenBookNew),
	/*0xD5*/ UMSG(SIZE_VARIABLE),
	/*0xD6*/ BMSG("+Mega cliloc", SIZE_VARIABLE),
	/*0xD7*/ BMSG("+AoS command", SIZE_VARIABLE),
	/*0xD8*/ RMSG("+Custom house", SIZE_VARIABLE),
	/*0xD9*/ SMSG("+Metrics", 0x10c),
	/*0xDA*/ UMSG(SIZE_VARIABLE),
	/*0xDB*/ UMSG(SIZE_VARIABLE),
	/*0xDC*/ RMSG("OPL Info Packet", 9),
	/*0xDD*/ RMSGH("Compressed Gump", SIZE_VARIABLE, OpenCompressedGump),
	/*0xDE*/ UMSG(SIZE_VARIABLE),
	/*0xDF*/ RMSG("Buff/Debuff", SIZE_VARIABLE),
	/*0xE0*/ SMSG("Bug Report KR", SIZE_VARIABLE),
	/*0xE1*/ SMSG("Client Type KR/SA", 0x09),
	/*0xE2*/ RMSG("New Character Animation", 0xa),
	/*0xE3*/ RMSG("KR Encryption Responce", 0x4d),
	/*0xE4*/ UMSG(SIZE_VARIABLE),
	/*0xE5*/ UMSG(SIZE_VARIABLE),
	/*0xE6*/ UMSG(SIZE_VARIABLE),
	/*0xE7*/ UMSG(SIZE_VARIABLE),
	/*0xE8*/ UMSG(SIZE_VARIABLE),
	/*0xE9*/ UMSG(SIZE_VARIABLE),
	/*0xEA*/ UMSG(SIZE_VARIABLE),
	/*0xEB*/ UMSG(SIZE_VARIABLE),
	/*0xEC*/ SMSG("Equip Macro", SIZE_VARIABLE),
	/*0xED*/ SMSG("Unequip item macro", SIZE_VARIABLE),
	/*0xEE*/ UMSG(SIZE_VARIABLE),
	/*0xEF*/ SMSG("KR/2D Client Login/Seed", 0x15),
	/*0xF0*/ BMSGH("Karrios client special", SIZE_VARIABLE, KarriosClientSpecial),
	/*0xF1*/ BMSG("Freeshard List", SIZE_VARIABLE),
	/*0xF2*/ UMSG(SIZE_VARIABLE),
	/*0xF3*/ RMSGH("Update Item (SA)", 0x18, UpdateItemSA),
	/*0xF4*/ UMSG(SIZE_VARIABLE),
	/*0xF5*/ RMSGH("Display New Map", 0x15, DisplayMap),
	/*0xF6*/ UMSG(SIZE_VARIABLE),
	/*0xF7*/ UMSG(SIZE_VARIABLE),
	/*0xF8*/ SMSG("Character Creation (7.0.16.0)", 0x6a),
	/*0xF9*/ UMSG(SIZE_VARIABLE),
	/*0xFA*/ UMSG(SIZE_VARIABLE),
	/*0xFB*/ UMSG(SIZE_VARIABLE),
	/*0xFC*/ UMSG(SIZE_VARIABLE),
	/*0xFE*/ UMSG(SIZE_VARIABLE),
	/*0xFD*/ RMSG("Razor Handshake", 0x8),
	/*0xFF*/ UMSG(SIZE_VARIABLE)
};
//---------------------------------------------------------------------------
TPacketManager::TPacketManager()
: m_ClientVersion(CV_OLD)
{
}
//---------------------------------------------------------------------------
#define CV_PRINT 0

#if CV_PRINT!=0
#define CVPRINT(s) trace_printf(s)
#else //CV_PRINT==0
#define CVPRINT(s)
#endif //CV_PRINT!=0
//---------------------------------------------------------------------------
void TPacketManager::SetClientVersion(CLIENT_VERSION cver)
{
	m_ClientVersion = cver;
	
	if (m_ClientVersion >= CV_500A)
	{
		CVPRINT("Set new length for packet 0x0B (>= 5.0.0a)\n");
		m_MessageTypes[0x0B].size = 0x07;
		CVPRINT("Set new length for packet 0x16 (>= 5.0.0a)\n");
		m_MessageTypes[0x16].size = SIZE_VARIABLE;
		CVPRINT("Set new length for packet 0x31 (>= 5.0.0a)\n");
		m_MessageTypes[0x31].size = SIZE_VARIABLE;
	}
	else
	{
		CVPRINT("Set standart length for packet 0x0B (< 5.0.0a)\n");
		m_MessageTypes[0x0B].size = 0x10A;
		CVPRINT("Set standart length for packet 0x16 (< 5.0.0a)\n");
		m_MessageTypes[0x16].size = 0x01;
		CVPRINT("Set standart length for packet 0x31 (< 5.0.0a)\n");
		m_MessageTypes[0x31].size = 0x01;
	}

	if (m_ClientVersion >= CV_5090)
	{
		CVPRINT("Set new length for packet 0xE1 (>= 5.0.9.0)\n");
		m_MessageTypes[0xE1].size = SIZE_VARIABLE;
	}
	else
	{
		CVPRINT("Set standart length for packet 0xE1 (<= 5.0.9.0)\n");
		m_MessageTypes[0xE1].size = 0x09;
	}

	if (m_ClientVersion >= CV_6013)
	{
		CVPRINT("Set new length for packet 0xE3 (>= 6.0.1.3)\n");
		m_MessageTypes[0xE3].size = SIZE_VARIABLE;
		CVPRINT("Set new length for packet 0xE6 (>= 6.0.1.3)\n");
		m_MessageTypes[0xE6].size = 0x05;
		CVPRINT("Set new length for packet 0xE7 (>= 6.0.1.3)\n");
		m_MessageTypes[0xE7].size = 0x0C;
		CVPRINT("Set new length for packet 0xE8 (>= 6.0.1.3)\n");
		m_MessageTypes[0xE8].size = 0x0D;
		CVPRINT("Set new length for packet 0xE9 (>= 6.0.1.3)\n");
		m_MessageTypes[0xE9].size = 0x4B;
		CVPRINT("Set new length for packet 0xEA (>= 6.0.1.3)\n");
		m_MessageTypes[0xEA].size = 0x03;
	}
	else
	{
		CVPRINT("Set standart length for packet 0xE3 (<= 6.0.1.3)\n");
		m_MessageTypes[0xE3].size = 0x4D;
		CVPRINT("Set standart length for packet 0xE6 (<= 6.0.1.3)\n");
		m_MessageTypes[0xE6].size = SIZE_VARIABLE;
		CVPRINT("Set standart length for packet 0xE7 (<= 6.0.1.3)\n");
		m_MessageTypes[0xE7].size = SIZE_VARIABLE;
		CVPRINT("Set standart length for packet 0xE8 (<= 6.0.1.3)\n");
		m_MessageTypes[0xE8].size = SIZE_VARIABLE;
		CVPRINT("Set standart length for packet 0xE9 (<= 6.0.1.3)\n");
		m_MessageTypes[0xE9].size = SIZE_VARIABLE;
		CVPRINT("Set standart length for packet 0xEA (<= 6.0.1.3)\n");
		m_MessageTypes[0xEA].size = SIZE_VARIABLE;
	}

	if (m_ClientVersion >= CV_6017)
	{
		CVPRINT("Set new length for packet 0x08 (>= 6.0.1.7)\n");
		m_MessageTypes[0x08].size = 0x0F;
		CVPRINT("Set new length for packet 0x25 (>= 6.0.1.7)\n");
		m_MessageTypes[0x25].size = 0x15;
	}
	else
	{
		CVPRINT("Set standart length for packet 0x08 (<= 6.0.1.7)\n");
		m_MessageTypes[0x08].size = 0x0E;
		CVPRINT("Set standart length for packet 0x25 (<= 6.0.1.7)\n");
		m_MessageTypes[0x25].size = 0x14;
	}

	if (m_ClientVersion == CV_6060)
	{
		CVPRINT("Set new length for packet 0xEE (>= 6.0.6.0)\n");
		m_MessageTypes[0xEE].size = 0x2000;
		CVPRINT("Set new length for packet 0xEF (>= 6.0.6.0)\n");
		m_MessageTypes[0xEF].size = 0x2000;
		CVPRINT("Set new length for packet 0xF1 (>= 6.0.6.0)\n");
		m_MessageTypes[0xF1].size = 0x09;
	}
	else
	{
		CVPRINT("Set standart length for packet 0xEE (<= 6.0.6.0)\n");
		m_MessageTypes[0xEE].size = SIZE_VARIABLE;
		CVPRINT("Set standart length for packet 0xEF (<= 6.0.6.0)\n");
		m_MessageTypes[0xEF].size = 0x15;
		CVPRINT("Set standart length for packet 0xF1 (<= 6.0.6.0)\n");
		m_MessageTypes[0xF1].size = SIZE_VARIABLE;
	}

	if (m_ClientVersion >= CV_60142)
	{
		CVPRINT("Set new length for packet 0xB9 (>= 6.0.14.2)\n");
		m_MessageTypes[0xB9].size = 0x05;
	}
	else
	{
		CVPRINT("Set standart length for packet 0xB9 (<= 6.0.14.2)\n");
		m_MessageTypes[0xB9].size = 0x03;
	}

	if (m_ClientVersion >= CV_7000)
	{
		CVPRINT("Set new length for packet 0xEE (>= 7.0.0.0)\n");
		m_MessageTypes[0xEE].size = 0x2000;
		CVPRINT("Set new length for packet 0xEF (>= 7.0.0.0)\n");
		m_MessageTypes[0xEF].size = 0x2000;
		/*CVPRINT("Set new length for packet 0xF0 (>= 7.0.0.0)\n");
		m_MessageTypes[0xF0].size = 0x2000;
		CVPRINT("Set new length for packet 0xF1 (>= 7.0.0.0)\n");
		m_MessageTypes[0xF1].size = 0x2000;
		CVPRINT("Set new length for packet 0xF2 (>= 7.0.0.0)\n");
		m_MessageTypes[0xF2].size = 0x2000;*/
	}
	else
	{
		CVPRINT("Set standart length for packet 0xEE (<= 7.0.0.0)\n");
		m_MessageTypes[0xEE].size = SIZE_VARIABLE;
		CVPRINT("Set standart length for packet 0xEF (<= 7.0.0.0)\n");
		m_MessageTypes[0xEF].size = 0x15;
		/*CVPRINT("Set standart length for packet 0xF0 (<= 7.0.0.0)\n");
		m_MessageTypes[0xF0].size = SIZE_VARIABLE;
		CVPRINT("Set standart length for packet 0xF1 (<= 7.0.0.0)\n");
		m_MessageTypes[0xF1].size = SIZE_VARIABLE;
		CVPRINT("Set standart length for packet 0xF2 (<= 7.0.0.0)\n");
		m_MessageTypes[0xF2].size = SIZE_VARIABLE;*/
	}

	if (m_ClientVersion >= CV_7090)
	{
		CVPRINT("Set new length for packet 0x24 (>= 7.0.9.0)\n");
		m_MessageTypes[0x24].size = 0x09;
		CVPRINT("Set new length for packet 0x99 (>= 7.0.9.0)\n");
		m_MessageTypes[0x99].size = 0x1E;
		CVPRINT("Set new length for packet 0xBA (>= 7.0.9.0)\n");
		m_MessageTypes[0xBA].size = 0x0A;
		CVPRINT("Set new length for packet 0xF3 (>= 7.0.9.0)\n");
		m_MessageTypes[0xF3].size = 0x1A;

		//� ������� 7.0.8.2 ��� ��������
		CVPRINT("Set new length for packet 0xF1 (>= 7.0.9.0)\n");
		m_MessageTypes[0xF1].size = 0x09;
		CVPRINT("Set new length for packet 0xF2 (>= 7.0.9.0)\n");
		m_MessageTypes[0xF2].size = 0x19;
	}
	else
	{
		CVPRINT("Set standart length for packet 0x24 (<= 7.0.9.0)\n");
		m_MessageTypes[0x24].size = 0x07;
		CVPRINT("Set standart length for packet 0x99 (<= 7.0.9.0)\n");
		m_MessageTypes[0x99].size = 0x1A;
		CVPRINT("Set standart length for packet 0xBA (<= 7.0.9.0)\n");
		m_MessageTypes[0xBA].size = 0x06;
		CVPRINT("Set standart length for packet 0xF3 (<= 7.0.9.0)\n");
		m_MessageTypes[0xF3].size = 0x18;

		//� ������� 7.0.8.2 ��� ��������
		CVPRINT("Set standart length for packet 0xF1 (<= 7.0.9.0)\n");
		m_MessageTypes[0xF1].size = SIZE_VARIABLE;
		CVPRINT("Set standart length for packet 0xF2 (<= 7.0.9.0)\n");
		m_MessageTypes[0xF2].size = SIZE_VARIABLE;
	}

	if (m_ClientVersion >= CV_70180)
	{
		CVPRINT("Set new length for packet 0x00 (>= 7.0.18.0)\n");
		m_MessageTypes[0x00].size = 0x6A;
	}
	else
	{
		CVPRINT("Set standart length for packet 0x24 (<= 7.0.18.0)\n");
		m_MessageTypes[0x00].size = 0x68;
	}
}
//---------------------------------------------------------------------------
BYTE TPacketManager::ReadByte()
{
	BYTE result = *Ptr;

	Ptr++;

	return result;
}
//---------------------------------------------------------------------------
WORD TPacketManager::ReadWord()
{
	WORD result = unpack16(Ptr);

	Ptr += 2;

	return result;
}
//---------------------------------------------------------------------------
DWORD TPacketManager::ReadDWord()
{
	DWORD result = unpack32(Ptr);

	Ptr += 4;

	return result;
}
//---------------------------------------------------------------------------
char TPacketManager::ReadChar()
{
	char result = (char)*Ptr;

	Ptr++;

	return result;
}
//---------------------------------------------------------------------------
short TPacketManager::ReadShort()
{
	short result = (short)unpack16(Ptr);

	Ptr += 2;

	return result;
}
//---------------------------------------------------------------------------
int TPacketManager::ReadInt()
{
	int result = (int)unpack32(Ptr);

	Ptr += 4;

	return result;
}
//---------------------------------------------------------------------------
string TPacketManager:: ReadString(int size)
{
	if (!size)
	{
		PBYTE buf = Ptr;

		while (*buf)
			buf++;

		size = (buf - Ptr) + 1;
	}

	char *str = new char[size + 1];
	memcpy(&str[0], &Ptr[0], size);
	str[size] = 0;

	Ptr += size;

	string result(str);
	delete str;

	return result;
}
//---------------------------------------------------------------------------
wstring TPacketManager::ReadUnicodeString(int size)
{
	int count = size;

	if (!count)
		count = 1024;

	wchar_t *str = new wchar_t[count + 1];
	wchar_t *pStr = str;

	IFOR(i, 0, count)
	{
		*pStr = (Ptr[0] << 8) | Ptr[1];
		Ptr += 2;
			
		if (!size && *pStr == 0)
			break;

		pStr++;
	}

	str[count] = 0;

	wstring result(str);
	delete str;

	return result;
}
//---------------------------------------------------------------------------
int TPacketManager::GetPacketSize(BYTE msg)
{
	if ((int)msg < 0 || (int)msg >= 0x100)
		return -1;

	return m_MessageTypes[(int)msg].size;
}
//---------------------------------------------------------------------------
void TPacketManager::ReceiveHandler(PBYTE buf, int size)
{
	DWORD ticks = GetTickCount();
	g_TotalRecvSize += size;
	
	TMessageType &type = m_MessageTypes[*buf];

	trace_printf("--- ^(%d) r(+%d => %d) Server:: %s\n", ticks - g_LastPacketTime, size, g_TotalRecvSize, type.name);
	TDUMP(buf, size);
	
	g_LastPacketTime = ticks;

	if (type.direction != DIR_RECV && type.direction != DIR_BOTH)
		warning_printf("message direction invalid: 0x%02X\n", *buf);
	else if (type.handler != 0)
	{
		if (PluginManager->PacketRecv(buf, size))
		{
			Ptr = buf + 1;

			if (!type.size)
				Ptr += 2;

			(this->*(type.handler))(buf, size);
		}
	}
}
//---------------------------------------------------------------------------
#define PACKET_HANDLER(name) void TPacketManager::Handle ##name (PBYTE buf, int size)
//---------------------------------------------------------------------------
PACKET_HANDLER(LoginError)
{
	if (g_GameState == GS_MAIN_CONNECT || g_GameState == GS_SERVER_CONNECT)
	{
		ConnectionScreen->ConnectionFailed = true;
		ConnectionScreen->ErrorCode = ReadByte();
		ConnectionManager.Disconnect();
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(ServerList)
{
	// We need to remember the server list so that we know the name of
	// the server when the relay message comes back from the server.

	ServerList.Clear();
	Move(1);

	int numServers = ReadShort();
	ServerList.Init(numServers);

	if (numServers == 0)
		WPRINT("empty server list\n");

	// Calculate expected message size
	int exSize = 6 + numServers * 40;
	if (size != exSize)
		warning_printf("server list message size should be %d\n", exSize);

	IFOR(i, 0, numServers)
	{
		WORD id = ReadWord();
		ServerList.SetServer(i, id, (char*)Ptr);
		Move(40);
	}

	if (numServers && MainScreen->AutoLogin)
		UO->ServerSelection(0);
	else
		UO->InitScreen(GS_SERVER);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(RelayServer)
{
	in_addr addr;
	PDWORD paddr = (PDWORD)Ptr;
	Move(4);
	addr.S_un.S_addr = *paddr;
	char relayIP[30] = {0};
	strncpy(relayIP, inet_ntoa(addr), 29);
	int relayPort = ReadShort();
	UO->RelayServer(relayIP, relayPort, Ptr);
	g_PacketLoginComplete = false;
	g_CurrentMap = 0;
}
//---------------------------------------------------------------------------
PACKET_HANDLER(CharacterList)
{
	UO->InitScreen(GS_CHARACTER);

	int numSlots = ReadChar();
	TPRINT("/======Chars===\n");

	CharacterList.Clear();
	CharacterList.Count = numSlots;

	int AutoPos = -1;
	bool autoLogin = MainScreen->AutoLogin;

	if (numSlots == 0)
		WPRINT("no slots in character list\n");
	else
	{
		IFOR(i, 0, numSlots)
		{
			CharacterList.SetName(i, (char*)Ptr);

			if (autoLogin && AutoPos == -1 && UO->AutoLoginNameExists((char*)Ptr))
				AutoPos = i;

			trace_printf("%d: %s\n", i, (char*)Ptr);

			Move(60);
		}
	}

	BYTE locCount = ReadByte();

	CityList->Clear();

	if (m_ClientVersion >= CV_70130)
	{
		IFOR(i, 0, locCount)
		{
			TCityItemNew *city = new TCityItemNew();

			city->LocationIndex = ReadByte();

			city->SetName(ReadString(32));
			city->SetArea(ReadString(32));

			city->X = ReadDWord();
			city->Y = ReadDWord();
			city->Z = ReadDWord();
			city->MapIndex = ReadDWord();

			DWORD clilocID = ReadDWord();

			city->CreateTextTexture(clilocID);

			Move(4);

			CityList->Add(city);
		}
	}
	else
	{
		IFOR(i, 0, locCount)
		{
			TCityItem *city = new TCityItem();

			city->LocationIndex = ReadByte();

			city->SetName(ReadString(31));
			city->SetArea(ReadString(31));

			city->InitCity();

			CityList->Add(city);
		}
	}

	g_ClientFlag = ReadDWord();

	CharacterList.OnePerson = (bool)(g_ClientFlag & LFF_TD);
	g_SendLogoutNotification = (bool)(g_ClientFlag & LFF_RE);
	g_NPCPopupEnabled = (bool)(g_ClientFlag & LFF_LBR);
	g_ChatEnabled = (bool)(g_ClientFlag & LFF_T2A);

	if (autoLogin && numSlots)
	{
		if (AutoPos == -1)
			AutoPos = 0;

		CharacterList.Selected = AutoPos ;

		if (CharacterList.GetName(AutoPos).length())
			UO->CharacterSelection(AutoPos);
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(ResendCharacterList)
{
	UO->InitScreen(GS_CHARACTER);

	int numSlots = ReadByte();
	TPRINT("/======Resend chars===\n");

	CharacterList.Clear();
	CharacterList.Count = numSlots;

	int AutoPos = -1;
	bool autoLogin = MainScreen->AutoLogin;

	if (numSlots == 0)
		WPRINT("no slots in character list\n");
	else
	{
		IFOR(i, 0, numSlots)
		{
			CharacterList.SetName(i, (char*)Ptr);

			if (autoLogin && AutoPos == -1 && UO->AutoLoginNameExists((char*)Ptr))
				AutoPos = i;

			trace_printf("%d: %s\n", i, (char*)Ptr);

			Move(60);
		}
	}

	if (autoLogin && numSlots)
	{
		if (AutoPos == -1)
			AutoPos = 0;

		CharacterList.Selected = AutoPos ;

		if (CharacterList.GetName(AutoPos).length())
			UO->CharacterSelection(AutoPos);
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(LoginComplete)
{
	g_PacketLoginComplete = true;

	if (g_Player->GetName().length())
		UO->LoginComplete();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(SetTime)
{
	g_ServerTimeHour = ReadByte();
	g_ServerTimeMinute = ReadByte();
	g_ServerTimeSecond = ReadByte();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(EnterWorld)
{
	DWORD serial = ReadDWord();

	if (World != NULL)
		error_printf("duplicate enter world message\n");
	else
	{
		if (World != NULL)
			delete World;
		World = new TGameWorld(serial);
		
		if (Walker != NULL)
			delete Walker;
		Walker = new TWalker();
		g_LastStepTime = 0;

		if (ObjectInHand != NULL)
			delete ObjectInHand;
		ObjectInHand = NULL;

		g_WalkRequestCount = 0;
		g_PingCount = 0;
		g_PingSequence = 0;
		g_ClickObject.Init(COT_NONE);
		Weather.Reset();
		g_LastConsoleEntry = L"";
		g_SkillsTotal = 0.0f;
		g_ConsolePrompt = PT_NONE;
		MacroPointer = NULL;
		g_Season = 0;
	}

	Move(4);

	g_Player->Graphic = ReadWord();
	g_Player->UpdateSex();

	g_Player->X = ReadWord();
	g_Player->Y = ReadWord();
	Move(1);
	g_Player->Z = ReadByte();
	BYTE dir = ReadByte();
	g_Player->Direction = dir;
	g_Player->Flags = buf[28];

	MapManager->Init();
	
	MapManager->AddRender(g_Player);

	Walker->SetSequence(0, dir);
	g_Player->OffsetX = 0;
	g_Player->OffsetY = 0;
	g_Player->OffsetZ = 0;

	trace_printf("Player 0x%08lX entered the world.\n", serial);

	UO->LoadStartupConfig();

	g_LastSpellIndex = 0;
	g_LastSkillIndex = 1;

	UO->Click(g_PlayerSerial);
	
	UO->StatusReq(g_PlayerSerial);
	
	TPacketGameWindowSize packet;
	packet.Send();
	
	BYTE wbuf[4] = {0x65, 0x02, 0x3C, 0};
	Ptr = wbuf + 1;
	HandleSetWeather(wbuf, 4);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdateHitpoints)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();

	TGameCharacter *obj = World->FindWorldCharacter(serial);
	if (obj == NULL)
		return;
	
	obj->MaxHits = ReadShort();
	obj->Hits = ReadShort();
	
	TGump *gump = GumpManager->GetGump(serial, 0, GT_STATUSBAR);
	if (gump != NULL)
		gump->UpdateFrame();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdateMana)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();

	TGameCharacter *obj = World->FindWorldCharacter(serial);
	if (obj == NULL)
		return;

	obj->MaxMana = ReadShort();
	obj->Mana = ReadShort();
	
	TGump *gump = GumpManager->GetGump(serial, 0, GT_STATUSBAR);
	if (gump != NULL)
		gump->UpdateFrame();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdateStamina)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();

	TGameCharacter *obj = World->FindWorldCharacter(serial);
	if (obj == NULL)
		return;

	obj->MaxStam = ReadShort();
	obj->Stam = ReadShort();
	
	TGump *gump = GumpManager->GetGump(serial, 0, GT_STATUSBAR);
	if (gump != NULL)
		gump->UpdateFrame();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdatePlayer)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();

	if (serial != g_PlayerSerial)
	{
		warning_printf("Current player changed from 0x%08lX to 0x%08lX\n", g_PlayerSerial, serial);
		World->SetPlayer(serial);
	}

	g_Player->Graphic = ReadWord();
	
	//WORD LastX = g_Player->X;
	//WORD LastY = g_Player->Y;
	Move(1);

	g_Player->Color = ReadWord() & 0x7FFF;
	g_Player->Flags = ReadByte();
	g_Player->X = ReadWord();
	g_Player->Y = ReadWord();
	Move(2);

	g_Player->m_WalkStack.Clear();

	BYTE dir = ReadByte();
	Walker->SetSequence(0, dir);
	g_WalkRequestCount = 0;
	g_Player->OffsetX = 0;
	g_Player->OffsetY = 0;
	g_Player->OffsetZ = 0;

	g_Player->Direction = dir;
	g_Player->Z = ReadByte();
	
	World->MoveToTop(g_Player);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(CharacterStatus)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	TGameCharacter *obj = World->FindWorldCharacter(serial);
	if (obj == NULL)
		return;

	string name = ReadString(30);
	obj->SetName(name);

	obj->Hits = ReadShort();
	obj->MaxHits = ReadShort();

	obj->CanChangeName = (ReadByte() != 0);

	BYTE flag = ReadByte();

	if (flag > 0)
	{
		obj->Sex = ReadByte(); //buf[43];

		if (serial == g_PlayerSerial)
		{
			if (ConfigManager.StatReport && g_Player->Str)
			{
				short pstr = g_Player->Str;
				short pdex = g_Player->Dex;
				short pint = g_Player->Int;
			
				short nstr = ReadShort();
				short ndex = ReadShort();
				short nint = ReadShort();
			
				g_Player->Str = nstr;
				g_Player->Dex = ndex;
				g_Player->Int = nint;
			
				short cstr = nstr - pstr;
				short cdex = ndex - pdex;
				short cint = nint - pint;
			
				char str[64] = {0};
				if (cstr)
				{
					sprintf(str, "Your strength has changed by %d.  It is now %d.", cstr, nstr);
					UO->CreateTextMessage(TT_SYSTEM, 0, 3, 0x0170, str);
				}
			
				if (cdex)
				{
					sprintf(str, "Your dexterity has changed by %d.  It is now %d.", cdex, ndex);
					UO->CreateTextMessage(TT_SYSTEM, 0, 3, 0x0170, str);
				}
			
				if (cint)
				{
					sprintf(str, "Your intelligence has changed by %d.  It is now %d.", cint, nint);
					UO->CreateTextMessage(TT_SYSTEM, 0, 3, 0x0170, str);
				}
			}
			else
			{
				g_Player->SetStr(ReadShort());
				g_Player->SetDex(ReadShort());
				g_Player->SetInt(ReadShort());
			}

			g_Player->Stam = ReadShort();
			g_Player->MaxStam = ReadShort();
			g_Player->Mana = ReadShort();
			g_Player->MaxMana = ReadShort();
			g_Player->Gold = ReadShort();
			Move(2);
			g_Player->Armor = ReadShort();
			g_Player->Weight = ReadShort(); //+64

			if (flag >= 5)
				g_Player->MaxWeight = ReadShort(); //unpack16(buf + 66);
			else
				g_Player->MaxWeight = (g_Player->Str * 4) + 25;

			if (!ConnectionScreen->Completed && g_PacketLoginComplete)
				UO->LoginComplete();
		}
	}
	
	TGump *gump = GumpManager->GetGump(serial, 0, GT_STATUSBAR);
	if (gump != NULL)
	{
		TGumpStatusbar *sbg = (TGumpStatusbar*)gump;

		if (sbg->TextEntry != NULL && sbg->TextEntry != EntryPointer)
			sbg->TextEntry->SetText(name);
		
		gump->UpdateFrame();
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdateItem)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	WORD graphic = ReadWord();
	WORD count = 0;

	if (serial & 0x80000000)
	{
		serial &= 0x7FFFFFFF;
		count = ReadWord();
	}

	TGameItem *obj = World->GetWorldItem(serial);
	if (obj == NULL)
	{
		TPRINT("no memory??");
		return;
	}
	
	if (ObjectInHand != NULL && ObjectInHand->Serial == obj->Serial)
	{
		delete ObjectInHand;
		ObjectInHand = NULL;
	}

	if (obj->Dragged)
		GumpManager->CloseGump(serial, 0, GT_DRAG);

	obj->Dragged = false;
	obj->MapIndex = g_CurrentMap;

	if (graphic & 0x8000)
	{
		graphic &= 0x7FFF;
		graphic += ReadByte();
	}

	if (!obj->Graphic)
		TPRINT("created ");
	else
		TPRINT("updated ");

	obj->Graphic = graphic;
	obj->Count = count;
	WORD X = ReadWord();
	WORD Y = ReadWord();
	BYTE dir = 0;

	if (X & 0x8000)
	{
		X &= 0x7FFF;

		//obj->Direction = *Ptr; //����������� ��������?
		dir = ReadByte();
	}

	if (obj->IsCorpse())
	{
		if (World->FindWorldCorpseOwner(serial))
			obj->AnimIndex = 0;
		else
			obj->AnimIndex = 99;

		if (dir & 0x80)
		{
			obj->UsedLayer = 1;
			dir &= 0x7F;
		}
		else
			obj->UsedLayer = 0;

		obj->Layer = dir;

		obj->RenderQueueIndex = 7;
	}

	obj->X = X;
	obj->Z = ReadByte();

	if (Y & 0x8000)
	{
		Y &= 0x7FFF;
		obj->Color = ReadWord();
	}
	else
		obj->Color = 0;

	if (Y & 0x4000)
	{
		Y &= 0x3FFF;
		obj->Flags = ReadByte();
	}
	
	obj->Y = Y;

	graphic = obj->Graphic;

	if (graphic < 0x4000)
	{
		STATIC_TILES &st = UO->m_StaticData[graphic / 32].Tiles[graphic % 32];
		obj->ObjectFlags = st.Flags;

		if (obj->IsWearable())
		{
			obj->ImageID = st.AnimID + 0xC350;
			obj->AnimID = st.AnimID;
			UO->ExecuteGump(st.AnimID + 50000, (st.Flags & 0x00040000));
			UO->ExecuteGump(st.AnimID + 60000, (st.Flags & 0x00040000));
			obj->UsedLayer = st.Quality;
		}

		if (!obj->IsCorpse())
		{
			if (obj->IsBackground())
				obj->RenderQueueIndex = 3 - (int)obj->IsSurface();
			else if (obj->IsSurface())
				obj->RenderQueueIndex = 4;
			else
				obj->RenderQueueIndex = 6;

			obj->IncRenderQueueIndex();
		}

		UO->ExecuteStaticArt(graphic);
	}
	else if (obj->m_Items == NULL)
		obj->LoadMulti();

	World->MoveToTop(obj);

	trace_printf("0x%08lX:0x%04X*%d %d:%d:%d\n", serial, graphic, obj->Count, obj->X, obj->Y, obj->Z);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdateItemSA)
{
	if (World == NULL)
		return;

	Move(2);
	char type = ReadByte(); //buf[3];
	DWORD serial = ReadDWord();
	WORD graphic = ReadWord();
	BYTE dir = ReadByte();
	WORD count = ReadWord();
	Move(2);
	WORD x = ReadWord();
	WORD y = ReadWord();
	char z = ReadByte();
	Move(1);
	WORD color = ReadWord();
	char flags = ReadByte();

	TGameItem *obj = World->GetWorldItem(serial);
	if (obj == NULL)
	{
		TPRINT("no memory??");
		return;
	}
	
	if (ObjectInHand != NULL && ObjectInHand->Serial == obj->Serial)
	{
		delete ObjectInHand;
		ObjectInHand = NULL;
	}

	if (obj->Dragged)
		GumpManager->CloseGump(serial, 0, GT_DRAG);

	obj->Dragged = false;
	obj->MapIndex = g_CurrentMap;

	if (!obj->Graphic)
		TPRINT("created ");
	else
		TPRINT("updated ");

	obj->Graphic = graphic;
	obj->Count = count;

	if (obj->IsCorpse())
	{
		if (World->FindWorldCorpseOwner(serial))
			obj->AnimIndex = 0;
		else
			obj->AnimIndex = 99;

		if (dir & 0x80)
		{
			obj->UsedLayer = 1;
			dir &= 0x7F;
		}
		else
			obj->UsedLayer = 0;

		obj->Layer = dir;

		obj->RenderQueueIndex = 7;
	}

	obj->X = x;
	obj->Y = y;
	obj->Z = z;
	obj->Color = color;

	obj->Flags = flags;

	trace_printf("0x%08lX:", serial);
	trace_printf("0x%04X*%d %d:%d:%d\n", obj->Graphic, obj->Count, obj->X, obj->Y, obj->Z);

	World->MoveToTop(obj);

	if (type != 0x02)
	{
		STATIC_TILES &st = UO->m_StaticData[graphic / 32].Tiles[graphic % 32];
		obj->ObjectFlags = st.Flags;

		if (obj->IsWearable())
		{
			obj->ImageID = st.AnimID + 0xC350;
			obj->AnimID = st.AnimID;
			UO->ExecuteGump(st.AnimID + 50000, (st.Flags & 0x00040000));
			UO->ExecuteGump(st.AnimID + 60000, (st.Flags & 0x00040000));
			obj->UsedLayer = st.Quality;
		}

		if (!obj->IsCorpse())
		{
			if (obj->IsBackground())
				obj->RenderQueueIndex = 3 - (int)obj->IsSurface();
			else if (obj->IsSurface())
				obj->RenderQueueIndex = 4;
			else
				obj->RenderQueueIndex = 6;

			obj->IncRenderQueueIndex();
		}

		UO->ExecuteStaticArt(graphic);
	}
	else
		obj->LoadMulti();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdateObject)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();

	TGameItem *item = NULL;
	TGameCharacter *character = NULL;
	TGameObject *obj = NULL;

	if (serial & 0x80000000)
	{
		item = World->GetWorldItem(serial & 0x7FFFFFFF);
		obj = item;
	}
	else
	{
		character = World->GetWorldCharacter(serial & 0x7FFFFFFF);
		obj = character;
	}

	if (obj == NULL)
	{
		TPRINT("no memory??");
		return;
	}

	if (ObjectInHand != NULL && ObjectInHand->Serial == obj->Serial)
	{
		delete ObjectInHand;
		ObjectInHand = NULL;
	}

	obj->MapIndex = g_CurrentMap;

	trace_printf("0x%08X ", serial);

	WORD graphic = ReadWord();

	obj->Graphic = graphic & 0x7FFF;
	trace_printf("0x%04X ", obj->Graphic);

	if (serial & 0x80000000)
	{
		obj->Count = ReadWord();
		obj->NPC = false;
	}
	else
	{
		obj->NPC = true;
		TPRINT("NPC ");
	}

	if (graphic & 0x8000)
	{
		//obj->GraphicIncrement = unpack16(ptr);
		Move(2);
	}

	if (character != NULL)
	{
		character->m_WalkStack.Clear();
		character->UpdateSex();
	}

	WORD X = ReadWord();
	obj->X = X & 0x7FFF;

	obj->Y = ReadWord();

	if (X & 0x8000)
		Move(1); //direction2 ?????

	obj->Z = ReadByte();

	BYTE dir = ReadByte();
	if (character != NULL)
		character->Direction = dir;
	else if (obj->IsCorpse())
	{
		if (World->FindWorldCorpseOwner(serial))
			obj->AnimIndex = 0;
		else
			obj->AnimIndex = 99;

		if (dir & 0x80)
		{
			dir &= 0x7F;
			item->UsedLayer = 1;
		}
		else
			item->UsedLayer = 0;

		item->Layer = dir;
	}

	obj->Color = ReadWord() & 0x7FFF;

	if (character == NULL)
	{
		graphic = item->Graphic;

		if (item->Graphic < 0x4000)
		{
			STATIC_TILES &st = UO->m_StaticData[graphic / 32].Tiles[graphic % 32];
			item->ObjectFlags = st.Flags;

			if (item->IsWearable())
			{
				item->ImageID = st.AnimID + 0xC350;
				item->AnimID = st.AnimID;
				UO->ExecuteGump(st.AnimID + 50000, (st.Flags & 0x00040000));
				UO->ExecuteGump(st.AnimID + 60000, (st.Flags & 0x00040000));
				item->UsedLayer = st.Quality;
			}

			if (!item->IsCorpse())
			{
				if (item->IsBackground())
					obj->RenderQueueIndex = 3 - (int)item->IsSurface();
				else if (item->IsSurface())
					obj->RenderQueueIndex = 4;
				else
					obj->RenderQueueIndex = 6;

				obj->IncRenderQueueIndex();
			}
			else
				obj->RenderQueueIndex = 7;

			UO->ExecuteStaticArt(item->Graphic);
		}
		else if (item->m_Items == NULL)
			item->LoadMulti();
	}

	obj->Flags = ReadByte();

	BYTE noto = ReadByte();
	if (character != NULL)
		character->Notoriety = noto;

	if (character != NULL)
		trace_printf("%d,%d,%d C%04X D%d F%02X N%d\n", obj->X, obj->Y, obj->Z, obj->Color, character->Direction, obj->Flags, character->Notoriety);
	else
		trace_printf("%d,%d,%d C%04X F%02X\n", obj->X, obj->Y, obj->Z, obj->Color, obj->Flags);

	serial = ReadDWord();

	World->MoveToTop(obj);

	PBYTE end = buf + size;

	if (*buf != 0x78)
		end -= 6;

	while (serial != 0)
	{
		if (Ptr >= end)
			break;

		TGameItem *obj2 = World->GetWorldItem(serial);

		obj2->MapIndex = g_CurrentMap;

		graphic = ReadWord();

		BYTE layer = ReadByte();

		if (graphic & 0x8000)
			obj2->Color = ReadWord();

		obj2->Graphic = graphic & 0x7FFF;

		trace_printf("\t0x%08X:%04X ", obj2->Serial, obj2->Graphic);

		STATIC_TILES &st1 = UO->m_StaticData[obj2->Graphic / 32].Tiles[obj2->Graphic % 32];

		obj2->ObjectFlags = st1.Flags;

		if (obj2->IsWearable())
		{
			obj2->ImageID = st1.AnimID + 0xC350;
			obj2->AnimID = st1.AnimID;
			UO->ExecuteGump(st1.AnimID + 50000, (st1.Flags & 0x00040000));
			UO->ExecuteGump(st1.AnimID + 60000, (st1.Flags & 0x00040000));
			obj2->UsedLayer = st1.Quality;
		}
		else if (layer == OL_MOUNT)
		{
			obj2->AnimID = st1.AnimID;
			obj2->UsedLayer = st1.Quality;
		}

		UO->ExecuteStaticArt(obj2->Graphic);

		World->PutEquipment(obj2, obj, layer);

		trace_printf("[%d] %04X\n", layer, obj2->Color);

		World->MoveToTop(obj2);

		serial = ReadDWord();
	}

	if (obj->NPC && !obj->Clicked && obj->Serial != g_PlayerSerial)
	{
		serial = obj->Serial;
		UO->StatusReq(serial);

		if (ConfigManager.ShowIncomingNames)
			UO->NameReq(serial);
	}
	
	TGump *gump = GumpManager->GetGump(obj->Serial, 0, GT_PAPERDOLL);
	if (gump != NULL)
		gump->UpdateFrame();

	gump = GumpManager->GetGump(obj->Serial, 0, GT_STATUSBAR);
	if (gump != NULL)
		gump->UpdateFrame();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(EquipItem)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	
	if (ObjectInHand != NULL && ObjectInHand->Serial == serial)
	{
		delete ObjectInHand;
		ObjectInHand = NULL;
	}

	TGameItem *obj = World->GetWorldItem(serial);
	obj->MapIndex = g_CurrentMap;
	obj->Graphic = ReadWord();
	Move(1);
	int layer = ReadByte();
	DWORD cserial = ReadDWord();
	obj->Color = ReadWord();

	int id = obj->Graphic;
	STATIC_TILES &st = UO->m_StaticData[id / 32].Tiles[id % 32];
	obj->ObjectFlags = st.Flags;

	if (obj->IsWearable())
	{
		obj->ImageID = st.AnimID + 0xC350;
		obj->AnimID = st.AnimID;
		UO->ExecuteGump(st.AnimID + 50000, (st.Flags & 0x00040000));
		UO->ExecuteGump(st.AnimID + 60000, (st.Flags & 0x00040000));
		obj->UsedLayer = st.Quality;
	}

	World->PutEquipment(obj, cserial, layer);

	if (layer < OL_MOUNT)
	{
		TGump *gump = GumpManager->GetGump(cserial, 0, GT_PAPERDOLL);
		if (gump != NULL)
			gump->UpdateFrame();
	}

	if (layer >= OL_BUY_RESTOCK && layer <= OL_SELL)
		obj->Clear();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdateContainedItem)
{
	if (World == NULL)
		return;

	TGameItem *obj = World->GetWorldItem(ReadDWord());
	if (obj == NULL)
		return;

	if (ObjectInHand != NULL && ObjectInHand->Serial == obj->Serial)
	{
		if (ObjectInHand->Separated)
			ObjectInHand->Separated = false;
		else
		{
			delete ObjectInHand;
			ObjectInHand = NULL;
		}
	}

	if (obj->Dragged)
		GumpManager->CloseGump(obj->Serial, 0, GT_DRAG);

	obj->Dragged = false;
	obj->MapIndex = g_CurrentMap;

	obj->Layer = OL_NONE;
	obj->Graphic = ReadWord();
	Move(1);
	obj->Count = ReadWord();
	obj->X = ReadWord();
	obj->Y = ReadWord();
	obj->Z = 0;

	if (m_ClientVersion >= CV_6017)
		Move(1);

	DWORD cserial = ReadDWord();
	World->PutContainer(obj, cserial);

	obj->Color = ReadWord();

	if (obj->Graphic == 0x0EB0) //Message board item
	{
		TPacketBulletinBoardRequestMessageSummary bbPacket(cserial, obj->Serial);
		bbPacket.Send();

		TGump *bbGump = GumpManager->GetGump(cserial, 0, GT_BULLETIN_BOARD);

		if (bbGump != NULL)
			bbGump->UpdateFrame();
	}
	else
	{
		int ID = obj->Graphic / 32;
		STATIC_TILES &st = UO->m_StaticData[ID].Tiles[obj->Graphic - (ID * 32)];
		obj->ObjectFlags = st.Flags;

		if (obj->IsWearable())
		{
			obj->ImageID = st.AnimID + 0xC350;
			obj->AnimID = st.AnimID;
			UO->ExecuteGump(st.AnimID + 50000, (st.Flags & 0x00040000));
			UO->ExecuteGump(st.AnimID + 60000, (st.Flags & 0x00040000));
			obj->UsedLayer = st.Quality;
		}

		if (obj->Color)
			UO->ExecuteColoredStaticArt(obj->Graphic, obj->Color);
		else
			UO->ExecuteStaticArt(obj->Graphic);
	}

	World->MoveToTop(obj);

	TGameItem *container = World->FindWorldItem(cserial);
	if (container != NULL)
	{
		if (container->Opened)
		{
			TGump *gump = GumpManager->GetGump(cserial, 0, GT_SPELLBOOK);
			if (gump != NULL)
				gump->UpdateFrame();
			else
			{
				gump = GumpManager->GetGump(cserial, 0, GT_CONTAINER);
				if (gump != NULL)
					gump->UpdateFrame();
			}
		}

		TGameObject *top = container->GetTopObject();

		if (top != NULL)
		{
			TGumpSecureTrading *tradeGump = (TGumpSecureTrading*)GumpManager->GetGump(top->Serial, 0, GT_TRADE);
			if (tradeGump != NULL)
				tradeGump->UpdateFrame();
		}
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdateContainedItems)
{
	if (World == NULL)
		return;

	WORD count = ReadWord();
	DWORD cupd = 0;
	TGameItem *contobj = NULL;
	bool isContGameBoard = false;
	bool bbUpdated = false;
	vector<CORPSE_EQUIPMENT_DATA> vced;
	bool containerIsCorpse = false;

	IFOR(i, 0, count)
	{
		DWORD serial = ReadDWord();
		WORD graphic = ReadWord();
		Move(1);
		WORD count = ReadWord();
		WORD x = ReadWord();
		WORD y = ReadWord();

		if (m_ClientVersion >= CV_6017)
			Move(1);

		DWORD cserial = ReadDWord();
		WORD color = ReadWord();

		contobj = World->GetWorldItem(cserial);

		contobj->MapIndex = g_CurrentMap;
		WORD contgraphic = contobj->Graphic;

		if (!cupd)
		{
			cupd = cserial;
			TGameObject *objA = World->FindWorldObject(cupd);
			trace_printf("Making %08X empty...\n", cupd);

			if (objA != NULL)
			{
				if (objA->IsCorpse())
				{
					containerIsCorpse = true;

					for (TGameItem *citem = (TGameItem*)objA->m_Items; citem != NULL; citem = (TGameItem*)citem->m_Next)
					{
						int lay = citem->Layer;

						if (lay > 0 && lay < OL_MOUNT)
						{
							CORPSE_EQUIPMENT_DATA ced = {citem->Serial, lay};
							vced.push_back(ced);
						}
					}
				}

				objA->Clear();

				if (contobj->Opened)
				{
					TGump *gameGump = GumpManager->GetGump(contobj->GetSerial(), 0, GT_CONTAINER);
					if (gameGump != NULL)
						isContGameBoard = ((TGumpContainer*)gameGump)->IsGameBoard;
				}
			}
		}

		trace_printf("\t|");
		TGameItem *obj = World->GetWorldItem(serial);

		//if (obj->GetDragged()) UO->CloseGump(obj->GetSerial(), 0, GT_DRAG);
		//obj->SetDragged(false);

		obj->MapIndex = g_CurrentMap;
		obj->Layer = 0;

		World->PutContainer(obj, cserial);

		obj->Graphic = graphic;
		obj->Count = count;
		obj->X = x;
		obj->Y = y;
		obj->Color = color;

		if (obj->Graphic == 0x0EB0) //Message board item
		{
			TPacketBulletinBoardRequestMessageSummary bbPacket(cserial, serial);
			bbPacket.Send();

			if (!bbUpdated)
			{
				bbUpdated = true;

				TGump *bbGump = GumpManager->GetGump(cserial, 0, GT_BULLETIN_BOARD);

				if (bbGump != NULL)
					bbGump->UpdateFrame();
			}
		}
		else
		{
			int ID = obj->Graphic / 32;
			STATIC_TILES &st = UO->m_StaticData[ID].Tiles[obj->Graphic - (ID * 32)];
			obj->ObjectFlags = st.Flags;

			if (obj->IsWearable())
			{
				obj->ImageID = st.AnimID + 0xC350;
				obj->AnimID = st.AnimID;
				UO->ExecuteGump(st.AnimID + 50000, (st.Flags & 0x00040000));
				UO->ExecuteGump(st.AnimID + 60000, (st.Flags & 0x00040000));
				obj->UsedLayer = st.Quality;
			}

			if (obj->Color)
				UO->ExecuteColoredStaticArt(obj->Graphic, obj->Color);
			else
				UO->ExecuteStaticArt(obj->Graphic);
		}

		trace_printf("0x%08X<0x%08X:%04X*%d (%d,%d) %04X\n", obj->Container, obj->Serial,
			obj->Graphic, obj->Count, obj->X, obj->Y, obj->Color);
	}

	if (containerIsCorpse)
	{
		IFOR(i, 0, (int)vced.size())
		{
			TGameItem *gi = World->FindWorldItem(vced[i].Serial);

			if (gi != NULL)
				World->PutEquipment(gi, cupd, vced[i].Layer);
		}

		vced.clear();
	}

	if (contobj != NULL)
	{
		if (contobj->Opened)
		{
			TGump *gump = GumpManager->GetGump(contobj->GetSerial(), 0, GT_SPELLBOOK);
			if (gump != NULL)
				gump->UpdateFrame();
			else
			{
				gump = GumpManager->GetGump(contobj->GetSerial(), 0, GT_CONTAINER);
				if (gump != NULL)
					gump->UpdateFrame();
			}
		}

		TGameObject *top = contobj->GetTopObject();

		if (top != NULL)
		{
			TGumpSecureTrading *tradeGump = (TGumpSecureTrading*)GumpManager->GetGump(top->Serial, 0, GT_TRADE);
			if (tradeGump != NULL)
				tradeGump->UpdateFrame();
		}
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(DenyMoveItem)
{
	if (World == NULL)
		return;

	if (ObjectInHand != NULL)
	{
		TGameItem *obj = World->FindWorldItem(ObjectInHand->Serial);

		if (obj == NULL)
		{
			obj = World->GetWorldItem(ObjectInHand->Serial);
			if (obj != NULL)
			{
				obj->Paste(ObjectInHand);
				World->PutContainer(obj, ObjectInHand->Container);

				World->MoveToTop(obj);
			}
		}

		delete ObjectInHand;
		ObjectInHand = NULL;
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(DeleteObject)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	TGameObject *obj = World->FindWorldObject(serial);

	if (ObjectInHand != NULL && ObjectInHand->Serial == serial)
	{
		bool sep = ObjectInHand->Separated;

		if (sep)
			ObjectInHand->Separated = false;
		else if (ObjectInHand->Deleted)
		{
			delete ObjectInHand;
			ObjectInHand = NULL;
		}
		
		if (ObjectInHand != NULL)
		{
			if (obj != NULL && sep)
			{
				if (obj->Count != ObjectInHand->Count)
					ObjectInHand->Deleted = true;
				else
					ObjectInHand->Separated = true;
			}
			else
				ObjectInHand->Deleted = true;
		}
	}

	if (obj != NULL && obj->Serial != g_PlayerSerial)
	{
		DWORD cont = obj->Container;

		if (cont != 0)
		{
			TGameObject *top = obj->GetTopObject();

			if (top != NULL)
			{
				TGumpSecureTrading *tradeGump = (TGumpSecureTrading*)GumpManager->GetGump(top->Serial, 0, GT_TRADE);
				if (tradeGump != NULL)
					tradeGump->UpdateFrame();
			}

			TGump *gump = GumpManager->GetGump(cont, 0, GT_CONTAINER);
			if (gump != NULL)
				gump->UpdateFrame();
		
			if (obj->Graphic == 0x0EB0)
			{
				GumpManager->CloseGump(serial, cont, GT_BULLETIN_BOARD_ITEM);
				
				gump = GumpManager->GetGump(cont, 0, GT_BULLETIN_BOARD);
				if (gump != NULL)
				{
					for (TGumpObject *go = (TGumpObject*)gump->m_Items; go != NULL; go = (TGumpObject*)go->m_Next)
					{
						if (go->Serial == serial)
						{
							gump->Delete(go);
							break;
						}
					}

					gump->UpdateFrame();
				}
			}
		}

		if (obj->NPC && Party.Contains(obj->Serial))
			obj->RemoveRender();
		else
			World->RemoveObject(obj);
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdateCharacter)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	TGameCharacter *obj = World->GetWorldCharacter(serial);

	if (obj == NULL)
		return;

	obj->MapIndex = g_CurrentMap;
	obj->Graphic = ReadWord();
	obj->UpdateSex();
	
	WORD x = ReadWord();
	WORD y = ReadWord();
	char z = ReadChar();
	BYTE dir = ReadByte();

	if (serial != g_PlayerSerial && !obj->IsTeleportAction(x, y, dir))
	{
		TWalkData *wd = new TWalkData();
		wd->X = x;
		wd->Y = y;
		wd->Z = z;
		wd->Direction = dir;

		if (obj->m_WalkStack.Empty())
			obj->LastStepTime = GetTickCount();

		obj->m_WalkStack.Push(wd);
		//obj->GetAnimationGroup();
	}
	else
	{
		obj->X = x;
		obj->Y = y;
		obj->Z = z;
		obj->Direction = dir;
	}

	obj->Color = ReadWord() & 0x7FFF;
	obj->Flags = ReadByte();
	
	TGump *gump = GumpManager->GetGump(serial, 0, GT_STATUSBAR);
	if (gump != NULL)
		gump->UpdateFrame();
	
	World->MoveToTop(obj);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(Warmode)
{
	if (World == NULL)
		return;

	g_Player->Warmode = ReadByte();
	
	TGump *gump = GumpManager->GetGump(g_PlayerSerial, 0, GT_STATUSBAR);
	if (gump != NULL)
		gump->UpdateFrame();

	gump = GumpManager->GetGump(g_PlayerSerial, 0, GT_PAPERDOLL);
	if (gump != NULL)
		gump->UpdateFrame();

	World->MoveToTop(g_Player);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(PauseControl)
{
	/*g_ClientPaused = ReadByte();

	if (!g_ClientPaused)
		UO->ResumeClient();*/
}
//---------------------------------------------------------------------------
PACKET_HANDLER(OpenPaperdoll)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();

	TGameCharacter *obj = World->FindWorldCharacter(serial);
	if (obj != NULL)
		obj->SetPaperdollText(ReadString(0));

	TGump *gump = GumpManager->GetGump(serial, 0, GT_PAPERDOLL);
	if (gump != NULL)
		gump->UpdateFrame();
	else
	{
		TGumpPaperdoll *gump = new TGumpPaperdoll(serial, 0, 0, false);
		GumpManager->AddGump(gump);
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(ClientVersion)
{
	TPacketClientVersion packet(g_ClientVersionText);
	packet.Send();
	packet.Free();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(Ping)
{
	g_PingSequence = ReadByte();

	if (g_PingCount)
		g_PingCount--;
	else
		UO->Send(buf, 2);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(SetWeather)
{
	Weather.Reset();
	BYTE type = ReadByte();
	Weather.Type = type;
	Weather.SetCount(ReadByte());

	if (Weather.GetCount() > 70)
		Weather.SetCount(70);

	Weather.SetTemperature(ReadByte());
	Weather.SetTimer(GetTickCount() + WEATHER_TIMER);
	Weather.Generate();

	switch (type)
	{
		case 0:
		{
			UO->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "It begins to rain.");
			break;
		}
		case 1:
		{
			UO->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "A fierce storm approaches.");
			break;
		}
		case 2:
		{
			UO->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "It begins to snow.");
			break;
		}
		case 3:
		{
			UO->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "A storm is brewing.");
			break;
		}
		case 0xFE:
		case 0xFF:
		{
			Weather.SetTimer(0);
			break;
		}
		default:
			break;
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(PersonalLightLevel)
{
	DWORD serial = ReadDWord();

	if (serial == g_PlayerSerial)
	{
		BYTE level = ReadByte();
		if (level > 0x1F)
			level = 0x1F;

		g_PersonalLightLevel = level;
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(LightLevel)
{
	BYTE level = ReadByte();
	if (level > 0x1F)
		level = 0x1F;

	g_LightLevel = level;
}
//---------------------------------------------------------------------------
PACKET_HANDLER(EnableLockedFeatures)
{
	if (m_ClientVersion >= CV_60142)
		g_LockedClientFeatures = ReadDWord();
	else
		g_LockedClientFeatures = ReadWord();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(OpenContainer)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	WORD gumpid = ReadWord();

	TGump *gump = NULL;

	if (gumpid == 0xFFFF) //Spellbook
	{
		int GameWindowCenterX = (g_GameWindowPosX - 4) + g_GameWindowSizeX / 2;
		int GameWindowCenterY = (g_GameWindowPosY - 4) + g_GameWindowSizeY / 2;

		int x = GameWindowCenterX - 200;
		int y = GameWindowCenterY - 100;

		if (x < 0)
			x = 0;

		if (y < 0)
			y = 0;

		gump = new TGumpSpellbook(serial, x, y);
	}
	else if (gumpid == 0x0030) //Buylist
	{
		GumpManager->CloseGump(0, 0, GT_SHOP);

		gump = new TGumpBuy(serial, 150, 5);
	}
	else //Container
	{
		WORD graphic = 0xFFFF;

		IFOR(i, 0, CONTAINERS_COUNT)
		{
			if (gumpid == g_ContainerOffset[i].Gump)
			{
				graphic = i;
				break;
			}
		}

		if (graphic == 0xFFFF)
			return;

		ContainerRect.Calculate(graphic);

		gump = new TGumpContainer(serial, ContainerRect.X, ContainerRect.Y);
		gump->Graphic = graphic;
		((TGumpContainer*)gump)->IsGameBoard = (gumpid == 0x091A || gumpid == 0x092E);
		UO->ExecuteGump(gumpid, 0);
	}

	if (gump == NULL)
		return;

	gump->ID = gumpid;
	
	if (gumpid != 0x0030)
	{
		if (ContainerStack != NULL)
		{
			TContainerStack *cont = ContainerStack;

			while (cont != NULL)
			{
				if (cont->Serial == serial)
				{
					gump->X = cont->X;
					gump->Y = cont->Y;
					gump->Minimized = cont->Minimized;
					gump->MinimizedX = cont->MinimizedX;
					gump->MinimizedY = cont->MinimizedY;
					gump->LockMoving = cont->LockMoving;

					if (cont == ContainerStack)
					{
						ContainerStack = ContainerStack->m_Next;

						if (ContainerStack != NULL)
							ContainerStack->m_Prev = NULL;
					}
					else
					{
						cont->m_Prev->m_Next = cont->m_Next;

						if (cont->m_Next != NULL)
							cont->m_Next->m_Prev = cont->m_Prev;
					}

					cont->m_Next = NULL;
					cont->m_Prev = NULL;

					delete cont;

					break;
				}

				cont = cont->m_Next;
			}

			if (g_CheckContainerStackTimer < GetTickCount() && ContainerStack != NULL)
			{
				delete ContainerStack;
				ContainerStack = NULL;
			}
		}

		TGameItem *obj = World->FindWorldItem(serial);
		if (obj != NULL)
		{
			/*if (gumpid != 0xFFFF)*/ obj->Opened = true;
			if (!obj->IsCorpse())
				World->ClearContainer(obj);
		}
	}

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UpdateSkills)
{
	if (World == NULL)
		return;

	int type = ReadByte();
	bool HaveCap = (type == 0x02 || type == 0xDF);
	bool IsSingleUpdate = (type == 0xFF || type == 0xDF);
	trace_printf("Skill update type %i (Cap=%d)\n", type, HaveCap);

	PBYTE end = buf + size;

	while (Ptr < end)
	{
		WORD id = ReadWord();

		if (!id && !type)
			break;
		else if (!type || type == 0x02)
			id--;

		WORD BaseVal = ReadWord();
		WORD RealVal = ReadWord();
		BYTE lock = ReadByte();
		WORD Cap = 0;

		if (HaveCap)
			Cap = ReadWord();

		if (id < g_SkillsCount)
		{
			if (IsSingleUpdate)
			{
				float change = (float)(BaseVal / 10.0f) - g_Player->GetSkillBaseValue(id);

				if (change)
				{
					char str[128] = {0};
					sprintf(str, "Your skill in %s has %s by %.1f%%.  It is now %.1f%%.", g_Skills[id].m_Name.c_str(), ((change < 0) ? "decreased" : "increased"), change, g_Player->GetSkillBaseValue(id) + change);
					//else if (change > 0) sprintf(str, "Your skill in %s has increased by %.1f%%.  It is now %.1f%%.", UO->m_Skills[id].m_Name.c_str(), change, obj->GetSkillBaseValue(id) + change);
					UO->CreateTextMessage(TT_SYSTEM, 0, 3, 0x58, str);
				}
			}

			g_Player->SetSkillBaseValue(id, (float)(BaseVal / 10.0f));
			g_Player->SetSkillValue(id, (float)(RealVal / 10.0f));
			g_Player->SetSkillCap(id, (float)(Cap / 10.0f));
			g_Player->SetSkillStatus(id, lock);

			if (HaveCap)
				trace_printf("Skill %i is %i|%i|%i\n", id, BaseVal, RealVal, Cap);
			else
				trace_printf("Skill %i is %i|%i\n", id, BaseVal, RealVal);
		}
		else
			trace_printf("Unknown skill update %d\n", id);
	}

	g_SkillsTotal = 0.0f;
	IFOR(i, 0, g_SkillsCount)
		g_SkillsTotal += g_Player->GetSkillValue(i);

	TGump *gump = GumpManager->GetGump(g_PlayerSerial, 0, GT_SKILLS);
	if (gump != NULL)
		gump->UpdateFrame();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(ExtendedCommand)
{
	WORD cmd = ReadWord();

	switch (cmd)
	{
		case 0: break;
		case 1: //Initialize Fast Walk Prevention
		{
			IFOR(i, 0, 6)
				Walker->m_FastWalkStack.Push(ReadDWord());

			break;
		}
		case 2: //Add key to Fast Walk Stack
		{
			Walker->m_FastWalkStack.Push(ReadDWord());

			break;
		}
		case 4: //Close generic gump
		{
			DWORD Serial = ReadDWord();
			DWORD ID = ReadDWord();
			GumpManager->CloseGump(Serial, ID, GT_GENERIC);

			break;
		}
		case 5: //Screen size
		{
			//g_GameWindowSizeX = unpack16(buf + 5);
			//g_GameWindowSizeY = unpack16(buf + 9);
			break;
		}
		case 6: //Party commands
		{
			Party.ParsePacketData(buf + 5, size);

			break;
		}
		case 8: //Set cursor / map
		{
			UO->ChangeMap(ReadByte());

			break;
		}
		case 0xC: //Close statusbar gump
		{
			DWORD Serial = ReadDWord();
			GumpManager->CloseGump(Serial, 0, GT_STATUSBAR);

			break;
		}
		case 0x16: ////Close User Interface Windows
		{
			//ID:
			//0x01: Paperdoll
			//0x02: Status
			//0x08: Character Profile
			//0x0C: Container
			DWORD ID = ReadDWord();
			DWORD Serial = ReadDWord();

			switch (ID)
			{
				case 1: //Paperdoll
				{
					GumpManager->CloseGump(Serial, 0, GT_PAPERDOLL);
					break;
				}
				case 2: //Statusbar
				{
					GumpManager->CloseGump(Serial, 0, GT_STATUSBAR);
					break;
				}
				case 8: //Character Profile
				{
					//UO->CloseGump(Serial, 0, GT_PROFILE);
					break;
				}
				case 0xC: //Container
				{
					GumpManager->CloseGump(Serial, 0, GT_CONTAINER);
					break;
				}
				default:
					break;
			}

			break;
		}
		default:
			break;
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(DenyWalk)
{
	g_WalkRequestCount = 0;

	if (g_Player == NULL)
		return;

	Move(1);
	g_Player->X = ReadWord();
	g_Player->Y = ReadWord();
	BYTE dir = ReadByte();
	g_Player->Direction = dir;
	g_Player->Z = ReadByte();

	Walker->SetSequence(0, dir);
	g_Player->OffsetX = 0;
	g_Player->OffsetY = 0;
	g_Player->OffsetZ = 0;

	g_Player->m_WalkStack.Clear();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(ConfirmWalk)
{
	if (g_WalkRequestCount)
		g_WalkRequestCount--;

	if (g_Player == NULL)
		return;

	BYTE Seq = ReadByte();
	//player->SetDirection(newdir);

	BYTE newnoto = ReadByte();

	if (newnoto >= 0x40)
		newnoto ^= 0x40;

	if (!newnoto || newnoto >= 7)
		newnoto = 0x01;

	g_Player->Notoriety = newnoto;

	World->MoveToTop(g_Player);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(OpenUrl)
{
	UO->GoToWebLink(ReadString(0));
}
//---------------------------------------------------------------------------
PACKET_HANDLER(Target)
{
	Target.SetData(buf, size);

	if (g_PartyHelperTimer > GetTickCount() && g_PartyHelperTarget)
	{
		Target.SendTargetObject(g_PartyHelperTarget);
		g_PartyHelperTimer = 0;
		g_PartyHelperTarget = 0;
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(Talk)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	WORD graphic = ReadWord();
	SPEECH_TYPE type = (SPEECH_TYPE)ReadByte();
	WORD text_color = ReadWord();
	WORD font = ReadWord();

	if (!serial && font == 0xFFFF && text_color == 0xFFFF)
	{
		BYTE sbuffer[0x28] =
		{
			0x03, 0x00, 0x28, 0x20, 0x00, 0x34, 0x00, 0x03, 0xdb, 0x13, 0x14, 0x3f, 0x45, 0x2c, 0x58, 0x0f,
			0x5d, 0x44, 0x2e, 0x50, 0x11, 0xdf, 0x75, 0x5c, 0xe0, 0x3e, 0x71, 0x4f, 0x31, 0x34, 0x05, 0x4e,
			0x18, 0x1e, 0x72, 0x0f, 0x59, 0xad, 0xf5, 0x00
		};

		UO->Send(sbuffer, 0x28);

		return;
	}
	
	string name(ReadString(0));
	string str = "";

	if (size > 44)
	{
		Ptr = buf + 44;
		str = ReadString(0);
	}

	if (type == ST_BROADCAST || /*type == ST_SYSTEM ||*/ serial == 0xFFFFFFFF || !serial || name == string("System"))
		UO->CreateTextMessage(TT_SYSTEM, serial, font, text_color, str);
	else
	{
		if (type == ST_EMOTE)
		{
			text_color = ConfigManager.EmoteColor;
			str = "*" + str + "*";
		}

		UO->CreateTextMessage(TT_OBJECT, serial, font, text_color, str);

		//if (serial >= 0x40000000) //������ ��� ���������
		{
			TGameObject *obj = World->FindWorldObject(serial);

			if (obj != NULL && !obj->GetName().length())
			{
				obj->SetName(name);

				if (obj->NPC)
				{
					TGump *gump = GumpManager->GetGump(serial, 0, GT_STATUSBAR);

					if (gump != NULL)
						gump->UpdateFrame();
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UnicodeTalk)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	WORD graphic = ReadWord();
	SPEECH_TYPE type = (SPEECH_TYPE)ReadByte();
	WORD text_color = ReadWord();
	WORD font = ReadWord();
	DWORD language = ReadDWord();

	if (!serial && font == 0xFFFF && text_color == 0xFFFF)
	{
		BYTE sbuffer[0x28] =
		{
			0x03, 0x00, 0x28, 0x20, 0x00, 0x34, 0x00, 0x03, 0xdb, 0x13, 0x14, 0x3f, 0x45, 0x2c, 0x58, 0x0f,
			0x5d, 0x44, 0x2e, 0x50, 0x11, 0xdf, 0x75, 0x5c, 0xe0, 0x3e, 0x71, 0x4f, 0x31, 0x34, 0x05, 0x4e,
			0x18, 0x1e, 0x72, 0x0f, 0x59, 0xad, 0xf5, 0x00
		};

		UO->Send(sbuffer, 0x28);

		return;
	}
	
	wstring name((wchar_t*)Ptr);
	wstring str = L"";

	if (size > 48)
	{
		Ptr = buf + 48;
		str = ReadUnicodeString((size - 48) / 2);
	}
	
	if (type == ST_BROADCAST || type == ST_SYSTEM || serial == 0xFFFFFFFF || !serial || name == wstring(L"System"))
		UO->CreateUnicodeTextMessage(TT_SYSTEM, serial, ConfigManager.SpeechFont, text_color, str);
	else
	{
		if (type == ST_EMOTE)
		{
			text_color = ConfigManager.EmoteColor;
			str = L"*" + str + L"*";
		}
		
		UO->CreateUnicodeTextMessage(TT_OBJECT, serial, ConfigManager.SpeechFont, text_color, str);
		
		//if (serial >= 0x40000000) //������ ��� ���������
		{
			TGameObject *obj = World->FindWorldObject(serial);

			if (obj != NULL && !obj->GetName().length())
			{
				obj->SetName(ToString(name));
				
				if (obj->NPC)
				{
					TGump *gump = GumpManager->GetGump(serial, 0, GT_STATUSBAR);

					if (gump != NULL)
						gump->UpdateFrame();
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(OpenMenuGump)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	DWORD id = ReadWord();

	BYTE nameLen = ReadByte();
	string name = ReadString(nameLen);

	BYTE count = ReadByte();

	TGump *gump = NULL;

	if (unpack16(Ptr)) //menu
	{
		gump = new TGumpMenu(serial, 0, 0, name);
		((TGumpMenu*)gump)->Count = count;
	}
	else //gray menu
	{
		int x = (g_ClientWidth / 2) - 200;
		int y = (g_ClientHeight / 2) - ((121 + (count * 21)) / 2);

		gump = new TGumpGrayMenu(serial, x, y, name);
		((TGumpGrayMenu*)gump)->Count = count;
	}

	gump->ID = id;

	IFOR(i, 0, count)
	{
		WORD graphic = ReadWord();
		UO->ExecuteStaticArt(graphic);

		WORD color = ReadWord();
		if (color)
			color++;

		nameLen = ReadByte();
		name = ReadString(nameLen);

		TGumpObject *go = new TGumpMenuObject(graphic, color, name);
		gump->Add(go);
	}

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(SecureTrading)
{
	if (World == NULL)
		return;

	BYTE type = ReadByte();
	DWORD serial = ReadDWord();

	if (type == 0) //����� ����� ����
	{
		DWORD id1 = ReadDWord();
		DWORD id2 = ReadDWord();
		BYTE hasName = ReadByte();

		TGumpSecureTrading *gump = new TGumpSecureTrading(serial, 0, 0, id1, id2);

		if (hasName && *Ptr)
			gump->SetText(ReadString(0));

		GumpManager->AddGump(gump);
	}
	else if (type == 1) //������
		GumpManager->CloseGump(serial, 0, GT_TRADE);
	else if (type == 2) //����������
	{
		TGumpSecureTrading *gump = (TGumpSecureTrading*)GumpManager->GetGump(serial, 0, GT_TRADE);
		if (gump != NULL)
		{
			DWORD id1 = ReadDWord();
			DWORD id2 = ReadDWord();

			gump->StateMy = id1 != 0;
			gump->StateOpponent = id2 != 0;

			gump->UpdateFrame();
		}
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(TextEntryDialog)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	BYTE parentID = ReadByte();
	BYTE buttonID = ReadByte();

	short textLen = ReadShort();
	string text = ReadString(textLen);

	bool haveCancel = !ReadByte();
	BYTE variant = ReadByte();
	int maxLength = ReadDWord();
	
	short descLen = ReadShort();
	string desc = ReadString(descLen);

	TGumpTextEntryDialog *gump = new TGumpTextEntryDialog(serial, 143, 172, variant, maxLength, text, desc);
	gump->NoClose = haveCancel;
	gump->ParentID = parentID;
	gump->ButtonID = buttonID;

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(OpenGump)
{
	if (World == NULL)
		return;

	//TPRINT("Gump dump::\n");
	//TDUMP(buf, size);

	DWORD serial = ReadDWord();
	DWORD id = ReadDWord();
	int x = ReadInt();
	int y = ReadInt();

	TGumpGeneric *gump = new TGumpGeneric(serial, x, y, id);

	PBYTE p = buf + 21;
	PBYTE e = p;

	int commandsLen = unpack16(buf + 19);
	PBYTE end = buf + 21 + commandsLen;

	while (p < end)
	{
		while (p < end && *p && *p != '{')
			p++;

		e = p + 1;

		while (e < end && *e && *e == ' ')
			e++;

		char lowc[20] = {0};

		int eLen = strlen((char*)e);
		eLen = (eLen > 19 ? 20 : eLen);
		memcpy(&lowc[0], &e[0], eLen);
		_strlwr(lowc);

		TGumpObject *go = NULL;

		//trace_printf("\tlwr.token=%s\n", lowc);
		if (!memcmp(lowc, "nodispose", 9))
			e += 10;
		else if (!memcmp(lowc, "nomove", 6))
		{
			gump->NoMove = true;
			e += 7;
		}
		else if (!memcmp(lowc, "noclose", 7))
		{
			gump->NoClose = true;
			e += 7;
		}
		else if (!memcmp(lowc, "page", 4))
		{
			e += 5;
			int page = 0;
			sscanf((char*)e, "%d", &page);

			go = new TGumpPage(page);
		}
		else if (!memcmp(lowc, "group", 5))
		{
			e += 6;
			int group = 0;
			sscanf((char*)e, "%d", &group);

			go = new TGumpGroup(group);
		}
		else if (!memcmp(lowc, "endgroup", 8))
		{
			e += 9;

			go = new TGumpEndGroup();
		}
		else if (!memcmp(lowc, "resizepic", 9))
		{
			e += 10;
			int x = 0, y = 0, w = 0, h = 0, graphic = 0;
			sscanf((char*)e, "%d %d %d %d %d", &x, &y, &graphic, &w, &h);

			UO->ExecuteResizepic(graphic);

			go = new TGumpResizepic(graphic, x, y, w, h);
		}
		else if (!memcmp(lowc, "checkertrans", 12))
		{
			e += 13;
			int x = 0, y = 0, w = 0, h = 0;
			sscanf((char*)e, "%d %d %d %d", &x, &y, &w, &h);

			go = new TGumpChecktrans(x, y, w, h);
			//gump->SetTransparent(true);
		}
		else if (!memcmp(lowc, "buttontileart", 13))
		{
			e += 14;
			int x = 0, y = 0, action = 0, topage = 0, number = 0, up = 0, down = 0, tid = 0, tcolor = 0, tx = 0, ty = 0;
			sscanf((char*)e, "%d %d %d %d %d %d %d %d %d %d %d", &x, &y, &up, &down, &action, &topage, &number, &tid, &tcolor, &tx, &ty);

			UO->ExecuteButton(up);
			UO->ExecuteColoredStaticArt(tid, tcolor);

			go = new TGumpButtonTileArt(up, up + 1, down, number, topage, action != 0, x, y, tid, tcolor,tx, ty);
		}
		else if (!memcmp(lowc, "button", 6))
		{
			e += 7;
			int x = 0, y = 0, action = 0, topage = 0, number = 0, up = 0, down = 0;
			sscanf((char*)e, "%d %d %d %d %d %d %d", &x, &y, &up, &down, &action, &topage, &number);

			UO->ExecuteButton(up);

			go = new TGumpButton(up, up + 1, down, number, topage, action != 0, x, y);
		}
		else if (!memcmp(lowc, "checkbox", 8))
		{
			e += 9;
			int x = 0, y = 0, state = 0, number = 0, up = 0, down = 0;
			sscanf((char*)e, "%d %d %d %d %d %d", &x, &y, &up, &down, &state, &number);

			UO->ExecuteGump(up, 0);
			UO->ExecuteGump(down, 0);

			go = new TGumpCheckbox(up, down, up + 1, number, state != 0, x, y);
		}
		else if (!memcmp(lowc, "radio", 5))
		{
			e += 6;
			int x = 0, y = 0, state = 0, number = 0, up = 0, down = 0;
			sscanf((char*)e, "%d %d %d %d %d %d", &x, &y, &up, &down, &state, &number);

			UO->ExecuteGump(up, 0);
			UO->ExecuteGump(down, 0);

			go = new TGumpRadio(up, down, up + 1, number, state != 0, x, y);
		}
		else if (!memcmp(lowc, "croppedtext", 11))
		{
			e +=12;
			int x = 0, y = 0, w = 0, h = 0, number = 0, color = 0;
			sscanf((char*)e, "%d %d %d %d %d %d", &x, &y, &w, &h, &color, &number);

			go = new TGumpCroppedText(number, color, x, y, w, h);
		}
		else if (!memcmp(lowc, "textentrylimited", 16))
		{
			e += 17;
			int x = 0, y = 0, w = 0, h = 0, index = 0, number = 0, length = 0, color = 0;
			sscanf((char*)e, "%d %d %d %d %d %d %d %d", &x, &y, &w, &h, &color, &index, &number, &length);

			go = new TGumpTextEntryLimited(number, color, x, y, w, h, index, length);
		}
		else if (!memcmp(lowc, "textentry", 9))
		{
			e += 10;
			int x = 0, y = 0, w = 0, h = 0, index = 0, number = 0, color = 0;
			sscanf((char*)e, "%d %d %d %d %d %d %d", &x, &y, &w, &h, &color, &index, &number);

			go = new TGumpTextEntry(number, color, x, y, w, h, index);
		}
		else if (!memcmp(lowc, "text", 4))
		{
			e += 5;
			int x = 0, y = 0, n = 0, color = 0;
			sscanf((char*)e, "%d %d %d %d", &x, &y, &color, &n);

			go = new TGumpText(n, color, x, y);
		}
		else if (!memcmp(lowc, "tilepichue", 10))
		{
			e += 11;
			int x = 0, y = 0, graphic = 0, color = 0;
			sscanf((char*)e, "%d %d %d %d", &x, &y, &graphic, &color);

			UO->ExecuteStaticArt(graphic);

			go = new TGumpTilepicHue(graphic, color, x, y);
		}
		else if (!memcmp(lowc, "tilepic", 7))
		{
			e += 8;
			int x = 0, y = 0, graphic = 0;
			sscanf((char*)e, "%d %d %d", &x, &y, &graphic);

			UO->ExecuteStaticArt(graphic);

			go = new TGumpTilepic(graphic, x, y);
		}
		else if (!memcmp(lowc, "gumppictiled", 12))
		{
			e += 13;
			int x = 0, y = 0, w = 0, h = 0, graphic = 0;
			sscanf((char*)e, "%d %d %d %d %d", &x, &y, &w, &h, &graphic);

			UO->ExecuteGump(graphic, 0);

			go = new TGumpGumppicTiled(graphic, x, y, w, h);
		}
		else if (!memcmp(lowc, "gumppic", 7))
		{
			e += 8;
			int x = 0, y = 0, graphic = 0, color = 0;
			sscanf((char*)e, "%d %d %d", &x, &y, &graphic);

			UO->ExecuteGump(graphic, 0);

			char bufptr[20] = {0};
			sprintf(bufptr, "%d %d %d", x, y, graphic);

			int curlen = strlen(bufptr);
			while (e + curlen < end && e[curlen] == ' ')
				curlen++;

			if (e[curlen] != '}')
				sscanf((char*)(e + strlen(bufptr) + 5), "%d", &color);
			
			go = new TGumpGumppic(graphic, color, x, y);
		}
		else if (!memcmp(lowc, "xfmhtmlgump", 11))
		{
			e += 12;
			int x = 0, y = 0, w = 0, h = 0, background = 0, scrollbar = 0, clilocID = 0;
			sscanf((char*)e, "%d %d %d %d %d %d %d", &x, &y, &w, &h, &clilocID, &background, &scrollbar);

			go = new TGumpXFMHTMLGump(clilocID, 0, x, y, w, h, background, scrollbar);
		}
		else if (!memcmp(lowc, "xfmhtmlgumpcolor", 16))
		{
			e += 17;
			int x = 0, y = 0, w = 0, h = 0, background = 0, scrollbar = 0, clilocID = 0, color = 0;
			sscanf((char*)e, "%d %d %d %d %d %d %d %d", &x, &y, &w, &h, &clilocID, &background, &scrollbar, &color);

			go = new TGumpXFMHTMLGump(clilocID, color, x, y, w, h, background, scrollbar);
		}
		else if (!memcmp(lowc, "htmlgump", 8))
		{
			e += 9;
			int x = 0, y = 0, w = 0, h = 0, background = 0, scrollbar = 0, textID = 0;
			sscanf((char*)e, "%d %d %d %d %d %d %d", &x, &y, &w, &h, &textID, &background, &scrollbar);

			go = new TGumpHTMLGump(textID, x, y, w, h, background, scrollbar);
		}
		/*else if (!memcmp(lowc, "xfmhtmltok", 10))
		{
			e += 11;
			int x = 0, y = 0, w = 0, h = 0, background = 0, scrollbar = 0, clilocID = 0;
			sscanf((char*)e, "%d %d %d %d %d %d", &x, &y, &w, &h, &background, &scrollbar, &clilocID);

			go = new TGumpXFMHTMLToken(clilocID, x, y, w, h, background, scrollbar);
		}*/
		else if (!memcmp(lowc, "tooltip", 7))
		{
			e += 8;
			int clilocID = 0;
			sscanf((char*)e, "%d", &clilocID);

			go = new TGumpTooltip(clilocID);
		}
		else if (!memcmp(lowc, "mastergump", 10))
		{
			e += 11;
			int index = 0;
			sscanf((char*)e, "%d", &index);

			go = new TGumpMasterGump(index);
		}

		if (go != NULL)
			gump->Add(go);

		while (e < end && *e && *e != '}')
			e++;

		p = e + 1;
	}
	
	Ptr = buf + 21 + commandsLen;

	short textLinesCount = ReadShort();
	
	IFOR(i, 0, textLinesCount)
	{
		int linelen = ReadShort();

		if (linelen)
			gump->AddText(i, ReadUnicodeString(linelen));
	}
	
	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(OpenCompressedGump)
{
	if (World == NULL)
		return;

	DWORD senderID = ReadDWord();
	DWORD gumpID = ReadDWord();
	DWORD x = ReadDWord();
	DWORD y = ReadDWord();
	DWORD cLen = ReadDWord() - 4; //Compressed Length (4 == sizeof(DecompressedLen)
	DWORD dLen = ReadDWord(); //Decompressed Length

	if (cLen < 1)
	{
		trace_printf("CLen=%d\n", cLen);
		TPRINT("Server Sends bad Compressed Gumpdata!\n");

		return;
	}
	if ((int)(28 + cLen) > size)
	{
		TPRINT("Server Sends bad Compressed Gumpdata!\n");

		return;
	}

	// Layout data.....
	PBYTE decLayoutData = new BYTE[dLen];
	trace_printf("Gump layout:\n\tSenderID=0x%08X\n\tGumpID=0x%08X\n\tCLen=%d\n\tDLen=%d\n", senderID, gumpID, cLen, dLen);

	TPRINT("Decompressing layout gump data...\n");

	int z_err = uncompress(decLayoutData, &dLen, Ptr, cLen);

	if (z_err != Z_OK)
	{
		delete decLayoutData;
		trace_printf("Decompress layout gump error %d\n", z_err);

		return;
	}

	TPRINT("Layout gump data decompressed!\n");
	//trace_dump(DecLayoutData,DLen);
	// Text data.....

	Move(cLen);

	DWORD linesCount = ReadDWord(); //Text lines count
	DWORD cTLen = 0;
	DWORD dTLen = 0;
	PBYTE gumpDecText = NULL;

	if (linesCount > 0)
	{
		cTLen = ReadDWord(); //Compressed lines length
		dTLen = ReadDWord(); //Decompressed lines length

		gumpDecText = new BYTE[dTLen];

		TPRINT("Decompressing text gump data...\n");

		z_err = uncompress(gumpDecText, &dTLen, Ptr, cTLen);

		if (z_err != Z_OK)
		{
			delete decLayoutData;
			delete gumpDecText;
			trace_printf("Decompress text gump error %d\n", z_err);

			return;
		}

		TPRINT("Text gump data decompressed!\n");
		trace_printf("Gump text lines:\n\tLinesCount=%d\n\tCTLen=%d\n\tDTLen=%d\n", linesCount, cTLen, dTLen);
		//trace_dump(GumpDecText,DTLen);
	}

	int newsize = 21 + dLen + 2 + dTLen;

	PBYTE newbuf = new BYTE[newsize];
	newbuf[0] = 0xb0;
	pack16(newbuf + 1, newsize);
	pack32(newbuf + 3, senderID);
	pack32(newbuf + 7, gumpID);
	pack32(newbuf + 11, x);
	pack32(newbuf + 15, y);
	pack16(newbuf + 19, (WORD)dLen);
	memcpy(newbuf + 21, decLayoutData, dLen);
	pack16(newbuf + 21 + dLen, (WORD)linesCount);

	if (linesCount > 0)
		memcpy(newbuf + 23 + dLen, gumpDecText, dTLen);
	else
		newbuf[newsize - 1] = 0x00;

	trace_printf("Gump decompressed! newsize=%d\n", newsize);
	//trace_dump(newbuf,newsize);

	Ptr = newbuf + 3;
	HandleOpenGump(newbuf, newsize);

	delete decLayoutData;

	if (linesCount > 0)
		delete gumpDecText;

	delete newbuf;
}
//---------------------------------------------------------------------------
PACKET_HANDLER(TipWindow)
{
	BYTE flag = ReadByte();

	if (flag != 1) //1 - ignore
	{
		DWORD serial = ReadDWord();
		short len = ReadShort();

		string str = ReadString(len);

		int x = 20;
		int y = 20;

		if (!flag)
		{
			x = 200;
			y = 100;
		}

		TGumpTip *gump = new TGumpTip(serial, x, y, flag != 0);
		FontManager->GenerateA(6, gump->m_Text, str.c_str(), 0, 200);

		GumpManager->AddGump(gump);
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(MultiPlacement)
{
	if (World == NULL)
		return;

	//DWORD serial = unpack32(buf + 2);
	//WORD graphic = unpack16(buf + 18);

	Target.SetMultiData(buf, size);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(GraphicEffect)
{
	if (World == NULL)
		return;

	BYTE type = ReadByte();

	if (type > 3) //error
		return;

	DWORD sourceSerial = ReadDWord();
	DWORD destSerial = ReadDWord();
	WORD graphic = ReadWord();
	short sourceX = ReadShort();
	short sourceY = ReadShort();
	char sourceZ = ReadChar();
	short destX = ReadShort();
	short destY = ReadShort();
	char destZ = ReadChar();
	BYTE speed = ReadByte();
	BYTE duration = ReadByte();
	//what is in 24-25 bytes?
	Move(2);
	BYTE fixedDirection = ReadByte();
	BYTE explode = ReadByte();
	
	DWORD color = 0;
	DWORD renderMode = 0;

	if (*buf == 0xC0) 
	{
		color = ReadDWord();
		renderMode = ReadDWord();
	}

	TGameEffect *effect = NULL;
	if (!type) //Moving
		effect = new TGameEffectMoving();
	else
		effect = new TGameEffect();

	effect->EffectType = (EFFECT_TYPE)type;
	effect->Serial = sourceSerial;
	effect->DestSerial = destSerial;
	effect->Graphic = graphic;
	effect->X = sourceX;
	effect->Y = sourceY;
	effect->Z = sourceZ;
	effect->DestX = destX;
	effect->DestY = destY;
	effect->DestZ = destZ;
	effect->Speed = speed * 5;
	effect->Duration = GetTickCount() + (duration * 50);
	effect->FixedDirection = (fixedDirection != 0);
	effect->Explode = (explode != 0);
	
	effect->Color = (WORD)color;
	effect->RenderMode = renderMode;

	EffectManager->AddEffect(effect);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(BuyList)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();

	TGameItem *container = World->FindWorldItem(serial);

	if (container == NULL)
	{
		EPRINT("Buy container is not found!!!\n");
		return;
	}

	TGameCharacter *vendor = World->FindWorldCharacter(container->Container);

	if (vendor == NULL)
	{
		EPRINT("Buy vendor is not found!!!\n");
		return;
	}

	if (container->Layer == OL_BUY_RESTOCK || container->Layer == OL_BUY)
	{
		BYTE count = ReadByte();

		int i = (g_InverseBuylist ? count - 1 : 0);
		int end = (g_InverseBuylist ? - 1 : count);
		int add = (g_InverseBuylist ? - 1 : 1);

		TGameItem *item = (TGameItem*)container->m_Items;

		if (g_InverseBuylist)
		{
			while (item != NULL && item->m_Next != NULL)
				item = (TGameItem*)item->m_Next;
		}

		for (; i != end; i += add)
		{
			if (item == NULL)
			{
				EPRINT("Buy item is not found!!!\n");
				break;
			}

			if (item->ShopItem == NULL)
				item->ShopItem = new TShopItem();

			TShopItem *shop = item->ShopItem;

			shop->Price = ReadDWord();

			BYTE nameLen = ReadByte();

			shop->SetName(ReadString(nameLen));

			if (g_InverseBuylist)
				item = (TGameItem*)item->m_Prev;
			else
				item = (TGameItem*)item->m_Next;
		}
	}
	else
		TPRINT("Unknown layer for buy container!!!\n");
}
//---------------------------------------------------------------------------
PACKET_HANDLER(SellList)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();

	TGameCharacter *vendor = World->FindWorldCharacter(serial);

	if (vendor == NULL)
	{
		EPRINT("Sell vendor is not found!!!\n");
		return;
	}

	WORD count = ReadWord();

	if (!count)
	{
		EPRINT("Sell list is empty.\n");
		return;
	}

	GumpManager->CloseGump(0,0, GT_SHOP);

	TGumpSell *gump = new TGumpSell(serial, 100, 0, count);

	IFOR(i, 0, count)
	{
		TGumpSellObject *go = new TGumpSellObject();
		go->Serial = ReadDWord();
		go->Graphic = ReadWord();
		go->Color = ReadWord();
		go->Count = ReadWord();
		go->Price = ReadWord();
		int nameLen = ReadShort();
		go->SetName(ReadString(0));
		
		if (!i) //First item must be selected
			go->Selected = true;

		gump->Add(go);
	}

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(BuyReply)
{
	DWORD serial = ReadDWord();
	BYTE flag = ReadByte();

	if (!flag) //Close shop gump
		GumpManager->CloseGump(serial, 0, GT_SHOP);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(DeathScreen)
{
	Weather.Reset();
	Target.Reset();

	g_DeathScreenTimer = GetTickCount() + g_DeathScreenDelay;
}
//---------------------------------------------------------------------------
PACKET_HANDLER(PlaySoundEffect)
{
	Move(1);
	WORD index = ReadWord();

	//trace_printf("Play sound 0x%04X\n", index);
	UO->PlaySoundEffect(index);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(PlayMusic)
{
	WORD index = ReadWord();

	//trace_printf("Play midi music 0x%04X\n", index);
	SoundManager.PlayMidi(index);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(DragAnimation)
{
	if (World == NULL)
		return;

	WORD graphic = ReadWord();

	if (graphic == 0x0EED)
		graphic = 0x0EEF;

	Move(3);
	WORD count = ReadWord();

	DWORD sourceSerial = ReadDWord();
	short sourceX = ReadShort();
	short sourceY = ReadShort();
	char sourceZ = ReadChar();
	DWORD destSerial = ReadDWord();
	short destX = ReadShort();
	short destY = ReadShort();
	char destZ = ReadChar();
	
	TGameEffect *effect = NULL;

	if (sourceSerial < 0x40000000) //�����/��� ������ ������� � ���������
	{
		effect = new TGameEffectMoving();
		effect->FixedDirection = true;
	}
	else //������� ����� �� ����������
	{
		effect = new TGameEffectDrag();
	}

	effect->EffectType = EF_DRAG;
	effect->Serial = sourceSerial;
	effect->DestSerial = destSerial;
	effect->X = sourceX;
	effect->Y = sourceY;
	effect->Z = sourceZ;
	effect->DestX = destX;
	effect->DestY = destY;
	effect->DestZ = destZ;
	effect->Speed = 5;
	effect->Duration = GetTickCount() + 5000;

	effect->Graphic = graphic;

	EffectManager->AddEffect(effect);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(CorpseEquipment)
{
	if (World == NULL)
		return;

	DWORD cserial = ReadDWord();

	PBYTE end = buf + size;

	int layer = ReadByte();

	while (layer && Ptr < end)
	{
		DWORD serial = ReadDWord();

		TGameItem *obj = World->FindWorldItem(serial);

		if (obj != NULL)
			World->PutEquipment(obj, cserial, layer);

		layer = ReadByte();
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(ASCIIPrompt)
{
	if (World == NULL)
		return;

	if (g_ConsolePrompt != PT_NONE)
	{
		UO->ConsolePromptCancel();
		UO->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "Previous prompt cancelled.");
	}

	g_ConsolePrompt = PT_ASCII;
	memcpy(&g_LastASCIIPrompt[0], &buf[0], 11);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(UnicodePrompt)
{
	if (World == NULL)
		return;

	if (g_ConsolePrompt != PT_NONE)
	{
		UO->ConsolePromptCancel();
		UO->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "Previous prompt cancelled.");
	}

	g_ConsolePrompt = PT_UNICODE;
	memcpy(&g_LastUnicodePrompt[0], &buf[0], 11);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(CharacterAnimation)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	TGameCharacter *obj = World->FindWorldCharacter(serial);

	if (obj != NULL)
	{
		WORD action = ReadWord();
		Move(1);
		BYTE frameCount = ReadByte();
		WORD repeatMode = ReadWord();
		bool frameDirection = (ReadByte() == 0); //true - forward, false - backward
		bool repeat = (ReadByte() != 0);
		BYTE delay = ReadByte();

		obj->SetAnimation((BYTE)action, delay, frameCount, (BYTE)repeatMode, repeat, frameDirection);
		obj->AnimationFromServer = true;

		TPRINT("Anim is set\n");
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(DisplayQuestArrow)
{
	QuestArrow.Enabled = (ReadByte() != 0);
	QuestArrow.X = ReadWord();
	QuestArrow.Y = ReadWord();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(DisplayMap)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	WORD gumpid = ReadWord();

	//trace_printf("gumpid = 0x%04X\n", gumpid);

	WORD startX = ReadWord();
	WORD startY = ReadWord();
	WORD endX = ReadWord();
	WORD endY = ReadWord();
	WORD width = ReadWord();
	WORD height = ReadWord();
	WORD facet = 0;

	if (*buf == 0xF5)
		facet = ReadWord();

	TGumpMap *gump = new TGumpMap(serial, 0, 0);

	gump->StartX = startX;
	gump->StartY = startY;
	gump->EndX = endX;
	gump->EndY = endY;
	gump->Width = width;
	gump->Height = height;

	MultiMap->LoadMap(gump);

	//trace_printf("GumpX=%d GumpY=%d\n", startX, startY);
	//trace_printf("GumpTX=%d GumpTY=%d\n", endX, endY);
	//trace_printf("GumpW=%d GumpH=%d\n", width, height);

	GumpManager->AddGump(gump);

	TGameItem *obj = World->FindWorldItem(serial);
	if (obj != NULL)
		obj->Opened = true;
}
//---------------------------------------------------------------------------
PACKET_HANDLER(MapData)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	TGumpMap *gump = (TGumpMap*)GumpManager->GetGump(serial, 0, GT_MAP);

	if (gump != NULL)
	{
		switch ((MAP_MESSAGE)ReadByte()) //Action
		{
			case MM_ADD: //Add Pin
			{
				Move(1);

				short x = ReadShort();
				short y = ReadShort();

				TGumpObject *go = new TGumpObject(GOT_NONE, 0, 0, x, y);

				gump->Add(go);

				break;
			}
			case MM_INSERT: //Insert New Pin
			{
				break;
			}
			case MM_MOVE: //Change Pin
			{
				break;
			}
			case MM_REMOVE: //Remove Pin
			{
				break;
			}
			case MM_CLEAR: //Clear Pins
			{
				gump->Clear();

				break;
			}
			case MM_EDIT_RESPONSE: //Reply From Server to Action 6 (Plotting request)
			{
				gump->PlotState = ReadByte();

				break;
			}
			default:
				break;
		}

		gump->UpdateFrame();
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(DyeData)
{
	DWORD serial = ReadDWord();
	Move(2);
	WORD graphic = ReadWord();

	TGumpDye *gump = new TGumpDye(serial, 0, 0, graphic);

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(CharacterProfile)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();

	wstring topText = ToWString(ReadString(0));

	wstring bottomText = ReadUnicodeString(0);
	wstring dataText = ReadUnicodeString(0);

	TGumpProfile *gump = new TGumpProfile(serial, 170, 90, topText, bottomText, dataText);

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(ClientViewRange)
{
	g_UpdateRange = ReadByte();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(OpenBook)
{
	if (World == NULL)
		return;

	DWORD serial = unpack32(buf + 1);
	BYTE flags = buf[5];
	WORD pageCount = unpack16(buf + 7);

	TGumpBook *gump = new TGumpBook(serial, 0, 0, pageCount, flags != 0, false);

	char title[60] = {0};
	memcpy(&title[0], &buf[9], 60);

	gump->TextEntryTitle->SetText(title);

	char author[30] = {0};
	memcpy(&author[0], &buf[9], 30);

	gump->TextEntryAuthor->SetText(author);

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(OpenBookNew)
{
	if (World == NULL)
		return;

	DWORD serial = ReadDWord();
	BYTE flag1 = ReadByte();
	BYTE flag2 = ReadByte();
	WORD pageCount = ReadWord();

	TGumpBook *gump = new TGumpBook(serial, 0, 0, pageCount, (flag1 + flag2) != 0, true);

	PBYTE ptr = buf + 11;

	int authorLen = ReadWord();
	ptr += 2;

	if (authorLen > 0)
	{
		wchar_t *author = new wchar_t[authorLen];
		*author = 0;
		PBYTE aptr = (PBYTE)author;
		//
		ptr += (authorLen * 2);
		//
		gump->TextEntryAuthor->SetText(author);

		delete author;
	}
	else
		ptr += 2;

	int titleLen = unpack16(ptr);
	ptr += 2;

	if (titleLen > 0)
	{
		wchar_t *title = new wchar_t[titleLen];
		*title = 0;
		PBYTE tptr = (PBYTE)title;
		//
		gump->TextEntryTitle->SetText(title);

		delete title;
	}

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
PACKET_HANDLER(BookData)
{
	if (World == NULL)
		return;

	DWORD serial = unpack32(buf + 3);

	TGumpBook *gump = (TGumpBook*)GumpManager->GetGump(serial, 0, GT_BOOK);

	if (gump != NULL)
	{
		WORD pageCount = unpack16(buf + 7);
		bool unicode = gump->Unicode;

		PBYTE ptr = buf + 9;

		IFOR(i, 0, pageCount)
		{
			WORD page = unpack16(ptr);
			ptr += 2;
			gump->Page = page - 1;

			WORD lineCount = unpack16(ptr);
			ptr += 2;

			TEntryText *entry = gump->TextEntry[i % 2];
			entry->Clear();

			if (!unicode)
			{
				IFOR(j, 0, lineCount)
				{
					char ch = *ptr++;

					if (!ch)
						break;

					entry->Insert(ch);
				}
			}
			else
			{
				IFOR(j, 0, lineCount)
				{
					wchar_t ch = (ptr[1] << 16) | ptr[0];
					ptr += 2;

					if (!ch)
						break;

					entry->Insert(ch);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(KarriosClientSpecial)
{
	TPacketRazorAnswer packet;
	packet.Send();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(AssistVersion)
{
	DWORD version = ReadDWord();

	TPacketAssistVersion packet(version, g_ClientVersionText);
	packet.Send();
	packet.Free();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(CharacterListNotification)
{
	UO->InitScreen(GS_DELETE);
	ConnectionScreen->Type = CST_CHARACTER_LIST;
	ConnectionScreen->ConnectionFailed = true;
	ConnectionScreen->ErrorCode = ReadByte();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(ErrorCode)
{
	BYTE code = ReadByte();

	UO->InitScreen(GS_DELETE);
	ConnectionScreen->Type = CST_GAME_LOGIN;
	ConnectionScreen->ErrorCode = code;

	if (code > 7)
		ConnectionScreen->ErrorCode = 3;
}
//---------------------------------------------------------------------------
PACKET_HANDLER(AttackCharacter)
{
	g_LastAttackObject = ReadDWord();
}
//---------------------------------------------------------------------------
PACKET_HANDLER(Season)
{
	g_Season = ReadByte();

	if (ReadByte()) //Play sound
	{
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(BulletinBoardData)
{
	if (World == NULL)
		return;

	switch (ReadByte())
	{
		case 0: //Open board
		{
			DWORD serial = ReadDWord();

			TGameItem *item = World->FindWorldItem(serial);
			if (item != NULL)
			{
				TGump *bbGump = GumpManager->GetGump(serial, 0, GT_BULLETIN_BOARD);

				if (bbGump != NULL)
				{
					bbGump->Clear();
					bbGump->UpdateFrame();
				}

				item->Opened = true;
			}

			string str((char*)Ptr);

			int x = (g_ClientWidth / 2) - 245;
			int y = (g_ClientHeight / 2) - 205;

			TGumpBulletinBoard *gump = new TGumpBulletinBoard(serial, x, y, str);

			GumpManager->AddGump(gump);

			break;
		}
		case 1: //Summary message
		{
			DWORD boardSerial = ReadDWord();

			TGumpBulletinBoard *gump = (TGumpBulletinBoard*)GumpManager->GetGump(boardSerial, 0, GT_BULLETIN_BOARD);

			if (gump != NULL)
			{
				DWORD serial = ReadDWord();
				DWORD parentID = ReadDWord();

				//poster
				int len = ReadByte();
				string text = ReadString(len) + " - ";

				//subject
				len = ReadByte();
				text += ReadString(len) + " - ";

				//data time
				len = ReadByte();
				text += ReadString(len);

				TGumpBulletinBoardObject *item = new TGumpBulletinBoardObject(serial, text);

				gump->Add(item);
			}

			break;
		}
		case 2: //Message
		{
			DWORD boardSerial = ReadDWord();

			TGumpBulletinBoard *gump = (TGumpBulletinBoard*)GumpManager->GetGump(boardSerial, 0, GT_BULLETIN_BOARD);

			if (gump != NULL)
			{
				DWORD serial = ReadDWord();

				//poster
				int len = ReadByte();
				string poster = ReadString(len);

				//subject
				len = ReadByte();
				string subject = ReadString(len);

				//data time
				len = ReadByte();
				string dataTime = ReadString(len);

				Move(5);

				BYTE lines = ReadByte();
				string data = "";

				IFOR(i, 0, lines)
				{
					BYTE linelen = ReadByte();

					if (data.length())
						data += "\n";

					data += ReadString(linelen);
				}

				BYTE variant = 1 + (int)(poster == g_Player->GetName());
				TGumpBulletinBoardItem *gump = new TGumpBulletinBoardItem(serial, 0, 0, variant, boardSerial, poster, subject, dataTime, data);

				GumpManager->AddGump(gump);
			}

			break;
		}
		default:
			break;
	}
}
//---------------------------------------------------------------------------
PACKET_HANDLER(DisplayDeath)
{
	DWORD serial = ReadDWord();
	DWORD corpseSerial = ReadDWord();

	TGameCharacter *gc = World->FindWorldCharacter(serial);
	if (gc != NULL)
		gc->CorpseLink = corpseSerial;
}
//---------------------------------------------------------------------------