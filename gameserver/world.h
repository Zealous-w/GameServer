#ifndef WORLD_H
#define WORLD_H

class World {
public:
    static World& getInstance() {
        static World w;
        return w;
    }
private:
    World();
    ~World();
public:

};

#define gWorld World::getInstance() 

#endif