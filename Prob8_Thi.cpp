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

pair<int, vector<int>> insert_point(int p, vector<int> path)
{
    path.push_back(p);
    int dist = distance(path);
    vector<int> tmp_path = path;

    for (int i = path.size() - 2; i > 0; --i)
    {
        swap(path[i + 1], path[i]);
        int new_dist = distance(path);

        if (new_dist < dist)
        {
            dist = new_dist;
            tmp_path = path;
        }
    }
    path = tmp_path;

    return {dist, path};
}
// Insert new point to current path, return the shortest distance can be found and the new path
// If cannot find a new path < limit, return {-1, {}}
pair<int, vector<int>> insert_point_to_path(int p, vector<int> path, int limit = 0)
{
    pair<int, vector<int>> tmp = insert_point(p, path);
    int dist = tmp.first;
    path = tmp.second;

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

// Try to decrease the longest path
// by removing some point and adding it to the shortest path
void decrese_longest_path()
{
    while (1)
    {
        int better = 0;

        int max_dist = 0, min_dist = 1e9;
        int max_truck = 1, min_truck = 1;

        for (int x = 1; x <= k; ++x)
        {
            int dist = distance(paths[x]);
            if (max_dist < dist)
            {
                max_dist = dist;
                max_truck = x;
            }

            if (min_dist > dist)
            {
                min_dist = dist;
                min_truck = x;
            }
        }

        vector<int> &tmp = paths[max_truck];

        for (int i = 1; i < tmp.size() - 1; ++i)
        {
            // remove point paths[max_truck][i]
            int new_dist = max_dist - (d[tmp[i - 1]][tmp[i]] + d[tmp[i]][tmp[i + 1]]);
            new_dist += d[tmp[i - 1]][tmp[i + 1]];

            if (new_dist >= max_dist)
                continue;

            for (int x = 1; x <= k; ++x)
            {
                if (x == max_truck)
                    continue;
                pair<int, vector<int>> res = insert_point(tmp[i], paths[x]);
                if (res.first >= max_dist)
                    continue;

                tmp.erase(tmp.begin() + i);
                paths[x] = res.second;
                better = 1;
                break;
            }
        }

        if (better == 0)
            break;
    }
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

    decrese_longest_path();
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