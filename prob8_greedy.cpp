#include <bits/stdc++.h>
using namespace std;

int n, k;
int d[1005][1005];
vector<int> paths[1005];
int choosen[1005];

void solve()
{
    vector<int> dist(k + 1, 0);

    for (int turn = 1; turn <= n; ++turn)
    {
        int p;
        int x;
        int min_dist = 1e9;

        for (int i = 1; i <= k; ++i)
        {
            int last_point = paths[i].back();

            for (int u = 1; u <= n; ++u)
                if (!choosen[u])
                {
                    if (dist[i] + d[last_point][u] < min_dist)
                    {
                        min_dist = dist[i] + d[last_point][u];
                        p = u;
                        x = i;
                    }
                }
        }

        dist[x] = min_dist;
        paths[x].push_back(p);
        choosen[p] = 1;
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