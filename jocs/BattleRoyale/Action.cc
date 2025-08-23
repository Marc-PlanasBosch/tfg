#include "Action.hh"
#include "Board.hh"


using namespace std;


Action::Action (istream& is) {

    // Warning: all read operations must be checked for SecGame.

    u_ = set<int>();
    q_ = queue<Movement>();

    int i;
    if (is >> i) {
        while (i != -1) {
            char d;
            if (is >> d) {
                q_.push(Movement(i, c2d(d)));
                u_.insert(i);
                if (is >> i) {
                } else {
                    return;
                }
            } else {
                return;
            }
        }
    }
}


void Action::print (ostream& os) const {
    queue<Movement> qq = q_;
    while (not qq.empty()) {
        Movement m = qq.front(); qq.pop();
        os << m.i << " " << d2c(m.d) << endl;
    }
    os << -1 << endl;
}

