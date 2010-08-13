#include <vector>
#include <iostream>

using namespace std;

const int UP=+1, DOWN=-1, LEFT=-2, RIGHT=+2;

class Field {
public:
    vector<bool> can_move;
    int width, height;

    bool check_movable(int x, int y) {
        return can_move[y * width + x];
    }
};

struct Pos {
    int x,y;
};

class Enemy {
public:
    Pos curr;
    int type;
    int last_direct;
    int j_count;

    Enemy(int type, int x, int y) {
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
    }
    void move_r(const Field &field, int t, const Pos &target) {
    }
    void move_j(const Field &field, int t, const Pos &target) {
    }
    void move(const Field &field, int t, const Pos &target) {
        if (t == 0) {
            move_t0(field, t, target);
            return;
        }
        if (move_t1(field, t, target))
            return;
        move_orig(field, t, target);
    }
    // 敵の種類ごとに異なる移動方法.
    virtual void move_orig(const Field &field, int t, const Pos &target) = 0;
};

class V : public Enemy {
};

