#ifndef _DEBUG
    #define PUGIXML_HEADER_ONLY
#endif

#include <Windows.h>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <cstdlib>
#include <clocale>
#include "pugixml-1.11/src/pugixml.hpp"

#define pw2(x) (x)*(x)
//#define double long double

using namespace std;
using namespace pugi;

vector<string> transport_types_names = {
    "Трамвай",
    "Троллейбус",
    "Автобус"
};

class TransportNode{
public:
    int id;
    string transport_type_name;
    pair<double, double> coordinates;
    string name;
    vector<string> routes;
    vector<string> streets;

    TransportNode(xml_node data){
        id = stoi(data.child_value("number"));
        transport_type_name = data.child_value("type_of_vehicle");
//        transport_type_name.pop_back();

        string coord_data = data.child_value("coordinates");
        string coord = "";
        for (auto& c: coord_data){
            if (c != ','){
//                if (c == '.') c = ',';
                coord += c;
            }
            else {
//                cout << coord << " ";
                coordinates.first = stod(coord) ;
                coord = "";
            }
        }
//        cout << coord << endl;
        coordinates.second = stod(coord) ;

        string routes_data = data.child_value("routes");
        string route = "";
        for (auto& c: routes_data){
            if (c != ',' && c != '.'){
                route += c;
            }
            else {
                routes.push_back(route);
                route = "";
            }
        }
        routes.push_back(route);

        string streets_data = data.child_value("location");
        string street = "";
        for (auto& c: streets_data){
            if (c != ','){
                street += c;
            }
            else {
                streets.push_back(street);
                street = "";
            }
        }
        streets.push_back(street);

        name = data.child_value("the_official_name");
    }

    int get_type(){
        for (int i = 0; i < transport_types_names.size(); i ++){
            if (transport_types_names[i] == transport_type_name) return i;
        }
        return -1;
    }
};

vector<unordered_map<string, vector<TransportNode> > > routes;
map<string, int> streets_cnt;

double dist(pair<double, double> p1, pair<double, double> p2){
    return sqrt((long double)pw2(p1.first - p2.first) + (long double)pw2(p1.second - p2.second));
}

double get_max_coord_len(int type, string route){
    vector<pair<double, double> > p;

    for (auto& c: routes[type][route]) p.push_back(c.coordinates);
    int n = p.size();

    const int INF = 2e18;

    vector<bool> used (n);
    vector<long double> min_e (n, INF), sel_e (n, -1);
    int last_v = -1;
    long double res = 0;
    min_e[0] = 0;
    for (int i = 0; i < n; ++ i) {
        int v = -1;
        for (int j = 0; j < n; ++ j)
            if (!used[j] && (v == -1 || min_e[j] < min_e[v]))
                v = j;

        if (min_e[v] == INF) {
//            cout << "No mst";
        }

        used[v] = true;
        if (sel_e[v] != -1){
            res += dist(p[v], p[sel_e[v]]);
        }

        for (int to = 0; to < n; ++ to)
            if (dist(p[v], p[to]) < min_e[to]) {
                min_e[to] = dist(p[v], p[to]);
                sel_e[to] = v;
            }
    }

    return res;
}

int main() {
    cout.precision(20);
    SetConsoleOutputCP(CP_UTF8);
//    SetConsoleOutputCP(1251);
    xml_document doc;
    xml_parse_result result = doc.load_file("data.xml");
    xml_node dataset = doc.child("dataset");
    routes.resize(transport_types_names.size());

    if (result){
        cout << "Data is loaded\n";
        for (xml_node station = dataset.first_child(); station; station = station.next_sibling()){
            TransportNode nw(station);
            for (auto& route: nw.routes){
                routes[nw.get_type()][route].push_back(nw);
            }

            for (auto& street: nw.streets){
                streets_cnt[street] ++;
            }
//            cout << nw.coordinates.first << " " << nw.coordinates.second << endl;
//            cout << "added" << endl;
        }

        cout << "Routes is created" << endl;
        vector<pair<int, string>> max_routes(transport_types_names.size(), {-1, ""});
        for (int i = 0; i < routes.size(); i ++){
            for (auto& p: routes[i]){
                max_routes[i] = max(max_routes[i], {p.second.size(), p.first});
            }
        }

        cout << "The most popular rotes: \n";
        for (int i = 0; i < max_routes.size(); i ++){
            cout << transport_types_names[i] << ": maxlen = " << max_routes[i].first << " name = " << max_routes[i].second << endl;
        }

        cout << "The most long routes: \n";
        for (int i = 0; i < transport_types_names.size(); i ++){
            double mx = -1000;
            string long_route = "";
            for (auto& p: routes[i]){
                if (get_max_coord_len(i, p.first) > mx){
                    mx = get_max_coord_len(i, p.first);
                    long_route = p.first;
                }
            }
            cout << transport_types_names[i] << " : " << long_route << " (" << mx << ")" << endl ;
        }

//        auto it = streets_cnt.begin();
////        it --;
//        cout << "The most popular street is " << (*it).first << " (" << (*it).second << ")" <<  endl;

        int mx = -1;
        string mx_street = "";
        for (auto& p: streets_cnt){
            if (p.first == "") continue;
            if (p.second > mx){
                mx = p.second;
                mx_street = p.first;
            }
        }
        cout << "The most popular street is " << mx_street << " (" << mx << ")" << endl;
    }
    else{
        cout << "Load error\n";
        cout << result.description() << endl;
    }





}
