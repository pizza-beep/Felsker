#include "cheats.hpp"
#include "types.h"
#include "3ds.h"

namespace CTRPluginFramework
{

void dropEverything(MenuEntry *entry){
    if (!Process::IsPaused()){
    static int myInt = 1;
    if (myInt == 1){
        OSD::Notify("Use ZL+ZR to Drop Item in Hand.");
        myInt++;
    }
    if (hidKeysDown() & (KEY_ZL | KEY_ZR))
    {
        Process::Write32(0xB329E8, 0x02);
        Process::Write32(0xB32948, 0x02);
        Process::Write32(0xB32A00, 0x02);
        Process::Write32(0xB32A18, 0x02);
        Process::Write32(0xB32A30, 0x02);
        Process::Write32(0xB32A48, 0x02);
        Process::Write32(0xB32A60, 0x02);
        Process::Write32(0xB32A78, 0x02);
        Process::Write32(0xB32A90, 0x02);
        Process::Write32(0xB32AB0, 0x02);
        svcSleepThread(16666);
        Process::Write32(0xB329E8, 0x00);
        Process::Write32(0xB32948, 0x00);
        Process::Write32(0xB32A00, 0x00);
        Process::Write32(0xB32A18, 0x00);
        Process::Write32(0xB32A30, 0x00);
        Process::Write32(0xB32A48, 0x00);
        Process::Write32(0xB32A60, 0x00);
        Process::Write32(0xB32A78, 0x00);
        Process::Write32(0xB32A90, 0x00);
        Process::Write32(0xB32AB0, 0x00);
        svcSleepThread(16666);
    }
}
}

void ninetyFov(MenuEntry *entry){
    if (!Process::IsPaused()){
    static int myInt = 1;
    if (myInt == 1){
        OSD::Notify("Set FOV to 90.");
        myInt++;
    }
    float fovVal = 90;
    Process::WriteFloat(0x3CEE80, fovVal);
}
}

void defaultCodes(MenuEntry *entry){
    if (!Process::IsPaused()){
    static int myInt = 1;
    if (myInt == 1){
        OSD::Notify("Set Megapack Default Codes.");
        myInt++;
    }
    float swimVal = 0.075;
    float viewBobVal = 10.0;
    Process::WriteFloat(0x4EA090, swimVal);
    Process::WriteFloat(0x3CF2A0, viewBobVal);
}
}

void removeMobCap(MenuEntry *entry){
    if (!Process::IsPaused()){
    static int myInt = 1;
    if (myInt == 1){
        OSD::Notify("Removed the Mob Spawn-Limit.");
        myInt++;
    }
    Process::Write32(0xA33898, 0x00);
    Process::Write32(0xA338A8, 0x00);
    Process::Write32(0xA338AC, 0x00);
    Process::Write32(0xA338B0, 0x00);
    Process::Write32(0xA338B4, 0x00);
    Process::Write32(0xA338B8, 0x00);
    Process::Write32(0xA338BC, 0x00);
    Process::Write32(0xA338C0, 0x00);
}
}

void stopMobSpawns(MenuEntry *entry){
    if (!Process::IsPaused()){
    static int myInt = 1;
    if (myInt == 1){
        OSD::Notify("Disabled Mob-Spawning.");
        myInt++;
    }
    Process::Write32(0xA33898, 0xB0);
    Process::Write32(0xA338A8, 0xB0);
    Process::Write32(0xA338AC, 0xB0);
    Process::Write32(0xA338B0, 0xB0);
    Process::Write32(0xA338B4, 0xB0);
    Process::Write32(0xA338B8, 0xB0);
    Process::Write32(0xA338BC, 0xB0);
    Process::Write32(0xA338C0, 0xB0);
}
}

void itemExpLimit(MenuEntry *entry){
    if (!Process::IsPaused()){
    static int myInt = 1;
    if (myInt == 1){
        OSD::Notify("Removed Ground-Item Limit.");
        myInt++;
    }
    Process::Write32(0xA339C0, 0x00);
    Process::Write32(0xA339AC, 0x00);
}
}

void enhancedParticles(MenuEntry *entry){
    if (!Process::IsPaused()){
    static int myInt = 1;
    if (myInt == 1){
        OSD::Notify("Fixed Write32 Panic.");
        myInt++;
    }
    Process::Write8(0x14A4F, 0xE2);
}
}

bool displayPlayerCoordsTopScreen(const Screen& screen){
	if (!screen.IsTop) return false;
    static bool erb, uwtr;
	static float x, y, z;
    static u16 mid;
    static u32 mc1, mc2, mc3, mc4, mc5, mc6, mc7, mc8, er3, itm, exp, wtr0, wtr1;
    if (!Process::IsPaused()){
    	Process::ReadFloat(0xAC1E48, x);
	    Process::ReadFloat(0xAC1E4C, y);
		Process::ReadFloat(0xAC1E50, z);
        Process::Read32(0xA33898, mc1);
        Process::Read32(0xA338A8, mc2);
        Process::Read32(0xA338AC, mc3);
        Process::Read32(0xA338B0, mc4);
        Process::Read32(0xA338B4, mc5);
        Process::Read32(0xA338B8, mc6);
        Process::Read32(0xA338BC, mc7);
        Process::Read32(0xA338C0, mc8);
        Process::Read16(0x3504A092, mid);
        Process::Read32(0x3018AEF0, er3);
        Process::Read32(0xA339C0, itm);
        Process::Read32(0xA339AC, exp);
        Process::Read32(0xFFFE064, wtr0);
        Process::Read32(0xFFFDFFC, wtr1);
    }
    if (wtr0 == 1 || wtr1 == 1){
        uwtr = true;
    } else {
        uwtr = false;
    }
    u32 noe = mc1+mc2+mc3+mc4+mc5+mc6+mc7+mc8;
    if (er3 == 1) {
        erb = true;
    } else {
        erb = false;
    }
	screen.Draw("Player.X: " + Utils::Format("%.2f", x), 10, 45, Color::White, Color::Black);
    screen.Draw("Player.Y: " + Utils::Format("%.2f", y), 10, 55, Color::White, Color::Black);
    screen.Draw("Player.Z: " + Utils::Format("%.2f", z), 10, 65, Color::White, Color::Black);
    screen.Draw("3D Engine?: " + Utils::Format("%s", erb ? "true" : "false"), 10, 85, Color::White, Color::Black);
    screen.Draw("IsSwimming?: " + Utils::Format("%s", uwtr ? "true" : "false"), 10, 95, Color::White, Color::Black);
    screen.Draw("EXP Rendered: " + Utils::Format("%u", exp), 10, 105, Color::White, Color::Black);
    screen.Draw("Mobs Rendered: " + Utils::Format("%u", noe), 10, 115, Color::White, Color::Black);
    screen.Draw("Items Rendered: " + Utils::Format("%u", itm), 10, 125, Color::White, Color::Black);
    // screen.Draw("Entity In-Hand: " + Utils::Format("%u", mid), 10, 85, Color::White, Color::Black);
	return true;
}

// fun codes
void betterMinecartPhysics(MenuEntry *entry){
    if (!Process::IsPaused()){
    static int myInt = 1;
    if (myInt == 1){
        OSD::Notify("Configured Better Minecart Physics.");
        myInt++;
    }
    Process::WriteFloat(0x659B48, 0.0);
    Process::WriteFloat(0x659B54, 0.0085);
}
}

void removeHeads(MenuEntry *entry){
    if (!Process::IsPaused()){
    static int myInt = 1;
    if (myInt == 1){
        OSD::Notify("Removed Most Mob Heads.");
        myInt++;
    }
    Process::WriteFloat(0x21F42C, 0.0);
}
}

void everythingSpinny(MenuEntry *entry){
    if (!Process::IsPaused()){
    static int myInt = 1;
    static float playerPos;
    if (myInt == 1){
        OSD::Notify("Everything is now spinning.");
        myInt++;
    }
    Process::ReadFloat(0x6A6D3C, playerPos);
    Process::WriteFloat(0x6A6D3C, playerPos+1);
    Process::WriteFloat(0x17B728, 0.0);
}
}

}