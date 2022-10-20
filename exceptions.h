#include <exception>
#include <map>

class MyExcept : public std::exception {
public:
    enum class Code { CantParse, CantLoad, CantWrite, CropParams, EDPararms, SharpParams, GSParams, NegativeParams };

    const char *what() const noexcept override {
        return mssgs_.at(code_);
    };

    MyExcept(Code code) : code_(code){};

protected:
    std::map<Code, const char *> mssgs_ = {{Code::CantParse, "wrong input format"},
                                           {Code::CantLoad, "impossible to load file"},
                                           {Code::CantWrite, "impossible to create file"},
                                           {Code::CropParams, "wrong crop filter params size"},
                                           {Code::EDPararms, "wrong edge detection filter params size"},
                                           {Code::SharpParams, "wrong sharpening filter params size"},
                                           {Code::GSParams, "wrong grayscale filter params size"},
                                           {Code::NegativeParams, "wrong negative filter params size"}};
    Code code_;
};