//Object IDS
enum IDS {PLAYER, HEAT, BULLET, ENERGY, ENEMY, AREA};

//Our Player
struct Gamer
{
    int ID;
    int energy;
    int score;
    int lives;
};
struct Heat
{
    int ID;
    int x;
    int y;
    int live;
    int boundx;
    int boundy;
};

struct Bullet
{
    int ID;
    int x;
    int y;
    int speed;
    bool live;
};

struct Energy
{
    int ID;
    int x;
    int y;
    int live;
    int speed;
    int boundx;
    int boundy;
};


struct Zombie
{
    int ID;
    int x;
    int y;
    bool live;
    int speed;
    int boundx;
    int boundy;
};

struct Zone
{
    int ID;
    int x;
    int y;
    int draw;
    int transfer;
};

