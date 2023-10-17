#include "GenericImageService.h"
#include <chrono>
#include <vector>
#include <fstream>
#include "RcConfig.h"

//#include "utils.h"

GenericImageService::GenericImageService() {
	genericImageRepository.reset(new GenericImageRepository);
	taskRepository.reset(new TaskRepository);
	processPatternRepository.reset(new ProcessPatternRepository);
	processRecordRepository.reset(new ProcessRecordRepository);
};

std::shared_ptr<GenericImageService> GenericImageService::dpt{new (std::nothrow) GenericImageService()};

std::shared_ptr<GenericImageService>& GenericImageService::GetInstance()
{
	return dpt;
}

bool GenericImageService::load(std::string fn, IMAGELEVEL, RawImageIO& rio)
{
	return rio.load1LDat(fn);
}

bool GenericImageService::loadImage(std::string fn, IMAGELEVEL lvl)
{
	RawImageIO rio;
	jws::RcConfig rcconfig;
	std::ifstream  src(fn, std::ios::binary);
	std::string localDatRootPath = rcconfig.ReadString("DAT", "LOCALPATH", "C:/Users/86138/Desktop/zdg_calibratio/FSDB_BC_JWS_20230706/FSDB_BC_JWS_20230706/data");
	std::string localDatPath = localDatRootPath + '/' + UTILS::STR::split(fn, '/').back();
	std::ofstream  dst(localDatPath, std::ios::binary);

	dst << src.rdbuf();
			//todo 入库
	if (rio.load1LDat(localDatPath)) {
		auto  frms=rio.getFrameNums();
		GenericImage g;
		ImageHeader firstheader,lastheader;

		if (!frms.size() > 0) return false;
		rio.getHeaderViaFrame(frms[0], firstheader);
		rio.getHeaderViaFrame(frms[frms.size()-1], lastheader);
		g.taskId = 0;
		g.satId = firstheader.dataMeta._SatID;
		g.payloadId = firstheader.dataMeta._SensorID;
		g.waveBandId = firstheader.dataMeta._BandID;
		g.channelId = firstheader.dataMeta._ChannelID;
		g.integralTime = firstheader.frameHeader._IT;
		strcpy(g.startTimeStamp , firstheader.frameHeader._ImageTime.toString().c_str());
		g.startFrameCount = firstheader.frameHeader._FrameNum;
		g.stareWorkMode = 0;
		strcpy(g.endTimeStamp , lastheader.frameHeader._ImageTime.toString().c_str());
		g.endFrameCount = lastheader.frameHeader._FrameNum;

		auto now = std::chrono::system_clock::now();
		strcpy(g.entryTime, UTILS::TIME::cur_dt().toString().c_str());
		strcpy(g.gentime, UTILS::TIME::cur_dt().toString().c_str());

		//todo
		//std::string::replace(
		//	begin,
		//	end,
		//	new_substring
		//);
		//g.remoteFilePath = fn.replace()
		strcpy(g.remoteFilePath, fn.c_str());
		strcpy(g.localFilePath, localDatPath.c_str());
		g.lifeStatus = 0; //0 extracted  1 deleted 
		genericImageRepository->addOne(g);
	};
	return true;
}


bool GenericImageService::process(std::string id, std::string dat,
	std::map<std::string, std::tuple<BaseImagePluginInterface*, std::map<std::string, double>, std::map<std::string, std::string>>> algoDict)
{
	/*
		std::string step1=_initializedSubTask(t, "UNCModule", "");
		std::string step2 = _initializedSubTask(t, "BLDModule", step1);
		AutoBLDModule
		std::string step3 = _initializedSubTask(t, "BackgroundModule", step2);
		std::string step5 = _initializedSubTask(t, "MergePixelModule", step4);
		std::string step6 = _initializedSubTask(t, "MultiFrameModule", step5);
		std::string step7 = _initializedSubTask(t, "DeNoiseModule", step6);
	*/
	std::tuple<BaseImagePluginInterface*, std::map<std::string, double>, std::map<std::string, std::string>>  t;
	
	cv::Mat src2, dst;
	std::map<std::string, double> params;
	std::map<std::string, std::string> context;
	RawImageIO rio;

	rio.load1LDat(dat); //30ms

	//更新img缓存
	const std::vector<cv::Mat>& src1 = rio.getAllImage();
	std::vector<cv::Mat> res;

	 t = algoDict["UNCModule"];
	 if (context.count("hasUNC") && context["hasUNC"] == "True") {
		 BaseImagePluginInterface* uncP = std::get<0>(t);
		 params = std::get<1>(t);
		 context = std::get<2>(t);

		 if (context.count("UNCFn")) {
			 rio.loadUNC(context["UNCFn"], src2);
		 }//5ms
		 else {
			 std::cerr << "[UNCModule] missing UNCFn param" << std::endl;return false;
		 }

		 for (int i = 0;i < 10;i++) {
			 uncP->process(src1[i], src2, dst, params, context);
			 res.push_back(src2);
		 }
		 delete uncP;uncP = nullptr;src2.release();dst.release();
	 }

	 t = algoDict["BLDModule"];
	 if (context.count("hasBTL") && context["hasBTL"] == "True") {
		 BaseImagePluginInterface* btlP = std::get<0>(t);
		 params = std::get<1>(t);
		 context = std::get<2>(t);

		 if (context.count("BTLFn")) {
			 rio.loadBlindTbl(context["BTLFn"], src2);
		 } //5ms
		 {
			 std::cerr << "[BLDModule] missing BTLFn param" << std::endl;return false;
		 }

		 for (int i = 0;i < 10;i++) {
			 btlP->process(res[i], src2, dst, params, context);
			 res[i] = dst;
		 }
		 
		 delete btlP;btlP = nullptr;src2.release();dst.release();
	 }

	 t = algoDict["AutoBLDModule"];
	 if (context.count("hasAutoBTL") && context["hasAutoBTL"] == "True") {
		 BaseImagePluginInterface* abtlP = std::get<0>(t);
		 params = std::get<1>(t);
		 context = std::get<2>(t);

		 for (int i = 0;i < 10;i++) {
			 abtlP->process(res[i], src2, dst, params, context);
			 res[i] = dst;
		 }
		 delete abtlP;abtlP = nullptr;src2.release();dst.release();
	 }

	 t = algoDict["BackgroundModule"];
	 if (context.count("hasBGEliminate") && context["hasBGEliminate"] == "True") {
		 BaseImagePluginInterface* bgP = std::get<0>(t);
		 params = std::get<1>(t);
		 context = std::get<2>(t);

		 if (context.count("BGEliminatePattern") && context["BGEliminatePattern"] == "FIXED") {
			 if (context.count("BGFn")) {
				 RawImageIO bgrio;
				 bgrio.load1LDat(context["BGFn"]);
				 const std::vector<cv::Mat>& bg = bgrio.getAllImage();;

				 src2 = (bg[0] + bg[1] + bg[2] + bg[3] + bg[4] + bg[5] + bg[6] + bg[7] + bg[8] + bg[9]) / 10;
			 }//5ms
		 }else if (context.count("BGEliminatePattern") && context["BGEliminatePattern"] == "AUTO" ) {
			 int bgStartFrmNum=-1,bgEndFrmNum = -1;
			 if (params.count("BGFrmMergeInterval")) {
				 bgEndFrmNum = m_imgFrmNum - params["BGFrmMergeInterval"];
			 }
			 else {
				 std::cerr << "[BackgroundModule] missing BGFrmMergeInterval param" << std::endl;return false;
			 }

			 if (params.count("BGFrmMergeCnt")) {
				 bgStartFrmNum = bgEndFrmNum - params["BGFrmMergeCnt"];
			 }
			 else {
				 std::cerr << "[BackgroundModule] missing BGFrmMergeCnt param" << std::endl;return false;
			 }

			 RawImageIO bgrio;
			 for (int i = bgStartFrmNum;i < bgEndFrmNum + 1;i++) {
				 if (!m_bgMap.count(i) )continue;
				 if (std::find(m_lastLoadBg.begin(), m_lastLoadBg.end(), m_bgMap[i]) != m_lastLoadBg.end()) continue;
				 
				 if (m_bgCache.size() == 0 && m_bgMap.count(i - 10)) {
						 bgrio.load1LDat(m_bgMap[i - 10]); //30ms
						 const std::vector<cv::Mat>& bgs = rio.getAllImage();
						 m_bgCache.insert(m_bgCache.begin(), bgs.begin(), bgs.end());
				 }
				 
				 bgrio.load1LDat(m_bgMap[i]); //30ms
				 const std::vector<cv::Mat>& bgs = rio.getAllImage();
				 m_bgCache.insert(m_bgCache.begin(), bgs.begin(), bgs.end());

			 }
			 int cnt = 0;
			 int offset =10-( bgrio.getFrameNums()[9] - bgEndFrmNum);
			 for (auto it = m_bgCache.begin();it != m_bgCache.end();it++) {
				 offset--;
				 if (offset == 0) {
					 src2 = *it;
					 cnt = 1;
				 }
				 else if(offset <0)
				 {
					 src2 += *it;
					 cnt++;
				 }
			 }
			 src2 = src2 / cnt;//40ms
		 }else {
			 std::cerr << "[BackgroundModule] wrong BackgroundModule param" << std::endl;return false;
		 }

		 for (int i = 0;i < 10;i++) {
			 dst = res[i] - src2;
			 res[i] = dst;
		 }
		 delete bgP;bgP = nullptr;src2.release();dst.release();
	 }

	t = algoDict["MergePixelModule"];
	if (context.count("hasPixelMerge") && context["hasPixelMerge"] == "True") {
		BaseImagePluginInterface* mpP = std::get<0>(t);
		params = std::get<1>(t);
		context = std::get<2>(t);

		for (int i = 0;i < 10;i++) {
			mpP->process(res[i], src2, dst, params, context);
		}
		delete mpP;mpP = nullptr;dst.release();
	}

	//将非均匀，盲元，减背景、自动减背景、像元合并图像放入缓存
	if (src1.size() > 0) {
		m_imgCache.insert(m_imgCache.begin(), res.begin(), res.end());
		m_imgFrmNum = rio.getFrameNums()[9];
		while (m_imgCache.size() > 40) {
			m_imgCache.pop_front();
		}
		m_bgMap.insert({ rio.getFrameNums()[0] ,dat });
	}

	t = algoDict["MultiFrameModule"];
	if (context.count("hasMultiFrmMerge") && context["hasMultiFrmMerge"] == "True") {
		BaseImagePluginInterface* mfP = std::get<0>(t);
		params = std::get<1>(t);
		context = std::get<2>(t);

		//for (int i = 0;i < 10;i++) {
		//	mfP->process(res[i], src2, dst, params, context);
		//}
		cv::Mat sumUp;
		int cnt = 0;
		auto header = m_imgCache.begin();
		for (auto iter = m_imgCache.begin();iter != m_imgCache.end();iter++) {
			if (cnt == 0) sumUp = *iter/30;
			if (cnt < 30) sumUp += *iter / 30;
			if (cnt >= 30) {
				sumUp = sumUp + *iter / 30 - *header / 30;
				header++;
				res[cnt - 30] = sumUp;
			}
		}
		delete mfP;mfP = nullptr;dst.release();
	}

	if (context.count("hasMaxProj") && context["hasMaxProj"] == "True") {
		BaseImagePluginInterface* maxPP = std::get<0>(t);
		params = std::get<1>(t);
		context = std::get<2>(t);


		cv::Mat maxM;
		int cnt = 0;
		auto header = m_imgCache.begin();
		for (auto iter = m_imgCache.begin();iter != m_imgCache.end();iter++) {
			if (cnt == 0) maxM = *iter ;
			if (cnt > 30) maxM =cv::max(maxM,*iter);
			//todo optimize
			//if (cnt >= 30) {
			//	sumUp = sumUp + *iter / 30 - *header / 30;
			//	header++;
			//	res[cnt - 30] = sumUp;
			//}
			if (cnt >=0) {
				res[cnt - 30] = maxM;
			}
		}
		delete maxPP;maxPP = nullptr;dst.release();
	}

	t = algoDict["DeNoiseModule"];
	if (context.count("hasImageFilter") && context["hasImageFilter"] == "True") {
		BaseImagePluginInterface* filterP = std::get<0>(t);
		params = std::get<1>(t);
		context = std::get<2>(t);
		for (int i = 0;i < 10;i++) {
			filterP->process(res[i], src2, dst, params, context);
		}
	}
	return true;
}


bool GenericImageService::extractDatInfo()
{
	return false;
}

std::vector<uint32_t> GenericImageService::getFrameNums(BaseImageIO& rio)
{
	return rio.getFrameNums();
}

