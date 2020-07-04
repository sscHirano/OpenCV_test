#include <opencv2/highgui.hpp> // highgui���W���[���̃w�b�_�[���C���N���[�h
#include <opencv2/imgproc.hpp>		// cv:circle
#include <opencv2/imgproc/types_c.h>	// CV_BGR2GRAY

#include "MoveDetector.h"

///////////////////////////////////////////////////
// �����1�t���[�����擾���āA�������`�F�b�N���āA�摜�ɍ�����`�悷��B
// https://kimamani89.com/2019/04/30/post-420/
// A. �A������3���i�摜1�E�摜2�E�摜3�j�̉摜��p��
// B. 3���̉摜���O���[�X�P�[���ɕϊ�
// C.���ꂼ��̉摜�i�摜2 - �摜1�E�摜3 - �摜2�j�̍����摜���쐬����
// D. 2���̍����摜�̘_���ς��v�Z���A�_���ω摜���쐬����
// E.�_���ω摜�ɓ�l���������s���A�w�i�ƑO�i�ɕ������}�X�N�摜���쐬����
//
void MoveDetector::startDetectingActiveObject()
{
	// �C���v�b�g�ݒ�ɉ�����VideoCapture���擾
	cv::VideoCapture vcap = getVideoCapture();
	if (!vcap.isOpened())
	{
		std::cout << "Can't open video.\n";
		return;
	}

	const int MAX_FRAME_NUM = 3;
	int frameBuffIndex = 0;
	cv::Mat frames[MAX_FRAME_NUM];
	cv::Mat* frame;


	for (;;)
	{
		//3��MAT���Â����ɉ摜����ׂ�B
		// TODO:�������͎����ł��Ă���B(�|�C���^�����ς����clone�v��Ȃ��͂�)

		// �摜����O�ɋl�߂�
		if (!frames[1].empty())
		{
			// 2�ڂ�1�ڂ�
			frames[0] = frames[1].clone();
		}
		if (!frames[2].empty())
		{
			// 3�ڂ�2�ڂ�
			frames[1] = frames[2].clone();
		}

		// �ŐV�̈ʒu�ɉ摜�i�[
		frame = &frames[MAX_FRAME_NUM - 1];
		vcap >> *frame;
		if (!frame->empty()) {
			// frame�擾����

			//�O���[�X�P�[����
			cv::Mat gray;
			cvtColor(*frame, gray, CV_BGR2GRAY);
			//imshow("gray", gray);

			*frame = gray.clone();

			std::vector<cv::Rect> rects = detectMoveObject(frames[0], frames[1], frames[2]);//���̌��m

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
			// frame�擾���s
			std::cout << "End of frame.\n";
			break;
		}
		// ����wait�������ƁA�������ǂ��t���Ȃ��̂��A�����`��͍X�V����Ȃ��B
		// �`��̌��Ԃ����̂ƁA�L�[�������ꂽ���~���邽�߂�wait
		if (cv::waitKey(30) >= 0) break;
	}
}

cv::VideoCapture MoveDetector::getVideoCapture()
{
	cv::VideoCapture vcap;
	switch (commonSetting.inputType) {
	case CommonSetting::EInputType::E_INPUT_TYPE_CAMERA:
		//�J�����f�o�C�X�̃I�[�v��
		std::cout << "���̌��m�C���v�b�g�J����No " << commonSetting.inputCameraDeviceNo << "\n";
		vcap.open(commonSetting.inputCameraDeviceNo);
		break;
	case CommonSetting::EInputType::E_INPUT_TYPE_FILE:
		std::cout << "���̌��m�C���v�b�g�t�@�C�� " << commonSetting.inputFilePath << "\n";
		//����t�@�C���̃I�[�v��
		vcap.open(commonSetting.inputFilePath);
		break;
	default:
		// �ُ�
		std::cout << "���̌��m�C���v�b�g��ʈُ� " << (int)commonSetting.inputType << "\n";
		assert(false);
	}
	return vcap;
}

//�Q�l
//https://kimamani89.com/2019/04/30/post-420/
std::vector<cv::Rect> MoveDetector::detectMoveObject(cv::Mat image1st, cv::Mat image2nd, cv::Mat image3rd)
{
	std::vector<cv::Rect> rects;

	static int frameCnt;
	frameCnt++;
	if (image1st.empty() || image2nd.empty() || image3rd.empty())
	{
		//3���̉摜��������ĂȂ��ꍇ�͉������Ȃ�
		std::cout << "Not ready(wait 3 frames).\n";
		return rects;
	}
	std::cout << "frame[" << frameCnt << "] proc.\n";
	// ��Βl�̋��߂��̂��A�w�i���������߂� 
	cv::Mat diff1, diff2;
	cv::absdiff(image2nd, image1st, diff1);
	//imshow("diff1", diff1);
	cv::absdiff(image3rd, image2nd, diff2);
	//imshow("diff2", diff2);

	cv::Mat im;
	// �w�i��������_���ς̎Z�o
	cv::bitwise_and(diff1, diff2, im);

	//��l��
	cv::Mat img_th;
	cv::threshold(im, img_th, 10, 255, CV_THRESH_BINARY);

	//�c�������E���k�������{���ă}�X�N�摜�𐶐�
	cv::Mat element(moveDetectSetting.maskSize, moveDetectSetting.maskSize, CV_8U, cv::Scalar(1));
	cv::Mat img_dilate;
	cv::dilate(img_th, img_dilate, element, cv::Point(-1, -1), 1);
	cv::Mat mask;
	cv::erode(img_dilate, mask, element, cv::Point(-1, -1), 1);

	// �}�X�N�摜���g���đΏۂ�؂�o��
	cv::Mat img_dst;
	cv::bitwise_and(image3rd, mask, img_dst);

	//�傫�ȍ����̋�`��T��
	rects = GetDiffRect(img_dst);


	imshow("img_dst", img_dst);

	return rects;
}

std::vector<cv::Rect> MoveDetector::GetDiffRect(cv::Mat maskImage)
{
	// ���m���̂̓񎟌��z��
	std::vector<std::vector<cv::Point> > contours;

	// ���̗̈�̈ꎟ���z��
	std::vector<cv::Rect> rects;

	// ���̌��m
	cv::findContours(maskImage, contours, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);
	for (std::vector<cv::Point>& points : contours)
	{
		//���̖̂ʐώ擾
		double areaSize = cv::contourArea(points);
		// ���������͖̂�������
		if (areaSize > moveDetectSetting.moveDetectThresholdSize)
		{
			std::cout << "areaSize = " << areaSize << ".\n";
			std::cout << "point size = " << points.size() << ".\n";

			cv::Rect rect;

			int top, bottom, left, right;
			top = left = INT_MAX;
			bottom = right = INT_MIN;

			// �_�̏W�������`�̍��W���쐬 (�����ƊȒP�ȕ��@������\��)
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

