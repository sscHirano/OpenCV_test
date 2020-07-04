

#include <iostream>

#if 1
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
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cout << "ビデオが開けません。" << std::endl;
        return;
    }

    // Trackerの生成
    cv::Ptr<cv::TrackerKCF> trackerKCF = cv::TrackerKCF::create();
    cv::Ptr<cv::TrackerTLD> trackerTLD = cv::TrackerTLD::create();
    cv::Ptr<cv::TrackerMedianFlow> trackerMEDIANFLOW = cv::TrackerMedianFlow::create();
    cv::Ptr<cv::TrackerBoosting> trackerBOOSTING = cv::TrackerBoosting::create();
    cv::Ptr<cv::TrackerMIL> trackerMIL = cv::TrackerMIL::create();

    cv::Mat frame;

#if 1 //パターンマッチングモード
    // 検知対象を探す。
    TargetInfo targetInfo = searchTargetObject(cap);
    frame = targetInfo.frame;
    cv::Rect2d roi = targetInfo.roi;
    cv::Rect2d roiTLD = roi;
    cv::Rect2d roiMEDIANFLOW = roi;
    cv::Rect2d roiBOOSTING = roi;
    cv::Rect2d roiMIL = roi;

#else   // オブジェクトトラッキングモード
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

#endif

    std::cout << "(x, y, width, height) = (" << roi.x << "," << roi.y << "," << roi.width << "," << roi.height << ")" << std::endl;

    if (roi.width == 0 || roi.height == 0)
    {
        return;
    }

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
    return;
}

/*
    frameから追跡対象を探す
    最後のフレーム画像と、追跡対象の矩形を返却する
*/
TargetInfo ObjectTracker::searchTargetObject(cv::VideoCapture cap)
{
    cv::Mat frame;
    TargetInfo targetInfo;

    // 対象画像の読み込み
    cv::Mat targetImage = cv::imread("C:\\Users\\s.hirano\\Documents\\プロジェクト\\InsectEye\\OpenCV\\追跡対象.png");

    // 物体を探す。
    while (1) {
        //std::cout << "next frame.\n";
        cap >> frame;
        if (frame.empty())
        {
            std::cout << "frame empty in search target loop.\n";
            break;
        }
        searchTargetInternal(frame, targetImage);

        // 物体を見つけた。
        if (false)
        {
            std::cout << "Find target object!\n";
            targetInfo.frame = frame;
//            targetInfo.roi =  //cv::Rect2d
            break;
        }
        cv::imshow("tracker", frame);
        int key = cv::waitKey(1);
        if (key == 0x1b) //ESCキー
            break;
    }
    return targetInfo;
}

void ObjectTracker::searchTargetInternal(cv::Mat frame, cv::Mat targetImage)
{
    cv::Mat result;
    // テンプレートと，それに重なった画像領域とを比較
    cv::matchTemplate(
        frame,               // テンプレートの探索対象となる画像．8ビットまたは32ビットの浮動小数点型．
        targetImage,         // 探索されるテンプレート．探索対象となる画像以下のサイズで，同じデータ型でなければならない
        result,              // 比較結果のマップ
        CV_TM_SQDIFF_NORMED  // 比較手法
        );

    double val;
    cv::Point pt;

    // 配列全体あるいは部分配列に対する，大域的最小値
    cv::minMaxLoc(result, &val, nullptr, &pt, nullptr);
    std::cout << "minmax:" << val <<"\n";
    // 結果が小さいほど一致率が高いと判断する
    if (val < 0.3)
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

    //cv::namedWindow("frame", 1);
    //imshow("frame", frame);

    //cv::namedWindow("targetImage", 1);
    //cv::imshow("targetImage", targetImage);

//    cv::waitKey(0);
}
void ObjectTracker::update(cv::Ptr<cv::Tracker> tracker, cv::Mat& frame, cv::Rect2d& roi, cv::Scalar& color)
{
    // 更新
    tracker->update(frame, roi);
    // 矩形で囲む
    cv::rectangle(frame, roi, color, 1, 1);
    //cv::putText(frame, "- XXX", cv::Point(5, 20), cv::FONT_HERSHEY_SIMPLEX, .5, color, 1, 16);//CV_AAは16だったらしい
}
