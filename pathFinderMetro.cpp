#include <bits/stdc++.h>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <climits>
#include <ctime>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <iomanip>

using namespace std;

template <typename T>
class Heap {
public:
    std::vector<T> data;
    std::unordered_map<T, int> map;

    Heap(){

    }

    void add(T item) {
        data.push_back(item);
        map[item] = data.size() - 1;
        upheapify(data.size() - 1);
    }

    void upheapify(int ci) {
        int pi = (ci - 1) / 2;
        if (isLarger(data[ci], data[pi]) > 0) {
            swap(pi, ci);
            upheapify(pi);
        }
    }

    void swap(int i, int j) {
        T ith = data[i];
        T jth = data[j];

        data[i] = jth;
        data[j] = ith;
        map[ith] = j;
        map[jth] = i;
    }

    void display() {
        for (const T &item : data) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    int size() {
        return static_cast<int>(data.size());
    }

    bool isEmpty() {
        return size() == 0;
    }

    T remove() {
        swap(0, size() - 1);
        T rv = data.back();
        data.pop_back();
        downheapify(0);

        map.erase(rv);
        return rv;
    }

    void downheapify(int pi) {
        int lci = 2 * pi + 1;
        int rci = 2 * pi + 2;
        int mini = pi;

        if (lci < size() && isLarger(data[lci], data[mini]) > 0) {
            mini = lci;
        }

        if (rci < size() && isLarger(data[rci], data[mini]) > 0) {
            mini = rci;
        }

        if (mini != pi) {
            swap(mini, pi);
            downheapify(mini);
        }
    }

    T get() {
        return data[0];
    }

    int isLarger(T t, T o) {
        if (t > o) {
            return 1;
        } else if (t < o) {
            return -1;
        }
        return 0;
    }

    void updatePriority(T pair) {
        int index = map[pair];
        upheapify(index);
    }
};

class Vertex {
public:
    unordered_map<string, int> nbrs;
};

class Graph_M {
public:

    unordered_map<string, Vertex> vtces;

    void display_Stations() {
        cout << "\n***********************************************************************\n";
        vector<string> keys(vtces.begin(), vtces.end());
        int i = 1;
        for (const string& key : keys) {
            cout << i << ". " << key << endl;
            i++;
        }
        cout << "\n***********************************************************************\n";
    }
    void display_Map() 
    {
        cout << "   Delhi Metro Map" << endl;
        cout << "\t------------------" << endl;
        cout << "----------------------------------------------------" << endl;

        vector<string> keys(vtces.begin(), vtces.end());

        for (const string& key : keys) {
            string str = key + " =>\n";
            Vertex vtx = vtces[key];
            vector<string> vtxnbrs(vtx.nbrs.begin(), vtx.nbrs.end());

            for (const string& nbr : vtxnbrs) {
                str = str + "\t" + nbr + "\t";
                if (nbr.length() < 16)
                    str = str + "\t";
                if (nbr.length() < 8)
                    str = str + "\t";
                str = str + to_string(vtx.nbrs[nbr]) + "\n";
            }
            cout << str;
        }
        cout << "\t------------------" << endl;
        cout << "---------------------------------------------------" << endl;
    }
    void addVertex(const string& vname) 
    {
        Vertex vtx;
        vtces[vname] = vtx;
    }
    void addEdge(const string& vname1, const string& vname2, int value) {
        Vertex& vtx1 = vtces[vname1]; 
        Vertex& vtx2 = vtces[vname2]; 

        if (vtx1.nbrs.find(vname2) != vtx1.nbrs.end() || vtx2.nbrs.find(vname1) != vtx2.nbrs.end()) {
            return;
        }

        vtx1.nbrs[vname2] = value;
        vtx2.nbrs[vname1] = value;
    }
    bool containsVertex(string &vname)
    {
        return vtces.count(vname);
    }
    class DijkstraPair {
    public:
        string vname;
        string psf;
        int cost;

        bool operator<(const DijkstraPair& other) const {
            // We reverse the comparison to maintain a max-heap behavior
            return cost > other.cost;
        }
    };
    class Pair {
    public:
        string vname;
        string psf;
        int min_dis;
        int min_time;
    };
    int dijkstra(const string& src, const string& des, bool nan) {
        int val = 0;
        vector<string> ans;
        unordered_map<string, DijkstraPair> map;

        Heap<DijkstraPair> heap; // Use your Heap implementation

        for (const auto& entry : vtces) {
            const string& key = entry.first;
            DijkstraPair np;
            np.vname = key;
            np.cost = INT_MAX;

            if (key == src) {
                np.cost = 0;
                np.psf = key;
            }

            heap.add(np);
            map[key] = np;
        }

        while (!heap.isEmpty()) {
            DijkstraPair rp = heap.remove();
            
            if (rp.vname == des) {
                val = rp.cost;
                break;
            }
            
            map.erase(rp.vname);

            ans.push_back(rp.vname);
            
            Vertex v = vtces[rp.vname];
            for (const auto& nbrEntry : v.nbrs) {
                const string& nbr = nbrEntry.first;
                if (map.count(nbr)) {
                    int oc = map[nbr].cost;
                    Vertex k = vtces[rp.vname];
                    int nc;
                    if (nan) {
                        nc = rp.cost + 120 + 40 * k.nbrs[nbr];
                    } else {
                        nc = rp.cost + k.nbrs[nbr];
                    }

                    if (nc < oc) {
                        DijkstraPair& gp = map[nbr];
                        gp.psf = rp.psf + nbr;
                        gp.cost = nc;

                        heap.updatePriority(gp);
                    }
                }
            }
        }
        return val;
    }
    bool containsEdge(const string& vname1, const string& vname2)
    {
        auto vtx1It = vtces.find(vname1);
        auto vtx2It = vtces.find(vname2);

        if (vtx1It == vtces.end() || vtx2It == vtces.end() || vtx1It->second.nbrs.find(vname2) == vtx1It->second.nbrs.end()) {
            return false;
        }
        return true;
    }
    bool hasPath(const string& vname1, const string& vname2, unordered_map<string, bool>& processed) {
        if (containsEdge(vname1, vname2)) {
            return true;
        }

        processed[vname1] = true;

        const Vertex& vtx = vtces[vname1];
        vector<string> nbrs;
        for (const auto& nbrEntry : vtx.nbrs) {
            nbrs.push_back(nbrEntry.first);
        }

        for (const string& nbr : nbrs) {
            if (processed.find(nbr) == processed.end() && hasPath(nbr, vname2, processed)) {
                return true;
            }
        }

        return false;
    }
    vector<string> get_Interchanges(const string& str) 
    {
        vector<string> arr;
        size_t startPos = 0;
        size_t endPos = str.find("  ");
        
        while (endPos != string::npos) {
            arr.push_back(str.substr(startPos, endPos - startPos));
            startPos = endPos + 2; // Move to the next position after "  "
            endPos = str.find("  ", startPos);
        }
        
        // Process the last part
        arr.push_back(str.substr(startPos));

        vector<string> result;
        result.push_back(arr[0]);
        int count = 0;

        for (size_t i = 1; i < arr.size() - 1; i++) {
            size_t index = arr[i].find('~');
            string s = arr[i].substr(index + 1);

            if (s.length() == 2) {
                string prev = arr[i - 1].substr(arr[i - 1].find('~') + 1);
                string next = arr[i + 1].substr(arr[i + 1].find('~') + 1);

                if (prev == next) {
                    result.push_back(arr[i]);
                } else {
                    result.push_back(arr[i] + " ==> " + arr[i + 1]);
                    i++;
                    count++;
                }
            } else {
                result.push_back(arr[i]);
            }
        }

        result.push_back(to_string(count));
        result.push_back(arr[arr.size() - 1]);
        return result;
    }
    string Get_Minimum_Distance(const string& src, const string& dst) {
        int min = INT_MAX;
        string ans = "";
        unordered_map<string, bool> processed;
        list<Pair> stack;

        Pair sp;
        sp.vname = src;
        sp.psf = src + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;
        
        stack.push_front(sp);

        while (!stack.empty()) {
            Pair rp = stack.front();
            stack.pop_front();

            if (processed.count(rp.vname)) {
                continue;
            }

            processed[rp.vname] = true;

            if (rp.vname == dst) {
                int temp = rp.min_dis;
                if (temp < min) {
                    ans = rp.psf;
                    min = temp;
                }
                continue;
            }

            Vertex rpvtx = vtces[rp.vname];
            for (const auto& neighbor : rpvtx.nbrs) {
                if (!processed.count(neighbor.first)) {
                    Pair np;
                    np.vname = neighbor.first;
                    np.psf = rp.psf + neighbor.first + "  ";
                    np.min_dis = rp.min_dis + neighbor.second; 
                    stack.push_front(np);
                }
            }
        }

        ans = ans + to_string(min);
        return ans;
    }
    string Get_Minimum_Time(const string& src, const string& dst) 
    {
        int min = INT_MAX;
        string ans = "";
        unordered_map<string, bool> processed;
        list<Pair> stack;

        
        Pair sp;
        sp.vname = src;
        sp.psf = src + "  ";
        sp.min_dis = 0;
        sp.min_time = 0;
        
        stack.push_front(sp);

        while (!stack.empty()) {
            Pair rp = stack.front();
            stack.pop_front();

            if (processed.count(rp.vname)) {
                continue;
            }

            processed[rp.vname] = true;

            if (rp.vname == dst) {
                int temp = rp.min_time;
                if (temp < min) {
                    ans = rp.psf;
                    min = temp;
                }
                continue;
            }

            Vertex rpvtx = vtces[rp.vname];
            for (const auto& neighbor : rpvtx.nbrs) {
                if (!processed.count(neighbor.first)) {
                    Pair np;
                    np.vname = neighbor.first;
                    np.psf = rp.psf + neighbor.first + "  ";
                    np.min_time = rp.min_time + 120 + 40 * neighbor.second; 
                    stack.push_front(np);
                }
            }
        }

        double minutes = ceil((double)min / 60);
        ans = ans + to_string(minutes);
        return ans;
    }

};

void Create_Metro_Map(Graph_M& g) 
{
    g.addVertex("Noida Sector 62~B");
    g.addVertex("Botanical Garden~B");
    g.addVertex("Yamuna Bank~B");
    g.addVertex("Rajiv Chowk~BY");
    g.addVertex("Vaishali~B");
    g.addVertex("Moti Nagar~B");
    g.addVertex("Janak Puri West~BO");
    g.addVertex("Dwarka Sector 21~B");
    g.addVertex("Huda City Center~Y");
    g.addVertex("Saket~Y");
    g.addVertex("Vishwavidyalaya~Y");
    g.addVertex("Chandni Chowk~Y");
    g.addVertex("New Delhi~YO");
    g.addVertex("AIIMS~Y");
    g.addVertex("Shivaji Stadium~O");
    g.addVertex("DDS Campus~O");
    g.addVertex("IGI Airport~O");
    g.addVertex("Rajouri Garden~BP");
    g.addVertex("Netaji Subhash Place~PR");
    g.addVertex("Punjabi Bagh West~P");

    g.addEdge("Noida Sector 62~B", "Botanical Garden~B", 8);
    g.addEdge("Botanical Garden~B", "Yamuna Bank~B", 10);
    g.addEdge("Yamuna Bank~B", "Vaishali~B", 8);
    g.addEdge("Yamuna Bank~B", "Rajiv Chowk~BY", 6);
    g.addEdge("Rajiv Chowk~BY", "Moti Nagar~B", 9);
    g.addEdge("Moti Nagar~B", "Janak Puri West~BO", 7);
    g.addEdge("Janak Puri West~BO", "Dwarka Sector 21~B", 6);
    g.addEdge("Huda City Center~Y", "Saket~Y", 15);
    g.addEdge("Saket~Y", "AIIMS~Y", 6);
    g.addEdge("AIIMS~Y", "Rajiv Chowk~BY", 7);
    g.addEdge("Rajiv Chowk~BY", "New Delhi~YO", 1);
    g.addEdge("New Delhi~YO", "Chandni Chowk~Y", 2);
    g.addEdge("Chandni Chowk~Y", "Vishwavidyalaya~Y", 5);
    g.addEdge("New Delhi~YO", "Shivaji Stadium~O", 2);
    g.addEdge("Shivaji Stadium~O", "DDS Campus~O", 7);
    g.addEdge("DDS Campus~O", "IGI Airport~O", 8);
    g.addEdge("Moti Nagar~B", "Rajouri Garden~BP", 2);
    g.addEdge("Punjabi Bagh West~P", "Rajouri Garden~BP", 2);
    g.addEdge("Punjabi Bagh West~P", "Netaji Subhash Place~PR", 3);
}

vector<string> printCodelist(Graph_M g) 
{
    cout << "List of station along with their codes:\n";
    vector<string> keys(g.vtces.begin(), g.vtces.end());
    int i = 1, j = 0, m = 1;
    string temp = "";
    vector<string> codes(keys.size());
    char c;
    for (const string& key : keys) {
        istringstream stname(key);
        codes[i - 1] = "";
        j = 0;
        while (stname >> temp) {
            c = temp[0];
            while (c >= '0' && c <= '9') {
                codes[i - 1] += c;
                j++;
                c = temp[j];
            }
            if ((c < '0' || c > '9') && c < 'a') {
                codes[i - 1] += c;
            }
        }
        if (codes[i - 1].length() < 2)
            codes[i - 1] += toupper(temp[1]);

        cout << i << ". " << key << "\t";
        if (key.length() < (22 - m))
            cout << "\t";
        if (key.length() < (14 - m))
            cout << "\t";
        if (key.length() < (6 - m))
            cout << "\t";
        cout << codes[i - 1] << endl;
        i++;
        if (i == static_cast<int>(pow(10, m)))
            m++;
    }
    return codes;
}

int main() {
    Graph_M g;
    Create_Metro_Map(g);

    cout << "\n\t\t\t****WELCOME TO THE METRO APP*****";

    while (true) {
        cout << "\t\t\t\t~~LIST OF ACTIONS~~\n\n";
        cout << "1. LIST ALL THE STATIONS IN THE MAP\n";
        cout << "2. SHOW THE METRO MAP\n";
        cout << "3. GET SHORTEST DISTANCE FROM A 'SOURCE' STATION TO 'DESTINATION' STATION\n";
        cout << "4. GET SHORTEST TIME TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION\n";
        cout << "5. GET SHORTEST PATH (DISTANCE WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION\n";
        cout << "6. GET SHORTEST PATH (TIME WISE) TO REACH FROM A 'SOURCE' STATION TO 'DESTINATION' STATION\n";
        cout << "7. EXIT THE MENU\n";
        cout << "\nENTER YOUR CHOICE FROM THE ABOVE LIST (1 to 7) : ";

        int choice = -1;
        cin >> choice;
        cout << "\n***********************************************************\n";

        if (choice == 7) {
            exit(0);
        }

        switch(choice) {
            case 1:
            {
                g.display_Stations();
                break;
            }

            case 2:
            {
                g.display_Map();
                break;
            }
            case 3:
            {
                vector<string> keys(g.vtces.begin(), g.vtces.end());
                vector<string> codes = printCodelist(g);
                cout << "\n1. TO ENTER SERIAL NO. OF STATIONS\n2. TO ENTER CODE OF STATIONS\n3. TO ENTER NAME OF STATIONS\n";
                cout << "ENTER YOUR CHOICE:";
                int ch;
                cin >> ch;
                int j;

                string st1 = "", st2 = "";
                cout << "ENTER THE SOURCE AND DESTINATION STATIONS";
                if (ch == 1) {
                    int serial1, serial2;
                    cin >> serial1 >> serial2;
                    st1 = keys[serial1 - 1];
                    st2 = keys[serial2 - 1];
                } else if (ch == 2) {
                    string a, b;
                    cin >> a >> b;
                    for(char &c: a) c = _toupper(c);
                    //a = _toupper(a);
                    for (j = 0; j < keys.size(); j++) {
                        if (a == codes[j]) {
                            break;
                        }
                    }
                    st1 = keys[j];
                    cin >> b;
                    for(char &c: b) c = _toupper(c);
                    for (j = 0; j < keys.size(); j++) {
                        if (b == codes[j]) {
                            break;
                        }
                    }
                    st2 = keys[j];
                } else if (ch == 3) {
                    cin.ignore();
                    getline(cin, st1);
                    getline(cin, st2);
                } else {
                    cout << "Invalid choice" << endl;
                    exit(0);
                }

                unordered_map<string, bool> processed;
                if (!g.containsVertex(st1) || !g.containsVertex(st2) || !g.hasPath(st1, st2, processed)) {
                    cout << "THE INPUTS ARE INVALID" << endl;
                } else {
                    cout << "SHORTEST DISTANCE FROM " << st1 << " TO " << st2 << " IS " << g.dijkstra(st1, st2, false) << "KM\n";
                }
                break;
            }

            case 4:
            {
                cout << "ENTER THE SOURCE STATION: ";
                string sat1, sat2;
                cin.ignore();
                getline(cin, sat1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, sat2);

                unordered_map<string, bool> processed1;
                double shortestTime = g.dijkstra(sat1, sat2, true);
                cout << "SHORTEST TIME FROM (" << sat1 << ") TO (" << sat2 << ") IS " << shortestTime / 60 << " MINUTES\n\n";
                break;
            }

            case 5:
            {
                cout << "ENTER THE SOURCE AND DESTINATION STATIONS" << endl;
                string s1, s2;
                cin.ignore();
                getline(cin, s1);
                getline(cin, s2);

                unordered_map<string, bool> processed2;
                if (!g.containsVertex(s1) || !g.containsVertex(s2) || !g.hasPath(s1, s2, processed2)) {
                    cout << "THE INPUTS ARE INVALID" << endl;
                } else {
                    vector<string> str = g.get_Interchanges(g.Get_Minimum_Distance(s1, s2));
                    int len = str.size();
                    cout << "SOURCE STATION : " << s1 << endl;
                    cout << "DESTINATION STATION : " << s2 << endl;
                    cout << "DISTANCE : " << str[len - 1] << endl;
                    cout << "NUMBER OF INTERCHANGES : " << str[len - 2] << endl;
                    cout << "~~~~~~~~~~~~~" << endl;
                    cout << "START  ==>  " << str[0] << endl;
                    for (int i = 1; i < len - 3; i++) {
                        cout << str[i] << endl;
                    }
                    cout << str[len - 3] << "   ==>    END" << endl;
                    cout << "~~~~~~~~~~~~~" << endl;
                }
                break;
            }

            case 6:
            {
                cout << "ENTER THE SOURCE STATION: ";
                string ss1, ss2;
                cin.ignore();
                getline(cin, ss1);
                cout << "ENTER THE DESTINATION STATION: ";
                getline(cin, ss2);

                unordered_map<string, bool> processed3;
                if (!g.containsVertex(ss1) || !g.containsVertex(ss2) || !g.hasPath(ss1, ss2, processed3)) {
                    cout << "THE INPUTS ARE INVALID" << endl;
                } else {
                    vector<string> str = g.get_Interchanges(g.Get_Minimum_Time(ss1, ss2));
                    int len = str.size();
                    cout << "SOURCE STATION : " << ss1 << endl;
                    cout << "DESTINATION STATION : " << ss2 << endl;
                    cout << "TIME : " << str[len - 1] << " MINUTES" << endl;
                    cout << "NUMBER OF INTERCHANGES : " << str[len - 2] << endl;
                    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
                    cout << "START  ==>  " << str[0] << " ==>  ";
                    for (int i = 1; i < len - 3; i++) {
                        cout << str[i] << endl;
                    }
                    cout << str[len - 3] << "   ==>    END" << endl;
                    cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
                }
                break;
            }

            default:
            {
                cout << "Please enter a valid option! ";
                cout << "The options you can choose are from 1 to 6.\n";
            }
        }
    }
    return 0;
}
