#include "./trie_timeline.hpp"

ds_viz::pages::trie::SearchTimeline::SearchTimeline(TrieScene &scene,
                                                    const std::string &key)
    : animationTimeline(), scene(scene)
{
    auto curNode = scene.root.get();
    animationTimeline.push_back(std::make_unique<SetVariableAction<TrieNode *>>(
        var_curNode, 
        [&scene]{ return scene.root.get(); }
    ));

    for (const char &c : key) 
    {
        if (curNode->children[c] == nullptr) 
        {
            animationTimeline.push_back(
                std::make_unique<SetVariableAction<std::optional<bool>>>(
                    returnValue,
                    []{ return false; }
                ));
            return;
        }

        animationTimeline.push_back(std::make_unique<SetVariableAction<TrieNode *>>(
            var_curNode, 
            [this, c] { return this->var_curNode->children[c].get(); }
        ));
        curNode = curNode->children[c].get();
    }

    animationTimeline.push_back(
        std::make_unique<SetVariableAction<std::optional<bool>>>(
            returnValue,
            [this] { return var_curNode->isTerminal; }
        ));
    
    currentStepInAnim = animationTimeline.begin();
}

void ds_viz::pages::trie::SearchTimeline::RenderCurrentState(TrieScene &scene)
{
    if (!scene.root)
        return;
    RenderUtil(scene.root.get());
}

void ds_viz::pages::trie::SearchTimeline::RenderUtil(TrieNode *node) 
{
    for (int i = 0; i < TrieNode::ALPHABET_SIZE; ++i) {
        if (!node->children[i])
            continue;

        // Draw connector. To be refactored.
        node->GetPosition().DrawLine(node->children[i]->GetPosition(), 2.0f,
                                                                 nodeDefaultColor);
        RenderUtil(node->children[i].get());

        // Draw caption. To be refactored.
        auto txt = raylib::Text(std::string(1, i), 10, raylib::Color::Black());
        txt.Draw(node->children[i]->GetPosition() -
                         raylib::Vector2{txt.Measure() * 0.5f, 5});
    }

    // Draw node. To be refactored.
    auto nodeCol = node == var_curNode ? (returnValue.has_value() ? nodeReturnColor : nodeHighlightColor) : nodeDefaultColor;
    node->GetPosition().DrawCircle(10.0f, nodeCol);
}

void ds_viz::pages::trie::SearchTimeline::StepForward() 
{
    if (currentStepInAnim == animationTimeline.end()) 
        return;

    (*currentStepInAnim)->Do(*this);
    ++currentStepInAnim;
}

void ds_viz::pages::trie::SearchTimeline::StepBackward()
{
    if (currentStepInAnim == animationTimeline.begin()) 
        return;
    
    --currentStepInAnim;
    (*currentStepInAnim)->Undo(*this);
}

void ds_viz::pages::trie::SearchTimeline::ApplyTimeline(TrieScene &scene) {}
