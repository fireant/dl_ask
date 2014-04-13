#include "SExprReader.hpp"
#include <assert.h>
#include <ctype.h>
#include <sstream>
#include <stdio.h>


// ----------
// SExprObject
// ----------

SExprObject::SExprObject ()
    : _type(TYPE_UNSPECIFIED), _string(""), _name(""), _number(0)
{
}

SExprObject::~SExprObject ()
{
    if (_type == TYPE_LIST)
    {
        clear_list();
    }
}

void SExprObject::clear_list()
{
    SExprObjectList::iterator it;
    for (it = _list.begin(); it != _list.end(); ++it)
    {
        delete *it;
    }
    _list.clear();
}

SExprObject::SExprObject (const SExprObject& other)
{
    _type = other._type;

    switch (_type)
    {
    case TYPE_NUMBER:
        _number = other._number;
        break;
    case TYPE_STRING:
        _string = other._string;
        break;
    case TYPE_SYMBOL:
        _name = other._name;
        break;
    case TYPE_LIST:
        {
            clear_list();
            SExprObjectList::const_iterator it;
            for (it = other._list.begin(); it != other._list.end(); ++it)
            {
                SExprObject* obj = new SExprObject();
                *obj = **it;
                _list.push_back(obj);
            }        
        }
        break;
    case TYPE_UNSPECIFIED:
        break;
    }
}

SExprObject::Type
SExprObject::get_type () const
{
    return _type;
}

const std::string& 
SExprObject::get_string () const
{
    return _string;
}

const std::string& 
SExprObject::get_symbol_name () const
{
    return _name;
}

int
SExprObject::get_number () const
{
    return _number;
}

const SExprObject::SExprObjectList&
SExprObject::get_list () const
{
    return _list;
}

void
SExprObject::set_string (const std::string& string)
{
    _type = TYPE_STRING;
    _string = string;
}

void
SExprObject::set_symbol_name (const std::string& name)
{
    _type = TYPE_SYMBOL;
    _name = name;
}

void
SExprObject::set_number (int number)
{
    _type = TYPE_NUMBER;
    _number = number;
}

void
SExprObject::set_list (const SExprObjectList& list)
{
    _type = TYPE_LIST;
    _list = list;
}

void
SExprObject::print (std::ostream& stream) const
{
    switch (_type)
    {
    case TYPE_NUMBER:
        stream << _number;
        break;
    case TYPE_SYMBOL:
        stream << _name;
        break;
    case TYPE_STRING:
        stream << "\"" << _string << "\"";
        break;
    case TYPE_LIST:
        stream << "(";
        for (SExprObjectList::const_iterator i = _list.begin(); i != _list.end(); i++)
        {
            const SExprObject& o = **i;
            if (i != _list.begin())
            {
                stream << " ";
            }
            o.print(stream);
        }
        stream << ")";
        break;
    case TYPE_UNSPECIFIED:
        break;
    }        
}

std::ostream&
operator<< (std::ostream& out, const SExprObject& obj)
{
    obj.print(out);
    return out;
}



// ----------
// SExprReader
// ----------

SExprReader::SExprReader (std::istream& input)
  : _input(input)
{
}

SExprReader::~SExprReader ()
{
}

void
SExprReader::read_list (SExprObject** objptr)
{
    log("--> read_list");

    SExprObject::SExprObjectList list;
    bool keep_reading = true;
    
    while (keep_reading)
    {
        if (peek_char() == ')')
        {
            keep_reading = false;
            read_char();
        }
        else
        {
            SExprObject* obj;
            read(&obj);
            
            if (obj == NULL)
            {
                keep_reading = false;
            }
            else
            {
                list.push_front(obj);
                //delete obj;
            }
        }
    }
    
    list.reverse();
    SExprObject *obj = new SExprObject();
    obj->set_list(list);
    *objptr = obj;

    log("<-- read_list");
}

void
SExprReader::read_number (SExprObject** objptr)
{
    log("--> read_number");

    int num;
    _input >> num;
    SExprObject* obj = new SExprObject();
    obj->set_number(num);
    *objptr = obj;

    log("<-- read_number");
}

void
SExprReader::read_string (SExprObject** objptr)
{
    log("--> read_string");

    bool in_escape = false;
    std::stringstream str;

    int c = read_char();
    while (!(c == '\"' && !in_escape))
    {
        str << (char) c;
        c = read_char();
    }
    
    SExprObject* obj = new SExprObject();
    obj->set_string(str.str());
    *objptr = obj;

    log("<-- read_string");
}

void
SExprReader::read_symbol (SExprObject** objptr)
{
    log("--> read_symbol");

    SExprObject* obj = new SExprObject();
    std::stringstream str;
    int c = peek_char();
    while (is_symbol_char(c))
    {
        str << (char) c;
        read_char();
        c = peek_char();
    }

    obj->set_symbol_name(str.str());
    *objptr = obj;

    log("<-- read_symbol");
}

void
SExprReader::eat_comment ()
{
    int c = peek_char();
    while (c != '\n')
    {
        read_char();
        c = peek_char();
    }
}


void
SExprReader::read (SExprObject** obj)
{
    log("--> read");

    read_int(obj);

    log("<-- read");
}

void
SExprReader::read_int (SExprObject** obj)
{
    assert( obj != NULL );
    int c = peek_char();
    *obj = NULL;
    if (!(_input.eof() || c == EOF))
    {
        if (c == '\"')
        {
            read_char();
            read_string(obj);
        }
        else if (c == '(')
        {
            read_char();
            read_list(obj);
        }
        else if (c == ')')
        {
            error("Parsing error: unmatched ')'");
        }
        else if (is_whitespace(c))
        {
            eat_whitespace();
            read_int(obj);
        }
        else if (is_numeric(c)) 
        {
            read_number(obj);
        }
        else if (is_symbol_char(c))
        {
            read_symbol(obj);
        }
        else if (c == ';')
        {
            eat_comment();
            read_int(obj);
        }
        else
        {
            std::cerr << (int) c << "\n";
            std::stringstream str;
            str << "Unexpected character '" << (char) c << "' in input.";
            error(str.str().c_str());
        }
    }
}

void
SExprReader::eat_whitespace ()
{
    log("eating whitespace");
    char c = peek_char();
    while (is_whitespace(c))
    {
        read_char();
        c = peek_char();
    }
}

bool
SExprReader::is_whitespace (char c) const
{
  return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

int
SExprReader::read_char ()
{
    char ch;
    _input.get(ch);
    return ch;
}

int
SExprReader::peek_char ()
{
    return _input.peek();
}

void
SExprReader::error (const char* msg)
{
    std::cerr << "ERROR: " << msg << std::endl;
}

bool
SExprReader::is_numeric (char ch) const
{
    return (isdigit(ch) != 0);
}

bool
SExprReader::is_symbol_char (char ch) const
{
    return isalnum(ch) || ch == '-' || ch == ':';
}


void
SExprReader::log (const char* msg) const
{
//    std::cerr << msg << "\n";
}
