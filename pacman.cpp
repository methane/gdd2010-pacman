#include <vector>
#include <iostream>
#include <bitset>
#include <queue>
#include <string>
#include <cassert>

#ifndef PAC3
#define PAC3 (0)
#endif

#define MAX_WIDTH (64)
#define MAX_HEIGHT (20)
#define MAX_SIZE (MAX_WIDTH*MAX_HEIGHT)

using namespace std;

enum DIRECTION {
    UP    = -MAX_WIDTH,
    DOWN  = +MAX_WIDTH,
    LEFT  = -1,
    RIGHT = +1,
    NO_DIRECTION = 0
};

struct Pos {
    short pos_;
    Pos(int x, int y) : pos_(y*MAX_WIDTH + x) {}
    Pos(const Pos &r) : pos_(r.pos_) {}
    Pos() : pos_(0) {}

    int x() const { return pos_ % MAX_WIDTH; }
    int y() const { return pos_ / MAX_WIDTH; }

    const Pos& operator+=(int d) {
        pos_ += d;
        return *this;
    }
    const Pos operator+(int d) const {
        Pos res = *this;
        res += d;
        return res;
    }

};
bool operator<(const Pos &lh, const Pos &rh) {
    return lh.pos_ < rh.pos_;
}
bool operator==(const Pos &lh, const Pos &rh) {
    return lh.pos_ == rh.pos_;
}


class Field {
public:
    int width, height;
    bitset<MAX_SIZE> can_move;

    void init(int width, int height) {
        this->width = width;
        this->height = height;
        can_move.set();
    }

    // 壁への衝突チェック. 移動可能なら true.
    bool movable(int x, int y) const {
        return can_move.test(y * MAX_WIDTH + x);
    }
    bool movable(const Pos &pos) const {
        return can_move.test(pos.pos_);
    }
};
Field field;


class Enemy {
public:
    Pos curr;
    char type;
    char last_direct;
    char j_count;

    Enemy(char type, int x, int y) : curr(x, y), type(type) {
        last_direct = NO_DIRECTION;
        j_count = 0;
    }
    Enemy(const Enemy &rh) : curr(rh.curr), type(rh.type) {
        last_direct = rh.last_direct;
        j_count = rh.j_count;
    }

    bool is_dynamic() const {
        return type == 'V' || type == 'H';
    }

    void move_t0(const Field &field, const Pos &target) {
        if (check_and_move(field, DOWN)) return;
        if (check_and_move(field, LEFT)) return;
        if (check_and_move(field, UP)) return;
        if (check_and_move(field, RIGHT)) return;
    }
    // 行き止まりなら引き返す。通路なら進む.分岐ならfalse.
    bool move_t1(const Field &field, const Pos &target) {
        int directions[4];
        int num_direct = 0;
        if (field.movable(curr + UP)) directions[num_direct++] = UP;
        if (field.movable(curr + RIGHT)) directions[num_direct++] = RIGHT;
        if (field.movable(curr + DOWN)) directions[num_direct++] = DOWN;
        if (field.movable(curr + LEFT)) directions[num_direct++] = LEFT;
        assert(num_direct > 0);

        if (num_direct > 2)
            return false;

        if (num_direct == 1) {
            last_direct = directions[0];
        } else {
            if (-last_direct == directions[0])
                last_direct = directions[1];
            else
                last_direct = directions[0];
        }
        curr += last_direct;
        return true;
    }

    bool check_and_move(const Field &field, int direction) {
        if (field.movable(curr + direction)) {
            curr += direction;
            last_direct = direction;
            return true;
        }
        return false;
    }

    void move_v(const Field &field, const Pos &target) {
        if (curr.y() < target.y() && check_and_move(field, DOWN)) return;
        if (curr.y() > target.y() && check_and_move(field, UP)) return;
        if (curr.x() < target.x() && check_and_move(field, RIGHT)) return;
        if (curr.x() > target.x() && check_and_move(field, LEFT)) return;

        if (check_and_move(field, DOWN)) return;
        if (check_and_move(field, LEFT)) return;
        if (check_and_move(field, UP)) return;
        if (check_and_move(field, RIGHT)) return;
    }

    void move_h(const Field &field, const Pos &target) {
        if (curr.x() < target.x() && check_and_move(field, RIGHT)) return;
        if (curr.x() > target.x() && check_and_move(field, LEFT)) return;
        if (curr.y() < target.y() && check_and_move(field, DOWN)) return;
        if (curr.y() > target.y() && check_and_move(field, UP)) return;

        if (check_and_move(field, DOWN)) return;
        if (check_and_move(field, LEFT)) return;
        if (check_and_move(field, UP)) return;
        if (check_and_move(field, RIGHT)) return;
    }

    void move_l(const Field &field, const Pos &target) {
        switch (last_direct) {
        case DOWN:
            if (check_and_move(field, RIGHT)) return;
            if (check_and_move(field, DOWN)) return;
            if (check_and_move(field, LEFT)) return;
            break;
        case LEFT:
            if (check_and_move(field, DOWN)) return;
            if (check_and_move(field, LEFT)) return;
            if (check_and_move(field, UP)) return;
            break;
        case UP:
            if (check_and_move(field, LEFT)) return;
            if (check_and_move(field, UP)) return;
            if (check_and_move(field, RIGHT)) return;
            break;
        case RIGHT:
            if (check_and_move(field, UP)) return;
            if (check_and_move(field, RIGHT)) return;
            if (check_and_move(field, DOWN)) return;
            break;
        }
    }
    void move_r(const Field &field, const Pos &target) {
        switch (last_direct) {
        case DOWN:
            if (check_and_move(field, LEFT)) return;
            if (check_and_move(field, DOWN)) return;
            if (check_and_move(field, RIGHT)) return;
            break;
        case LEFT:
            if (check_and_move(field, UP)) return;
            if (check_and_move(field, LEFT)) return;
            if (check_and_move(field, DOWN)) return;
            break;
        case UP:
            if (check_and_move(field, RIGHT)) return;
            if (check_and_move(field, UP)) return;
            if (check_and_move(field, LEFT)) return;
            break;
        case RIGHT:
            if (check_and_move(field, DOWN)) return;
            if (check_and_move(field, RIGHT)) return;
            if (check_and_move(field, UP)) return;
            break;
        }
    }
    void move_j(const Field &field, const Pos &target) {
        if (j_count) {
            move_r(field, target);
        } else {
            move_l(field, target);
        }
        j_count ^= 1;
    }
    void move(const Field &field, const Pos &target) {
        if (last_direct == NO_DIRECTION) {
            move_t0(field, target);
            return;
        }
        if (move_t1(field, target))
            return;
        switch (type) {
        case 'V':
            move_v(field, target);
            return;
        case 'H':
            move_h(field, target);
            return;
        case 'L':
            move_l(field, target);
            return;
        case 'R':
            move_r(field, target);
            return;
        case 'J':
            move_j(field, target);
            return;
        }
    }
};

struct Global {
    int max_turn;
    int limit;
    // ターンごとの、自機にLRJタイプの敵.
    vector<vector<Enemy> > static_enemies;
    vector<Enemy> dynamic_enemies;

    void init(int max_turn, int width, int height) {
        this->max_turn = max_turn;
        this->limit = max_turn;
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
        Pos pos(0, 0);
        for (int t = 0; t+1 < max_turn; ++t) {
            for (int e = 0; e < num_enemies; ++e) {
                Enemy ene = static_enemies[t][e];
                ene.move(field, pos);
                static_enemies[t+1].push_back(ene);
            }
        }
    }

};
Global global;

void read_quest(vector<Enemy> &enemies, Pos &mine, bitset<992> &dots)
{
    int w, h, limit;
    cin >> limit;
    cin >> w >> h;

    cerr << "limit: " << limit << "\nw x h = " << w << ' ' << h << endl;
    global.init(limit, w, h);
    dots.reset();

    for (int y = 0; y < h;) {
        string sbuf;
        getline(cin, sbuf);
        if (sbuf.size() < w) continue;

        for (int x = 0; x < w; ++x) {
            int c = sbuf[x];
            switch (c) {
            case '#':
                field.can_move.reset(y*MAX_WIDTH+x);
                break;
            case '@':
                mine = Pos(x, y);
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
                global.add_enemy(c, x, y);
                break;
            }
        }
        ++y;
    }

    global.pre_calc();
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
bool check_kill(const State &old, const State &next) {
    int num_enemies;
    // RLJ
    num_enemies = global.static_enemies[0].size();
    for (int i = 0; i < num_enemies; ++i) {
        if (next.mine == global.static_enemies[next.turn][i].curr)
            return true;
        if (next.mine == global.static_enemies[old.turn][i].curr &&
                old.mine == global.static_enemies[next.turn][i].curr)
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
            if (!f.movable(x,y)) {
                cerr.put('#');
                continue;
            }
            bool hit_enemy = false;
            for (vector<Enemy>::const_iterator it = s.enemies.begin();
                    it != s.enemies.end(); ++it) {
                if (it->curr == Pos(x, y)) {
                    hit_enemy = true;
                    cerr.put(it->type);
                    break;
                }
            }
            if (hit_enemy) continue;

            if (s.mine == Pos(x, y)) {
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

bool check_goal(const State &s) {
    if (s.dot_count == 0) {
        cerr << "goal!! : " << s.turn << "\n";
        assert(s.turn == s.log.size());
        for (int i = 0; i < s.log.size(); ++i) {
            char d = s.log[i];
            char dc = '.';
            switch (d) {
            case UP:
                dc = 'k';
                break;
            case DOWN:
                dc = 'j';
                break;
            case LEFT:
                dc = 'h';
                break;
            case RIGHT:
                dc = 'l';
                break;
            case NO_DIRECTION:
            default:
                dc = '.';
            }
            cout << dc;
        }
        cout << endl;
        if (global.limit >= s.turn) {
            global.limit = s.turn - 1;
        }
        return true;
    }
    return false;
}

// true ならまだ可能性がある.
bool check_limit(const State &state)
{
#if 1
    int mx = state.mine.x();
    int my = state.mine.y();
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
    return (state.dot_count + state.turn + dist - 1) < global.limit;
}

struct comp_state {
    bool operator()(const State *lh, const State *rh) {
        return lh->point() < rh->point();
    }
};

inline State* self_move(const State &curr, const State &next, int direction)
{
    if (!field.movable(next.mine + direction)) return 0;
    State *s = new State(next);
    s->mine += direction;

    if (check_kill(curr, *s)) {
        delete s;
        return 0;
    }

    s->log += (char)direction;

    int dot_pos = s->mine.y() * field.width + s->mine.x();

    if (s->dots.test(dot_pos)) {
        s->dots.reset(dot_pos);
        s->dot_count--;
        if (check_goal(*s)) {
            delete s;
            return 0;
        }
    }

    if (check_limit(*s)) return s;
    delete s;
    return 0;
}

int main()
{
    vector<Enemy> enemies;
    Pos mine;
    bitset<992> dots;
    read_quest(enemies, mine, dots);

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
            it->move(field, next->mine);
        }
        next->turn++;

        State *moved;
        moved = self_move(*st, *next, DOWN);
        if (moved) states.push(moved);

        moved = self_move(*st, *next, UP);
        if (moved) states.push(moved);

        moved = self_move(*st, *next, LEFT);
        if (moved) states.push(moved);

        moved = self_move(*st, *next, RIGHT);
        if (moved) states.push(moved);
        moved = 0;

        if (check_kill(*st, *next) || !check_limit(*next)) {
            delete next;
        } else {
            next->log += (char)NO_DIRECTION;
            states.push(next);
        }

        delete st;
    }
    
    if (global.max_turn == global.limit) {
        cerr << "Can't find any ways." << endl;
    }
}


