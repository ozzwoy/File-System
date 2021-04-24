#ifndef FILE_SYSTEM_ENTITY_H
#define FILE_SYSTEM_ENTITY_H


class Entity {
public:
    virtual void parse(const char *bytes) = 0;
    virtual void copyBytes(char *buffer) const = 0;
    virtual void clear() = 0;
    virtual ~Entity() = default;
};


#endif //FILE_SYSTEM_ENTITY_H
