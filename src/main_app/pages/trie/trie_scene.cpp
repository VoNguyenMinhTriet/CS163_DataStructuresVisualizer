#include "./trie_scene.hpp"
#include "./trie_timeline.hpp"
#include "raylib-cpp/Vector2.hpp"
#include <cmath>

void ds_viz::pages::trie::TrieScene::Render() {
    cam.BeginMode();

    if (animationTimeline == nullptr) 
        DefaultRender();
    else
        animationTimeline->RenderCurrentState(*this);

    cam.EndMode();
}

void ds_viz::pages::trie::TrieScene::BuildSearchTimeline(
        const std::string &key) {
    animationTimeline = std::make_unique<SearchTimeline>(*this, key);
}

double ds_viz::pages::trie::TrieNode::ComputeWidth() const {
    double totalWidth = -SPACING_X;
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (!children[i])
            continue;
        totalWidth += children[i]->ComputeWidth() + SPACING_X;
    }
    return totalWidth >= 0 ? totalWidth : 0;
}

double ds_viz::pages::trie::TrieNode::ComputeHeight() const {
    double maxChildHeight = -SPACING_Y;
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (!children[i])
            continue;
        double childHeight = children[i]->ComputeHeight();
        if (childHeight > maxChildHeight)
            maxChildHeight = childHeight;
    }

    auto y = maxChildHeight + SPACING_Y;
    return y >= 0 ? y : 0;
}

raylib::Vector2 ds_viz::pages::trie::TrieNode::GetPosition() const {
    return _position;
}

void ds_viz::pages::trie::TrieNode::SetPosition(
    const raylib::Vector2 &position) {
    _position = position;

    // Reposition children
    double totalWidth = ComputeWidth();
    double currentWidth = 0.0;

    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (!children[i])
            continue;

        double childWidth = children[i]->ComputeWidth();
        float childRelXPos = -totalWidth / 2 + (currentWidth + childWidth / 2);
        children[i]->SetPosition(_position +
                                                         raylib::Vector2{childRelXPos, SPACING_Y});

        currentWidth += childWidth + SPACING_X;
    }
}

void ds_viz::pages::trie::TrieNode::RecomputePosition() {
    SetPosition(_position);
}

void ds_viz::pages::trie::TrieNode::Add(char character) {
    if (character < 0 || character >= ALPHABET_SIZE)
        return;
    if (!children[character])
        children[character] = std::make_unique<TrieNode>();
    RecomputePosition();
}

void ds_viz::pages::trie::TrieNode::Add(std::string key) {
    TrieNode *curNode = this;
    for (const char &c : key) {
        curNode->Add(c);
        curNode = curNode->children[c].get();
    }
    curNode->isTerminal = true;
    RecomputePosition();
}

void ds_viz::pages::trie::TrieNode::Remove(std::string key) {
    TrieNode *curNode = this;
    for (const char &c : key) {
        if (!curNode->children[c])
            return;
        curNode = curNode->children[c].get();
    }
    curNode->isTerminal = false;
}

bool ds_viz::pages::trie::TrieNode::Search(std::string key) {
    TrieNode *curNode = this;
    for (const char &c : key) {
        if (!curNode->children[c])
            return false;
        curNode = curNode->children[c].get();
    }
    return curNode->isTerminal;
}

void ds_viz::pages::trie::TrieScene::RenderUtil(TrieNode *node,
    raylib::Color color) {
    for (int i = 0; i < TrieNode::ALPHABET_SIZE; ++i) {
        if (!node->children[i])
            continue;

        node->GetPosition().DrawLine(node->children[i]->GetPosition(), 2.0f, color);

        RenderUtil(node->children[i].get(), color);

        auto txt = raylib::Text(std::string(1, i), 10, raylib::Color::Black());
        txt.Draw(node->children[i]->GetPosition() -
                         raylib::Vector2{txt.Measure() * 0.5f, 5});
    }

    node->GetPosition().DrawCircle(10.0f, color);
}

void ds_viz::pages::trie::TrieScene::DefaultRender() {
    if (root)
        RenderUtil(root.get(), nodeDefaultColor);
}

void ds_viz::pages::trie::TrieScene::Update(float deltaTime) {
    // Camera smoothing
    float timeConstant = 10.0f;
    
    raylib::Vector2 target {0, (float)root->ComputeHeight() * 0.5f};
    raylib::Vector2 toTarget = target - cam.GetTarget();
    raylib::Vector2 deltaTarget =
        toTarget * (1.0f - std::exp(-timeConstant * deltaTime));

    cam.SetTarget((raylib::Vector2)cam.GetTarget() + deltaTarget);
}

void ds_viz::pages::trie::TrieScene::StepForward() {
    animationTimeline->StepForward();
}

void ds_viz::pages::trie::TrieScene::StepBackward() {
    animationTimeline->StepBackward();
}
std::string ds_viz::pages::trie::TrieScene::GetStatusMessage() const {
  return animationTimeline ? animationTimeline->GetStatusMessage()
                           : "No animation is currently on display.";
}
void ds_viz::pages::trie::TrieScene::BuildAddTimeline(const std::string& key)
{
    animationTimeline = std::make_unique<AddTimeline>(*this, key);
}
void ds_viz::pages::trie::TrieScene::BuildRemoveTimeline(const std::string& key)
{
    animationTimeline = std::make_unique<RemoveTimeline>(*this, key);
}
