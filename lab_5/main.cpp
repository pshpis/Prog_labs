#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template<class T>
class CircularBuffer {
private:
    T *buffer;
    int n, beg, fsh, sz;

    int plus(int x, int y) {
        return (x + y + n) % n;
    }

    int minus(int x, int y) {
        return (x - y + n) % n;
    }

    void pp(int &x) {
        x = (x + 1) % n;
    }

    int mm(int &x) {
        x = (x - 1 + n) % n;
        return x;
    }

public:
    explicit CircularBuffer(int n_ = 0) {
        n = n_;
        buffer = (T *) malloc(n * sizeof(T));
        beg = n / 2;
        fsh = minus(n / 2, 1);
        sz = 0;
    }

    void push_back(T x) {
        cout << beg << " " << fsh << " " << x << endl;
        pp(fsh);
        cout << beg << " " << fsh << " " << x << endl;
        cout << "----------------------" << endl;
        if (fsh == beg && sz > 0) pp(beg);
        buffer[fsh] = x;

        sz++;
        if (sz > n) sz = n;
    }

    void pop_front() {
        if (sz <= 0) return;
        pp(beg);
        sz--;
    }

    void push_front(T x) {
        mm(beg);
        if (beg == fsh && sz > 0) mm(fsh);
        buffer[beg] = x;

        sz++;
        if (sz > n) sz = n;
    }

    void pop_back() {
        if (sz <= 0) return;
        mm(fsh);
        sz--;
    }

    int size() { return sz; }


    vector<int> get(){
        vector<int> res;
        for (int it = beg; it <= fsh; pp(it)) res.push_back(buffer[it]);
        return res;
    }

    void write(){
        cout << size() << endl;
        int cnt = 0;
        int it;
        for (it = beg; it != fsh; pp(it)){
            if (it == beg && cnt != 0) break;
            cout << buffer[it] << " ";
            cnt ++;
        }
        cout << buffer[it] << endl;
    }

    void write_buffer(){
        cout << n << " " << beg << " " << fsh << endl;
        for (int i = 0; i < n; i ++) cout << buffer[i] << " ";
        cout << endl;
    }

    auto begin(){
        return buffer;
    }

    auto end(){
        return buffer + n;
    }

    T& operator [](int i){
        return &buffer[i];
    }

    void resize(int new_n){
        buffer = (T*) realloc(buffer, new_n * sizeof(T));
        n = new_n;
    }
};

int main() {
    CircularBuffer<int> a(5);
    a.push_back(3);
    a.push_back(4);
    a.push_front(2);
    a.push_front(-2);
    a.push_back(5);
    a.push_back(3);
    a.push_back(4);
    a.pop_front();
// 3 4 5 3 4
    a.write();
    a.write_buffer();

    reverse(a.begin(), a.end());
    a.write_buffer();
    sort(a.begin(), a.end());
    a.write_buffer();

    a.resize(8);
    a.write_buffer();
}
