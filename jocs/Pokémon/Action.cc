#include "Action.hh"
#include "Board.hh"

using namespace std;


Action::Action (istream& is) {

  // Warning: all read operations must be checked for SecGame.
    
  char ca, cd;
  is >> ca >> cd;
  type = c2a(ca);
  dir  = c2d(cd);
  if (not ok(type) or not ok(dir)) {
    type = Undefined;
    dir = None;
  }
}

void Action::print (ostream& os) const {
  os << a2c(type) << " " << d2c(dir) << endl;
}
