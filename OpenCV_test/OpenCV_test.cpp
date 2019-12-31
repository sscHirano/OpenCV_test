// OpenCV_test.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#include <iostream>
#include <opencv2/core.hpp>    // coreモジュールのヘッダーをインクルード
#include "opencv2/core/mat.hpp"		// cv:mat
#include <opencv2/highgui.hpp> // highguiモジュールのヘッダーをインクルード
#include <opencv2/imgproc.hpp>		// cv:circle
#include <opencv2/imgproc/types_c.h>	// CV_BGR2GRAY

//関数宣言
void DetectActiveObjectFromVideo();
std::vector<cv::Rect> DoutaiKenchi(cv::Mat image1st, cv::Mat image2nd, cv::Mat image3rd);//動体検知
std::vector<cv::Rect> GetDiffRect(cv::Mat);

#define BASE_MOVIE_01 "C:\\Users\\s.hirano\\source\\repos\\OpenCV_test\\contents\\video\\vtest.avi"

///////////////////////////////////////////
// 調整オプション
// マスク時のサイズ
const int MASK_SIZE = 5;
// 動体検知サイズ
const double OBJECT_SIZE_THREASHOLD = 100;


int main()
{
//    std::cout << "Hello World!\n";

	// 動画を1フレームずつ取得して、差分をチェックして、画像に差分を描画する。
	DetectActiveObjectFromVideo();

	// 終了待ち
	std::cout << "終了\n";
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
		if (!frames[1].empty())
		{
			// 2つ目を1つ目へ
			frames[0] = frames[1].clone();
		}
		if (!frames[2].empty())
		{
			// 3つ目を2つ目へ
			frames[1] = frames[2].clone();
		}

		// 最新の位置に画像格納
		frame = &frames[MAX_FRAME_NUM - 1];
		vcap >> *frame;
		if (!frame->empty()) {
			// frame取得成功
						
			//グレースケール化
			cv::Mat gray;
			cvtColor(*frame, gray, CV_BGR2GRAY);
			//imshow("gray", gray);

			*frame = gray.clone();

			std::vector<cv::Rect> rects = DoutaiKenchi(frames[0], frames[1], frames[2]);//動体検知

			if (!rects.empty())
			{
				cv::Mat showFrame = frame->clone();
				for (cv::Rect rect : rects)
				{
					cv::rectangle(showFrame, cv::Point(rect.x, rect.y), cv::Point(rect.x + rect.width, rect.y + rect.height), cv::Scalar(0, 0, 255), 3, 4);
				}
				imshow("frame", showFrame);
			}
			else
			{
				imshow("frame", *frame);
			}
		}
		else
		{
			// frame取得失敗
			std::cout << "End of frame.\n";
			break;
		}
		// このwaitが無いと、処理が追い付かないのか、何も描画は更新されない。
		// 描画の隙間を作るのと、キーが押されたら停止するためのwait
		if (cv::waitKey(30) >= 0) break; 
	}
}

//参考
//https://kimamani89.com/2019/04/30/post-420/
std::vector<cv::Rect> DoutaiKenchi(cv::Mat image1st, cv::Mat image2nd, cv::Mat image3rd)
{
	std::vector<cv::Rect> rects;

	static int frameCnt;
	frameCnt++;
	if (image1st.empty() || image2nd.empty() || image3rd.empty())
	{
		//3枚の画像がそろってない場合は何もしない
		std::cout << "Not ready(wait 3 frames).\n";
		return rects;
	}
	std::cout << "frame[" << frameCnt << "] proc.\n";
	// 絶対値の求めたのち、背景差分を求める 
	cv::Mat diff1,diff2;
	cv::absdiff(image2nd, image1st, diff1);
	//imshow("diff1", diff1);
	cv::absdiff(image3rd, image2nd, diff2);
	//imshow("diff2", diff2);

	cv::Mat im;
	// 背景差分から論理積の算出
	cv::bitwise_and(diff1, diff2, im);

	//二値化
	cv::Mat img_th;
	cv::threshold(im, img_th, 10, 255, CV_THRESH_BINARY);

	//膨張処理・収縮処理を施してマスク画像を生成
	cv::Mat element(MASK_SIZE, MASK_SIZE, CV_8U, cv::Scalar(1));
	cv::Mat img_dilate;
	cv::dilate(img_th, img_dilate, element, cv::Point(-1, -1), 1);
	cv::Mat mask;
	cv::erode(img_dilate, mask, element, cv::Point(-1, -1), 1);

	// マスク画像を使って対象を切り出す
	cv::Mat img_dst;
	cv::bitwise_and(image3rd, mask, img_dst);

	//大きな差分の矩形を探す
	rects = GetDiffRect(img_dst);


	imshow("img_dst", img_dst);

	return rects;
}

std::vector<cv::Rect> GetDiffRect(cv::Mat maskImage)
{
	// 検知物体の二次元配列
	std::vector<std::vector<cv::Point> > contours;

	// 物体領域の一次元配列
	std::vector<cv::Rect> rects;

	// 物体検知
	cv::findContours(maskImage, contours, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);
	for (std::vector<cv::Point>& points : contours)
	{
		//物体の面積取得
		double areaSize = cv::contourArea(points);
		// 小さい物体は無視する
		if (areaSize > OBJECT_SIZE_THREASHOLD)
		{
			std::cout << "areaSize = " << areaSize << ".\n";
			std::cout << "point size = " << points.size() << ".\n";

			cv::Rect rect;

			int top, bottom, left, right;
			top = left = INT_MAX;
			bottom = right = INT_MIN;

			// 点の集合から矩形の座標を作成 (もっと簡単な方法がある予感)
			for (cv::Point point : points)
			{
				if (top > point.y)
				{
					top = point.y;
				}
				if (bottom < point.y)
				{
					bottom = point.y;
				}
				if (left > point.x)
				{
					left = point.x;
				}
				if (right < point.x)
				{
					right = point.x;
				}
			}

			rect.x = left;
			rect.y = top;
			rect.width = right - left;
			rect.height = bottom - top;
			printf("Rect x(%d) y(%d) w(%d) h(%d)\n", rect.x, rect.y, rect.width, rect.height);

			rects.push_back(rect);
		}
	}
	std::cout << "bigPoint size = " << rects.size() << ".\n";
	return rects;
}
