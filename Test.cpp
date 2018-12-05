
#include <stdio.h>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <string>
#include "XGuid.h"

using namespace std;

void TestVector();
void TestHashSet();
void TestSharedPtr();

struct Rect
{
    Rect(int x,int y,int w,int h)
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }
    int x;
    int y;
    int w;
    int h;

    bool operator== (const Rect& other) const
    {
        return (x==other.x && y==other.y && w==other.w && h==other.h);
    }
};

struct Grid
{
    int row;
    int col;
    Grid(int row, int col)
    {
        this->row = row;
        this->col = col;
    }

    bool operator== (const Grid& other) const
    {
        return (row==other.row && col==other.col);
    }
    ~Grid()
    {
        printf("~Grid()\n");
    }
};

struct Hash 
{ 
    static string Seed;
    static void GenerateNewSeed()
    {
        Seed = XGUID::CreateGuidString();
    }

   size_t operator() (const Grid &grid) const 
   {
        string temp = to_string(grid.row) + to_string(grid.col) + Seed;
        return hash<string>()(temp);
   }
};

string Hash::Seed = "";

int main(int argc, char *argv[])
{
    //TestVector();
    //TestHashSet();
    TestSharedPtr();
    getchar();
    return 0;
}

void TestVector()
{
    vector<Rect> list;
    
    list.push_back(Rect(1,2,3,4));
    list.push_back(Rect(1,2,4,5));
    list.push_back(Rect(1,2,6,7));
    list.push_back(Rect(1,2,8,9));

    Rect rect = Rect(1,2,6,7);
    vector<Rect>::iterator it = find(list.begin(),list.end(),rect);
    list.erase(it);
    it = find(list.begin(),list.end(),rect);

    if (it != list.end())
        printf("find value at %ld!\n",it-list.begin());
    else
        printf("not find ! \n");
}

void TestHashSet()
{
    unordered_set<Grid,Hash> hashSet;
    hashSet.insert(Grid(1,2));
    hashSet.insert(Grid(1,2));
    hashSet.insert(Grid(2,3));
    hashSet.insert(Grid(3,4));
    printf("hashSet size %ld ! \n",hashSet.size());
    Grid grid = Grid(2,3);
    hashSet.erase(grid);

    unordered_set<Grid,Hash>::const_iterator it = hashSet.find(Grid(3,4));
    if (it == hashSet.end())
        printf("not find ! \n");
    else
        printf("find (%d,%d)! \n",it->row,it->col);

    for (it=hashSet.begin(); it!=hashSet.end(); it++)
        printf("grid(%d,%d)\n",it->row,it->col);
}

void TestSharedPtr()
{
    Grid *qGrid = NULL;
    printf("qGrid = %p\n",qGrid);
    
    shared_ptr<Grid> pGrid(new Grid(2,4));
    //auto pInt = std::make_shared<int>(2);
    qGrid = pGrid.get();
    weak_ptr<Grid> wGrid = pGrid;
        
    printf("qGrid = %p\n",qGrid);
    printf("grid(%d,%d)\n",pGrid->row,pGrid->col);
    printf("use count = %ld\n",pGrid.use_count());
    {
        shared_ptr<Grid> rGrid = pGrid;
        printf("use count = %ld\n",pGrid.use_count());
    }
    printf("use count = %ld\n",pGrid.use_count());

    pGrid.reset();
    printf("qGrid = %p\n",qGrid);
    printf("use count = %ld\n",wGrid.use_count());
    if (wGrid.expired())
        printf("expired\n");
    shared_ptr<Grid> sGrid = wGrid.lock();
    if (sGrid == nullptr)
        printf("nullptr\n");

}