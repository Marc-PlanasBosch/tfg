#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Dummy


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */

  typedef vector<int> VE;
  typedef vector<Dir> VD;
  typedef vector<VE> VVE;

  typedef queue<Pos>  QP;

  void codi_comu(const VE& V, QP& Q, VVE& T, bool knights) {
    if (Q.empty()) return;

    int s = 2;
    do {
      QP A;
      do {
        Pos q = Q.front(); Q.pop();
        for (int d = 0; d < 8; d += 2 - knights) {
          Pos p = q + Dir(d);
          if (not T[p.i][p.j]) {
            T[p.i][p.j] = s;
            A.push(p);
          }
        }
      } while (not Q.empty());
      swap(Q, A);
      ++s;
    } while (not Q.empty());

    for (int k = 0; k < (int)V.size(); ++k) {
      Pos q = unit(V[k]).pos;
      int minim = 1e9;
      VE D;
      for (int d = 0; d < 8; d += 2 - knights) {
        Pos p = q + Dir(d);
        int c = T[p.i][p.j];
        if (c < minim) {
          minim = c;
          D = VE(1, d);
        }
        else if (c == minim) D.push_back(d);
      }
      if (not D.empty())
        command(V[k], Dir(D[random(0, D.size() - 1)]));
    }
  }

  void mou_bruixes() {
    VVE T(rows(), VE(cols(), 0));
    QP Q;
    for (int i = 0; i < rows(); ++i)
      for (int j = 0; j < cols(); ++j) {
        Cell c = cell(i, j);
        if (c.type == Wall) T[i][j] = 1e9;
        else if (c.id != -1) {
          if (unit(c.id).player == 0 or unit(c.id).type == Witch)
            T[i][j] = 1e9;
          else {
            T[i][j] = 1;
            Q.push(Pos(i, j));
          }
        }
      }
    codi_comu(witches(0), Q, T, false);
  }

  void mou_pagesos() {
    VVE T(rows(), VE(cols(), 0));
    QP Q;
    for (int i = 0; i < rows(); ++i)
      for (int j = 0; j < cols(); ++j) {
        Cell c = cell(i, j);
        if (c.type == Wall or c.id != -1 or c.haunted) T[i][j] = 1e9;
        else if (c.owner != 0) {
          T[i][j] = 1;
          Q.push(Pos(i, j));
        }
      }
    codi_comu(farmers(0), Q, T, false);
  }

  void mou_cavallers() {
    VVE T(rows(), VE(cols(), 0));
    QP Q;
    for (int i = 0; i < rows(); ++i)
      for (int j = 0; j < cols(); ++j) {
        Cell c = cell(i, j);
        if (c.type == Wall) T[i][j] = 1e9;
        else if (c.id != -1) {
          if (unit(c.id).player == 0 or unit(c.id).type == Witch or c.haunted)
            T[i][j] = 1e9;
          else {
            T[i][j] = 1;
            Q.push(Pos(i, j));
          }
        }
      }
    codi_comu(knights(0), Q, T, true);
  }

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    mou_bruixes();
    mou_pagesos();
    mou_cavallers();
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
