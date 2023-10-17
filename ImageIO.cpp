#include "ImageIO.h"
#include "common_define.h"
#include <chrono>
#include <iostream>

bool RawImageIO::load1LDat(std::string fn)
{
	auto start = std::chrono::high_resolution_clock::now();
    FILE* fp = fopen(fn.c_str(), "rb");
	if (fp == NULL)
	{
		printf("open file %s error\n", fn.c_str());
		return false;
	};
	fread(&m_frameCnt, sizeof(m_frameCnt), 1, fp);
	fseek(fp, 8 * m_frameCnt, SEEK_CUR);
	
	uint16_t* data= nullptr;
	for (int i = 0;i < m_frameCnt;i++) {
		ImageHeader imageHeader;
		fread(&imageHeader, sizeof(ImageHeader), 1, fp);
		m_headers.insert({ imageHeader.frameHeader._FrameNum,imageHeader });
		data = new uint16_t[imageHeader.frameHeader._Cols * imageHeader.frameHeader._Lines];
		fread(data, sizeof(uint16_t), imageHeader.frameHeader._Cols * imageHeader.frameHeader._Lines, fp);
		if (i == 0) {
			m_firstFrmNum = imageHeader.frameHeader._FrameNum;
		}
		if (data) {
			m_images.push_back(cv::Mat(imageHeader.frameHeader._Lines, imageHeader.frameHeader._Cols, CV_16U, data));
			//m_images.insert({ imageHeader.frameHeader._FrameNum,cv::Mat(imageHeader.frameHeader._Lines, imageHeader.frameHeader._Cols, CV_16U, data) });
		}
	
	}

	auto end = std::chrono::high_resolution_clock::now();
	__int64 duration = (end - start).count();
	std::cout << "datÍ¼Ïñ¼ÓÔØÊ±¼ä£º" << duration / 1000000000.0 << "s"
		<< "£»  " << duration / 1000000.0 << "ms"
		<< "£»  " << duration / 1000.0 << "us"
		<< std::endl;
	
    return true;
}

bool RawImageIO::loadUNC(std::string fn,cv::Mat& uncM)
{
	CommNonUniformCheckPara uncHead;
	FILE* fpo = NULL;
	double* unc;
	fpo = fopen(fn.c_str(), "rb");
	fread(&uncHead, sizeof(uncHead), 1, fpo);
	unc = new double[uncHead.width* uncHead.height * 3];
	fread(unc, sizeof(double), uncHead.width * uncHead.height * 3, fpo);

	uncM =cv::Mat(uncHead.height, uncHead.width, CV_64FC3, unc);


	fclose(fpo);

	return true;
}

bool RawImageIO::loadBlindTbl(std::string fn, cv::Mat& bldM)
{
	CommBlindProc head;
	FILE* fp = fopen(fn.c_str(), "rb");
	fread((char*)(&head), sizeof(head), 1, fp);
	uint16_t* blindTbl = new uint16_t[head.width* head.height];
	fread((char*)blindTbl, sizeof(uint16_t), head.width * head.height, fp);
	bldM = cv::Mat(head.height, head.width, CV_16U, blindTbl);
	fclose(fp);
	return false;
}

std::vector<uint32_t> RawImageIO::getFrameNums()
{
	std::vector<uint32_t> res;
	for (auto& entry : m_headers) {
		res.push_back(entry.first);
	};
	return res;
}

uint32_t RawImageIO::getImageViaFrame(uint32_t relativeFrameNum, cv::Mat& img)
{
	if (relativeFrameNum - m_firstFrmNum >= 0 && relativeFrameNum - m_firstFrmNum<10) {
		img= m_images[relativeFrameNum - m_firstFrmNum];
		return relativeFrameNum;
	}

	return -1;
}

uint32_t RawImageIO::getHeaderViaFrame(uint32_t relativeFrameNum, ImageHeader& header)
{
	if (m_headers.count(relativeFrameNum)) {
		header = m_headers[relativeFrameNum];
		return relativeFrameNum;
	}

	return -1;;
}

std::vector<cv::Mat>& RawImageIO::getAllImage()
{
	return m_images;
}
