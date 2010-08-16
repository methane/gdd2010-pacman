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
        return -dot_count;
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

void replay(const State &initial, const Field &f, const string &log)
{
    State sl = initial;
    print_state(sl, f);
    for (int i = 0; i < log.size(); ++i) {
        for (vector<Enemy>::iterator it = sl.enemies.begin();
                it != sl.enemies.end(); ++it) {
            it->move(f, sl.turn, sl.mine);
        }
        sl.turn++;

        switch (log[i]) {
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
}

bool check_goal(const State &s, const State &initial, const Field &f) {
    if (s.dot_count == 0) {
        cerr << "goal!! : " << s.turn << "\n";
        cout << s.log << endl;
        replay(initial, f, s.log);
        return true;
    }
    return false;
}

bool check_limit(const State &state, const Field &field, int limit)
{
#if 1
    int mx = state.mine.x;
    int my = state.mine.y;
    int W = field.width;
    int H = field.height;
    int off = 0;
    int dist;

    for (dist = 1; dist < 100; ++dist) {
        for (int y = my-dist; y <= my+dist; ++y) {
            if (y < 1 || H <= y) continue;
            int d = my-y < 0 ? y-my : my-y;
            d = dist - d;
            if ((mx - d > 1) && state.dots.test(y*W+mx-d)) goto dist_found;
            if ((mx + d <=W) && state.dots.test(y*W+mx+d)) goto dist_found;
        }
    }
dist_found:
#else
    int dist = 1;
#endif
    return (state.dot_count + state.turn + dist - 1) < limit;
}

struct comp_state {
    bool operator()(const State *lh, const State *rh) {
        return lh->point() < rh->point();
    }
};

int main(int argc, char *argv[])
{
    Field field;
    vector<Enemy> enemies;
    Pos mine;
    bitset<992> dots;
    int limit;
    int best = 10;
    string log;
    getline(cin, log);
    read_quest(field, enemies, mine, dots, limit);

    State initial_state(0, mine, enemies, dots);
    replay(initial_state, field, log);
}


