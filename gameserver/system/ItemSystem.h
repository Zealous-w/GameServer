#ifndef ITEM_SYSTEM_H
#define ITEM_SYSTEM_H
#include <player.h>
#include <base/basic.h>

DEFINE_CLASS(ItemSystem)
public:
    bool InitService();
public:
    void AddItem(Player* player, uint32 itemId, uint32 size);
    void RemoveItem(Player* player, uint32 itemId, uint32 size);
    uint32 GetItemSizeById(uint32 itemId);
DEFINE_CLASS_END
#endif