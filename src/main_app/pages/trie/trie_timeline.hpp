#pragma once

#include "./trie_scene.hpp"
#include "raylib-cpp/raylib-cpp.hpp"

#include <functional>
#include <ios>
#include <list>
#include <memory>
#include <optional>
#include <sstream>
#include <string>

namespace ds_viz::pages::trie
{

struct TrieNode;
class TrieScene;

class ITimeline
{
  public:
    virtual std::string GetStatusMessage() const = 0;
    virtual void RenderCurrentState(TrieScene& scene) = 0;
    virtual void StepForward() = 0;
    virtual void StepBackward() = 0;
    virtual void ApplyTimeline(TrieScene& scene) = 0;
};

class SearchTimeline : public ITimeline
{
    class IAction
    {
      public:
        virtual ~IAction() = default;
        virtual std::string GetMessage() const = 0;
        virtual void Do(SearchTimeline& timeline) = 0;
        virtual void Undo(SearchTimeline& timeline) = 0;
    };

    template <typename T>
    class SetVariableStep : public IAction
    {
        T& _variable;
        T _previousValue;
        std::string _varName;
        std::function<T()> _newValueExpression;

      public:
        SetVariableStep(T& variable, std::string varName,
                        std::function<T()> newValueExpression) :
            _variable(variable), _newValueExpression(newValueExpression),
            _previousValue(variable), _varName(varName)
        {}

        std::string GetMessage() const override
        {
            return "Setting variable " + _varName + " to " +
                   std::to_string(_newValueExpression());
        }

        void Do(SearchTimeline& timeline) override
        {
            _previousValue = _variable;
            _variable = _newValueExpression();
        }

        void Undo(SearchTimeline& timeline) override
        {
            _variable = _previousValue;
        }
    };

    template <>
    class SetVariableStep<std::optional<bool>> : public IAction
    {
        std::optional<bool>& _variable;
        std::optional<bool> _previousValue;
        std::string _varName;
        std::function<std::optional<bool>()> _newValueExpression;

      public:
        SetVariableStep(
            std::optional<bool>& variable, std::string varName,
            std::function<std::optional<bool>()> newValueExpression) :
            _variable(variable), _newValueExpression(newValueExpression),
            _previousValue(variable), _varName(varName)
        {}

        std::string GetMessage() const override
        {
            auto result = _newValueExpression();
            std::stringstream msgSS;

            msgSS << "Setting variable " << _varName << " to ";
            if (result == std::nullopt)
                msgSS << "null";
            else
                msgSS << std::boolalpha << result.value();

            return msgSS.str();
        }

        void Do(SearchTimeline& timeline) override
        {
            _previousValue = _variable;
            _variable = _newValueExpression();
        }

        void Undo(SearchTimeline& timeline) override
        {
            _variable = _previousValue;
        }
    };

    template <>
    class SetVariableStep<TrieNode*> : public IAction
    {
        TrieNode*& _variable;
        TrieNode* _previousValue;
        std::string _varName;
        std::string _expName;
        std::function<TrieNode*()> _newValueExpression;

      public:
        SetVariableStep(TrieNode*& variable, std::string varName,
                        std::function<TrieNode*()> newValueExpression,
                        std::string expName) :
            _variable(variable), _newValueExpression(newValueExpression),
            _previousValue(variable), _varName(varName), _expName(expName)
        {}

        std::string GetMessage() const override
        {
            return "Setting variable " + _varName + " to " + _expName;
        }

        void Do(SearchTimeline& timeline) override
        {
            _previousValue = _variable;
            _variable = _newValueExpression();
        }

        void Undo(SearchTimeline& timeline) override
        {
            _variable = _previousValue;
        }
    };

    class GoToChildStep : public IAction
    {
        TrieNode*& _nodeVar;
        char childChar;
        std::string varName;
        TrieNode* _pPreviousNode;

      public:
        GoToChildStep(TrieNode*& nodeVar, std::string varName, char childChar) :
            _nodeVar(nodeVar), childChar(childChar), _pPreviousNode(nodeVar),
            varName(varName)
        {}

        std::string GetMessage() const override;

        void Do(SearchTimeline& timeline) override;

        void Undo(SearchTimeline& timeline) override;
    };

    class MessageOnlyStep : public IAction
    {
        std::function<std::string()> messageFunc;

      public:
        MessageOnlyStep(std::function<std::string()> messageFunc) :
            messageFunc(messageFunc)
        {}

        std::string GetMessage() const override
        {
            return messageFunc();
        }

        void Do(SearchTimeline& timeline) override {}

        void Undo(SearchTimeline& timeline) override {}
    };

  private:
    raylib::Color nodeDefaultColor = raylib::Color::White();
    raylib::Color nodeHighlightColor = raylib::Color::Yellow();
    raylib::Color edgeHighlightColor = raylib::Color::Yellow();
    raylib::Color nodeReturnColor = raylib::Color::Green();
    raylib::Color nodeUnavailColor = raylib::Color::Red();

    std::list<std::unique_ptr<IAction>> stepTimeline;
    std::list<std::unique_ptr<IAction>>::iterator currentStep;

    TrieScene& scene;

    std::string _key;
    TrieNode* var_curNode = nullptr;
    std::optional<bool> returnValue = std::nullopt;

    std::unique_ptr<MessageOnlyStep> CheckChildStep(char c);
    std::unique_ptr<MessageOnlyStep> CheckTerminalStep();

  public:
    SearchTimeline(TrieScene& scene, const std::string& key);

    std::string GetStatusMessage() const override;

    void RenderCurrentState(TrieScene& scene) override;

    void RenderUtil(TrieNode* node);

    void StepForward() override;

    void StepBackward() override;

    void ApplyTimeline(TrieScene& scene) override;
};

} // namespace ds_viz::pages::trie
