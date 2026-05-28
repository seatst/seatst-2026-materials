#include "trip.h"
#include <cassert>
#include <cstdio>
#include <string>
#include <ctime>


int main() {
    int n,m,a,b;
    assert(1 == scanf("%d", &n));
    std::vector<int> u, v;
    if(n>1'000'000){
		clock_t program_start_time = clock();

        int tc_num;
        assert(1 == scanf("%d", &tc_num));
        GraphTestCase tc;
        if(tc_num==1){
            tc = AdversarialGenerator::generate_pyramid(n);
        }
        else if(tc_num==2){
            //5 layers
            tc = AdversarialGenerator::generate_stalactite(n,10'000'000,5);
        }
        else if(tc_num==3){
            //4000 layers
            tc = AdversarialGenerator::generate_stalactite(n,10'000'000,4000);
        }
        m = tc.M;
        a = tc.A;
        b = tc.B;
        assert(n<=10'000'000 && m<=10'000'000);
        swap(u,tc.U);
        swap(v,tc.V);
        
		for (int i = 0; i < m; ++i) {
			--u[i];
			--v[i];
		}
		
		// Burn CPU cycles until exactly 1.5 seconds of CPU time have passed
		// since the program started.
		while (double(clock() - program_start_time) / CLOCKS_PER_SEC < 1.5) {
			// Busy wait - forces the OJ to register CPU usage
		}
    }
    else{
        assert(1 == scanf("%d", &m));
        assert(1 == scanf("%d", &a));
        assert(1 == scanf("%d", &b));
        u.resize(m); v.resize(m);
        for (int k = 0; k < m; k++){
            assert(1 == scanf("%d", &u[k]));
            assert(1 == scanf("%d", &v[k]));
        }
        
		for (int i = 0; i < m; ++i) {
			--u[i];
			--v[i];
		}
    }
    fclose(stdin);

    int ret = min_distinct(n,m,a-1,b-1,u,v);

    printf("%d\n", ret);

    fclose(stdout);

    return 0;
}
