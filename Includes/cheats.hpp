#ifndef CHEATS_H
#define CHEATS_H

#include <CTRPluginFramework.hpp>
#include "Helpers.hpp"
#include "Unicode.h"

namespace CTRPluginFramework
{
    using StringVector = std::vector<std::string>;
    void dropEverything(MenuEntry *entry);
    void ninetyFov(MenuEntry *entry);
    void defaultCodes();
    void removeMobCap(MenuEntry *entry);
    void itemExpLimit(MenuEntry *entry);
    void enhancedParticles(MenuEntry *entry);
    void betterMinecartPhysics(MenuEntry *entry);
    void stopMobSpawns(MenuEntry *entry);
    void removeHeads(MenuEntry *entry);
    void everythingSpinny(MenuEntry *entry);
    bool displayPlayerCoordsTopScreen(const Screen& screen);
    void ModifySignedIntFromFile(const std::string& filename, std::streampos offset);
    void moveBodyPart(MenuEntry *entry);
    void backupWorld();
    void selectAndModifyOffset();
    int getOffset(const std::string &key);
    void creativeMode(MenuEntry *entry);
    void checkAndCreateDirectories();
    void initializePaths();
    void dumpStriptExecutable();
    void sha256_hash(const uint8_t *data, size_t length, uint8_t *hash);
    void dumpExecutable();
    void mobController();
    void dynaClouds(MenuEntry *entry);
    void thickFogWeather(MenuEntry *entry);
    void getCstickMovement(MenuEntry *entry);
    void displayMegapackVersion();
    void changeSkinToCustom();
    void animateSkinHead(MenuEntry *entry);
    std::vector<std::string> list3dstFiles(const std::string& path);
    class Player;
    bool IsSwimming();
    int GetSlotNumber();
    int GetItemCount();
    bool IsFlying();
    bool IsJumping();
    bool IsIdle();
}
#endif
