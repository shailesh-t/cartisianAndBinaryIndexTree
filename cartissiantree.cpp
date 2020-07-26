// cartissiantree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
using namespace std;

class node {
    int dV;
    shared_ptr<node> dL = nullptr;
    shared_ptr<node> dR = nullptr;
    using N = shared_ptr<node>;
public:
    node(int x) : dV(x) {}
    int mGetVal() const { return dV; }
    void mSetVal(int xVal) { dV = xVal; }
    void mSetLeftPtr(N x) { dL = x; }
    void mSetRightPtr(N x) { dR = x; }
    N mGetLeftPtr()  const { return dL; }
    N mGetRightPtr() const { return dR; }
    void mPrint() { cout << dV << '\n'; }
};

class cartisianTree {
    using N = shared_ptr<node>;
    N dRoot = nullptr;
    N _mCreateTree(N xPtr, int xMin, int xMax, vector<int>& xVec)
    {
        if (xMin > xMax) return nullptr;
        auto lMin = min_element(begin(xVec) + xMin, begin(xVec) + xMax + 1);
        xPtr = make_shared<node>(*lMin);
        int xDistance = distance(begin(xVec), lMin);
        xPtr->mSetLeftPtr(_mCreateTree(xPtr->mGetLeftPtr(), xMin, xDistance - 1, xVec));
        xPtr->mSetRightPtr(_mCreateTree(xPtr->mGetRightPtr(), xDistance + 1, xMax, xVec));
        return xPtr;
    }
    void _mGetLCA(N xRoot, int& xKey1, int& xKey2, int xDepth, int& xCD, int& xValue, bool& lFound)
    {
        if (xRoot == nullptr) return;
        if ((xKey1 == INT_MAX && xRoot->mGetVal() == xKey2) ||
            (xKey2 == INT_MAX && xRoot->mGetVal() == xKey1))
        {
            if (xCD > xDepth) { xCD = xDepth; xValue = xRoot->mGetVal(); }
            lFound = true;
            return;
        }
        if (xRoot->mGetVal() == xKey1) 
        {   
            xCD = xDepth;
            xValue = xRoot->mGetVal();
            xKey1 = INT_MAX;
        }
        else if (xRoot->mGetVal() == xKey2)
        {
            xCD = xDepth;
            xValue = xRoot->mGetVal();
            xKey2 = INT_MAX;
        }
        _mGetLCA(xRoot->mGetLeftPtr(), xKey1, xKey2, xDepth + 1, xCD, xValue,lFound);
        if (!lFound) {
            if (xCD > xDepth)
            {
                xCD = xDepth; xValue = xRoot->mGetVal();
            }
            _mGetLCA(xRoot->mGetRightPtr(), xKey1, xKey2, xDepth + 1, xCD, xValue, lFound);
        }
    }

public:
    N mGetRoot() const { return dRoot; }
    void mSetRoot(N x) { dRoot = x; }
    // O(n2)
    void mCreateTree(vector<int>& xVec) {
        mSetRoot(_mCreateTree(mGetRoot(), 0, xVec.size() - 1, xVec));
    }
    // O(n) good
    void mCreateTree1(vector<int>& xVec) {
        N prev = nullptr;
        for (auto y : xVec)
        {
            N x = make_shared<node>(y);
            if (prev == nullptr)
            { 
                mSetRoot(x);
            } else {
                if (prev->mGetVal() <= y)
                {
                    prev->mSetRightPtr(x);
                }
                else {
                    // search on right wing for min that is greater than y value
                    if (y <= mGetRoot()->mGetVal()) {
                        x->mSetLeftPtr(mGetRoot());
                        mSetRoot(x);
                    }
                    else {
                        auto lPrev = mGetRoot();
                        auto lNext = lPrev->mGetRightPtr();
                        while (y > lNext->mGetVal())
                        {
                            lPrev = lNext;
                            lNext = lNext->mGetRightPtr();
                        }
                        x->mSetLeftPtr(lNext);
                        lPrev->mSetRightPtr(x);
                    }
                }
            }
            prev = x;
        }
    }
    // print EulerTour 
    //        1
    //       2  3 
    // printing will bw {1 2 1 3 1}all edges twice
    void mPrintEulerTour(N root) {
        if (root == nullptr) return;
        root->mPrint();
        if (root->mGetLeftPtr()) {
            mPrintEulerTour(root->mGetLeftPtr());
            root->mPrint();
        }
        if (root->mGetRightPtr())
        {
            mPrintEulerTour(root->mGetRightPtr());
            root->mPrint();
        }
    }

    void mPrint(N root) {
        if (root == nullptr) return;
            mPrint(root->mGetLeftPtr());
            root->mPrint();
            mPrint(root->mGetRightPtr());
    }

    void mGetLCA(int xKey1, int xKey2)
    {
        int xVal = INT_MAX;
        bool lStatus = false;
        int xCD = INT_MAX;
        _mGetLCA(mGetRoot(), xKey1, xKey2, 0, xCD, xVal, lStatus);
        if (lStatus)
            cout << xVal << " : LCA\n";
    }
};

class BinrayIndexTree {
    vector<int> dTree;
public: 
    BinrayIndexTree(size_t x) : dTree(x) {}
    int mGetSum(int l) {
        int ret = 0;
        for (;l >= 0;l = ((l & (l + 1)) - 1))
            ret += dTree[l];
        return ret;
    }
    void update(int lIdx, int lValue, int lOld) {
        int lDiff = lOld - lValue;
        for (;lIdx < dTree.size();lIdx = (lIdx | (lIdx + 1)))
            dTree[lIdx] += lDiff;
    }
    // here l and r both inclusive 
    int mGetSumRange(int l, int r) {
        return mGetSum(r) - mGetSum(l-1);
    }
    // l and r inclusive
    void mUpdateRangeWithConstValueAdd(int l, int r, int lValue) {
        update(l, 0, lValue);
        update(r + 1, 0, -lValue);
    }
    void mPrint() {
        for (auto x : dTree)
            cout << x << " ";
    }
};

void cartisianTreeUtil()
{
    cartisianTree cT;
    vector<int> lVec{ 8,7,2,8,6,9,4,5 };
    cT.mCreateTree1(lVec);
    //cT.mCreateTree(lVec);
    cT.mPrint(cT.mGetRoot());
    cT.mGetLCA(8, 9);
    cT.mGetLCA(8, 7);
    cT.mGetLCA(8, 2);
    cT.mGetLCA(6, 5);
    cT.mGetLCA(7, 9);
    cT.mGetLCA(6, 4);
    cT.mGetLCA(9, 5);
}
// operation like 0 to range max/sum/min with update , sum between two range  but not min,
//for range related query one can use segment tree/ cartisian tree / min heap
void binaryIndexTreeUtil()
{
    vector<int> lVec{ 8,7,2,8,6,9,4,5 };
    BinrayIndexTree bt(lVec.size());
    int i = 0;
    for (auto x : lVec)
        bt.update(i++, 0, x);
    bt.mPrint();
    cout << '\n';
    cout << bt.mGetSum(1) << '\n';
    cout << bt.mGetSum(4) << '\n';
    cout << bt.mGetSum(6) << '\n';
    cout << bt.mGetSum(3) << '\n';
    cout << bt.mGetSumRange(3,5) << '\n';
    bt.mUpdateRangeWithConstValueAdd(1, 6, 10);
    bt.mPrint();
}
int main()
{
    //cartisianTreeUtil();
    binaryIndexTreeUtil();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
