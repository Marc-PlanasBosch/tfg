#ifndef Board_hh
#define Board_hh

#include "Utils.hh"
#include "PosDir.hh"
#include "Action.hh"

using namespace std;

/**
 * Enum type for all possible contents of a cell.
 */

enum CType {
	Empty, Wall, Stone, Point, Scope, Attack, Defense
};

/**
 * Printing function for CType
 */

inline ostream& operator <<(ostream& ost, CType t) {
	switch (t) {
	case Empty:
		ost << "Empty";
		return ost;
	case Wall:
		ost << "Wall";
		return ost;
	case Stone:
		ost << "Stone";
		return ost;
	case Point:
		ost << "Point";
		return ost;
	case Scope:
		ost << "Scope";
		return ost;
	case Attack:
		ost << "Attack";
		return ost;
	case Defense:
		ost << "Defense";
		return ost;
	default:
		return ost;
	}
}

/**
 * The description of a cell stores its position,
 * its type (Empty|Wall|Stone|Point|Scope|Attack|Defense),
 * and a player identifier (none when id==-1).
 */

struct Cell {
	Pos pos;
	CType type;
	int id;
};

/**
 * The description of a ghost wall stores its position,
 * a flag indicating whether it is present on the board or not,
 * and the time remaining to change the state (present, or not).
 */

struct GhostWall {
	Pos pos;
	bool present;
	int time;
};

/**
 * The description of a point bonus stores its value, its position,
 * a flag indicating whether it is present on the board or not, 
 * and the time left before it regenerates again if not present.
 */

struct PointBonus {
	int value;
	Pos pos;
	bool present;
	int time;
};

/**
 * The description of a stone bonus stores its position,
 * a flag indicating whether it is present on the board or not, 
 * and the time left before it regenerates again if not present.
 * Value will be always 0 in this original game. Future versions
 * may change it.
 */

struct StoneBonus {
	int value;
	Pos pos;
	bool present;
	int time;
};

/**
 * The description of a ScopeBonus stores its position,
 * a flag indicating whether it is present on the board or not, 
 * and the time left before it regenerates again if not present.
 * Value will be always 0 in this original game. Future versions
 * may change it.
 */

struct ScopeBonus {
	int value;
	Pos pos;
	bool present;
	int time;
};

/**
 * The description of a AttackBonus stores its position,
 * a flag indicating whether it is present on the board or not, 
 * and the time left before it regenerates again if not present.
 * Value will be always 0 in this original game. Future versions
 * may change it.
 */

struct AttackBonus {
	int value;
	Pos pos;
	bool present;
	int time;
};

/**
 * The description of a DefenseBonus stores its position,
 * a flag indicating whether it is present on the board or not, 
 * and the time left before it regenerates again if not present.
 * Value will be always 0 in this original game. Future versions
 * may change it.
 */

struct DefenseBonus {
	int value;
	Pos pos;
	bool present;
	int time;
};

/**
 * The description of a Poquémon stores its Poquémon id, player, position,
 * attack, defense and scope value, the number of stones collected,
 * alive state, the number of pints of this Poquémon and the time before
 * the Poquémon becomes alive again (if dead).
 */

struct Poquemon {
	int id;
	int player;
	Pos pos;
	int attack;
	int defense;
	int scope;
	int stones;
	bool alive;
	int points;
	int time;
};

/**
 * Forward declarations.
 */
class Action;

/**
 * The board.
 */

class Board {

public:

	/**
	 * Empty constructor.
	 */
	Board() {
	}

	/**
	 * Returns the number of rounds in the game.
	 */
	inline int nb_rounds() const {
		return nb_rounds_;
	}

	/**
	 * Returns the number of players in the game.
	 */
	inline int nb_players() const {
		return nb_players_;
	}

	/**
	 * Returns the number of Poquémon for player in the game.
	 * In this version of the game each player only has a single
	 * Poquémon.
	 */
	inline int nb_poquemon() const {
		return nb_poquemon_;
	}

	/**
	 * Returns the total number of Poquémon in the game.
	 */
	inline int nb_total_poquemon() const {
		return nb_poquemon() * nb_players();
	}

	/**
	 * Returns the number of ghost walls in the game.
	 */
	inline int nb_ghost_wall() const {
		return nb_ghost_wall_;
	}

	/**
	 * Returns the number of Point bonuses in the game.
	 */
	inline int nb_point() const {
		return nb_point_;
	}

	/**
	 * Returns the number of Stones bonuses in the game.
	 */
	inline int nb_stone() const {
		return nb_stone_;
	}

	/**
	 * Returns the number of Scope bonuses in the game.
	 */
	inline int nb_scope() const {
		return nb_scope_;
	}

	/**
	 * Returns the number of Attack bonuses in the game.
	 */
	inline int nb_attack() const {
		return nb_attack_;
	}

	/**
	 * Returns the number of Defense bonuses in the game.
	 */
	inline int nb_defense() const {
		return nb_defense_;
	}

	/**
	 * Returns the regeneration time (in rounds) of players in the game.
	 */
	inline int player_regen_time() const {
		return player_regen_time_;
	}

	/**
	 * Return the time (in rounds) before a ghost wall changes its status
	 * (from present to hidden, and vice versa)
	 */
	inline int wall_change_time() const {
		return wall_change_time_;
	}

	/**
	 * Returns the regeneration time (in rounds) of Point bonuses in the game.
	 */
	inline int point_regen_time() const {
		return point_regen_time_;
	}

	/**
	 * Returns the regeneration time (in rounds) of Stones bonuses in the game.
	 */
	inline int stone_regen_time() const {
		return stone_regen_time_;
	}

	/**
	 * Returns the regeneration time (in rounds) of ScopeBonus bonuses in the game.
	 */
	inline int scope_regen_time() const {
		return scope_regen_time_;
	}

	/**
	 * Returns the regeneration time (in rounds) of AttackBonus bonuses
	 * in the game.
	 */
	inline int attack_regen_time() const {
		return attack_regen_time_;
	}

	/**
	 * Returns the regeneration time (in rounds) of DefenseBonus bonuses
	 * in the game.
	 */
	inline int defense_regen_time() const {
		return defense_regen_time_;
	}

	/**
	 * Returns the bonification of killing another Poquémon
	 */
	inline int battle_reward() const {
		return battle_reward_;
	}

	/**
	 * Returns the maximum possible scope of Poquémon in the game.
	 */
	inline int max_scope() const {
		return max_scope_;
	}

	/**
	 * Returns the maximum possible stones per Poquémon in the game.
	 */
	inline int max_stone() const {
		return max_stone_;
	}

	/**
	 * Returns the number of rows of the board in the game.
	 */
	inline int rows() const {
		return rows_;
	}

	/**
	 * Returns the number of columns of the board in the game.
	 */
	inline int cols() const {
		return cols_;
	}

	/**
	 * Returns the current round.
	 */
	inline int round() const {
		return round_;
	}

	/**
	 * Return whether player is a valid player identifier.
	 */
	inline bool player_ok(int player) const {
		return player >= 0 and player < nb_players();
	}

	/**
	 * Return whether (i,j) is a position inside the board.
	 */
	inline bool pos_ok(int i, int j) const {
		return i >= 0 and i < rows() and j >= 0 and j < cols();
	}

	/**
	 * Return whether p is a position inside the board.
	 */
	inline bool pos_ok(const Pos& p) const {
		return pos_ok(p.i, p.j);
	}

	/**
	 * Return whether (i,j) + d is a position inside the board.
	 */
	inline bool pos_ok(int i, int j, Dir d) const {
		return pos_ok(Pos(i, j) + d);
	}

	/**
	 * Return whether p+d is a position inside the board.
	 */
	inline bool pos_ok(const Pos& p, Dir d) const {
		return pos_ok(p + d);
	}

	/**
	 * Returns the position resulting from moving from
	 * position p towards direction d.
	 */
	inline Pos dest(const Pos& p, Dir d) const {
		Pos q = p + d;
		return q;
	}

	/**
	 * Returns the current cpu status of a player.
	 * This is a value from 0 (0%) to 1 (100%) or negative if frozen.
	 */
	inline double status(int id) const {
		check(player_ok(id));
		return status_.at(id);
	}

	/**
	 * Returns the Poquémon of a player.
	 */
	inline const Poquemon& poquemon(int id) const {
		check(player_ok(id));
		return poquemons_.at(id);
	}

	/**
	 * Returns the cell type at (i, j).
	 */
	inline const CType& cell_type(int i, int j) const {
		check(pos_ok(i, j));
		return cells_[i][j].type;
	}

	/**
	 * Returns the cell type at p.
	 */
	inline const CType& cell_type(const Pos& p) const {
		return cell(p.i, p.j).type;
	}

	/**
	 * Returns the cell Poquémon id at pObj. -1 if there aren't
	 * any Poquémon or you are unable to see the cell pObj.
	 *
	 * In this version the poquémon id will be always the same
	 * as the player id because each player only has a single
	 * Poquémon.
	 */
	inline const int cell_id(Pos pObj) {
		check(pos_ok(pObj));
		return cell(pObj).id;
	}

	/**
	 * Returns the points at position p
	 */
	inline const int points_value(Pos p) const {
		int size = points_.size();
		for (int i = 0; i < size; ++i) {
			if (points_[i].present and points_[i].pos == p)
				return points_[i].value;
		}
		return -1;
	}

	/**
	 * Returns the time remaining to change the ghost wall at position p.
	 */
	inline const int ghost_wall(Pos p) const {
		int size = ghostWall_.size();
		for (int i = 0; i < size; ++i) {
			if (ghostWall_[i].pos == p)
				return ghostWall_[i].time;
		}
		return -1;
	}

	/**
	 * Returns random number.
	 */
	int randomize(void);

	/**
	 * Returns a random number between l and u.
	 */
	int randomize(int l, int u);

	/****************************************************/
	/***************** STOP READING HERE ****************/
	/****************************************************/

private:

	// Allow access to the private part of Board.
	friend class Game;
	friend class SecGame;

	Random_generator rgen;

	// Game settings
	int nb_players_;
	int nb_poquemon_;
	int nb_ghost_wall_;
	int nb_rounds_;
	int nb_point_;
	int nb_stone_;
	int nb_scope_;
	int nb_attack_;
	int nb_defense_;
	int player_regen_time_;
	int wall_change_time_;
	int point_regen_time_;
	int stone_regen_time_;
	int scope_regen_time_;
	int attack_regen_time_;
	int defense_regen_time_;
	int battle_reward_;
	int max_scope_;
	int max_stone_;

	int rows_;
	int cols_;
	vector<string> names_;

	// Game state
	int round_;
	vector<vector<Cell> > cells_;
	vector<Poquemon> poquemons_;
	vector<GhostWall> ghostWall_;
	vector<PointBonus> points_;
	vector<StoneBonus> stones_;
	vector<ScopeBonus> scopes_;
	vector<AttackBonus> attacks_;
	vector<DefenseBonus> defenses_;

	vector<double> status_;
	// cpu status. <-1: dead, 0..1: %of cpu time limit

	/**
	 * Construct a board by reading first round from a stream.
	 */
	Board(istream& is);

	/**
	 * Print the board preamble to a stream.
	 */
	void print_preamble(ostream& os) const;

	/**
	 * Print the board to a stream.
	 */
	void print(ostream& os) const;

	/**
	 * Computes the next board applying the given actions as to the current board.
	 * It also returns the actual actions performed.
	 */
	Board next(const vector<Action>& asked, vector<Action>& done) const;

	/**
	 * Applies a Poquémon attack. Returns true if the action can be done.
	 */
	bool apply_attack(int p, AType a, Dir d);

	/**
	 * Applies a movement. Returns true if the action can be done.
	 */
	bool apply_move(int p, AType a, Dir d);

	/**
	 * Checks invariants of the board. For debugging.
	 */
	bool ok(void) const;

	/**
	 * Returns a reference to the current cpu status of a player.
	 */
	inline double& statusPriv(int player) {
		return status_[player];
	}

	/**
	 * Returns the name of a player.
	 */
	inline string name(int id) const {
		check(player_ok(id));
		return names_.at(id);
	}

	/**
	 * Returns if exists any other player from position p
	 * to direction d until distMax distance.
	 */
	bool exists_objective(Pos p, Dir d, int distMax) const;

	/**
	 * Returns random safe position to regenerate a bonus
	 */
	Pos get_random_safe_position(CType c, Board& b) const;

	/**
	 * Returns the total score of a the player id adding the points
	 * of all his Poquémon.
	 */
	int score(int id) const;

	/**
	 * If position p is a ghost wall it returns true, otherwise false.
	 */
	bool is_ghost_wall(Pos p, vector<GhostWall> gw) const;

	/**
	 * Returns the cell at (i, j).
	 */
	inline const Cell& cell(int i, int j) const {
		check(pos_ok(i, j));
		return cells_[i][j];
	}

	/**
	 * Returns the cell at p.
	 */
	inline const Cell& cell(const Pos& p) const {
		return cell(p.i, p.j);
	}

	/**
	 * This function sets to -1 the id of the opponents Poquémon
	 * you cannot see.
	 */
	void hide_opponents(int num);

	/**
	 * Returns if the position (i,j) is visible from any of the
	 * positions of poquemonPlayer vector.
	 */
	bool poquemon_visible(vector<Pos> poquemonPlayer, Pos p);

	/**
	 * Returns true if position b can be seen from position a
	 * going through direction d. Otherwise false.
	 */
	bool is_visible(Pos a, Pos b, Dir d);

	void srandomize(int s);

	vector<int> random_permutation(int n);


};


inline vector<int> Board::random_permutation(int n) {
  vector<int> v(n);
  for (int i = 0; i < n; ++i) v[i] = i;
  for (int i = 0; i < n; ++i) swap(v[i], v[randomize(i, n-1)]);
  return v;
}

inline int Board::randomize(void) {
	  return rgen.randomize();
}

inline int Board::randomize(int l, int u) {
  return rgen.randomize(l, u);
}

inline void Board::srandomize(int s) {
  return rgen.srandomize(s);
}

#endif // Board_hh
