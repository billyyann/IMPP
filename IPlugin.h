#ifndef _IPLUGIN_INTERFACE_H_
#define _IPLUGIN_INTERFACE_H_
#include <opencv2/core.hpp>
#include <string>
#include <map>


class BaseImagePluginInterface
{
public:
    BaseImagePluginInterface() = default;
    virtual ~BaseImagePluginInterface() {};
    virtual bool process(const cv::Mat& src1, cv::Mat& src2, cv::Mat dst, std::map<std::string, double>& params, std::map<std::string, std::string>& context) = 0;
    //#ifdef CUDA
    //	virtual bool process_cuda(uint16_t* src, uint16_t* dst, std::map<std::string, double>& context) = 0;
    //#endif // CUDA
};

class AddOp :public BaseImagePluginInterface {
public:
    AddOp() {};
    ~AddOp() {};
    bool process(const cv::Mat& src1, cv::Mat& src2, cv::Mat dst, std::map<std::string, double>& params, std::map<std::string, std::string>& context) override {

            double ratio = 0.5;
            if (params.count("ratio")) {
                ratio = params["ratio"];
            }
            dst = ratio * src1 + (1 - ratio) * src2;
            return true;

    };
};

class MinusOp :public BaseImagePluginInterface {
public:
    MinusOp() {};
    ~MinusOp() {};
    bool process(const cv::Mat& src1, cv::Mat& src2, cv::Mat dst, std::map<std::string, double>& params, std::map<std::string, std::string>& context) override {
        double ratio = 1;
        if (params.count("ratio")) {
            ratio = params["ratio"];
        }

        dst = src1 - ratio * src2;
        return true;
    };
};

class MultiplyOp :public BaseImagePluginInterface {
public:
    MultiplyOp() {};
    ~MultiplyOp() {};
    bool process(const cv::Mat& src1, cv::Mat& src2, cv::Mat dst, std::map<std::string, double>& params, std::map<std::string, std::string>& context) override {
        if (!context.count("OPName")) return false;

        if (context["OPName"] == "MultiplyOp") {
            cv::Mat A = cv::Mat(src1.rows, src1.cols, CV_64FC1);
            cv::Mat B = cv::Mat(src2.rows, src2.cols, CV_64FC1);

            cv::convertScaleAbs(src1, A, 1.0f, 0.0f);
            cv::convertScaleAbs(src2, B, 1.0f, 0.0f);

            cv::Mat AB = A.mul(B);
            dst = cv::Mat(src1.rows, src1.cols, CV_16U);
            cv::convertScaleAbs(AB, dst, 1.0f, 0.0f);
            return true;
        }

        return false;
    };
};

#endif