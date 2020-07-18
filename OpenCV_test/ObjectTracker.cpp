

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
    // input�I�[�v��
    cv::VideoCapture cap;

    if (commonSetting.inputType == CommonSetting::EInputType::E_INPUT_TYPE_CAMERA)
    {
        // �J����
        cap.open(0);
    }
    else
    {
        // ����
        cap.open(commonSetting.inputFilePath);
    }

    if (!cap.isOpened()) {
        std::cout << "�r�f�I���J���܂���B" << std::endl;
        return;
    }

    if (objectTrackSetting.trackingMode == ObjectTrackSetting::ETrackingMode::E_PATARN_MATCH)
    {
        //�p�^�[���}�b�`���O���[�h
        TargetInfo targetInfo = trackPatternMatching(cap);
    }
    else
    {
        // �I�u�W�F�N�g�g���b�L���O���[�h
        trackObject(cap);
    }
    return;
}

/*
    frame����ǐՑΏۂ�T��
    �Ō�̃t���[���摜�ƁA�ǐՑΏۂ̋�`��ԋp����
*/
TargetInfo ObjectTracker::trackPatternMatching(cv::VideoCapture cap)
{
    cv::Mat frame;
    TargetInfo targetInfo;

    // �Ώۉ摜�̓ǂݍ���
    cv::Mat targetImage = cv::imread(objectTrackSetting.targetFilePath);

    cv::imshow("targetImage", targetImage);
    cap >> frame;
    int cutLeft = 300;
    int cutRight = 200;
    int cutTop = 50;
    int cutBottom = 50;
    cv::Size frameSize = cv::Size(frame.cols - cutLeft - cutRight, frame.rows - cutTop - cutBottom);
    cv::VideoWriter writer = startWriteVideo(frameSize);

    // ���̂�T���B
    while (1) {
        //std::cout << "next frame.\n";
        if (frame.empty())
        {
            std::cout << "frame empty in search target loop.\n";
            break;
        }
        // �]���ȗ̈���폜���Ă���摜��������B
        frame = frame(cv::Rect(cutLeft, cutTop, frame.cols - cutLeft - cutRight, frame.rows - cutTop - cutBottom));

        trackPatternMatchingInternal(frame, targetImage);

        //����ۑ�
        writeVideo(writer, frame);

        cv::imshow("trackPatternMatching", frame);
        int key = cv::waitKey(1);
        if (key == 0x1b) //ESC�L�[
            break;

        // ���t���[���ǂݍ���
        cap >> frame;
    }
    return targetInfo;
}

cv::VideoWriter ObjectTracker::startWriteVideo(cv::Size size)
{
    if (!objectTrackSetting.enableSaveVideo)
    {
        //�ۑ����Ȃ��ݒ�B
        cv::VideoWriter nowrite;
        return nowrite;
    }
    int    fourcc = cv::VideoWriter::fourcc('X', 'V', 'I', 'D'); // �R�[�f�b�N���w�� 
    double fps = 30.0;   // ����̃t���[�����[�g���w��
    bool   isColor = true;   // �J���[�ŕۑ����邩�ۂ�

    // �o�͂��铮��t�@�C���̐ݒ�
    cv::VideoWriter writer(objectTrackSetting.saveVideoPath, fourcc, fps, size, isColor);

    // ����t�@�C���̏������ɐ�������������
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
    // ����t�@�C���ɏo��
    writer << frame;
}

void ObjectTracker::trackPatternMatchingInternal(cv::Mat &frame, cv::Mat &targetImage)
{

    //�T���͈͂����߂�Ή�
    static int x = 0;
    static int y = 0;

    // �O��̃p�^�[���}�b�`�G���A��菭���傫���G���A����T������
    int searchExPixel = 100;
    // x����T���g��s�N�Z�����������B0��菬������΁A0�ɂ���
    x = (x - searchExPixel < 0) ? 0 : x - searchExPixel;
    y = (y - searchExPixel < 0) ? 0 : y - searchExPixel;

    // �J�n�n�_��0�Ȃ�S�G���A�T���B�����łȂ���΁A�������G���A��菭���傫���G���A��ΏۂƂ���
    int width = (x == 0) ? frame.cols : targetImage.cols + searchExPixel * 2; //�J�n�n�_��serchExpixel�������Ă���̂ŁA�I�_�́A2�{����
    int height = (y == 0) ? frame.rows : targetImage.rows + searchExPixel * 2;


    // �T���͈͂����߂邽�߂̉摜�؂蔲��
    // �͈̓I�[�o�[������␳����
    if ((x + width > frame.cols) || (y + height > frame.rows))
    {
        x = y = 0;
        width = frame.cols;
        height = frame.rows;
    }
        
    std::cout << "x:" << x << " y:" << y << " width:" << width << " height:" << height << "\n";
    cv::Mat cutFrame = frame(cv::Rect(x, y, width, height));
    //�f�o�b�O�p�؂���t���[���\��
    //cv::imshow("cutFrame", cutFrame);

    //���摜�A�Ώۉ摜������2�l��
    convertColor2Monochrome(cutFrame);
    //convertColor2Monochrome(targetImage);       //TODO:�^�[�Q�b�g�C���[�W�͖��񓯂��摜�Ȃ̂ŁA����𖈉�2�l������͖̂��ʁB

    // �e���v���[�g�ƁC����ɏd�Ȃ����摜�̈�Ƃ��r
    cv::Mat result;
    cv::matchTemplate(
        cutFrame,               // �e���v���[�g�̒T���ΏۂƂȂ�摜�D8�r�b�g�܂���32�r�b�g�̕��������_�^�D
        targetImage,         // �T�������e���v���[�g�D�T���ΏۂƂȂ�摜�ȉ��̃T�C�Y�ŁC�����f�[�^�^�łȂ���΂Ȃ�Ȃ�
        result,              // ��r���ʂ̃}�b�v
        objectTrackSetting.matchMeathod  // ��r��@
        );

    double val;
    cv::Point pt;

    // �z��S�̂��邢�͕����z��ɑ΂���C���I�ŏ��l
    cv::minMaxLoc(result, &val, nullptr, &pt, nullptr);

    // pt�́A�J�b�g�����摜����ʒu�����߂Ă��邽�߁A���̉摜�̍��W�ɖ߂�
    pt.x += x;
    pt.y += y;

    showXYPosition(frame, pt);

    //TODO:�Ƃ肠��������̍��W���o���Ă��邪�A�{���͒��S�_�����߂�ׂ��B
    //cv::Point targetPoint = pt + cv::Point(targetImage.cols, targetImage.rows);
    std::cout << "x:" << pt.x << " y:" << pt.y << "\n";

    //std::cout << "minmax:" << val <<"\n";
    // ���ʂ��������قǈ�v���������Ɣ��f����
    //if (val < objectTrackSetting.matchThreashold)
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

    // ����̒T�����o���邽�߁A�ʒu���L��
    x = pt.x;
    y = pt.y;
}

/*
��l��
�O���[�X�P�[���ɕϊ����Ă���2�l������B
*/
cv::Mat ObjectTracker::convertColor2Monochrome(cv::Mat& image)
{
    cv::Mat gray_img;
    cv::Mat bin_img;
    cvtColor(image, gray_img, CV_BGR2GRAY);
//    threshold(gray_img, bin_img, 160, 255, cv::THRESH_BINARY);                    // 160��臒l��2�l��
    threshold(gray_img, bin_img, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);    // ������臒l�����߂�2�l��

    //�f�o�b�O�p2�l���摜�\��
    cv::imshow("monochromeFrame", bin_img);

//    bin_img.copyTo(image);
    return bin_img;
}

void ObjectTracker::showXYPosition(cv::Mat& frame, cv::Point& pt)
{
    if (!objectTrackSetting.enableShowXYPosition)
    {
        //�ݒ�OFF�Ȃ�\�����Ȃ�
        return;
    }
    cv::Scalar stringColor = cv::Scalar(255, 255, 255);   //�����F
    std::string printString = "X:" + std::to_string(pt.x) + " Y:" + std::to_string(pt.y);
    cv::putText(frame, printString, cv::Point(5, 30), cv::FONT_HERSHEY_SIMPLEX, 1, stringColor);
}

void ObjectTracker::trackObject(cv::VideoCapture cap)
{
    cv::Mat frame;
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

    std::cout << "(x, y, width, height) = (" << roi.x << "," << roi.y << "," << roi.width << "," << roi.height << ")" << std::endl;

    if (roi.width == 0 || roi.height == 0)
    {
        return;
    }

    // Tracker�̐���
    cv::Ptr<cv::TrackerKCF> trackerKCF = cv::TrackerKCF::create();
    cv::Ptr<cv::TrackerTLD> trackerTLD = cv::TrackerTLD::create();
    cv::Ptr<cv::TrackerMedianFlow> trackerMEDIANFLOW = cv::TrackerMedianFlow::create();
    cv::Ptr<cv::TrackerBoosting> trackerBOOSTING = cv::TrackerBoosting::create();
    cv::Ptr<cv::TrackerMIL> trackerMIL = cv::TrackerMIL::create();

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

}

void ObjectTracker::update(cv::Ptr<cv::Tracker> tracker, cv::Mat& frame, cv::Rect2d& roi, cv::Scalar& color)
{
    // �X�V
    tracker->update(frame, roi);
    // ��`�ň͂�
    cv::rectangle(frame, roi, color, 1, 1);
    //cv::putText(frame, "- XXX", cv::Point(5, 20), cv::FONT_HERSHEY_SIMPLEX, .5, color, 1, 16);//CV_AA��16�������炵��
}
