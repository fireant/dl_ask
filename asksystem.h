/* Copyright (c) 2014 Mosalam Ebrahimi <m.ebrahimi@ieee.org>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.*/

#ifndef ASKSYSTEM_H
#define ASKSYSTEM_H

#include <string>
#include <vector>
#include <unordered_map>

#include "SExprReader.hpp"

/**
 * @brief This class provides functions needed to interact with stories
 *  in ASK systems.
 *
 */
class AskSystem
{
public:
    // question, category
    /**
     * @brief Type to represent each question.
     * first element holds the text of the question
     * second element holds the category of the question
     *
     */
    typedef std::pair<std::string, std::string> Question;
    /**
     * @brief Type to represent the list of questions raisd
     *  by the current story.
     *
     */
    typedef std::vector<Question> QuestionList;

    /**
     * @brief Initialize the object with the data from the sx file
     *
     * @param filename the address of the sx file
     */
    AskSystem(const std::string filename);

    /**
     * @brief Return the title of the loaded ASK system
     * (from command: def-ask-system)
     *
     * @return const std::string the title
     */
    const std::string GetSystemTitle() const;

    /**
     * @brief Reinitialize the object by loading a new ASK system
     *  from the given file
     *
     * @param filename the address of the sx file
     * @return bool whether the file exists and can be opened
     */
    bool LoadFromFile(const std::string filename);

    /**
     * @brief Return the title of the current story
     *
     * @return const std::string the title of the current story
     */
    const std::string& GetCurrentStoryTitle() const;
    /**
     * @brief Return the text of the current story
     *
     * @return const std::string the text of the current story
     */
    const std::string& GetCurrentStoryText() const;

    /**
     * @brief Return list of question raised by the current story
     *
     * @param questions List of questions in pairs <question, category>
     */
    void GetCurrentStoryQuestions(QuestionList& questions) const;

    /**
     * @brief Select one of the raised questions
     *
     * @param question the zero-based index of the question
     * @return bool whether the question exists in the system
     */
    bool SelectQuestion(const unsigned question);

private:
    /**
     * @brief Add a new story to the system
     *
     * @param list list of SExprObject's
     */
    void AddStory(SExprObject::SExprObjectList& list);
    /**
     * @brief Add a new question to the system
     *
     * @param list list of SExprObject's
     */
    void AddQuestion(SExprObject::SExprObjectList& list);
    /**
     * @brief Add a new question category
     *
     * @param list list of SExprObject's
     */
    void AddCategory(SExprObject::SExprObjectList& list);
    /**
     * @brief Set the title of the system
     *
     * @param list list of SExprObject's
     */
    void SetSystemTitle(SExprObject::SExprObjectList& list);

    // title, text, first question, second question
    /**
     * @brief Type to represent each story.
     * First element: pair of <Title, Text>
     * Second element: vector of indices of raised questions
     *
     */
    typedef std::pair<std::pair<std::string, std::string>,
            std::vector<unsigned> > Story;

    std::vector<Question> questions_; /**< the system questions */
    /**< corresponding story to each question */
    std::vector<unsigned> questionToStory_;

    std::vector<Story> stories_; /**< the system stories */
    /**< the system categories*/
    std::unordered_map<std::string, std::string> categories_;

    std::string systemTitle_; /**< the title of the loaded system */

    unsigned currentStory_; /**< the index of the current story */


};

#endif // ASKSYSTEM_H
