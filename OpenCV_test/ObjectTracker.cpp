

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
    // input�I�[�v��
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cout << "�r�f�I���J���܂���B" << std::endl;
        return;
    }

    // Tracker�̐���
    cv::Ptr<cv::TrackerKCF> trackerKCF = cv::TrackerKCF::create();
    cv::Ptr<cv::TrackerTLD> trackerTLD = cv::TrackerTLD::create();
    cv::Ptr<cv::TrackerMedianFlow> trackerMEDIANFLOW = cv::TrackerMedianFlow::create();
    cv::Ptr<cv::TrackerBoosting> trackerBOOSTING = cv::TrackerBoosting::create();
    cv::Ptr<cv::TrackerMIL> trackerMIL = cv::TrackerMIL::create();

    cv::Mat frame;
    cap >> frame;

    // �ŏ��̃t���[������ǐՑΏۂ�I��
    cv::Rect2d roi = cv::selectROI("tracker", frame); // �}�E�X�Œ��S��I�����ăh���b�O
    cv::Rect2d roiTLD = roi;
    cv::Rect2d roiMEDIANFLOW = roi;
    cv::Rect2d roiBOOSTING = roi;
    cv::Rect2d roiMIL = roi;

    cv::Mat target(frame, roi);
    cv::imwrite("target.jpeg", target);
    std::cout << "(x, y, width, height) = (" << roi.x << "," << roi.y << "," << roi.width << "," << roi.height << ")" << std::endl;

    if (roi.width == 0 || roi.height == 0)
    {
        return;
    }

    // Tracker�̏�����

    trackerKCF->init(frame, roi);
    trackerTLD->init(frame, roiTLD);
    trackerMEDIANFLOW->init(frame, roiMEDIANFLOW);
    trackerBOOSTING->init(frame, roiBOOSTING);
    trackerMIL->init(frame, roiMIL);

    // Tracker���Ƃ̐F
    cv::Scalar colorkcf = cv::Scalar(0, 255, 0);            // KCF          :G
    cv::Scalar colortld = cv::Scalar(0, 255, 255);          // TLD          :GB
    cv::Scalar colormedianflow = cv::Scalar(0, 0, 255);     // MedianFlow   :B
    cv::Scalar colorboosting = cv::Scalar(255, 255, 0);     // Boosting     :RG
    cv::Scalar colormil = cv::Scalar(255, 0, 255);          // MIL          :RB

    // ����ۑ��̐ݒ�
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
            break;
        
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

        // �����𖳌��ɂ���Ɖ摜�t���[�����ς��Ȃ��B�B�B
        int key = cv::waitKey(1);
        if (key == 0x1b) //ESC�L�[
            break;
    }
    return;
}

void ObjectTracker::update(cv::Ptr<cv::Tracker> tracker, cv::Mat& frame, cv::Rect2d& roi, cv::Scalar& color)
{
    // �X�V
    tracker->update(frame, roi);
    // ��`�ň͂�
    cv::rectangle(frame, roi, color, 1, 1);
    //cv::putText(frame, "- XXX", cv::Point(5, 20), cv::FONT_HERSHEY_SIMPLEX, .5, color, 1, 16);//CV_AA��16�������炵��
}
