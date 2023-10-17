#include "repository.h"
#include <memory>
#include <future>
#include "utils.h"
#include "ImageIO.h"
#include "IPlugin.h"
#include <list>
/**
 * @brief ����ͼ������
 * ͼ�����������
 * ͼ�����
 * �ļ�ͷ����
 * ͼ�����ݿ�crud
 * ͼ��洢
 * ͼ����
 */
#pragma once
enum IMAGELEVEL
{
	ONE,
	TWO
};

class GenericImageService
{
public:
	static std::shared_ptr<GenericImageService>& GetInstance();
	bool load(std::string fn, IMAGELEVEL, RawImageIO& rio);
	bool loadImage(std::string fn, IMAGELEVEL);
	bool process(std::string id,std::string fn,
		std::map<std::string, std::tuple<BaseImagePluginInterface*, std::map<std::string, double>, std::map<std::string, std::string>>> algoDict);
	bool extractDatInfo();
	std::vector<uint32_t> getFrameNums(BaseImageIO& rio);

private:
	GenericImageService();
	std::list<cv::Mat> m_imgCache;//30 
	std::list<cv::Mat> m_bgCache;//50
	std::list<std::string> m_lastLoadBg;//50
	std::map<uint32_t,std::string> m_bgMap; 
	uint32_t m_imgFrmNum=-1;
	static std::shared_ptr<GenericImageService> dpt;
	std::shared_ptr<GenericImageRepository> genericImageRepository{nullptr};
	std::shared_ptr < TaskRepository> taskRepository{nullptr};
	std::shared_ptr < ProcessPatternRepository> processPatternRepository{nullptr};
	std::shared_ptr < ProcessRecordRepository> processRecordRepository{nullptr};
};

