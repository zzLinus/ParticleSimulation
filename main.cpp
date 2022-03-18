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

  void HandleMovement(std::vector<Particle *> particles, uint16_t size);

private:
  void iniParticleColor(uint8_t partiType);
};

class SandSimu : public olc::PixelGameEngine {
public:
  SandSimu();
  bool OnUserCreate() override;
  bool OnUserUpdate(float fElapsedTime) override;

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

void Particle::HandleMovement(std::vector<Particle *> particles,
                              uint16_t size) {
  Pos2D pDPos, pLPos, pRPos;
  uint16_t flags = 0;
  pDPos.X = this->particlePos->X;
  pDPos.Y = this->particlePos->Y + 1;
  pLPos.X = this->particlePos->X - 1;
  pLPos.Y = this->particlePos->Y + 1;
  pRPos.X = this->particlePos->X + 1;
  pRPos.Y = this->particlePos->Y + 1;
  for (int idx = 0; idx < size; idx++) {
    if (cmpPos2D(particles[idx]->particlePos, &pDPos)) {
      flags |= 1;
    }
    if (cmpPos2D(particles[idx]->particlePos, &pLPos)) {
      flags |= 2;
    }
    if (cmpPos2D(particles[idx]->particlePos, &pRPos)) {
      flags |= 4;
    }
  }
  if (this->particlePos->Y == 239 || flags == 7)
    return;
  if (flags == 1) {
    if (rand() > 0.5) {
      this->particlePos->X++;
      this->particlePos->Y++;
    } else {
      this->particlePos->X--;
      this->particlePos->Y++;
    }
  }
  if (flags == 0 || flags == 2 || flags == 4)
    this->particlePos->Y++;
  if (flags == 3) {
    this->particlePos->X++;
    this->particlePos->Y++;
  }
  if (flags == 5) {
    this->particlePos->X--;
    this->particlePos->Y++;
  }
}

void Particle::iniParticleColor(uint8_t pType) {
  if (pType == SAND) {
    this->pColor->R = 0xff;
    this->pColor->G = 0xbc;
    this->pColor->B = 0x00;
  } else if (pType == WATER) {
    this->pColor->R = 0x00;
    this->pColor->G = 0x5f;
    this->pColor->B = 0xe3;
  } else {
    this->pColor->R = 0x84;
    this->pColor->G = 0x7e;
    this->pColor->B = 0x87;
  }
}

Particle::~Particle() {}

SandSimu::SandSimu() { sAppName = "SandSimu"; }

bool SandSimu::OnUserCreate() {
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
    particles.push_back(p);
  } else if (GetMouse(1).bHeld) {
    this->mousePos->X = uint16_t(GetMouseX());
    this->mousePos->Y = uint16_t(GetMouseY());
    Particle *p = new Particle(WATER, mousePos->X, mousePos->Y);
    particles.push_back(p);
  } else if (GetMouse(2).bHeld) {
    particles.clear();
  }

  // for (int pIdx = 0; pIdx < particles.size(); pIdx++)
  for (Particle *p : particles) {
    p->HandleMovement(particles, particles.size());
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
