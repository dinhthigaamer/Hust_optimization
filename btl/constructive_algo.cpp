#include <bits/stdc++.h>
#include <chrono>
using namespace std;
using namespace chrono;
#define MAXN 1005
#define MAXK 105

int n, k;
int d[MAXN][MAXN];
vector<vector<int>> paths;

int distance(const vector<int> &path)
{
    int sum = 0;
    for (int i = 1; i < path.size(); ++i)
        sum += d[path[i - 1]][path[i]];

    return sum;
}

// Chèn 1 đỉnh vào đường đi
struct insert_point_method
{
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

    int find_suitable_vector(int p, vector<vector<int>> &paths)
    {
        // cerr << p << endl;
        int min_dist = 1e9;
        int x = 1;
        vector<int> tmp_path = {};

        for (int i = 0; i < k; ++i)
        {
            pair<int, vector<int>> new_path = insert_point(p, paths[i]);
            if (new_path.first < min_dist)
            {
                min_dist = new_path.first;
                x = i;
                tmp_path = new_path.second;
            }
        }

        paths[x] = tmp_path;
        return x;
    }

    void solve(vector<vector<int>> &paths)
    {
        for (int p = 1; p <= n; ++p)
        {
            // cerr << p << endl;
            int min_dist = 1e9;
            int x = 1;
            vector<int> tmp_path = {};

            for (int i = 0; i < k; ++i)
            {
                pair<int, vector<int>> new_path = insert_point(p, paths[i]);
                if (new_path.first < min_dist)
                {
                    min_dist = new_path.first;
                    x = i;
                    tmp_path = new_path.second;
                }
            }

            paths[x] = tmp_path;

            // for (int u : paths[x])
            //     cerr << u << ' ';
            // cerr << endl;
        }
    }
};

// Đổi chỗ 2 điểm trong cùng 1 đường đi
struct swap_point_method
{
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

    void solve_each_vector(vector<int> &path)
    {
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
        // return {dist, path};
    }

    void solve(vector<vector<int>> &paths)
    {
        for (int i = 0; i < k; ++i)
        {
            solve_each_vector(paths[i]);
        }
    }
};

// Đổi đỉnh từ đường này sang đường đi khác
struct decrese_longest_path
{
    // Lấy hàm insert
    insert_point_method is;

    void solve(vector<vector<int>> &paths)
    {
        // Thực hiện đến khi ko tìm đc phương án tối ưu
        for (int tmp = 1; tmp <= 500; ++tmp)
        {
            // Đi tìm path có độ dài max
            int max_dist = 0, max_path = 0;
            for (int i = 0; i < k; ++i)
            {
                int dist = distance(paths[i]);
                if (dist > max_dist)
                {
                    max_dist = dist;
                    max_path = i;
                }
            }

            // cerr << 1 << endl;
            for (int i = 0; i < k; ++i)
                if (i != max_path)
                {
                    int dist_i = distance(paths[i]);
                    int found_better_solution = 0;

                    // Thử thêm đỉnh mới vào đường đi dài nhất
                    for (int j = paths[i].size() - 1; j > 0; --j)
                    {
                        int p = paths[i][j];
                        int prev_p = paths[i][j - 1];
                        int next_p = paths[i][j];

                        if (j < paths[i].size() - 1)
                            next_p = paths[i][j + 1];

                        int delta_i = d[prev_p][next_p] - d[prev_p][p] - d[p][next_p];

                        if (dist_i + delta_i >= max_dist)
                            continue;

                        pair<int, vector<int>> res = is.insert_point(p, paths[max_path]);
                        if (res.first < max_dist)
                        {
                            paths[max_path] = res.second;
                            paths[i].erase(paths[i].begin() + j);
                            ++j;
                            break;
                        }
                    }

                    if (found_better_solution)
                        break;

                    // Thử loại bớt đỉnh của max_path và gán cho đường khác
                    for (int j = paths[max_path].size() - 1; j > 0; --j)
                    {
                        int p = paths[max_path][j];
                        int prev_p = paths[max_path][j - 1];
                        int next_p = p;

                        if (j < paths[max_path].size() - 1)
                            next_p = paths[max_path][j + 1];

                        // Độ chênh lệch sau khi bỏ đỉnh p đi
                        int delta = d[prev_p][next_p] - d[prev_p][p] - d[p][next_p];

                        if (delta >= 0)
                            continue;

                        pair<int, vector<int>> res = is.insert_point(p, paths[i]);

                        if (res.first < max_dist)
                        {
                            paths[i] = res.second;
                            paths[max_path].erase(paths[max_path].begin() + j);
                            ++j;
                            found_better_solution = 1;
                            break;
                        }
                    }
                }
        }
    }
};

// Thay vi xay dung xong quang duong trong tung thuat toan,
// ta thuc hien ca 3 thuat moi khi them dinh
struct hybrid_algo
{
    insert_point_method ism;
    swap_point_method spm;
    decrese_longest_path dlp;

    void solve(vector<vector<int>> &paths)
    {
        for (int p = 1; p <= n; ++p)
        {
            int x = ism.find_suitable_vector(p, paths);
            spm.solve_each_vector(paths[x]);
            // dlp.solve(paths);
        }
    }
};

void solve()
{
    insert_point_method ipm;
    swap_point_method spm;
    decrese_longest_path dlp;
    hybrid_algo ha;
    // generate_method gm;

    // ipm.solve(paths);
    // spm.solve(paths);
    ha.solve(paths);
    // dlp.solve(paths);
    // gm.solve(paths);
}

int main()
{
    freopen("file.inp", "r", stdin);
    freopen("file.out", "w", stdout);

    cin >> n >> k;

    for (int i = 0; i <= n; ++i)
    {
        for (int j = 0; j <= n; ++j)
            cin >> d[i][j];
    }

    for (int i = 0; i < k; ++i)
        paths.push_back({0});

    solve();

    cout << k << endl;

    for (int i = 0; i < k; ++i)
    {
        cout << paths[i].size() << '\n';
        for (int u : paths[i])
            cout << u << ' ';
        cout << '\n';
    }
    return 0;
}