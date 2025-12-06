#pragma once

struct player_info_t {
	char u0[0x10];
	char name[128];
	int useid;
	char guid[33];
	unsigned long friendsid;
	char friends_name[128];
	bool fakeplayer;
	bool ishltv;
	unsigned int custom_files[4];
	unsigned char files_downloaded;
	char u2[0x200];
};