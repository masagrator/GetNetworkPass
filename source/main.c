// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

typedef struct {
    u8 ssid_len;                                         ///< NifmSfWirelessSettingData::ssid_len
    char ssid[0x21];                                     ///< NifmSfWirelessSettingData::ssid
    u8 unk_x22;                                          ///< NifmSfWirelessSettingData::unk_x21
    u8 pad;                                              ///< Padding
    u32 unk_x24;                                         ///< NifmSfWirelessSettingData::unk_x22
    u32 unk_x28;                                         ///< NifmSfWirelessSettingData::unk_x23
    u8 passphrase_len;                                   ///< Passphrase length
    u8 passphrase[0x41];                                 ///< NifmSfWirelessSettingData::passphrase
    u8 pad2[0x2];                                        ///< Padding
} NifmWirelessSettingData_new;

/// NetworkProfileData. Converted from/to \ref NifmSfNetworkProfileData.
typedef struct {
    Uuid uuid;                                           ///< NifmSfNetworkProfileData::uuid
    char network_name[0x40];                             ///< NifmSfNetworkProfileData::network_name
    u32 unk_x50;                                         ///< NifmSfNetworkProfileData::unk_x112
    u32 unk_x54;                                         ///< NifmSfNetworkProfileData::unk_x113
    u8 unk_x58;                                          ///< NifmSfNetworkProfileData::unk_x114
    u8 unk_x59;                                          ///< NifmSfNetworkProfileData::unk_x115
    u8 pad[2];                                           ///< Padding
    NifmWirelessSettingData_new wireless_setting_data;   ///< \ref NifmWirelessSettingData
    NifmIpSettingData ip_setting_data;                   ///< \ref NifmIpSettingData
} NifmNetworkProfileData_new;

// Main program entrypoint
int main(int argc, char* argv[])
{
	// This example uses a text console, as a simple way to output text to the screen.
	// If you want to write a software-rendered graphics application,
	//   take a look at the graphics/simplegfx example, which uses the libnx Framebuffer API instead.
	// If on the other hand you want to write an OpenGL based application,
	//   take a look at the graphics/opengl set of examples, which uses EGL instead.
	consoleInit(NULL);

	// Configure our supported input layout: a single player with standard controller styles
	padConfigureInput(1, HidNpadStyleSet_NpadStandard);

	// Initialize the default gamepad (which reads handheld mode inputs as well as the first connected controller)
	PadState pad;
	padInitializeDefault(&pad);

	Result rc = nifmInitialize(NifmServiceType_Admin);
	// Other initialization goes here. As a demonstration, we print hello world.
	if (R_FAILED(rc))
		printf("Initialize failed: 0x%x\n", rc);
	else {
		NifmNetworkProfileData_new* profile = malloc(sizeof(NifmNetworkProfileData_new));
		rc = nifmGetCurrentNetworkProfile((NifmNetworkProfileData*)profile);
		nifmExit();
		if (R_FAILED(rc)) {
			printf("GetCurrentNetworkProfile failed: 0x%x\n", rc);
			if (rc == 0xd46e)
				printf("Are you connected to network?\n");
		}
		else {
			printf("Network Password: %s\n", profile->wireless_setting_data.passphrase);
			free(profile);
		}

	}
	printf("To exit, press +\n");

	// Main loop
	while (appletMainLoop())
	{
		// Scan the gamepad. This should be done once for each frame
		padUpdate(&pad);

		// padGetButtonsDown returns the set of buttons that have been
		// newly pressed in this frame compared to the previous one
		u64 kDown = padGetButtonsDown(&pad);

		if (kDown & HidNpadButton_Plus)
			break; // break in order to return to hbmenu

		// Your code goes here

		// Update the console, sending a new frame to the display
		consoleUpdate(NULL);
	}

	// Deinitialize and clean up resources used by the console (important!)
	consoleExit(NULL);
	return 0;
}
