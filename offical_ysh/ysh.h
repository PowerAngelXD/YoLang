#include "../yvm/vmfront.h"

namespace ysh_offical {
    class YolangShellInsSet;

    class YolangShellOffical : public yvm::yshBase {
    public:
        YolangShellOffical();

        YolangShellInsSet* ysi;
        std::string shellVersion = "v1.0.0";

        void yshStart() override;
        void yshUpdate() override;
        void yshEnd() override;

        void catchYoError(yoexception::YoError ye) override;
    };

    class YolangShellInsSet : public yvm::InsSet {
    public:
        YolangShellInsSet()=default;

        void repl(yvm::ins_method_para para, YolangShellOffical* yso);
        void help(yvm::ins_method_para para, YolangShellOffical* yso);
        void info(yvm::ins_method_para para, YolangShellOffical* yso);
        void exit(yvm::ins_method_para para, YolangShellOffical* yso);
        void run(yvm::ins_method_para para, YolangShellOffical* yso);
        void view(yvm::ins_method_para para, YolangShellOffical* yso);
    };
}
