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
class LiquidParticle;
class Particle;
enum { SAND, WATER, STONE };
union ParticlePointer {
  LiquidParticle *lp;
  Particle *p;
};

// ==>Particle Parent Class(the solid particles)
class Particle {
public:
  Particle(uint8_t particleType, uint16_t X, uint16_t Y);
  virtual ~Particle();

  Pos2D *particlePos;
  ParticleColor *pColor;
  uint8_t particleType;

  virtual void HandleMovement(uint8_t **particleTable);

private:
  void iniParticleColor(uint8_t partiType);
};

// ==>LiquidParticle Class
class LiquidParticle : public Particle {
public:
  LiquidParticle(uint8_t particleType, uint16_t X, uint16_t Y);
  virtual void HandleMovement(uint8_t **particleTable);
  uint8_t floatDir;

private:
};

class SandSimu : public olc::PixelGameEngine {
public:
  SandSimu();
  bool OnUserCreate() override;
  bool OnUserUpdate(float fElapsedTime) override;
  uint8_t **particleTable;
  ParticlePointer **particlePointerTable;

public:
  std::vector<Particle *> particles;
  std::vector<LiquidParticle *> liquidParticles;

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

LiquidParticle::LiquidParticle(uint8_t particleType, uint16_t X, uint16_t Y)
    : Particle(particleType, X, Y) {
  floatDir = 255;
};

void LiquidParticle::HandleMovement(uint8_t **particleTable) {
  Pos2D aroundPos[8];
  aroundPos[0].X = particlePos->X - 1;
  aroundPos[0].Y = particlePos->Y - 1;
  aroundPos[1].X = particlePos->X;
  aroundPos[1].Y = particlePos->Y - 1;
  aroundPos[2].X = particlePos->X + 1;
  aroundPos[2].Y = particlePos->Y - 1;
  aroundPos[3].X = particlePos->X + 1;
  aroundPos[3].Y = particlePos->Y;
  aroundPos[4].X = particlePos->X + 1;
  aroundPos[4].Y = particlePos->Y + 1;
  aroundPos[5].X = particlePos->X;
  aroundPos[5].Y = particlePos->Y + 1;
  aroundPos[6].X = particlePos->X - 1;
  aroundPos[6].Y = particlePos->Y + 1;
  aroundPos[7].X = particlePos->X - 1;
  aroundPos[7].Y = particlePos->Y;
  uint16_t flags = 0;
  if (this->particlePos->Y == 239)
    return;

  if (particleType == WATER) {
    for (int idx = 0; idx < 8; idx++)
      if (particleTable[aroundPos[idx].Y][aroundPos[idx].X] == WATER)
        flags |= (1 << idx);
      else if (particleTable[aroundPos[idx].Y][aroundPos[idx].X] == SAND)
        flags |= (1 << (idx + 8));
    // < -1
    // > 1
    //

    if ((flags & 0x2020) == 0x00) {
      particleTable[this->particlePos->Y][this->particlePos->X] = 10;
      this->particlePos->Y++;
      particleTable[this->particlePos->Y][this->particlePos->X] = particleType;
    } else if ((flags & 0x1010) == 0x00) {
      particleTable[this->particlePos->Y][this->particlePos->X] = 10;
      this->particlePos->X++;
      this->particlePos->Y++;
      particleTable[this->particlePos->Y][this->particlePos->X] = particleType;
    } else if ((flags & 0x4040) == 0x00) {
      particleTable[this->particlePos->Y][this->particlePos->X] = 10;
      this->particlePos->X--;
      this->particlePos->Y++;
      particleTable[this->particlePos->Y][this->particlePos->X] = particleType;
    } else if ((flags & 0x8080) == 0x00 && (flags & 0x0808) != 0x00) {
      particleTable[this->particlePos->Y][this->particlePos->X] = 10;
      this->particlePos->X--;
      particleTable[this->particlePos->Y][this->particlePos->X] = particleType;
    } else if ((flags & 0x0808) == 0x00 && (flags & 0x8080) != 0x00) {
      particleTable[this->particlePos->Y][this->particlePos->X] = 10;
      this->particlePos->X++;
      particleTable[this->particlePos->Y][this->particlePos->X] = particleType;
    }
  }
}

bool cmpPos2D(Pos2D *p1, Pos2D *p2) {
  if (p1->X != p2->X)
    return false;
  if (p1->Y != p2->Y)
    return false;
  return true;
}

void Particle::HandleMovement(uint8_t **particleTable) {
  Pos2D aroundPos[8];
  aroundPos[0].X = particlePos->X - 1;
  aroundPos[0].Y = particlePos->Y - 1;
  aroundPos[1].X = particlePos->X;
  aroundPos[1].Y = particlePos->Y - 1;
  aroundPos[2].X = particlePos->X + 1;
  aroundPos[2].Y = particlePos->Y - 1;
  aroundPos[3].X = particlePos->X + 1;
  aroundPos[3].Y = particlePos->Y;
  aroundPos[4].X = particlePos->X + 1;
  aroundPos[4].Y = particlePos->Y + 1;
  aroundPos[5].X = particlePos->X;
  aroundPos[5].Y = particlePos->Y + 1;
  aroundPos[6].X = particlePos->X - 1;
  aroundPos[6].Y = particlePos->Y + 1;
  aroundPos[7].X = particlePos->X - 1;
  aroundPos[7].Y = particlePos->Y;
  uint16_t flags = 0;
  if (this->particlePos->Y == 239)
    return;

  if (particleType == SAND) {
    for (int idx = 0; idx < 8; idx++)
      if (particleTable[aroundPos[idx].Y][aroundPos[idx].X] == WATER)
        flags |= (1 << idx);
      else if (particleTable[aroundPos[idx].Y][aroundPos[idx].X] == SAND)
        flags |= (1 << (idx + 8));
    // < -1
    // > 1
    //

    if ((flags & 0x2020) == 0x00) {
      particleTable[this->particlePos->Y][this->particlePos->X] = 10;
      this->particlePos->Y++;
      particleTable[this->particlePos->Y][this->particlePos->X] = particleType;
      return;
    } else if ((flags & 0x1010) == 0x00) {
      if ((flags & 0x4040) == 0x00)

        if (rand() > 0.5)
          goto LEFT;
        else
          goto RIGHT;

      else
        goto RIGHT;

    LEFT:
      particleTable[this->particlePos->Y][this->particlePos->X] = 10;
      this->particlePos->X--;
      this->particlePos->Y++;
      particleTable[this->particlePos->Y][this->particlePos->X] = particleType;
      return;
    RIGHT:
      particleTable[this->particlePos->Y][this->particlePos->X] = 10;
      this->particlePos->X++;
      this->particlePos->Y++;
      particleTable[this->particlePos->Y][this->particlePos->X] = particleType;
      return;
    } else if ((flags & 0x4040) == 0x00) {
      if ((flags & 0x1010) == 0x00)

        if (rand() > 0.5)
          goto LEFT;
        else
          goto RIGHT;

      else
        goto LEFT;
    } else {
      return;
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
  particlePointerTable = (ParticlePointer **)std::malloc(
      sizeof(ParticlePointer *) * ScreenHeight());
  for (int i = 0; i < ScreenHeight(); i++) {
    particleTable[i] = (uint8_t *)std::malloc(sizeof(uint8_t) * ScreenWidth());
    particlePointerTable[i] =
        (ParticlePointer *)std::malloc(sizeof(ParticlePointer) * ScreenWidth());
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
    particlePointerTable[mousePos->Y][mousePos->X].p = p;
    particles.push_back(p);
  } else if (GetMouse(1).bHeld) {
    this->mousePos->X = uint16_t(GetMouseX());
    this->mousePos->Y = uint16_t(GetMouseY());
    LiquidParticle *p = new LiquidParticle(WATER, mousePos->X, mousePos->Y);
    particleTable[mousePos->Y][mousePos->X] = WATER;
    particlePointerTable[mousePos->Y][mousePos->X].lp = p;
    liquidParticles.push_back(p);
  } else if (GetMouse(2).bHeld) {
    for (int col = 0; col < ScreenHeight(); col++)
      for (int row = 0; row < ScreenWidth(); row++)
        *(*(particleTable + col) + row) = 10;
    particles.clear();
    liquidParticles.clear();
  }
  for (LiquidParticle *p : liquidParticles) {
    p->HandleMovement(particleTable);
    Draw(p->particlePos->X, p->particlePos->Y,
         olc::Pixel(p->pColor->R, p->pColor->G, p->pColor->B));
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
