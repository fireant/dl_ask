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

#ifndef ASKBROWSER_H
#define ASKBROWSER_H

#include "asksystem.h"

/**
 * @brief This class implements the user interface with the ASK system
 *
 */
class AskBrowser
{
public:
    /**
     * @brief Initialize the browser with the given ASK system
     *
     * @param system pointer to an inialized AskSystem object
     */
    AskBrowser(AskSystem* const system);
    /**
     * @brief Runs the main loop, shows stories to user and ask questions
     *
     */
    void Run();

private:
    AskSystem* system_; /**< Initialized AskSystem object pointer */
};

#endif // ASKBROWSER_H
