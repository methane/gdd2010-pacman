#include <vector>
#include <iostream>
#include <bitset>
#include <queue>
#include <string>
#include <cassert>

#ifndef PAC3
#define PAC3 (0)
#endif

using namespace std;

const int UP=+1, DOWN=-1, LEFT=-2, RIGHT=+2;

struct Pos {
    short x,y;
};

bool operator==(const Pos &lh, const Pos &rh) {
    return lh.x == rh.x && lh.y == rh.y;
}

class Field {
public:
    int width, height;
    bitset<992> can_move;

    void init(int width, int height) {
        this->width = width;
        this->height = height;
        can_move.set();
    }

    // 壁への衝突チェック. 移動可能なら true.
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

    bool is_dynamic() const {
        return type == 'V' || type == 'H';
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

struct Global {
    int max_turn;
    Field field;
    // ターンごとの、自機にLRJタイプの敵.
    vector<vector<Enemy> > static_enemies;
    vector<Enemy> dynamic_enemies;

    void init(int max_turn, int width, int height) {
        this->max_turn = max_turn;
        field.init(width, height);
        static_enemies.resize(max_turn+1);
    }

    // 自機の位置が影響しない敵を登録する.
    void add_enemy(char type, int x, int y) {
        Enemy enemy = Enemy(type, x, y);
        if (enemy.is_dynamic()) {
            dynamic_enemies.push_back(enemy);
        } else {
            static_enemies[0].push_back(enemy);
        }
    }

    void pre_calc() {
        int num_enemies = static_enemies[0].size();
        Pos pos;
        pos.x = pos.y = 0;
        for (int t = 0; t+1 < max_turn; ++t) {
            for (int e = 0; e < num_enemies; ++e) {
                Enemy ene = static_enemies[t][e];
                ene.move(field, t, pos);
                static_enemies[t+1].push_back(ene);
            }
        }
    }

};
Global g;

void read_quest(vector<Enemy> &enemies, Pos &mine, bitset<992> &dots, int &limit)
{
    int w, h;
    cin >> limit;
    cin >> w >> h;

    cerr << "limit: " << limit << "\nw x h = " << w << ' ' << h << endl;
    g.init(limit, w, h);
    dots.reset();

    for (int y = 0; y < h;) {
        string sbuf;
        getline(cin, sbuf);
        if (sbuf.size() < w) continue;

        for (int x = 0; x < w; ++x) {
            int c = sbuf[x];
            switch (c) {
            case '#':
                g.field.can_move.reset(y*w+x);
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
                enemies.push_back(Enemy(c, x, y));
            case 'L':
            case 'R':
            case 'J':
                g.add_enemy(c, x, y);
                break;
            }
        }
        ++y;
    }

    g.pre_calc();
}

struct State {
    short turn;
    short dot_count;
    Pos mine;
    vector<Enemy> enemies; //VHタイプの敵たち.
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
        return -dot_count*100;
    }
};

bool operator < (const State &lh, const State &rh) {
    return lh.point() < rh.point();
}

// 敵に殺されたらtrue
bool check_kill(const State &old, const State &next, const Global &g) {
    int num_enemies;
    // RLJ
    num_enemies = g.static_enemies[0].size();
    for (int i = 0; i < num_enemies; ++i) {
        if (next.mine == g.static_enemies[next.turn][i].curr)
            return true;
        if (next.mine == g.static_enemies[old.turn][i].curr &&
                old.mine == g.static_enemies[next.turn][i].curr)
            return true;
    }
    //VH
    num_enemies = next.enemies.size();
    for (int i = 0; i < num_enemies; ++i) {
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

bool check_goal(const State &s, int &limit) {
    if (s.dot_count == 0) {
        cerr << "goal!! : " << s.turn << "\n";
        cout << s.log << endl;
        limit = s.log.size()-1;
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

#if PAC3
    // TODO: 行き止まり箇所の自動検出.
    if (state.dots.test(8*W+10) && state.dots.test(9*W+49)) {
        for (vector<Enemy>::const_iterator it = state.enemies.begin();
                it != state.enemies.end(); ++it) {
            if (it->type == 'H' || it->type == 'V') {
                int dx = it->curr.x - mx;
                if (dx < 0) dx = -dx;
                int dy = it->curr.y - my;
                if (dy < 0) dy = -dy;
                if (dx+dy < 4) return false;
            }
        }
    }
#endif

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

int main()
{
    Field &field = g.field;
    vector<Enemy> enemies;
    Pos mine;
    bitset<992> dots;
    int limit;
    read_quest(enemies, mine, dots, limit);
    const int initial_limit = limit;

    priority_queue<State*, vector<State*>, comp_state> states;
    State initial_state(0, mine, enemies, dots);
    states.push(new State(initial_state));

    int check_count = 100000;
    int best = 100000;

    while (!states.empty()) {
        if (states.size() > 20000) {
            cerr << "\ncutting down" << endl;
            priority_queue<State*, vector<State*>, comp_state> new_states;
            for (int i = 0; i < 16000; ++i) {
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
        if (st->dot_count < best) {
            best = st->dot_count;
        }
        if (--check_count == 0) {
            cerr << ' ' << st->dot_count << ' ';
            check_count = best = 100000;
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
            if (!check_kill(*st, *s, g)) {
                s->log += 'j';
                if (s->dots.test(s->mine.y * field.width + s->mine.x)) {
                    s->dot_count--;
                    s->dots.reset(s->mine.y * field.width + s->mine.x);
                    if (check_goal(*s, limit)) {
                        delete s;
                        continue;
                    }
                }
                if (check_limit(*s, field, limit)) states.push(s);
                else delete s;
            }
            else delete s;
        }
        if (field.check_movable(next->mine.x, next->mine.y-1)) {
            State *s = new State(*next);
            s->mine.y--;
            if (!check_kill(*st, *s, g)) {
                s->log += 'k';
                if (s->dots.test(s->mine.y * field.width + s->mine.x)) {
                    s->dot_count--;
                    s->dots.reset(s->mine.y * field.width + s->mine.x);
                    if (check_goal(*s, limit)) {
                        delete s;
                        continue;
                    }
                }
                if (check_limit(*s, field, limit)) states.push(s);
                else delete s;
            }
            else delete s;
        }
        if (field.check_movable(next->mine.x+1, next->mine.y)) {
            State *s = new State(*next);
            s->mine.x++;
            if (!check_kill(*st, *s, g)) {
                s->log += 'l';
                if (s->dots.test(s->mine.y * field.width + s->mine.x)) {
                    s->dot_count--;
                    s->dots.reset(s->mine.y * field.width + s->mine.x);
                    if (check_goal(*s, limit)) {
                        delete s;
                        continue;
                    }
                }
                if (check_limit(*s, field, limit)) states.push(s);
                else delete s;
            }
            else delete s;
        }
        if (field.check_movable(next->mine.x-1, next->mine.y)) {
            State *s = new State(*next);
            s->mine.x--;
            if (!check_kill(*st, *s, g)) {
                s->log += 'h';
                if (s->dots.test(s->mine.y * field.width + s->mine.x)) {
                    s->dot_count--;
                    s->dots.reset(s->mine.y * field.width + s->mine.x);
                    if (check_goal(*s, limit)) {
                        delete s;
                        continue;
                    }
                }
                if (check_limit(*s, field, limit)) states.push(s);
                else delete s;
            }
            else delete s;
        }

        if (!check_kill(*st, *next, g)) {
            next->log += '.';
            if (check_limit(*next, field, limit)) states.push(next);
            else delete next;
        }
        else delete next;

        delete st;
    }
    
    if (initial_limit == limit) {
        cerr << "Can't find any ways." << endl;
    }
}


