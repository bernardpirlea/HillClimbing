#include <iostream>
#include <time.h>
#include <math.h>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

#define PI 3.14159265

float Dixon_Price(vector<float> x, int size)
{
    float result = pow(x[0] - 1, 2);

    for(int i = 1; i < size; ++i)
    {
        result += i * pow (( 2 * pow(x[i], 2) - x[i-1]), 2);
    }

    return result;
}

float Rosenbrock(vector<float> x, int size)
{
    float result = 0;

    for(int i = 0; i < size - 1; ++i)
    {
        result += 100 * pow (x[i+1] - pow(x[i], 2), 2) + pow(x[i]-1, 2);
    }

    return result;
}

float sphere(vector<float> x, int size)
{
    float result = 0;

    for(int i = 0; i < size; i++)
    {
        result += pow(x[i],2);
    }
    return result;
}

float Rastrigin(vector<float> x, int size)
{
    float result = 10 * size;

    for(int i = 0; i < size; ++i)
    {
        result += pow(x[i],2) - 10 * cos(2*PI*x[i]);
    }
    return result;
}

float convert_binary(int x_binar[], int L, float a, float b)
{
    float x = 0;
    /*
    for(int i = 0; i < L; ++i)
    {
        cout<<x_binar[i];
    }
    cout<<endl;
     */
    for(int i = 0; i < L; ++i)
    {
        x = 2 * x;
        x += x_binar[i];
    }
    //cout<<endl;
    x = x / pow(2,L);
    x *= (b-a);
    x += a;
    return x;
}

vector<int> neighbor(vector<int> x_binar, int pos)
{
    vector<int> candidat;
    candidat = x_binar;
    candidat[pos] = !candidat[pos];
    return candidat;
}


vector<float> deco(vector<int> x_binar, int L, int D, float a, float b)
{
    vector<float> x;
    int vec[L];
    for(int i = 0, j = 0; i < L*D; ++i, ++j){
        if(i % L == 0 && i > 0){
            j = 0;
            x.push_back(convert_binary(vec, L, a, b));
        }
        vec[j] = x_binar.at(i);
    }
    x.push_back(convert_binary(vec, L, a, b));
    return x;
}

void afisare(vector<int> vect, int D, int L){
    // afisare biti solutie
    for (int i = 0; i < D*L; i++) {
        if(i % L == 0 && i > 0) cout<< "\n";
        cout<< vect[i]<< " ";
    }
    cout<<endl;
}

vector<int> Improve(vector<int> x_binar, int L, int D, float a, float b, int optiune)
{
    float min_gasit;
    vector<float> x_arg;
    float fc;
    vector<int> candidat;
    vector<float> arg_cand;
    vector<int> best;
    
    x_arg = deco(x_binar, L, D, a, b);
    min_gasit = Rosenbrock(x_arg, D);
    
    for(int i = 0; i < L*D; ++i)
    {
        candidat = neighbor(x_binar, i);
        
        /* afisare candidati
        for (int i = 0; i < D*L; i++) {
            if(i % L == 0 && i > 0) cout<< "\n";
            cout<< candidat[i]<< " ";
        }
        cout<<endl;
         */
        
        //calculam functia in candidat
        arg_cand = deco(candidat, L, D, a, b);
        /*
        for(int i = 0; i < D; ++i)
        {
            cout<<arg_cand[i]<<" ";
        }
        cout<<endl;
         */
        fc = Rosenbrock(arg_cand, D);
        
        
        if(fc < min_gasit && optiune == 0)
        {
            min_gasit = fc;
            best = candidat;
            break;
        }
        if(fc < min_gasit && optiune == 1)
        {
            min_gasit = fc;
            best = candidat;
        }
    }
    if(best.empty())
    {
        return x_binar;
    }
    //cout<<min_gasit;
    return best;
}

vector<int> Hill_Climbing(int L, int D, float a, float b, int optiune)
{
    float candidat_value;
    float best_value;
    float new_sol_value;
    vector<int> candidat;
    vector<int> best;
    vector<int> new_sol;
    vector<float> x_arg;
    
    //generam solutia
    for (int i = 0; i < D * L; i++) {
            best.push_back(rand() % 2);
    }
    
    //afisare(x_binar, D, L);
    x_arg = deco(best, L, D, a, b);
    /*
    for(int i = 0; i < D; ++i)
    {
        cout<<x_arg[i]<<" ";
    }
    cout<<endl;*/
    //evaluam valoarea functiei in x random
    
    best_value = Rosenbrock(x_arg, D);
    
    //cout<<first_Fvalue<<endl;
    
    
    //afisare(new_sol, D, L);
    int t = 1000;
    while(t)
    {
        int restart = 10;
        //select candidate
        for (int i = 0; i < D * L; i++) {
                   candidat.push_back(rand() % 2);
           }
        // eval candidat
        x_arg = deco(candidat, L, D, a, b);
        candidat_value = Rosenbrock(x_arg, D);
        while(restart){
            new_sol = Improve(candidat, L, D, a, b, optiune);
            x_arg = deco(new_sol,L,D,a,b);
            new_sol_value = Rosenbrock(x_arg, D);
            if(new_sol_value < candidat_value)
            {
                candidat = new_sol;
                
            }
            else
            {
                restart = 0;
            }
        }
        --t;
        if(candidat_value < best_value)
        {
            best = candidat;
        }
        candidat.clear();
    }
    return best;
}

vector<int> Simulated_Annealing(int L, int D, float a, float b)
{
    int t = 100;
    int Tempetature = 1000;
    float eval_candidat;
    float eval_new_sol;
    vector<int> candidat;
    vector<int> new_sol;
    vector<float> x_arg;
    
    
    //generam solutia random
    for (int i = 0; i < D * L; i++) {
            candidat.push_back(rand() % 2);
    }
     
    
    //evaluam solutia
    x_arg = deco(candidat, L, D, a, b);
    
    for(int i = 0; i < D; ++i)
    {
        cout<<x_arg[i]<<" ";
    }
    cout<<endl;
    
    eval_candidat = Rosenbrock(x_arg, D);
    cout<<eval_candidat<<endl;
    while(t)
    {
        //selectam o noua solutie vecina random
        /*
        for (int i = 0; i < D * L; i++) {
                new_sol.push_back(rand() % 2);
        }
        //afisare(new_sol, D, L);
         */
        int vecini = 10;
        while(vecini)
        {
            int p = 0 + (rand() % static_cast<int>(L*D - 1));
            new_sol = neighbor(candidat, p);
            
            //evaluam noua solutie
            x_arg = deco(new_sol, L, D, a, b);
            
            eval_new_sol = Rosenbrock(x_arg, D);
            //cout<<eval_new_sol<<endl;
            
            if(eval_new_sol < eval_candidat)
            {
                candidat = new_sol;
            }
            else
            {
                float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                float dif = abs(eval_new_sol - eval_candidat);
                //cout<<r<<endl;
                //cout<<dif<<endl;
                if(r < exp(-dif/Tempetature))
                {
                    candidat = new_sol;
                }
            }
            new_sol.clear();
            vecini--;
        }
        
        Tempetature *= 0.9;
        --t;
        
    }
    
    return candidat;
}



int main()
{
    clock_t tStart = clock();
    srand(static_cast<unsigned int>(time(NULL)));
    vector<int> vec;
    vector<float> x;
    
    int D = 30;
    float a = -5;
    float b = 10;
    
    ofstream f;
    f.open ("SA_Rosenbrock_30_100.txt");
    for(int i = 0; i < 30; i++)
    {
        //vec = Hill_Climbing(8, D, a, b, 1);
        vec = Simulated_Annealing(8, D, a, b);
        x = deco(vec, 8, D, a, b);
        f << sphere(x, D) << "\n";
    }
    f << "Time taken: " << (double)(clock() - tStart)/CLOCKS_PER_SEC;
    //vec = Simulated_Annealing(8, 2, -10, 10);
    /*x = deco(vec, 8, 2, -10, 10);
    for(int i = 0; i < 2; ++i)
    {
        cout<<x[i]<<" ";
    }
    cout<<endl;
    cout<< Dixon_Price(x, 2);
    cout<<endl;*/
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    return 0;
}
