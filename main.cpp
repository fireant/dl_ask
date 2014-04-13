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

#include "asksystem.h"
#include "askbrowser.h"

using namespace std;

int main(int argc, const char** argv)
{
    // ask user to give the filename of the data file
    if (argc < 2) {
        cout << "usage: ASK <ASK-system-file>" << endl;
        return 1;
    }

    // create a system object and initalize it with the data from the given file
    AskSystem system(argv[1]);
    // create a text-based browser and pass the pointer to the system object
    AskBrowser browser(&system);
    // enter the main loop and interact with the user until they decides to exit
    browser.Run();

    return 0;
}
