// Objetos IDS
enum IDS{ELECTRONICS, BULLETS, ZOMBIES};

// Tipos de poderes
enum PODERES{CALOR, MAGNETISMO, ELETRONS, LUZ, ROTACAO, BARREIRA, AMPLIFICACAO, ARMAZENAMENTO, REDIRECIONAMENTO};


struct Electronics
{
    int ID; // Tipo de objeto
    int x; // Posiçao X
    int y; // Posiçao Y
    int life; // Vida do objeto
    int boundx; //Colisao X
    int boundy; // Colisao Y
};

struct Tiros
{
    int ID; // Tipo de objeto
    int x; // Posiçao X
    int y; // Posiçao Y
    bool live; // Define quando o tiro é acionado
    int PODER; // Tipo de poder do objeto
    int forca_tiro; // Força do tiro
    int boundx; // Colisao X
    int boundy; // Colisao Y
    int speed; // Velocidade do tiro

};

struct ZOMBIES
{
    int ID; // Tipo de objeto
    int x; // Posiçao X
    int y; // Posiçao Y
    bool live; // Define quando um novo Zombie surge
    int speed; // Velocidade do Zombie
    int boundx; // Colisao X
    int boundy; // Colisao Y

};
