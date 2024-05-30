#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <cctype>
#include <climits>
#include "cudd.h"

using namespace std;

DdNode *buildBDD(DdManager *manager, const string &booleanExpr, const map<char, int> &varOrder) {
    DdNode *bdd = Cudd_ReadLogicZero(manager);
    Cudd_Ref(bdd);

    istringstream iss(booleanExpr);
    string sop;

    while (getline(iss, sop, '+')) {
        DdNode *term = Cudd_ReadOne(manager);
        Cudd_Ref(term);

        for (char var : sop) {
            bool isComplement = isupper(var);
            char varLower = tolower(var);
            DdNode *varBdd = Cudd_bddIthVar(manager, varOrder.at(varLower));

            if (isComplement) {
                varBdd = Cudd_Not(varBdd);
            }

            DdNode *temp = Cudd_bddAnd(manager, term, varBdd);
            Cudd_Ref(temp);
            Cudd_RecursiveDeref(manager, term);
            term = temp;
        }

        DdNode *temp = Cudd_bddOr(manager, bdd, term);
        Cudd_Ref(temp);
        Cudd_RecursiveDeref(manager, bdd);
        Cudd_RecursiveDeref(manager, term);
        bdd = temp;
    }

    return bdd;
}

int main(int argc, char *argv[]) {
    ifstream fin(argv[1]);
    ofstream fout(argv[2]);

    string booleanExpr;
    getline(fin, booleanExpr);
    booleanExpr.pop_back();

    DdManager *manager = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
    cout << "debug" << endl;
    string ordering;

    int minNodeCount = INT_MAX;
    while (getline(fin, ordering)) {
        map<char, int> varOrder;
        for (int i = 0; i < ordering.size(); ++i)
        {
            varOrder[ordering[i]] = i;
        }

        DdNode *bdd = buildBDD(manager, booleanExpr, varOrder);
        Cudd_Ref(bdd);

        DdNode *add = Cudd_BddToAdd(manager, bdd);
        Cudd_Ref(add);

        int nodeCount = Cudd_DagSize(add);
        if (nodeCount < minNodeCount) {
            minNodeCount = nodeCount;
        }

        Cudd_RecursiveDeref(manager, bdd);
    }

    fout << minNodeCount << endl;

    Cudd_Quit(manager);
    fin.close();
    fout.close();
    return 0;
}