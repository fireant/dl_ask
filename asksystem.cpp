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

#include<fstream>

#include "asksystem.h"
#include "SExprReader.hpp"

using namespace std;

AskSystem::AskSystem(const string filename)
{
    LoadFromFile(filename);
    currentStory_ = 0;
}

bool AskSystem::LoadFromFile(const string filename)
{
    questions_.clear();
    stories_.clear();
    currentStory_ = 0;

    cout<<"filename "<<filename<<endl;
    ifstream in(filename.c_str());
    if (! in.is_open()) {
        return false;
    }
    SExprReader reader(in);
    SExprObject* obj;

    reader.read(&obj);
    while (obj != NULL) {
        SExprObject::SExprObjectList list = obj->get_list();
        string command = list.front()->get_symbol_name();
        list.pop_front();
        if (command.compare("def-ask-system") == 0) {
            SetSystemTitle(list);
        } else if (command.compare("def-cac-link") == 0) {
            AddCategory(list);
        } else if (command.compare("def-question") == 0) {
            AddQuestion(list);
        } else if (command.compare("def-story") == 0) {
            AddStory(list);
        }
        delete obj;
        reader.read(&obj);
    }

    return true;
}

const string& AskSystem::GetCurrentStoryTitle() const
{
    return stories_[currentStory_].first.first;
}

const string& AskSystem::GetCurrentStoryText() const
{
    return stories_[currentStory_].first.second;
}

void AskSystem::GetCurrentStoryQuestions(QuestionList& questions) const
{
    questions.clear();
    for (unsigned q : stories_[currentStory_].second) {
        if (q >= questions_.size()) {
            // oops the data file is incomplete
            // let's ignore this for now!
            // TODO: a proper method is needed to warn the user and
            //       blame the testers
            continue;
        }
        questions.push_back(make_pair(questions_[q].first,
                                      questions_[q].second));
    }
}

// 0-based index
bool AskSystem::SelectQuestion(const unsigned questionIdx)
{
    unsigned questionNum = stories_[currentStory_].second[questionIdx];
    currentStory_ = questionToStory_[questionNum];
    cout<<currentStory_<<endl;
    if (currentStory_ < 0) {
        // oops the data file is incomplete
        // let's ignore this for now!
        // TODO: a proper method is needed to warn the user and
        //       blame the testers
        currentStory_ = 0;
        return false;
    }

    return true;
}

const string AskSystem::GetSystemTitle() const
{
    return systemTitle_;
}

void AskSystem::SetSystemTitle(SExprObject::SExprObjectList& list)
{
    // drop the internal name
    list.pop_front();
    // next item is the name
    systemTitle_ = list.front()->get_string();
}

void AskSystem::AddStory(SExprObject::SExprObjectList& list)
{
    // drop internal name
    list.pop_front();

    SExprObject::Type type = list.front()->get_type();
    if (type == SExprObject::TYPE_NUMBER) {
        list.pop_front();
    }
    string title = list.front()->get_string();
    list.pop_front();
    vector<unsigned> questionsVec;
    SExprObject::SExprObjectList questionsList = list.front()->get_list();
    for (SExprObject::SExprObjectList::const_iterator i = questionsList.begin();
         i != questionsList.end(); i++) {
        questionsVec.push_back((*i)->get_number());
    }

    list.pop_front();

    SExprObject::SExprObjectList answersList = list.front()->get_list();
    for (SExprObject::SExprObjectList::const_iterator i = answersList.begin();
         i != answersList.end(); i++) {
        questionToStory_[(*i)->get_number()] = stories_.size();
    }

    list.pop_front();
    string text = list.front()->get_string();
    cout<<"t: "<<text<<endl;

    stories_.push_back(make_pair(make_pair(title, text), questionsVec));

}

void AskSystem::AddQuestion(SExprObject::SExprObjectList& list)
{
    // drop internal name
    list.pop_front();
    // drop question number
    list.pop_front();

    string category = list.front()->get_symbol_name();
    // drop internal category name
    list.pop_front();
    string question = list.front()->get_string();

    questions_.push_back(Question(question, categories_[category]));
    questionToStory_.push_back(-1);
}

void AskSystem::AddCategory(SExprObject::SExprObjectList& list)
{
    // drop internal name
    list.pop_front();
    string key = list.front()->get_symbol_name();
    // drop internal category name
    list.pop_front();
    string category = list.front()->get_string();

    categories_.emplace(key, category);
}
