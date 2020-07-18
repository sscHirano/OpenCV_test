

#include <iostream>

#if 0
#pragma comment(lib, "opencv_core412.lib")
#pragma comment(lib, "opencv_imgcodecs412.lib")
#pragma comment(lib, "opencv_videoio412.lib")
#pragma comment(lib, "opencv_highgui412.lib")
#pragma comment(lib, "opencv_tracking412.lib")
#pragma comment(lib, "opencv_imgproc412.lib")
#endif

#include "ObjectTracker.h"

void ObjectTracker::startTracking()
{
    // inputオープン
    cv::VideoCapture cap;

    if (commonSetting.inputType == CommonSetting::EInputType::E_INPUT_TYPE_CAMERA)
    {
        // カメラ
        cap.open(0);
    }
    else
    {
        // 動画
        cap.open(commonSetting.inputFilePath);
    }

    if (!cap.isOpened()) {
        std::cout << "ビデオが開けません。" << std::endl;
        return;
    }

    if (objectTrackSetting.trackingMode == ObjectTrackSetting::ETrackingMode::E_PATARN_MATCH)
    {
        //パターンマッチングモード
        TargetInfo targetInfo = trackPatternMatching(cap);
    }
    else
    {
        // オブジェクトトラッキングモード
        trackObject(cap);
    }
    return;
}

/*
    frameから追跡対象を探す
    最後のフレーム画像と、追跡対象の矩形を返却する
*/
TargetInfo ObjectTracker::trackPatternMatching(cv::VideoCapture cap)
{
    cv::Mat frame;
    TargetInfo targetInfo;

    // 対象画像の読み込み
    cv::Mat targetImage = cv::imread(objectTrackSetting.targetFilePath);

    cv::imshow("targetImage", targetImage);
    cap >> frame;
    int cutLeft = 300;
    int cutRight = 200;
    int cutTop = 50;
    int cutBottom = 50;
    cv::Size frameSize = cv::Size(frame.cols - cutLeft - cutRight, frame.rows - cutTop - cutBottom);
    cv::VideoWriter writer = startWriteVideo(frameSize);

    // 物体を探す。
    while (1) {
        //std::cout << "next frame.\n";
        if (frame.empty())
        {
            std::cout << "frame empty in search target loop.\n";
            break;
        }
        // 余分な領域を削除してから画像検索する。
        frame = frame(cv::Rect(cutLeft, cutTop, frame.cols - cutLeft - cutRight, frame.rows - cutTop - cutBottom));

        trackPatternMatchingInternal(frame, targetImage);

        //動画保存
        writeVideo(writer, frame);

        cv::imshow("trackPatternMatching", frame);
        int key = cv::waitKey(1);
        if (key == 0x1b) //ESCキー
            break;

        // 次フレーム読み込み
        cap >> frame;
    }
    return targetInfo;
}

cv::VideoWriter ObjectTracker::startWriteVideo(cv::Size size)
{
    if (!objectTrackSetting.enableSaveVideo)
    {
        //保存しない設定。
        cv::VideoWriter nowrite;
        return nowrite;
    }
    int    fourcc = cv::VideoWriter::fourcc('X', 'V', 'I', 'D'); // コーデックを指定 
    double fps = 30.0;   // 動画のフレームレートを指定
    bool   isColor = true;   // カラーで保存するか否か

    // 出力する動画ファイルの設定
    cv::VideoWriter writer(objectTrackSetting.saveVideoPath, fourcc, fps, size, isColor);

    // 動画ファイルの初期化に成功したか判定
    if (!writer.isOpened()) {
        std::cout << "Can't VideoWriter open" << std::endl;
        assert(false);
    }
    return writer;
}

void ObjectTracker::writeVideo(cv::VideoWriter writer, cv::Mat& frame)
{
    if (!writer.isOpened())
    {
        return;
    }
    // 動画ファイルに出力
    writer << frame;
}

void ObjectTracker::trackPatternMatchingInternal(cv::Mat &frame, cv::Mat &targetImage)
{

    //探索範囲を狭める対応
    static int x = 0;
    static int y = 0;

    // 前回のパターンマッチエリアより少し大きいエリアから探索する
    int searchExPixel = 100;
    // xから探索拡大ピクセル数を引く。0より小さければ、0にする
    x = (x - searchExPixel < 0) ? 0 : x - searchExPixel;
    y = (y - searchExPixel < 0) ? 0 : y - searchExPixel;

    // 開始地点が0なら全エリア探索。そうでなければ、見つけたエリアより少し大きいエリアを対象とする
    int width = (x == 0) ? frame.cols : targetImage.cols + searchExPixel * 2; //開始地点はserchExpixel分引いているので、終点は、2倍足す
    int height = (y == 0) ? frame.rows : targetImage.rows + searchExPixel * 2;


    // 探索範囲を狭めるための画像切り抜き
    // 範囲オーバーしたら補正する
    if ((x + width > frame.cols) || (y + height > frame.rows))
    {
        x = y = 0;
        width = frame.cols;
        height = frame.rows;
    }
        
    std::cout << "x:" << x << " y:" << y << " width:" << width << " height:" << height << "\n";
    cv::Mat cutFrame = frame(cv::Rect(x, y, width, height));
    //デバッグ用切り取りフレーム表示
    //cv::imshow("cutFrame", cutFrame);

    //元画像、対象画像を共に2値化
    convertColor2Monochrome(cutFrame);
    //convertColor2Monochrome(targetImage);       //TODO:ターゲットイメージは毎回同じ画像なので、これを毎回2値化するのは無駄。

    // テンプレートと，それに重なった画像領域とを比較
    cv::Mat result;
    cv::matchTemplate(
        cutFrame,               // テンプレートの探索対象となる画像．8ビットまたは32ビットの浮動小数点型．
        targetImage,         // 探索されるテンプレート．探索対象となる画像以下のサイズで，同じデータ型でなければならない
        result,              // 比較結果のマップ
        objectTrackSetting.matchMeathod  // 比較手法
        );

    double val;
    cv::Point pt;

    // 配列全体あるいは部分配列に対する，大域的最小値
    cv::minMaxLoc(result, &val, nullptr, &pt, nullptr);

    // ptは、カットした画像から位置を求めているため、元の画像の座標に戻す
    pt.x += x;
    pt.y += y;

    showXYPosition(frame, pt);

    //TODO:とりあえず左上の座標を出しているが、本来は中心点を求めるべき。
    //cv::Point targetPoint = pt + cv::Point(targetImage.cols, targetImage.rows);
    std::cout << "x:" << pt.x << " y:" << pt.y << "\n";

    //std::cout << "minmax:" << val <<"\n";
    // 結果が小さいほど一致率が高いと判断する
    //if (val < objectTrackSetting.matchThreashold)
    {
        // 矩形を描画
        cv::rectangle(frame,
            pt,                                   // 矩形の1つの頂点座標
            pt + cv::Point(targetImage.cols, targetImage.rows),   // 矩形の反対側の頂点座標
            cv::Scalar(0, 0, 255),                // 色
            2,                                    // 矩形の枠線の太さ
            cv::LINE_8                            // 枠線の種類
        );
    }

    // 次回の探索を覚えるため、位置を記憶
    x = pt.x;
    y = pt.y;
}

/*
二値化
グレースケールに変換してから2値化する。
*/
cv::Mat ObjectTracker::convertColor2Monochrome(cv::Mat& image)
{
    cv::Mat gray_img;
    cv::Mat bin_img;
    cvtColor(image, gray_img, CV_BGR2GRAY);
//    threshold(gray_img, bin_img, 160, 255, cv::THRESH_BINARY);                    // 160を閾値に2値化
    threshold(gray_img, bin_img, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);    // 自動で閾値を決めて2値化

    //デバッグ用2値化画像表示
    cv::imshow("monochromeFrame", bin_img);

//    bin_img.copyTo(image);
    return bin_img;
}

void ObjectTracker::showXYPosition(cv::Mat& frame, cv::Point& pt)
{
    if (!objectTrackSetting.enableShowXYPosition)
    {
        //設定OFFなら表示しない
        return;
    }
    cv::Scalar stringColor = cv::Scalar(255, 255, 255);   //文字色
    std::string printString = "X:" + std::to_string(pt.x) + " Y:" + std::to_string(pt.y);
    cv::putText(frame, printString, cv::Point(5, 30), cv::FONT_HERSHEY_SIMPLEX, 1, stringColor);
}

void ObjectTracker::trackObject(cv::VideoCapture cap)
{
    cv::Mat frame;
    cap >> frame;
    // 最初のフレームから追跡対象を選択
    cv::Rect2d roi = cv::selectROI("tracker", frame); // マウスで中心を選択してドラッグ
    cv::Rect2d roiTLD = roi;
    cv::Rect2d roiMEDIANFLOW = roi;
    cv::Rect2d roiBOOSTING = roi;
    cv::Rect2d roiMIL = roi;

    // ターゲット画像の保存
    //cv::Mat target(frame, roi);
    //cv::imwrite("target.jpeg", target);

    std::cout << "(x, y, width, height) = (" << roi.x << "," << roi.y << "," << roi.width << "," << roi.height << ")" << std::endl;

    if (roi.width == 0 || roi.height == 0)
    {
        return;
    }

    // Trackerの生成
    cv::Ptr<cv::TrackerKCF> trackerKCF = cv::TrackerKCF::create();
    cv::Ptr<cv::TrackerTLD> trackerTLD = cv::TrackerTLD::create();
    cv::Ptr<cv::TrackerMedianFlow> trackerMEDIANFLOW = cv::TrackerMedianFlow::create();
    cv::Ptr<cv::TrackerBoosting> trackerBOOSTING = cv::TrackerBoosting::create();
    cv::Ptr<cv::TrackerMIL> trackerMIL = cv::TrackerMIL::create();

    // Trackerの初期化
    trackerKCF->init(frame, roi);
    trackerTLD->init(frame, roiTLD);
    trackerMEDIANFLOW->init(frame, roiMEDIANFLOW);
    trackerBOOSTING->init(frame, roiBOOSTING);
    trackerMIL->init(frame, roiMIL);

    // Trackerごとの色
    cv::Scalar colorkcf = cv::Scalar(0, 255, 0);            // KCF          :G
    cv::Scalar colortld = cv::Scalar(0, 255, 255);          // TLD          :GB
    cv::Scalar colormedianflow = cv::Scalar(0, 0, 255);     // MedianFlow   :B
    cv::Scalar colorboosting = cv::Scalar(255, 255, 0);     // Boosting     :RG
    cv::Scalar colormil = cv::Scalar(255, 0, 255);          // MIL          :RB

    // 動画保存の設定
    /*
    double fps = cap.get(CV_CAP_PROP_FPS);
    cv::Size size = cv::Size(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));
    const int fourcc = cv::VideoWriter::fourcc('X', 'V', 'I', 'D');
    std::string filename = "output.avi";
    cv::VideoWriter writer(filename, fourcc, fps, size);
    */

    while (1) {
        //std::cout << "next frame.\n";
        cap >> frame;
        if (frame.empty())
        {
            std::cout << "frame empty in tracking loop.\n";
            break;
        }

        // KCF
        if (objectTrackSetting.enableTrackKcf) {
            update(trackerKCF, frame, roi, colorkcf);
        }

        //TLD
        if (objectTrackSetting.enableTrackTld) {
            update(trackerTLD, frame, roiTLD, colortld);
        }

        //MedianFlow
        if (objectTrackSetting.enableTrackMedianFlow) {
            update(trackerMEDIANFLOW, frame, roiMEDIANFLOW, colormedianflow);
        }

        //Boosting
        if (objectTrackSetting.enableTrackBoosting) {
            update(trackerBOOSTING, frame, roiBOOSTING, colorboosting);
        }

        //MIL
        if (objectTrackSetting.enableTrackMil) {
            update(trackerMIL, frame, roiMIL, colormil);
        }

        cv::imshow("tracker", frame);
        //writer << frame;

        // ここを無効にすると画像フレームが変わらない。。。
        int key = cv::waitKey(1);
        if (key == 0x1b) //ESCキー
            break;
    }

}

void ObjectTracker::update(cv::Ptr<cv::Tracker> tracker, cv::Mat& frame, cv::Rect2d& roi, cv::Scalar& color)
{
    // 更新
    tracker->update(frame, roi);
    // 矩形で囲む
    cv::rectangle(frame, roi, color, 1, 1);
    //cv::putText(frame, "- XXX", cv::Point(5, 20), cv::FONT_HERSHEY_SIMPLEX, .5, color, 1, 16);//CV_AAは16だったらしい
}
