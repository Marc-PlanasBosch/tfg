#include "Player.hh"


#define PLAYER_NAME Dummy


struct PLAYER_NAME : public Player {

  static Player* factory () {
    return new PLAYER_NAME;
  }
  int max_dist_neg = 4;
  int max_dist = 7;
  double INF = 1e200;
  double used_time = 0;
  int rango_expansion = 100;

  typedef long double ld;
  typedef vector<ld> vld;
  typedef vector<bool> vb;
  typedef vector<vb> vvb;
  typedef vector<int> vi;
  typedef pair<int, Pos> iPos;
  typedef queue<iPos> qiPos;
  typedef vector<Dir> vDir;
  typedef vector<vDir> vvDir;
  typedef vector<Pos> vPos;
  typedef map<Unit, Pos> mUnitPos;
  typedef pair<Pos, Pos> PosPos;
  typedef pair<int, Dir> iDir;
  typedef map<PosPos, iDir> distancia;
  typedef set<Pos> sPos;
  typedef queue<Pos> qPos;
  typedef pair<iDir, Pos> iDirPos;
  typedef queue<iDirPos> qiDirPos;

  vPos farmer_clave;
  vPos knight_clave;
  vPos witch_clave;
  distancia predet4;
  distancia predet8;
  ld pot[21];
  ld potter[21];
  Dir dir_todas[9];
  Dir dir_basicas[5];
  Dir dir_avanzadas[4];
  Dir dir_todasmov[8];
  Dir dir_basmov[4];

  void movimiento_expansivo(int id) {
    Unit unidad = unit(id);
    if (unidad.type == Farmer) {
      iDir mejor = iDir(1000, None);
      for (Pos u : farmer_clave) {
        iDir idir = predet4[PosPos(u, unidad.pos)];
        if (idir.first < mejor.first) mejor = idir;
      }
      command(id, mejor.second);
    }
    if (unidad.type == Knight) {
      iDir mejor = iDir(1000, None);
      for (Pos u : knight_clave) {
        iDir idir = predet8[PosPos(u, unidad.pos)];
        if (idir.first < mejor.first) mejor = idir;
      }
      command(id, mejor.second);
    }
    if (unidad.type == Witch) {
      iDir mejor = iDir(1000, None);
      for (Pos u : witch_clave) {
        iDir idir = predet4[PosPos(u, unidad.pos)];
        if (idir.first < mejor.first) mejor = idir;
      }
      command(id, mejor.second);
    }
  }

  bool dentro(Pos u) {
    if (u.i > 0 and u.j > 0) {
      if (u.i < rows() and u.j < cols()) return true;
    }
    return false;
  }

  Pos nonwall(Pos u) {
    sPos visitado;
    qPos q;
    q.push(u);
    while (cell(q.front()).type == Wall) {
      Pos v = q.front();
      q.pop();
      if (visitado.find(v) != visitado.end()) continue;
      visitado.insert(v);
      for (Dir d : dir_basmov) {
        Pos w = v + d;
        if (dentro(w)) q.push(w);
      }
    }
    return q.front();
  }

  Dir dir_opuesta(Dir op) {
    if (op == None) return None;
    return Dir((int(op) + 4) % 8);
  }

  void imprime_dir(Dir dir) {
    if (dir == None) cerr << "None";
    if (dir == Top) cerr << "Top";
    if (dir == Bottom) cerr << "Bottom";
    if (dir == Left) cerr << "Left";
    if (dir == Right) cerr << "Right";
    if (dir == RT) cerr << "RT";
    if (dir == TL) cerr << "TL";
    if (dir == LB) cerr << "LB";
    if (dir == BR) cerr << "BR";
  }

  void bfs_pre(qiDirPos& q, Pos origen, bool diagonal) {

    if (q.empty()) return;
    iDir aux = q.front().first;
    Pos u = q.front().second;
    q.pop();
    int dist = aux.first;
    PosPos pareja = PosPos(origen, u);
    Dir pd = dir_opuesta(aux.second);

    if (not dentro(u)) {
      bfs_pre(q, origen, diagonal);
      return;
    }
    if (predet4.find(pareja) != predet4.end() and (not diagonal)) {
      bfs_pre(q, origen, diagonal);
      return;
    }
    if (predet8.find(pareja) != predet8.end() and diagonal) {
      bfs_pre(q, origen, diagonal);
      return;
    }
    if (cell(u).type == Wall) {
      bfs_pre(q, origen, diagonal);
      return;
    }
    if (not diagonal) predet4[pareja] = iDir(dist, pd);
    else predet8[pareja] = iDir(dist, pd);
    if (not diagonal) {
      for (Dir d : dir_basmov) {
        Pos v = u + d;
        iDir idir = iDir(dist + 1, d);
        q.push(iDirPos(idir, v));
      }
    }
    else {
      for (Dir d : dir_todasmov) {
        Pos v = u + d;
        iDir idir = iDir(dist + 1, d);
        q.push(iDirPos(idir, v));
      }
    }
    bfs_pre(q, origen, diagonal);
    return;
  }


  void precalculos() {

    vi aux = random_permutation(9);
    for (int i = 0; i < 9; ++i) {
      dir_todas[i] = Dir(aux[i]);
    }
    aux = random_permutation(5);
    for (int i = 0; i < 5; ++i) {
      dir_basicas[i] = Dir(2 * aux[i]);
    }
    aux = random_permutation(4);
    for (int i = 0; i < 4; ++i) {
      dir_avanzadas[i] = Dir(2 * aux[i] + 1);
    }
    aux = random_permutation(4);
    for (int i = 0; i < 4; ++i) {
      dir_basmov[i] = Dir(2 * aux[i]);
    }
    aux = random_permutation(8);
    for (int i = 0; i < 8; ++i) {
      dir_todasmov[i] = Dir(aux[i]);
    }

    for (int i = 0; i < 21; ++i) {
      pot[i] = pow(2, i);
      potter[i] = pow(1.5, i);
    }

    farmer_clave.push_back(nonwall(Pos(rows() / 2, cols() - 5)));
    farmer_clave.push_back(nonwall(Pos(rows() - 5, cols() / 2)));
    knight_clave = farmer_clave;
    witch_clave.push_back(nonwall(Pos(rows() - 5, 5)));
    witch_clave.push_back(nonwall(Pos(5, cols() - 5)));

    for (Pos u : farmer_clave) {
      qiDirPos q;
      iDir idir = iDir(0, None);
      q.push(iDirPos(idir, u));
      bfs_pre(q, u, false);
    }
    for (Pos u : knight_clave) {
      qiDirPos q;
      iDir idir = iDir(0, None);
      q.push(iDirPos(idir, u));
      bfs_pre(q, u, true);
    }
    for (Pos u : witch_clave) {
      qiDirPos q;
      iDir idir = iDir(0, None);
      q.push(iDirPos(idir, u));
      bfs_pre(q, u, false);
    }
  }


  ld valor_farmer(Pos u, int dist) {
    int id = cell(u).id;
    if (cell(u).type == Wall) return 0;
    if (id != -1) return 0;
    if (cell(u).owner == 0) return 0;
    else if (cell(u).owner == -1) return pot[20 - dist];
    else return 2 * pot[20 - dist];
  }

  ld valor_knight(Pos u, int dist) {
    int id = cell(u).id;
    if (cell(u).type == Wall) return 0;
    if (id != -1) {
      Unit pieza = unit(id);
      if (pieza.type == Knight and pieza.player != 0) {
        if (dist == 0) return 1e10;
        else return 100;
      }
      if (pieza.type == Farmer and pieza.player != 0) {
        if (dist == 0) return 1e10;
        else return pot[20 - dist];
      }
      return 0;
    }
    else if (cell(u).owner > 0) return 0;
    else return 0;
  }

  ld valor_witch(Pos u, int dist) {
    int id = cell(u).id;
    if (cell(u).type == Wall) return 0;
    if (id != -1) {
      Unit pieza = unit(id);
      if (pieza.type == Knight and pieza.player != 0) {
        return 3 * potter[20 - dist];
      }
      if (pieza.type == Farmer and pieza.player != 0) {
        return potter[20 - dist];
      }
      return 0;
    }
    if (cell(u).owner > 0) return 0;
    if (cell(u).owner > 0) return 0;
    return 0;
  }

  ld valor(Pos u, int dist, UnitType tipo) {
    if (tipo == Farmer) return valor_farmer(u, dist);
    if (tipo == Knight) return valor_knight(u, dist);
    if (tipo == Witch) return valor_witch(u, dist);
    return 0;
  }

  ld valor_neg(Pos u, int dist, UnitType tipo) {
    Unit enemy = unit(cell(u).id);
    bool amigo = (enemy.player == 0);
    if (tipo == Farmer) {
      if (dist > 4) return 0;
      if (enemy.type == Farmer and amigo) return 0;
      if (enemy.type == Knight and !amigo) return 0;
      if (enemy.type == Knight and amigo) return 100;
      if (enemy.type == Witch) return - pot[20 - dist] * 1e14;
    }
    if (tipo == Knight) {
      if (dist > 4) return 0;
      if (enemy.type == Farmer) return 0;
      if (enemy.type == Knight and !amigo) return 0;
      if (enemy.type == Knight and amigo) return -100;
      if (enemy.type == Witch) return - pot[20 - dist] * 1e14;
    }
    if (tipo == Witch) {
      if (enemy.type == Farmer) return 0;
      if (enemy.type == Knight) return 0;
      if (enemy.type == Witch and !amigo) return 0;
      if (enemy.type == Witch and amigo) return -100 * pot[20 - dist];
    }
    return 0;
  }


  ld valor_quieto(int id) {
    Unit pieza = unit(id);
    if (pieza.type == Farmer) {
      if(pieza.health < farmers_health()) return 1.5 * pot[18]; // TOCAR AIXO
      else return 0;
    }
    if (pieza.type == Knight) {
      if (pieza.health < knights_health()) return 1.5 * pot[17]; // TOCAR AIXO
      else return 0;
    }
    if (pieza.type == Witch) {
      return 0;
    }
    return 0;
  }

  ld bfs_pos(qiPos& q, vvb& visitados, UnitType tipo) {
    if (q.empty()) return 0;
    int dist = q.front().first;
    Pos u = q.front().second;
    q.pop();
    if (not dentro(u)) {
      return bfs_pos(q, visitados, tipo);
    }
    if (cell(u).type == Wall) {
      return valor(u, dist, tipo) + bfs_pos(q, visitados, tipo);
    }
    if (visitados[u.i][u.j]) {
      return bfs_pos(q, visitados, tipo);
    }
    visitados[u.i][u.j] = true;
    if (dist > max_dist) return 0;
    if (cell(u).id != -1) {
      return valor(u, dist, tipo) + bfs_pos(q, visitados, tipo);
    }
    if (tipo == Knight) {
      for (Dir d : dir_todasmov) {
        q.push(iPos(dist + 1, u + d));
      }
    }
    else {
      for (Dir d : dir_basmov) {
        q.push(iPos(dist + 1, u + d));
      }
    }
    return valor(u, dist, tipo) + bfs_pos(q, visitados, tipo);
  }

  ld bfs_neg(qiPos& q, sPos& visitados, UnitType tipo) {
    if (q.empty()) return 0;
    int dist = q.front().first;
    Pos u = q.front().second;
    q.pop();
    if (not dentro(u)) {
      return bfs_neg(q, visitados, tipo);
    }
    if (cell(u).type == Wall) {
      return bfs_neg(q, visitados, tipo);
    }
    if (visitados.find(u) != visitados.end()) {
      return bfs_neg(q, visitados, tipo);
    }
    visitados.insert(u);
    if (dist > max_dist_neg) return 0;
    if (tipo == Knight) {
      for (Dir d : dir_todasmov) {
        q.push(iPos(dist + 1, u + d));
      }
    }
    else {
      for (Dir d : dir_basmov) {
        q.push(iPos(dist + 1, u + d));
      }
    }
    if (cell(u).id != -1) {
      return valor_neg(u, dist, tipo) + bfs_neg(q, visitados, tipo);
    }
    else return bfs_neg(q, visitados, tipo);
  }

  void play () {
    if (round() == 0) precalculos();
    vi f = farmers(0), k = knights(0), w = witches(0);
    //~ ld r_gastado = ld(round()) / nb_rounds();
    //~ if (round() % 25 == 24 and 0.9 * status(0) > r_gastado) max_dist--;

    for (int id : f) {
      Pos v = unit(id).pos;
      vld valor_dir(9);
      for (int dirr = 1; dirr < 8; dirr += 2) {
        valor_dir[dirr] = -INF;
      }
      for (int dirr = 0; dirr <= 8; dirr += 2) {
        Dir pd = Dir(dirr);
        Cell obj = cell(v + pd);
        if (obj.type == Wall) valor_dir[dirr] = -INF;
        else if (obj.id != - 1) valor_dir[dirr] = -1e50;
        else {
          vvb visitados(rows(), vb(cols(), false));
          sPos visitados_n;
          visitados_n.clear();
          qiPos q, q_n;
          q.push(iPos(0, v + pd));
          q_n.push(iPos(0, v + pd));
          if (pd == None) {
            valor_dir[dirr] = valor_quieto(id) + bfs_pos(q, visitados, Farmer)
              + bfs_neg(q_n, visitados_n, Farmer);
          }
          else valor_dir[dirr] = bfs_pos(q, visitados, Farmer)
            + bfs_neg(q_n, visitados_n, Farmer);
        }
      }
      double mejor_valor = -INF;
      double peor_valor = INF;
      Dir mejor_dir = None;
      for (int dirr = 0; dirr <= 8; ++dirr) {
        if (valor_dir[dirr] > mejor_valor) {
          mejor_valor = valor_dir[dirr];
          mejor_dir = Dir(dirr);
        }
        if (valor_dir[dirr] != -INF) {
          if (valor_dir[dirr] < peor_valor) {
            peor_valor = valor_dir[dirr];
          }
        }
      }
      if (mejor_valor - peor_valor > rango_expansion) command(id, mejor_dir);
      else movimiento_expansivo(id);
    }

    for (int id : k) {
      Pos v = unit(id).pos;
      vld valor_dir(9);
      for (int dirr = 0; dirr <= 8; ++dirr) {
        Dir pd = Dir(dirr);
        Cell obj = cell(v + pd);
        if (obj.type == Wall) valor_dir[dirr] = -INF;
        else {
          vvb visitados(rows(), vb(cols(), false));
          sPos visitados_n;
          visitados_n.clear();
          qiPos q, q_n;
          q.push(iPos(0, v + pd));
          q_n.push(iPos(0, v + pd));
          if (pd == None) {
            valor_dir[dirr] = valor_quieto(id) + bfs_pos(q, visitados, Knight)
              + bfs_neg(q_n, visitados_n, Knight);
          }
          else valor_dir[dirr] = bfs_pos(q, visitados, Knight)
            + bfs_neg(q_n, visitados_n, Knight);
        }
      }
      double mejor_valor = -INF;
      double peor_valor = INF;
      Dir mejor_dir = None;
      for (int dirr = 0; dirr <= 8; ++dirr) {
        if (valor_dir[dirr] > mejor_valor) {
          mejor_valor = valor_dir[dirr];
          mejor_dir = Dir(dirr);
        }
        if (valor_dir[dirr] != -INF) {
          if (valor_dir[dirr] < peor_valor) {
            peor_valor = valor_dir[dirr];
          }
        }
      }
      if (mejor_valor - peor_valor > rango_expansion) command(id, mejor_dir);
      else movimiento_expansivo(id);
    }

    for (int id : w) {
      Pos v = unit(id).pos;
      vld valor_dir(9);
      for (int dirr = 1; dirr < 8; dirr += 2) {
        valor_dir[dirr] = -INF;
      }
      for (int dirr = 0; dirr <= 8; dirr += 2) {
        Dir pd = Dir(dirr);
        Cell obj = cell(v + pd);
        if (obj.type == Wall) valor_dir[dirr] = -INF;
        else {
          vvb visitados(rows(), vb(cols(), false));
          sPos visitados_n;
          visitados_n.clear();
          qiPos q, q_n;
          q.push(iPos(0, v + pd));
          q_n.push(iPos(0, v + pd));
          if (pd == None) {
            valor_dir[dirr] = valor_quieto(id) + bfs_pos(q, visitados, Witch)
              + bfs_neg(q_n, visitados_n, Witch);
          }
          else valor_dir[dirr] = bfs_pos(q, visitados, Witch)
            + bfs_neg(q_n, visitados_n, Witch);
        }
      }
      double mejor_valor = -INF;
      double peor_val = INF;
      Dir mejor_dir = None;
      for (int dirr = 0; dirr <= 8; ++dirr) {
        if (valor_dir[dirr] != -INF) {
          if (valor_dir[dirr] < peor_val) {
            peor_val = valor_dir[dirr];
          }
        }
        if (valor_dir[dirr] > mejor_valor) {
          mejor_valor = valor_dir[dirr];
          mejor_dir = Dir(dirr);
        }
      }
      if (mejor_valor - peor_val > rango_expansion) command(id, mejor_dir);
      else movimiento_expansivo(id);
    }

  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

