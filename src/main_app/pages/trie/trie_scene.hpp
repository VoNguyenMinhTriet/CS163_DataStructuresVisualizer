#include "raylib-cpp/Color.hpp"
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
            constexpr static int ALPHABET_SIZE = 128; // ASCII size

        private:
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
                double totalWidth = -SPACING_X;
                for (int i = 0; i < ALPHABET_SIZE; ++i) {
                    if (!children[i]) continue;
                    totalWidth += children[i]->ComputeWidth() + SPACING_X;
                }
                return totalWidth >= 0 ? totalWidth : 0;
            }

            double ComputeHeight () const 
            {
                double maxChildHeight = -SPACING_Y;
                for (int i = 0; i < ALPHABET_SIZE; ++i) {
                    if (!children[i]) continue;
                    double childHeight = children[i]->ComputeHeight();
                    if (childHeight > maxChildHeight) maxChildHeight = childHeight;
                }

                auto y = maxChildHeight + SPACING_Y;
                return y >= 0 ? y : 0;
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
                    children[i]->SetPosition(_position + raylib::Vector2{ childRelXPos, SPACING_Y });

                    currentWidth += childWidth + SPACING_X;
                }
            }

            void RecomputePosition () 
            {
                SetPosition(_position);
            }

            void Add (char character) 
            {
                if (character < 0 || character >= ALPHABET_SIZE) return;
                if (!children[character])
                    children[character] = std::make_unique<TrieNode>();
                RecomputePosition();
            }

            void Add (std::string key) 
            {
                TrieNode *curNode = this;
                for (const char& c : key) {
                    curNode->Add(c);
                    curNode = curNode->children[c].get();
                }
                curNode->isTerminal = true;
            }
            void Remove (std::string key) 
            {
                TrieNode *curNode = this;
                for (const char& c : key) {
                    if (!curNode->children[c]) return;
                    curNode = curNode->children[c].get();
                }
                curNode->isTerminal = false;
            }
            bool Search (std::string key) 
            {
                TrieNode *curNode = this;
                for (const char& c : key) {
                    if (!curNode->children[c]) return false;
                    curNode = curNode->children[c].get();
                }
                return curNode->isTerminal;
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

            void RenderUtil (TrieNode *node, raylib::Color defaultColor)
            {
                for (int i = 0; i < TrieNode::ALPHABET_SIZE; ++i) {
                    if (!node->children[i]) continue;
                    node->GetPosition().DrawLine(node->children[i]->GetPosition(), 2.0f, defaultColor);
                    RenderUtil(node->children[i].get(), defaultColor);
                    auto txt = raylib::Text(std::string(1, i), 10, raylib::Color::Black());
                    txt.Draw(node->children[i]->GetPosition() - raylib::Vector2{ txt.Measure() * 0.5f, 5 });
                }

                node->GetPosition().DrawCircle(10.0f, node == _node ? _color : defaultColor);
            }

            void RenderCurrentAction (TrieScene& scene) override 
            {
                RenderUtil(scene.root.get(), scene.nodeDefaultColor);
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

            void RenderUtil (TrieNode *node, raylib::Color defaultColor)
            {
                for (int i = 0; i < TrieNode::ALPHABET_SIZE; ++i) {
                    if (!node->children[i]) continue;
                    node->GetPosition().DrawLine(node->children[i]->GetPosition(), 2.0f, (node->children[i].get() == _nodeTo ? _color : defaultColor));
                    RenderUtil(node->children[i].get(), defaultColor);
                    auto txt = raylib::Text(std::string(1, i), 10, raylib::Color::Black());
                    txt.Draw(node->children[i]->GetPosition() - raylib::Vector2{ txt.Measure() * 0.5f, 5 });
                }

                node->GetPosition().DrawCircle(10.0f, defaultColor);
            }

            void RenderCurrentAction (TrieScene& scene) override 
            {
                RenderUtil(scene.root.get(), scene.nodeDefaultColor);
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
                scene.DefaultRender();
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

        raylib::Color nodeDefaultColor = raylib::Color::White();
        raylib::Color nodeHighlightColor = raylib::Color::Yellow();
        raylib::Color edgeHighlightColor = raylib::Color::Yellow();
        raylib::Color nodeReturnColor = raylib::Color::Green();


        raylib::Camera2D cam;
        std::list<std::unique_ptr<IAction>> animationTimeline;
        std::list<std::unique_ptr<IAction>>::iterator currentStepInAnim;
        
        public:
        std::unique_ptr<TrieNode> root = std::make_unique<TrieNode>();
        TrieScene () : cam(raylib::Vector2 {640, 360}, raylib::Vector2 {0, 0}, 0, 2), currentStepInAnim(animationTimeline.begin()) { }

        void Update (float deltaTime) override {
            cam.SetTarget(raylib::Vector2{ 0, (float)root->ComputeHeight() * 0.5f });
        }
        
        static void RenderUtil (TrieNode *node, raylib::Color color)
        {
            for (int i = 0; i < TrieNode::ALPHABET_SIZE; ++i) {
                if (!node->children[i]) continue;
                node->GetPosition().DrawLine(node->children[i]->GetPosition(), 2.0f, color);
                RenderUtil(node->children[i].get(), color);
                auto txt = raylib::Text(std::string(1, i), 10, raylib::Color::Black());
                txt.Draw(node->children[i]->GetPosition() - raylib::Vector2{ txt.Measure() * 0.5f, 5 });
            }
            node->GetPosition().DrawCircle(10.0f, color);
        }

        void DefaultRender ()
        {
            if (root) RenderUtil(root.get(), nodeDefaultColor);
        }

        void Render () override 
        {
            cam.BeginMode();
            if (currentStepInAnim == animationTimeline.end()) {
                DefaultRender();
                return;
            }

            (*currentStepInAnim)->RenderCurrentAction(*this);
            cam.EndMode();
        }

        void BuildSearchTimeline (std::string key)
        {
            if (key.empty() || !root) return;

            TrieNode *curNode = root.get();
            animationTimeline.clear();
            animationTimeline.push_back(std::make_unique<HighlightNodeAction>(curNode, nodeHighlightColor));
            
            for (const char& c : key) 
            {
                if (curNode->children[c] == nullptr) 
                {
                    animationTimeline.push_back(std::make_unique<NullAction>());
                    break;
                }
                animationTimeline.push_back(std::make_unique<HighlightEdgeAction>(curNode->children[c].get(), edgeHighlightColor));
                curNode = curNode->children[c].get();
                animationTimeline.push_back(std::make_unique<HighlightNodeAction>(curNode, nodeHighlightColor));
                // Those two `push_back` lines are the same shit, just differ in Edge and Node
            }

            animationTimeline.push_back(std::make_unique<HighlightNodeAction>(curNode, nodeReturnColor));

            currentStepInAnim = animationTimeline.begin();
        }

        void StepForward () 
        {
            if (currentStepInAnim == animationTimeline.end()) return;
            (*currentStepInAnim)->Do(*this);
            ++currentStepInAnim;
        }
    };
}