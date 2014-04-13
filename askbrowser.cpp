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

#include <iostream>

#include "askbrowser.h"

using namespace std;

AskBrowser::AskBrowser(AskSystem* const system)
{
    system_ = system;
}

void AskBrowser::Run()
{
    cout<<"======================================================"<<endl;
    cout<<"You're browsing the "<<system_->GetSystemTitle()
        <<" ASK system."<<endl;
    cout<<"======================================================"<<endl<<endl;
    bool quit = false;
    while(!quit) {
        cout<<endl<<system_->GetCurrentStoryTitle()<<endl<<endl;
        cout<<system_->GetCurrentStoryText()<<endl<<endl;
        AskSystem::QuestionList questionsList;
        system_->GetCurrentStoryQuestions(questionsList);
        unsigned qNum = 1;
        string currentQuestionCategory;
        for (AskSystem::Question q: questionsList) {
            if (q.second.compare(currentQuestionCategory) != 0) {
                cout<<q.second<<endl;
                currentQuestionCategory = q.second;
            }
            cout<<qNum<<". "<<q.first<<endl<<endl;
            qNum += 1;
        }
        cout<<qNum<<". "<<"Quit"<<endl<<endl;
        unsigned int choice = qNum;
        cout<<"Enter a number and hit [Enter]: ";
        cin>>choice;
        if (choice >= qNum) {
            quit = true;
        } else {
            system_->SelectQuestion(choice-1);
        }
        cout<<"----------------------------------------------------------"<<endl;
    }
}
