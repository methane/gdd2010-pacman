#include <vector>
#include <iostream>
#include <bitset>
#include <queue>
#include <string>
#include <cassert>

using namespace std;

struct Pos {
    char x,y;
};

bool operator==(const Pos &lh, const Pos &rh) {
    return lh.x == rh.x && lh.y == rh.y;
}

const int UP=+1, DOWN=-1, LEFT=-2, RIGHT=+2;

class Field {
public:
    bitset<992> can_move;
    int width, height;

    bool check_movable(int x, int y) const {
        return can_move.test(y * width + x);
    }
    bool check_movable(const Pos &pos) const {
        return check_movable(pos.x, pos.y);
    }
};

class Enemy {
public:
    Pos curr;
    char type;
    char last_direct;
    char j_count;

    Enemy(char type, int x, int y) {
        this->type = type;
        curr.x  = x;
        curr.y  = y;
        last_direct = 0;
        j_count = 0;
    }
    Enemy(const Enemy &rh) {
        type = rh.type;
        curr = rh.curr;
        last_direct = rh.last_direct;
        j_count = rh.j_count;
    }

    void move_t0(const Field &field, int t, const Pos &target) {
        if (check_and_move(field, DOWN)) return;
        if (check_and_move(field, LEFT)) return;
        if (check_and_move(field, UP)) return;
        if (check_and_move(field, RIGHT)) return;
    }
    // 行き止まりなら引き返す。通路なら進む.分岐ならfalse.
    bool move_t1(const Field &field, int t, const Pos &target) {
        int directions[4];
        int num_direct = 0;
        if (field.check_movable(curr.x, curr.y-1))
            directions[num_direct++] = UP;
        if (field.check_movable(curr.x+1, curr.y))
            directions[num_direct++] = RIGHT;
        if (field.check_movable(curr.x, curr.y+1))
            directions[num_direct++] = DOWN;
        if (field.check_movable(curr.x-1, curr.y))
            directions[num_direct++] = LEFT;
        assert(num_direct > 0);
        if (num_direct > 2)
            return false;
        if (num_direct == 1) {
            last_direct = directions[0];
        } else {
            assert(num_direct == 2);
            if (-last_direct == directions[0])
                last_direct = directions[1];
            else
                last_direct = directions[0];
        }
        switch (last_direct) {
        case LEFT:
            curr.x--;
            return true;
        case RIGHT:
            curr.x++;
            return true;
        case UP:
            curr.y--;
            return true;
        case DOWN:
            curr.y++;
            return true;
        }
    }

    bool check_and_move(const Field &field, int direction) {
        switch (direction) {
        case UP:
            if (field.check_movable(curr.x, curr.y-1)) {
                curr.y--;
                last_direct = direction;
                return true;
            }
            return false;
        case DOWN:
            if (field.check_movable(curr.x, curr.y+1)) {
                curr.y++;
                last_direct = direction;
                return true;
            }
            return false;
        case LEFT:
            if (field.check_movable(curr.x-1, curr.y)) {
                curr.x--;
                last_direct = direction;
                return true;
            }
            return false;
        case RIGHT:
            if (field.check_movable(curr.x+1, curr.y)) {
                curr.x++;
                last_direct = direction;
                return true;
            }
            return false;
        }
        return false;
    }

    void move_v(const Field &field, int t, const Pos &target) {
        if (curr.y < target.y && check_and_move(field, DOWN)) return;
        if (curr.y > target.y && check_and_move(field, UP)) return;
        if (curr.x < target.x && check_and_move(field, RIGHT)) return;
        if (curr.x > target.x && check_and_move(field, LEFT)) return;

        if (check_and_move(field, DOWN)) return;
        if (check_and_move(field, LEFT)) return;
        if (check_and_move(field, UP)) return;
        if (check_and_move(field, RIGHT)) return;
        assert(0);
    }

    void move_h(const Field &field, int t, const Pos &target) {
        if (curr.x < target.x && check_and_move(field, RIGHT)) return;
        if (curr.x > target.x && check_and_move(field, LEFT)) return;
        if (curr.y < target.y && check_and_move(field, DOWN)) return;
        if (curr.y > target.y && check_and_move(field, UP)) return;

        if (check_and_move(field, DOWN)) return;
        if (check_and_move(field, LEFT)) return;
        if (check_and_move(field, UP)) return;
        if (check_and_move(field, RIGHT)) return;
    }

    void move_l(const Field &field, int t, const Pos &target) {
        switch (last_direct) {
        case DOWN:
            if (check_and_move(field, RIGHT)) return;
            if (check_and_move(field, DOWN)) return;
            if (check_and_move(field, LEFT)) return;
        case LEFT:
            if (check_and_move(field, DOWN)) return;
            if (check_and_move(field, LEFT)) return;
            if (check_and_move(field, UP)) return;
        case UP:
            if (check_and_move(field, LEFT)) return;
            if (check_and_move(field, UP)) return;
            if (check_and_move(field, RIGHT)) return;
        case RIGHT:
            if (check_and_move(field, UP)) return;
            if (check_and_move(field, RIGHT)) return;
            if (check_and_move(field, DOWN)) return;
        }
    }
    void move_r(const Field &field, int t, const Pos &target) {
        switch (last_direct) {
        case DOWN:
            if (check_and_move(field, LEFT)) return;
            if (check_and_move(field, DOWN)) return;
            if (check_and_move(field, RIGHT)) return;
        case LEFT:
            if (check_and_move(field, UP)) return;
            if (check_and_move(field, LEFT)) return;
            if (check_and_move(field, DOWN)) return;
        case UP:
            if (check_and_move(field, RIGHT)) return;
            if (check_and_move(field, UP)) return;
            if (check_and_move(field, LEFT)) return;
        case RIGHT:
            if (check_and_move(field, DOWN)) return;
            if (check_and_move(field, RIGHT)) return;
            if (check_and_move(field, UP)) return;
        }
    }
    void move_j(const Field &field, int t, const Pos &target) {
        if (j_count) {
            move_r(field, t, target);
        } else {
            move_l(field, t, target);
        }
        j_count ^= 1;
    }
    void move(const Field &field, int t, const Pos &target) {
        if (t == 0) {
            move_t0(field, t, target);
            return;
        }
        if (move_t1(field, t, target))
            return;
        switch (type) {
        case 'V':
            move_v(field, t, target);
            return;
        case 'H':
            move_h(field, t, target);
            return;
        case 'L':
            move_l(field, t, target);
            return;
        case 'R':
            move_r(field, t, target);
            return;
        case 'J':
            move_j(field, t, target);
            return;
        }
    }
};

void read_quest(Field &field, vector<Enemy> &enemies, Pos &mine,
        bitset<992> &dots, int &limit)
{
    int w, h;
    cin >> limit;
    cin >> w >> h;

    cerr << "limit: " << limit << "\nw x h = " << w << ' ' << h << endl;

    vector<string> buf;
    string sbuf;
    for (int i = 0; i < h;) {
        getline(cin, sbuf);
        if (sbuf.size() < w) continue;
        buf.push_back(sbuf);
        ++i;
    }
    field.width = w;
    field.height = h;
    field.can_move.set();
    dots.reset();

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int c = buf[y][x];
            switch (c) {
            case '#':
                field.can_move.reset(y*w+x);
                break;
            case '@':
                mine.x = x;
                mine.y = y;
                break;
            case '.':
                dots.set(y*w+x);
                break;
            case 'V':
            case 'H':
            case 'L':
            case 'R':
            case 'J':
                enemies.push_back(Enemy(c, x, y));
            }
        }
    }
}

struct State {
    short turn;
    short dot_count;
    Pos mine;
    vector<Enemy> enemies;
    bitset<992> dots;
    string log;

    State(int turn, const Pos &mine, const vector<Enemy> &enemies,
        const bitset<992> dots) :
        turn(turn), mine(mine), enemies(enemies),
        dots(dots), dot_count(dots.count()), log()
    {}

    State(const State &st) :
        turn(st.turn), mine(st.mine), enemies(st.enemies),
        dots(st.dots), dot_count(st.dot_count), log(st.log)
    {}

    int point() const {
        return -dot_count*10;
    }
};

bool operator < (const State &lh, const State &rh) {
    return lh.point() < rh.point();
}

bool check_kill(const State &old, const State &next) {
    for (size_t i = 0; i < next.enemies.size(); ++i) {
        if (next.mine == next.enemies[i].curr) return true;
        if (next.mine == old.enemies[i].curr &&
                old.mine == next.enemies[i].curr) return true;
    }
    return false;
}

void print_state(const State &s, const Field &f)
{
    cerr << "--- state ---\n"
        << "turn: " << (int)s.turn << "\n"
        << "dots: " << (int)s.dot_count << "\n";
    for (int y = 0; y < f.height; ++y) {
        for (int x = 0; x < f.width; ++x) {
            if (!f.check_movable(x,y)) {
                cerr.put('#');
                continue;
            }
            bool hit_enemy = false;
            for (vector<Enemy>::const_iterator it = s.enemies.begin();
                    it != s.enemies.end(); ++it) {
                if (it->curr.x == x && it->curr.y == y) {
                    hit_enemy = true;
                    cerr.put(it->type);
                    break;
                }
            }
            if (hit_enemy) continue;

            if (s.mine.x == x && s.mine.y == y) {
                cerr.put('@');
                continue;
            }

            if (s.dots.test(y * f.width + x)) {
                cerr.put('.');
            } else {
                cerr.put(' ');
            }
        }
        cerr.put('\n');
    }
    cerr << endl;
}

bool check_goal(const State &s, const State &initial, const Field &f) {
    if (s.dot_count == 0) {
        cerr << "goal!! : " << s.turn << "\n";
        cout << s.log << endl;

        State sl = initial;
        print_state(sl, f);
        for (int i = 0; i < s.log.size(); ++i) {
            for (vector<Enemy>::iterator it = sl.enemies.begin();
                    it != sl.enemies.end(); ++it) {
                it->move(f, sl.turn, sl.mine);
            }
            sl.turn++;

            switch (s.log[i]) {
            case 'h':
                sl.mine.x--;
                break;
            case 'l':
                sl.mine.x++;
                break;
            case 'j':
                sl.mine.y++;
                break;
            case 'k':
                sl.mine.y--;
                break;
            }

            if (sl.dots.test(sl.mine.y * f.width + sl.mine.x)) {
                sl.dot_count--;
                sl.dots.reset(sl.mine.y * f.width + sl.mine.x);
            }
            print_state(sl, f);
        }
        return true;
    }
    return false;
}

struct comp_state {
    bool operator()(const State *lh, const State *rh) {
        return lh->point() < rh->point();
    }
};

int main()
{
    Field field;
    vector<Enemy> enemies;
    Pos mine;
    bitset<992> dots;
    int limit;
    read_quest(field, enemies, mine, dots, limit);

    priority_queue<State*, vector<State*>, comp_state> states;
    State initial_state(0, mine, enemies, dots);
    states.push(new State(initial_state));

    static int check_count = 40000;

    while (!states.empty()) {
        if (states.size() > 4000) {
            cerr << "cutting down" << endl;
            priority_queue<State*, vector<State*>, comp_state> new_states;
            for (int i = 0; i < 2000; ++i) {
                new_states.push(states.top());
                states.pop();
            }
            while (!states.empty()) {
                delete states.top();
                states.pop();
            }
            states = new_states;
        }
        State *st = states.top();
        states.pop();
        if (--check_count == 0) {
            cerr << "Checking: " << st->dot_count << endl;
            check_count = 40000;
        }
        State *next = new State(*st);
        for (vector<Enemy>::iterator it = next->enemies.begin();
                it != next->enemies.end(); ++it) {
            it->move(field, next->turn, next->mine);
        }
        next->turn++;

        if (field.check_movable(next->mine.x, next->mine.y+1)) {
            State *s = new State(*next);
            s->mine.y++;
            if (!check_kill(*st, *s)) {
                s->log += 'j';
                if (s->dots.test(s->mine.y * field.width + s->mine.x)) {
                    s->dot_count--;
                    s->dots.reset(s->mine.y * field.width + s->mine.x);
                    if (check_goal(*s, initial_state, field)) {
                        delete s;
                        break;
                    }
                }
                if (s->turn + s->dot_count < limit) states.push(s);
                else delete s;
            }
            else delete s;
        }
        if (field.check_movable(next->mine.x, next->mine.y-1)) {
            State *s = new State(*next);
            s->mine.y--;
            if (!check_kill(*st, *s)) {
                s->log += 'k';
                if (s->dots.test(s->mine.y * field.width + s->mine.x)) {
                    s->dot_count--;
                    s->dots.reset(s->mine.y * field.width + s->mine.x);
                    if (check_goal(*s, initial_state, field)) {
                        delete s;
                        break;
                    }
                }
                if (s->turn + s->dot_count < limit) states.push(s);
                else delete s;
            }
            else delete s;
        }
        if (field.check_movable(next->mine.x+1, next->mine.y)) {
            State *s = new State(*next);
            s->mine.x++;
            if (!check_kill(*st, *s)) {
                s->log += 'l';
                if (s->dots.test(s->mine.y * field.width + s->mine.x)) {
                    s->dot_count--;
                    s->dots.reset(s->mine.y * field.width + s->mine.x);
                    if (check_goal(*s, initial_state, field)) {
                        delete s;
                        break;
                    }
                }
                if (s->turn + s->dot_count < limit) states.push(s);
                else delete s;
            }
            else delete s;
        }
        if (field.check_movable(next->mine.x-1, next->mine.y)) {
            State *s = new State(*next);
            s->mine.x--;
            if (!check_kill(*st, *s)) {
                s->log += 'h';
                if (s->dots.test(s->mine.y * field.width + s->mine.x)) {
                    s->dot_count--;
                    s->dots.reset(s->mine.y * field.width + s->mine.x);
                    if (check_goal(*s, initial_state, field)) {
                        delete s;
                        break;
                    }
                }
                if (s->turn + s->dot_count < limit) states.push(s);
                else delete s;
            }
            else delete s;
        }

        if (!check_kill(*st, *next)) {
            next->log += '.';
            if (next->turn + next->dot_count < limit) states.push(next);
            else delete next;
        }
        else delete next;

        delete st;
    }
}


