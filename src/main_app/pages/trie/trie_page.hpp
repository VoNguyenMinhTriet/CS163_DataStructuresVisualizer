#include "../page.hpp"

namespace ds_viz::pages
{
    class TriePage : public Page
    {
    public:
        void Update(float deltaTime) override;
        void Render() override;
    };
}
