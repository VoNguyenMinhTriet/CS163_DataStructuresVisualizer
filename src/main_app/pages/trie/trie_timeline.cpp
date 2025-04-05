#include "./trie_timeline.hpp"

#include <optional>
#include <string>

ds_viz::pages::trie::SearchTimeline::SearchTimeline(TrieScene& scene,
                                                    const std::string& key) :
    stepTimeline(), scene(scene), _key(key)
{
    auto curNode = scene.root.get();
    stepTimeline.push_back(std::make_unique<SetVariableStep<TrieNode*>>(
        var_curNode, "curNode", [&scene] { return scene.root.get(); }, "root"));

    for (const char& c : key) {
        stepTimeline.push_back(CheckChildStep(c));
        if (curNode->children[c] == nullptr) {
            stepTimeline.push_back(
                std::make_unique<SetVariableStep<std::optional<bool>>>(
                    returnValue, "<return value>", [] { return false; }));

            currentStep = stepTimeline.begin();
            return;
        }

        stepTimeline.push_back(
            std::make_unique<GoToChildStep>(var_curNode, "curNode", c));
        curNode = curNode->children[c].get();
    }

    stepTimeline.push_back(CheckTerminalStep());
    stepTimeline.push_back(
        std::make_unique<SetVariableStep<std::optional<bool>>>(
            returnValue, "<return value>",
            [this] { return var_curNode->isTerminal; }));

    currentStep = stepTimeline.begin();
}

void ds_viz::pages::trie::SearchTimeline::RenderCurrentState(TrieScene& scene)
{
    if (!scene.root)
        return;
    RenderUtil(scene.root.get());
}

void ds_viz::pages::trie::SearchTimeline::RenderUtil(TrieNode* node)
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
    auto nodeCol =
        node == var_curNode
            ? (returnValue.has_value()
                   ? (returnValue.value() ? nodeReturnColor : nodeUnavailColor)
                   : nodeHighlightColor)
            : nodeDefaultColor;
    node->GetPosition().DrawCircle(10.0f, nodeCol);
}

void ds_viz::pages::trie::SearchTimeline::StepForward()
{
    if (currentStep == stepTimeline.end())
        return;

    (*currentStep)->Do(*this);
    ++currentStep;
}

void ds_viz::pages::trie::SearchTimeline::StepBackward()
{
    if (currentStep == stepTimeline.begin())
        return;

    --currentStep;
    (*currentStep)->Undo(*this);
}

void ds_viz::pages::trie::SearchTimeline::ApplyTimeline(TrieScene& scene) {}

void ds_viz::pages::trie::SearchTimeline::GoToChildStep::Do(
    SearchTimeline& timeline)
{
    _pPreviousNode = _nodeVar;
    _nodeVar = _nodeVar->children[childChar].get();
}

void ds_viz::pages::trie::SearchTimeline::GoToChildStep::Undo(
    SearchTimeline& timeline)
{
    _nodeVar = _pPreviousNode;
}

std::string ds_viz::pages::trie::SearchTimeline::GoToChildStep::GetMessage()
    const
{
    return "Moving variable " + varName + " to child node of index '" +
           std::string(1, childChar) + "'";
}

std::string ds_viz::pages::trie::SearchTimeline::GetStatusMessage() const
{
    if (currentStep == stepTimeline.end())
        return "End of timeline";

    return "Searching for '" + _key +
           "'. " + (*currentStep)->GetMessage();
}

std::unique_ptr<ds_viz::pages::trie::SearchTimeline::MessageOnlyStep>
    ds_viz::pages::trie::SearchTimeline::CheckChildStep(char c)
{
    return std::make_unique<MessageOnlyStep>([this, c]() -> std::string {
        if (var_curNode == nullptr) {
            return "curNode is null";
        }

        std::stringstream msgSS;
        msgSS << "curNode has ";
        if (var_curNode->children.at(c) == nullptr)
            msgSS << "no child at index '" << c << "'";
        else 
            msgSS << "a child at index '" << c << "'";

        return msgSS.str();
    });
}
std::unique_ptr<ds_viz::pages::trie::SearchTimeline::MessageOnlyStep>
    ds_viz::pages::trie::SearchTimeline::CheckTerminalStep()
{
    return std::make_unique<MessageOnlyStep>([this]() -> std::string {
        if (var_curNode == nullptr)
            return "curNode is null";
        if (var_curNode->isTerminal)
            return "curNode is marked terminal (at end of word)";
        return "curNode is not marked terminal (at end of word)";
    });
}
