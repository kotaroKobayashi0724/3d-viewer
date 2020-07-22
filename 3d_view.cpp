#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <GL/glut.h>
#include <vector>

// 描画処理が必要なときに呼ばれる
void display(void)
{
	//　ファイル名を入力させる
	std::string file_name;
	std::cout << "ファイル名を入力してください" << std::endl;
	std::cin >> file_name;
	// 保存しているstlファイルを読み込む
	//std::ifstream ifs("dem02.stl");
	std::ifstream ifs(file_name);
	std::string str;
	if (ifs.fail()) {
		std::cerr << "失敗" << std::endl;
	}
	double xs = 1000, xl = -1000, ys = 1000, yl = -1000, zs = 1000, zl = -1000; //　読み込んだモデルの各座標の最大値と最小値を保持する変数（xsはx座標の最小値）
	int num = 0;
	double mx = 0, my = 0, mz = 0;
	GLdouble vertexp[3][3] = { {0,0,0} ,{0,0,0} ,{0,0,0} };
	std::vector<std::vector<GLdouble>> vertex_point;
	while (getline(ifs, str)) {
		getline(ifs, str);
		if (str[4] == 'v') {
			std::string str1[3];
			int n = 11;
			int m = 0;
			str1[m] = str[n];
			while ((float)n < (float)str.size()) {
				n++;
				if ((float)n >= (float)str.size()) {
					break;
				}
				if (str[n] == ' ') {
					m++;
				}
				else {
					str1[m] = str1[m] + str[n];
				}
			}
			std::vector<GLdouble> vec;
			vec.push_back(std::stod(str1[0]));
			vec.push_back(std::stod(str1[1]));
			vec.push_back(std::stod(str1[2]));
			vertex_point.push_back(vec);
			if (vec[0] < xs) {
				xs = vec[0];
			}
			if (vec[0] > xl) {
				xl = vec[0];
			}
			if (vec[1] < ys) {
				ys = vec[1];
			}
			if (vec[1] > yl) {
				yl = vec[1];
			}
			if (vec[2] < zs) {
				zs = vec[2];
			}
			if (vec[2] > zl) {
				zl = vec[2];
			}
			num++;
		}
	}

	glClear(GL_COLOR_BUFFER_BIT);       	// カラーバッファを初期化する
	glLoadIdentity();						// 行列の初期化
	// カメラ座標,カメラが映す座標,上方向
	gluLookAt((xs + xl) / 2, (ys + yl) / 2, ((zs + zl) / 2) + (zs + zl), (xs + xl)/2, (ys + yl) / 2, (zs + zl) / 2, 0.0, 1.0, 0.0);

	for (int i = 0; i < vertex_point.size(); i++) {
		if(i%3==2){
			// 読み込んだ頂点でワイヤーフレームモデルを作成
			GLdouble vertexp[3][3] = { {vertex_point[i - 2][0], vertex_point[i - 2][1], vertex_point[i - 2][2]},
										{vertex_point[i - 1][0], vertex_point[i - 1][1], vertex_point[i - 1][2]},
										{vertex_point[i][0], vertex_point[i][1], vertex_point[i][2]} };
			glColor3d(0.0, 0.0, 0.0);
			glBegin(GL_LINES);
			glVertex3dv(vertexp[0]);
			glVertex3dv(vertexp[1]);
			glVertex3dv(vertexp[2]);
			glEnd();

		}
	}
	glFlush();				// 描画を行う
}

// ウィンドウのサイズ変更時に呼ばれる
void reshape(int width, int height) {

	// ビューポートの設定
	glViewport(0, 0, width, height);

	// 視野領域の決定
	glMatrixMode(GL_PROJECTION);					// 射影行列を操作する
	glLoadIdentity();						// 行列を初期化
	gluPerspective( 60.0, (double)width / height, 1.0, 1000.0);

	// 視点位置の決定		
	glMatrixMode(GL_MODELVIEW);					// モデルビュー行列の設定
	glLoadIdentity();						// 行列を初期化
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void initialize(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);			    	// 画面を白にする
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);						// GLUTを初期化する
	glutInitWindowSize(800, 600);				     	// 画面サイズを指定する
	glutInitWindowPosition(100, 100);			    	// 画面の初期位置を指定する
	glutInitDisplayMode(GLUT_RGBA);				    	// 表示モード設定
	glutCreateWindow("OpenGL Window");		  		// ウィンドウの名前

	// コールバック関数の設定
	glutDisplayFunc(display);					// 描画処理が必要なときに呼ばれる
	glutReshapeFunc(reshape);					// reshapeが必要なときに呼ばれる

	initialize();							// 初期化

	glutMainLoop();							// 毎フレームのLoop
	return 0;
}