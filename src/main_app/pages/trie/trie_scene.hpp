#pragma once

#include "main_app/pages/trie/trie_timeline.hpp"
#include "raylib-cpp/Color.hpp"
#include "raylib-cpp/Vector2.hpp"
#include "raylib-cpp/raylib-cpp.hpp"
#include "widget_toolkit/interfaces.hpp"
#include <array>
#include <list>
#include <memory>
#include <optional>
#include <string>
#include <utility>

namespace ds_viz::pages::trie
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

        double ComputeWidth() const;

        double ComputeHeight() const;

        raylib::Vector2 GetPosition() const;

        void SetPosition(const raylib::Vector2 &position);

        void RecomputePosition();

        void Add(char character);

        void Add(std::string key);
        void Remove(std::string key);
        bool Search(std::string key);
    };

    class ITimeline;
    
    class TrieScene : public raywtk::IScreenElement
    {
        public:

        private:
        raylib::Color nodeDefaultColor = raylib::Color::White();
        raylib::Color nodeHighlightColor = raylib::Color::Yellow();
        raylib::Color edgeHighlightColor = raylib::Color::Yellow();
        raylib::Color nodeReturnColor = raylib::Color::Green();


        raylib::Camera2D cam;
        std::unique_ptr<ITimeline> animationTimeline = nullptr;
        
        public:
        std::unique_ptr<TrieNode> root = std::make_unique<TrieNode>();
        TrieScene () : cam(raylib::Vector2 {640, 360}, raylib::Vector2 {0, 0}, 0, 2) { }

        void Update(float deltaTime) override;

        static void RenderUtil(TrieNode *node, raylib::Color color);

        void DefaultRender();

        void Render() override;

        void BuildSearchTimeline(const std::string &key);
        void StepForward();
        void StepBackward();
    };
}