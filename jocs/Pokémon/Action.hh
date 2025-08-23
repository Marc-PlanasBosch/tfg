#ifndef Action_hh
#define Action_hh

#include "Utils.hh"
#include "PosDir.hh"

using namespace std;


/**
 * Enum type for all possible actions.
 */

enum AType {
	Undefined, Moving, Attacking
};

inline bool ok(AType a) {
  return a == Undefined or a == Moving or a == Attacking;
}

/**
 * Conversion from AType to char.
 */
inline char a2c (AType a) {
	switch (a) {
		case Undefined: return 'u';
		case Moving:    return 'm';
		case Attacking:  return 'a';
	}
	_unreachable();
}


/**
 * Conversion from char to AType.
 */
inline AType c2a (char c) {
	switch (c) {
		case 'u':   return Undefined;
		case 'm':   return Moving;
		case 'a':   return Attacking;
	}
	return Undefined;
}

/**
 * Class that stores the actions requested by a player in a round.
 */

class Action {

	friend class Game;
	friend class SecGame;
	friend class Board;

	AType type;
	Dir   dir;

	/**
	 * Constructor reading one action from a stream.
	 */
	Action (istream& is);

	/**
	 * Print the action to a stream.
	 */
	void print (ostream& os) const;


public:

	/**
	 * Creates an empty action.
	 */
	Action () : type(Undefined), dir(None)
	{   }

	/**
	 * Moves the poquemon of the player in a given direction.
	 */
	void move (Dir d) {
	  if (d != None and d != Top and d != Bottom and d != Left and d != Right) return;
	  if (type == Undefined) {
			type = Moving;
			dir = d;
		} else {
			cerr << "warning: another action already requested for this poquémon." << endl;
		}
	}

	/**
	 * Throws an attack in a given direction.
	 *
	 */
	void attack (Dir d) {
	  if (d != None and d != Top and d != Bottom and d != Left and d != Right) return;
	  if (type == Undefined) {
			type = Attacking;
			dir = d;
		} else {
			cerr << "warning: another action already requested for this poquémon." << endl;
		}
	}

};


#endif
