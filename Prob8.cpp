#include <bits/stdc++.h>
using namespace std;

#define MAXN 1005
#define MAXK 105

int n, k;
int d[MAXN][MAXN];
vector<int> paths[MAXK], final_paths[MAXK];

int distance(const vector<int> &path)
{
    int sum = 0;
    for (int i = 1; i < path.size(); ++i)
        sum += d[path[i - 1]][path[i]];

    return sum;
}

int swap_point(const vector<int> &path, int dist, int p1, int p2)
{
    int new_dist = dist;
    int old_edge, new_edge;

    if (p1 == p2 - 1)
    {
        if (p2 < path.size() - 1)
        {
            old_edge = d[path[p1]][path[p2]] + d[path[p1 - 1]][path[p1]] + d[path[p2]][path[p2 + 1]];
            new_edge = d[path[p2]][path[p1]] + d[path[p1 - 1]][path[p2]] + d[path[p1]][path[p2 + 1]];
        }
        else
        {
            old_edge = d[path[p1]][path[p2]] + d[path[p1 - 1]][path[p1]];
            new_edge = d[path[p2]][path[p1]] + d[path[p1 - 1]][path[p2]];
        }

        new_dist -= old_edge;
        new_dist += new_edge;
    }
    else
    {
        if (p2 < path.size() - 1)
        {
            old_edge = d[path[p1 - 1]][path[p1]] + d[path[p1]][path[p1 + 1]] + d[path[p2 - 1]][path[p2]] + d[path[p2]][path[p2 + 1]];
            new_edge = d[path[p1 - 1]][path[p2]] + d[path[p2]][path[p1 + 1]] + d[path[p2 - 1]][path[p1]] + d[path[p1]][path[p2 + 1]];
        }
        else
        {
            old_edge = d[path[p1 - 1]][path[p1]] + d[path[p1]][path[p1 + 1]] + d[path[p2 - 1]][path[p2]];
            new_edge = d[path[p1 - 1]][path[p2]] + d[path[p2]][path[p1 + 1]] + d[path[p2 - 1]][path[p1]];
        }

        new_dist -= old_edge;
        new_dist += new_edge;
    }

    return new_dist;
}

// Insert new point to current path, return the shortest distance can be found and the new path
// If cannot find a new path < limit, return {-1, {}}
pair<int, vector<int>> insert_point_to_path(int p, vector<int> path, int limit = 0)
{
    path.push_back(p);
    int dist = distance(path);

    while (1)
    {
        pair<int, int> solution = {0, 0};
        int min_dist = dist;
        // swap 2 point in the road
        for (int i = 1; i < path.size() - 1; ++i)
        {
            for (int j = i + 1; j < path.size(); ++j)
            {
                int new_dist = swap_point(path, dist, i, j);

                if (min_dist > new_dist)
                {
                    min_dist = new_dist;
                    solution = {i, j};
                }
            }
        }

        if (solution.first == 0)
            break;
        dist = min_dist;
        swap(path[solution.first], path[solution.second]);
    }

    return {dist, path};
}

void solve()
{
    for (int p = 1; p <= n; ++p)
    {
        int min_dist = 1e9;
        int x = 1;
        vector<int> tmp_path = {};

        for (int i = 1; i <= k; ++i)
        {
            pair<int, vector<int>> new_path = insert_point_to_path(p, paths[i]);
            if (new_path.first < min_dist)
            {
                min_dist = new_path.first;
                x = i;
                tmp_path = new_path.second;
            }
        }

        paths[x] = tmp_path;
    }
}

int main()
{
    // freopen("file.inp", "r", stdin);
    // freopen("file.out", "w", stdout);

    cin >> n >> k;

    for (int i = 0; i <= n; ++i)
    {
        for (int j = 0; j <= n; ++j)
            cin >> d[i][j];
    }

    for (int i = 1; i <= k; ++i)
        paths[i].push_back(0);

    solve();

    cout << k << endl;

    for (int i = 1; i <= k; ++i)
    {
        cout << paths[i].size() << '\n';
        for (int u : paths[i])
            cout << u << ' ';
        cout << '\n';
    }
    return 0;
}