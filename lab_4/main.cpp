#include <iostream>
#include <vector>
//using namespace std;


class Algo{
public:
    template <class InputIt, class UnaryPredicate>
    static bool all_of(InputIt first, InputIt last, UnaryPredicate p){
        for (; first != last; first ++)
            if (!p(*first)) return false;
        return true;
    }

    template<class InputIt, class UnaryPredicate>
    static bool any_of(InputIt first, InputIt last, UnaryPredicate p){
        for (; first != last; first ++)
            if (p(*first)) return true;
        return false;
    }

    template<class InputIt, class UnaryPredicate>
    static bool none_of(InputIt first, InputIt last, UnaryPredicate p){
        for (; first != last; first ++){
            if (p(*first)) return false;
        }
        return true;
    }

    template<class InputIt, class UnaryPredicate>
    static bool one_of(InputIt first, InputIt last, UnaryPredicate p){
        for (; first != last; first ++){
            if (p(*first)){
                return none_of(first + 1, last, p);
            }
        }
        return false;
    }

    template<class InputIt, class BinaryPredicate>
    static bool is_sorted(InputIt first, InputIt last, BinaryPredicate p){
        for (; first != last - 1; first ++){
            if (p(*first, *(first + 1))) return false;
        }
        return true;
    }

    template<class InputIt, class UnaryPredicate>
    static bool is_partitioned(InputIt first, InputIt last, UnaryPredicate p){
//        for (; first != last; first ++){
//            if (all_of(first, first - 1, p) && none_of(first + 1, last, p)) return true;
//        }
//        return false;
        bool fl = false;
        for (; first != last; first ++) {
            if (p(*first) && fl) return false;
            if (!p(*first) && !fl)fl = true;
        }
        return true;
    }

    template<class InputIt, class T>
    static InputIt find_not(InputIt first, InputIt last, T val){
        for (; first != last; first ++){
            if (*first != val) return first;
        }
    }

    template<class InputIt, class T>
    static InputIt find_backward(InputIt first, InputIt last, T val){
        first --;
        last --;
        for (; last != first; last --){
            if (*last == val) return last;
        }
    }

    template<class InputIt, class UnaryPredicate>
    static bool is_palindrome(InputIt first, InputIt last, UnaryPredicate p){
        for (auto it = first; it != last; it ++){
            if (p(*it) != p(*(last - (it - first + 1)))) return false;
        }
        return true;
    }
};

class AlgoTests{
private:
    static bool is_odd(int x){return x % 2 == 0;}

    static void write_bool(bool x){std::cout << (x ? "Yes": "No") << std::endl;}

    static bool more(int x, int y){return x > y;}

    class CPoint{
    public:
        double x, y;
        CPoint(double x_, double y_): x(x_), y(y_){}
    };

    static bool plus(CPoint a){
        return a.x > 0 && a.y > 0;
    }
public:
    static void all_of_test(){
        std::vector<int> a = {0, 2, 4, 6};
        write_bool(Algo::all_of(a.begin(), a.end(), is_odd));
    }

    static void any_of_test(){
        std::vector<int> a = {1, 3, 5, 7, 19};
        write_bool(Algo::any_of(a.begin(), a.end(), is_odd));
    }

    static void none_of_test(){
        std::vector<int> a = {1, 3, 5, 7, 19};
        write_bool(Algo::none_of(a.begin(), a.end(), is_odd));
    }

    static void one_of_test(){
        std::vector<int> a = {1, 3, 1, 9, 2};
        write_bool(Algo::one_of(a.begin(), a.end(), is_odd));
    }

    static void is_sorted_test(){
        std::vector<int> a = {1, 3, 3, 4};
        write_bool(Algo::is_sorted(a.begin(), a.end(), more));
    }

    static void is_partitioned_test(){
        std::vector<int> a = {1, 3, 5, 7, 9};
        write_bool(Algo::is_partitioned(a.begin(), a.end(), is_odd));
    }

    static void find_not_test(){
        std::vector<int> a = {2, -1902, 2, 5};
        std::cout << Algo::find_not(a.begin(), a.end(), 2) - a.begin() << std::endl;
    }

    static void find_backward_test(){
        std::vector<int> a = {1, 4, 2, 9};
        std::cout << Algo::find_backward(a.begin(), a.end(), 2) - a.begin() << std::endl;
    }

    static void is_palindrome_test(){
        std::vector<int> a = {1, 4, 8, 6, 3};
        write_bool(Algo::is_palindrome(a.begin(), a.end(), is_odd));
    }

    static void user_class_test(){
        std:: vector<CPoint> a = {{-1, 2}, {-1, -2}, {3, -1}};
        write_bool(Algo::any_of(a.begin(), a.end(), plus));
    }

    void operator()() const{
        std::cout << "Testing start" << std::endl;
        all_of_test();
        any_of_test();
        none_of_test();
        one_of_test();
        is_sorted_test();
        is_partitioned_test();
        find_not_test();
        find_backward_test();
        is_palindrome_test();
        user_class_test();
    }
};


int main(){
    AlgoTests()();
}
