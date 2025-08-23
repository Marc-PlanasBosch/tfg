#include <algorithm>
#include <iomanip>

#include "Board.hh"
#include "Action.hh"

#include <cctype>

using namespace std;

Board::Board (istream& is) {
  string s;

  string version;
  is >> s >> version;
  assert(s == "Poquemon");
  assert(version == "v1");

  is >> s >> nb_players_;
  assert(s == "nb_players");
  assert(nb_players_ >= 1);
  assert(nb_players_ <= 4);
	
  is >> s >> nb_poquemon_;
  assert(s == "nb_poquemon");
  assert(nb_players_ >= 1);

  is >> s >> nb_rounds_;
  assert(s == "nb_rounds");
  assert(nb_rounds_ >= 1);
  assert(nb_rounds_ % 2 == 0); // To simplify visualization stuff.

  is >> s >> nb_ghost_wall_;
  assert(s == "nb_ghost_wall");
  assert(nb_ghost_wall_ >= 0);

  is >> s >> nb_point_;
  assert(s == "nb_point");
  assert(nb_point_ >= 0);

  is >> s >> nb_stone_;
  assert(s == "nb_stone");
  assert(nb_stone_ >= 0);

  is >> s >> nb_scope_;
  assert(s == "nb_scope");
  assert(nb_scope_ >= 0);

  is >> s >> nb_attack_;
  assert(s == "nb_attack");
  assert(nb_attack_ >= 0);

  is >> s >> nb_defense_;
  assert(s == "nb_defense");
  assert(nb_defense_ >= 0);

  is >> s >> player_regen_time_;
  assert(s == "player_regen_time");
  assert(player_regen_time_ >= 1);
	
  is >> s >> wall_change_time_;
  assert(s == "wall_change_time");
  assert(wall_change_time_ >= 1);
	
  is >> s >> point_regen_time_;
  assert(s == "point_regen_time");
  assert(point_regen_time_ >= 1);
	
  is >> s >> stone_regen_time_;
  assert(s == "stone_regen_time");
  assert(stone_regen_time_ >= 1);
		
  is >> s >> scope_regen_time_;
  assert(s == "scope_regen_time");
  assert(scope_regen_time_ >= 1);
	
  is >> s >> attack_regen_time_;
  assert(s == "attack_regen_time");
  assert(attack_regen_time_ >= 1);
	
  is >> s >> defense_regen_time_;
  assert(s == "defense_regen_time");
  assert(defense_regen_time_ >= 1);

  is >> s >> battle_reward_;
  assert(s == "battle_reward");
  assert(battle_reward_ >= 0);

  is >> s >> max_scope_;
  assert(s == "max_scope");
  assert(max_scope_ >= 1);
	
  is >> s >> max_stone_;
  assert(s == "max_stone");
  assert(max_stone_ >= 1);

  is >> s >> rows_;
  assert(s == "rows");
  assert(rows_ >= 1);

  is >> s >> cols_;
  assert(s == "cols");
  assert(cols_ >= 1);


  names_ = vector<string>(nb_players());
  is >> s;
  assert(s == "names");
  for (int pl = 0; pl < nb_players(); ++pl)
    is >> names_[pl];


  is >> s >> round_;
  assert(s == "round");
  assert(round_ >= 0);
  assert(round_ <= nb_rounds());

  cells_ = vector< vector<Cell> >(rows(), vector<Cell>(cols()));

  vector< vector<bool> > empty  = 
    vector< vector<bool> >(rows(), vector<bool>(cols(), true));
		
  // The map shows empty cells, walls, ghost walls, bag points,
  // stones, scopeUp, attackUp and defenseUp.
  // Players, ghost walls and all bonus are listed in tables too.
	
  is >> s;
  assert(s == "board");
  is >> s;
  // assert(s == "0000000000111111111122222222223333333333");
  is >> s;
  // assert(s == "0123456789012345678901234567890123456789");

  for (int i = 0; i < rows(); ++i) {
    // reading the number of the raw.
    int aux;
    is >> aux;
    assert(aux == i);
    for (int j = 0; j < cols(); ++j) {
      cells_[i][j].id = -1;
      cells_[i][j].pos = Pos(i, j);
      char c;
      is >> c;
      c = toupper(c);
      switch (c) {
      case '.': cells_[i][j].type = Empty;	break;
      case 'X': cells_[i][j].type = Wall;		break;
      case 'M': cells_[i][j].type = Empty;	break;
      case 'P': cells_[i][j].type = Point;	break;
      case 'S': cells_[i][j].type = Stone;	break;
      case 'R': cells_[i][j].type = Scope;	break;
      case 'A': cells_[i][j].type = Attack;	break;
      case 'D': cells_[i][j].type = Defense;	break;
      default: _unreachable();
	/* no break, unreachable has an assert */
      }
      if (cells_[i][j].type != Empty) empty[i][j] = false;

      assert((i == 0)       	implies (cells_[i][j].type == Wall));
      assert((i == rows()-1) 	implies (cells_[i][j].type == Wall));
      assert((j == 0)       	implies (cells_[i][j].type == Wall));
      assert((j == cols()-1) 	implies (cells_[i][j].type == Wall));
    }
  }

  is >> s;
  assert(s == "status");
  status_ = vector<double>(nb_players());
  for (int i = 0; i < nb_players(); ++i)
    is >> status_[i];

  is >> s;
  assert(s == "poquemon");
  poquemons_ = vector<Poquemon>(nb_total_poquemon());
  status_ = vector<double>(nb_players());
  // Reading the header;
  is >> s >> s >> s >> s >> s >> s >> s >> s >> s >> s >> s;
  //   id   ply   i    j    at  def   ab  ped  pts  time  alive
  for (int k = 0; k < nb_total_poquemon(); ++k) {
    int id, ply, i, j, at, def, ab, ped, pts, time;
    char alive;
    is >> id >> ply >> i >> j >> at >> def >> ab >> ped >> pts >> time >> alive;
    assert(pos_ok(i, j));
    assert(at >= 1);
    assert(def >= 0);
    assert(ab > 0);
    assert(ped >= 0 and ped <= max_stone());
    assert(pts >= 0);
    assert((time == 0) implies (alive == 'a'));
    assert((time > 0 and time < player_regen_time()) implies (alive == 'd'));
    assert(alive == 'a' or alive == 'd');
    Poquemon p;
    p.id 		= id;
    p.player	= ply;
    p.pos.i		= i;
    p.pos.j		= j;
    p.attack	= at;
    p.defense	= def;
    p.scope		= ab;
    p.stones	= ped;
    p.alive		= (alive == 'a');
    p.points	= pts;
    p.time		= time;
    if (p.alive) {
      empty[i][j] = false;
      cells_[i][j].id = id;
    }
    poquemons_[k] = p;
  }



  /**
   * 
   * Reading all the ghost walls.
   * 
   */ 
	 
  is >> s;
  assert(s == "walls");
  ghostWall_ = vector<GhostWall>(nb_ghost_wall());
  // Reading the header
  is >> s >> s >> s >> s;
  //    i    j   time present
  for (int k = 0; k < nb_ghost_wall(); ++k) {
    int i, j, time, present;
    is >> i >> j >> time >> present;
    assert(pos_ok(i, j));

    ghostWall_[k].pos = Pos(i, j);
    ghostWall_[k].present = present;
    ghostWall_[k].time = time;

    if (present) {
      cells_[i][j].type = Wall;
      assert(empty[i][j] and cell(Pos(i,j)).id == -1);
    }
    else cells_[i][j].type = Empty;
    empty[i][j] = not present;
  }

  is >> s;
  assert(s == "bonus");
					
  // Reading the header
  is >> s >> s >> s >> s >> s >> s;
  // type    i    j   pts	time present

  int nb_bonuses = nb_point() + nb_stone() + nb_scope() +
    nb_attack()+ nb_defense();

  for (int id = 0; id < nb_bonuses; ++id) {
    char t;
    int i, j, pts, time, present;
    is >> t >> i >> j >> pts >> time >> present;

    assert(pos_ok(i, j));
    assert(	t == 'P' or t == 'S' or 
		t == 'R' or t == 'D' or t == 'A');
    assert(pts >= 0);

    switch (t) {
    case 'P':
      {
	assert((present == 1) implies (pts >= 100));
	assert(pts % 100 == 0);
	PointBonus pb;
	pb.value = pts;
	pb.pos = Pos(i,j);
	pb.present = present;
	pb.time = time;
	points_.push_back(pb);
	break;
      }
    case 'S':
      {
	assert(pts == 0);
	StoneBonus sb;
	sb.pos = Pos(i,j);
	sb.present = present;
	sb.time = time;
	sb.value = pts;
	stones_.push_back(sb);
	break;
      }
    case 'R':
      {
	assert(pts == 0);
	ScopeBonus sb;
	sb.pos = Pos(i,j);
	sb.present = present;
	sb.time = time;
	sb.value = pts;
	scopes_.push_back(sb);
	break;
      }
    case 'A':
      {
	assert(pts == 0);
	AttackBonus ab;
	ab.pos = Pos(i,j);
	ab.present = present;
	ab.time = time;
	ab.value = pts;
	attacks_.push_back(ab);
	break;
      }
    case 'D':
      {
	assert(pts == 0);
	DefenseBonus db;
	db.pos = Pos(i,j);
	db.present = present;
	db.time = time;
	db.value = pts;
	defenses_.push_back(db);
	break;
      }
    default:
      cerr << "This type of bonus is not valid" << endl;
      break;
    }
  }

  // check if the initial number of each bonus is the
  // same as the number of bonus introduced in the board.

  assert(points_.size() 	== (unsigned) nb_point());
  assert(stones_.size() 	== (unsigned) nb_stone());
  assert(scopes_.size() 	== (unsigned) nb_scope());
  assert(attacks_.size() 	== (unsigned) nb_attack());
  assert(defenses_.size()	== (unsigned) nb_defense());

  assert(ok());

}


void Board::print_preamble (ostream& os) const {
  os << "Poquemon v1" 								<< endl << endl;
  os << "nb_players "			<< nb_players() 		<< endl;
  os << "nb_poquemon "		<< nb_poquemon()		<< endl;
  os << "nb_rounds " 			<< nb_rounds() 			<< endl;
  os << "nb_ghost_wall "		<< nb_ghost_wall()		<< endl;
  os << "nb_point " 			<< nb_point() 			<< endl;
  os << "nb_stone " 			<< nb_stone() 			<< endl;
  os << "nb_scope " 			<< nb_scope() 			<< endl;
  os << "nb_attack " 			<< nb_attack() 			<< endl;
  os << "nb_defense " 		<< nb_defense() 		<< endl << endl;
  os << "player_regen_time " 	<< player_regen_time() 	<< endl;
  os << "wall_change_time "	<< wall_change_time()	<< endl;
  os << "point_regen_time " 	<< point_regen_time() 	<< endl;
  os << "stone_regen_time " 	<< stone_regen_time() 	<< endl;
  os << "scope_regen_time " 	<< scope_regen_time() 	<< endl;
  os << "attack_regen_time " 	<< attack_regen_time() 	<< endl;
  os << "defense_regen_time " << defense_regen_time() << endl;
  os << "battle_reward "		<< battle_reward()		<< endl << endl;
  os << "max_scope " 			<< max_scope() 			<< endl;
  os << "max_stone "			<< max_stone()			<< endl << endl;
  os << "rows " 				<< rows() 				<< endl;
  os << "cols " 				<< cols() 				<< endl << endl;
  os << "names";
  for (int player = 0; player < nb_players(); ++player) os << " " << name(player);
  os << endl << endl;
}


void Board::print (ostream& os) const {
  os << endl;
  os << "round " << round() << endl << endl;
	
  os << "board " << endl << endl;

  os << "   ";
  for (int j = 0; j < cols(); ++j) os << (j / 10);
  os << endl;

  os << "   ";
  for (int j = 0; j < cols(); ++j) os << (j % 10);
  os << endl;
  os << endl;

  for (int i = 0; i < rows(); ++i) {
    os << (i / 10) << (i % 10) << " "; // Adding col numbers.
    for (int j = 0; j < cols(); ++j) {
      CType c = cell(i, j).type;
      bool found;
      switch (c) {
      case Empty  :
	found = false;
	for (int k = 0; k < nb_ghost_wall() and not found; ++k)
	  if (Pos(i,j) == ghostWall_[k].pos) {
	    os << 'M';
	    found = true;
	  }
	if (not found) os << '.';
	break;
      case Wall   : 
	found = false;
	for (int k = 0; k < nb_ghost_wall() and not found; ++k)
	  if (Pos(i,j) == ghostWall_[k].pos) {
	    os << 'M';
	    found = true;
	  }
	if (not found) os << 'X';
	break;
      case Point : os << 'P'; break;
      case Stone 	: os << 'S'; break;
      case Scope  : os << 'R'; break;
      case Attack : os << 'A'; break;
      case Defense: os << 'D'; break;
      default:  _unreachable();
	/* no break, unreachable code. */
      };
    }
    os << endl;
  }
  os << endl;

  os << "status";
  for (int i = 0; i < nb_players(); ++i) os << " " << fixed << setprecision(3) << status_[i];
  os << endl << endl;

  os << "poquemon" << endl;
  os << "id  ply  i  j  at  def  ab  ped  pts  time  alive" << endl;
  for (int i = 0; i < nb_total_poquemon(); ++i)
    {
      os << poquemons_[i].id << "  " << poquemons_[i].player << "  ";
      os << poquemons_[i].pos.i << "  " << poquemons_[i].pos.j << "  ";
      os << poquemons_[i].attack << "  " << poquemons_[i].defense << "  ";
      os << poquemons_[i].scope << "  " << poquemons_[i].stones << "  ";
      os << poquemons_[i].points << "  " << poquemons_[i].time << "  ";
      if (poquemons_[i].alive) os << 'a' << endl;
      else os << 'd' << endl;
    }
  os << endl;

  os << "walls" << endl;
  os << "i  j  time  present" << endl;
  for (int i = 0; i < nb_ghost_wall(); ++i)
    os << ghostWall_[i].pos.i << "  " << ghostWall_[i].pos.j << "  "
       << ghostWall_[i].time << " " << ghostWall_[i].present << endl;
  os << endl;

  os << "bonus" << endl;
  os << "type  i  j  pts  time present" << endl;
  // Point bonus.
  for (int i = 0; i < nb_point(); ++i)
    os << "P" << "  " << "  " << points_[i].pos.i << "  "
       << points_[i].pos.j << "  " << points_[i].value << "  "
       << points_[i].time << " " << points_[i].present << endl;
		
  // Stones.
  for (int i = 0; i < nb_stone(); ++i)
    os << "S" << "  " << "  " << stones_[i].pos.i << "  "
       << stones_[i].pos.j << " " << stones_[i].value << " "
       << stones_[i].time << " " << stones_[i].present << endl;
		
  // Scope up bonus.
  for (int i = 0; i < nb_scope(); ++i)
    os << "R" << "  " << "  " << scopes_[i].pos.i << "  "
       << scopes_[i].pos.j << " " << scopes_[i].value << " "
       << scopes_[i].time << " " << scopes_[i].present << endl;
		
  // Attack up bonus.
  for (int i = 0; i < nb_attack(); ++i)
    os << "A" << "  " << "  " << attacks_[i].pos.i << "  "
       << attacks_[i].pos.j << " " << attacks_[i].value << " "
       << attacks_[i].time << " " << attacks_[i].present << endl;
		
  // Defense up bonus.
  for (int i = 0; i < nb_defense(); ++i)
    os << "D" << "  " << "  " << defenses_[i].pos.i << "  "
       << defenses_[i].pos.j << " " << defenses_[i].value << " "
       << defenses_[i].time << " " << defenses_[i].present << endl;
		
  os << endl;
}


Board Board::next (const vector<Action>& asked, vector<Action>& done) const {


  assert(ok());

  // b will be the new board we shall return
  Board b = *this;

  // increment the round
  ++b.round_;

  // randomize turns
  vector<int> turns(nb_players());
  turns = b.random_permutation(nb_players());

  /*for (int player = 0; player < nb_players(); ++player) {
    turns[player] = player;
    }
    random_shuffle(turns.begin(), turns.end());*/

  // handle poquemons action
  bool oneAttackRoundDone = false;
  for (int turn = 0; turn < nb_players(); ++turn) {
    int player = turns[turn];
    assert(done[player].type == Undefined);
    assert(done[player].dir  == None     );
    AType a = asked[player].type;
    Dir   d = asked[player].dir;
    switch (a) {
    case Moving:
      {
	bool move_ok = b.apply_move(player, a, d);
	if (move_ok) {
	  done[player].type = a;
	  done[player].dir  = d;
	}
	break;
      }
    case Attacking:
      {
	Pos p = b.poquemons_[player].pos;
	if (b.exists_objective(p, d, b.poquemons_[player].scope) and not oneAttackRoundDone) {
	  bool throw_ok = b.apply_attack(player, a, d);
	  if (throw_ok) {
	    done[player].type = a;
	    done[player].dir  = d;
	    oneAttackRoundDone = true;
	  }
	}
	break;
      }
    case Undefined:
      {
	break;
      }
    default:
      cerr << "Invalid action asked of player " << player << endl;
      break;
    }
  }

  // GhostWalls regeneration
  for (int i = 0; i < nb_ghost_wall(); ++i) {
    GhostWall& gw = b.ghostWall_[i];
    if (gw.time > 0) --gw.time;
    if (gw.time == 0) {
      gw.present = not gw.present;
      gw.time = wall_change_time();
      if (gw.present) {
	int id_cell = b.cells_[gw.pos.i][gw.pos.j].id;
	/** If some player is on a position where a wall will
	 * 	appear, we kill this player without stats penalty.
	 */
	if (id_cell != -1) {
	  assert(id_cell >= 0 and id_cell < nb_total_poquemon());
	  b.poquemons_[id_cell].alive 	= false;
	  b.poquemons_[id_cell].time 	= player_regen_time();
	  b.cells_[gw.pos.i][gw.pos.j].id = -1;
	}
	b.cells_[gw.pos.i][gw.pos.j].type = Wall;
      }
      else b.cells_[gw.pos.i][gw.pos.j].type = Empty;
    }
  }

  // Poquemons regeneration
  for (int id = 0; id < nb_total_poquemon(); ++id) {
    Poquemon& pkm = b.poquemons_[id];

    // check for end of regeneration time of Poquemon
    if (not pkm.alive) {
      if (pkm.time > 0) --pkm.time;
      if (pkm.time == 0) {
	bool fnd = false;
	for (int reps = 0; reps < 200 and not fnd; ++reps) {
	  Pos p = Pos(b.randomize() % rows(), b.randomize() % cols());
	  fnd = (b.cell(p).type == Empty);
	  for (int ii = -2; fnd and ii <= 2; ++ii)
	    for (int jj = -2; fnd and jj <= 2; ++jj)
	      // we check there aren't any other poquemon
	      // in a radius of 2 respect the cell
	      // that the main poquemon will appear.
	      fnd = 	(not b.pos_ok(p.i + ii, p.j + jj)) 
		or b.cell(p.i + ii, p.j + jj).id == -1;
	  if (fnd) {
	    b.cells_[p.i][p.j].id = id;
	    pkm.pos   = p;
	    pkm.alive = true;
	  }
	}
	if (not fnd) cerr << "info: cannot place poquemon on board." << endl;
      }
    }
  }
	
  // check for end of regeneration time of Points bonus
  for (int i = 0; i < nb_point(); ++i) {
    PointBonus& pb = b.points_[i];
    if (pb.time > 0) --pb.time;
    if (pb.time == 0 and not pb.present) {
      pb.present = true;
      pb.value = ((rand() % 5) + 1) * 100;
      pb.pos = get_random_safe_position(Point, b);
    }
  }
	
  // check for end of regeneration time of Stone Bonus
  for (int i = 0; i < nb_stone(); ++i) {
    StoneBonus& s = b.stones_[i];
    if (s.time > 0) --s.time;
    if (s.time == 0 and not s.present) {
      s.present = true;
      s.pos = get_random_safe_position(Stone, b);
    }
  }
	
  // check for end of regeneration time of Scope Bonus
  for (int i = 0; i < nb_scope(); ++i) {
    ScopeBonus& r = b.scopes_[i];
    if (r.time > 0) --r.time;
    if (r.time == 0 and not r.present) {
      r.present = true;
      r.pos = get_random_safe_position(Scope, b);
    }
  }
	
  // check for end of regeneration time of Attack Bonus
  for (int i = 0; i < nb_attack(); ++i) {
    AttackBonus& a = b.attacks_[i];
    if (a.time > 0) --a.time;
    if (a.time == 0 and not a.present) {
      a.present = true;
      a.pos = get_random_safe_position(Attack, b);
    }
  }

  // check for end of regeneration time of Defense Bonus
  for (int i = 0; i < nb_defense(); ++i) {
    DefenseBonus& d = b.defenses_[i];
    if (d.time > 0) --d.time;
    if (d.time == 0 and not d.present) {
      d.present = true;
      d.pos = get_random_safe_position(Defense, b);
    }
  }

  assert(b.ok());
  return b;
}

bool Board::apply_attack(int id, AType a, Dir d) {

  if (a != Attacking or d == None) return false;

  Poquemon& pkm = poquemons_[id];
  if (not pkm.alive) return false;

  Pos p = pkm.pos;
  assert(pos_ok(p));


  bool endAttack = false;
  int dist = 0;
  while (not endAttack and dist <= pkm.scope) {
    p = dest(p, d);
    assert(pos_ok(p)); // Board is surrounded by rocks.
    if (cell(p).type == Wall) endAttack = true;
    // check it's not me in position P
    else if (cell(p).id != -1 and cell(p).id != pkm.id) {
      Poquemon& pkm2 = poquemons_[cell(p).id];
      assert(pkm2.alive);
      if (pkm2.defense <= pkm.attack) {
	cells_[p.i][p.j].id = -1;
	cells_[p.i][p.j].type = Empty;
	pkm2.alive	= false;
	pkm2.time	= player_regen_time();
	pkm.points	+= pkm2.points * (battle_reward()/100.0);
      }
      else {
	if (pkm2.defense > 1) --pkm2.defense;
      }
      if (pkm.attack > 1) --pkm.attack;
      return true;
    }
    ++dist;

  }

  return false;
}


bool Board::apply_move(int id, AType a, Dir d) {

  if (a != Moving or d == None) return false;

  Poquemon& pkm = poquemons_[id];
  if (not pkm.alive) return false;

  Pos p1 = pkm.pos;
  assert(pos_ok(p1));

  Pos p2 = dest(p1, d);

  if (not pos_ok(p2)) return false;

  Cell& c1 = cells_[p1.i][p1.j];
  assert(c1.id == id);

  Cell& c2 = cells_[p2.i][p2.j];

  if (c2.type == Wall) return false;

  if (c2.id != -1) return false;
	
  if (c2.type == Empty) {
    c1.id = -1;
    c2.id = id;
    pkm.pos = p2;
    return true;
  }

  // take an stone
  if (c2.type == Stone) {
    c2.type = Empty;
    if (pkm.stones < max_stone()) {
      ++pkm.stones;
      if (pkm.scope < max_scope()) pkm.scope += 1;
      pkm.attack += 2;
      pkm.defense += 2;
    }
    int k;
    for (k = 0; k < nb_stone(); ++k)
      if (stones_[k].pos == p2 and stones_[k].present) break;
    assert(k < nb_stone());
    stones_[k].present = false;
    stones_[k].time = stone_regen_time();
  }
  // take a scope bonus
  else if (c2.type == Scope) {
    c2.type = Empty;
    if (pkm.scope < max_scope()) pkm.scope += 1;
    int k;
    for (k = 0; k < nb_scope(); ++k)
      if (scopes_[k].pos == p2 and scopes_[k].present) break;
    assert(k < nb_scope());
    scopes_[k].present = false;
    scopes_[k].time = scope_regen_time();
  }
  // take an attack bonus
  else if (c2.type == Attack) {
    c2.type = Empty;
    pkm.attack += 1;		
    int k;
    for (k = 0; k < nb_attack(); ++k)
      if (attacks_[k].pos == p2 and attacks_[k].present) break;
    assert(k < nb_attack());
    attacks_[k].present = false;
    attacks_[k].time = attack_regen_time();
  }
  // take a defense bonus
  else if (c2.type == Defense) {
    c2.type = Empty;
    pkm.defense += 1;		
    int k;
    for (k = 0; k < nb_defense(); ++k)
      if (defenses_[k].pos == p2 and defenses_[k].present) break;
    assert(k < nb_defense());
    defenses_[k].present = false;
    defenses_[k].time = defense_regen_time();
  }
  // take a points bag bonus
  else if (c2.type == Point) {
    c2.type = Empty;
    int k;
    for (k = 0; k < nb_point(); ++k)
      if (points_[k].pos == p2 and points_[k].present) {
	pkm.points += points_[k].value;
	break;
      }
    assert(k < nb_point());
    points_[k].present = false;
    points_[k].time = point_regen_time();
    points_[k].value = 0;
  }

  c1.id = -1;
  c2.id = id;
  pkm.pos = p2;

  return true;
}

bool Board::exists_objective(Pos p, Dir d, int distMax) const {
  Pos p1 = p + d;
  while (distMax > 0) {
    if (cell(p1).type == Wall) return false;
    else if (cell(p1).id != -1) return true;
    p1 += d;
    --distMax;
  }
  return false;
}

Pos Board::get_random_safe_position(CType c, Board& b) const {
  bool fnd = false;
  for (int reps = 0; reps < 200 and not fnd; ++reps) {
    Pos p = Pos(b.randomize() % rows(), b.randomize() % cols());
    fnd = (b.cells_[p.i][p.j].type == Empty);
    for (int ii = -2; fnd and ii <= 2; ++ii)
      for (int jj = -2; fnd and jj <= 2; ++jj) {
	// we check there aren't any other poquemon in a radius
	// of 2 respect the cell that the main bonus will
	// appear, to avoid advantatges.
	fnd = (not b.pos_ok(p.i + ii, p.j + jj) ) or b.cell(p.i + ii, p.j + jj).id == -1;
	fnd = fnd and not b.is_ghost_wall(p, ghostWall_);
      }
    if (fnd) {
      b.cells_[p.i][p.j].id = -1;
      b.cells_[p.i][p.j].type = c;
      return p;
    }
  }
  if (not fnd) cerr << "info: cannot place " << c << " on board." << endl;
  return Pos(-1,-1);
}

bool Board::is_ghost_wall(Pos p, vector<GhostWall> gw) const {
  for(int i = 0; i < nb_ghost_wall(); ++i)
    if (gw[i].pos == p) return true;
  return false;
}

int Board::score (int id) const {
  int score = 0;
  int participants = poquemons_.size();
  for (int i = 0; i < participants; ++i) {
    if (poquemons_[i].player == id) score += poquemons_[i].points;
  }
  return score;
}

bool Board::ok(void) const {

  if (int(cells_.size()) != rows_) {
    cerr << "debug: problems with row dimension of the board." << endl;
    return false;
  }
  if (int(cells_[0].size()) != cols_) {
    cerr << "debug: problems with col dimension of the board." << endl;
    return false;
  }
  if (int(poquemons_.size()) != nb_players_ * nb_poquemon_) {
    cerr << "debug: problems with number of agents." << endl;
    return false;
  }
  if (int(names_.size()) != nb_players_) {
    cerr << "debug: problems with number of players." << endl;
    return false;
  }
  if (int(points_.size()) != nb_point_) {
    cerr << "debug: wrong size of vector of points bag." << endl;
    return false;
  }
  if (int(stones_.size()) != nb_stone_) {
    cerr << "debug: wrong size of vector of stones." << endl;
    return false;
  }
  if (int(scopes_.size()) != nb_scope_) {
    cerr << "debug: wrong size of vector of scope bonus." << endl;
    return false;
  }	
  if (int(attacks_.size()) != nb_attack_) {
    cerr << "debug: wrong size of vector of attacks bonus." << endl;
    return false;
  }	
  if (int(defenses_.size()) != nb_defense_) {
    cerr << "debug: wrong size of vector of defenses bonus." << endl;
    return false;
  }

  int n_points_bag = 0;
  int n_stone = 0;
  int n_scopeUp = 0;
  int n_attackUp = 0;
  int n_defenseUp = 0;
  set<int> players_set;
  for (int i = 0; i < rows(); ++i) {
    for (int j = 0; j < cols(); ++j) {

      if (cells_[i][j].pos != Pos(i, j)) {
	cerr << "debug: problem with positions in cells." << endl;
	return false;
      }

      if (i == 0          and cells_[i][j].type != Wall) {
	cerr << "debug: borders of board should be walls (1)." << endl;
	return false;
      }
      if (i == rows() - 1 and cells_[i][j].type != Wall) {
	cerr << "debug: borders of board should be walls (2)." << endl;
	return false;
      }
      if (j == 0          and cells_[i][j].type != Wall) {
	cerr << "debug: borders of board should be walls (3)." << endl;
	return false;
      }
      if (j == cols() - 1 and cells_[i][j].type != Wall) {
	cerr << "debug: borders of board should be walls (4)." << endl;
	return false;
      }
			
      int id = cells_[i][j].id;
      if (id != -1) {
	if (id < -1 or id > nb_players_) {
	  cerr << "debug: problem with players in cells." << endl;
	  return false;
	}
	if (players_set.count(id) == 1) {
	  cerr << "debug: poquemon " << id << " is repeated on board." << endl;
	  return false;
	}
	players_set.insert(id);
	if (poquemons_[id].pos != Pos(i,j)) {
	  cerr << "debug: wrong position in poquemon " << id << "." << endl;
	  return false;
	}
	if (not poquemons_[id].alive) {
	  cerr << "debug: the poquemon " << id << " should be alive." << endl;
	  return false;
	}
      }

    }
  }
  for (int id = 0; id < nb_total_poquemon(); ++id) {
    if (players_set.count(id) == 0) {
      if (poquemons_[id].alive) {
	cerr << "debug: poquemon " << id << " should be dead." << endl;
	return false;
      }
    }
  }

  for (int id = 0; id < nb_players_; ++id) {
    const Poquemon& p = poquemons_[id];
    if (p.id != id) {
      cerr << "debug: poquemon has wrong identifier." << endl;
      return false;
    }
    if (not pos_ok(p.pos)) {
      cerr << "debug: problem with position of poquemon (1)." << endl;
      return false;
    }
    if (p.attack < 1) {
      cerr << "debug: invalid attack value." << endl;
      return false;
    }
    if (p.defense < 1) {
      cerr << "debug: invalid defense value." << endl;
      return false;
    }
    if (p.scope < 1 or p.scope > max_scope()) {
      cerr << "debug: invalid scope value." << endl;
      return false;
    }
    if (p.points < 0) {
      cerr << "debug: invalid points value." << endl;
      return false;
    }
    if (p.stones < 0) {
      cerr << "debug: invalid stone value." << endl;
      return false;
    }
    if (p.alive and cell(p.pos).id != id) {
      cerr << "debug: problem with position of poquemon (2)." << endl;
      return false;
    }
    if (p.alive and cell(p.pos).type == Wall) {
      cerr << "debug: poquemon cannot be in wall." << endl;
      return false;
    }
    if (p.alive and cell(p.pos).type == Stone and p.stones > max_stone()) {
      cerr << "debug: the player has more stones than maximum allowed." << endl;
      return false;
    }
    if (p.alive and cell(p.pos).type == Scope and p.scope > max_scope()) {
      cerr << "debug: the player has more scope than maximum allowed." << endl;
      return false;
    }

    if (p.time < 0) {
      cerr << "debug: regeneration time cannot be negative." << endl;
      return false;
    }
    if (p.time > player_regen_time()) {
      cerr << "debug: regeneration time cannot exceed max value." << endl;
      return false;
    }
    if (p.alive and p.time > 0) {
      cerr << "debug: live poquemon cannot have non-null regeneration time." << endl;
      return false;
    }
  }

  for (int k = 0; k < nb_ghost_wall(); ++k) {
    Pos p = ghostWall_[k].pos;
    if (not pos_ok(p)) {
      cerr << "debug: invalid position of a Ghost Wall." << endl;
      return false;
    }
    if (ghostWall_[k].time == 0) {
      cerr << "debug: regeneration time of ghost wall should be wall_change_time" << endl;
      return false;
    }
    if (ghostWall_[k].present and cell(p).id != -1) {
      cerr << "debug: poquemon can't be here." << endl;
      return false;
    }
    if (cell(p).type == Empty and ghostWall_[k].present) {
      cerr << "debug: problems with cells and Ghost Walls (1)." << endl;
      return false;
    }
    if (cell(p).type == Wall and not ghostWall_[k].present) {
      cerr << "debug: problems with cells and Ghost Walls (2)." << endl;
      return false;
    }
  }

  for (int k = 0; k < nb_point(); ++k) {
    Pos p = points_[k].pos;
    if (not pos_ok(p)) {
      cerr << "debug: invalid position of a points bag." << endl;
      return false;
    }
    if (points_[k].present and points_[k].time > 0) {
      cerr << "debug: regeneration time of bags should be 0." << endl;
      return false;
    }
    if (not points_[k].present and points_[k].time == 0) {
      cerr << "debug: bag should be present." << endl;
      return false;
    }
    if (cell(p).type == Empty and points_[k].present) {
      cerr << p << endl;
      cerr << "debug: problems with cells and bags (1)." << endl;
      return false;
    }
    if (cell(p).type == Point and not points_[k].present and points_[k].time == 0) {
      cerr << "debug: problems with cells and bags (2)." << endl;
      return false;
    }
  }	
  for (int k = 0; k < nb_stone(); ++k) {
    Pos p = stones_[k].pos;
    if (not pos_ok(p)) {
      cerr << p << endl;
      cerr << "debug: invalid position of a Stone." << endl;
      return false;
    }
    if (stones_[k].present and stones_[k].time > 0) {
      cerr << "debug: regeneration time of stones should be null." << endl;
      return false;
    }
    if (not stones_[k].present and stones_[k].time == 0) {
      cerr << "debug: stone should be present." << endl;
      return false;
    }
    if (cell(p).type == Empty and stones_[k].present) {
      cerr << "debug: problems with cells and stones (1)." << endl;
      return false;
    }
    if (cell(p).type == Stone and not stones_[k].present and stones_[k].time == 0) {
      cerr << "debug: problems with cells and stones (2)." << endl;
      return false;
    }
  }	
  for (int k = 0; k < nb_scope(); ++k) {
    Pos p = scopes_[k].pos;
    if (not pos_ok(p)) {
      cerr << "debug: invalid position of a scope bonus." << endl;
      return false;
    }
    if (scopes_[k].present and scopes_[k].time > 0) {
      cerr << "debug: regeneration time of scope bonus should be null." << endl;
      return false;
    }
    if (not scopes_[k].present and scopes_[k].time == 0) {
      cerr << "debug: scope bonus should be present." << endl;
      return false;
    }
    if (cell(p).type == Empty and scopes_[k].present) {
      cerr << "debug: problems with cells and scope bonus (1)." << endl;
      return false;
    }
    if (cell(p).type == Scope and not scopes_[k].present and scopes_[k].time == 0) {
      cerr << "debug: problems with cells and scope bonus (2)." << endl;
      return false;
    }
  }
  for (int k = 0; k < nb_attack(); ++k) {
    Pos p = attacks_[k].pos;
    if (not pos_ok(p)) {
      cerr << "debug: invalid position of an attack bonus." << endl;
      return false;
    }
    if (attacks_[k].present and attacks_[k].time > 0) {
      cerr << "debug: regeneration time of attacks bonus should be null." << endl;
      return false;
    }
    if (not attacks_[k].present and attacks_[k].time == 0) {
      cerr << "debug: attacks bonus should be present." << endl;
      return false;
    }
    if (cell(p).type == Empty and attacks_[k].present) {
      cerr << "debug: problems with cells and attacks bonus (1)." << endl;
      return false;
    }
    if (cell(p).type == Attack and not attacks_[k].present and attacks_[k].time == 0) {
      cerr << "debug: problems with cells and attacks bonus (2)." << endl;
      return false;
    }
  }
  for (int k = 0; k < nb_defense(); ++k) {
    Pos p = defenses_[k].pos;
    if (not pos_ok(p)) {
      cerr << "debug: invalid position of a defense bonus." << endl;
      return false;
    }
    if (defenses_[k].present and defenses_[k].time > 0) {
      cerr << "debug: regeneration time of defenses bonus should be null." << endl;
      return false;
    }
    if (not defenses_[k].present and defenses_[k].time == 0) {
      cerr << "debug: defense bonus should be present." << endl;
      return false;
    }
    if (cell(p).type == Empty and defenses_[k].present) {
      cerr << "debug: problems with cells and defenses bonus (1)." << endl;
      return false;
    }
    if (cell(p).type == Defense and not defenses_[k].present and defenses_[k].time == 0) {
      cerr << "debug: problems with cells and defenses bonus (2)." << endl;
      return false;
    }
  }
	
  return true;
}

void Board::hide_opponents(int idPlayer) {

  vector<Pos> poquemonPlayer(nb_poquemon());

  // We take the position of the Poquémon
  // of the player idPlayer.
  for (int i = 0; i < nb_total_poquemon(); ++i)
    if (poquemon(i).player == idPlayer)
      poquemonPlayer.push_back(poquemon(i).pos);


  for (int i = 0; i < rows(); ++i) {
    for (int j = 0; j < cols(); ++j) {
      int idCell = cells_[i][j].id;
      /**
       * if we find any Poquémon that is not our,
       * we check if it is visible
       */
      if (idCell != -1 and poquemon(idCell).player != idPlayer) {
	if (not poquemon_visible(poquemonPlayer, Pos(i,j)))
	  cells_[i][j].id = -1;
      }
    }
  }
}

bool Board::poquemon_visible(vector<Pos> poquemonPlayer, Pos p) {
  bool found = false;
  for (int i = 0; i < int(poquemonPlayer.size()); ++i)
    if (poquemonPlayer[i].i == p.i or poquemonPlayer[i].j == p.j) found = true;
  /**
   * if we return means that p has not the same raw nor column
   * with any of the poquemonPlayer's position.
   */

  if (not found) return false;

  found = false;
  for (int i = 0; i < int(poquemonPlayer.size()) and not found; ++i) {
    // same raw
    if (poquemonPlayer[i].i == p.i) {
      // p is on the left respect poquemonPlayer[i].
      if (poquemonPlayer[i].j > p.j)
	found = is_visible(poquemonPlayer[i], p, Left);
      // p is on the right respect poquemonPlayer[i].
      else
	found = is_visible(poquemonPlayer[i], p, Right);
    }
    /**
     * same column. Else if necessary in case there are more
     * than one poquemon par player.
     */
    else if (poquemonPlayer[i].j == p.j){
      // p is on the top respect poquemonPlayer[i].
      if (poquemonPlayer[i].i > p.i)
	found = is_visible(poquemonPlayer[i], p, Top);
      // p is on the bottom respect poquemonPlayer[i].
      else
	found = is_visible(poquemonPlayer[i], p, Bottom);

    }
  }
  return found;
}

bool Board::is_visible(Pos a, Pos b, Dir d) {
  while (a != b) {
    a = a + d;
    if (a == b) return true;
    if (cell(a).type == Wall) return false;
  }
  return false;
}

