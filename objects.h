// Objetos IDS
enum IDS{ELECTRONICS, BULLETS, ZOMBIES, ENERGIA};

// Tipos de poderes
enum PODERES{CALOR, MAGNETISMO, ELETRONS, LUZ, ROTACAO, BARREIRA, AMPLIFICACAO, ARMAZENAMENTO, REDIRECIONAMENTO};


struct Tiros
{
    int ID; // Tipo de objeto
    int x; // Posiçao X
    int y; // Posiçao Y
    bool live; // Define quando o tiro é acionado
    int PODER; // Tipo de poder do objeto
    int forca_tiro; // Força do tiro
    int speed; // Velocidade do tiro

};

struct Electronics
{
    int ID; // Tipo de objeto
    int x; // Posiçao X
    int y; // Posiçao Y
    int life; // Vida do objeto
    bool live; // Define quando o Electronic ta vivo
    int boundx; // Colisao X
    int boundy; // Colisao Y
    struct Tiros tiro;
};

struct Zombies
{
    int ID; // Tipo de objeto
    int x; // Posiçao X
    int y; // Posiçao Y
    bool live; // Define quando um novo Zombie surge
    int life;
    float speed; // Velocidade do Zombie
    float boundx; // Colisao X
    int boundy; // Colisao Y

};

struct Energia
{
    int ID;
    int x;
    int y;
    bool live;
    int speed;
    int boundx;
    int boundy;
};
