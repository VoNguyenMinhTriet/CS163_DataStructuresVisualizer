#include "raylib-cpp/Vector2.hpp"
#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/interfaces.hpp"
#include <array>
#include <list>
#include <memory>
#include <optional>

namespace ds_viz
{
    class TrieScene : public raywtk::IScreenElement
    {
        struct TrieNode 
        {
        private:
            constexpr static int ALPHABET_SIZE = 128; // ASCII size
            constexpr static int SPACING_X = 32; 
            constexpr static int SPACING_Y = 32;
            using NodePtr = std::unique_ptr<TrieNode>;
            using NodeArr = std::array<NodePtr, ALPHABET_SIZE>;

            raylib::Vector2 _position;
            
        public:
            NodeArr children;
            bool isTerminal = false;

            double ComputeWidth () const 
            {
                double totalWidth = 0.0;
                for (int i = 0; i < ALPHABET_SIZE; ++i) {
                    if (!children[i]) continue;
                    totalWidth += children[i]->ComputeWidth() + SPACING_X;
                }
                return totalWidth;
            }

            raylib::Vector2 GetPosition () const { return _position;}

            void SetPosition (const raylib::Vector2& position) 
            {
                _position = position;

                // Reposition children
                double totalWidth = ComputeWidth();
                double currentWidth = 0.0;
                
                for (int i = 0; i < ALPHABET_SIZE; ++i) {
                    if (!children[i]) continue;
                    
                    double childWidth = children[i]->ComputeWidth();
                    float childRelXPos = -totalWidth / 2 + (currentWidth + childWidth / 2);
                    children[i]->SetPosition(_position + raylib::Vector2{ childRelXPos, 0 } + raylib::Vector2{ SPACING_X, SPACING_Y });

                    currentWidth += childWidth + SPACING_X;
                }
            }
        };

        class IAction 
        { 
        public:
            virtual ~IAction () = default;
            virtual void RenderCurrentAction (TrieScene& scene) = 0;
            virtual void Do (TrieScene& scene) = 0;
            virtual void Undo (TrieScene& scene) = 0;
        };

        class HighlightNodeAction : public IAction 
        {
            TrieNode *_node;
            raylib::Color _color;
            std::unique_ptr<IAction> _previousAction;
        public:
            HighlightNodeAction (TrieNode *node, raylib::Color color)
                : _node(node), _color(color) {}

            void RenderCurrentAction (TrieScene& scene) override 
            {                
            }

            void Do (TrieScene& scene) override 
            {
            }

            void Undo (TrieScene& scene) override 
            {
            }
        };

        class HighlightEdgeAction : public IAction 
        {
            TrieNode* _nodeTo;
            raylib::Color _color;
            std::unique_ptr<IAction> _previousAction;
        public:
            HighlightEdgeAction (TrieNode *nodeTo, raylib::Color color)
            : _nodeTo(nodeTo), _color(color) {}

            void RenderCurrentAction (TrieScene& scene) override 
            {                
            }

            void Do (TrieScene& scene) override 
            {
            }

            void Undo (TrieScene& scene) override 
            {
            }
        };
        
        class AddNodeAction : public IAction 
        {
            TrieNode *_nodeToAddTo;
            char _character;
            std::unique_ptr<IAction> _previousAction;

            void RenderCurrentAction (TrieScene& scene) override 
            {                
            }

            void Do (TrieScene& scene) override 
            {
            }

            void Undo (TrieScene& scene) override 
            {
            }
        };

        class NullAction : public IAction 
        {
        public:
            void RenderCurrentAction (TrieScene& scene) override 
            {
                // No action to render
            }

            void Do (TrieScene& scene) override 
            {
                // No action to perform
            }

            void Undo (TrieScene& scene) override 
            {
                // No action to undo
            }
        };


        raylib::Camera2D cam;
        std::list<std::unique_ptr<IAction>> actions;
        std::list<std::unique_ptr<IAction>>::iterator currentAction;
        std::unique_ptr<TrieNode> root;

    public:
        void Update (float deltaTime) override;
        void Render () override 
        {
            (*currentAction)->RenderCurrentAction(*this);
        }

        void BuildSearchTimeline (std::string key)
        {
            TrieNode *curNode = root.get();
            actions.clear();
            actions.push_back(std::make_unique<HighlightNodeAction>(curNode, raylib::Color::Red()));
            
            for (const char& c : key) 
            {
                if (curNode->children[c] == nullptr) 
                {
                    actions.push_back(std::make_unique<NullAction>());
                    break;
                }
                actions.push_back(std::make_unique<HighlightEdgeAction>(curNode->children[c].get(), raylib::Color::Green()));
                curNode = curNode->children[c].get();
                actions.push_back(std::make_unique<HighlightNodeAction>(curNode, raylib::Color::Red()));
            }

            currentAction = actions.begin();
        }
    };
}