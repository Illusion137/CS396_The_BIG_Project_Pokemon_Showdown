#ifndef ITEM_H
#define ITEM_H

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

};

#endif // ITEM_H