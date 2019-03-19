# include <iostream>
# include <cmath>
using namespace std;
//八皇后问题
int ans = 0;
// row 为列  line为行 判断是否斜线 |x1-x2| == |y1-y2|
void queen_set(int *queen, int n){
	if( n == 8 ){
		for(int count = 0; count < 7; ++count) cout << count << " " << queen[count]+1 << endl;
		::ans++;
		system("pause");
		queen[7] = 0;
	}
	else{
		int row,line;
		for(row = 0; row < 8; ++row){
			for(line = 0; line < n; ++line)
				if(row == queen[line] || fabs(line-n) == fabs(queen[line]-row)) break;//若找到与前面位置冲突 跳出
			if(line == n){
				queen[n] = row;
				queen_set(queen,n+1);
			}
		}
		if( row == 8 ){
			queen[n-1] = 0;
			return;
		}
	}
}

int main()
{
	int queen[8] = {};
	queen_set(queen,0);
	cout << "共"<< ::ans << "种解决方案!";
}
