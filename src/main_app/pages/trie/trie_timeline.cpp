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

    for (const char& c : key)
    {
        stepTimeline.push_back(CheckChildStep(c));
        if (curNode->children[c] == nullptr)
        {
            stepTimeline.push_back(
                std::make_unique<SetVariableStep<std::optional<bool>>>(
                    returnValue, "<return value>", [] { return false; }, 4
                ));

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
            [this] { return var_curNode->isTerminal; },
            curNode->isTerminal ? 7 : 8));

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
    for (int i = 0; i < TrieNode::ALPHABET_SIZE; ++i)
    {
        if (!node->children[i])
            continue;

        // Draw connector. To be refactored.
        node->GetPosition().DrawLine(node->children[i]->GetPosition(), 2.0f,
                                     nodeDefaultColor);
    }

    for (int i = 0; i < TrieNode::ALPHABET_SIZE; ++i)
    {
        if (!node->children[i])
            continue;

        RenderUtil(node->children[i].get());

        const auto txtCol =
            node->children.at(i).get() == var_curNode
                ? (returnValue.has_value()
                    ? (returnValue.value() ? nodeReturnColor : nodeUnavailColor)
                    : nodeHighlightColor)
                : nodeDefaultColor;
        auto txt = raylib::Text(
            std::string(1, i), 10,
            node->children[i]->isTerminal ? raylib::Color::Black() : txtCol);
        txt.Draw(node->children[i]->GetPosition() -
                 raylib::Vector2 {txt.Measure() * 0.5f, 5});
    }

    // Draw node. To be refactored.
    auto nodeCol =
        node == var_curNode
            ? (returnValue.has_value()
                   ? (returnValue.value() ? nodeReturnColor : nodeUnavailColor)
                   : nodeHighlightColor)
            : nodeDefaultColor;
    if (node->isTerminal)
        TrieScene::RenderNodeTerminal(node, nodeCol);
    else
        TrieScene::RenderNodeNormal(node, nodeCol);
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

    return (*currentStep)->GetMessage();
}

std::unique_ptr<ds_viz::pages::trie::SearchTimeline::MessageOnlyStep>
    ds_viz::pages::trie::SearchTimeline::CheckChildStep(char c)
{
    return std::make_unique<MessageOnlyStep>([this, c]() -> std::string {
        if (var_curNode == nullptr)
        {
            return "curNode is null";
        }

        std::stringstream msgSS;
        msgSS << "curNode has ";
        if (var_curNode->children.at(c) == nullptr)
            msgSS << "no child at index '" << c << "'";
        else
            msgSS << "a child at index '" << c << "'";

        return msgSS.str();
    }, 3);
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
    }, 6);
}

std::string ds_viz::pages::trie::AddTimeline::AddChildStep::GetMessage() const
{
    return "Adding child node at index '" + std::string(1, childChar) +
           "' of " + varName;
}

void ds_viz::pages::trie::AddTimeline::AddChildStep::Do(AddTimeline& timeline)
{
    _nodeVar->Add(childChar);
    timeline.scene.root->RecomputePosition();
}

void ds_viz::pages::trie::AddTimeline::AddChildStep::Undo(AddTimeline& timeline)
{
    auto discardedChildNodeOwner = std::move(
        _nodeVar
            ->children[childChar]); // Automatically deleted when out of scope
    _nodeVar->children[childChar] = nullptr;

    timeline.scene.root->RecomputePosition();
}

ds_viz::pages::trie::AddTimeline::AddChildStep::AddChildStep(
    TrieNode*& nodeVar, std::string varName, char childChar) :
    _nodeVar(nodeVar), childChar(childChar), _pPreviousNode(nodeVar),
    varName(varName)
{}

void ds_viz::pages::trie::AddTimeline::GoToChildStep::Do(AddTimeline& timeline)
{
    _pPreviousNode = _nodeVar;
    _nodeVar = _nodeVar->children[childChar].get();
}

void ds_viz::pages::trie::AddTimeline::GoToChildStep::Undo(
    AddTimeline& timeline)
{
    _nodeVar = _pPreviousNode;
}

std::string ds_viz::pages::trie::AddTimeline::GoToChildStep::GetMessage() const
{
    return "Moving variable " + varName + " to child node of index '" +
           std::string(1, childChar) + "'";
}

std::string ds_viz::pages::trie::AddTimeline::GetStatusMessage() const
{
    if (currentStep == stepTimeline.end())
        return "End of timeline";

    return (*currentStep)->GetMessage();
}
std::string ds_viz::pages::trie::AddTimeline::SetNodeVarToRootStep::GetMessage()
    const
{
    return "Setting variable " + _varName + " to root";
}
ds_viz::pages::trie::AddTimeline::SetNodeVarToRootStep::SetNodeVarToRootStep(
    TrieNode*& nodeVar, std::string varName) :
    _nodeVar(nodeVar), _pPreviousNode(nodeVar), _varName(varName)
{}
void ds_viz::pages::trie::AddTimeline::SetNodeVarToRootStep::Do(
    AddTimeline& timeline)
{
    _pPreviousNode = _nodeVar;
    _nodeVar = timeline.scene.root.get();
}
void ds_viz::pages::trie::AddTimeline::SetNodeVarToRootStep::Undo(
    AddTimeline& timeline)
{
    _nodeVar = _pPreviousNode;
}
ds_viz::pages::trie::AddTimeline::MarkTerminalStep::MarkTerminalStep(
    TrieNode*& nodeVar, std::string varName) :
    _nodeVar(nodeVar), varName(varName)
{}
std::string ds_viz::pages::trie::AddTimeline::MarkTerminalStep::GetMessage()
    const
{
    return "Marking " + varName + " as terminal";
}
void ds_viz::pages::trie::AddTimeline::MarkTerminalStep::Do(
    AddTimeline& timeline)
{
    _nodeVar->isTerminal = true;
}
void ds_viz::pages::trie::AddTimeline::MarkTerminalStep::Undo(
    AddTimeline& timeline)
{
    _nodeVar->isTerminal = prevVal;
}
ds_viz::pages::trie::AddTimeline::AddTimeline(TrieScene& scene,
                                              const std::string& key) :
    scene(scene), _key(key)
{
    stepTimeline.push_back(
        std::make_unique<SetNodeVarToRootStep>(var_curNode, "curNode"));

    auto curNode = scene.root.get();
    bool nulled = false;
    for (const char& c : key)
    {
        stepTimeline.push_back(CheckChildStep(c));

        if (curNode->children[c] == nullptr)
            nulled = true;
        if (nulled)
            stepTimeline.push_back(
                std::make_unique<AddChildStep>(var_curNode, "curNode", c));

        stepTimeline.push_back(
            std::make_unique<GoToChildStep>(var_curNode, "curNode", c));
        if (!nulled)
            curNode = curNode->children[c].get();
    }

    stepTimeline.push_back(
        std::make_unique<MarkTerminalStep>(var_curNode, "curNode"));

    currentStep = stepTimeline.begin();
}
std::unique_ptr<ds_viz::pages::trie::AddTimeline::MessageOnlyStep>
    ds_viz::pages::trie::AddTimeline::CheckChildStep(char c)
{
    return std::make_unique<MessageOnlyStep>([this, c]() -> std::string {
        if (var_curNode == nullptr)
        {
            return "curNode is null";
        }

        std::stringstream msgSS;
        msgSS << "curNode has ";
        if (var_curNode->children.at(c) == nullptr)
            msgSS << "no child at index '" << c << "'";
        else
            msgSS << "child at index '" << c << "'";

        return msgSS.str();
    }, 3);
}

void ds_viz::pages::trie::AddTimeline::StepForward()
{
    if (currentStep == stepTimeline.end())
        return;

    (*currentStep)->Do(*this);
    ++currentStep;
}

void ds_viz::pages::trie::AddTimeline::StepBackward()
{
    if (currentStep == stepTimeline.begin())
        return;

    --currentStep;
    (*currentStep)->Undo(*this);
}

void ds_viz::pages::trie::AddTimeline::RenderCurrentState(TrieScene& scene)
{
    if (!scene.root)
        return;
    RenderUtil(scene.root.get());
}

void ds_viz::pages::trie::AddTimeline::RenderUtil(TrieNode* node)
{
    for (int i = 0; i < TrieNode::ALPHABET_SIZE; ++i)
    {
        if (!node->children[i])
            continue;

        // Draw connector. To be refactored.
        node->GetPosition().DrawLine(node->children[i]->GetPosition(), 2.0f,
                                     nodeDefaultColor);
    }

    for (int i = 0; i < TrieNode::ALPHABET_SIZE; ++i)
    {
        if (!node->children[i])
            continue;

        RenderUtil(node->children[i].get());

        const auto txtCol =
            node->children.at(i).get() == var_curNode ? nodeHighlightColor : nodeDefaultColor;
        auto txt = raylib::Text(
            std::string(1, i), 10,
            node->children[i]->isTerminal ? raylib::Color::Black() : txtCol);
        txt.Draw(node->children[i]->GetPosition() -
                 raylib::Vector2 {txt.Measure() * 0.5f, 5});
    }

    const auto nodeCol =
        node == var_curNode ? nodeHighlightColor : nodeDefaultColor;
    if (node->isTerminal)
        TrieScene::RenderNodeTerminal(node, nodeCol);
    else
        TrieScene::RenderNodeNormal(node, nodeCol);
}

void ds_viz::pages::trie::RemoveTimeline::GoToChildStep::Do(
    RemoveTimeline& timeline)
{
    _pPreviousNode = _nodeVar;
    _nodeVar = _nodeVar->children[childChar].get();
}

void ds_viz::pages::trie::RemoveTimeline::GoToChildStep::Undo(
    RemoveTimeline& timeline)
{
    _nodeVar = _pPreviousNode;
}

std::string ds_viz::pages::trie::RemoveTimeline::GoToChildStep::GetMessage()
    const
{
    return "Moving variable " + varName + " to child node of index '" +
           std::string(1, childChar) + "'";
}

std::string ds_viz::pages::trie::RemoveTimeline::GetStatusMessage() const
{
    if (currentStep == stepTimeline.end())
        return "End of timeline";

    return (*currentStep)->GetMessage();
}
std::string
    ds_viz::pages::trie::RemoveTimeline::SetNodeVarToRootStep::GetMessage()
        const
{
    return "Setting variable " + _varName + " to root";
}
ds_viz::pages::trie::RemoveTimeline::SetNodeVarToRootStep::SetNodeVarToRootStep(
    TrieNode*& nodeVar, std::string varName) :
    _nodeVar(nodeVar), _pPreviousNode(nodeVar), _varName(varName)
{}
void ds_viz::pages::trie::RemoveTimeline::SetNodeVarToRootStep::Do(
    RemoveTimeline& timeline)
{
    _pPreviousNode = _nodeVar;
    _nodeVar = timeline.scene.root.get();
}
void ds_viz::pages::trie::RemoveTimeline::SetNodeVarToRootStep::Undo(
    RemoveTimeline& timeline)
{
    _nodeVar = _pPreviousNode;
}

ds_viz::pages::trie::RemoveTimeline::UnmarkTerminalStep::UnmarkTerminalStep(
    TrieNode*& nodeVar, std::string varName) :
    _nodeVar(nodeVar), varName(varName)
{}
std::string
    ds_viz::pages::trie::RemoveTimeline::UnmarkTerminalStep::GetMessage() const
{
    return "Removing terminal mark of " + varName;
}
void ds_viz::pages::trie::RemoveTimeline::UnmarkTerminalStep::Do(
    RemoveTimeline& timeline)
{
    prevVal = _nodeVar->isTerminal;
    _nodeVar->isTerminal = false;
}
void ds_viz::pages::trie::RemoveTimeline::UnmarkTerminalStep::Undo(
    RemoveTimeline& timeline)
{
    _nodeVar->isTerminal = prevVal;
}
ds_viz::pages::trie::RemoveTimeline::RemoveTimeline(TrieScene& scene,
                                                    const std::string& key) :
    scene(scene), _key(key)
{
    stepTimeline.push_back(
        std::make_unique<SetNodeVarToRootStep>(var_curNode, "curNode"));

    auto curNode = scene.root.get();
    bool done = false;
    for (const char& c : key)
    {
        stepTimeline.push_back(CheckChildStep(c));

        if (curNode->children[c] == nullptr)
        {
            done = true;
            break;
        }

        stepTimeline.push_back(
            std::make_unique<GoToChildStep>(var_curNode, "curNode", c));
        curNode = curNode->children[c].get();
    }

    if (!done)
        stepTimeline.push_back(
            std::make_unique<UnmarkTerminalStep>(var_curNode, "curNode"));

    currentStep = stepTimeline.begin();
}
std::unique_ptr<ds_viz::pages::trie::RemoveTimeline::MessageOnlyStep>
    ds_viz::pages::trie::RemoveTimeline::CheckChildStep(char c)
{
    return std::make_unique<MessageOnlyStep>([this, c]() -> std::string {
        if (var_curNode == nullptr)
        {
            return "curNode is null";
        }

        std::stringstream msgSS;
        msgSS << "curNode has ";
        if (var_curNode->children.at(c) == nullptr)
            msgSS << "no child at index '" << c << "'";
        else
            msgSS << "child at index '" << c << "'";

        return msgSS.str();
    }, 3);
}

void ds_viz::pages::trie::RemoveTimeline::StepForward()
{
    if (currentStep == stepTimeline.end())
        return;

    (*currentStep)->Do(*this);
    ++currentStep;
}

void ds_viz::pages::trie::RemoveTimeline::StepBackward()
{
    if (currentStep == stepTimeline.begin())
        return;

    --currentStep;
    (*currentStep)->Undo(*this);
}

void ds_viz::pages::trie::RemoveTimeline::RenderCurrentState(TrieScene& scene)
{
    if (!scene.root)
        return;
    RenderUtil(scene.root.get());
}

void ds_viz::pages::trie::RemoveTimeline::RenderUtil(TrieNode* node)
{
    for (int i = 0; i < TrieNode::ALPHABET_SIZE; ++i)
    {
        if (!node->children[i])
            continue;

        // Draw connector. To be refactored.
        node->GetPosition().DrawLine(node->children[i]->GetPosition(), 2.0f,
                                     nodeDefaultColor);
    }

    for (int i = 0; i < TrieNode::ALPHABET_SIZE; ++i)
    {
        if (!node->children[i])
            continue;

        RenderUtil(node->children[i].get());

        const auto txtCol =
            node->children.at(i).get() == var_curNode ? nodeHighlightColor : nodeDefaultColor;
        auto txt = raylib::Text(
            std::string(1, i), 10,
            node->children[i]->isTerminal ? raylib::Color::Black() : txtCol);
        txt.Draw(node->children[i]->GetPosition() -
                 raylib::Vector2 {txt.Measure() * 0.5f, 5});
    }

    // Draw node. To be refactored.
    auto nodeCol = node == var_curNode ? nodeHighlightColor : nodeDefaultColor;
    if (node->isTerminal)
        TrieScene::RenderNodeTerminal(node, nodeCol);
    else
        TrieScene::RenderNodeNormal(node, nodeCol);
}
void ds_viz::pages::trie::RemoveTimeline::ApplyTimeline(TrieScene& scene)
{
    while (currentStep != stepTimeline.end())
        StepForward();
};
void ds_viz::pages::trie::AddTimeline::ApplyTimeline(TrieScene& scene)
{
    while (currentStep != stepTimeline.end())
        StepForward();
};
