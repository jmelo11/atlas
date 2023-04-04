#ifndef A3B1FF9B_DE72_47E2_9FB1_698962E11728
#define A3B1FF9B_DE72_47E2_9FB1_698962E11728

namespace Atlas {
    template <typename adouble>
    class Stage {
       public:
        Stage(const std::string& name, const std::function<adouble(adouble)>& func) : name_(name), func_(func){};

        void run(adouble x) const { return func_(x); };

        const std::string& name() const { return name_; };

       private:
        std::string name_;
        std::function<adouble(adouble)> func_;
    };

    template <typename adouble>
    class Pipeline {
       public:
        Pipeline() = default;

        void addStage(const Stage<adouble>& stage) { stages_.push_back(stage); };

        adouble run(adouble x) const {
            for (const auto& stage : stages_) { x = stage.run(x); }
            return x;
        };

       private:
        std::vector<Stage<adouble>> stages_;
    };
}  // namespace Atlas

#endif /* A3B1FF9B_DE72_47E2_9FB1_698962E11728 */
