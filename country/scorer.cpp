#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef long double ld;
typedef pair<int,int> ii;

std::ifstream inp;
std::ifstream out;
std::ifstream con;

void ac(std::string reason="") {
  std::cout << "AC\n";
  if (reason != "") {
    std::cout << reason << '\n';
  }
  exit(0);
}

void wa(std::string reason="") {
  std::cout << "WA\n";
  if (reason != "") {
    std::cout << reason << '\n';
  }
  exit(0);
}

void ok(double points, std::string reason="") {
  std::cout << "OK\n";
  std::cout << points;
  if (reason != "") {
    std::cout << " " << reason << '\n';
  }
  exit(0);
}

void registerScorer(int argc, char* argv[]) {
  if (argc != 4) {
    std::cout << "Must be run with arguments [input-file] [output-file] [contestant-output]\n";
    exit(0);
  }

  inp = std::ifstream(argv[1]);
  out = std::ifstream(argv[2]);
  con = std::ifstream(argv[3]);
}

template<class T> inline void readStream(std::ifstream &ifs, T &t) { if (!(ifs >> t)) wa(); }
int readInt(std::ifstream &ifs) { int t; readStream(ifs, t); return t; }
ll readLong(std::ifstream &ifs) { ll t; readStream(ifs, t); return t; }
std::string readString(std::ifstream &ifs) { std::string s; readStream(ifs, s); return s; }
std::string readLine(std::ifstream &ifs) { std::string s; std::getline(ifs, s); return s; }
double readDouble(std::ifstream &ifs) { double x; readStream(ifs, x); return x; }


vi scores = {0,10,20,20,10,10,30};
int subtaskPoints(int subtask) {
    if(!subtask)return 0;
    for(int i=1;i<=6;i++){
        if(subtask == i){
            return (scores[i]/10)*3;
        }
        if(subtask == 6+i){
            return (scores[i]/10)*7;
        }
    }
    return 0;

}
int main(int argc, char* argv[]) {
    registerScorer(argc, argv);

    int subtask = readInt(inp);
    int points = subtaskPoints(subtask);

    ll ans0 = readLong(out), ans1 = readLong(out);
    ll con0 = readLong(con), con1 = readLong(con);

    if(subtask <= 6){
        if(con0 == ans0)ac();
        else wa();
    }

    if(subtask >  6){
        if(con1 == ans1)ac();
        else wa();
    }
}
