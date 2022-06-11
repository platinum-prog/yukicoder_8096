#include <iostream>
#include <queue>
#include <random>
#include <cassert>

using namespace std;

const int number_of_files = 100; // 生成するケース数
const int grid_width = 20; // グリッドの一辺の長さ
const int min_p = 6;  // 確率pの最小値
const int max_p = 15; // 確率pの最大値
const int max_length = 400; // 出力された文字列の最大の長さ
const int max_trial = 1001; // 最大試行回数

// 壁の情報
int h[grid_width][grid_width+1];
int v[grid_width+1][grid_width];

// 何マス進めるかの情報
int steps[max_trial-1];

uniform_int_distribution<> cent(1, 100);

// 行けないマスがあるかどうかの判定
bool bfs(){
	int grid[grid_width][grid_width];
	for(int i = 0; i < grid_width; i++){
		for(int j = 0; j < grid_width; j++){
			grid[i][j] = -1;
		}
	}
	grid[0][0] = 0;
	queue<pair<int,int>> q;
	q.emplace(0, 0);
	while(!q.empty()){
		pair<int,int> p = q.front(); q.pop();
		int y = p.first, x = p.second;
		int d = grid[y][x];
		if(v[y][x] == 0 && grid[y-1][x] == -1){
			grid[y-1][x] = d + 1;
			q.emplace(y - 1, x);
		}
		if(v[y+1][x] == 0 && grid[y+1][x] == -1){
			grid[y+1][x] = d + 1;
			q.emplace(y + 1, x);
		}
		if(h[y][x] == 0 && grid[y][x-1] == -1){
			grid[y][x-1] = d + 1;
			q.emplace(y, x - 1);
		}
		if(h[y][x+1] == 0 && grid[y][x+1] == -1){
			grid[y][x+1] = d + 1;
			q.emplace(y, x + 1);
		}
	}
	for(int i = 0; i < grid_width; i++){
		for(int j = 0; j < grid_width; j++){
			if(grid[i][j] == -1) return false;
		}
	}
	return true;
}

int main(){
	// シード値の読み込み
	FILE *rnd_in = freopen("seeds.txt", "r", stdin);
	int number;
	cin >> number;
	assert(number == number_of_files);
	uint64_t seeds[number_of_files];
	for(int i = 0; i < number_of_files; i++){
		cin >> seeds[i];
	}
	fclose(rnd_in);

	// 入力生成開始
	int cnt = 1;
	while(cnt <= number_of_files){
		// 初期化
		for(int i = 0; i < grid_width; i++){
			for(int j = 0; j < grid_width + 1; j++){
				h[i][j] = 0;
			}
		}
		for(int i = 0; i < grid_width + 1; i++){
			for(int j = 0; j < grid_width; j++){
				v[i][j] = 0;
			}
		}
		mt19937_64 mt(seeds[cnt-1]);
		// ファイル名の決定
		string str = "testcase_";
		string num = to_string(cnt);
		int siz = num.size();
		for(int i = 0; i < 3 - siz; i++){
			num = '0' + num;
		}
		str += num;
		str += ".txt";
		string in_str = "in/" + str, out_str = "out/" + str;
		const char* in_fname = in_str.c_str();
		const char* out_fname = out_str.c_str();
		// テストケースの生成
		int H = 20, W = 20;
		int p = min_p + (cnt - 1) % (max_p - min_p + 1);
		// グリッド情報の生成
		for(int i = 0; i < grid_width; i++){
			h[i][0] = 1;
			h[i][grid_width] = 1;
		}
		for(int j = 0; j < grid_width; j++){
			v[0][j] = 1;
			v[grid_width][j] = 1;
		}
		uniform_int_distribution<> walls(100, 200);
		uniform_int_distribution<> dir(0, 1);
		uniform_int_distribution<> d1(0, grid_width - 1);
		uniform_int_distribution<> d2(1, grid_width - 1);
		int M = walls(mt);
		int wall_number = 0;
		while(wall_number < M){
			int D = dir(mt);
			if(D == 0){
				int i = d1(mt), j = d2(mt);
				if(h[i][j] == 1) continue;
				h[i][j] = 1;
				if(!bfs()){
					h[i][j] = 0;
					continue;
				}
			}
			else{
				int i = d2(mt), j = d1(mt);
				if(v[i][j] == 1) continue;
				v[i][j] = 1;
				if(!bfs()){
					v[i][j] = 0;
					continue;
				}
			}
			wall_number++;
		}
		// 壁の数の確認
		int check = 0;
		for(int i = 0; i < grid_width; i++){
			for(int j = 1; j < grid_width; j++){
				if(h[i][j] == 1) check++;
			}
		}
		for(int i = 1; i < grid_width; i++){
			for(int j = 0; j < grid_width; j++){
				if(v[i][j] == 1) check++;
			}
		}
		assert(check == M);
		assert(bfs());
		// 進める歩数の決定
		for(int i = 0; i < max_trial - 1; i++){
			int res = 0;
			for(int j = 0; j < max_length; j++){
				int val = cent(mt);
				if(val <= p) break;
				res++;
			}
			assert(0 <= res && res <= max_length);
			steps[i] = res;
		}
		// ファイルへの書き込み
		FILE *in = freopen(in_fname, "w", stdout);
		cout << H << " " << W << " " << p << endl;
		fclose(in);
		FILE *out = freopen(out_fname, "w", stdout);
		cout << M << endl;
		for(int i = 0; i < grid_width; i++){
			for(int j = 0; j < grid_width + 1; j++){
				cout << h[i][j];
				if(j == grid_width) cout << endl;
				else cout << " ";
			}
		}
		for(int i = 0; i < grid_width + 1; i++){
			for(int j = 0; j < grid_width; j++){
				cout << v[i][j];
				if(j == grid_width - 1) cout << endl;
				else cout << " ";
			}
		}
		for(int i = 0; i < max_trial - 1; i++){
			if(i == max_trial - 2) cout << steps[i] << endl;
			else cout << steps[i] << " ";
		}
		fclose(out);
		cnt++;
	}

	return 0;
}
