#ifndef DC7C50D7_7F76_4634_9A9E_22314AC5FC3D
#define DC7C50D7_7F76_4634_9A9E_22314AC5FC3D

#include <atlas/atlasdefine.hpp>
#include <atlas/visitors/newvisitors/basevisitor.hpp>

namespace Atlas {

    template <typename adouble = double>
    class PositionGenerator {
       public:
        explicit PositionGenerator(std::unique_ptr<Strategy> strategy_) : strategy_(std::move(strategy_)){};

        std::vector<InstrumentVariant<adouble>> generatePositions() { return strategy_->generatePositions(); }

       protected:
        class Strategy {
           public:
            virtual std::vector<InstrumentVariant<adouble>> generatePositions() = 0;
        };

       private:
        std::vector<PositionConfiguration> configs_;
        std::unique_ptr<Strategy> strategy_;
    }
}  // namespace Atlas

#endif /* DC7C50D7_7F76_4634_9A9E_22314AC5FC3D */
