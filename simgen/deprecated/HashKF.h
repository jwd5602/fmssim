#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <math.h>

using namespace std;

extern const int TABLE_SIZE;

class HashKF
{
  private:
    struct pdg
    {
      int kf;
      int kc;
      //string particle;
      //string antiparticle;
      char particle[64];
      char antiparticle[64];
      int chg;
      int col;
      int anti;
      float mass;
      float width;
      float wcut;
      float lifetime;
      int decay;
    };
    int key;


    pdg * pdgtable;
    pdg pdgread;

  public:
    HashKF(const char * codesfile);
    ~HashKF();
    int Hash(int kf);
    int CheckKey(int check);
};


