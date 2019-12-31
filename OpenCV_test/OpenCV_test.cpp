﻿// OpenCV_test.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#include <opencv2/core.hpp>    // coreモジュールのヘッダーをインクルード
#include <opencv2/highgui.hpp> // highguiモジュールのヘッダーをインクルード
#include <iostream>

#include "opencv2/core/mat.hpp"		// cv:mat
#include <opencv2/imgproc.hpp>		// cv:circle

#include <opencv2/features2d.hpp>	// cv::AKAZE

#include <opencv2/imgproc/types_c.h>	// CV_BGR2GRAY


void CreateColoerWindow();
void CheckDiffAbsImage();
void CheckDiffImage();
void Diff();
void Diff2Image();

void CaptureVideo();

void DetectActiveObjectFromVideo();
void DoutaiKenchi(cv::Mat image1st, cv::Mat image2nd, cv::Mat image3rd);//動体検知

#define BASE_IMAGE_PATH "C:\\Users\\s.hirano\\source\\repos\\OpenCV_test\\images\\"
#define BASE_IMAGE_01 "C:\\Users\\s.hirano\\source\\repos\\OpenCV_test\\contents\\image\\circle_01.png"
#define BASE_IMAGE_02 "C:\\Users\\s.hirano\\source\\repos\\OpenCV_test\\contents\\image\\circle_02.png"
#define BASE_MOVIE_01 "C:\\Users\\s.hirano\\source\\repos\\OpenCV_test\\contents\\video\\vtest.avi"

#define OUTPUT_IMAGE_01 "C:\\Users\\s.hirano\\source\\repos\\OpenCV_test\\contents\\image\\output_01.png"

int main()
{
    std::cout << "Hello World!\n";

	//赤いウィンドウ生成
//	CreateColoerWindow();

	//画像の差分比較
//	CheckDiffAbsImage();

	//特異点を表示
//	CheckDiffImage();

	//2毎の画像の差分を表示
	//Diff();

	//2つの画像の特異点抽出
	//Diff2Image();

	// 動画のframe取り出し
	//CaptureVideo();


	// 本チャン
	// 動画を1フレームずつ取得して、差分をチェックして、画像に差分を描画する。
	DetectActiveObjectFromVideo();

	// 終了待ち
	cv::waitKey(0);
	return 0;

}

///////////////////////////////////////////////////
// 動画を1フレームずつ取得して、差分をチェックして、画像に差分を描画する。
// https://kimamani89.com/2019/04/30/post-420/
// A. 連続する3枚（画像1・画像2・画像3）の画像を用意
// B. 3枚の画像をグレースケールに変換
// C.それぞれの画像（画像2 - 画像1・画像3 - 画像2）の差分画像を作成する
// D. 2枚の差分画像の論理積を計算し、論理積画像を作成する
// E.論理積画像に二値化処理を行い、背景と前景に分けたマスク画像を作成する
//
void DetectActiveObjectFromVideo()
{
 	cv::VideoCapture vcap(BASE_MOVIE_01);
	if (!vcap.isOpened())
	{
		std::cout << "Can't open video.\n";
		return;
	}

	const int MAX_FRAME_NUM = 3;
	int frameBuffIndex = 0;
	cv::Mat frames[MAX_FRAME_NUM];
	cv::Mat *frame;


	for (;;)
	{
		//3つのMATを古い順に画像を並べる。
		// TODO:高速化は実現できてから。(ポインタだけ変えればclone要らないはず)
		
		// 画像を手前に詰める
		if (!frames[MAX_FRAME_NUM - 1].empty())
		{
			frames[MAX_FRAME_NUM - 2] = frames[MAX_FRAME_NUM - 1].clone();
		}
		if (!frames[MAX_FRAME_NUM - 2].empty())
		{
			frames[MAX_FRAME_NUM - 3] = frames[MAX_FRAME_NUM - 2].clone();
		}

		// 最新の位置に画像格納
		frame = &frames[MAX_FRAME_NUM - 1];
		vcap >> *frame;
		if (!frame->empty()) {
			// frame取得成功
						
			cv::Mat gray;
			cvtColor(*frame, gray, CV_BGR2GRAY);
			cv::Mat bin;
			cv::threshold(gray, bin, 64, 255, cv::THRESH_BINARY);//二値化画像化
			imshow("gray", bin);

			// お試し枠描画
			//cv::rectangle(frame, cv::Point(50, 50), cv::Point(100, 100), cv::Scalar(0, 0, 200), 3, 4);

			DoutaiKenchi(frames[0], frames[1], frames[2]);//動体検知



			imshow("frame", *frame);
		}
		else
		{
			// frame取得失敗
			std::cout << "End of frame.\n";
			break;
		}
		// このwaitが無いと、処理が追い付かないのか、何も描画は更新されない。
		// 描画の隙間を作るのと、キーが押されたら停止する
		if (cv::waitKey(30) >= 0) break; 
	}
	std::cout << "\n\nEnd.\n\n";


}

void DoutaiKenchi(cv::Mat image1st, cv::Mat image2nd, cv::Mat image3rd)
{

}



//動画を1フレームずつ画像に保存
//https://teratail.com/questions/57801
void CaptureVideo()
{
	const int strSize = 80;
	char filename[strSize];
	int filenumber;
	// （1）動画ファイルを開くための準備を行う
	cv::VideoCapture cap(BASE_MOVIE_01);

	// （2）動画ファイルが正しく開けているかをチェックする（正しく開けていなければエラー終了する）
	if (!cap.isOpened())
	{
		std::cout << "Can't open video.\n";
		return;
	}

	
	// 画像データを格納するための変数を宣言する
	cv::Mat frame;
	filenumber = 1;
	for (;;)
	{
		// （3）動画ファイルから1フレーム分の画像データを取得して、変数frameに格納する
		cap >> frame;
		// 画像データ取得を取得できたら書き込み。
		if (!frame.empty()) {
			
			sprintf_s(filename, strSize, "img%d.jpg", filenumber++);
			cv::imwrite(filename, frame);
		}
		if (cv::waitKey(30) >= 0) break;
	}
	
}

//OpenCV3でAKAZE特徴量を検出する
//https://dronebiz.net/tech/opencv3/akaze
void Diff2Image()
{
	//読み込む画像のパス
	cv::String scene1_path = "C:\\Users\\s.hirano\\source\\repos\\OpenCV_test\\contents\\image\\28_1.jpg";
	cv::String scene2_path = "C:\\Users\\s.hirano\\source\\repos\\OpenCV_test\\contents\\image\\28_2.jpg";
	   
	//書き出す画像のパス
//	cv::String scene_12_path = OUTPUT_IMAGE_01;


	//比較用画像を読み込む (アルファチャンネル非対応のため、IMREAD_COLORで強制する)
	cv::Mat scene1 = cv::imread(scene1_path, cv::IMREAD_COLOR);
	cv::Mat scene2 = cv::imread(scene2_path, cv::IMREAD_COLOR);

	//アルゴリズムにAKAZEを使用する
	auto algorithm = cv::AKAZE::create();

	// 特徴点抽出
	std::vector<cv::KeyPoint> keypoint1, keypoint2;
	algorithm->detect(scene1, keypoint1);
	algorithm->detect(scene2, keypoint2);

	// 特徴記述
	cv::Mat descriptor1, descriptor2;
	algorithm->compute(scene1, keypoint1, descriptor1);
	algorithm->compute(scene2, keypoint2, descriptor2);

	// マッチング (アルゴリズムにはBruteForceを使用)
	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce");
	std::vector<cv::DMatch> match, match12, match21;
	matcher->match(descriptor1, descriptor2, match12);
	matcher->match(descriptor2, descriptor1, match21);
	//クロスチェック(1→2と2→1の両方でマッチしたものだけを残して精度を高める)
	for (size_t i = 0; i < match12.size(); i++)
	{
		cv::DMatch forward = match12[i];
		cv::DMatch backward = match21[forward.trainIdx];
		if (backward.trainIdx == forward.queryIdx)
		{
			std::cout << "Match!\n";
			match.push_back(forward);
		}
	}

	// マッチング結果の描画
	cv::Mat dest;
	cv::drawMatches(scene1, keypoint1, scene2, keypoint2, match, dest);
	imshow("dest", dest);

	//マッチング結果の書き出し
	//cv::imwrite(scene_12_path, dest);
}

void Diff()
{

	// 画像をグレースケールで読み込む.
	std::string strFileName1 = BASE_IMAGE_01;
	cv::Mat img1 = cv::imread(strFileName1, 0);

	std::string strFileName2 = BASE_IMAGE_02;
	cv::Mat img2 = cv::imread(strFileName2, 0);
	   
	cv::Mat dst, diff, gry, bg;

	//閾値
	const char th = 20;

	imshow("img1", img1);
	imshow("img2", img2);

	absdiff(img1, img2, diff);//背景画像との差分を取得
	imshow("diff", diff);

	cv::threshold(diff, dst, th, 255, cv::THRESH_BINARY);//二値化画像化

	imshow("dst", dst);
	


}

void CheckDiffAbsImage()
{
	cv::Mat img1 = cv::Mat::zeros(500, 500, CV_8UC1);
	circle(img1, cv::Point(200, 250), 100, cv::Scalar(100), 30, 4);
	imshow("1", img1);

	cv::Mat img2 = cv::Mat::zeros(500, 500, CV_8UC1);
	circle(img2, cv::Point(300, 250), 100, cv::Scalar(50), 30, 4);
	imshow("2", img2);

	//同じことを実行するコード パターン1
	cv::Mat output_img1;
	absdiff(img1, img2, output_img1);
	printf("%d\n", countNonZero(output_img1));
	///////////////////////


	//同じことを実行するコード パターン2
	cv::Mat output_img2;
	output_img2 = abs(img1 - img2);
	printf("%d\n", countNonZero(output_img2));
	///////////////////////

	imshow("3", output_img1);
	imshow("4", output_img2);

	return;
}

// 特異点を表示
void CheckDiffImage()
{

	// 画像をグレースケールで読み込む.
	std::string strFileName1 = BASE_IMAGE_01;
	cv::Mat img1 = cv::imread(strFileName1, 0);

	// 画像を二値化する.
	cv::Mat imgThreshold1;
	cv::threshold(img1, imgThreshold1, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

	std::vector<cv::KeyPoint> keypoints;
	cv::Mat descriptors;

	cv::Ptr<cv::AKAZE> akaze = cv::AKAZE::create();
	// 特徴点の抽出.
	akaze->detect(imgThreshold1, keypoints);

	// 特徴点の描画.
	cv::drawKeypoints(img1, keypoints, img1);

	// 画像を表示する.
	cv::imshow("match", img1);

	cv::waitKey(0);
	cv::destroyAllWindows();

}

void CreateColoerWindow()
{

	// （1）幅320px、高さ240pxで赤色の画像データを生成
	cv::Mat redImg(cv::Size(320, 240), CV_8UC3, cv::Scalar(0, 0, 255));//RGBではなく、BGR

	// （2）画像表示用のウィンドウを生成
	cv::namedWindow("red", cv::WINDOW_AUTOSIZE);

	// （3）ウィンドウに画像を表示
	cv::imshow("red", redImg);

	// （4）キー入力を待機
	cv::waitKey(0);

	// （5）作成したウィンドウを全て破棄
	cv::destroyAllWindows();
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
