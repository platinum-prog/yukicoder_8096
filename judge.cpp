#include <bits/stdc++.h>

using namespace std;

const int max_trial = 1001; // 最大試行回数
const int max_length = 400; // 出力された文字列の最大の長さ
const int grid_width = 20; // グリッドの一辺の長さ

int dy[4] = {-1, 1, 0, 0};
int dx[4] = {0, 0, -1, 1};
string dir = "UDLR";

// 壁の情報
int h[grid_width][grid_width+1];
int v[grid_width+1][grid_width];
// 各ターンで何マス進めるかを記録した配列
int steps[max_trial];
// 最初に与えられる入力
int H, W, p;

int direction(char c){
	int d = -1;
	for(int i = 0; i < 4; i++){
		if(c == dir[i]) d = i;
	}
	return d;
}

bool is_valid(string S){ // 出力された文字列が正当かどうか
	int siz = S.size();
	for(int i = 0; i < siz; i++){
		char c = S[i];
		if(c != 'U' && c != 'D' && c != 'L' && c != 'R'){
			return false;
		}
	}
	return true;
}

bool is_goal(string S){ // 出力された経路が目的の経路かどうか
	int y = 0, x = 0;
	int siz = S.size();
	for(int i = 0; i < siz; i++){
		char c = S[i];
		if(c == 'U' && v[y][x]) return false;
		if(c == 'D' && v[y+1][x]) return false;
		if(c == 'L' && h[y][x]) return false;
		if(c == 'R' && h[y][x+1]) return false;
		int d = direction(c);
		y += dy[d], x += dx[d];
	}
	if(y == grid_width - 1 && x == grid_width - 1) return true;
	else return false;
}

int moves(string S, int turn){ // ロボットが移動できた回数
	int y = 0, x = 0;
	int siz = S.size();
	int cnt = 0;
	for(int i = 0; i < siz; i++){
		// 確率pで移動に失敗する
		if(cnt == steps[turn]) return cnt;
		char c = S[i];
		// 進行方向に壁が存在したら移動に失敗する
		if(c == 'U' && v[y][x]) return cnt;
		if(c == 'D' && v[y+1][x]) return cnt;
		if(c == 'L' && h[y][x]) return cnt;
		if(c == 'R' && h[y][x+1]) return cnt;
		// 移動に成功
		int d = direction(c);
		y += dy[d], x += dx[d];
		cnt++;
	}
	return cnt;
}

int main(int argc, char* argv[]){

	// テストケースの入力ファイルの input stream = h w p
	ifstream input_ifs(argv[1]);
	// テストケースの出力ファイルの input stream = マップの情報
	ifstream output_ifs(argv[2]);
	// 提出されたコードのファイルの input stream
	//ifstream code_ifs(argv[3]);
	// スコアファイル（スコア問題のみ利用）の output stream
	ofstream score_ofs(argv[4]);

	// マップ情報の読み込み
	int M; // 外周を除く壁の数
	output_ifs >> M;
	for(int i = 0; i < grid_width; i++){
		for(int j = 0; j < grid_width + 1; j++){
			output_ifs >> h[i][j];
		}
	}
	for(int i = 0; i < grid_width + 1; i++){
		for(int j = 0; j < grid_width; j++){
			output_ifs >> v[i][j];
		}
	}
	for(int i = 1; i < max_trial; i++){
		output_ifs >> steps[i];
	}

	// コンテスタントへの初期入力
	input_ifs >> H >> W >> p;
	cout << H << " " << W << " " << p << endl;

	// リアクティブ開始
	int trial = 1;
	string S;
	while(trial <= max_trial){
		if(trial == max_trial){ //最大試行回数に到達したら終了
			score_ofs << 0 << endl;
			cout << -1 << endl;
			return 0;
		}
		// 標準入力に提出コードでの標準出力が渡される
		cin >> setw(max_length) >> S;
		if(!is_valid(S)){ //出力された文字列が不正だった場合はWA
			score_ofs << 0 << endl;
			exit(1);
		}
		// 目的の経路だった場合は終了する
		if(is_goal(S)){
			score_ofs << max_trial - trial << endl;
			cout << -1 << endl;
			return 0;
		}
		// 移動できた回数が返される
		int res = moves(S, trial);
		cout << res << endl;
		trial++;
	}

	return 0;
}
