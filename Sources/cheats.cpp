#include "cheats.hpp"
#include "sha256.hpp"
#include "types.h"
#include "3ds.h"
#include <iostream>
#include <dirent.h>
#include <fstream>
#include <unordered_map>
#include <sstream>
#include <cstring>
#include <cstdint>
#include <random>

namespace CTRPluginFramework
{
    std::random_device rd;
    std::mt19937 gen(rd());    
    
    float getChance(){
        std::uniform_real_distribution<float> myCnt(0.0f, 10.0f);
        return myCnt(gen);
    }

    float getWeatherAmount(){
        std::uniform_real_distribution<float> wtherSetAmnt(0.01f, 0.05f);
        return wtherSetAmnt(gen);
    }

    float getFogDensity(){
        std::uniform_real_distribution<float> fgDnsty(35.0f, 80.0f);
        return fgDnsty(gen);
    }

    int getTimerCountFog(){
        std::uniform_int_distribution<u32> tmrFgCnt(40000, 55000);
        return tmrFgCnt(gen);
    }

    int getTimerCountClouds(){
        std::uniform_int_distribution<u32> tmrCldCnt(750, 1500);
        return tmrCldCnt(gen);
    }

    float getCloudRange(){
        std::uniform_real_distribution<float> cldRange(0.001f, 0.01f);
        return cldRange(gen);
    }

    class Player {
        public:
            bool IsSwimming() {
                u16 a, b;
                Process::Read16(0x0FFFE064, a); // Always one of these two offsets
                Process::Read16(0x0FFFDFFC, b);
                if (a == 1 || b == 1){
                    return true;
                }
                return false;
            }
            int GetSlotNumber() {
                u32 slotCount;
                Process::Read32(0x3502D0E4, slotCount);
                slotCount++;
                return slotCount;
            }
            int GetItemCount() {
                u32 itemCount;
                Process::Read32(0x3504A0FC, itemCount);
                return itemCount;
            }
            bool IsFlying() {
                u16 flying;
                Process::Read16(0x3507E5D0, flying);
                if (flying == 1){
                    return true;
                }
                return false;
            }
            bool IsIdle() { // timer require additional logic, add at later date
                u32 a, b, c, d, e, f, g, h, i, j;
                Process::Read32(0xB329E8, a);
                Process::Read32(0xB32948, b);
                Process::Read32(0xB32A00, c);
                Process::Read32(0xB32A18, d);
                Process::Read32(0xB32A30, e);
                Process::Read32(0xB32A48, f);
                Process::Read32(0xB32A60, g);
                Process::Read32(0xB32A78, h);
                Process::Read32(0xB32A90, i);
                Process::Read32(0xB32AB0, j);
                if (a == 0x00 && b == 0x00 && c == 0x00 && d == 0x00 && e == 0x00 && f == 0x00 && g == 0x00 && h == 0x00 && i == 0x00 && j == 0x00) {
                    return true;
                }
                return false;
            }
            bool IsJumping() {
                u32 a, b, c, d, e, f, g, h, i, j;
                Process::Read32(0xB329E8, a);
                Process::Read32(0xB32948, b);
                Process::Read32(0xB32A00, c);
                Process::Read32(0xB32A18, d);
                Process::Read32(0xB32A30, e);
                Process::Read32(0xB32A48, f);
                Process::Read32(0xB32A60, g);
                Process::Read32(0xB32A78, h);
                Process::Read32(0xB32A90, i);
                Process::Read32(0xB32AB0, j);
                if (a == 0x00 && b == 0x01 && c == 0x01 && d == 0x01 && e == 0x01 && f == 0x01 && g == 0x01 && h == 0x01 && i == 0x01 && j == 0x01) {
                    return true;
                }
                return false;
            }
    };

    std::stringstream skinsBjson, mPath;
    u64 processId;
    std::string filename;
    std::string modsPath;
    std::string FelskerDir;
    std::string FelskerWorldDir;
    std::string FelskerCodeDir;
    std::string FelskerSkinsDir;
    std::string FelskerAnimationsHeadDir;

    void initializePaths() {
        processId = Process::GetTitleID();
        skinsBjson << "sdmc:/luma/titles/000" << std::hex << processId << "/romfs/resourcepacks/skins/skinpacks/skins.bjson";
        mPath << "sdmc:/luma/titles/000" << std::hex << processId << "/";
        modsPath = mPath.str();
        filename = skinsBjson.str();
    }

    void checkAndCreateDirectories() {
        FelskerDir = "sdmc:/Minecraft 3ds/Felsker";
        FelskerWorldDir = "sdmc:/Minecraft 3ds/Felsker/worldBackups";
        FelskerCodeDir = "sdmc:/Minecraft 3ds/Felsker/codeBackups";
        FelskerSkinsDir = "sdmc:/Minecraft 3ds/Felsker/Skins";
        FelskerAnimationsHeadDir = "sdmc:/Minecraft 3ds/Felsker/Animations/Helmet";
        if (Directory::IsExists(FelskerDir) == 0){
            Directory::Create(FelskerDir);
            Directory::Create(FelskerWorldDir);
            Directory::Create(FelskerCodeDir);
            Directory::Create(FelskerSkinsDir);
            Directory::Create(FelskerAnimationsHeadDir);
        }
        if (Directory::IsExists(FelskerSkinsDir) == 0 || Directory::IsExists(FelskerAnimationsHeadDir) == 0){
            Directory::Create(FelskerSkinsDir);
            Directory::Create(FelskerAnimationsHeadDir);
        }
    }

void dropEverything(MenuEntry *entry){
    if (!Process::IsPaused()){
    static int myInt = 1;
    if (myInt == 1){
        OSD::Notify("Use ZL+ZR to drop items in hand.");
        myInt++;
    }
    Process::Write32(0xB329F8, 0x8F0004);
    Process::Write32(0xB32A10, 0x8F0004);
    Process::Write32(0xB32A28, 0x8F0004);
    Process::Write32(0xB32A40, 0x8F0004);
    Process::Write32(0xB32A58, 0x8F0004);
    Process::Write32(0xB32A70, 0x8F0004);
    Process::Write32(0xB32A88, 0x8F0004);
    Process::Write32(0xB32AA0, 0x8F0004);
    Process::Write32(0xB32AA8, 0x8F0004);
    }
}

void ninetyFov(MenuEntry *entry){
    if (!Process::IsPaused()){
    static int myInt = 1;
    if (myInt == 1){
        OSD::Notify("Set item FOV to 90.");
        myInt++;
    }
    float fovVal = 90;
    Process::WriteFloat(0x3CEE80, fovVal);
}
}

void defaultCodes(){
    if (!Process::IsPaused()){
    Process::WriteFloat(0x4EA090, 0.035);
    Process::WriteFloat(0x3CF2A0, 10.0);
    Process::WriteFloat(0x4E61D0, 2.5);
}
}

void removeMobCap(MenuEntry *entry){
    if (!Process::IsPaused()){
    static int myInt = 1;
    if (myInt == 1){
        OSD::Notify("Removed the mob spawn limit.");
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
        OSD::Notify("Disabled mob spawning.");
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
        OSD::Notify("Removed dropped item limit.");
        myInt++;
    }
    Process::Write32(0xA339C0, 0x00);
    Process::Write32(0xA339C4, 0x00);
}
}

void enhancedParticles(MenuEntry *entry){
    if (!Process::IsPaused()){
    static int myInt = 1;
    if (myInt == 1){
        OSD::Notify("Enabled Enhanced Particles.");
        myInt++;
    }
    Process::Write8(0x14A4F, 0xE2);
}
}
void seedchanger(MenuEntry *entry) {
    u32 seed;
    u32 b1;
    Process::Read32(0x0FFFE0A0, b1);
    if (b1 != 0) {
        Process::Read32(b1 + 0xC0, seed);
    } else {
        seed = 0;
    }
    Keyboard T("Enter a seed\n[Current value] : " + std::to_string(seed));
    T.IsHexadecimal(false);
    if (T.Open(seed) != -1) { // Only write if user pressed OK
        if (b1 != 0) {
            Process::Write32(b1 + 0xC0, seed);
            OSD::Notify("Seed updated.");
        } else {
            OSD::Notify("Seed address not found.");
        }
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
	screen.Draw("PlayerX: " + Utils::Format("%.2f", x), 10, 45, Color::White, Color::Black);
    screen.Draw("PlayerY: " + Utils::Format("%.2f", y), 10, 55, Color::White, Color::Black);
    screen.Draw("PlayerZ: " + Utils::Format("%.2f", z), 10, 65, Color::White, Color::Black);
    screen.Draw("3DEngine?: " + Utils::Format("%s", erb ? "true" : "false"), 10, 85, Color::White, Color::Black);
    screen.Draw("IsSwimming?: " + Utils::Format("%s", uwtr ? "true" : "false"), 10, 95, Color::White, Color::Black);
    screen.Draw("ExpRendered: " + Utils::Format("%u", exp), 10, 105, Color::White, Color::Black);
    screen.Draw("MobsRendered: " + Utils::Format("%u", noe), 10, 115, Color::White, Color::Black);
    screen.Draw("ItemsRendered: " + Utils::Format("%u", itm), 10, 125, Color::White, Color::Black);
	return true;
}

// fun codes
void betterMinecartPhysics(MenuEntry *entry){
    if (!Process::IsPaused()){
    static int myInt = 1;
    if (myInt == 1){
        OSD::Notify("Enabled Better Minecart Physics.");
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
        OSD::Notify("Removed mob heads.");
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

void ModifySignedIntFromFile(const std::string& filename, std::streampos offset) {
    File file;
    int32_t value;

    // Open the file for reading and writing
    if (File::Open(file, filename, File::READ | File::WRITE) != 0) {
        OSD::Notify("Failed to open file!");
        return;
    }

    // Seek to the offset and read 4 bytes (signed 32-bit integer)
    file.Seek(offset, File::SET);
    file.Read(&value, sizeof(value));

    OSD::Notify(Utils::Format("Initial Value: %d", value));

    // Infinite loop (press SELECT + B in CTRPF to exit)
    while (true) {
        // Increment 5 times
        for (int i = 0; i < 5; ++i) {
            value++;
            file.Seek(offset, File::SET);
            file.Write(&value, sizeof(value));
            OSD::Notify(Utils::Format("Incremented: %d", value));
        }

        // Decrement 5 times
        for (int i = 0; i < 5; ++i) {
            value--;
            file.Seek(offset, File::SET);
            file.Write(&value, sizeof(value));
            OSD::Notify(Utils::Format("Decremented: %d", value));
        }
    }

    file.Close();  // Close file (though we never reach this due to infinite loop)
}

// Lookup table for offsets
std::unordered_map<std::string, int> modelOffsets = {
    {"Body.Pivot.X", 0x50}, {"Body.Pivot.Y", 0x5C}, {"Body.Pivot.Z", 0x68},
    {"Body.Origin.X", 0x98}, {"Body.Origin.Y", 0xA4}, {"Body.Origin.Z", 0xB0},
    {"Body.Size.X", 0xC8}, {"Body.Size.Y", 0xD4}, {"Body.Size.Z", 0xE0},
    {"Body.UV.X", 0xF8}, {"Body.UV.Y", 0x104},
    {"Head.Pivot.X", 0x134}, {"Head.Pivot.Y", 0x140}, {"Head.Pivot.Z", 0x14C},
    {"Head.Origin.X", 0x17C}, {"Head.Origin.Y", 0x188}, {"Head.Origin.Z", 0x194},
    {"Head.Size.X", 0x1AC}, {"Head.Size.Y", 0x1B8}, {"Head.Size.Z", 0x1C4},
    {"Head.UV.X", 0x1DC}, {"Head.UV.Y", 0x1E8},
    {"Hat.Pivot.X", 0x218}, {"Hat.Pivot.Y", 0x224}, {"Hat.Pivot.Z", 0x230},
    {"Hat.Origin.X", 0x260}, {"Hat.Origin.Y", 0x26C}, {"Hat.Origin.Z", 0x278},
    {"Hat.Size.X", 0x290}, {"Hat.Size.Y", 0x29C}, {"Hat.Size.Z", 0x2A8},
    {"Hat.UV.X", 0x2C0}, {"Hat.UV.Y", 0x2CC},
    {"RightArm.Pivot.X", 0x314}, {"RightArm.Pivot.Y", 0x320}, {"RightArm.Pivot.Z", 0x32C},
    {"RightArm.Origin.X", 0x35C}, {"RightArm.Origin.Y", 0x368}, {"RightArm.Origin.Z", 0x374},
    {"RightArm.Size.X", 0x38C}, {"RightArm.Size.Y", 0x398}, {"RightArm.Size.Z", 0x3A4},
    {"RightArm.UV.X", 0x3BC}, {"RightArm.UV.Y", 0x3C8},
    {"LeftArm.Pivot.X", 0x3F8}, {"LeftArm.Pivot.Y", 0x404}, {"LeftArm.Pivot.Z", 0x410},
    {"LeftArm.Origin.X", 0x440}, {"LeftArm.Origin.Y", 0x44C}, {"LeftArm.Origin.Z", 0x458},
    {"LeftArm.Size.X", 0x470}, {"LeftArm.Size.Y", 0x47C}, {"LeftArm.Size.Z", 0x488},
    {"LeftArm.UV.X", 0x4A0}, {"LeftArm.UV.Y", 0x4AC},
    {"RightLeg.Pivot.X", 0x4E8}, {"RightLeg.Pivot.Y", 0x4F4}, {"RightLeg.Pivot.Z", 0x500},
    {"RightLeg.Origin.X", 0x530}, {"RightLeg.Origin.Y", 0x53C}, {"RightLeg.Origin.Z", 0x548},
    {"RightLeg.Size.X", 0x560}, {"RightLeg.Size.Y", 0x56C}, {"RightLeg.Size.Z", 0x578},
    {"RightLeg.UV.X", 0x590}, {"RightLeg.UV.Y", 0x59C},
    {"LeftLeg.Pivot.X", 0x5CC}, {"LeftLeg.Pivot.Y", 0x5D8}, {"LeftLeg.Pivot.Z", 0x5E4},
    {"LeftLeg.Origin.X", 0x614}, {"LeftLeg.Origin.Y", 0x620}, {"LeftLeg.Origin.Z", 0x62C},
    {"LeftLeg.Size.X", 0x644}, {"LeftLeg.Size.Y", 0x650}, {"LeftLeg.Size.Z", 0x65C},
    {"LeftLeg.UV.X", 0x674}, {"LeftLeg.UV.Y", 0x680},
};

int getOffset(const std::string &key) {
    auto it = modelOffsets.find(key);
    return (it != modelOffsets.end()) ? it->second : -1;
}

void selectAndModifyOffset() {
    std::vector<std::string> bodyParts = {"Body", "Head", "Hat", "RightArm", "LeftArm", "RightLeg", "LeftLeg"};
    std::vector<std::string> properties = {"Pivot", "Origin", "Size", "UV"};
    std::vector<std::string> coordinates = {"X", "Y", "Z"};

    Keyboard kb1("Select body part:");
    kb1.Populate(bodyParts);
    int bodyIndex = kb1.Open();
    if (bodyIndex < 0) return;

    Keyboard kb2("Select property:");
    kb2.Populate(properties);
    int propIndex = kb2.Open();
    if (propIndex < 0) return; 
    
    if (properties[propIndex] == "UV"){
        coordinates = {"X", "Y"};
    }

    Keyboard kb3("Select coordinate:");
    kb3.Populate(coordinates);
    int coordIndex = kb3.Open();
    if (coordIndex < 0) return; 

    std::string key = bodyParts[bodyIndex] + "." + properties[propIndex] + "." + coordinates[coordIndex];

    int offset = getOffset(key);
    if (offset != -1) {
        File file;
        u32 value;

        if (File::Open(file, filename, File::READ | File::WRITE) != 0) {
            OSD::Notify("Failed to open file!");
            return;
        }

        file.Seek(offset, File::SET);
        OSD::Notify("Selected: " + key + " | Offset: 0x" + Utils::Format("%X", offset));

        Keyboard kb4("Enter new value:");
        kb4.IsHexadecimal(false);
        if (kb4.Open(value) != -1) {
            file.Write(&value, sizeof(value));
            OSD::Notify("Updated " + key + " to " + std::to_string(value));
        }
        file.Close();
    } else {
        OSD::Notify("Invalid selection!");
    }
}

void backupWorld(){
    std::vector<u8> valToSearch {0x98, 0xEF, 0xCD, 0xAB};  // Pattern to search for
    u32 endAddress = 0x36480000;
    static u32 startAddress = 0x33000000;
    static u32 size = endAddress - startAddress; 
    static u16 i = 0;
    File file;
    while (true){
        u32 getBaseAddress = Utils::Search(startAddress, size, valToSearch);
        if (getBaseAddress != 0x00){
            OSD::Notify(Utils::Format("Found save address at: 0x%X", getBaseAddress));
            File::Create(FelskerWorldDir + Utils::Format("/slt%u.cdb", i));

            if (File::Open(file, FelskerWorldDir + Utils::Format("/slt%u.cdb", i), File::WRITE) != 0){
                OSD::Notify("Failed to open the CDB Slot file!");
                return;
            } else{
                file.Dump(getBaseAddress, 0x140000);
                OSD::Notify("Dumped CDB Chunk.");
            }
            file.Close();
            startAddress += 0x140000;
            size = endAddress - startAddress;
            i++;
        } else{
            break;
        }
    }
    i = 0;
    startAddress = 0x33000000;
    size = endAddress - startAddress;
    return;
}

void creativeMode(MenuEntry *entry){
    Process::Write32(0x005FF184, 0xEB0C65DD);
    Process::Write32(0x00918900, 0xE5940000);
    Process::Write32(0x00918904, 0xE58F4000);
    Process::Write32(0x00918908, 0xEA000000);
    Process::Write32(0x00918910, 0xE12FFF1E);

    u32 baseAddress;
    Process::Read32(0x0091890C, baseAddress);
    Process::Write32(baseAddress + 0x196C, 0x00000001);
}

void dumpExecutable() {
    u32 baseAddress = 0x100000;
    u32 totalSize = 0x93A000;
    File file;
    File::Create(FelskerCodeDir + "/code_full.bin");
    if (File::Open(file, FelskerCodeDir + "/code_full.bin", File::WRITE) == 0){
        file.Dump(baseAddress, totalSize);
        MessageBox("Notice", "Dumped executable to:\n" + FelskerCodeDir, DialogType::DialogOk, ClearScreen::Both)();
        OSD::Notify("Dumped executable to: " + FelskerCodeDir);
    }
    file.Close();
}

void dumpStriptExecutable() { // gets rid of variable data stored inside of mc3ds code.bin memory
    u32 baseAddress = 0x100000;
    u32 totalSize = 0x73A000;
    File file;
    File::Create(FelskerCodeDir + "/code_stripped.bin");
    if (File::Open(file, FelskerCodeDir + "/code_stripped.bin", File::WRITE) == 0){
        file.Dump(baseAddress, totalSize);
        MessageBox("Notice", "Dumped stripped executable to:\n" + FelskerCodeDir, DialogType::DialogOk, ClearScreen::Both)();
        OSD::Notify("Dumped stripped executable to: " + FelskerCodeDir);
    }
    file.Close();
}

void mobController() { // Thanks to Darksiders for Base AR Cheats and allowing us to create this using his codes as references to use and expand the og codes
    u32 ourVal, baseAddress;
    std::vector<std::string> mobSelector = {
        "Player", "Chicken", "Cow", "Mooshroom", "Pig", "Sheep", "Bat",
        "Wolf", "Ender Dragon", "Villager", "Zombie", "Zombie Pigman", "Ghast",
        "Silverfish", "Creeper", "Enderman", "Shulker Bullet", "Iron Golem",
        "Cat", "Snow Golem", "Guardian", "Wither", "EXP Orb", "Zombie Villager",
        "Shulker", "Rabbit", "Witch", "Llama", "Camera", "Husk", "Stray", "Skeleton",
        "Endermite", "Evoker", "Vex", "Vindicator", "Polar Bear", "Blaze"
    };
    std::vector<u32> mobValues = {
        0x1E, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, // Player, Chicken, Cow, Mooshroom, Pig, Sheep, Bat
        0x0B, 0x0C, 0x0E, 0x10, 0x11, 0x13, // Wolf, Dragon, Villager, Zombie, Zombie Pigman, Ghast
        0x17, 0x18, 0x1A, 0x1C, 0x30, // Silverfish, Creeper, Enderman, Shulker Bullet, Iron Golem
        0x31, 0x32, 0x3E, 0x41, 0x2E, 0x2D, // Cat, Snow Golem, Guardian, Wither, EXP Orb, Zombie Villager
        0x33, 0x35, 0x36, 0x38, 0x39, 0x44, 0x45, 0x46, // Shulker, Rabbit, Witch, Llama, Camera, Husk, Stray, Skeleton (0x15 as well)
        0x49, 0x4A, 0x4C, 0x4D, 0x0D, 0x14 // Endermite, Evoker, Vex, Vindicator, Polar Bear, Blaze
    };
    Keyboard kb1("Select a mob/entity to possess:");
    kb1.Populate(mobSelector);
    int mobIndex = kb1.Open();
    if (mobIndex < 0) return;
    ourVal = mobValues[mobIndex];
    Process::Read32(0xFFFDF74, baseAddress);
    Process::Write32(baseAddress+0x278, ourVal);
    OSD::Notify("Change player into: " + mobSelector[mobIndex]);
}

void sha256_hash(const uint8_t *data, size_t length, uint8_t *hash) {
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, data, length);
    sha256_final(&ctx, hash);
}

void dynaClouds(MenuEntry *entry) {
    static u32 timerCount = 1000;
    static u32 timer = 0;
    static float baseCloudHeight = 0.33;
    static float baseCloudThickness = 4.0;
    static float heightRate = 0.0035;
    static float thicknessRate = 0.0035;
    static bool increasing = true; 
    u32 chAdd = 0x3C5398;
    u32 ctAdd = 0x3C53BC;
    float a, b;
    
    Process::ReadFloat(ctAdd, a);
    Process::ReadFloat(chAdd, b);
    
    if (increasing) {
        a += thicknessRate;
        b -= heightRate;
        timer++;
        if (timer == timerCount) {
            increasing = false;
        }
    } else {
        a -= thicknessRate;
        b += heightRate;
        timer--;
        if (timer == 0) {
            increasing = true;
            timerCount = getTimerCountClouds();

            
            heightRate = getCloudRange();
            thicknessRate = getCloudRange();
        }
    }
    Process::WriteFloat(ctAdd, a);
    Process::WriteFloat(chAdd, b);
}

void thickFogWeather(MenuEntry *entry) {
    static u32 timer = 0;
    static float a, b;
    float baseFog = 4.0f;
    static u32 timerCount = 30000;
    static float fogDensity;
    static float weatherSetAmount; 
    static bool bWeather = false; 
    static bool bFadingIn = false;
    static bool bFadingOut = false;
    static bool bHolding = false; 
    float weatherChance = 1.26f; 
    u32 fogAdd = 0x3C7F9C; 

    Process::ReadFloat(fogAdd, a); 

    if (bWeather) { 
        // ---- Fading In ----
        if (bFadingIn) {
            if (a < fogDensity) {
                b = a + weatherSetAmount;
                if (b > fogDensity) b = fogDensity; 
                Process::WriteFloat(fogAdd, b);
            } else {
                bFadingIn = false;
                bHolding = true; 
                timer = 0;
            }
        }

        // ---- Holding at Max Fog ----
        else if (bHolding) {
            if (timer >= timerCount) {
                bHolding = false;
                bFadingOut = true;
                timer = 0; 
            } else {
                timer++;
            }
        }

        // ---- Fading Out ----
        else if (bFadingOut) {
            if (a > baseFog) {
                b = a - weatherSetAmount;
                if (b < baseFog) b = baseFog;
                Process::WriteFloat(fogAdd, b);
            } else {
                bFadingOut = false;
                bWeather = false;
                timerCount = getTimerCountFog();
            }
        }
    }

    // ---- Decide if Weather Starts ----
    else {
        if (timer >= timerCount) {
            float chance = getChance();
            if (weatherChance > chance) {
                weatherSetAmount = getWeatherAmount();
                fogDensity = getFogDensity();

                bWeather = true;
                bFadingIn = true;
                timer = 0;
            } else {
                timer = 0;
            }
        } else {
            timer++;
        }
    }
}

u32 getSkinAddress() {
    static u32 address = 0;
    if (address == 0) {
        std::vector<u8> initialPattern {
            0xFF, 0x37, 0x37, 0x37, 0xFF, 0x37, 0x37, 0x37,
            0xFF, 0x37, 0x37, 0x37, 0xFF, 0x3F, 0x3F, 0x3F,
            0xFF, 0x37, 0x37, 0x37, 0xFF, 0x3F, 0x3F, 0x3F,
            0xFF, 0x4A, 0x4A, 0x4A
        };
    
        u32 startAddress = 0x30800000;
        u32 endAddress = 0x31000000;
        u32 size = endAddress - startAddress;
        u32 found = Utils::Search(startAddress, size, initialPattern);
        if (found == 0) {
            OSD::Notify("Initial skin pattern not found!");
            return address;
        }
    
        address = found - 0x200;
    }
    return address;
}


void getCstickMovement(MenuEntry *entry) {
    circlePosition cstick;
    hidScanInput();
    hidCstickRead(&cstick);

    int cstickX = cstick.dx;
    int cstickY = cstick.dy;

    Process::Write16(0xB329FC, cstickX);
    Process::Write16(0xB329FE, cstickY);
    Process::Write16(0xB32A14, cstickX);
    Process::Write16(0xB32A16, cstickY);
    Process::Write16(0xB32A2C, cstickX);
    Process::Write16(0xB32A2E, cstickY);
    Process::Write16(0xB32A44, cstickX);
    Process::Write16(0xB32A46, cstickY);
    Process::Write16(0xB32A5C, cstickX);
    Process::Write16(0xB32A5E, cstickY);
    Process::Write16(0xB32A74, cstickX);
    Process::Write16(0xB32A76, cstickY);
    Process::Write16(0xB32A8C, cstickX);
    Process::Write16(0xB32A8E, cstickY);
    Process::Write16(0xB32AA4, cstickX);
    Process::Write16(0xB32AA6, cstickY);
    Process::Write16(0xB32AAC, cstickX);
    Process::Write16(0xB32AAE, cstickY);

    svcSleepThread(3200000ULL); // added 3.2ms delay because sometimes menu doesn't open when pressing button combo
}

void changeSkinToCustom() {
    static u32 address = 0;
    static std::vector<u8> valToSearch;

    std::string skinsDir = FelskerSkinsDir;
    OSD::Notify(skinsDir);

    std::vector<std::string> skinFiles;
    Directory dir;
    Directory::Open(dir, skinsDir);
    dir.ListFiles(skinFiles);

    Keyboard kb1("Select a Skin:");
    kb1.Populate(skinFiles);
    int skinIndex = kb1.Open();
    if (skinIndex < 0) return;

    std::string skinName = skinFiles[skinIndex];
    std::string selectedSkin = skinsDir + "/" + skinName;

    // If no address or search pattern exists, search memory for known pattern
    if (address == 0 || valToSearch.empty()) {
        address = getSkinAddress();
    }

    // Open and write the selected skin file
    File file;
    if (File::Open(file, selectedSkin, File::READ) == 0) {
        file.Seek(0x20);
        std::vector<u8> rawBuffer(0x4000);
        file.Read(rawBuffer.data(), rawBuffer.size());

        // Write the skin data to memory
        for (size_t i = 0; i < rawBuffer.size(); i += 4) {
            u32 value = 0;
            std::memcpy(&value, &rawBuffer[i], 4);
            Process::Write32(address + i, value);
        }

        // After writing, read 0x20 bytes from file @ offset 0x220
        file.Seek(0x220);  // 0x200 + 0x20
        valToSearch.resize(0x20);
        file.Read(valToSearch.data(), 0x20);

        OSD::Notify("Skin applied successfully.");
        file.Close();
    } else {
        OSD::Notify("Failed to open skin file!");
    }
    file.Close();
}

void animateBaseSkinHead() {
    static u32 address = 0;
    static std::vector<u8> valToSearch;
    std::string animationsDir = FelskerAnimationsHeadDir;
    //OSD::Notify(animationsDir);

    //std::vector<std::string> animationFiles;
    //Directory dir;
    //Directory::Open(dir, animationsDir);
    //dir.ListFiles(animationFiles);

    //Keyboard kb1("Select an Animation:");
    //kb1.Populate(animationFiles);
    //int skinIndex = kb1.Open();
    //if (skinIndex < 0) return;

    //std::string animationName = animationFiles[skinIndex];
    //std::string selectedAnimation = animationsDir + "/" + animationName;


    std::string selectedAnimation = animationsDir + "/fireHeadAnimation.3dst";
    // If no address or search pattern exists, search memory for known pattern
    if (address == 0 || valToSearch.empty()) {
        address = getSkinAddress();
    }

    File file;
    if (File::Open(file, selectedAnimation, File::READ) == 0) {
        file.Seek(0x20);
        std::vector<u8> animBuff0(0x100);
        file.Read(animBuff0.data(), animBuff0.size());

    }
}


void animateSkinHead(MenuEntry *entry) {
    static u32 address = 0;
    static std::vector<u8> valToSearch;

    std::string selectedAnimation = FelskerAnimationsHeadDir + "/default.3dst";
    File configFile;
    std::vector<u8> myConfigBuffer(0x25);
    if (File::Open(configFile, FelskerAnimationsHeadDir + "/config.txt", File::READ) == 0) {
        configFile.Seek(0xA9);
        configFile.Read(myConfigBuffer.data(), myConfigBuffer.size());
    }
    configFile.Close();

    // Get base address once
    if (address == 0 || valToSearch.empty()) {
        address = getSkinAddress();
    }

    File file;
    if (File::Open(file, selectedAnimation, File::READ) == 0) {
        file.Seek(0x20);

        std::vector<u8> buffer(0x100);
        std::vector<u32> offsetPattern = {0x0400, 0x0500, 0x0600, 0x0700, 0x0D00, 0x0E00}; // Right, Front, Left, Back, Top, Bottom

        size_t patternIndex = 0;
        for (int i = 0; i < 32; ++i) {
            file.Seek(0x20 + i * 0x100, File::SET);
            if (file.Read(buffer.data(), buffer.size()) == buffer.size()) {
                break;
            }

            u32 newVar = address + 0x3000 + offsetPattern[patternIndex];

            // Write 0x100 bytes to newVar
            for (size_t j = 0; j < buffer.size(); j += 4) {
                u32 val = 0;
                std::memcpy(&val, &buffer[j], 4);
                Process::Write32(newVar + j, val);
            }

            patternIndex = (patternIndex + 1) % offsetPattern.size();
        }

        file.Close();
    }
}


void blinkingTest() {

}


}