#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <unordered_map>

class ItemBase {
public:
    ItemBase(const char *name, const char *description):
        name_(name), description_(description){}
    const char *name() const noexcept { return this->name_; }
    const char *description() const noexcept { return this->description_; }
private:
    const char *name_;
    const char *description_;
};

class Item {
public:
    Item(ItemBase info): info_(info) {}
private:
    ItemBase info_;
};

extern std::unordered_map<std::string, ItemBase> item_from_string;

#endif // ITEM_H