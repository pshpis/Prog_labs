#include <iostream>
#include <vector>
#include <valarray>

using namespace std;

class Polynomial{
private:
    vector<double> indexes;
public:
    explicit Polynomial(const vector<double>& inds): indexes(inds){}

    Polynomial(const Polynomial& p) = default;

    Polynomial(const initializer_list<double> list): indexes(list){}

    Polynomial() = default;

    ~Polynomial(){
        indexes.clear();
    }

    Polynomial& operator =(const Polynomial& other)= default;

    Polynomial& operator =(const vector<double>& ids){
        indexes = ids;
        return *this;
    }

    [[nodiscard]] int size() const{
        return indexes.size();
    }

    double& operator[](int i) const{
        return (double &)indexes[i];
    }

    bool operator ==(const Polynomial& other) const{
        if (size() == other.size()){
            for (int i = 0; i < size(); i ++){
                if (indexes[i] != other[i]) return false;
            }
            return true;
        }
        return false;
    }

    bool operator !=(const Polynomial& other) const{
        return !(other == *this);
    }

    Polynomial& operator +(){
        return *this;
    }

    Polynomial operator -() const{
        auto nw = indexes;
        for (auto& i: nw) i = -i;
        return Polynomial(nw);
    }

    Polynomial operator +(const Polynomial& other) const{
        auto nw = other.size() > size()? other.indexes : indexes;
        for (int i = 0; i < nw.size(); i ++){
            nw[i] += other.size() > size()? indexes[i] : other.indexes[i];
        }
        return Polynomial(nw);
    }

    Polynomial operator -(const Polynomial& other) const{
        return *this + (-other);
    }

    void operator +=(const Polynomial& other){
        *this = *this + other;
    }

    void operator -=(const Polynomial& other){
        *this = *this - other;
    }

    Polynomial operator *(double x){
        auto nw = indexes;
        for (auto& i: nw) i *= x;
        return Polynomial(nw);
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
        vector<double> nw(size() + k, 0);
        for (int i = 0; i < size(); i ++) nw[i + k] = indexes[i];
        indexes = nw;
    }

    void operator >>(int k){
        vector<double> nw(size() - k, 0);
        for (int i = k; i < size(); i ++) nw[i - k] = indexes[i];
        indexes = nw;
    }

    [[nodiscard]] double value(double x) const{
        if (size() == 0) return 0;
        double x_pow = 1;
        double res = 0;
        for (int i = 0; i < size(); i ++){
            res += x_pow * indexes[i];
            x_pow *= x;
        }
        return res;
    }

};

ostream& operator <<(ostream& s, const Polynomial& a){
    for (int i = 0; i < a.size(); i ++) s << a[i] << " ";
    s << endl;
    return s;
}

istream& operator >>(istream& s, Polynomial& a){
    int n;
    s >> n;
    vector<double> arr(n);
    for (int i = 0; i < n; i ++) s >> arr[i];
    a = Polynomial(arr);
    return s;
}

int main() {
    Polynomial a;
    cin >> a;
    a *= 3; `
    a /= 2;
    cout << a;
    cout << a.value(2);
}
