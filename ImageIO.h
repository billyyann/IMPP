#pragma once
#include <string>
#include <opencv2/core.hpp>
#include <vector>
#include <map>
#include "common_define.h"


class BaseImageIO
{
public:
	BaseImageIO() {};
	virtual	~BaseImageIO() {};
	virtual bool loadUNC(std::string fn, cv::Mat& unc)=0;
	virtual bool loadBlindTbl(std::string fn, cv::Mat& unc)=0;
	virtual bool load1LDat(std::string fn)=0;
	virtual std::vector<uint32_t> getFrameNums()=0;
	virtual uint32_t getImageViaFrame(uint32_t relativeFrameNum, cv::Mat&)=0;
	virtual uint32_t getHeaderViaFrame(uint32_t relativeFrameNum, ImageHeader&)=0;
};

class RawImageIO:public BaseImageIO
{
public:
	RawImageIO() {
		m_frameCnt = 0;
		m_width = 0;
		m_height = 0;
		m_firstFrmNum = -1;
	};
	~RawImageIO() {} ;
	bool load1LDat(std::string fn) override;
	bool loadUNC(std::string fn, cv::Mat& unc) override;
	bool loadBlindTbl(std::string fn, cv::Mat& bldM) override;
	std::vector<uint32_t> getFrameNums() override;
	uint32_t getImageViaFrame(uint32_t relativeFrameNum, cv::Mat&) override;
	uint32_t getHeaderViaFrame(uint32_t relativeFrameNum, ImageHeader&) override;
	std::vector<cv::Mat>& getAllImage();
	
private:
	uint32_t m_frameCnt;
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_firstFrmNum;
	//std::map<uint32_t,cv::Mat> m_images;
	std::vector<cv::Mat> m_images;
	//cv::Mat m_UNC;
	//cv::Mat m_blindTbl;
	std::map<uint32_t, ImageHeader> m_headers;

};

