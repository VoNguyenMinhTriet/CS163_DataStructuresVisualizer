#pragma once

#include "raylib-cpp/raylib-cpp.hpp"

#include <functional>
#include <ios>
#include <iterator>
#include <list>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <concepts>
#include "./trie_scene.hpp"

namespace ds_viz::pages::trie
{

struct TrieNode;
class TrieScene;

class ITimeline
{
  public:
    virtual ~ITimeline() = default;
    virtual std::pair<int, int> Progress() const = 0;
    virtual std::string GetCaption() const = 0;
    virtual std::string GetStatusMessage() const = 0;
    virtual std::string GetCode() const = 0;
    virtual int GetCurrentLine() const = 0;
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
        virtual int GetHighlightedLine() const = 0;
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

    template <std::same_as<std::optional<bool>> T>
    class SetVariableStep<T> : public IAction
    {
        std::optional<bool>& _variable;
        std::optional<bool> _previousValue;
        std::string _varName;
        std::function<std::optional<bool>()> _newValueExpression;
        int _highlightedLine;

      public:
        SetVariableStep(
            std::optional<bool>& variable, std::string varName,
            std::function<std::optional<bool>()> newValueExpression, int line) :
            _variable(variable), _newValueExpression(newValueExpression),
            _previousValue(variable), _varName(varName), _highlightedLine(line)
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

        int GetHighlightedLine() const override
        {
            return _highlightedLine;
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

    template <std::same_as<TrieNode *> T>
    class SetVariableStep<T> : public IAction
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

        int GetHighlightedLine() const override
        {
            return 1;
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

        int GetHighlightedLine() const override
        {
            return 5;
        }

        void Do(SearchTimeline& timeline) override;

        void Undo(SearchTimeline& timeline) override;
    };

    class MessageOnlyStep : public IAction
    {
        std::function<std::string()> messageFunc;
        int highlightedLine;

      public:
        MessageOnlyStep(std::function<std::string()> messageFunc, int highlightedLine) :
            messageFunc(messageFunc), highlightedLine(highlightedLine)
        {}

        std::string GetMessage() const override
        {
            return messageFunc();
        }

        int GetHighlightedLine() const override
        {
            return highlightedLine;
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
    std::list<std::unique_ptr<IAction>>::const_iterator currentStep;

    TrieScene& scene;

    std::string _key;
    TrieNode* var_curNode = nullptr;
    std::optional<bool> returnValue = std::nullopt;

    std::unique_ptr<MessageOnlyStep> CheckChildStep(char c);
    std::unique_ptr<MessageOnlyStep> CheckTerminalStep();

  public:
    SearchTimeline(TrieScene& scene, const std::string& key);

    std::pair<int, int> Progress() const override
    {
        return {std::distance(stepTimeline.begin(), currentStep),
                stepTimeline.size()};
    }

    std::string GetCaption() const override
    {
        return "Searching for \"" + _key + "\" in the trie";
    }

    std::string GetCode() const override
    {
        return "curNode <- root\n"
               "for each char in key:\n"
               "  if curNode has no child at char:\n"
               "     return null\n"
               "  move curNode to child at char\n"
               "if curNode is terminal:\n"
               "  return curNode\n"
               "return null\n";
    }

    int GetCurrentLine() const override
    {
        if (currentStep == stepTimeline.end())
            return 0;
        return (*currentStep)->GetHighlightedLine();
    }

    std::string GetStatusMessage() const override;

    void RenderCurrentState(TrieScene& scene) override;

    void RenderUtil(TrieNode* node);

    void StepForward() override;

    void StepBackward() override;

    void ApplyTimeline(TrieScene& scene) override;
};

class AddTimeline : public ITimeline
{
    class IAction
    {
      public:
        virtual ~IAction() = default;
        virtual std::string GetMessage() const = 0;
        virtual int GetHighlightedLine() const = 0;
        virtual void Do(AddTimeline& timeline) = 0;
        virtual void Undo(AddTimeline& timeline) = 0;
    };

    class SetNodeVarToRootStep : public IAction
    {
        TrieNode*& _nodeVar;
        std::string _varName;
        TrieNode* _pPreviousNode;

      public:
        SetNodeVarToRootStep(TrieNode*& nodeVar, std::string varName);

        std::string GetMessage() const override;

        int GetHighlightedLine() const override
        {
            return 1;
        }

        void Do(AddTimeline& timeline) override;

        void Undo(AddTimeline& timeline) override;
    };

    class AddChildStep : public IAction
    {
        TrieNode*& _nodeVar;
        char childChar;
        std::string varName;
        TrieNode* _pPreviousNode;

      public:
        AddChildStep(TrieNode*& nodeVar, std::string varName, char childChar);

        int GetHighlightedLine() const override
        {
            return 4;
        }

        std::string GetMessage() const override;

        void Do(AddTimeline& timeline) override;

        void Undo(AddTimeline& timeline) override;
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

        int GetHighlightedLine() const override
        {
            return 5;
        }

        void Do(AddTimeline& timeline) override;

        void Undo(AddTimeline& timeline) override;
    };

    class MessageOnlyStep : public IAction
    {
        std::function<std::string()> messageFunc;
        int highlightedLine;

      public:
        MessageOnlyStep(std::function<std::string()> messageFunc, int highlightedLine) :
            messageFunc(messageFunc), highlightedLine(highlightedLine)
        {}

        std::string GetMessage() const override
        {
            return messageFunc();
        }

        int GetHighlightedLine() const override
        {
            return highlightedLine;
        }

        void Do(AddTimeline& timeline) override {}

        void Undo(AddTimeline& timeline) override {}
    };

    class MarkTerminalStep : public IAction
    {
        TrieNode*& _nodeVar;
        std::string varName;
        bool prevVal;

      public:
        MarkTerminalStep(TrieNode*& nodeVar, std::string varName);

        std::string GetMessage() const override;

        int GetHighlightedLine() const override
        {
            return 6;
        }

        void Do(AddTimeline& timeline) override;

        void Undo(AddTimeline& timeline) override;
    };

    TrieScene& scene;

    std::string _key;
    TrieNode* var_curNode = nullptr;

    std::list<std::unique_ptr<IAction>> stepTimeline;
    std::list<std::unique_ptr<IAction>>::const_iterator currentStep;

    raylib::Color nodeDefaultColor = raylib::Color::White();
    raylib::Color nodeHighlightColor = raylib::Color::Yellow();
    raylib::Color edgeHighlightColor = raylib::Color::Yellow();
    raylib::Color nodeReturnColor = raylib::Color::Green();
    raylib::Color nodeUnavailColor = raylib::Color::Red();

    void RenderUtil(TrieNode* node);

    std::unique_ptr<MessageOnlyStep> CheckChildStep(char c);

  public:
    AddTimeline(TrieScene& scene, const std::string& key);

    std::pair<int, int> Progress() const override
    {
        return {std::distance(stepTimeline.begin(), currentStep),
                stepTimeline.size()};
    }

    std::string GetCaption() const override
    {
        return "Adding \"" + _key + "\"";
    }

    std::string GetCode() const override
    {
        return "curNode <- root\n"
               "for each char in key:\n"
               "  if curNode has no child at char:\n"
               "    add child at char\n"
               "  move curNode to child at char\n"
               "mark curNode as terminal\n";
    }

    int GetCurrentLine() const override 
    {
        if (currentStep == stepTimeline.end())
            return 0;
        return (*currentStep)->GetHighlightedLine();
    }

    std::string GetStatusMessage() const override;

    void RenderCurrentState(TrieScene& scene) override;

    void StepForward() override;

    void StepBackward() override;

    void ApplyTimeline(TrieScene& scene) override;
};

class RemoveTimeline : public ITimeline
{
    class IAction
    {
      public:
        virtual ~IAction() = default;
        virtual std::string GetMessage() const = 0;
        virtual int GetHighlightedLine() const = 0;
        virtual void Do(RemoveTimeline& timeline) = 0;
        virtual void Undo(RemoveTimeline& timeline) = 0;
    };

    class MessageOnlyStep : public IAction
    {
        std::function<std::string()> messageFunc;
        int highlightedLine;

      public:
        MessageOnlyStep(std::function<std::string()> messageFunc, int highlightedLine) :
            messageFunc(messageFunc), highlightedLine(highlightedLine)
        {}

        std::string GetMessage() const override
        {
            return messageFunc();
        }

        int GetHighlightedLine() const override
        {
            return highlightedLine;
        }

        void Do(RemoveTimeline& timeline) override {}

        void Undo(RemoveTimeline& timeline) override {}
    };

    class SetNodeVarToRootStep : public IAction
    {
        TrieNode*& _nodeVar;
        std::string _varName;
        TrieNode* _pPreviousNode;

      public:
        SetNodeVarToRootStep(TrieNode*& nodeVar, std::string varName);

        std::string GetMessage() const override;

        int GetHighlightedLine() const override
        {
            return 1;
        }

        void Do(RemoveTimeline& timeline) override;

        void Undo(RemoveTimeline& timeline) override;
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

        int GetHighlightedLine() const override
        {
            return 5;
        }

        void Do(RemoveTimeline& timeline) override;

        void Undo(RemoveTimeline& timeline) override;
    };

    class UnmarkTerminalStep : public IAction
    {
        TrieNode*& _nodeVar;
        std::string varName;
        bool prevVal;

      public:
        UnmarkTerminalStep(TrieNode*& nodeVar, std::string varName);

        std::string GetMessage() const override;

        int GetHighlightedLine() const override
        {
            return 6;
        }

        void Do(RemoveTimeline& timeline) override;

        void Undo(RemoveTimeline& timeline) override;
    };

    TrieScene& scene;

    std::string _key;
    TrieNode* var_curNode = nullptr;

    std::list<std::unique_ptr<IAction>> stepTimeline;
    std::list<std::unique_ptr<IAction>>::const_iterator currentStep;

    raylib::Color nodeDefaultColor = raylib::Color::White();
    raylib::Color nodeHighlightColor = raylib::Color::Yellow();

    std::unique_ptr<MessageOnlyStep> CheckChildStep(char c);
    void RenderUtil(TrieNode* node);

  public:
    RemoveTimeline(TrieScene& scene, const std::string& key);

    std::pair<int, int> Progress() const override
    {
        return {std::distance(stepTimeline.begin(), currentStep),
                stepTimeline.size()};
    }

    std::string GetCaption() const override
    {
        return "Removing \"" + _key + "\" from the trie";
    }

    std::string GetCode() const override
    {
        return "curNode <- root\n"
               "for each char in key:\n"
               "  if curNode has no child at char:\n"
               "     return\n"
               "  move curNode to child at char\n"
               "unmark curNode as terminal\n";
    }

    int GetCurrentLine() const override
    {
        if (currentStep == stepTimeline.end())
            return 0;
        return (*currentStep)->GetHighlightedLine();
    }

    std::string GetStatusMessage() const override;

    void RenderCurrentState(TrieScene& scene) override;

    void StepForward() override;

    void StepBackward() override;

    void ApplyTimeline(TrieScene& scene) override;
};

} // namespace ds_viz::pages::trie
