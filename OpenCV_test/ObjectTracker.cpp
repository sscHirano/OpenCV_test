

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

#if 1 //�p�^�[���}�b�`���O���[�h
    // ���m�Ώۂ�T���B
    TargetInfo targetInfo = searchTargetObject(cap);
    frame = targetInfo.frame;
    cv::Rect2d roi = targetInfo.roi;
    cv::Rect2d roiTLD = roi;
    cv::Rect2d roiMEDIANFLOW = roi;
    cv::Rect2d roiBOOSTING = roi;
    cv::Rect2d roiMIL = roi;

#else   // �I�u�W�F�N�g�g���b�L���O���[�h
    cap >> frame;
    // �ŏ��̃t���[������ǐՑΏۂ�I��
    cv::Rect2d roi = cv::selectROI("tracker", frame); // �}�E�X�Œ��S��I�����ăh���b�O
    cv::Rect2d roiTLD = roi;
    cv::Rect2d roiMEDIANFLOW = roi;
    cv::Rect2d roiBOOSTING = roi;
    cv::Rect2d roiMIL = roi;

    // �^�[�Q�b�g�摜�̕ۑ�
    //cv::Mat target(frame, roi);
    //cv::imwrite("target.jpeg", target);

#endif

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

        // �����𖳌��ɂ���Ɖ摜�t���[�����ς��Ȃ��B�B�B
        int key = cv::waitKey(1);
        if (key == 0x1b) //ESC�L�[
            break;
    }
    return;
}

/*
    frame����ǐՑΏۂ�T��
    �Ō�̃t���[���摜�ƁA�ǐՑΏۂ̋�`��ԋp����
*/
TargetInfo ObjectTracker::searchTargetObject(cv::VideoCapture cap)
{
    cv::Mat frame;
    TargetInfo targetInfo;

    // �Ώۉ摜�̓ǂݍ���
    cv::Mat targetImage = cv::imread("C:\\Users\\s.hirano\\Documents\\�v���W�F�N�g\\InsectEye\\OpenCV\\�ǐՑΏ�.png");

    // ���̂�T���B
    while (1) {
        //std::cout << "next frame.\n";
        cap >> frame;
        if (frame.empty())
        {
            std::cout << "frame empty in search target loop.\n";
            break;
        }
        searchTargetInternal(frame, targetImage);

        // ���̂��������B
        if (false)
        {
            std::cout << "Find target object!\n";
            targetInfo.frame = frame;
//            targetInfo.roi =  //cv::Rect2d
            break;
        }
        cv::imshow("tracker", frame);
        int key = cv::waitKey(1);
        if (key == 0x1b) //ESC�L�[
            break;
    }
    return targetInfo;
}

void ObjectTracker::searchTargetInternal(cv::Mat frame, cv::Mat targetImage)
{
    cv::Mat result;
    // �e���v���[�g�ƁC����ɏd�Ȃ����摜�̈�Ƃ��r
    cv::matchTemplate(
        frame,               // �e���v���[�g�̒T���ΏۂƂȂ�摜�D8�r�b�g�܂���32�r�b�g�̕��������_�^�D
        targetImage,         // �T�������e���v���[�g�D�T���ΏۂƂȂ�摜�ȉ��̃T�C�Y�ŁC�����f�[�^�^�łȂ���΂Ȃ�Ȃ�
        result,              // ��r���ʂ̃}�b�v
        CV_TM_SQDIFF_NORMED  // ��r��@
        );

    double val;
    cv::Point pt;

    // �z��S�̂��邢�͕����z��ɑ΂���C���I�ŏ��l
    cv::minMaxLoc(result, &val, nullptr, &pt, nullptr);
    std::cout << "minmax:" << val <<"\n";
    // ���ʂ��������قǈ�v���������Ɣ��f����
    if (val < 0.3)
    {
        // ��`��`��
        cv::rectangle(frame,
            pt,                                   // ��`��1�̒��_���W
            pt + cv::Point(targetImage.cols, targetImage.rows),   // ��`�̔��Α��̒��_���W
            cv::Scalar(0, 0, 255),                // �F
            2,                                    // ��`�̘g���̑���
            cv::LINE_8                            // �g���̎��
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
    // �X�V
    tracker->update(frame, roi);
    // ��`�ň͂�
    cv::rectangle(frame, roi, color, 1, 1);
    //cv::putText(frame, "- XXX", cv::Point(5, 20), cv::FONT_HERSHEY_SIMPLEX, .5, color, 1, 16);//CV_AA��16�������炵��
}
