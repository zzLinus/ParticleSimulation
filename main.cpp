#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <cstdint>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <vector>

// Struct declaration
typedef struct Pos2D Pos2D;
typedef struct ParticleColor ParticleColor;
enum { SAND, WATER, STONE };

// Classes declaration
class Particle {
public:
  Particle(uint8_t particleType, uint16_t X, uint16_t Y);
  virtual ~Particle();

  Pos2D *particlePos;
  ParticleColor *pColor;
  uint8_t particleType;

  void HandleMovement(uint8_t **particleTable);

private:
  void iniParticleColor(uint8_t partiType);
};

class SandSimu : public olc::PixelGameEngine {
public:
  SandSimu();
  bool OnUserCreate() override;
  bool OnUserUpdate(float fElapsedTime) override;
  uint8_t **particleTable;

public:
  std::vector<Particle *> particles;

private:
  Pos2D *mousePos;
};

// Struct definition
struct Pos2D {
  uint16_t X;
  uint16_t Y;
};

struct ParticleColor {
  uint8_t R;
  uint8_t G;
  uint8_t B;
};

// Classes definition
Particle::Particle(uint8_t particleType, uint16_t X, uint16_t Y) {
  this->pColor = new ParticleColor;
  this->particlePos = new Pos2D;
  particlePos->X = X;
  particlePos->Y = Y;
  iniParticleColor(particleType);
}

bool cmpPos2D(Pos2D *p1, Pos2D *p2) {
  if (p1->X != p2->X)
    return false;
  if (p1->Y != p2->Y)
    return false;
  return true;
}

void Particle::HandleMovement(uint8_t **particleTable) {
  Pos2D pDPos, pLPos, pRPos;
  uint16_t flags = 0;
  pDPos.X = this->particlePos->X;
  pDPos.Y = this->particlePos->Y + 1;
  pLPos.X = this->particlePos->X - 1;
  pLPos.Y = this->particlePos->Y + 1;
  pRPos.X = this->particlePos->X + 1;
  pRPos.Y = this->particlePos->Y + 1;
  if (this->particlePos->Y == 239)
    return;

  if (particleType == SAND) {
    if (particleTable[pDPos.Y][pDPos.X] == SAND)
      flags |= 1;
    if (particleTable[pLPos.Y][pLPos.X] == SAND)
      flags |= 2;
    if (particleTable[pRPos.Y][pRPos.X] == SAND)
      flags |= 4;

    if (flags == 7)
      return;
    if (flags == 1) {
      if (rand() > 0.5) {
        particleTable[this->particlePos->Y][this->particlePos->X] = 10;
        this->particlePos->X++;
        this->particlePos->Y++;
        particleTable[this->particlePos->Y][this->particlePos->X] =
            particleType;
      } else {
        particleTable[this->particlePos->Y][this->particlePos->X] = 10;
        this->particlePos->X--;
        this->particlePos->Y++;
        particleTable[this->particlePos->Y][this->particlePos->X] =
            particleType;
      }
    }
    if (flags == 0 || flags == 2 || flags == 4) {
      particleTable[this->particlePos->Y][this->particlePos->X] = 10;
      this->particlePos->Y++;
      particleTable[this->particlePos->Y][this->particlePos->X] = particleType;
    }
    if (flags == 3) {
      particleTable[this->particlePos->Y][this->particlePos->X] = 10;
      this->particlePos->X++;
      this->particlePos->Y++;
      particleTable[this->particlePos->Y][this->particlePos->X] = particleType;
    }
    if (flags == 5) {
      particleTable[this->particlePos->Y][this->particlePos->X] = 10;
      this->particlePos->X--;
      this->particlePos->Y++;
      particleTable[this->particlePos->Y][this->particlePos->X] = particleType;
    }
  }
}

void Particle::iniParticleColor(uint8_t pType) {
  if (pType == SAND) {
    particleType = SAND;
    this->pColor->R = 0xff;
    this->pColor->G = 0xbc;
    this->pColor->B = 0x00;
  } else if (pType == WATER) {
    particleType = WATER;
    this->pColor->R = 0x00;
    this->pColor->G = 0x5f;
    this->pColor->B = 0xe3;
  } else {
    particleType = STONE;
    this->pColor->R = 0x84;
    this->pColor->G = 0x7e;
    this->pColor->B = 0x87;
  }
}

Particle::~Particle() {}

SandSimu::SandSimu() { sAppName = "SandSimu"; }

bool SandSimu::OnUserCreate() {

  particleTable = (uint8_t **)std::malloc(sizeof(uint8_t *) * ScreenHeight());
  for (int i = 0; i < ScreenHeight(); i++) {
    particleTable[i] = (uint8_t *)std::malloc(sizeof(uint8_t) * ScreenWidth());
  }
  for (int row = 0; row < ScreenWidth(); row++)
    for (int col = 0; col < ScreenHeight(); col++)
      particleTable[col][row] = 10;
  this->mousePos = new Pos2D;
  return true;
}

bool SandSimu::OnUserUpdate(float fElapsedTime) {
  // Get mouse position if left botton is bhold
  Clear(olc::BLACK);
  if (GetMouse(0).bHeld) {
    this->mousePos->X = uint16_t(GetMouseX());
    this->mousePos->Y = uint16_t(GetMouseY());
    Particle *p = new Particle(SAND, mousePos->X, mousePos->Y);
    particleTable[mousePos->Y][mousePos->X] = SAND;
    particles.push_back(p);
  } else if (GetMouse(1).bHeld) {
    this->mousePos->X = uint16_t(GetMouseX());
    this->mousePos->Y = uint16_t(GetMouseY());
    Particle *p = new Particle(WATER, mousePos->X, mousePos->Y);
    particleTable[mousePos->Y][mousePos->X] = WATER;
    particles.push_back(p);
  } else if (GetMouse(2).bHeld) {
    for (int col = 0; col < ScreenHeight(); col++)
      for (int row = 0; row < ScreenWidth(); row++)
        *(*(particleTable + col) + row) = 10;
    particles.clear();
  }

  // for (int pIdx = 0; pIdx < particles.size(); pIdx++)
  for (Particle *p : particles) {
    p->HandleMovement(particleTable);
    Draw(p->particlePos->X, p->particlePos->Y,
         olc::Pixel(p->pColor->R, p->pColor->G, p->pColor->B));
  }

  return true;
}

int main(int argc, char *argv[]) {
  SandSimu game;
  if (game.Construct(256, 240, 4, 4))
    game.Start();

  return 0;
}
