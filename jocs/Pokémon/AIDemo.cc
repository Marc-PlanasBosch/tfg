#include "Player.hh"

using namespace std;


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Demo


struct PLAYER_NAME : public Player {


	  /**
	   * Factory: returns a new instance of this class.
	   * Do not modify this function.
	   */
	  static Player* factory () {
		return new PLAYER_NAME;
	  }


	  /**
	   * Attributes for your player can be defined here.
	   */


	  /**
	   * Play method.
	   *
	   * This method will be invoked once per each round.
	   * You have to read the board here to place your actions
	   * for this round.
	   *
	   */
	  virtual void play () {
		// initialization

		const Poquemon& p = poquemon(me());

		if (p.alive) {
			Dir d = rand_dir(None);
			int dist = search(p, d);
			if (dist <= p.scope) attack(d);
			else move(d);
		}

	}

	Dir rand_dir (Dir notd) {
	  Dir DIRS[] = {Top, Bottom, Left, Right};
	  while (true) {
		Dir d = DIRS[randomize() % 4];
		if (d != notd) return d;
	  }
	}

	int search(const Poquemon& p, Dir d) {
		int dist = 1;
		Pos pIni = p.pos;
		while (dist <= p.scope) {
			pIni = dest(pIni, d);
			if (cell_type(pIni) == Wall) return p.scope+1;
			int idCell = cell_id(pIni);
			if (idCell != -1) {
				Poquemon p1 = poquemon(idCell);
				if (p1.defense <= p.attack) return dist;
			}
			++dist;
		}
		return p.scope+1;
	}

	int dir2num (Dir d) {
		switch(d) {
			case Top: return 0;
			case Bottom: return 1;
			case Left: return 2;
			case Right: return 3;
			case None: return -1;
		}
		return -1;
	}

};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
