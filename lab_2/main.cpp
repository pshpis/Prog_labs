#include <iostream>
#include <vector>
#include <valarray>
#include <unordered_map>
#include <unordered_set>

using namespace std;

double eps = 1e-8;

double pow2(double x){
    return x * x;
}

double fast_pow(double x, int pow){
    if (pow == 0) return 1;
    if (pow == 1) return x;
    if (pow % 2 == 0) return pow2(fast_pow(x, pow / 2));
    else return pow2(fast_pow(x, pow / 2)) * x;
}

class Polynomial {
private:
    mutable unordered_map<int, double> indexes;
public:
    explicit Polynomial(const vector<int> &inds) {
        for (int i = 0; i < inds.size(); i++)
            if (inds[i] != 0) indexes[i] = inds[i];
    }

    Polynomial(const Polynomial &p) = default;

    explicit Polynomial(const unordered_map<int, double>& inds){
        indexes = inds;
    }

    Polynomial() = default;

    Polynomial &operator=(const Polynomial &p) = default;

    Polynomial &operator=(const vector<double> &inds) {
        for (int i = 0; i < inds.size(); i++)
            if (inds[i] != 0) indexes[i] = inds[i];
        return *this;
    }

    Polynomial &operator=(double x) {
        indexes[0] = x;
        return *this;
    }

    [[nodiscard]] int size() const {
        return indexes.size();
    }

    double& operator[](int i){
        return indexes[i];
    }

    const double& operator[](int i) const{
        return indexes[i];
    }

    double at(int i) const{
        return indexes.at(i);
    }

    void del_zero(){
        for(auto it = indexes.begin(); it != indexes.end(); ) {
            if (it->second == 0)
                it = indexes.erase(it);
            else
                ++it;
        }
    }

    bool operator ==(const Polynomial& other) const{
        del_zero();
        other.del_zero();
        return indexes == other.indexes;
    }

    bool operator !=(Polynomial& other){
        return !(other == *this); // быстро ли работает звездочка?
    }

    Polynomial& operator +(){
        return *this;
    }

    Polynomial operator -() const{
        auto nw = indexes;
        for (auto& p: nw) p.second = -p.second;
        return Polynomial(nw);
    }

    Polynomial operator +(const Polynomial& other) const{
        auto nw = indexes;
        unordered_set<int> used_indexes;
        for (auto& p: nw){
            if (other.indexes.count(p.first) > 0) p.second += other.at(p.first);
            used_indexes.insert(p.first);
        }

        for (auto& p: other.indexes){
            if (used_indexes.count(p.first) == 0) nw[p.first] += p.second;
        }

        auto res = Polynomial(nw);
        res.del_zero();
        return res;
    }

    Polynomial operator -(const Polynomial& other) const{
        return *this + (-other);
    }

    Polynomial& operator +=(const Polynomial& other){
        *this = *this + other;
        return *this;
    }

    Polynomial& operator -=(const Polynomial& other){
        *this = *this - other;
        return *this;
    }

    Polynomial operator *(double x){
        auto nw = indexes;
        for (auto& i: nw) i.second *= x;
        return Polynomial(nw);
    }

    Polynomial operator* (const Polynomial& other){
        unordered_map<int, double> nw;
        for (auto p1: indexes){
            for (auto p2: other.indexes){
                nw[p1.first + p2.first] += p1.second * p2.second;
            }
        }
        return Polynomial(nw);
    }

    Polynomial& operator *=(const Polynomial& other){
        *this = *this * other;
        return *this;
    }

    Polynomial operator /(double x){
        return *this * (1 / x);
    }

    void operator *=(double x){
        *this = *this * x;
    }

    void operator /=(double x){
        *this = *this / x;
    }

    friend ostream& operator <<(ostream& s, const Polynomial& a);
    friend istream& operator >>(istream& s, Polynomial& a);

    void operator <<(int k){
        unordered_map<int, double> nw;
        for (auto p: indexes) nw[p.first + k] = p.second;
        indexes = nw;
    }

    void operator >>(int k){
        unordered_map<int, double> nw;
        for (auto p: indexes) nw[p.first - k] = p.second;
        indexes = nw;
    }

    [[nodiscard]] double value(double x) const{
        double res = 0;
        for (auto p: indexes){
            res += fast_pow(x, p.first) * p.second;
        }
        return res;
    }

};

ostream& operator <<(ostream& s, const Polynomial& a){
    for (auto p: a.indexes) s << p.first << " " << p.second << endl;
    return s;
}

istream& operator >>(istream& s, Polynomial& a){
    int n;
    s >> n;
    unordered_map<int, double> arr;
    for (int i = 0; i < n; i ++) s >> arr[i];
    a = Polynomial(arr);
    return s;
}


int main() {
    Polynomial a;
    cin >> a;
    cout << a << "------" << endl;
    a *= 3;
    a /= 2;
    cout << a;
    cout << a.value(2);

}